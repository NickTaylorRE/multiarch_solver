#![allow(warnings)]

use crate::stack_decoder::stackvm_disassembler;
use crate::reg_decoder::regvm_disassembler;
use symbex::symbex_vm::SymbolicContext;
use symbex::symbex_engine::SymVar;
use symbex::symbex_engine::SymVarVec;
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

pub fn dispatcher(mapped_masm_sections: &MappedMasmSections, emu: bool) {
    let mut i:usize = 0;
    // some instructions require a stack value. so we maintain a stack here
    let mut context = SymbolicContext::new();
    //while mapped_masm_sections.text[i] != 0 {
    let mut loop_counter = 0;
    while i < 0x164 { // > 0x164 there is no more code for our challenge progra
        if emu {
            // must be a multiple of 4
            let chal2_loop = 0x4;
            // this match statement gives us control to do things in the middle of exection.
            // like breakpoints
            match i {
                // we are at a conditional jmp so we solve
                0x55 => {
                    if let Some(val) = context.flags.try_solve_u32() {
                        println!("Solution 1 found: {}({:#X})", val, val);
                    }
                },
                // at this point we are in are in a point to choose the length
                // way to solve here is to test different loop lengths manually until one solves
                /*0x77 => {
                    // manually set reg C which determines the string length
                    context.set_reg(0x8, SymVarVec::concrete_u32(chal2_loop));
                }
                0x83 => {
                    // manually set reg B which determines the loop length.
                    context.set_reg(0x4, SymVarVec::concrete_u32(chal2_loop));
                },*/
                // this is the conditional jump for the second challenge after the hashing loop
                0x8D => {
                    if let Some(val) = context.flags.try_solve_u32() {
                        println!("Solution 2 found: {}({:#X})", val, val);
                    } 
                },
/*                0x137 => {
                    if loop_counter == 1 {
                        return
                    }
                    loop_counter += 1;
                }*/
                _ => {}
            }
        }

        // pc is always +0x1000 because it gets instantiated that way in the code
        let pc_arch: u8 = get_pc_arch(i+0x1000, &mapped_masm_sections.arch);
        if pc_arch == 0 {
            let mut instruction = StackvmInstruction {
                operator: mapped_masm_sections.text[i],
                operand: u32::from_le_bytes(mapped_masm_sections.text[i+1..i+5].try_into().expect("Failed to parse operand")),
                position: i,
            };
            stackvm_disassembler(&mut instruction, &mut context, &mapped_masm_sections.data, &emu);
            if instruction.current_position() == i {
                i += 5 // default increment by 5
            } else {
                i = instruction.current_position();
            }
        } else {
            let mut regvm_instruction_reader = RegvmInstructionReader::new(&mapped_masm_sections.text, i);
            regvm_disassembler(&mut regvm_instruction_reader, &mut context, &mapped_masm_sections.data, &emu);
            i = regvm_instruction_reader.current_position();
        }
        if emu {
            println!("[DEBUG] \nA:{} \nB:{} \nC:{} \nD:{} \nE:{} \nflags:{} \nstack:{:?} \nsp:{:#X}\n", context.A, context.B, context.C, context.D, context.E, context.flags, &context.stack[(context.stack.len()-(0x1000 - context.sp))..], context.sp);
        }
    }
}

