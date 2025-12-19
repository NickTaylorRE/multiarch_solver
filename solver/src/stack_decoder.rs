#![allow(warnings)]

use crate::dispatcher::StackvmInstruction;
use crate::symbex_vm::SymbolicContext;
use crate::symbex_engine::SymVarVec;

fn read_string(data: &Vec<u8>, loc: &usize, size: usize) -> String {
    let mut string: Vec<u8> = Vec::new();
    let mut loc_clone = loc.clone();
    for i in 0..size.clone() {
        string.push(data[loc_clone].clone());
        loc_clone += 1;
    }
    return String::from_utf8(string).expect("failed to convert from vec to string")
}


pub fn stackvm_disassembler(instruction: &mut StackvmInstruction, context: &mut SymbolicContext, data_section: &Vec<u8>) {
    let mut mnemonic: String;
    let mut argument: String;
    let mut pc_update: usize = 5;

    let instruction_location = instruction.current_position();

    match instruction.operator {
        0x10 => {
            mnemonic = "S.LDB".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(SymVarVec::concrete_u32(instruction.operand));
        }
        0x20 => {
            mnemonic = "S.LDW".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(SymVarVec::concrete_u32(instruction.operand));
        }
        0x30 => {
            mnemonic = "S.PUSHP".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(SymVarVec::concrete_u32(instruction.operand));
        }
        0x40 => {
            mnemonic = "S.LDP".to_string();
            argument = instruction.operand.to_string();
            context.stack.push(SymVarVec::concrete_u32(instruction.operand));
        }
        0x50 => {
            mnemonic = "S.POPP".to_string();
            argument = "".to_string();
            let junk = context.stack.popp().expect("failed to pop from stack in S.POPP");
        }
        0x60 => {
            mnemonic = "S.ADDP".to_string();
            argument = "".to_string();
            context.addp();
        }
        0x61 => {
            mnemonic = "S.SUBP".to_string();
            let arg1 = &context.stack[context.stack.len()-1];
            let arg2 = &context.stack[context.stack.len()-2];
            argument = format!("{}, {}", arg1, arg2);
            context.subp();
        }
        0x62 => {
            mnemonic = "S.XORP".to_string();
            let arg1 = &context.stack[context.stack.len()-1];
            let arg2 = &context.stack[context.stack.len()-2];
            argument = format!("{}, {}", arg1, arg2);
            context.xorp();
        }
        0x63 => {
            mnemonic = "S.ANDP".to_string();
            let arg1 = &context.stack[context.stack.len()-1];
            let arg2 = &context.stack[context.stack.len()-2];
            argument = format!("{}, {}", arg1, arg2);
            context.andp();
        }
        0x70 => {
            mnemonic = "S.JMP".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            // TODO: the VM will require us to follow this jump. the disassembler does not.
        }
        0x71 => {
            mnemonic = "S.JE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            // TODO: at this point we should try to find a solution which satisfies this constraint
        }
        0x72 => {
            mnemonic = "S.JNE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            // TODO: at this point we should try to find a solution which satisfies this constraint
            match context.flags.try_solve() {
                Some(solution) => println!("solution: {}", solution),
                None => println!("failed to solve")
            }
        }
        0x80 => {
            mnemonic = "S.CMPP".to_string();
            let arg1 = &context.stack[context.stack.len()-1];
            let arg2 = &context.stack[context.stack.len()-2];
            argument = format!("{}, {}", arg1, arg2);
            context.cmpp();
        }
        0xa0 => {
            mnemonic = "S.SYSCALL".to_string();
            let syscall = context.stack.pop().expect("failed to pop syscall from stack in S.SYSCALL")
                            .try_concrete().expect("failed to try_concrete for syscall");
            match syscall {
                0 => {
                    // this is the only place that we actually create a symbolic variable
                    context.stack.push(SymVar::symbolic("input".to_string()));
                    argument = format!("fscanf");
                },
                1 => argument = "unsupported syscall".to_string(),
                2 => {
                    let param1_ptr = context.stack.pop().expect("failed to pop from stack in S.SYSCALL")
                                        .try_concrete().expect("failed to try_concrete for param1 in syscall");
                    let param2_len = context.stack.pop().expect("failed to pop from stack in S.SYSCALL")
                                        .try_concrete().expect("failed to try_concrete for param1 in syscall");
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
                    let param1_seed = context.stack.pop().expect("failed to pop from stack in S.SYSCALL")
                                        .try_concrete().expect("failed to try_concrete for param1_seed");
                    // TODO: should we ACTUALLY seed rand and call rand? 
                    argument = format!("srand, {}", param1_seed);
                },
                4 => {
                    argument = "rand".to_string();
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
