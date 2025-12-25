#![allow(warnings)]

use crate::dispatcher::RegvmInstructionReader;
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

pub fn regvm_disassembler(regvm_instruction_reader: &mut RegvmInstructionReader, context: &mut SymbolicContext, data_section: &Vec<u8>, emu: &bool) {
    let mut mnemonic: String = "R.UNKNOWN".to_string();
    let mut argument: String = "R.UNKNOWN".to_string();
    let mut pc_update: usize = 5;

    let instruction_location = regvm_instruction_reader.current_position();

    let mut opcode: u8 = regvm_instruction_reader.read_byte();
    let mut modifier_nibble:u8 = 0;

    if opcode >= 0xa0 && opcode <= 0xaf {
        // modifier instruction
        // opcode = next opcode
        // store modifier nibble
        modifier_nibble = opcode & 0x0f;
        opcode = regvm_instruction_reader.read_byte();
    }
    if (opcode & 0xc0) == 0xc0 { 
        let opcode_nibble:u8 = opcode & 7;
        // this is a mov instruction
        if (opcode & 4) == 0 {
            // the first parameter is a register
            // the nibble * 4 is the index of the register in the context structure
            // VM: load value in emulated register to VM real variable
            let reg_num1 = ((opcode & 7) << 2);
            if (modifier_nibble & 3) > 0 {
                // bit 1 and 2 were set from modifier instruction
                // VM: dereference value that was loaded into VM variable
                // we cover this functionality in the instructions themselves
            }
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.RRm".to_string();
                    let reg_num1 = ((opcode & 7) << 2);
                    let reg_num2 = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg2:{}, reg1:{}", context.reg_name(reg_num2), context.reg_name(reg_num1));
                    if *emu {
                        let mut reg1_value = context.get_reg(reg_num1);
                        if (modifier_nibble & 3) > 0 {
                            reg1_value = context.getp((reg1_value).try_concrete_u32().expect("could not concrete to u32 in modifier") as usize);
                        }
                        context.set_reg(reg_num2, reg1_value);
                    }
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.PRm".to_string();
                    let register = ((opcode & 7) << 2);
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("reg:{}, ptr:{:#X}", context.reg_name(register), pointer);
                    if *emu { 
                        let mut reg1_value = context.get_reg(reg_num1);
                        if (modifier_nibble & 3) > 0 {
                            reg1_value = context.getp((reg1_value).try_concrete_u32().expect("could not concrete to u32 in modifier") as usize);
                        }
                        context.stack.assign(pointer as usize, &reg1_value);
                    }
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.RR".to_string();
                let reg_num1 = ((opcode & 7) << 2);
                let reg_num2 = (((opcode >> 3) & 7) << 2);
                argument = format!("reg2:{}, reg1:{}", context.reg_name(reg_num2), context.reg_name(reg_num1));
                if *emu {
                    let mut reg1_value = context.get_reg(reg_num1);
                    if (modifier_nibble & 3) > 0 {
                        reg1_value = context.getp((reg1_value).try_concrete_u32().expect("could not concrete to u32 in modifier") as usize);
                    }
                    context.set_reg(reg_num2, reg1_value);
                }
            }
        } else if opcode_nibble == 4 { //b0100
            // first parameter is a immediate pointer in the instruction to dereference
            // modifier_nibble & 3 cannot be set
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.RPm".to_string();
                    let pointer = regvm_instruction_reader.read_pointer();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg:{}, ptr:{:#X}", context.reg_name(reg_num), pointer);
                    emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(pointer)));
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.PPm".to_string();
                    let pointer1 = regvm_instruction_reader.read_pointer();
                    let pointer2 = regvm_instruction_reader.read_pointer();
                    argument = format!("ptr1:{:#X}, ptr2:{:#X}", pointer1, pointer2);
                    emu.then(|| context.stack.assign(pointer2 as usize, &context.getp(pointer1 as usize)));
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.RP".to_string();
                let pointer = regvm_instruction_reader.read_pointer();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("reg:{}, ptr:{:#X}", context.reg_name(reg_num), pointer);
                emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(pointer)));
            }
        } else if opcode_nibble == 5 { //b0101
            // first parameter is an immediate
            // modifier_nibble & 3 cannot be set
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.RIm".to_string();
                    let immediate = regvm_instruction_reader.read_pointer();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg:{}, imm:{:#X}", context.reg_name(reg_num), immediate);
                    emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(immediate)));
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.PIm".to_string();
                    let immediate = regvm_instruction_reader.read_pointer();
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("ptr:{:#X}, imm:{:#X}", pointer, immediate);
                    emu.then(|| context.stack.assign(pointer as usize,&SymVarVec::concrete_u32(immediate)));
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.RI".to_string();
                let immediate = regvm_instruction_reader.read_pointer();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("reg:{}, imm:{:#X}", context.reg_name(reg_num), immediate);
                emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(immediate)));
            }
        } else if opcode_nibble == 6 { //b0110
            // just load the stack pointer into the VM register
            if (modifier_nibble & 3) > 0{
                // bit 1 and 2 were set from modifier instruction
                // VM: dereference value that was loaded into VM real variable
                // NOTE: we would cover this in the instructions themselves, but i happen to know
                // they're not used in this CTF so i'm just ignoring it
            }
            // in this VM, the only pointers(i think) are pointers to locations on the stack.
            // pointers to data section locations are loaded as immediates. 
            // so i'm going to say that a pointer is an index to a location to the stack and that
            // the SP == stack.len()
            // we'll see how that goes.
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.RSm".to_string();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg:{}", context.reg_name(reg_num));
                    emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(context.sp as u32)
                                                .try_into().expect("failed to set the stack location")));
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.PSm".to_string();
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("ptr:{:#X}", pointer);
                    emu.then(|| context.stack.assign(pointer as usize, &SymVarVec::concrete_u32(context.sp as u32)
                                                        .try_into().expect("failed to set the stack location")));
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.RS".to_string();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("reg:{}", context.reg_name(reg_num));
                emu.then(|| context.set_reg(reg_num, SymVarVec::concrete_u32(context.sp as u32)
                                            .try_into().expect("failed to set the stack location")));
            }
        }
    } else if (opcode - 0x11 <= 3) {
        mnemonic = "R.PUSH.R".to_string();
        let reg_num = (opcode - 0x11) << 2;
        argument = format!("reg:{}", context.reg_name(reg_num));
        emu.then(|| context.pushp(context.get_reg(reg_num)));
    } else if (opcode - 0x15 <= 3) {
        mnemonic = "R.POP.R".to_string();
        let reg_num = (opcode - 0x15) << 2;
        argument = format!("reg:{}", context.reg_name(reg_num));
        if *emu {
            let value = context.popp().expect("failed to pop from context.stack");
            context.set_reg(reg_num, value);
        }
    } else if (opcode >> 4) == 7 {
        mnemonic = "R.CMP.RR".to_string();
        let register1 = ((opcode & 3) << 2);
        let register2 = (((opcode >> 2) & 3) << 2);
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
        emu.then(|| context.cmp_rr(register1, register2));
    } else if (opcode >> 4) == 8 {
        mnemonic = "R.CMP.RI".to_string();
        let immediate = regvm_instruction_reader.read_pointer();
        let register = ((opcode & 3) << 2);
        argument = format!("reg:{}, imm:{:#X}", context.reg_name(register), immediate);
        emu.then(|| context.cmp_ri(register, immediate));
    } else if (opcode == 0x00) {
        mnemonic = "R.HLT".to_string();
        argument = "".to_string();
        emu.then(|| regvm_instruction_reader.set_position(0x170));
    } else if (opcode == 0x01) {
        mnemonic = "R.SYSCALL".to_string();
        argument = "".to_string();
        if *emu {
            let syscall = context.A.try_concrete_u32().expect("failed to try_concrete for syscall in reg_decoder");
            let mut string: String = "".to_string();
            match syscall {
                0 => {
                    context.set_reg(context.reg_value('A'),(SymVarVec::symbolic_u32("input_3".to_string())));
                    argument = format!("fscanf({:#X})", syscall);
                },
                1 => {
                    let input_pointer = context.B.try_concrete_u32().expect("failed to extract B in fgetc") as usize;
                    let mut input_size = context.C.try_concrete_u32().expect("failed to extract C in fgetc") as usize;
                    //input.push(SymVarVec::concrete_u8(0)); // null terminator
                    // there is an off by 1 error in the crackme.masm code that causes the hashing algorithm
                    // to hash 4 bytes past the end of the string.
                    // unfortunately, this is specific to this crackme and not a failure of the VM,
                    // but i have to deal with it anyway.
                    //input_size += 4;
                    let mut input = SymVarVec::symbolic_n("input_2".to_string(), input_size);
                    input.reverse();
                    context.stack.assign(input_pointer+input.len(), &input);
                    argument = format!("fgetc({:#X})", syscall)
                },
                2 => {
                    let data_pointer = context.B.try_concrete_u32().expect("failed to try_concrete for data_pointer in reg_decoder");
                    if data_pointer > 0x2000 {
                        let size = context.C.try_concrete_u32().expect("failed to try_concrete for string size in reg_decoder") as usize;
                        string = read_string(data_section, &(data_pointer as usize - 0x2000), size);
                    }
                    argument = format!("fputs({:#X}), {}", syscall, string)
                },
                3 => {
                    let rand_seed = context.B.clone();
                    context.srand(rand_seed);
                    argument = format!("srand({:#X})",syscall);
                },
                4 => {
                    let rand_value: SymVarVec = context.rand();
                    context.set_reg(context.reg_value('A'),rand_value);
                    argument = format!("rand({:#X})",syscall);
                },
                5 => argument = format!("unsupported({:#X})",syscall),
                6 => argument = format!("split_register({:#X})",syscall),
                _ => argument = format!("UNKNOWN({:#X})",syscall),
            }
        }
    } else if (opcode == 0x10) {
        mnemonic = "R.PUSH.I".to_string();
        let immediate = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}", immediate);
        emu.then(|| context.pushp(SymVarVec::concrete_u32(immediate)));
    } else if (opcode == 0x20) {
        mnemonic = "R.ADD.RR".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let register1 = ((((reg_param >> 4) - 1) & 3) << 2);
        let register2 = (((reg_param - 1) & 3) << 2);
        emu.then(|| context.add_rr(register1, register2));
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
    } else if (opcode == 0x21) {
        mnemonic = "R.ADD.RI".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        let register = (((reg_param >> 4) - 1) << 2);
        emu.then(|| context.add_ri(register, immediate));
        argument = format!("reg:{}, imm:{:#X}", context.reg_name(register), immediate);
    } else if (opcode == 0x30) {
        mnemonic = "R.SUB.RR".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        // for some reason be base pointer for the register locations (0x3b) is used in most
        // instructions except just a few. in this case, there are more registers above D which
        // may be used. we subtract 0x3b to convert the pointer from the beginning of the context
        // struct to the beginning of the register fields.
        let register1 = (((((reg_param >> 4) - 1) + 0xc) << 2) + 0xb) - 0x3b;
        let register2 = (((reg_param & 0xf) - 1) << 2);
        emu.then(|| context.sub_rr(register1, register2));
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
    } else if (opcode == 0x31) {
        let reg_param = regvm_instruction_reader.read_byte();
        let mut immediate = regvm_instruction_reader.read_pointer();
        if ((reg_param >> 4) - 1) <= 3 {
            mnemonic = "R.SUB.RI".to_string();
            let register = (((((reg_param >> 4) - 1) + 0xc) << 2) + 0xb) - 0x3b;
            argument = format!("reg:{}, imm:{:#X}", context.reg_name(register), immediate);
            emu.then(|| context.sub_ri(register,immediate));
        } else {
            mnemonic = "R.SUB.SI".to_string();
            argument = format!("imm:{:#X}", immediate);
            if *emu {
                // there is an off by 1 error in the crackme.masm code that causes the hashing algorithm
                // to hash 4 bytes past the end of the string.
                // unfortunately, this is specific to this crackme and not a failure of the VM,
                // but i have to deal with it anyway.
                immediate += 4;
                // adding 4 and doing nothing else works only because the hashing algorithm in the crackme
                // multiplies the extra 4 bytes from the string, instantiated to 0, then xoring.
                // the multiplication ends with 0, and the xor with 0 changes nothing about our hash.
                // i suspect the VM also has memory instantiated to 0 that doesn't affect the hash just like we have here.
                context.stack.assign(context.sp, &SymVarVec::concrete_n(immediate as usize)); // assign can grow if needed
                context.sp -= immediate as usize;
            }
        }
    } else if (opcode == 0x40) {
        mnemonic = "R.XOR.RR".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let register1 = (((reg_param >> 4) - 1) << 2);
        let register2 = (((reg_param & 0xf) - 1) << 2);
        emu.then(|| context.xor_rr(register1, register2));
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
    } else if (opcode == 0x41) {
        mnemonic = "R.XOR.RI".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        let register = (((((reg_param >> 4) - 1) + 0xc) << 2) + 0xb) - 0x3b;
        emu.then(|| context.xor_ri(register,immediate));
        argument = format!("reg:{}, imm:{:#X}", context.reg_name(register), immediate);
    } else if (opcode == 0x50) {
        mnemonic = "R.MUL.RR".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let register1 = (((reg_param & 0xf) - 1) << 2);
        let register2 = (((reg_param >> 4) - 1) << 2);
        emu.then(|| context.mul_rr(register1, register2));
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
    } else if (opcode == 0x51) {
        mnemonic = "R.MUL.RI".to_string();
        let reg_param = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        let register = ((((reg_param >> 4) + 3) & 3) << 2);
        emu.then(|| context.mul_ri(register,immediate));
        argument = format!("reg:{}, imm:{:#X}", context.reg_name(register), immediate);
    } else if (opcode == 0x60) {
        mnemonic = "R.CALL".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        if *emu {
            context.pushp(SymVarVec::concrete_u32(regvm_instruction_reader.position as u32));
            regvm_instruction_reader.set_position(target as usize - 0x1000); 
        }
    } else if (opcode == 0x61) {
        mnemonic = "R.RET".to_string();
        if *emu {
            context.sp += (regvm_instruction_reader.read_byte() << 2) as usize;
            let target = context.popp().expect("failed to pop off stack")
                          .try_concrete_u32().expect("failed to pop u32 off stack");
            regvm_instruction_reader.set_position(target as usize);
        }
        argument = "".to_string();
    } else if (opcode == 0x62) {
        mnemonic = "R.JE".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        // the way this challenge is written, JE is often used for loops
        if *emu {
            if context.flags.try_concrete().expect("failed to try_concrete JE flags") as usize == 1 {
                regvm_instruction_reader.set_position(target as usize - 0x1000);
            }
        }
    } else if (opcode == 0x63) {
        mnemonic = "R.JNE".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        // the way this challenge is written, the only uses of JNE go to the fail condition
        // so we just skip it.
/*        if *emu {
            if context.flags.try_concrete().expect("failed to try_concrete JNE flags") as usize != 1 {
                regvm_instruction_reader.set_position(target as usize - 0x1000);
            }
        }*/
    } else if (opcode == 0x64) {
        mnemonic = "R.JEFLAGS2".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000);
    } else if (opcode == 0x68) {
        mnemonic = "R.JMP".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        emu.then(|| regvm_instruction_reader.set_position(target as usize - 0x1000));
    }

    if (modifier_nibble & 3) > 0 {
        argument = format!("{} dereferenced",argument);
    }

    println!("{:#X}: {}({:#X}), {}", instruction_location, mnemonic, opcode, argument);
}
