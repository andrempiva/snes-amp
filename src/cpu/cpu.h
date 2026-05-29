#pragma once

#include <stdint.h>

#include "cpu/addressing_modes.h"
#include "cpu/instructions.h"
#include "cpu/flags.h"

typedef struct AddressingMode AddressingMode;
typedef struct CPUStack CPUStack;

typedef struct CPU {
    void* memory;
    size_t cycle;
    uint8_t extraCycles;

    uint16_t a;
    uint16_t x;
    uint16_t y;
    uint16_t pc;
    uint16_t sp;
    uint8_t p;
    Flags* flags;
    uint16_t d;

    uint8_t db;
    uint8_t pb;

    CPUStack* stack;
} CPU;

CPU cpu_create(void);
void cpu_destroy(CPU *cpu);


// ## CPU Memory and Register Transfers

// Register to Register Transfer

//   Opcode    Flags Clks  Native  Nocash    Bits  Effect
//   A8        nz----  2   TAY     MOV Y,A   x     Y=A
//   AA        nz----  2   TAX     MOV X,A   x     X=A
//   BA        nz----  2   TSX     MOV X,S   x     X=S
//   98        nz----  2   TYA     MOV A,Y   m     A=Y
//   8A        nz----  2   TXA     MOV A,X   m     A=X
//   9A        ------  2   TXS     MOV S,X   e     S=X
//   9B        nz----  2   TXY     MOV Y,X   x     Y=X
//   BB        nz----  2   TYX     MOV X,Y   x     X=Y
//   7B        nz----  2   TDC     MOV A,D   16    A=D
//   5B        nz----  2   TCD     MOV D,A   16    D=A
//   3B        nz----  2   TSC     MOV A,SP  16    A=SP
//   1B        ------  2   TCS     MOV SP,A  e?    SP=A

// Load Register from Memory

//   Opcode      Flags Clks  Native       Nocash         Bits  Effect
//   A9 nn       nz----  2   LDA #nn      MOV A,nn          A=nn
//   A5 nn       nz----  3   LDA nn       MOV A,[nn]        A=[D+nn]
//   B5 nn       nz----  4   LDA nn,X     MOV A,[nn+X]      A=[D+nn+X]
//   A3 nn       nz----      LDA nn,S     MOV A,[nn+S]      A=[nn+S]
//   AD nn nn    nz----  4   LDA nnnn     MOV A,[nnnn]      A=[DB:nnnn]
//   BD nn nn    nz----  4*  LDA nnnn,X   MOV A,[nnnn+X]    A=[DB:nnnn+X]
//   B9 nn nn    nz----  4*  LDA nnnn,Y   MOV A,[nnnn+Y]    A=[DB:nnnn+Y]
//   AF nn nn nn nz----      LDA nnnnnn   MOV A,[nnnnnn]    A=[nnnnnn]
//   BF nn nn nn nz----      LDA nnnnnn,X MOV A,[nnnnnn+X]  A=[nnnnnn+X]
//   B2 nn       nz----      LDA (nn)     MOV A,[[nn]]      A=[WORD[D+nn]]
//   A1 nn       nz----  6   LDA (nn,X)   MOV A,[[nn+X]]    A=[WORD[D+nn+X]]
//   B1 nn       nz----  5*  LDA (nn),Y   MOV A,[[nn]+Y]    A=[WORD[D+nn]+Y]
//   B3 nn       nz----      LDA (nn,S),Y MOV A,[[nn+S]+Y]  A=[WORD[nn+S]+Y]
//   A7 nn       nz----      LDA [nn]     MOV A,[FAR[nn]]   A=[FAR[D+nn]]
//   B7 nn       nz----      LDA [nn],y   MOV A,[FAR[nn]+Y] A=[FAR[D+nn]+Y]
//   A2 nn       nz----  2   LDX #nn      MOV X,nn          X=nn
//   A6 nn       nz----  3   LDX nn       MOV X,[nn]        X=[D+nn]
//   B6 nn       nz----  4   LDX nn,Y     MOV X,[nn+Y]      X=[D+nn+Y]
//   AE nn nn    nz----  4   LDX nnnn     MOV X,[nnnn]      X=[DB:nnnn]
//   BE nn nn    nz----  4*  LDX nnnn,Y   MOV X,[nnnn+Y]    X=[DB:nnnn+Y]
//   A0 nn       nz----  2   LDY #nn      MOV Y,nn          Y=nn
//   A4 nn       nz----  3   LDY nn       MOV Y,[nn]        Y=[D+nn]
//   B4 nn       nz----  4   LDY nn,X     MOV Y,[nn+X]      Y=[D+nn+X]
//   AC nn nn    nz----  4   LDY nnnn     MOV Y,[nnnn]      Y=[DB:nnnn]
//   BC nn nn    nz----  4*  LDY nnnn,X   MOV Y,[nnnn+X]    Y=[DB:nnnn+X]

// * Add one cycle if indexing crosses a page boundary.

// Store Register in Memory

//   Opcode      Flags Clks  Native       Nocash         Bits  Effect
//   64 nn       ------  3   STZ nn       MOV [nn],0        m  [D+nn]=0
//   74 nn       ------  4   STZ nn_x     MOV [nn+X],0      m  [D+nn+X]=0
//   9C nn nn    ------  4   STZ nnnn     MOV [nnnn],0      m  [DB:nnnn]=0
//   9E nn nn    ------  5   STZ nnnn_x   MOV [nnnn+X],0    m  [DB:nnnn+X]=0
//   85 nn       ------  3   STA nn       MOV [nn],A        m  [D+nn]=A
//   95 nn       ------  4   STA nn,X     MOV [nn+X],A      m  [D+nn+X]=A
//   83 nn       ------      STA nn,S     MOV [nn+S],A      m  [nn+S]=A
//   8D nn nn    ------  4   STA nnnn     MOV [nnnn],A      m  [DB:nnnn]=A
//   9D nn nn    ------  5   STA nnnn,X   MOV [nnnn+X],A    m  [DB:nnnn+X]=A
//   99 nn nn    ------  5   STA nnnn,Y   MOV [nnnn+Y],A    m  [DB:nnnn+Y]=A
//   8F nn nn nn ------      STA nnnnnn   MOV [nnnnnn],A    m  [nnnnnn]=A
//   9F nn nn nn ------      STA nnnnnn,X MOV [nnnnnn+X],A  m  [nnnnnn+X]=A
//   81 nn       ------  6   STA (nn,X)   MOV [[nn+X]],A    m  [WORD[D+nn+X]]=A
//   91 nn       ------  6   STA (nn),Y   MOV [[nn]+Y],A    m  [WORD[D+nn]+Y]=A
//   92 nn       ------      STA (nn)     MOV [[nn]],A      m  [WORD[D+nn]]=A
//   93 nn       ------      STA (nn,S),Y MOV [[nn+S]+Y],A  m  [WORD[nn+S]+Y]=A
//   87 nn       ------      STA [nn]     MOV [FAR[nn]],A   m  [FAR[D+nn]]=A
//   97 nn       ------      STA [nn],y   MOV [FAR[nn]+Y],A m  [FAR[D+nn]+Y]=A
//   86 nn       ------  3   STX nn       MOV [nn],X        x  [D+nn]=X
//   96 nn       ------  4   STX nn,Y     MOV [nn+Y],X      x  [D+nn+Y]=X
//   8E nn nn    ------  4   STX nnnn     MOV [nnnn],X      x  [DB:nnnn]=X
//   84 nn       ------  3   STY nn       MOV [nn],Y        x  [D+nn]=Y
//   94 nn       ------  4   STY nn,X     MOV [nn+X],Y      x  [D+nn+X]=Y
//   8C nn nn    ------  4   STY nnnn     MOV [nnnn],Y      x  [DB:nnnn]=Y


// Push/Pull (Stack)

//   Opcode    Flags Clks  Native    Nocash       Bits  Effect
//   48        ------  3   PHA       PUSH A          m  [S]=A
//   DA        ------  3   PHX       PUSH X          x  [S]=X
//   5A        ------  3   PHY       PUSH Y          x  [S]=Y
//   08        ------  3   PHP       PUSH P          8  [S]=P
//   8B        ------  3   PHB       PUSH DB         8  [S]=DB
//   4B        ------  3   PHK       PUSH PB         8  [S]=PB
//   0B        ------  4   PHD       PUSH D         16  [S]=D
//   D4 nn     ------  6   PEI nn    PUSH WORD[nn]  16  [S]=WORD[D+nn]
//   F4 nn nn  ------  5   PEA nnnn  PUSH nnnn      16  [S]=NNNN
//   62 nn nn  ------  6   PER rel16 PUSH disp16    16  [S]=$+3+disp
//   68        nz----  4   PLA       POP  A          m  A=[S]
//   FA        nz----  4   PLX       POP  X          x  X=[S]
//   7A        nz----  4   PLY       POP  Y          x  Y=[S]
//   2B        nz----  5   PLD       POP  D         16  D=[S]
//   AB        nz----  4   PLB       POP  DB         8  DB=[S]
//   28        nzcidv  4   PLP       POP  P          8  P=[S]

// Notes: PLA sets Z and N according to content of A. The B-flag and unused flags cannot be changed by PLP, these flags are always written as "1" by PHP.

// Memory Block Transfer Commands

//   Opcode    Flags Clks  Native      Nocash                  ;Notes
//   44 dd ss  ------  7x  MVP ss,dd   LDDR [dd:Y],[ss:X],A+1  ;DEC X/Y
//   54 dd ss  ------  7x  MVN ss,dd   LDIR [dd:Y],[ss:X],A+1  ;INC X/Y

// Sets DB=dstbank (!), copies one byte from [srcbank:X] to [dstbank:Y], increments or decrements X and Y (8bit or 16bit, depending on X and E flags), decrements A (all 16bits, no matter of M and E flags), continues at next opcode when resulting A=FFFFh, otherwise sets PC=PC-3 (thus repeats the transfer opcode).
// Note: The native opcode names (MVP/MVN, Move Positive/Negative) are chosen so that "positive" refers to transfers from "to higher" memory addresses (which would matter on overlapping source/dest blocks), accordingly, the practical meaning is that "positive" means to DECREASE the X,Y registers.


//   CPU Arithmetic/Logical Operations

// ALU Opcodes

//   Base  Flags    Native   Nocash     Operands    Name      Function
//   00    nz----   ORA op   OR  A,op   <alu_types> OR        A=A OR op
//   20    nz----   AND op   AND A,op   <alu_types> AND       A=A AND op
//   40    nz----   EOR op   XOR A,op   <alu_types> XOR       A=A XOR op
//   60    nzc--v   ADC op   ADC A,op   <alu_types> Add       A=A+C+op
//   E0    nzc--v   SBC op   SBC A,op   <alu_types> Subtract  A=A+C-1-op
//   C0    nzc---   CMP op   CMP A,op   <alu_types> Compare   A-op
//   E0    nzc---   CPX op   CMP X,op   <cpx_types> Compare   X-op
//   C0    nzc---   CPY op   CMP Y,op   <cpx_types> Compare   Y-op


// alu_types (Operands for OR,AND,XOR,ADC,SBC,CMP Opcodes)

//   Opcode        Clks  Native   Nocash     Name          Effect
//   Base+09 nn      2   #nn      nn         Immediate     nn
//   Base+05 nn      3   nn       [nn]       Zero Page     [D+nn]
//   Base+15 nn      4   nn,X     [nn+X]     Zero Page,X   [D+nn+X]
//   Base+0D nn nn   4   nnnn     [nnnn]     Absolute      [DB:nnnn]
//   Base+1D nn nn   4*  nnnn,X   [nnnn+X]   Absolute,X    [DB:nnnn+X]
//   Base+19 nn nn   4*  nnnn,Y   [nnnn+Y]   Absolute,Y    [DB:nnnn+Y]
//   Base+01 nn      6   (nn,X)   [[nn+X]]   (Indirect,X)  [WORD[D+nn+X]]
//   Base+11 nn      5*  (nn),Y   [[nn]+Y]   (Indirect),Y  [WORD[D+nn]+Y]
//   Base+12 nn          (nn)     [[nn]]     (Indirect)    [WORD[D+nn]]
//   Base+03 nn          nn,S     [nn+S]                   [nn+S]
//   Base+13 nn          (nn,S),Y [[nn+S]+Y]               [WORD[nn+S]+Y]
//   Base+07 nn          [nn]     [FAR[nn]]                [FAR[D+nn]]
//   Base+17 nn          [nn],y   [FAR[nn]+Y]              [FAR[D+nn]+Y]
//   Base+0F nn nn nn    nnnnnn   [nnnnnn]                 [nnnnnn]
//   Base+1F nn nn nn    nnnnnn,X [nnnnnn+X]               [nnnnnn+X]

// * Add one cycle if indexing crosses a page boundary.

// cpx_types (Operands for CMP Opcodes with X,Y Operand)

//   Opcode        Clks  Native   Nocash     Name                Effect
//   Base+00 nn      2   #nn      nn         Immediate           nn
//   Base+04 nn      3   nn       [nn]       Zero Page           [D+nn]
//   Base+0C nn nn   4   nnnn     [nnnn]     Absolute            [DB:nnnn]


// Bit Test

//   Opcode    Flags Clks  Native     Nocash             Operand
//   24 nn     xz---x  3   BIT nn     TEST A,[nn]        [D+nn]
//   2C nn nn  xz---x  4   BIT nnnn   TEST A,[nnnn]      [DB:nnnn]
//   34 nn     xz---x      BIT nn,X   TEST A,[nn+X]      [D+nn+X]
//   3C nn nn  xz---x      BIT nnnn,X TEST A,[nnnn+X]    [DB:nnnn+X]
//   89 nn     -z----      BIT #nn    TEST A,nn          nn

// Flags are set as "Z=((A AND op)=0)", and "N=op.Bit(MSB)", and "V=op.Bit(MSB-1)". Where MSB=bit15/bit7, and MSB-1=bit14/bit6 (depending on M-flag). Note that N and V do rely only on "op" (ie. not on "A AND op").

// Increment by one

//   Opcode          Clks  Native     Nocash       Effect
//   E6 nn     nz----  5   INC nn     INC [nn]     [D+nn]=[D+nn]+1
//   F6 nn     nz----  6   INC nn,X   INC [nn+X]   [D+nn+X]=[D+nn+X]+1
//   EE nn nn  nz----  6   INC nnnn   INC [nnnn]   [DB:nnnn]=[DB:nnnn]+1
//   FE nn nn  nz----  7   INC nnnn,X INC [nnnn+X] [DB:nnnn+X]=[DB:nnnn+X]+1
//   E8        nz----  2   INX        INC X        X=X+1
//   C8        nz----  2   INY        INC Y        Y=Y+1
//   1A        nz----  2   INA        INC A        A=A+1


// Decrement by one

//   Opcode          Clks  Native     Nocash       Effect
//   C6 nn     nz----  5   DEC nn     DEC [nn]     [D+nn]=[D+nn]-1
//   D6 nn     nz----  6   DEC nn,X   DEC [nn+X]   [D+nn+X]=[D+nn+X]-1
//   CE nn nn  nz----  6   DEC nnnn   DEC [nnnn]   [DB:nnnn]=[DB:nnnn]-1
//   DE nn nn  nz----  7   DEC nnnn,X DEC [nnnn+X] [DB:nnnn+X]=[DB:nnnn+X]-1
//   CA        nz----  2   DEX        DEC X        X=X-1
//   88        nz----  2   DEY        DEC Y        Y=Y-1
//   3A        nz----  2   DEA        DEC A        A=A-1


// TSB/TRB (Test and Set/Reset)

//   Opcode          Clks  Native      Nocash
//   04 nn     -z----  5   TSB nn      SET [nn],A    ;\"TEST op,A" --> z
//   0C nn nn  -z----  6   TSB nnnn    SET [nnnn],A  ;/then "OR op,A"
//   14 nn     -z----  5   TRB nn      CLR [nn],A    ;\"TEST op,A" --> z
//   1C nn nn  -z----  6   TRB nnnn    CLR [nnnn],A  ;/then "AND op,NOT A"



//   CPU Rotate and Shift Instructions

// Shift Left Logical/Arithmetic

//   Opcode          Clks  Native      Nocash        Effect
//   0A        nzc---  2   ASL A       SHL A         SHL A
//   06 nn     nzc---  5   ASL nn      SHL [nn]      SHL [D+nn]
//   16 nn     nzc---  6   ASL nn,X    SHL [nn+X]    SHL [D+nn+X]
//   0E nn nn  nzc---  6   ASL nnnn    SHL [nnnn]    SHL [DB:nnnn]
//   1E nn nn  nzc---  7   ASL nnnn,X  SHL [nnnn+X]  SHL [DB:nnnn+X]


// Shift Right Logical

//   4A        0zc---  2   LSR A       SHR A         SHR A
//   46 nn     0zc---  5   LSR nn      SHR [nn]      SHR [D+nn]
//   56 nn     0zc---  6   LSR nn,X    SHR [nn+X]    SHR [D+nn+X]
//   4E nn nn  0zc---  6   LSR nnnn    SHR [nnnn]    SHR [DB:nnnn]
//   5E nn nn  0zc---  7   LSR nnnn,X  SHR [nnnn+X]  SHR [DB:nnnn+X]


// Rotate Left through Carry

//   2A        nzc---  2   ROL A       RCL A         RCL A
//   26 nn     nzc---  5   ROL nn      RCL [nn]      RCL [D+nn]
//   36 nn     nzc---  6   ROL nn,X    RCL [nn+X]    RCL [D+nn+X]
//   2E nn nn  nzc---  6   ROL nnnn    RCL [nnnn]    RCL [DB:nnnn]
//   3E nn nn  nzc---  7   ROL nnnn,X  RCL [nnnn+X]  RCL [DB:nnnn+X]


// Rotate Right through Carry

//   6A        nzc---  2   ROR A       RCR A         RCR A
//   66 nn     nzc---  5   ROR nn      RCR [nn]      RCR [D+nn]
//   76 nn     nzc---  6   ROR nn,X    RCR [nn+X]    RCR [D+nn+X]
//   6E nn nn  nzc---  6   ROR nnnn    RCR [nnnn]    RCR [DB:nnnn]
//   7E nn nn  nzc---  7   ROR nnnn,X  RCR [nnnn+X]  RCR [DB:nnnn+X]


// Notes:
// ROR instruction is available on MCS650X microprocessors after June, 1976.
// ROL and ROR rotate an 8bit value through carry (rotates 9bits in total).


//   CPU Jump and Control Instructions

// Normal Jumps

//   Opcode       Flags Clks  Native       Nocash        Effect
//   80 dd        ------  3xx BRA disp8    JMP disp      PC=PC+/-disp8
//   82 dd dd     ------  4   BRL disp16   JMP disp      PC=PC+/-disp16
//   4C nn nn     ------  3   JMP nnnn     JMP nnnn      PC=nnnn
//   5C nn nn nn  ------  4   JMP nnnnnn   JMP nnnnnn    PB:PC=nnnnnn
//   6C nn nn     ------  5   JMP (nnnn)   JMP [nnnn]    PC=WORD[00:nnnn]
//   7C nn nn     ------  6   JMP (nnnn,X) JMP [nnnn+X]  PC=WORD[PB:nnnn+X]
//   DC nn nn     ------  6   JML ...      JMP FAR[nnnn] PB:PC=[00:nnnn]
//   20 nn nn     ------  6   JSR nnnn     CALL nnnn     [S]=PC+2,PC=nnnn
//   22 nn nn nn  ------  4   JSL nnnnnn   CALL nnnnnn   PB:PC=nnnnnn [S]=PB:PC+3
//   FC nn nn     ------  6   JSR (nnnn,X) CALL [nnnn+X] PC=WORD[PB:nnnn+X] [S]=PC
//   40           nzcidv  6   RTI          RETI          P=[S+1],PB:PC=[S+2],S=S+4
//   6B           ------  ?   RTL          RETF          PB:PC=[S+1]+1, S=S+3
//   60           ------  6   RTS          RET           PC=[S+1]+1, S=S+2

// Note: RTI cannot modify the B-Flag or the unused flag.
// Glitch: For JMP [nnnn] the operand word cannot cross page boundaries, ie. JMP [03FFh] would fetch the MSB from [0300h] instead of [0400h]. Very simple workaround would be to place a ALIGN 2 before the data word.

// Conditional Branches (Branch on condition, to PC=PC+/-nn)

//   Opcode    Flags Clks  Native  Nocash        Condition (jump if)
//   10 dd     ------  2** BPL     JNS     disp  ;N=0 (plus/positive)
//   30 dd     ------  2** BMI     JS      disp  ;N=1 (minus/negative/signed)
//   50 dd     ------  2** BVC     JNO     disp  ;V=0 (no overflow)
//   70 dd     ------  2** BVS     JO      disp  ;V=1 (overflow)
//   90 dd     ------  2** BCC/BLT JNC/JB  disp  ;C=0 (less/below/no carry)
//   B0 dd     ------  2** BCS/BGE JC/JAE  disp  ;C=1 (above/greater/equal/carry)
//   D0 dd     ------  2** BNE/BZC JNZ/JNE disp  ;Z=0 (not zero/not equal)
//   F0 dd     ------  2** BEQ/BZS JZ/JE   disp  ;Z=1 (zero/equal)

// ** The execution time is 2 cycles if the condition is false (no branch executed). Otherwise, 3 cycles if the destination is in the same memory page, or 4 cycles if it crosses a page boundary (see below for exact info).
// Note: After subtractions (SBC or CMP) carry=set indicates above-or-equal, unlike as for 80x86 and Z80 CPUs.

// Interrupts, Exceptions, Breakpoints

//   Opcode                                                      6502     65C816
//   00   BRK    Break      B=1 [S]=$+2,[S]=P,D=0 I=1, PB=00, PC=[00FFFE] [00FFE6]
//   02   COP    ;65C816    B=1 [S]=$+2,[S]=P,D=0 I=1, PB=00, PC=[00FFF4] [00FFE4]
//   --   /ABORT ;65C816                               PB=00, PC=[00FFF8] [00FFE8]
//   --   /IRQ   Interrupt  B=0 [S]=PC, [S]=P,D=0 I=1, PB=00, PC=[00FFFE] [00FFEE]
//   --   /NMI   NMI        B=0 [S]=PC, [S]=P,D=0 I=1, PB=00, PC=[00FFFA] [00FFEA]
//   --   /RESET Reset      D=0 E=1 I=1 D=0000, DB=00  PB=00, PC=[00FFFC] N/A

// Notes: IRQs can be disabled by setting the I-flag, a BRK command, a NMI, and a /RESET signal cannot be masked by setting I.
// Exceptions do first change the B-flag (in 6502 mode), then write P to stack, and then set the I-flag, the D-flag IS cleared (unlike as on original 6502).
// In 6502 mode, the same vector is shared for BRK and IRQ, software can separate between BRK and IRQ by examining the pushed B-flag only.
// The RTI opcode can be used to return from BRK/IRQ/NMI, note that using the return address from BRK skips one dummy/parameter byte following after the BRK opcode.
// Software or hardware must take care to acknowledge or reset /IRQ or /NMI signals after processing it.

//   IRQs are executed whenever "/IRQ=LOW AND I=0".
//   NMIs are executed whenever "/NMI changes from HIGH to LOW".

// If /IRQ is kept LOW then same (old) interrupt is executed again as soon as setting I=0. If /NMI is kept LOW then no further NMIs can be executed.

// CPU Control

//   Opcode    Flags Clks  Native    Nocash    Effect
//   18        --0---  2   CLC       CLC       C=0    ;Clear carry flag
//   58        ---0--  2   CLI       EI        I=0    ;Clear interrupt disable bit
//   D8        ----0-  2   CLD       CLD       D=0    ;Clear decimal mode
//   B8        -----0  2   CLV       CL?       V=0    ;Clear overflow flag
//   38        --1---  2   SEC       STC       C=1    ;Set carry flag
//   78        ---1--  2   SEI       DI        I=1    ;Set interrupt disable bit
//   F8        ----1-  2   SED       STD       D=1    ;Set decimal mode
//   C2 nn     xxxxxx  3   REP #nn   CLR P,nn  P=P AND NOT nn
//   E2 nn     xxxxxx  3   SEP #nn   SET P,nn  P=P OR nn
//   FB        --c---  2   XCE       XCE       C=E, E=C


// Special Opcodes

//   Opcode    Flags Clks  Native      Nocash
//   DB        ------  -   STP         KILL      ;STOP/KILL
//   EB        nz----  3   XBA         SWAP A    ;A=B, B=A, NZ=LSB
//   CB        ------  3x  WAI         HALT      ;HALT
//   42 nn             2   WDM #nn     NUL nn    ;No operation
//   EA        ------  2   NOP         NOP       ;No operation

// WAI/HALT stops the CPU until an exception (usually an IRQ or NMI) request occurs; in case of IRQs this works even if IRQs are disabled (via I=1).

// Conditional Branch Page Crossing
// The branch opcode with parameter takes up two bytes, causing the PC to get incremented twice (PC=PC+2), without any extra boundary cycle. The signed parameter is then added to the PC (PC+disp), the extra clock cycle occurs if the addition crosses a page boundary (next or previous 100h-page).