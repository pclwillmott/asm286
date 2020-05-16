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

  SPD_TOP             PT_IGNORE   SPD_DIRECTIVE_LIST,
  SPD_TOP             PT_IGNORE   STK_NEWLINE,
  
  SPD_DIRECTIVE_LIST  PT_IGNORE   SPD_DIRECTIVE_LIST  SPD_DIRECTIVE,
  SPD_DIRECTIVE_LIST  PT_IGNORE   SPD_DIRECTIVE,

  //SPD_DIRECTIVE       PT_IGNORE   SPD_GENERAL_DIR,
  SPD_DIRECTIVE       PT_IGNORE   SPD_SEGMENT_DEF,

  SPD_SEGMENT_DEF     PT_IGNORE   SPD_SEGMENT_DIR  SPD_IN_SEG_DIR_LIST  SPD_ENDS_DIR,
  SPD_SEGMENT_DEF     PT_IGNORE   SPD_SEGMENT_DIR  SPD_ENDS_DIR,

  //SPD_SEGMENT_DIR     PT_IGNORE   SPD_SEG_ID STK_SEGMENT SPD_SEG_OPTION_LIST SPD_NEWLINE,
  SPD_SEGMENT_DIR     PT_IGNORE   SPD_SEG_ID STK_SEGMENT STK_NEWLINE,

  SPD_ENDS_DIR        PT_IGNORE   SPD_SEG_ID STK_ENDS STK_NEWLINE,
  
  SPD_SEG_ID          PT_IGNORE   STK_IDENTIFIER,

  SPD_IN_SEG_DIR_LIST PT_IGNORE   SPD_IN_SEG_DIR_LIST   SPD_IN_SEG_DIR,
  SPD_IN_SEG_DIR_LIST PT_IGNORE   SPD_IN_SEG_DIR,
  
  SPD_IN_SEG_DIR      PT_IGNORE   SPD_LABEL_DEF  SPD_IN_SEGMENT_DIR,
  SPD_IN_SEG_DIR      PT_IGNORE   SPD_IN_SEGMENT_DIR,

  SPD_LABEL_DEF       PT_IGNORE   STK_IDENTIFIER  STK_COLON  STK_COLON,
  SPD_LABEL_DEF       PT_IGNORE   STK_IDENTIFIER  STK_COLON,
  SPD_LABEL_DEF       PT_IGNORE   STK_ATSIGN STK_ATSIGN STK_COLON,

  SPD_IN_SEGMENT_DIR  PT_IGNORE   SPD_INSTRUCTION,
  
  SPD_INSTRUCTION     PT_IGNORE   SPD_INST_PREFIX  SPD_ASM_INSTRUCTION,
  SPD_INSTRUCTION     PT_IGNORE   SPD_ASM_INSTRUCTION,
  
  SPD_INST_PREFIX    PT_IGNORE   STK_REP,
  SPD_INST_PREFIX    PT_IGNORE   STK_REPE,
//  SPD_INST_PREFIX    PT_IGNORE   STK_REPZ,
  SPD_INST_PREFIX    PT_IGNORE   STK_REPNE,
  SPD_INST_PREFIX    PT_IGNORE   STK_REPNZ,
  SPD_INST_PREFIX    PT_IGNORE   STK_LOCK,

  SPD_ASM_INSTRUCTION PT_IGNORE   SPD_MNEMONIC  SPD_EXPR_LIST SPD_NEWLINE,
  SPD_ASM_INSTRUCTION PT_IGNORE   SPD_MNEMONIC STK_NEWLINE,
  
  SPD_MNEMONIC      PT_EXECUTE      STK_AAA,
  SPD_MNEMONIC      PT_EXECUTE      STK_AAD,
  SPD_MNEMONIC      PT_EXECUTE      STK_AAM,
  SPD_MNEMONIC      PT_EXECUTE      STK_AAS,
  SPD_MNEMONIC      PT_EXECUTE      STK_CBW,
  SPD_MNEMONIC      PT_EXECUTE      STK_CLC,
  SPD_MNEMONIC      PT_EXECUTE      STK_CLD,
  SPD_MNEMONIC      PT_EXECUTE      STK_CLI,
  SPD_MNEMONIC      PT_EXECUTE      STK_CLTS,
  SPD_MNEMONIC      PT_EXECUTE      STK_CMC,
  SPD_MNEMONIC      PT_EXECUTE      STK_CWD,
  SPD_MNEMONIC      PT_EXECUTE      STK_DAA,
  SPD_MNEMONIC      PT_EXECUTE      STK_DAS,
  SPD_MNEMONIC      PT_EXECUTE      STK_HLT,
  SPD_MNEMONIC      PT_EXECUTE      STK_IRET,
  SPD_MNEMONIC      PT_EXECUTE      STK_LAHF,
  SPD_MNEMONIC      PT_EXECUTE      STK_LEAVE,
  SPD_MNEMONIC      PT_IGNORE       STK_NOP,
  SPD_MNEMONIC      PT_EXECUTE      STK_POPA,
  SPD_MNEMONIC      PT_EXECUTE      STK_POPF,
  SPD_MNEMONIC      PT_EXECUTE      STK_PUSHA,
  SPD_MNEMONIC      PT_EXECUTE      STK_PUSHF,
  SPD_MNEMONIC      PT_EXECUTE      STK_SAHF,
  SPD_MNEMONIC      PT_EXECUTE      STK_STC,
  SPD_MNEMONIC      PT_EXECUTE      STK_STD,
  SPD_MNEMONIC      PT_EXECUTE      STK_STI,
  SPD_MNEMONIC      PT_EXECUTE      STK_WAIT,
  
  SPD_EXPR_LIST     PT_IGNORE       SPD_EXPR_LIST     SPD_EXPR,
  SPD_EXPR_LIST     PT_IGNORE       SPD_EXPR,

  SPD_EXPR          PT_IGNORE       STK_SHORT         SPD_E05,
  SPD_EXPR          PT_IGNORE       SPD_E01,

  SPD_INST_LABEL  PT_EXECUTE      STK_INST_LABEL,
  
  SPD_LABEL_STMT  PT_IGNORE       SPD_INST_LABEL  SPD_INSTRUCTION,
  SPD_LABEL_STMT  PT_IGNORE       SPD_INST_LABEL,
  SPD_LABEL_STMT  PT_IGNORE       SPD_INSTRUCTION,
  
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
  
//  SPD_SEGMENT     PT_EXECUTE      STK_IDENTIFIER  STK_SEGMENT     SPD_SEG_ATT,
//  SPD_SEGMENT     PT_EXECUTE      STK_IDENTIFIER  STK_SEGMENT,

  SPD_SEG_ATT     PT_DROP         SPD_SEG_ATT     SPD_SEG_AITEM,
  SPD_SEG_ATT     PT_DROP         SPD_SEG_AITEM,
  
  SPD_SEG_AITEM   PT_EXECUTE      STK_RO,
  SPD_SEG_AITEM   PT_EXECUTE      STK_EO,
  SPD_SEG_AITEM   PT_EXECUTE      STK_ER,
  SPD_SEG_AITEM   PT_EXECUTE      STK_RW,
  SPD_SEG_AITEM   PT_EXECUTE      STK_PUBLIC,
  SPD_SEG_AITEM   PT_EXECUTE      STK_COMMON,
  
 // SPD_ENDS        PT_EXECUTE      STK_IDENTIFIER  STK_ENDS,
  
  SPD_ORG         PT_EXECUTE      STK_ORG         SPD_NUM_EXP,

/*
 * Numerical Operators
 */

  SPD_E9_OP       PT_IGNORE       STK_PTR,
  SPD_E9_OP       PT_IGNORE       STK_COLON,
  
  SPD_HIGH_OP     PT_IGNORE       STK_HIGH,
  SPD_HIGH_OP     PT_IGNORE       STK_LOW,

  SPD_MUL_OP      PT_IGNORE       STK_MULTIPLY, /* DONE */
  SPD_MUL_OP      PT_IGNORE       STK_DIVIDE,   /* DONE */
  SPD_MUL_OP      PT_IGNORE       STK_MOD,      /* DONE */
  SPD_MUL_OP      PT_IGNORE       STK_SHL,      /* DONE */
  SPD_MUL_OP      PT_IGNORE       STK_SHR,      /* DONE */
  
  SPD_ADD_OP      PT_IGNORE       STK_PLUS,     /* DONE */
  SPD_ADD_OP      PT_IGNORE       STK_MINUS,    /* DONE */

  SPD_REL_OP      PT_IGNORE       STK_EQ,       /* DONE */
  SPD_REL_OP      PT_IGNORE       STK_NE,       /* DONE */
  SPD_REL_OP      PT_IGNORE       STK_LT,       /* DONE */
  SPD_REL_OP      PT_IGNORE       STK_GT,       /* DONE */
  SPD_REL_OP      PT_IGNORE       STK_LE,       /* DONE */
  SPD_REL_OP      PT_IGNORE       STK_GE,       /* DONE */

  SPD_NOT_OP      PT_IGNORE       STK_NOT,      /* DONE */

  SPD_AND_OP      PT_IGNORE       STK_AND,      /* DONE */
  
  SPD_OR_OP       PT_IGNORE       STK_OR,       /* DONE */
  SPD_OR_OP       PT_IGNORE       STK_XOR,      /* DONE */
  
/*
 * Variables and Constants
 */
  
//  SPD_CON_NUM     PT_EXECUTE      STK_DOUBLE,
  
  SPD_CONSTANT     PT_EXECUTE      STK_INTEGERHEX,
  SPD_CONSTANT     PT_EXECUTE      STK_INTEGERBIN,
  SPD_CONSTANT     PT_EXECUTE      STK_INTEGEROCT,
  SPD_CONSTANT     PT_EXECUTE      STK_INTEGERDEC,

/*
 * Numerical Expressions
 */
  
  SPD_E01    PT_EXECUTE      SPD_E01    SPD_OR_OP     SPD_E02,
  SPD_E01    PT_DROP         SPD_E02,

  SPD_E02    PT_EXECUTE      SPD_E02    SPD_AND_OP    SPD_E03,
  SPD_E02    PT_DROP         SPD_E03,

  SPD_E03    PT_EXECUTE      SPD_NOT_OP SPD_E04,
  SPD_E03    PT_DROP         SPD_E04,

  SPD_E04    PT_EXECUTE      SPD_E04    SPD_REL_OP     SPD_E05,
  SPD_E04    PT_DROP         SPD_E05,
  
  SPD_E05    PT_EXECUTE      SPD_E05    SPD_ADD_OP     SPD_E06,
  SPD_E05    PT_DROP         SPD_E06,
  
  SPD_E06    PT_EXECUTE      SPD_E06    SPD_MUL_OP     SPD_E07,
  SPD_E06    PT_DROP         SPD_E07,
  
  SPD_E07    PT_EXECUTE      SPD_E07    SPD_ADD_OP     SPD_E08,
  SPD_E07    PT_DROP         SPD_E08,
  
  SPD_E08    PT_EXECUTE      SPD_HIGH_OP SPD_E09,
  SPD_E08    PT_DROP         SPD_E09,

  SPD_E09    PT_EXECUTE      STK_E09     SPD_E9_OP  SPD_E10,
  SPD_E09    PT_EXECUTE      STK_OFFSET  SPD_E10,
  SPD_E09    PT_EXECUTE      STK_SEG     SPD_E10,
//  SPD_E09    PT_EXECUTE      STK_LROFFSET SPD_E10,
  SPD_E09    PT_EXECUTE      STK_TYPE    SPD_E10,
  SPD_E09    PT_EXECUTE      STK_THIS    SPD_E10,
  SPD_E09    PT_DROP         SPD_E10,

  SPD_E10    PT_EXECUTE      SPD_E10     STK_PERIOD   SPD_E11,
  SPD_E10    PT_DROP         SPD_E11,

  SPD_E11    PT_DROP         STK_OBRACE    SPD_EXPR     STK_CBRACE,
  SPD_E11    PT_DROP         STK_OBRACKET  SPD_EXPR     STK_CBRACKET,
  SPD_E11    PT_EXECUTE      STK_WIDTH     STK_IDENTIFIER,
  SPD_E11    PT_EXECUTE      STK_MASK      STK_IDENTIFIER,
  SPD_E11    PT_EXECUTE      STK_SIZE      SPD_SIZE_ARG,
//  SPD_E11    PT_EXECUTE      STK_SIZEOF    SPD_SIZE_ARG,
  SPD_E11    PT_EXECUTE      STK_LENGTH     STK_IDENTIFIER,
  SPD_E11    PT_EXECUTE      STK_LENGTHOF   STK_IDENTIFIER,
//  SPD_E11    PT_DROP         SPD_RECORD_CONST,
  SPD_E11    PT_DROP         STK_STRING,
  SPD_E11    PT_DROP         SPD_CONSTANT,
//  SPD_E11    PT_DROP         SPD_TYPE,
  SPD_E11    PT_DROP         STK_IDENTIFIER,
  SPD_E11    PT_DROP         STK_DOLLAR,
  SPD_E11    PT_DROP         SPD_SEGMENT_REGISTER,
  SPD_E11    PT_DROP         SPD_GP_REGISTER,
  SPD_E11    PT_DROP         SPD_BYTE_REGISTER,
  SPD_E11    PT_DROP         STK_ST STK_OBRACE SPD_EXPR STK_CBRACE,
  SPD_E11    PT_DROP         STK_ST,


  SPD_db          PT_EXECUTE      SPD_NUM_EXP,
  
  SPD_dw          PT_EXECUTE      SPD_NUM_EXP,
  
  SPD_DISP        PT_IGNORE       SPD_NUM_EXP,
  
/*
 * ----------
 * Statements
 * ----------
 */

  SPD_WARNING     PT_EXECUTE      STK_WARNING     SPD_NUM_EXP,
  

  SPD_GP_REGISTER PT_EXECUTE      STK_AX,
  SPD_GP_REGISTER PT_EXECUTE      STK_CX,
  SPD_GP_REGISTER PT_EXECUTE      STK_DX,
  SPD_GP_REGISTER PT_EXECUTE      STK_BX,
  SPD_GP_REGISTER PT_EXECUTE      STK_SP,
  SPD_GP_REGISTER PT_EXECUTE      STK_BP,
  SPD_GP_REGISTER PT_EXECUTE      STK_SI,
  SPD_GP_REGISTER PT_EXECUTE      STK_DI,

  SPD_BYTE_REGISTER PT_EXECUTE      STK_AL,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_CL,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_DL,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_BL,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_AH,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_CH,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_DH,
  SPD_BYTE_REGISTER PT_EXECUTE      STK_BH,

  SPD_IND_BX      PT_IGNORE       STK_OBRACKET      STK_BX          STK_CBRACKET,
  SPD_IND_BP      PT_IGNORE       STK_OBRACKET      STK_BP          STK_CBRACKET,
  SPD_IND_SI      PT_IGNORE       STK_OBRACKET      STK_SI          STK_CBRACKET,
  SPD_IND_DI      PT_IGNORE       STK_OBRACKET      STK_DI          STK_CBRACKET,

  SPD_rm          PT_IGNORE       SPD_IND_BX      STK_PLUS        SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_BX      STK_PLUS        SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_BP      STK_PLUS        SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_BP      STK_PLUS        SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_SI,
  SPD_rm          PT_IGNORE       SPD_IND_DI,
  SPD_rm          PT_IGNORE       SPD_IND_BP,
  SPD_rm          PT_IGNORE       SPD_IND_BX,
  
  SPD_MEMPTR      PT_IGNORE       STK_BYTE        STK_PTR,
  SPD_MEMPTR      PT_IGNORE       STK_WORD        STK_PTR,

  SPD_rm_disp     PT_IGNORE       SPD_MEMPTR      SPD_rm          STK_PLUS        SPD_NUM_EXP,
  SPD_rm_disp     PT_IGNORE       SPD_MEMPTR      SPD_rm,
  SPD_rm_disp     PT_IGNORE       SPD_rm          STK_PLUS        SPD_NUM_EXP,
  SPD_rm_disp     PT_IGNORE       SPD_rm,

//  SPD_eb          PT_IGNORE       SPD_rb,
//  SPD_eb          PT_IGNORE       SPD_rm,

//  SPD_ew          PT_IGNORE       SPD_rw,
//  SPD_ew          PT_IGNORE       SPD_rm,
  
  SPD_SEGMENT_REGISTER          PT_IGNORE       STK_ES,
  SPD_SEGMENT_REGISTER          PT_IGNORE       STK_CS,
  SPD_SEGMENT_REGISTER          PT_IGNORE       STK_SS,
  SPD_SEGMENT_REGISTER          PT_IGNORE       STK_DS,

  SPD_ALUOP       PT_IGNORE       STK_ADD,
  SPD_ALUOP       PT_IGNORE       STK_OR,
  SPD_ALUOP       PT_IGNORE       STK_ADC,
  SPD_ALUOP       PT_IGNORE       STK_SBB,
  SPD_ALUOP       PT_IGNORE       STK_AND,
  SPD_ALUOP       PT_IGNORE       STK_SUB,
  SPD_ALUOP       PT_IGNORE       STK_XOR,
  SPD_ALUOP       PT_IGNORE       STK_CMP,
  
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       STK_AL          STK_COMMA       SPD_NUM_EXP,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       STK_AX          STK_COMMA       SPD_NUM_EXP,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rb          STK_COMMA       SPD_rb,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rw          STK_COMMA       SPD_rw,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rb          STK_COMMA       SPD_rm_disp,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rw          STK_COMMA       SPD_rm_disp,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_rb,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_rw,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rb          STK_COMMA       SPD_NUM_EXP,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rw          STK_COMMA       SPD_NUM_EXP,
  SPD_ALU         PT_EXECUTE      SPD_ALUOP       SPD_rm_disp     STK_COMMA       SPD_NUM_EXP,

  SPD_ROTOP       PT_IGNORE       STK_ROL,
  SPD_ROTOP       PT_IGNORE       STK_ROR,
  SPD_ROTOP       PT_IGNORE       STK_RCL,
  SPD_ROTOP       PT_IGNORE       STK_RCR,
  SPD_ROTOP       PT_IGNORE       STK_SAL,
  SPD_ROTOP       PT_IGNORE       STK_SHR,
  SPD_ROTOP       PT_IGNORE       STK_SHR, // deliberate duplicate
  SPD_ROTOP       PT_IGNORE       STK_SAR,
  
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rb          STK_COMMA       SPD_NUM_EXP,
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rw          STK_COMMA       SPD_NUM_EXP,
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rm_disp     STK_COMMA       SPD_NUM_EXP,
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rb          STK_COMMA       STK_CL,
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rw          STK_COMMA       STK_CL,
  SPD_ROTATE      PT_EXECUTE      SPD_ROTOP       SPD_rm_disp     STK_COMMA       STK_CL,

  SPD_BOUND       PT_EXECUTE      STK_BOUND       SPD_rw          STK_COMMA       SPD_rm_disp,
  
  SPD_ARPL        PT_EXECUTE      STK_ARPL        SPD_rm_disp     STK_COMMA       SPD_rw,
  SPD_ARPL        PT_EXECUTE      STK_ARPL        SPD_rw          STK_COMMA       SPD_rw,

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
