/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    token.c       Tokenize statement
 *
 *  This revision:
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include "asm286.h"

const char *keyword[ NUM_KEYWORDS ] = {
  "_",
  "-",
  ";",
  ":",
  "?",
  ".",
  "'",
  "(",
  ")",
  "[",
  "]",
  "@",
  "*",
  "/",
  "\"",
  "&",
  "+",
  "<",
  ">",
  "$",
  "AAA",
  "AAD",
  "AAM",
  "AAS",
  "ABS",
  "ADC",
  "ADD",
  "AH",
  "AL",
  "ALIGN",
  "AND",
  "ARPL",
  "ASSUME",
  "AX",
  "BH",
  "BIT",
  "BITOFFSET",
  "BL",
  "BOUND",
  "BP",
  "BSF",
  "BSR",
  "BSWAP",
  "BT",
  "BTC",
  "BTR",
  "BTS",
  "BX",
  "BYTE",
  "CALL",
  "CBW",
  "CDQ",
  "CH",
  "CL",
  "CLC",
  "CLD",
  "CLI",
  "CLTS",
  "CMC",
  "CMP",
  "CMPS",
  "CMPXCHG",
  "COMM",
  "COMMON",
  "CR0",
  "CR1",
  "CR2",
  "CS",
  "CWD",
  "CWDE",
  "CX",
  "DAA",
  "DAS",
  "DB",
  "DBIT",
  "DD",
  "DEC",
  "DH",
  "DI",
  "DIV",
  "DL",
  "DP",
  "DQ",
  "DR0",
  "DR1",
  "DR2",
  "DR3",
  "DR6",
  "DR7",
  "DS",
  "DT",
  "DUP",
  "DW",
  "DWORD",
  "DX",
  "EAX",
  "EBP",
  "EBX",
  "ECX",
  "EDI",
  "EDX",
  "END",
  "ENDP",
  "ENDS",
  "ENTER",
  "EO",
  "EQ",
  "EQU",
  "ER",
  "ES",
  "ESC",
  "ESI",
  "ESP",
  "EVEN",
  "EXTRN",
  "F2XM1",
  "FABS",
  "FADD",
  "FADDP",
  "FAR",
  "FBLD",
  "FBSTP",
  "FCHS",
  "FCLEX",
  "FCOM",
  "FCOMP",
  "FCOMPP",
  "FCOS",
  "FDECSTP",
  "FDISI",
  "FDIV",
  "FDIVP",
  "FDIVR",
  "FDIVRP",
  "FENI",
  "FFREE",
  "FIADD",
  "FICOM",
  "FICOMP",
  "FIDIV",
  "FIDIVR",
  "FILD",
  "FIMUL",
  "FINCSTP",
  "FINIT",
  "FIST",
  "FISTP",
  "FISUB",
  "FISUBR",
  "FLD",
  "FLD1",
  "FLDCW",
  "FLDENV",
  "FLDL2E",
  "FLDL2T",
  "FLDLG2",
  "FLDLN2",
  "FLDPI",
  "FLDZ",
  "FMUL",
  "FMULP",
  "FNCLEX",
  "FNDISI",
  "FNENI",
  "FNINIT",
  "FNOP",
  "FNSAVE",
  "FNSTCW",
  "FNSTENV",
  "FNSTSW",
  "FPATAN",
  "FPREM",
  "FPREM1"
  "FPTAN",
  "FRNDINT",
  "FRSTOR",
  "FS",
  "FSAVE",
  "FSCALE",
  "FSETPM",
  "FSIN",
  "FSINCOS",
  "FSQRT",
  "FST",
  "FSTCW",
  "FSTENV",
  "FSTP",
  "FSTSW",
  "FSUB",
  "FSUBP",
  "FSUBR",
  "FSUBRP",
  "FTST",
  "FUCOM",
  "FUCOMP",
  "FUCOMPP",
  "FWAIT",
  "FXAM",
  "FXCH",
  "FXTRACT",
  "FYL2X",
  "FYL2XP1",
  "GE",
  "GS",
  "GT",
  "HIGH",
  "HIGHW",
  "HLT",
  "IDIV",
  "IMUL",
  "IN",
  "INC",
  "INS",
  "INT",
  "INTO",
  "INVD",
  "INVLPG",
  "IRET",
  "JA",
  "JAE",
  "JB",
  "JBE",
  "JC",
  "JCXZ",
  "JE",
  "JECXZ",
  "JG",
  "JGE",
  "JL",
  "JLE",
  "JMP",
  "JNA",
  "JNAE",
  "JNB",
  "JNBE",
  "JNC",
  "JNE",
  "JNG",
  "JNGE",
  "JNL",
  "JNLE",
  "JNO",
  "JNP",
  "JNS",
  "JNZ",
  "JO",
  "JP",
  "JPE",
  "JPO",
  "JS",
  "JZ",
  "LABEL",
  "LAHF",
  "LAR",
  "LDS",
  "LE",
  "LEA",
  "LEAVE",
  "LENGTH",
  "LES",
  "LFS",
  "LGDT",
  "LGS",
  "LIDT",
  "LLDT",
  "LMSW",
  "LOCK",
  "LODS",
  "LOOP",
  "LOOPD",
  "LOOPE",
  "LOOPED",
  "LOOPNE",
  "LOOPNED",
  "LOW",
  "LOWW",
  "LSL",
  "LSS",
  "LT",
  "LTR",
  "MASK",
  "MOD",
  "MOV",
  "MOVS",
  "MOVSX",
  "MOVZX",
  "MUL",
  "NAME",
  "NE",
  "NEAR",
  "NEG",
  "NOP",
  "NOT",
  "NOTHING",
  "OFFSET",
  "OR",
  "ORG",
  "OUT",
  "OUTS",
  "POP",
  "POPA",
  "POPAD",
  "POPF",
  "POPFD",
  "PROC",
  "PROCLEN",
  "PTR",
  "PUBLIC",
  "PURGE",
  "PUSH",
  "PUSHA",
  "PUSHAD",
  "PUSHF",
  "PUSHFD",
  "PWORD",
  "QWORD",
  "RCL",
  "RCR",
  "RECORD",
  "REPE",
  "REPNE",
  "RET",
  "RO",
  "ROL",
  "ROR",
  "RW",
  "SAHF",
  "SAL",
  "SAR",
  "SCAS",
  "SEG",
  "SEGMENT",
  "SET",
  "SGDT",
  "SHL",
  "SHLD",
  "SHORT",
  "SHR",
  "SHRD",
  "SI",
  "SIDT",
  "SIZE",
  "SLDT",
  "SMSW",
  "SP",
  "SS",
  "STACKSEG",
  "STACKSTART",
  "STC",
  "STD",
  "STI",
  "STOS",
  "STR",
  "STRUC",
  "SUB",
  "TBYTE",
  "TEST",
  "THIS",
  "TR3",
  "TR4",
  "TR5",
  "TR6",
  "TR7",
  "TYPE",
  "USE16",
  "USE32",
  "VERR",
  "VERW",
  "WAIT",
  "WBINVD",
  "WIDTH",
  "WORD",
  "XADD",
  "XCHG",
  "XLAT",
  "XOR",
} ;

/*
 *------------------------------------------------------------------------------
 *  ASM286
 *
 *  Copyright (c) 2019 Paul C. L. Willmott
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *------------------------------------------------------------------------------
 */
