# Table of Contents
- [The Challenge](#challenge)
- [The VM](#the-vm)
  - [Stack Instruction Set](#stack-vm)
  - [Register Instruction Set](#register-vm)
- [The Symbex VM](#symbex-vm)
  - [Challenge 1](#challenge-1)
  - [Challenge 2](#challenge-2)
  - [Challenge 3](#challenge-3)
- [What Would Be Next](#what-would-be-next)
  

# Challenge
This challenge is a custom Virtual Machine architecture with 2 instruction sets. One stack machine and one that has register and stack operations. The VM then accepts a .masm program and the command line to execute. The goal of the challenge is to disassemble the instructions in the given `crackme.masm` and solve the 3 challenges within.

I initially sought to understand the VM architecture and build a small disassembler. Once i was done with the disassembler i thought i was set up pretty well to build a symbolic execution VM that i could use to solve the challenges somewhat automatically rather than trying to reimplement the algorithms in another language. This was not the most efficient nor quickest way to solve the challenge and not where i would likely spend my time during a CTF, but the CTF was already closed when i discovered it and this method was fun.
  

# The VM
The VM will read in 3 sections from the .masm file. Two of these get mapped into virtual memory. The third is used to change execution modes from one architecture to the other. This arch section is reserved for the VM emulator and not exposed to the program. The three sections in the file are

  1. text section - VM code
  2. data section - static data like strings and other data types
  3. arch section - defines which instruction set to use for an instruction, according to the program counter

After the file is parsed and the 3 sections are loaded, the VM initializes its `vm_context`, maps the text section and data section to memory regions, creates a memory region for the stack, and initializes the program counter to 0x1000(beginning of the text section), and the stack pointer is set to 0x8f00.

```
struct vm_context __packed
{
    uint64_t text_section_ptr;
    uint64_t data_section_ptr;
    uint64_t stack_section_ptr;
    uint64_t arch_modifier_buf;
    uint64_t arch_modifier_buf_len;
    uint64_t flag_check_callback;   // callback to flag retrieval function
    uint8_t vm_error;               // error flag when an instruction decodes incorrectly
    uint8_t privs;
    uint8_t eflags;
    uint32_t pc;                    // program counter
    uint32_t sp;                    // stack pointer
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
};
```
*vm_context structure*

Each region is mapped RWX and through individual calls to mmap. The size of each section is hardcoded to 0x1000 so this VM cannot handle large programs.
so the total ranges for each section is
  1. text = 0x1000 - 0x1fff
  2. data = 0x2000 - 0x2fff
  3. stack = 0x8000 - 0x8fff
  
  
# Stack VM
The stack VM instruction set is very simple and easy to parse. Each instruction is 5 bytes with just 1 byte defining the opcode and a 4 byte operand. Each instruction is 5 bytes even if the operands don't require a full 4 bytes or the operation has no usable operands. The stack VM also implements a very standard stack machine where an operation such as an ADD would pop two values off the stack, add them, then place the result back onto the stack.

```
struct stackvm_instruction __packed
{
    uint8_t opcode;
    uint32_t operand;
};
```
*stack vm instruction*

Each 1 byte opcode maps very cleanly to an operation.
```
0x10 => S.LDB
0x20 => S.LDW
0x30 => S.PUSHP
0x40 => S.LDP
0x50 => S.POPP
0x60 => S.ADDP
0x61 => S.SUBP
0x62 => S.XORP
0x63 => S.ANDP
0x70 => S.JMP
0x71 => S.JE
0x72 => S.JNE
0x80 => S.CMPP
0xa0 => S.SYSCALL
    0 => fscanf
    1 => unsupported syscall
    2 => fput
    3 => srand
    4 => wrand
    5 => flag_success
    6 => heap_write
    _ => UNKNOWN
0xff => S.HLT
_ => S.INVALID
```
*supported operations in the stack machine. S. stands for Stack operation*

The only weirdness in the stack machine is that some operations support pushing `Byte`(8b), `Word`(16b), or `Pointer`(32b) sized data to the stack. This means a program could have 2 `LDW` instructions followed by a `POPP`. The result would be 2 word sized immediates loaded to the stack, then both poped off as 1 pointer sized value. 1 `PUSHP` followed by 4 `POPB` instructions is also possible. Something like this may even just be an optimization, where the bytes aren't totally related to eachother.


# Reg VM
The reg VM instruction set is much more complicated, with some inconsistent encodings between the different types of instructions.

TODO
  
  
# Symbex VM
The structure for the disassembler matches the structure in the challenge VM quite closely with the only difference being the challenge VM is just a VM, and mine is on top of a symbex engine.
```
                       ┌─> stack_decoder ─┐
                       │                  │
    dispatcher ────────┤                  ├──> symbex_vm ──> symbex_engine ──> z3
        ^              │                  │
        │              └─> reg_decoder ───┘
        v
    get_pc_arch
```
*symbolic execution vm architecture*

dispatcher [dispatcher:118](solver/src/dispatcher.rs#L118)
  - Determines which instruction set the next instruction is for. 
  - Reads that instruction into the correct data type for the respective decoder. 
  - Calls the correct instruction decoder. 
  - Allow out of band changes to be made at certain executing instructions. Like breakpoints. [dispatcher:124](solver/src/dispatcher.rs#L124)
  
get_pc_arch [dispatcher:20](solver/src/dispatcher.rs#L20)
  - Reads a value from the arch buffer corresponding to the program counter. 
  - Parses out which decoder should be used. [dispatcher:30](solver/src/dispatcher.rs#L30)
  
stack_decoder
  - Matches the instruction opcode against defined instructions. 
  - Format a mnemonic and argument(disassembling) for the reverser to understand the program. 
  - Call a symbex_vm function to update the state of the symbex_vm accordingly.

reg_decoder
  - Parses the instruction opcode against defined instructions. 
  - Format a mnemonic and argument(disassembling) for the reverser to understand the program. 
  - Call a symbex_vm function to update the state of the symbex_vm accordingly.

symbex_vm
  - Performs the actual operations and updates the context.
  - Mostly a wrapper around the symbex engine with functions named after the operations found in the disassembly for convenience in writing the disassembler.
  - Manages the context.

symbex_engine
  - Exposes the primitive SymVar symbolic variable which represents a single byte. 
  - Exposes the SymVarVec which is a vector of SymVars. This acts as a vector of bytes effectively.
  - Provides instantiation, arithmetic operations, bitwise operations, and equality operations for SymVar and SymVarVec.
  - Provides conversion to z3 BitVectors and solving with Z3 for SymVars with and without rand brute forcing.
  
  
The Symbex VM has 2 operating modes. A straight linear disassembly, and an emulation mode. The emulation mode actually performs symbolic execution, solving, and produces an instruction trace rather than a linear disassembly. this is good enough for this challenge because there are no gigantic loops, but would likely need to be changed for more complex algorithms.
The emulation mode also provides dereferencing of strings and syscall numbers since it has the ability to do that. the linear disassembly just disassembles with no attempt to help the reverser see strings or syscall numbers.
  
<table>
<tr valign="top">
<td width="50%">

***linear disassembly***
```rust
Read in 757 bytes from ../crackme.masm
section type:1, section addr:19, section size: 357
section type:2, section addr:376, section size: 336
section type:3, section addr:712, section size: 45
0x0: S.LDB(0x10), 75(0x4B)
0x5: S.PUSHP(0x30), 8192(0x2000)
0xA: S.LDB(0x10), 2(0x2)
0xF: S.SYSCALL(0xA0), (0x0)
0x14: S.LDB(0x10), 43(0x2B)
0x19: S.PUSHP(0x30), 8365(0x20AD)
0x1E: S.LDB(0x10), 2(0x2)
0x23: S.SYSCALL(0xA0), (0x0)
0x28: S.LDB(0x10), 0(0x0)
0x2D: S.SYSCALL(0xA0), (0x0)
0x32: S.LDW(0x20), 4919(0x1337)
0x37: S.LDW(0x20), 1337(0x539)
0x3C: S.PUSHP(0x30), 140989193(0x8675309)
0x41: S.XORP(0x62), (0x0)
0x46: S.ADDP(0x60), (0x0)
0x4B: S.PUSHP(0x30), 2863311530(0xAAAAAAAA)
0x50: S.CMPP(0x80), (0x0)
0x55: S.JNE(0x72), 0x10B(0x110B)
0x5A: R.MOV.RI(0xC5), reg:A, imm:0x2
0x5F: R.MOV.RI(0xCD), reg:B, imm:0x20D8
0x64: R.MOV.RI(0xD5), reg:C, imm:0x1E
0x69: R.SYSCALL(0x1), 
0x6A: R.SUB.SI(0x31), imm:0x20
0x70: R.MOV.RS(0xCE), reg:B
0x71: R.PUSH.R(0x12), reg:B
0x72: R.MOV.RI(0xD5), reg:C, imm:0x20
0x77: R.MOV.RI(0xC5), reg:A, imm:0x1
0x7C: R.SYSCALL(0x1), 
0x7D: R.POP.R(0x15), reg:A
0x7E: R.MOV.RI(0xCD), reg:B, imm:0x20
0x83: R.CALL(0x60), 0x11C
0x88: R.CMP.RI(0x80), reg:A, imm:0x7331
0x8D: R.JNE(0x63), 0x10B
0x92: R.MOV.RI(0xC5), reg:A, imm:0x0
0x97: S.LDB(0x10), 90(0x5A)
0x9C: S.PUSHP(0x30), 8438(0x20F6)
0xA1: S.LDB(0x10), 2(0x2)
0xA6: S.SYSCALL(0xA0), (0x0)
0xAB: R.SYSCALL(0x1), 
0xAC: R.MOV.RR(0xC8), reg2:B, reg1:A
0xAD: R.MOV.RI(0xC5), reg:A, imm:0x3
0xB2: R.SYSCALL(0x1), 
0xB3: R.MOV.RI(0xD5), reg:C, imm:0x0
0xB8: R.CALL(0x60), 0x145
0xBD: R.PUSH.I(0x10), 0xFFFFFF
0xC2: R.PUSH.R(0x11), reg:A
0xC3: S.ANDP(0x63), (0x0)
0xC8: S.PUSHP(0x30), 12648430(0xC0FFEE)
0xCD: S.CMPP(0x80), (0x0)
0xD2: S.JE(0x71), 0xEC(0x10EC)
0xD7: R.ADD.RI(0x21), reg:C, imm:0x1
0xDD: R.CMP.RI(0x82), reg:C, imm:0xA
0xE2: R.JE(0x62), 0x10B
0xE7: R.JMP(0x68), 0xB8
0xEC: R.MOV.RI(0xD5), reg:C, imm:0x39
0xF1: R.MOV.RI(0xCD), reg:B, imm:0x2074
0xF6: R.MOV.RI(0xC5), reg:A, imm:0x2
0xFB: R.SYSCALL(0x1), 
0xFC: S.LDB(0x10), 5(0x5)
0x101: S.SYSCALL(0xA0), (0x0)
0x106: S.JMP(0x70), 0x11B(0x111B)
0x10B: R.MOV.RI(0xD5), reg:C, imm:0x29
0x110: R.MOV.RI(0xCD), reg:B, imm:0x204B
0x115: R.MOV.RI(0xC5), reg:A, imm:0x2
0x11A: R.SYSCALL(0x1), 
0x11B: R.HLT(0x0), 
0x11C: R.MOV.RR(0xD0), reg2:C, reg1:A
0x11D: R.ADD.RR(0x20), reg1:A, reg2:B
0x11F: R.PUSH.R(0x11), reg:A
0x120: R.MOV.RI(0xCD), reg:B, imm:0x0
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x142: R.MOV.RR(0xC1), reg2:A, reg1:B
0x143: R.RET(0x61), 
0x144: S.INVALID(0x1), (0x133700C5)
0x145: R.MOV.RI(0xC5), reg:A, imm:0x133700
0x14A: S.LDB(0x10), 4(0x4)
0x14F: S.SYSCALL(0xA0), (0x0)
0x154: R.POP.R(0x16), reg:B
0x155: R.XOR.RR(0x40), reg1:A, reg2:B
0x157: R.PUSH.R(0x11), reg:A
0x158: S.PUSHP(0x30), 4076008178(0xF2F2F2F2)
0x15D: S.XORP(0x62), (0x0)
0x162: R.POP.R(0x15), reg:A
0x163: R.RET(0x61), 
```

</td>
<td width="50%">

***emu trace with solving***
```rust
Read in 757 bytes from ../crackme.masm
section type:1, section addr:19, section size: 357
section type:2, section addr:376, section size: 336
section type:3, section addr:712, section size: 45
0x0: S.LDB(0x10), 75(0x4B)
0x5: S.PUSHP(0x30), 8192(0x2000)
0xA: S.LDB(0x10), 2(0x2)
0xF: S.SYSCALL(0xA0), fputs, Welcome to the multiarch of madness! Let's see how well you understand it.
(0x0)
0x14: S.LDB(0x10), 43(0x2B)
0x19: S.PUSHP(0x30), 8365(0x20AD)
0x1E: S.LDB(0x10), 2(0x2)
0x23: S.SYSCALL(0xA0), fputs, Challenge 1 - What's your favorite number? (0x0)
0x28: S.LDB(0x10), 0(0x0)
0x2D: S.SYSCALL(0xA0), fscanf(0x0)
0x32: S.LDW(0x20), 4919(0x1337)
0x37: S.LDW(0x20), 1337(0x539)
0x3C: S.PUSHP(0x30), 140989193(0x8675309)
0x41: S.XORP(0x62), (0x0)
0x46: S.ADDP(0x60), (0x0)
0x4B: S.PUSHP(0x30), 2863311530(0xAAAAAAAA)
0x50: S.CMPP(0x80), (0x0)
Solution found: input_b0, 0x8F
Solution found: input_b1, 0x5A
Solution found: input_b2, 0x54
Solution found: input_b3, 0x7A
0x55: S.JNE(0x72), 0x10B(0x110B)
0x5A: R.MOV.RI(0xC5), reg:A, imm:0x2
0x5F: R.MOV.RI(0xCD), reg:B, imm:0x20D8
0x64: R.MOV.RI(0xD5), reg:C, imm:0x1E
0x69: R.SYSCALL(0x1), fputs(0x2), Challenge 2 - Tell me a joke: 
0x6A: R.SUB.SI(0x31), imm:0x20
0x70: R.MOV.RS(0xCE), reg:B
0x71: R.PUSH.R(0x12), reg:B
0x72: R.MOV.RI(0xD5), reg:C, imm:0x20
0x77: R.MOV.RI(0xC5), reg:A, imm:0x1
0x7C: R.SYSCALL(0x1), fgetc(0x1)
0x7D: R.POP.R(0x15), reg:A
0x7E: R.MOV.RI(0xCD), reg:B, imm:0x20
0x83: R.CALL(0x60), 0x11C
0x11C: R.MOV.RR(0xD0), reg2:C, reg1:A
0x11D: R.ADD.RR(0x20), reg1:A, reg2:B
0x11F: R.PUSH.R(0x11), reg:A
0x120: R.MOV.RI(0xCD), reg:B, imm:0x0
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x142: R.MOV.RR(0xC1), reg2:A, reg1:B
0x143: R.RET(0x61), 
0x88: R.CMP.RI(0x80), reg:A, imm:0x7331
Solution found: input_2_b0, 0x46
Solution found: input_2_b1, 0x91
Solution found: input_2_b2, 0x0
Solution found: input_2_b3, 0x0
0x8D: R.JNE(0x63), 0x10B
0x92: R.MOV.RI(0xC5), reg:A, imm:0x0
0x97: S.LDB(0x10), 90(0x5A)
0x9C: S.PUSHP(0x30), 8438(0x20F6)
0xA1: S.LDB(0x10), 2(0x2)
0xA6: S.SYSCALL(0xA0), fputs, Challenge 3 - Almost there! But can you predict the future?
What number am I thinking of? (0x0)
0xAB: R.SYSCALL(0x1), fscanf(0x0)
0xAC: R.MOV.RR(0xC8), reg2:B, reg1:A
0xAD: R.MOV.RI(0xC5), reg:A, imm:0x3
0xB2: R.SYSCALL(0x1), srand(0x3)
0xB3: R.MOV.RI(0xD5), reg:C, imm:0x0
0xB8: R.CALL(0x60), 0x145
0x145: R.MOV.RI(0xC5), reg:A, imm:0x133700
0x14A: S.LDB(0x10), 4(0x4)
0x14F: S.SYSCALL(0xA0), rand(0x4)(0x0)
0x154: R.POP.R(0x16), reg:B
0x155: R.XOR.RR(0x40), reg1:A, reg2:B
0x157: R.PUSH.R(0x11), reg:A
0x158: S.PUSHP(0x30), 4076008178(0xF2F2F2F2)
0x15D: S.XORP(0x62), (0x0)
0x162: R.POP.R(0x15), reg:A
0x163: R.RET(0x61), 
0xBD: R.PUSH.I(0x10), 0xFFFFFF
0xC2: R.PUSH.R(0x11), reg:A
0xC3: S.ANDP(0x63), (0x0)
0xC8: S.PUSHP(0x30), 12648430(0xC0FFEE)
0xCD: S.CMPP(0x80), (0x0)
Brute forcing. This will take a while.
Solution found: input_3_b0, 0x2
Solution found: input_3_b1, 0xB6
Solution found: input_3_b2, 0x4
Solution found: input_3_b3, 0x3C
```
</td>
</tr>
</table>
  
  
Aside from all the work building the loader, multiple architecture dispatcher, and disassemblers, i also needed to build support in the symbex engine for all the operations supported in the VM, or at least in this crackme.  
  
The core data types in the symbex engine are the [SymVar::Var](symbex/src/symbex_engine.rs#L20) which is a symbolic variable, and the [SymVar::Concrete](symbex/src/symbex_engine.rs#L18) which is a concrete variable. Any operation that can be done with concrete variables will be as soon as the operation is requested. Otherwise the operation will be symbolically represented. The [SymVar::Add](symbex/src/symbex_engine.rs#L297) operation shows this. The `SymVar` is a `u64` size in rust to support operations of any size while handling carrying and overflow properly.  
  
The [SymVarVec](symbex/src/symbex_engine.rs#L583) type is a Vec<SymVar>. In a `SymVarVec`, the `SymVar` represents a single `u8` byte and the `SymVarVec` represents an arbitrary sized array of bytes. Much like a more typical computer represents data. There are also methods for converting the vec to a pointer size `SymVar` and other sizes, as well as methods for pointer sized operations. 
  
  
## Challenge 1
<table>
<tr valign="top">
<td width="50%">

Challenge 1 is a simple mathematical equation so it is a good first test for the symbex engine.  
```rust
0xAAAAAAAA = input + (0x13370539 ^ 0x08675309)
```
Of course for the first challenge there was a lot of work getting the operations working correctly and tested. Aside from that, the biggest challenge was trying to support the sized load operations `LDB`, `LDW`, `LDP` and their affect on the stack. Before tackling this challenge only the `SymVar` existed which was pointer sized including each stack location. I considered leaving everything pointer sized and annotating the data from the load operation to then check the data size when `POPP`ing, but decided that was going to be messy and not a good foundation for future work. I decided to build the SymVarVec to emulate the stack more correctly. This meant i had to create conversion methods between a vector of symbolic bytes and symbolic pointer sized data so that the math and bitwise operations carried and overflowed correctly.  
  
Second, i had to decide 
 * How i was going to solve in the middle of an emulation.
 * What degree of automated i wanted the symbex_vm to be. 
  
I settled on a pseudo breakpoint system [breakpoints](solver/src/dispatcher.rs#L129) which simply performs some operation before an instruction executes. This is not completely automated, but does give me everything i need to just specify a point at which solving should be done and just do that. More on this in the What Would Be Next section.  
  
For the purpose of solving for the challenge conditions, each conditional jump which may lead to the failure condition(`0x10b`) first calls the `CMPP` operation to set the flags register before calling the conditional jump. In the challenge VM, this flags register appears to store whether the subtract operation(typical compare operation) had an  equal result `0b001`, overflowed negative `0b110`, or was unequal and not negative `0b100`. The symbex VM only supports an `==` or `!=` result with no regard for negatives because it doesn't appear to be used in the crackme.  
  
Solving after the `CMPP` operation was as simple as calling the [SymVar::try_solve()](symbex/src/symbex_engine.rs#L62) method on the `SymVar` stored in the [SymbolicContext.flags](symbex/src/symbex_engine.rs#L62) register. `try_solve` simply converts the `SymVar` variables to z3 variables, assigns the constraints, then lets z3 solve and get a model which satisfies the constraints.  
  
The output shows each byte value individually. Inputting this value back into the challenge would be as simple as concattenating the bytes Big Endian and converting to decimal to be entered in the command line


</td>
<td width="50%">

***Challenge 1 code***
```rust
0x0: S.LDB(0x10), 75(0x4B)
0x5: S.PUSHP(0x30), 8192(0x2000)
0xA: S.LDB(0x10), 2(0x2)
0xF: S.SYSCALL(0xA0), fputs, Welcome to the multiarch of madness! Let's see how well you understand it.
(0x0)
0x14: S.LDB(0x10), 43(0x2B)
0x19: S.PUSHP(0x30), 8365(0x20AD)
0x1E: S.LDB(0x10), 2(0x2)
0x23: S.SYSCALL(0xA0), fputs, Challenge 1 - What's your favorite number? (0x0)
0x28: S.LDB(0x10), 0(0x0)
0x2D: S.SYSCALL(0xA0), fscanf(0x0)
0x32: S.LDW(0x20), 4919(0x1337)
0x37: S.LDW(0x20), 1337(0x539)
0x3C: S.PUSHP(0x30), 140989193(0x8675309)
0x41: S.XORP(0x62), (0x0)
0x46: S.ADDP(0x60), (0x0)
0x4B: S.PUSHP(0x30), 2863311530(0xAAAAAAAA)
0x50: S.CMPP(0x80), (0x0)
Solution found: input_b0, 0x8F
Solution found: input_b1, 0x5A
Solution found: input_b2, 0x54
Solution found: input_b3, 0x7A
0x55: S.JNE(0x72), 0x10B(0x110B)
```

</td>
</td>
</table>
  
  
# Challenge 2
<table>
<tr valign="top">
<td width="50%">

Challenge 2 was a slightly more complicated hashing algorithm. Luckily it was not complicated enough for z3 to fail at. This challenge accepts an up to 0x20 byte string at the command line, hashes each byte of that string 4 bytes at a time, then compares the result to a hardcoded result. Below is rust code showing the algorithm.
```rust
accumulator = 0;
for i in (0..0x20).step_by(4) {
    val = (0xCAFEBABE * input[i..i+4]) >> 0x20;
    accumulator = val ^ accumulator;
}
if(xor_result==0x7331) {
    // correct
}
```
The example above uses an iterator for the loop but the crackme code uses pointer arithmetic on the current position in the string and the end of the string as the condition to exit the loop. There is also code to push the length of the string to the stack and pop it later to be used as the length of the string. All of this required me to structure the `SymbolicContext` stack in the same or similar way that the challenge VM creates its stack.  
  
In short, i have a [SymbolicContext.stack](symbex/src/symbex_engine.rs#L26) that gets instantiated to a large size. Most of the values will be 0 and unused. Then there is the [SymbolicContext.sp](symbex/src/symbex_engine.rs#L32) that gets instantiated to the same value that the stack in the challenge VM gets instantiated to. All of this allows pointer arithmetic and proper stack growth. I was able to solve the first challenge with a stack that was just a 0 length `SymVarVec` that grew up rather than down, and pushed and popped values as needed. This challenge required me to remove pushing and popping and abstract those operations through the [SymVarVec::assign](symbex/src/symbex_engine.rs#L747) operation. So the symbex vm doesn't push and pop to the Vec like Vec.push() supports, it now assigns values up to a specified length to the prebuilt stack, and decrements the stack pointer.  
  
The syscall `fgetc` in this challenge supports an input up to 0x20 in length and z3 finds solutions of size 0x20, but there are solutions that are much smaller. So i change the size of the symbolic input with a [breakpoint](solver/src/dispatcher.rs#L138) so that the solution is easier to input at the command line. 

There is a bug in the crackme code that makes the loop run 1 extra time. The algorithm will do 1 iteration worth of work at the current work location, evaluate whether the current work pointer is equal to the end of the string, *Then* add 4 to the current work pointer. This means the algorithm will do work for 4 bytes after the end of the string. This is technically undefined behavior as the data after the string could be anything in a real program. In this little crackme though, the data will be the same each time.  
  
Luckily for our solving, we don't have to think about this problem. When the string is placed on the stack, it is the only thing on the stack and there is a 255 byte buffer of 0's at the top of the stack. This means the hashing algorithm overflows into a buffer that is all 0's. This also only works because this algorithm does not change the accumulator when processing a 0x00. The value stays the same. 0x00's not changing the result of the hashing algorithm may be intentional by the challenge author since the input buffer is always 0x20 bytes long and the loop will always run 0x20 times despite the length of the users input. So the algorithm may just not change the hash on 0x00 bytes to cheaply accommodate shorter inputs. However, that algorithm fact along with the general program structure of only having the input string on the stack at the time of processing works out perfectly and we can just ignore the problem!  
The challenge VM would fail randomly if the data after the input was non-deterministic. If the data was deterministic and the symbex vm had to factor for that, i would have to discover what those values were statically add that as concrete values. Symbolizing those 4 bytes then solving for them may work if there are a small number of inputs that satisfy the constraints of the hashing algorithm. Though having run this program many times, i think there would be too many inputs which satisfy the algorithm to just guess and check.

</td>
<td width="50%">

```rust
0x5A: R.MOV.RI(0xC5), reg:A, imm:0x2
0x5F: R.MOV.RI(0xCD), reg:B, imm:0x20D8
0x64: R.MOV.RI(0xD5), reg:C, imm:0x1E
0x69: R.SYSCALL(0x1), fputs(0x2), Challenge 2 - Tell me a joke: 
0x6A: R.SUB.SI(0x31), imm:0x20
0x70: R.MOV.RS(0xCE), reg:B
0x71: R.PUSH.R(0x12), reg:B
0x72: R.MOV.RI(0xD5), reg:C, imm:0x20
0x77: R.MOV.RI(0xC5), reg:A, imm:0x1
0x7C: R.SYSCALL(0x1), fgetc(0x1)
0x7D: R.POP.R(0x15), reg:A
0x7E: R.MOV.RI(0xCD), reg:B, imm:0x20
0x83: R.CALL(0x60), 0x11C
0x11C: R.MOV.RR(0xD0), reg2:C, reg1:A
0x11D: R.ADD.RR(0x20), reg1:A, reg2:B
0x11F: R.PUSH.R(0x11), reg:A
0x120: R.MOV.RI(0xCD), reg:B, imm:0x0
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x137: R.ADD.RI(0x21), reg:C, imm:0x4
0x13D: R.JMP(0x68), 0x125
// 7 loops removed for readability
0x125: R.MOV.RR(0xDA), reg2:D, reg1:C dereferenced
0x127: R.MUL.RI(0x51), reg:D, imm:0xCAFEBABE
0x12D: R.XOR.RR(0x40), reg1:B, reg2:D
0x12F: R.POP.R(0x15), reg:A
0x130: R.PUSH.R(0x11), reg:A
0x131: R.CMP.RR(0x72), reg1:C, reg2:A
0x132: R.JE(0x62), 0x142
0x142: R.MOV.RR(0xC1), reg2:A, reg1:B
0x143: R.RET(0x61), 
0x88: R.CMP.RI(0x80), reg:A, imm:0x7331
Solution found: input_2_b0, 0x46
Solution found: input_2_b1, 0x91
Solution found: input_2_b2, 0x0
Solution found: input_2_b3, 0x0
0x8D: R.JNE(0x63), 0x10B
```

</td>
</td>
</table>
# What Would Be Next

 A more automated system may have the reverser specifying the success and failure locations and letting some pre-emulation analysis engine build a tree which could specify the required condition at each conditional jump. Then when we reach that point, the dispatcher

 use z3 vars instead of SymVars