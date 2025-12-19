
use crate::symbex_engine::SymVar;
use crate::symbex_engine::SymVarVec;

pub struct SymbolicContext {
    pub stack: SymVarVec,
    pub A: SymVarVec,
    pub B: SymVarVec,
    pub C: SymVarVec,
    pub D: SymVarVec,
    pub E: SymVarVec,
    pub flags: SymVarVec,
}

impl SymbolicContext {
    pub fn new() -> Self {
        SymbolicContext {
            stack: SymVarVec::new(),
            A: SymVarVec::new(),
            B: SymVarVec::new(),
            C: SymVarVec::new(),
            D: SymVarVec::new(),
            E: SymVarVec::new(),
            flags: SymVarVec::new(),
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

    // === STACK VM IMPLEMENTATIONS ===
    // S.PUSH
    pub fn push(&mut self, value: SymVarVec) {
        self.stack.push(value);
    }
    // S.POP
    pub fn pop(&mut self) -> Option<SymVarVec> {
        return self.stack.pop()
    }
    // S.ADDP
    pub fn addp(&mut self) {
        if let (Some(b), Some(a)) = (self.pop(), self.pop()){
            self.push(a.add(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.SUBP
    pub fn subp(&mut self) {
        if let (Some(b), Some(a)) = (self.pop(), self.pop()){
            self.push(a.sub(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.XORP
    pub fn xorp(&mut self) {
        if let (Some(b), Some(a)) = (self.pop(), self.pop()){
            self.push(a.bitxor(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.ANDP
    pub fn andp(&mut self) {
        if let (Some(b), Some(a)) = (self.pop(), self.pop()){
            self.push(a.bitand(b));
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }
    // S.CMPP
    pub fn cmpp(&mut self) {
        if let (Some(b), Some(a)) = (self.pop(), self.pop()){
            self.flags = SymVarVec::eq(a,b);
        } else {
            panic!("Symbex VM failed to pop parameters");
        }
    }


    // === REG VM IMPLEMENTATIONS ===
    // R.ADD_RR
    pub fn add_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.add(r2));
    }
    // R.ADD_IR
    pub fn add_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, SymVarVec::concrete_u32(imm));
    }
    // R.SUB_RR
    pub fn sub_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.sub(r2));
    }
    // R.SUB_IR
    pub fn sub_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.sub(SymVarVec::concrete_u32(imm)));
    }
    // R.XOR_RR
    pub fn xor_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.bitxor(r2));
    }
    // R.XOR_IR
    pub fn xor_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.bitxor(SymVarVec::concrete_u32(imm)));
    }
    // R.MUL_RR
    pub fn mul_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.set_reg(reg1, r1.mul(r2));
    }
    // R.MUL_RI
    pub fn mul_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.set_reg(reg, r.mul(SymVarVec::concrete_u32(imm)));
    }
    // R.CMP_RR
    pub fn cmp_rr(&mut self, reg1: u8, reg2: u8) {
        let r1 = self.get_reg(reg1).clone();
        let r2 = self.get_reg(reg2).clone();
        self.flags = SymVarVec::eq(r1,r2);
    }
    // R.CMP_RI
    pub fn cmp_ri(&mut self, reg: u8, imm: u32) {
        let r = self.get_reg(reg).clone();
        self.flags = SymVarVec::eq(r,SymVarVec::concrete_u32(imm));
    }
}
