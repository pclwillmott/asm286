/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    action.c       Production decoder
 *
 *  This revision:
 *
 *    2019 November 10 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2019 November 10 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include "asm286.h"

/*
 * Production Table
 */

#define PT_IGNORE      "\001"
#define PT_DROP        "\002"
#define PT_EXECUTE     "\003"

char *prdlst[] = {
 
  SPD_STMT        PT_IGNORE       SPD_LABEL_STMT,
  SPD_STMT        PT_IGNORE       SPD_DATA_STMT,
  SPD_STMT        PT_IGNORE       SPD_DIRECTIVE,
  
  SPD_INST_LABEL  PT_EXECUTE      STK_INST_LABEL,
  
  SPD_LABEL_STMT  PT_IGNORE       SPD_INST_LABEL  SPD_INSTRUCTION,
  SPD_LABEL_STMT  PT_IGNORE       SPD_INST_LABEL,
  SPD_LABEL_STMT  PT_IGNORE       SPD_INSTRUCTION,
  
  SPD_INSTRUCTION PT_IGNORE       SPD_SIMPLE,
  SPD_INSTRUCTION PT_IGNORE       SPD_JR,
  SPD_INSTRUCTION PT_IGNORE       SPD_ALU,

  SPD_DIRECTIVE   PT_IGNORE       SPD_WARNING,
  SPD_DIRECTIVE   PT_IGNORE       SPD_NAME,
  SPD_DIRECTIVE   PT_IGNORE       SPD_STACKSEG,
  SPD_DIRECTIVE   PT_IGNORE       SPD_ENDS,
  SPD_DIRECTIVE   PT_IGNORE       SPD_SEGMENT,
  SPD_DIRECTIVE   PT_IGNORE       SPD_ORG,

  SPD_VAR_NAME    PT_EXECUTE      STK_IDENTIFIER,
  
  SPD_DATA_STMT   PT_IGNORE       SPD_VAR_NAME    SPD_DATA_STMT,
  SPD_DATA_STMT   PT_IGNORE       SPD_DDVARIABLE,
  SPD_DATA_STMT   PT_IGNORE       SPD_DBVARIABLE,
  SPD_DATA_STMT   PT_IGNORE       SPD_DWVARIABLE,

/*
 * Directives
 */
  
  SPD_NAME        PT_EXECUTE      STK_NAME        STK_IDENTIFIER,
  
  SPD_STACKSEG    PT_EXECUTE      STK_IDENTIFIER  STK_STACKSEG    SPD_NUM_EXP,
  
  SPD_SEGMENT     PT_EXECUTE      STK_IDENTIFIER  STK_SEGMENT     SPD_SEG_ATT,
  SPD_SEGMENT     PT_EXECUTE      STK_IDENTIFIER  STK_SEGMENT,

  SPD_SEG_ATT     PT_DROP         SPD_SEG_ATT     SPD_SEG_AITEM,
  SPD_SEG_ATT     PT_DROP         SPD_SEG_AITEM,
  
  SPD_SEG_AITEM   PT_EXECUTE      STK_RO,
  SPD_SEG_AITEM   PT_EXECUTE      STK_EO,
  SPD_SEG_AITEM   PT_EXECUTE      STK_ER,
  SPD_SEG_AITEM   PT_EXECUTE      STK_RW,
  SPD_SEG_AITEM   PT_EXECUTE      STK_PUBLIC,
  SPD_SEG_AITEM   PT_EXECUTE      STK_COMMON,
  
  SPD_ENDS        PT_EXECUTE      STK_IDENTIFIER  STK_ENDS,
  
  SPD_ORG         PT_EXECUTE      STK_ORG         SPD_NUM_EXP,

/*
 * Numerical Operators
 */

  SPD_GRP1_OP     PT_IGNORE       STK_LENGTH,
  SPD_GRP1_OP     PT_IGNORE       STK_SIZE,
  SPD_GRP1_OP     PT_IGNORE       STK_WIDTH,
  SPD_GRP1_OP     PT_IGNORE       STK_MASK,
  SPD_GRP1_OP     PT_IGNORE       STK_STACKSTART,

  
  SPD_GRP2_OP     PT_IGNORE       STK_PTR,
  SPD_GRP2_OP     PT_IGNORE       STK_OFFSET,
  SPD_GRP2_OP     PT_IGNORE       STK_SEG,
  SPD_GRP2_OP     PT_IGNORE       STK_TYPE,
  SPD_GRP2_OP     PT_IGNORE       STK_THIS,

  
  SPD_GRP3_OP     PT_IGNORE       STK_HIGH,
  SPD_GRP3_OP     PT_IGNORE       STK_LOW,

  SPD_GRP4_OP     PT_IGNORE       STK_MULTIPLY, /* DONE */
  SPD_GRP4_OP     PT_IGNORE       STK_DIVIDE,   /* DONE */
  SPD_GRP4_OP     PT_IGNORE       STK_MOD,      /* DONE */
  SPD_GRP4_OP     PT_IGNORE       STK_SHL,      /* DONE */
  SPD_GRP4_OP     PT_IGNORE       STK_SHR,      /* DONE */
  
  SPD_GRP5_OP     PT_IGNORE       STK_PLUS,     /* DONE */
  SPD_GRP5_OP     PT_IGNORE       STK_MINUS,    /* DONE */

  SPD_GRP6_OP     PT_IGNORE       STK_EQ,       /* DONE */
  SPD_GRP6_OP     PT_IGNORE       STK_NE,       /* DONE */
  SPD_GRP6_OP     PT_IGNORE       STK_LT,       /* DONE */
  SPD_GRP6_OP     PT_IGNORE       STK_GT,       /* DONE */
  SPD_GRP6_OP     PT_IGNORE       STK_LE,       /* DONE */
  SPD_GRP6_OP     PT_IGNORE       STK_GE,       /* DONE */

  SPD_GRP7_OP     PT_IGNORE       STK_NOT,      /* DONE */

  SPD_GRP8_OP     PT_IGNORE       STK_AND,      /* DONE */
  
  SPD_GRP9_OP     PT_IGNORE       STK_OR,       /* DONE */
  SPD_GRP9_OP     PT_IGNORE       STK_XOR,      /* DONE */
  
  SPD_GRP10_OP    PT_IGNORE       STK_SHORT,

/*
 * Variables and Constants
 */
  
  SPD_CON_NUM     PT_EXECUTE      STK_DOUBLE,
  
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERBIN,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGEROCT,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERDEC,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERHEX,

//  SPD_CON_STR     PT_EXECUTE      STK_STRING,
  
/*
 * Numerical Expressions
 */
  
  SPD_GRP0_EXP    PT_DROP         SPD_CON_NUM, 
  SPD_GRP0_EXP    PT_EXECUTE      STK_OBRACE      SPD_NUM_EXP     STK_CBRACE,
  SPD_GRP0_EXP    PT_EXECUTE      STK_IDENTIFIER,
  SPD_GRP0_EXP    PT_EXECUTE      STK_DOLLAR,

  SPD_GRP1_EXP    PT_EXECUTE      SPD_GRP1_EXP    SPD_GRP1_OP     SPD_GRP0_EXP,
  SPD_GRP1_EXP    PT_DROP         SPD_GRP0_EXP,
  
  SPD_GRP2_EXP    PT_EXECUTE      SPD_GRP2_EXP    SPD_GRP2_OP     SPD_GRP1_EXP,
  SPD_GRP2_EXP    PT_DROP         SPD_GRP1_EXP,
  
  SPD_GRP3_EXP    PT_EXECUTE      SPD_GRP3_OP     SPD_GRP2_EXP,
  SPD_GRP3_EXP    PT_DROP         SPD_GRP2_EXP,
  
  SPD_GRP4_EXP    PT_EXECUTE      SPD_GRP4_EXP    SPD_GRP4_OP     SPD_GRP3_EXP,
  SPD_GRP4_EXP    PT_DROP         SPD_GRP3_EXP,
  
  SPD_GRP5_EXP    PT_EXECUTE      SPD_GRP5_EXP    SPD_GRP5_OP     SPD_GRP4_EXP,
  SPD_GRP5_EXP    PT_DROP         SPD_GRP4_EXP,
  
  SPD_GRP6_EXP    PT_EXECUTE      SPD_GRP6_EXP    SPD_GRP6_OP     SPD_GRP5_EXP,
  SPD_GRP6_EXP    PT_DROP         SPD_GRP5_EXP,
  
  SPD_GRP7_EXP    PT_EXECUTE      SPD_GRP7_OP     SPD_GRP6_EXP,
  SPD_GRP7_EXP    PT_DROP         SPD_GRP6_EXP,
  
  SPD_GRP8_EXP    PT_EXECUTE      SPD_GRP8_EXP    SPD_GRP8_OP     SPD_GRP7_EXP,
  SPD_GRP8_EXP    PT_DROP         SPD_GRP7_EXP,
  
  SPD_GRP9_EXP    PT_EXECUTE      SPD_GRP9_EXP    SPD_GRP9_OP     SPD_GRP8_EXP,
  SPD_GRP9_EXP    PT_DROP         SPD_GRP8_EXP,
  
  SPD_NUM_EXP     PT_EXECUTE      SPD_NUM_EXP     SPD_GRP10_OP    SPD_GRP9_EXP,
  SPD_NUM_EXP     PT_DROP         SPD_GRP9_EXP,

  SPD_db          PT_EXECUTE      SPD_NUM_EXP,
  
  SPD_dw          PT_EXECUTE      SPD_NUM_EXP,
  
  SPD_DISP        PT_IGNORE       SPD_NUM_EXP,
  
/*
 * ----------
 * Statements
 * ----------
 */

  SPD_WARNING     PT_EXECUTE      STK_WARNING     SPD_NUM_EXP,
  
/*
 * Simple operands.
 */
 
  SPD_SIMPLE      PT_EXECUTE      STK_AAA,
  SPD_SIMPLE      PT_EXECUTE      STK_AAD,
  SPD_SIMPLE      PT_EXECUTE      STK_AAM,
  SPD_SIMPLE      PT_EXECUTE      STK_AAS,
  SPD_SIMPLE      PT_EXECUTE      STK_CBW,
  SPD_SIMPLE      PT_EXECUTE      STK_CLC,
  SPD_SIMPLE      PT_EXECUTE      STK_CLD,
  SPD_SIMPLE      PT_EXECUTE      STK_CLI,
  SPD_SIMPLE      PT_EXECUTE      STK_CLTS,
  SPD_SIMPLE      PT_EXECUTE      STK_CMC,
  SPD_SIMPLE      PT_EXECUTE      STK_CWD,
  SPD_SIMPLE      PT_EXECUTE      STK_DAA,
  SPD_SIMPLE      PT_EXECUTE      STK_DAS,
  SPD_SIMPLE      PT_EXECUTE      STK_HLT,
  SPD_SIMPLE      PT_EXECUTE      STK_IRET,
  SPD_SIMPLE      PT_EXECUTE      STK_LAHF,
  SPD_SIMPLE      PT_EXECUTE      STK_LEAVE,
  SPD_SIMPLE      PT_EXECUTE      STK_NOP,
  SPD_SIMPLE      PT_EXECUTE      STK_POPA,
  SPD_SIMPLE      PT_EXECUTE      STK_POPF,
  SPD_SIMPLE      PT_EXECUTE      STK_PUSHA,
  SPD_SIMPLE      PT_EXECUTE      STK_PUSHF,
  SPD_SIMPLE      PT_EXECUTE      STK_SAHF,
  SPD_SIMPLE      PT_EXECUTE      STK_STC,
  SPD_SIMPLE      PT_EXECUTE      STK_STD,
  SPD_SIMPLE      PT_EXECUTE      STK_STI,
  SPD_SIMPLE      PT_EXECUTE      STK_WAIT,

  SPD_rw          PT_IGNORE       STK_AX,
  SPD_rw          PT_IGNORE       STK_CX,
  SPD_rw          PT_IGNORE       STK_DX,
  SPD_rw          PT_IGNORE       STK_BX,
  SPD_rw          PT_IGNORE       STK_SP,
  SPD_rw          PT_IGNORE       STK_BP,
  SPD_rw          PT_IGNORE       STK_SI,
  SPD_rw          PT_IGNORE       STK_DI,

  SPD_rb          PT_IGNORE       STK_AL,
  SPD_rb          PT_IGNORE       STK_CL,
  SPD_rb          PT_IGNORE       STK_DL,
  SPD_rb          PT_IGNORE       STK_BL,
  SPD_rb          PT_IGNORE       STK_AH,
  SPD_rb          PT_IGNORE       STK_CH,
  SPD_rb          PT_IGNORE       STK_DH,
  SPD_rb          PT_IGNORE       STK_BH,

  SPD_IND_BX      PT_IGNORE       STK_OBRACE      STK_BX          STK_CBRACE,
  SPD_IND_BP      PT_IGNORE       STK_OBRACE      STK_BP          STK_CBRACE,
  SPD_IND_SI      PT_IGNORE       STK_OBRACE      STK_SI          STK_CBRACE,
  SPD_IND_DI      PT_IGNORE       STK_OBRACE      STK_DI          STK_CBRACE,

  SPD_rm          PT_IGNORE       SPD_IND_BX      STK_PLUS        SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_BX      STK_PLUS        SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_BP      STK_PLUS        SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_BP      STK_PLUS        SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_BP,
  SPD_rm          PT_IGNORE       SPD_IND_BX,
  
  SPD_rm_disp     PT_IGNORE       SPD_rm          STK_PLUS        SPD_NUM_EXP,
  SPD_rm_disp     PT_IGNORE       SPD_rm,

  
//  SPD_eb          PT_IGNORE       SPD_rb,
//  SPD_eb          PT_IGNORE       SPD_rm,

//  SPD_ew          PT_IGNORE       SPD_rw,
//  SPD_ew          PT_IGNORE       SPD_rm,
  
  SPD_RS          PT_IGNORE       STK_ES,
  SPD_RS          PT_IGNORE       STK_CS,
  SPD_RS          PT_IGNORE       STK_SS,
  SPD_RS          PT_IGNORE       STK_DS,

  SPD_ALUOP       PT_IGNORE       STK_ADD,
  SPD_ALUOP       PT_IGNORE       STK_OR,
  SPD_ALUOP       PT_IGNORE       STK_ADC,
  SPD_ALUOP       PT_IGNORE       STK_SBB,
  SPD_ALUOP       PT_IGNORE       STK_AND,
  SPD_ALUOP       PT_IGNORE       STK_SUB,
  SPD_ALUOP       PT_IGNORE       STK_XOR,
  SPD_ALUOP       PT_IGNORE       STK_CMP,
  
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       STK_AL          STK_COMMA       SPD_db,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       STK_AX          STK_COMMA       SPD_dw,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rb          STK_COMMA       SPD_rb,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rw          STK_COMMA       SPD_rw,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rb          STK_COMMA       SPD_rm_disp,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rw          STK_COMMA       SPD_rm_disp,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_rb,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_rw,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_rm_disp,

  SPD_JMPOP       PT_IGNORE       STK_JO,   // 0x70
  SPD_JMPOP       PT_IGNORE       STK_JNO,  // 0x71
  SPD_JMPOP       PT_IGNORE       STK_JB,   // 0x72
  SPD_JMPOP       PT_IGNORE       STK_JNAE, // 0x72
  SPD_JMPOP       PT_IGNORE       STK_JC,   // 0x72
  SPD_JMPOP       PT_IGNORE       STK_JAE,  // 0x73
  SPD_JMPOP       PT_IGNORE       STK_JNB,  // 0x73
  SPD_JMPOP       PT_IGNORE       STK_JNC,  // 0x73
  SPD_JMPOP       PT_IGNORE       STK_JE,   // 0x74
  SPD_JMPOP       PT_IGNORE       STK_JZ,   // 0x74
  SPD_JMPOP       PT_IGNORE       STK_JNE,  // 0x75
  SPD_JMPOP       PT_IGNORE       STK_JNZ,  // 0x75
  SPD_JMPOP       PT_IGNORE       STK_JBE,  // 0x76
  SPD_JMPOP       PT_IGNORE       STK_JNA,  // 0x76
  SPD_JMPOP       PT_IGNORE       STK_JA,   // 0x77
  SPD_JMPOP       PT_IGNORE       STK_JNBE, // 0x77
  SPD_JMPOP       PT_IGNORE       STK_JS,   // 0x78
  SPD_JMPOP       PT_IGNORE       STK_JNS,  // 0x79
  SPD_JMPOP       PT_IGNORE       STK_JP,   // 0x7a
  SPD_JMPOP       PT_IGNORE       STK_JPE,  // 0x7a
  SPD_JMPOP       PT_IGNORE       STK_JNP,  // 0x7b
  SPD_JMPOP       PT_IGNORE       STK_JPO,  // 0x7b
  SPD_JMPOP       PT_IGNORE       STK_JL,   // 0x7c
  SPD_JMPOP       PT_IGNORE       STK_JNGE, // 0x7c
  SPD_JMPOP       PT_IGNORE       STK_JGE,  // 0x7d
  SPD_JMPOP       PT_IGNORE       STK_JNL,  // 0x7d
  SPD_JMPOP       PT_IGNORE       STK_JLE,  // 0x7e
  SPD_JMPOP       PT_IGNORE       STK_JNG,  // 0x7e
  SPD_JMPOP       PT_IGNORE       STK_JG,   // 0x7f
  SPD_JMPOP       PT_IGNORE       STK_JNLE, // 0x7f
  SPD_JMPOP       PT_IGNORE       STK_JCXZ, // 0xe3

  SPD_JR          PT_EXECUTE      SPD_JMPOP       SPD_NUM_EXP,
  
  SPD_VARIABLE    PT_EXECUTE      STK_IDENTIFIER  SPD_VAR_TYPE    SPD_INITLIST,
  SPD_VARIABLE    PT_EXECUTE      SPD_VAR_TYPE    SPD_INITLIST,

  SPD_VAR_TYPE    PT_IGNORE       STK_DQ,
  SPD_VAR_TYPE    PT_IGNORE       STK_DT,

  SPD_INITLIST    PT_DROP         SPD_INITLIST    STK_COMMA       SPD_INITITEM,
  SPD_INITLIST    PT_DROP         SPD_INITITEM,
  
  SPD_INITITEM    PT_EXECUTE      SPD_NUM_EXP,
  SPD_INITITEM    PT_EXECUTE      STK_QMARK,

  SPD_DBVARIABLE  PT_DROP         STK_DB          SPD_DBLIST,
  
  SPD_DBLIST      PT_DROP         SPD_DBLIST      STK_COMMA       SPD_DBITEM,
  SPD_DBLIST      PT_DROP         SPD_DBITEM,

  SPD_DBITEM      PT_EXECUTE      SPD_NUM_EXP,
  SPD_DBITEM      PT_EXECUTE      STK_STRING,
  SPD_DBITEM      PT_EXECUTE      STK_QMARK,
  
  SPD_DWVARIABLE  PT_DROP         STK_DW          SPD_DWLIST,
  
  SPD_DWLIST      PT_DROP         SPD_DWLIST      STK_COMMA       SPD_DWITEM,
  SPD_DWLIST      PT_DROP         SPD_DWITEM,
  
  SPD_DWITEM      PT_EXECUTE      SPD_NUM_EXP,
  SPD_DWITEM      PT_EXECUTE      STK_STRING,
  SPD_DWITEM      PT_EXECUTE      STK_QMARK,
  
  SPD_DDVARIABLE  PT_DROP         STK_DD          SPD_DDLIST,
  
  SPD_DDLIST      PT_DROP         SPD_DDLIST      STK_COMMA       SPD_DDITEM,
  SPD_DDLIST      PT_DROP         SPD_DDITEM,
  
  SPD_DDITEM      PT_EXECUTE      SPD_NUM_EXP,
  SPD_DDITEM      PT_EXECUTE      STK_STRING,
  SPD_DDITEM      PT_EXECUTE      STK_QMARK,
  
  SPD_LAST,
  
} ;

/*
 *------------------------------------------------------------------------------
 *  ASM286
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott
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
