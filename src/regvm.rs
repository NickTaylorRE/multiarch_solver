#![allow(warnings)]

use crate::dispatcher::RegvmInstructionReader;
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

pub fn regvm_disassembler(regvm_instruction_reader: &mut RegvmInstructionReader, context: &mut Context, data_section: &Vec<u8>) {
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
            if (modifier_nibble & 3) > 0 {
                // bit 1 and 2 were set from modifier instruction
                // VM: dereference value that was loaded into VM variable
            }
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.RRM".to_string();
                    let reg_num1 = ((opcode & 7) << 2);
                    let reg_num2 = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg1:{}, reg2:{}", context.reg_name(reg_num1), context.reg_name(reg_num2));
                    context.set_reg(reg_num2, context.get_reg(reg_num1));
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.RP".to_string();
                    let reg_num = ((opcode & 7) << 2);
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("reg:{}, ptr:{:#X}", context.reg_name(reg_num), pointer);
                    //context.set_reg(reg_num, pointer) not quite sure how i would want to do this
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.RR".to_string();
                let reg_num1 = ((opcode & 7) << 2);
                let reg_num2 = (((opcode >> 3) & 7) << 2);
                argument = format!("reg1:{}, reg2:{}", context.reg_name(reg_num1), context.reg_name(reg_num2));
                context.set_reg(reg_num2, context.get_reg(reg_num1));
            }
        } else if opcode_nibble == 4 { //b0100
            // first parameter is a immediate pointer in the instruction to dereference
            // modifier_nibble & 3 cannot be set
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.PRM".to_string();
                    let pointer = regvm_instruction_reader.read_pointer();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("ptr:{:#X}, reg:{}",pointer, context.reg_name(reg_num));
                    context.set_reg(reg_num, pointer);
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.PP".to_string();
                    let pointer1 = regvm_instruction_reader.read_pointer();
                    let pointer2 = regvm_instruction_reader.read_pointer();
                    argument = format!("ptr1:{:#X}, ptr2:{:#X}", pointer1, pointer2);
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.PR".to_string();
                let pointer = regvm_instruction_reader.read_pointer();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("ptr:{:#X}, reg:{}",pointer, context.reg_name(reg_num));
                context.set_reg(reg_num, pointer);
            }
        } else if opcode_nibble == 5 { //b0101
            // first parameter is an immediate
            // modifier_nibble & 3 cannot be set
            if (modifier_nibble >> 2) > 0 {
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.IRM".to_string();
                    let immediate = regvm_instruction_reader.read_pointer();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("imm:{:#X}, reg:{}", immediate, context.reg_name(reg_num));
                    context.set_reg(reg_num, immediate);
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.IP".to_string();
                    let immediate = regvm_instruction_reader.read_pointer();
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("imm1:{:#X}, ptr2:{:#X}", immediate, pointer);
                    //context.set_reg(reg_num, pointer); not sure how i want to do this
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.IR".to_string();
                let immediate = regvm_instruction_reader.read_pointer();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("imm:{:#X}, reg:{}", immediate, context.reg_name(reg_num));
                context.set_reg(reg_num, immediate);
            }
        } else if opcode_nibble == 6 { //b0110
            // just load the stack pointer into the VM register
            if (modifier_nibble & 3) > 0{
                // bit 1 and 2 were set from modifier instruction
                // VM: dereference value that was loaded into VM real variable
            }
            if (modifier_nibble >> 2) > 0 {
                // TODO: none of the stack pointer stuff is implemented right now.
                if (opcode & 0x20) == 0 { //opcode & 0x20
                    // second parameter is register to store value into
                    mnemonic = "R.MOV.SRM".to_string();
                    let reg_num = (((opcode >> 3) & 7) << 2);
                    argument = format!("reg:{}", context.reg_name(reg_num));
                } else {
                    // second parameter is an pointer
                    mnemonic = "R.MOV.SP".to_string();
                    let pointer = regvm_instruction_reader.read_pointer();
                    argument = format!("op:{:#X}, ptr:{:#X}", opcode, pointer);
                }
            } else if ((opcode >> 3) & 4) == 0 {
                // second parameter is register
                // this path uses S.STP instead of a regular assignment
                mnemonic = "R.MOV.SR".to_string();
                let reg_num = (((opcode >> 3) & 7) << 2);
                argument = format!("reg:{}", context.reg_name(reg_num));
            }
        }
    } else if (opcode - 0x11 <= 3) {
        mnemonic = "R.PUSH.R".to_string();
        let reg_num = (opcode - 0x11) << 2;
        argument = format!("reg:{}", context.reg_name(reg_num));
        context.stack.push(context.get_reg(reg_num));
    } else if (opcode - 0x15 <= 3) {
        mnemonic = "R.POP.R".to_string();
        let reg_num = (opcode - 0x15) << 2;
        argument = format!("reg:{}", context.reg_name(reg_num));
        let value = context.stack.pop().expect("failed to pop from context.stack");
        context.set_reg(reg_num, value);
    } else if (opcode >> 4) == 7 {
        mnemonic = "R.CMP.RR".to_string();
        let register1 = ((opcode & 3) << 2);
        let register2 = (((opcode >> 2) & 3) << 2);
        argument = format!("reg1:{}, reg2:{}", context.reg_name(register1), context.reg_name(register2));
    } else if (opcode >> 4) == 8 {
        mnemonic = "R.CMP.IR".to_string();
        let immediate = regvm_instruction_reader.read_pointer();
        let register = ((opcode & 3) << 2);
        argument = format!("imm:{}, reg:{}", immediate,  context.reg_name(register));
    } else if (opcode == 0x00) {
        mnemonic = "R.INVALID".to_string();
        argument = "".to_string();
    } else if (opcode == 0x01) {
        mnemonic = "R.SYSCALL".to_string();
        let syscall = context.A;
        let mut string: String = "".to_string();
        if context.B > 0x2000 {
            string = read_string(data_section, &(context.B as usize - 0x2000), context.C as usize);
        }
        match syscall {
            0 => argument = format!("fscanf({:#X}), {}", syscall, string),
            1 => argument = format!("fgetc({:#X})", syscall),
            2 => argument = format!("fputs({:#X}), {}", syscall, string),
            3 => argument = format!("srand({:#X})",syscall),
            4 => argument = format!("rand({:#X})",syscall),
            5 => argument = format!("unsupported({:#X})",syscall),
            6 => argument = format!("split_register({:#X})",syscall),
            _ => argument = format!("UNKNOWN({:#X})",syscall),
        }
    } else if (opcode == 0x10) {
        mnemonic = "R.PUSHPC".to_string();
        argument = regvm_instruction_reader.read_pointer().to_string();
    } else if (opcode == 0x20) {
        mnemonic = "R.ADD_RR".to_string();
        argument = regvm_instruction_reader.read_byte().to_string();
    } else if (opcode == 0x21) {
        mnemonic = "R.ADD_RI".to_string();
        let register = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        argument = format!("reg:{:#X}, imm:{:#X}", register, immediate);
    } else if (opcode == 0x30) {
        mnemonic = "R.SUB_RR".to_string();
        argument = regvm_instruction_reader.read_byte().to_string();
    } else if (opcode == 0x31) {
        mnemonic = "R.SUB_RI".to_string();
        let register = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        argument = format!("reg:{:#X}, imm:{:#X}", register, immediate);
    } else if (opcode == 0x40) {
        mnemonic = "R.XOR_RR".to_string();
        argument = regvm_instruction_reader.read_byte().to_string();
    } else if (opcode == 0x41) {
        mnemonic = "R.XOR_RI".to_string();
        let register = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        argument = format!("reg:{:#X}, imm:{:#X}", register, immediate);
    } else if (opcode == 0x50) {
        mnemonic = "R.MUL_RR".to_string();
        argument = regvm_instruction_reader.read_byte().to_string();
    } else if (opcode == 0x51) {
        mnemonic = "R.MUL_RI".to_string();
        let register = regvm_instruction_reader.read_byte();
        let immediate = regvm_instruction_reader.read_pointer();
        argument = format!("reg:{:#X}, imm:{:#X}", register, immediate);
    } else if (opcode == 0x60) {
        mnemonic = "R.CALL".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000); //must sub 0x1000 because
                                                                         //its base of text section
        // update stack here
    } else if (opcode == 0x61) {
        mnemonic = "R.RET".to_string();
        // read from stack
        argument = "".to_string();
    } else if (opcode == 0x62) {
        mnemonic = "R.JE".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000); //must sub 0x1000 because
    } else if (opcode == 0x63) {
        mnemonic = "R.JNE".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000); //must sub 0x1000 because
    } else if (opcode == 0x64) {
        mnemonic = "R.JEFLAGS2".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000); //must sub 0x1000 because
    } else if (opcode == 0x68) {
        mnemonic = "R.JMP".to_string();
        let target = regvm_instruction_reader.read_pointer();
        argument = format!("{:#X}",target as usize - 0x1000);
        //regvm_instruction_reader.set_position(target as usize - 0x1000); //must sub 0x1000 because
    }

    println!("{:#X}: {}({:#X}), {}", instruction_location, mnemonic, opcode, argument);
}
