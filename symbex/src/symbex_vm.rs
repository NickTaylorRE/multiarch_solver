#![allow(warnings)]

use crate::symbex_engine::SymVar;
use crate::symbex_engine::SymVarVec;



pub struct SymbolicContext {
    pub stack: SymVarVec,
    pub A: SymVarVec,
    pub B: SymVarVec,
    pub C: SymVarVec,
    pub D: SymVarVec,
    pub E: SymVarVec,
    pub flags: SymVar,
    pub sp: usize,
    state: [SymVarVec; 31],
    fptr: usize,
    rptr: usize,
}

impl SymbolicContext {
    pub fn new() -> Self {
        SymbolicContext {
            stack: SymVarVec::new(0x1000),
            A: SymVarVec::new(4),
            B: SymVarVec::new(4),
            C: SymVarVec::new(4),
            D: SymVarVec::new(4),
            E: SymVarVec::new(4),
            flags: SymVar::concrete(0),
            sp: 0xfff,
            state: std::array::from_fn(|_| SymVarVec::new(4)),
            fptr: 3,
            rptr: 0,
        }
    }
    pub fn set_reg(&mut self, offset: u8, value: SymVarVec) {
        match offset {
            0x0 => self.A = value,
            0x4 => self.B = value,
            0x8 => self.C = value,
            0xc => self.D = value,
            0x10 => self.E = value,
            _ => panic!("Invalid register offset: {:#x}", offset),
        }
    }
    pub fn get_reg(&self, offset: u8) -> SymVarVec {
        match offset {
            0x0 => self.A.clone(),
            0x4 => self.B.clone(),
            0x8 => self.C.clone(),
            0xc => self.D.clone(),
            0x10 => self.E.clone(),
            _ => panic!("Invalid register offset: {:#x}", offset),
        }
    }
    pub fn reg_name(&self, offset: u8) -> char {
        match offset {
            0x0 => 'A',
            0x4 => 'B',
            0x8 => 'C',
            0xc => 'D',
            0x10 => 'E',
            _ => '?',
        }
    }
    pub fn reg_value(&self, offset: char) -> u8 {
        match offset {
            'A' => 0x0,
            'B' => 0x4,
            'C' => 0x8,
            'D' => 0xc,
            'E' => 0x10,
            _ => panic!("Invalid reg name"),
        }
    }

    // === STACK VM IMPLEMENTATIONS ===
    // S.PUSH
    pub fn pushp(&mut self, value: SymVarVec) {
        if value.len() > 4 {
            panic!("length of SymVarVec too long for pushp")
        }
        self.stack.assign(self.sp,&value);
        self.sp -= 4;
    }
    pub fn pushw(&mut self, value: SymVarVec) {
        if value.len() > 2 {
            panic!("length of SymVarVec too long for pushp")
        }
        self.stack.assign(self.sp,&value);
        self.sp -= 2;
    }
    pub fn pushb(&mut self, value: SymVarVec) {
        if value.len() > 1 {
            panic!("length of SymVarVec too long for pushp")
        }
        self.stack.assign(self.sp,&value);
        self.sp -= 1;
    }
    // S.POP
    pub fn popp(&mut self) -> Option<SymVarVec> {
        let value = self.stack.get(self.sp,4);
        self.sp += 4;
        return Some(value)
    }
    pub fn popw(&mut self) -> Option<SymVarVec> {
        let value = self.stack.get(self.sp, 2);
        self.sp += 2;
        return Some(value)
    }
    pub fn popb(&mut self) -> Option<SymVarVec> {
        let value = self.stack.get(self.sp, 1);
        self.sp += 1;
        return Some(value)
    }
    pub fn getp(&self, offset: usize) -> SymVarVec {
        self.stack.get(offset, 4)
    }
    // S.ADDP
    pub fn addp(&mut self) {
        if let (Some(b), Some(a)) = (self.popp(), self.popp()){
            self.pushp(a.addp(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.SUBP
    pub fn subp(&mut self) {
        if let (Some(b), Some(a)) = (self.popp(), self.popp()){
            self.pushp(a.subp(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.XORP
    pub fn xorp(&mut self) {
        if let (Some(b), Some(a)) = (self.popp(), self.popp()){
            self.pushp(a.bitxorp(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.ANDP
    pub fn andp(&mut self) {
        if let (Some(b), Some(a)) = (self.popp(), self.popp()){
            self.pushp(a.bitandp(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.CMPP
    pub fn cmpp(&mut self) {
        if let (Some(b), Some(a)) = (self.popp(), self.popp()){
            self.flags = SymVarVec::eqp(a,b);
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }


    // === REG VM IMPLEMENTATIONS ===
    // R.ADD_RR
    pub fn add_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.addp(r2));
    }
    // R.ADD_IR
    pub fn add_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.addp(SymVarVec::concrete_u32(imm)));
    }
    // R.SUB_RR
    pub fn sub_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.subp(r2));
    }
    // R.SUB_IR
    pub fn sub_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.subp(SymVarVec::concrete_u32(imm)));
    }
    // R.XOR_RR
    pub fn xor_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.bitxorp(r2));
    }
    // R.XOR_IR
    pub fn xor_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.bitxorp(SymVarVec::concrete_u32(imm)));
    }
    // R.MUL_RR
    pub fn mul_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        let (high,low) = r1.clone().mulp(r2.clone());
        self.set_reg(self.reg_value('A'), low.clone());
        self.set_reg(self.reg_value('D'), high.clone());
    }
    // R.MUL_RI
    pub fn mul_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        let (high,low) = r.clone().mulp(SymVarVec::concrete_u32(imm));
        self.set_reg(self.reg_value('A'), low.clone());
        self.set_reg(self.reg_value('D'), high.clone());
    }
    // R.CMP_RR
    pub fn cmp_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.flags = SymVarVec::eqp(r1,r2);
    }
    // R.CMP_RI
    pub fn cmp_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.flags = SymVarVec::eqp(r,SymVarVec::concrete_u32(imm));
    }
}

impl SymbolicContext {
    pub fn srand(&mut self, seed: SymVarVec) {
        // If seed is 0, use 1
        // For symbolic execution, you might want to add a constraint instead
        let mut state: [SymVarVec; 31] = std::array::from_fn(|_| SymVarVec::new(4));
        state[0] = seed.clone();

        // Use the special LCG from glibc
        let mut word = seed;
        for i in 1..31 {
            println!("lcg: {}", i);
            // hi = word / 127773
            // lo = word % 127773
            // word = 16807 * lo - 2836 * hi
            // if word < 0: word += 2147483647

            let hi = word.clone().divp(SymVarVec::concrete_u32(127773));
            let lo = word.clone().modp(SymVarVec::concrete_u32(127773));

            let term1 = lo.mulp_low(SymVarVec::concrete_u32(16807));
            let term2 = hi.mulp_low(SymVarVec::concrete_u32(2836));

            word = term1.subp(term2);

            // Handle negative: if word < 0, word += 2147483647
            // For symbolic execution, you may need to handle this conditionally
            // For now, we'll do modulo to keep it in range
            word = word.modp(SymVarVec::concrete_u32(2147483647));

            state[i] = word.clone();
        }

        self.state = state;
        self.fptr = 3;
        self.rptr = 0;

        // Warmup: 31 * 10 = 310 iterations
        for i in 0..310 {
            println!("warmup: {}", i);
            self.rand_primitive();
        }
    }

    pub fn rand_primitive(&mut self) -> SymVarVec {
        // val = *fptr += (uint32_t) *rptr
        let val = self.state[self.fptr].clone().addp(self.state[self.rptr].clone());
        self.state[self.fptr] = val.clone();

        // result = val >> 1
        let result = val.shrp(SymVarVec::concrete_u32(1));

        // Advance pointers
        self.fptr += 1;
        if self.fptr >= 31 {
            self.fptr = 0;
            self.rptr += 1;
        } else {
            self.rptr += 1;
            if self.rptr >= 31 {
                self.rptr = 0;
            }
        }

        result
    }

    pub fn rand(&mut self) -> SymVarVec {
        let low = self.rand_primitive().clone();
        let high = self.rand_primitive().clone();
        return high.clone().shlp(SymVarVec::concrete_u32(0x10))
                 .bitorp(
                   low.clone().bitandp(SymVarVec::concrete_u32(0xffff)))
    }
}

#[test]
pub fn test_rand() {
    unsafe {
        libc::srand(1);
    }

    let mut ctx = SymbolicContext::new();
    ctx.srand(SymVarVec::concrete_u32(1));

    for i in 0..5 {
        let mut libc_val: i32 = 0;
        unsafe {
            libc_val = libc::rand();
        }
        let our_val = ctx.rand();

        println!("Round {}: libc={}, ours={}", i, libc_val, our_val.try_concrete_u32().expect("fs"));
    }
}