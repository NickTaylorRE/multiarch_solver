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

I initially sought to understand the VM architecture and build a small disassembler. Once i was done with the disassembler i thought i was set up pretty well to build a symbolic execution VM that i could use to solve the challenges somewhat automatically rather than trying to reimplement the algorithms in another language. This was not the most efficient nor quickest way to solve the challenge and not where i would likely spend my time during a CTF. But the CTF was already closed when i discovered it and this method was fun.
  

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
    uint32_t A;                     // A register
    uint32_t B;                     // B register
    uint32_t C;                     // C register
    uint32_t D;                     // D register
};
```
*vm_context structure*

Each region is mapped RWX and through individual calls to mmap. The size of each section is hardcoded to 0x1000 so this VM cannot handle large programs.
so the total ranges for each section is
  1. text = 0x1000 - 0x1fff
  2. data = 0x2000 - 0x2fff
  3. stack = 0x8000 - 0x8fff
  
  
# Stack VM
The stack VM instruction set is very simple and easy to parse. Each instruction is 5 bytes with just 1 byte defining the opcode and a 4 byte operand. Each instruction is 5 bytes even if the operands don't require a full 4 bytes or the operation has no usable operands. The stack VM also implements a very standard stack machine where an operation such as an `ADD` would pop two values off the stack, add them, then place the result back onto the stack.

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

The only weirdness in the stack machine is that some operations support pushing `Byte`(8b), `Word`(16b), or `Pointer`(32b) sized data to the stack. This means a program could have 2 `LDW` instructions followed by a `POPP`. The result would be 2 word sized immediates loaded to the stack, then both poped off as 1 pointer sized value. 1 `PUSHP` followed by 4 `POPB` instructions is also possible. Something like this may even just be an optimization, where the bytes aren't guaranteed to be used together.


# Reg VM
The reg VM instruction set is much more complicated, with some inconsistent encodings between the different types of instructions. The instructions are also not a consistent size in the encoding like the stack vm. I represent instructions as ranges in the below descriptions, but not all values in a range are supported. The ranges are just my best effort to simplify the explanations.  
  
dereference modifier:
Anything in the `0xA0 - 0xAf` range is an instruction modifier. The only part of this range that is actually used is the `0xA1 - 0xA3` and `0xA5 - 0xA7` ranges. Any value in these ranges tells the decoder that the first parameter needs to be dereferenced. The value itself is not used in the operation.  
  
`MOV`:  
The whole `0xc0 - 0xff` range (>0b11000000) is reserved for  `MOV` operation  
The graphic below shows how each bit is used in the `MOV` opcode.  
  
  * 7 - `MOV` operation
  * 6 - `MOV` operation
  * 5 - 2nd parameter type. 0 for register, 1 for pointer
  * 3 and 4 - offset into register table. unused for pointer
  * 2 - 1st parameter type. 0 for register, 1 for pointer, immediate, or stack pointer
  * 0 and 1 - offset into register table. 00 for pointer, 01 for immediate, 10 for stack pointer
  
The 2nd parameter is easy since it can only be a register or a pointer. The 5th bit specifies register vs pointer, and the 3rd and 4th bits are used to reference which register.  
The 1st parameter can be a register, pointer, immediate, or stack pointer. The 3rd bit is set to 0 for a register, and 1 for the other 3 options. When its a register, the 0th and 1st bits are used specify which register. otherwise those bits are used to further specify whether the 1st parameter is a pointer, immediate, or stack pointer. The value for the operand for the pointer and immediate operand types are pulled from next 4 bytes in the instruction.
  
`PUSH.R`, `POP.R`:  
The decoding of `PUSH` and `POP` instructions are kind of crazy and depend on interger underflow to correctly decode, but the ranges are still pretty clean.  
  
  * `0x10 - 0x14` for `PUSH` instructions
  * `0x15 - 0x18` for `POP` instructions
  
`0x10` specifies a `PUSH.I` push immediate instruction where the immediate is the next 4 bytes in the instruction.
`0x11 - 0x14` is the range for `PUSH.R` push register instruction where the lower bits in the encoding are used to specify which register to push from.  
`0x15 - 0x18` specifies a `POP.R` pop register instruction where the lower bits in the encoding are used to specify which register to pop to.  

Other:  
  * `0x0 - 0x0f` - Syscalls
  * `0x70 - 0x7f` - `CMP.RR` operation. The register operands are encoded in the lower 4 bits similar to the MOV operation
  * `0x80 - 0x8f` - `CMP.RI` operation. The immediate is stored in the rest of the instruction and the register is stored in the lower bits of the opcode like the other instructions.
  * `0x60 - 0x6f` - Control flow operations like `CALL`, `RET`, `JMP`, `JE`, and `JNE`. Operands taken from the stack or from the rest of the instruction
  
Math instructions:  
The math instructions are specifically different because don't encode their register operands in the opcode. The instruction includes another byte which encodes the registers for the first and second operand. Each instruction encodes its operands different in this operand byte.
  * `0x20 - 0x21` - `ADD.RR` and `ADD.RI`
  * `0x30 - 0x31` - `SUB.RR` and `SUB.RI`
  * `0x40 - 0x41` - `XOR.RR` and `XOR.RI`
  * `0x50 - 0x51` - `MUL.RR` and `MUL.RI`

The `ADD.RR` instruction and some others decode their register operands like `((((reg_param >> 4) - 1) & 3) << 2)` and `(((reg_param - 1) & 3) << 2)`. The resulting value is added to the location of the `A` register. The `& 3` bitmasking forces the value to be small enough that we don't overflow the pointer past the register offsets. There is more in the `vm_context` than i have shown above. The instantiation of this struct is 0x88 in size. The rest of the structure appears to be related to tracking heap allocations.  

`ADD.RI`, `XOR.RR`, and some registers in other instructions decode their registers like `(((reg_param >> 4) - 1) << 2)` and `(((reg_param & 0xf) - 1) << 2)`. Missing the `& 3` bitmasking means its possible to corrupt the heap tracking values in the `vm_context` structure.

The `SUB.RR`, `SUB.RI` and `XOR.RI` instructions all decode registers like `(((((reg_param >> 4) - 1) + 0xc) << 2) + 0xb)` where the resulting value is added to the base of the vm_context struct.  
`SUB.RI` ensures the value calculated by `(reg_param >> 4) - 1` is <= 3. So this instruction ensures no corruption, but this is the only one.

Others have written about exploiting this bug in `XOR.RI` to corrupt the heap tracking data to solve the pwn challenge `multiarch-2`. This is beyond the scope of this project though.  
  
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
  - Parses out which decoder should be used. 
  
stack_decoder [stack_decoder](solver/src/stack_decoder.rs)
  - Matches the instruction opcode against defined instructions. 
  - Format a mnemonic and argument(disassembling) for the reverser to understand the program. 
  - Call a symbex_vm function to update the state of the symbex_vm accordingly.

reg_decoder [reg_decoder](solver/src/reg_decoder.rs)
  - Parses the instruction opcode against defined instructions. 
  - Format a mnemonic and argument(disassembling) for the reverser to understand the program. 
  - Call a symbex_vm function to update the state of the symbex_vm accordingly.

symbex_vm [symbex_vm](symbex/src/symbex_vm.rs)
  - Performs the actual operations and updates the context.
  - Mostly a wrapper around the symbex engine with functions named after the operations found in the disassembly for convenience in writing the disassembler.
  - Manages the context.

symbex_engine [symbex_engine](symbex/src/symbex_engine.rs)
  - Exposes the primitive SymVar symbolic variable which represents a single byte. 
  - Exposes the SymVarVec which is a vector of SymVars. This acts as a vector of bytes effectively.
  - Provides instantiation, arithmetic operations, bitwise operations, and equality operations for SymVar and SymVarVec.
  - Provides conversion to z3 BitVectors and solving with Z3 for SymVars with and without rand brute forcing.
  
  
The Symbex VM has 2 operating modes. A straight linear disassembly, and an emulation mode. The emulation mode actually performs symbolic execution, solving, and produces an instruction trace rather than a linear disassembly. this is good enough for this challenge because there are no gigantic loops, but would likely need to be changed for more complex algorithms.
The emulation mode also provides dereferencing of strings and syscall numbers since it has the data available when decoding the instruction. the linear disassembly just disassembles with no attempt to help the reverser see strings or syscall numbers.
  

<table>
<tr valign="top">
<td width="50%">

***linear disassembly***
```rust
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
0x0: S.LDB(0x10), 75(0x4B)
0x5: S.PUSHP(0x30), 8192(0x2000)
0xA: S.LDB(0x10), 2(0x2)
0xF: S.SYSCALL(0xA0), fputs, 
Welcome to the multiarch of madness! 
Let's see how well you understand it.
(0x0)
0x14: S.LDB(0x10), 43(0x2B)
0x19: S.PUSHP(0x30), 8365(0x20AD)
0x1E: S.LDB(0x10), 2(0x2)
0x23: S.SYSCALL(0xA0), fputs, 
Challenge 1 - What's your favorite number? (0x0)
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
0x69: R.SYSCALL(0x1), fputs(0x2), 
Challenge 2 - Tell me a joke: 
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
0x92: R.MOV.RI(0xC5), reg:A, imm:0x0
0x97: S.LDB(0x10), 90(0x5A)
0x9C: S.PUSHP(0x30), 8438(0x20F6)
0xA1: S.LDB(0x10), 2(0x2)
0xA6: S.SYSCALL(0xA0), fputs, 
Challenge 3 - Almost there! 
But can you predict the future?
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

Challenge 1 is a simple equation so it is a good first test for the symbex engine.  
```rust
0xAAAAAAAA = input + (0x13370539 ^ 0x08675309)
```
Of course for the first challenge there was a lot of work getting the operations working correctly and tested. Aside from that, the biggest challenge was trying to support the sized load operations `LDB`, `LDW`, `LDP` and their affect on the stack. Before tackling this challenge only the `SymVar` existed which was pointer sized including each stack location. I considered leaving everything pointer sized and annotating the data from the load operation to then check the data size when `POPP`ing, but decided that was going to be messy and not a good foundation for future work. I decided to build the SymVarVec to emulate the stack more correctly. This meant i had to create conversion methods between a vector of symbolic bytes and symbolic pointer sized data so that the math and bitwise operations carried and overflowed correctly.  
  
Second, i had to decide 
 * How i was going to solve in the middle of an emulation.
 * What degree of automated i wanted the symbex_vm to be. 
  
I settled on a pseudo breakpoint system [breakpoints](solver/src/dispatcher.rs#L129) which simply performs some operation before an instruction executes. This is not completely automated, but does give me everything i need to just specify a point at which solving should be done and just do that. More on this in the What Would Be Next section.  
  
For the purpose of solving for the challenge conditions, each conditional jump which may lead to the failure condition(`0x10b`) first calls the `CMPP` operation to set the flags register before calling the conditional jump. In the challenge VM, this flags register appears to store whether the subtract operation(typical compare operation) had an  equal result `0b001`, overflowed negative `0b110`, or was unequal and not negative `0b100`. The symbex VM only supports an `==` or `!=` result with no regard for negatives because it doesn't appear to be used in the crackme.  
  
Solving after the `CMPP` operation was as simple as calling the [SymVar::try_solve()](symbex/src/symbex_engine.rs#L62) method on the `SymVar` stored in the [SymbolicContext.flags](solver/src/dispatcher.rs#L131) register. `try_solve` simply converts the `SymVar` variables to z3 variables, assigns the constraints, then lets z3 solve and get a model which satisfies the constraints.  
  
The output shows each byte value individually. Inputing this value back into the challenge would be as simple as concattenating the bytes Big Endian and converting to decimal to be entered in the command line.
  
</td>
<td width="50%">

***Challenge 1 code***
```rust
0x0: S.LDB(0x10), 75(0x4B)
0x5: S.PUSHP(0x30), 8192(0x2000)
0xA: S.LDB(0x10), 2(0x2)
0xF: S.SYSCALL(0xA0), fputs,
Welcome to the multiarch of madness!
Let's see how well you understand it.
(0x0)
0x14: S.LDB(0x10), 43(0x2B)
0x19: S.PUSHP(0x30), 8365(0x20AD)
0x1E: S.LDB(0x10), 2(0x2)
0x23: S.SYSCALL(0xA0), fputs,
Challenge 1 -
What's your favorite number? (0x0)
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

Challenge 2 was a slightly more complicated hashing algorithm. Luckily the hashing loop only runs 9 times. Symbolic execution engines unroll loops into the expression tree, so large or unbounded loops can cause expression blowup. This challenge accepts an up to 0x20 byte string at the command line, hashes each byte of that string 4 bytes at a time, then compares the result to a hardcoded result. Below is rust code showing the algorithm.  
```rust
accumulator = 0;
for i in (0..0x20).step_by(4) {
    val = (0xCAFEBABE * input[i..i+4]);
    val = val >> 0x20;
    accumulator = val ^ accumulator;
}
if(xor_result==0x7331) {
    // correct
}
```
The example above uses an iterator for the loop but the crackme code uses pointer arithmetic on the current position in the string and the end of the string as the condition to exit the loop. There is also code to push the length of the string to the stack and pop it later to be used as the length of the string. All of this required me to structure the `SymbolicContext` stack in the same or similar way that the challenge VM creates its stack.  

In short, i have a [SymbolicContext.stack](symbex/src/symbex_engine.rs#L26) that gets instantiated to a large size. Most of the values will be 0 and unused. Then there is the [SymbolicContext.sp](symbex/src/symbex_engine.rs#L32) that gets instantiated to the same value that the stack pointer in the challenge VM gets instantiated to. All of this allows pointer arithmetic and proper stack growth. I was able to solve the first challenge with a stack that was just a 0 length `SymVarVec` that grew up rather than down, and pushed and popped values as needed. This challenge required me to remove pushing and popping and abstract those operations through the [SymVarVec::assign](symbex/src/symbex_engine.rs#L747) operation. So the symbex vm doesn't push and pop to the Vec like Vec.push() supports, it now assigns values up to a specified length to the prebuilt stack, and decrements the stack pointer.  
  
Once i had this stack issue figured out and ensured the instructions performed properly, the challenge solved without issue.

</td>
<td width="50%">

***Challenge 2 code***
```rust
0x5A: R.MOV.RI(0xC5), reg:A, imm:0x2
0x5F: R.MOV.RI(0xCD), reg:B, imm:0x20D8
0x64: R.MOV.RI(0xD5), reg:C, imm:0x1E
0x69: R.SYSCALL(0x1), fputs(0x2),
 Challenge 2 - Tell me a joke: 
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
Solution found: input_2_b0, 0xA8
Solution found: input_2_b1, 0x51
Solution found: input_2_b10, 0x48
Solution found: input_2_b11, 0x59
Solution found: input_2_b12, 0x70
Solution found: input_2_b13, 0x5
Solution found: input_2_b14, 0x0
Solution found: input_2_b15, 0xA0
Solution found: input_2_b16, 0xFA
Solution found: input_2_b17, 0x39
Solution found: input_2_b18, 0x57
Solution found: input_2_b19, 0x6
Solution found: input_2_b2, 0x3C
Solution found: input_2_b20, 0x3C
Solution found: input_2_b21, 0x19
Solution found: input_2_b22, 0x0
Solution found: input_2_b23, 0x1F
Solution found: input_2_b24, 0x36
Solution found: input_2_b25, 0x86
Solution found: input_2_b26, 0x22
Solution found: input_2_b27, 0xF2
Solution found: input_2_b28, 0x8F
Solution found: input_2_b29, 0x8F
Solution found: input_2_b3, 0xC3
Solution found: input_2_b30, 0x3A
Solution found: input_2_b31, 0xEA
Solution found: input_2_b4, 0xE4
Solution found: input_2_b5, 0x23
Solution found: input_2_b6, 0xE7
Solution found: input_2_b7, 0xFA
Solution found: input_2_b8, 0x3D
Solution found: input_2_b9, 0xB8
0x8D: R.JNE(0x63), 0x10B
```

</td>
</td>
</table>

### Off by 1 bug
There is a bug in the second challenge of the crackme code that makes the hashing loop run 1 extra time. The algorithm will do 1 iteration worth of work at the current work location, evaluate whether the current work pointer is equal to the end of the string, *Then* add 4 to the current work pointer. This means the algorithm will do work for 4 bytes after the end of the string. This is technically undefined behavior as the data after the string could be anything in a real program. In this little crackme though, the data will be the same each time.  
  
Luckily for our solving, we don't have to think about this problem. When the string is placed on the stack, it is the only thing on the stack and there is a 255 byte buffer of 0's at the top of the stack. This means the hashing algorithm overflows into a buffer that is all 0's. This also only works because this algorithm does not change the accumulator when processing a 0x00. The value stays the same. 0x00's not changing the result of the hashing algorithm may be intentional by the challenge author since the input buffer is always 0x20 bytes long and the loop will always run 0x20 times despite the length of the users input. So the algorithm may just not change the hash on 0x00 bytes to cheaply accommodate shorter inputs. However, that algorithm fact along with the general program structure of only having the input string on the stack at the time of processing works out perfectly and we can just ignore the problem!  
  
The challenge VM would fail randomly if the data after the input was non-deterministic. If the data was deterministic and the symbex vm had to factor for that, i would have to discover what those values were statically and add that as concrete values to the expression before solving. Symbolizing those 4 bytes then solving for them may work if there are a small number of inputs that satisfy the constraints of the hashing algorithm. Though having run this program many times, i think there would be too many inputs which satisfy the algorithm to just guess and check.
  
# Challenge 3
<table>
<tr valign="top">
<td width="50%">
Challenge 3 would be a simple algorithm if not for the rand calls. Below is the code showing the algorithm.

```rust
srand(input)
for i in 0..10 {
    let rand1 = rand() & 0xffff;
    let rand2 = rand() << 0x10;
    let fullrand = rand1 | rand2;
    let value = 
      (fullrand ^ 0x133700 ^ 0xf2f2f2f2)
        & 0xffffff
    if value == 0xc0ffee {
      //correct
    }
}
```

Default rand uses `TYPE_3` random number generator which is a modmul based algorithm. The source for the seed algorithm in glibc is available [here](https://elixir.bootlin.com/glibc/glibc-2.42.9000/source/stdlib/random_r.c#L176).  
Trying to build the seed algorithm into the symbex engine proved completely unsuccessful as the rust symbex engine isn't the most efficient. Encoding the problem into a z3 expression worked but z3 was unable to solve for my test cases. Bitwuzla was also unable to solve the problem.
  
I settled on brute forcing any rand calls [SymVar::solve_with_rand](symbex/src/symbex_engine.rs#L211). This increases the complexity of solving by a lot but does effectively break down our problem into solvable parts.  
The engine needs to 
  1. Encode rand calls in the symbolic expression preserving the order of the calls. [Rand](symbex/src/symbex_engine.rs#L405)
  2. Extract rand calls at solve time to brute force, splitting up the expression. [rand_inners](symbex/src/symbex_engine.rs#L219)
  3. Enter a loop where the engine guesses a seed, gets rand values, updates the symbolic rands with the guessed concrete ones, and attempts to solve. [brute force loop](symbex/src/symbex_engine.rs#L264)
  4. Once a solution has been found for the outer expression, the engine needs to make sure the inner expression (inside the srand call) solves with the guessed srand value. [inner solve](symbex/src/symbex_engine.rs#L285)
  5. If it does, we're done. otherwise we need to continue brute forcing.
  
Brute forcing is of course very slow so [multithreading](symbex/src/symbex_engine.rs#L229) the issue becomes necessary. With 14 threads, this stage of the challenge takes about 30 minutes to solve. Could be worse overall.  
  
The current implementation works well for this specific challenge where there is only 1 seed, but isn't set up for more complex expresions.  
If there was more symbolic seeds like `srand(x) + srand(y) == 0xcoffee` then the engine would need to recursively brute force which would take a very long time.  
If there was multiple layers of inner expressions like `srand(srand(srand(x)+5)+10) == 0xcoffee` the engine would have to solve then we would have to rebuild the rand solving function to solve for each nested rand call before claiming to have found a full solution.  
Any combination of these expressions blows up the problem and solve time exponentially. Trying to cover these is out of scope for this toy solver for this 1 specific CTF challenge. However the concept is there for more complex solves if we had a use case for it.  
  
</td>
<td width="50%">

***Challenge 3 code***
```rust
0x92: R.MOV.RI(0xC5), reg:A, imm:0x0
0x97: S.LDB(0x10), 90(0x5A)
0x9C: S.PUSHP(0x30), 8438(0x20F6)
0xA1: S.LDB(0x10), 2(0x2)
0xA6: S.SYSCALL(0xA0), fputs,
Challenge 3 - 
Almost there! But can you predict the future?
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
// execution ended for convenience.
```

</td>
</td>
</table>

# What Would Be Next

If i had to continue this project i would start by trying to replace the SymVar's with z3's bitvecs. Having the higher level abstraction lets me convert to other symbex engines but the rust implementation is considerably slower and uses more ram. using z3's data types is way more efficient in its own and allows me to use z3's expression simplification.  
  
I would also like to see a more automated system may have the reverser specifying the success and failure locations and letting some pre-emulation analysis engine build a tree which could specify the required condition at each conditional jump to reach the success location. At each conditional jump, the tool would attempt to solve and take the jump. There is also some value in keeping the whole tool somewhat modular with the current breakpoint system.
  
Not all the instructions are implemented perfectly especially in the reg_decoder. I make the assumption that all pointers go to the stack but there appears to be heap support as well that the crackme just doesn't use. Some instructions are not fully implemented because the crackme simply doesn't use them.
