#![allow(warnings)]

use crate::dispatcher::StackvmInstruction;
use symbex::symbex_vm::SymbolicContext;
use symbex::symbex_engine::SymVarVec;

fn read_string(data: &Vec<u8>, loc: &usize, size: usize) -> String {
    let mut string: Vec<u8> = Vec::new();
    let mut loc_clone = loc.clone();
    for i in 0..size.clone() {
        string.push(data[loc_clone].clone());
        loc_clone += 1;
    }
    return String::from_utf8(string).expect("failed to convert from vec to string")
}


pub fn stackvm_disassembler(instruction: &mut StackvmInstruction, context: &mut SymbolicContext, data_section: &Vec<u8>, emu: &bool) {
    let mut mnemonic: String;
    let mut argument: String;
    let mut pc_update: usize = 5;

    let instruction_location = instruction.current_position();

    match instruction.operator {
        0x10 => {
            mnemonic = "S.LDB".to_string();
            argument = instruction.operand.to_string();
            emu.then(|| context.pushb(SymVarVec::concrete_u8(instruction.operand as u8)));
        }
        0x20 => {
            mnemonic = "S.LDW".to_string();
            argument = instruction.operand.to_string();
            emu.then(|| context.pushw(SymVarVec::concrete_u16(instruction.operand as u16)));
        }
        0x30 => {
            mnemonic = "S.PUSHP".to_string();
            argument = instruction.operand.to_string();
            emu.then(|| context.pushp(SymVarVec::concrete_u32(instruction.operand)));
        }
        0x40 => {
            mnemonic = "S.LDP".to_string();
            argument = instruction.operand.to_string();
            emu.then(|| context.pushp(SymVarVec::concrete_u32(instruction.operand)));
        }
        0x50 => {
            mnemonic = "S.POPP".to_string();
            argument = "".to_string();
            emu.then(|| context.popp().expect("failed to pop from stack in S.POPP"));
        }
        0x60 => {
            mnemonic = "S.ADDP".to_string();
            argument = "".to_string();
            emu.then(|| context.addp());
        }
        0x61 => {
            mnemonic = "S.SUBP".to_string();
            argument = "".to_string();
            emu.then(|| context.subp());
        }
        0x62 => {
            mnemonic = "S.XORP".to_string();
            argument = "".to_string();
            emu.then(|| context.xorp());
        }
        0x63 => {
            mnemonic = "S.ANDP".to_string();
            argument = "".to_string();
            emu.then(|| context.andp());
        }
        0x70 => {
            mnemonic = "S.JMP".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            // TODO: the VM will require us to follow this jump. the disassembler does not.
        }
        0x71 => {
            mnemonic = "S.JE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            if *emu {
                
            }
        }
        0x72 => {
            mnemonic = "S.JNE".to_string();
            argument = format!("{:#X}",(instruction.operand as usize - 0x1000));
            // we would solve here if the crackme required it.
        }
        0x80 => {
            mnemonic = "S.CMPP".to_string();
            argument = "".to_string();
            emu.then(|| context.cmpp());
        }
        0xa0 => {
            mnemonic = "S.SYSCALL".to_string();
            argument = "".to_string();
            if *emu {
                let syscall = context.popb().expect("failed to pop syscall from stack in S.SYSCALL")
                                .try_concrete_u8().expect("failed to try_concrete for syscall");
                match syscall {
                    0 => {
                        // this is the only place that we actually create a symbolic variable
                        context.pushp(SymVarVec::symbolic_u32("input".to_string()));
                        argument = format!("fscanf");
                    },
                    1 => argument = "unsupported syscall".to_string(),
                    2 => {
                        let param1_ptr = context.popp().expect("failed to pop from stack in S.SYSCALL")
                                            .try_concrete_u32().expect("failed to try_concrete for param1 in syscall");
                        let param2_len = context.popb().expect("failed to pop from stack in S.SYSCALL")
                                            .try_concrete_u8().expect("failed to try_concrete for param1 in syscall");
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
                        let srand_seed = context.popp().expect("failed to pop from stack in S.SYSCALL");
                        context.srand(srand_seed.clone());
                        argument = format!("srand({:#X})", syscall);

                    },
                    4 => {
                        let rand_value: SymVarVec = context.rand();
                        context.pushp(rand_value);
                        argument = format!("rand({:#X})", syscall);
                    },
                    5 => argument = "flag_success".to_string(),
                    6 => argument = "split_register".to_string(),
                    _ => argument = "UNKNOWN".to_string(),
                }
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
