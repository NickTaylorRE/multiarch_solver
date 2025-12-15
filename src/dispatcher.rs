#![allow(warnings)]

use crate::stackvm::stackvm_disassembler;
use crate::regvm::regvm_disassembler;
use crate::MappedMasmSections;

use std::env;
use std::fs;


// each instruction encodes which of the 2 architectures it should be decoded by
fn get_pc_arch(pc: usize, arch_buf: &Vec<u8>) -> u8 {
    let mut pc_mut: i16 = pc.clone() as i16;
    pc_mut -= 0xff9;

    // make arch_byte negative because thats what the interpreter does
    if (pc as i16 - 0x1000 as i16) >= 0 {
        pc_mut = pc as i16 - 0x1000 as i16;
    }

    // get arch byte 0 or 1
    let byte = arch_buf[(pc_mut >> 3) as usize];
    let bit_pos = (pc & 7) as u8;
    let result = (byte >> bit_pos) & 1;
//    println!("[DEBUG] pc_mut:{:#X}, byte:{:#X}, bit_pos:{}, result:{}", pc_mut, byte, bit_pos, result);
    return result
}


// Instruction Parsing
pub struct StackvmInstruction {
    pub operator: u8,
    pub operand: u32,
    position: usize,
}

impl StackvmInstruction {
    pub fn set_position(&mut self, new_position: usize) {
        self.position = new_position;
    }
    pub fn current_position(&self) -> usize { 
        return self.position;
    }
}

pub struct RegvmInstructionReader<'text> {
    pub code: &'text [u8],
    pub position: usize,
}

impl<'text> RegvmInstructionReader<'text> {
    pub fn new(code: &'text [u8], start_pos: usize) -> Self { 
        Self {
            code,
            position: start_pos,
        }
    }
    pub fn read_byte(&mut self) -> u8 { 
        let byte = self.code[self.position];
        self.advance_byte();
        return byte;
    }  
    pub fn read_word(&mut self) -> u16 { 
        let word = u16::from_le_bytes(self.code[self.position..self.position+2].try_into().expect("failed to read word"));
        self.advance_word();
        return word;
    }
    pub fn read_pointer(&mut self) -> u32 { 
        let pointer = u32::from_le_bytes(self.code[self.position..self.position+4].try_into().expect("failed to read word"));
        self.advance_pointer();
        return pointer;
    }
    pub fn peek_byte(&mut self) -> u8 { 
        let byte = self.code[self.position];
        return byte;
    }  
    pub fn peek_word(&mut self) -> u16 { 
        let word = u16::from_le_bytes(self.code[self.position..self.position+2].try_into().expect("failed to read word"));
        return word;
    }
    pub fn peek_pointer(&mut self) -> u32 { 
        let pointer = u32::from_le_bytes(self.code[self.position..self.position+4].try_into().expect("failed to read word"));
        return pointer;
    }
    pub fn advance_byte(&mut self) {
        self.position += 1;
    }
    pub fn advance_word(&mut self) {
        self.position += 2;
    }
    pub fn advance_pointer(&mut self) {
        self.position += 4;
    }
    pub fn set_position(&mut self, new_position: usize) {
        self.position = new_position;
    }
    pub fn current_position(&self) -> usize { 
        return self.position;
    }
}

pub struct Context {
    pub stack: Vec<u32>,
    pub A: u32,
    pub B: u32,
    pub C: u32,
    pub D: u32,
}

impl Context {
    pub fn set_reg(&mut self, offset: u8, value: u32) {
        match offset {
            0x0 => self.A = value,
            0x4 => self.B = value,
            0x8 => self.C = value,
            0xc => self.D = value,
            _ => panic!("Invalid register offset: {:#x}", offset),
        }
    }
    pub fn get_reg(&self, offset: u8) -> u32 {
        match offset {
            0x0 => self.A,
            0x4 => self.B,
            0x8 => self.C,
            0xc => self.D,
            _ => panic!("Invalid register offset: {:#x}", offset),
        }
    }
    pub fn reg_name(&self, offset: u8) -> char {
        match offset {
            0x0 => 'A',
            0x4 => 'B',
            0x8 => 'C',
            0xc => 'D',
            _ => '?',
        }
    }
}

pub fn dispatcher(mapped_masm_sections: &MappedMasmSections) {
    let mut i:usize = 0;
    // some instructions require a stack value. so we maintain a stack here
    let mut context = Context {
        stack: Vec::new(),
        A: 0,
        B: 0,
        C: 0,
        D: 0,
    };
    //while mapped_masm_sections.text[i] != 0 {
    while i < 0x170 { // > 0x170 there is no more code for our challenge program
        // pc is always +0x1000 because it gets instantiated that way in the code
        let pc_arch: u8 = get_pc_arch(i+0x1000, &mapped_masm_sections.arch);
        if pc_arch == 0 {
            let mut instruction = StackvmInstruction {
                operator: mapped_masm_sections.text[i],
                operand: u32::from_le_bytes(mapped_masm_sections.text[i+1..i+5].try_into().expect("Failed to parse operand")),
                position: i,
            };
            stackvm_disassembler(&mut instruction, &mut context, &mapped_masm_sections.data);
            if instruction.current_position() == i {
                i += 5 // default increment by 5
            } else {
                i = instruction.current_position();
            }
//            println!("i:{:#X}", i);
        } else {
            let mut regvm_instruction_reader = RegvmInstructionReader::new(&mapped_masm_sections.text, i);
            regvm_disassembler(&mut regvm_instruction_reader, &mut context, &mapped_masm_sections.data);
            i = regvm_instruction_reader.current_position();
//            println!("i:{:#X}", i);
        }
    }
}

