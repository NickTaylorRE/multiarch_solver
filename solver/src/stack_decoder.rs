#![allow(warnings)]

use crate::dispatcher::StackvmInstruction;
use crate::dispatcher::Context;

fn read_string(data: &Vec<u8>, loc: &usize, size: usize) -> String {
    let mut string: Vec<u8> = Vec::new();
    let mut loc_clone = loc.clone();
    for i in 0..size.clone() {
        string.push(data[loc_clone].clone());
        loc_clone += 1;
    }
    return String::from_utf8(string).expect("failed to convert from vec to string")
}


pub fn stackvm_disassembler(instruction: &mut StackvmInstruction, context: &mut Context, data_section: &Vec<u8>) {
    let mut mnemonic: String;
    let mut argument: String;
    let mut pc_update: usize = 5;

    let instruction_location = instruction.current_position();

    match instruction.operator {
        0x10 => {
            mnemonic = "S.LDB".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(instruction.operand);
        }
        0x20 => {
            mnemonic = "S.LDW".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(instruction.operand);
        }
        0x30 => {
            mnemonic = "S.PUSHP".to_string();
/*            if instruction.operand < 0x9000 {
                argument = read_string(
                             data_section,
                             &(instruction.operand as usize - 0x2000), // stack starts at 0x2000. 
                                                                   // data_section vec starts at 0.
                             (context.stack.last().expect("stack is empty").clone() as usize)
                           );
            } else {
                argument = instruction.operand.to_string();
            }*/
            argument = instruction.operand.to_string();
            context.stack.push(instruction.operand);
        }
        0x40 => {
            mnemonic = "S.LDP".to_string();
            argument = "".to_string();
        }
        0x50 => {
            mnemonic = "S.POPP".to_string();
            argument = "".to_string();
        }
        0x60 => {
            mnemonic = "S.ADDP".to_string();
            argument = "".to_string();
        }
        0x61 => {
            mnemonic = "S.SUBP".to_string();
            argument = "".to_string();
        }
        0x62 => {
            mnemonic = "S.XORP".to_string();
            argument = "".to_string();
        }
        0x63 => {
            mnemonic = "S.ANDP".to_string();
            argument = "".to_string();
        }
        0x70 => {
            mnemonic = "S.JMP".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
        }
        0x71 => {
            mnemonic = "S.JE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
        }
        0x72 => {
            mnemonic = "S.JNE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            //instruction.set_position(instruction.operand as usize - 0x1000);
        }
        0x80 => {
            mnemonic = "S.CMPP".to_string();
            argument = "".to_string();
        }
        0xa0 => {
            mnemonic = "S.SYSCALL".to_string();
            let syscall = context.stack.pop().expect("failed to pop syscall from stack in S.SYSCALL");
            match syscall {
                0 => argument = format!("fscanf"),
                1 => argument = "unsupported syscall".to_string(),
                2 => {
                    let param1_ptr = context.stack.pop().expect("failed to pop from stack in S.SYSCALL");
                    let param2_len = context.stack.pop().expect("failed to pop from stack in S.SYSCALL");
                    let mut string = "".to_string();
                    if param1_ptr >= 0x2000 {
                        string = read_string(data_section, 
                            &(param1_ptr as usize - 0x2000),
                            (param2_len as usize)
                        );
                    }
                    argument = format!("fputs, {}", string);
                },
                3 => {
                    let param1_seed = context.stack.pop().expect("failed to pop from stack in S.SYSCALL");
                    argument = format!("srand, {}", param1_seed);
                },
                4 => {
                    argument = "rand".to_string();
                    context.stack.push(0xffffffff); // rand
                },
                5 => argument = "flag_success".to_string(),
                6 => argument = "split_register".to_string(),
                _ => argument = "UNKNOWN".to_string(),
            }
        }
        0xff => {
            mnemonic = "S.HLT".to_string();
            argument = "".to_string();
        }
        _ => {
            mnemonic = "S.INVALID".to_string();
            argument = "".to_string();
            instruction.set_position(instruction.current_position() + 1);
        }
    }

    println!("{:#X}: {}({:#X}), {}({:#X})", instruction_location, mnemonic, instruction.operator, argument, instruction.operand);
}
