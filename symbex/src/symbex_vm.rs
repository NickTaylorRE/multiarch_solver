#![allow(warnings)]

use crate::symbex_engine::SymVar;
use crate::symbex_engine::SymVarVec;

use z3::ast::{Ast, BV};
use z3::{Context, Solver, SatResult, Config};
use std::collections::HashMap;


pub struct SymbolicContext {
    pub stack: SymVarVec,
    pub A: SymVarVec,
    pub B: SymVarVec,
    pub C: SymVarVec,
    pub D: SymVarVec,
    pub E: SymVarVec,
    pub flags: SymVar,
    pub sp: usize,
    pub srand_seed: Option<SymVarVec>,
    pub rand_counter: u32,
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
            srand_seed: None,
            rand_counter: 0
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
        if seed.try_concrete_u32().is_some() {
            let seed_new = seed.try_concrete_u32().expect("failed to try_concrete seed");
            unsafe{
                libc::srand(seed_new);
            }
        } else {
            // Symbolic - just stash the seed, skip LCG computation
            self.srand_seed = Some(seed);
        }
    }

    pub fn rand_internal(&mut self) -> SymVarVec {
        if self.srand_seed.is_some() {
            let result = SymVarVec::rand(self.srand_seed.clone().expect("could not extract seed symvar"), self.rand_counter);
            self.rand_counter += 1;
            return result
        } else {
            let mut ret: u32 = 0;
            unsafe{
                ret = libc::rand() as u32;
            }
            return SymVarVec::concrete_u32(ret)
        }
    }

    pub fn rand(&mut self) -> SymVarVec {
        let rand1 = self.rand_internal();
        let rand2 = self.rand_internal();
        let rand3 = (rand1.bitandp(SymVarVec::concrete_u32(0xffff)).bitorp((rand2.shlp(SymVarVec::concrete_u32(0x10)))));
        return rand3
    }
}