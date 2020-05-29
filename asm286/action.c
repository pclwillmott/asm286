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

  SPD_module PT_IGNORE SPD_directiveList SPD_endDir,
  SPD_module PT_IGNORE SPD_endDir,

  SPD_endDir PT_EXECUTE STK_END SPD_immExpr STK_NEWLINE,
  SPD_endDir PT_IGNORE STK_END STK_NEWLINE,

  SPD_directiveList PT_IGNORE SPD_directiveList SPD_directive,
  SPD_directiveList PT_IGNORE SPD_directive,

  SPD_directive PT_IGNORE SPD_generalDir,
  SPD_directive PT_IGNORE SPD_segmentDef,
  SPD_directive PT_IGNORE STK_NEWLINE,

  SPD_segmentDef PT_IGNORE SPD_segmentDir SPD_inSegDirList SPD_endsDir,
  SPD_segmentDef PT_IGNORE SPD_segmentDir SPD_endsDir,
  SPD_segmentDef PT_IGNORE SPD_simpleSegDir SPD_inSegDirList SPD_endsDir,
  SPD_segmentDef PT_IGNORE SPD_simpleSegDir SPD_endsDir,
  SPD_segmentDef PT_IGNORE SPD_simpleSegDir,

  SPD_simpleSegDir PT_IGNORE SPD_segDir STK_NEWLINE,
  
  SPD_segDir PT_EXECUTE STK_CODE SPD_segId,
  SPD_segDir PT_EXECUTE STK_CODE,
  SPD_segDir PT_EXECUTE STK_DATAQ,
  SPD_segDir PT_EXECUTE STK_DATA,
  SPD_segDir PT_EXECUTE STK_CONST,
  SPD_segDir PT_EXECUTE STK_FARDATAQ SPD_segId,
  SPD_segDir PT_EXECUTE STK_FARDATAQ,
  SPD_segDir PT_EXECUTE STK_FARDATA SPD_segId,
  SPD_segDir PT_EXECUTE STK_FARDATA,
  SPD_segDir PT_EXECUTE STK_DOTSTACK SPD_constExpr,
  SPD_segDir PT_EXECUTE STK_DOTSTACK,
  
  SPD_endsDir PT_EXECUTE SPD_segId STK_ENDS STK_NEWLINE,
  
  SPD_segmentDir PT_EXECUTE SPD_segId STK_SEGMENT SPD_segOptionList STK_NEWLINE,
  SPD_segmentDir PT_EXECUTE SPD_segId STK_SEGMENT STK_NEWLINE,

  SPD_segId PT_DROP STK_IDENTIFIER,
  
  SPD_segOptionList PT_IGNORE SPD_segOptionList SPD_segOption,
  SPD_segOptionList PT_IGNORE SPD_segOption,

  SPD_segOption PT_IGNORE SPD_segAlign,
  SPD_segOption PT_IGNORE SPD_segAttrib,
  SPD_segOption PT_IGNORE SPD_className,
  SPD_segOption PT_EXECUTE STK_READONLY,

  SPD_segAlign PT_EXECUTE STK_BYTE,
  SPD_segAlign PT_EXECUTE STK_WORD,
  SPD_segAlign PT_EXECUTE STK_DWORD,
  SPD_segAlign PT_EXECUTE STK_PARA,
  SPD_segAlign PT_EXECUTE STK_PAGE,

  SPD_segAttrib PT_EXECUTE STK_PRIVATE,
  SPD_segAttrib PT_EXECUTE STK_PUBLIC,
  SPD_segAttrib PT_EXECUTE STK_STACK,
  SPD_segAttrib PT_EXECUTE STK_COMMON,
  SPD_segAttrib PT_EXECUTE STK_AT SPD_constExpr,
  SPD_segAttrib PT_EXECUTE STK_MEMORY,

  SPD_className PT_EXECUTE STK_STRING,
  
  SPD_inSegDirList PT_IGNORE SPD_inSegDirList SPD_inSegDir,
  SPD_inSegDirList PT_IGNORE SPD_inSegDir,

  SPD_inSegDir PT_IGNORE SPD_labelDef SPD_inSegmentDir,
  SPD_inSegDir PT_IGNORE SPD_labelDef STK_NEWLINE,
  SPD_inSegDir PT_IGNORE SPD_inSegmentDir,

  SPD_labelDef PT_EXECUTE STK_INST_LABEL,
  
  SPD_inSegmentDir PT_IGNORE SPD_instruction,
  SPD_inSegmentDir PT_IGNORE SPD_fpInstruction,
  SPD_inSegmentDir PT_IGNORE SPD_dataDir,
  SPD_inSegmentDir PT_IGNORE SPD_offsetDir,
  SPD_inSegmentDir PT_IGNORE SPD_labelDir,
  SPD_inSegmentDir PT_IGNORE SPD_generalDir,
  SPD_inSegmentDir PT_IGNORE SPD_segmentDef,

  SPD_instruction PT_IGNORE SPD_instPrefix SPD_asmInstruction,
  SPD_instruction PT_IGNORE SPD_asmInstruction,
  
  SPD_dataDir PT_IGNORE STK_IDENTIFIER SPD_dataItem STK_NEWLINE,
  SPD_dataDir PT_IGNORE SPD_dataItem STK_NEWLINE,

  SPD_dataItem PT_IGNORE SPD_dataDecl SPD_scalarInstList,
  
  SPD_dataDecl PT_IGNORE STK_DB,
  SPD_dataDecl PT_IGNORE STK_DW,
  SPD_dataDecl PT_IGNORE STK_DD,
  SPD_dataDecl PT_IGNORE STK_DF,
  SPD_dataDecl PT_IGNORE STK_DQ,
  SPD_dataDecl PT_IGNORE STK_DT,
  SPD_dataDecl PT_IGNORE SPD_dataType,

  SPD_scalarInstList PT_IGNORE SPD_scalarInstList SPD_commaOptNewline SPD_initValue,
  SPD_scalarInstList PT_IGNORE SPD_initValue,
  
  SPD_commaOptNewline PT_IGNORE STK_COMMA STK_NEWLINE,
  SPD_commaOptNewline PT_IGNORE STK_COMMA,

  SPD_initValue PT_IGNORE SPD_immExpr,
  SPD_initValue PT_IGNORE STK_STRING,
  SPD_initValue PT_IGNORE STK_QMARK,
  SPD_initValue PT_IGNORE SPD_constExpr STK_DUP STK_OBRACE SPD_scalarInstList STK_CBRACE,
  SPD_initValue PT_IGNORE STK_DOUBLE,
//  SPD_initValue PT_IGNORE SPD_bcdConstant,

  SPD_dataType PT_IGNORE STK_BYTE,
  SPD_dataType PT_IGNORE STK_SBYTE,
  SPD_dataType PT_IGNORE STK_WORD,
  SPD_dataType PT_IGNORE STK_SWORD,
  SPD_dataType PT_IGNORE STK_DWORD,
  SPD_dataType PT_IGNORE STK_SDWORD,
  SPD_dataType PT_IGNORE STK_FWORD,
  SPD_dataType PT_IGNORE STK_QWORD,
  SPD_dataType PT_IGNORE STK_TBYTE,
  SPD_dataType PT_IGNORE STK_REAL4,
  SPD_dataType PT_IGNORE STK_REAL8,
  SPD_dataType PT_IGNORE STK_REAL10,
  
  SPD_instPrefix PT_IGNORE STK_REPE,
  SPD_instPrefix PT_IGNORE STK_REPZ,
  SPD_instPrefix PT_IGNORE STK_REPNE,
  SPD_instPrefix PT_IGNORE STK_REPNZ,
  SPD_instPrefix PT_IGNORE STK_REP,
  SPD_instPrefix PT_IGNORE STK_LOCK,

  SPD_offsetDir PT_IGNORE SPD_offsetDirType STK_NEWLINE,
  
  SPD_offsetDirType PT_IGNORE STK_EVEN,
  SPD_offsetDirType PT_IGNORE STK_ORG SPD_immExpr,
  SPD_offsetDirType PT_IGNORE STK_ALIGN SPD_constExpr,

  SPD_labelDir PT_IGNORE STK_IDENTIFIER STK_LABEL SPD_qualifiedType STK_NEWLINE,
  
  SPD_qualifiedType PT_IGNORE SPD_nearfar STK_PTR SPD_type,
  
  SPD_nearfar PT_IGNORE STK_NEAR,
  SPD_nearfar PT_IGNORE STK_FAR,
  
  SPD_type PT_EXECUTE SPD_dataType,
  
  SPD_generalDir PT_IGNORE SPD_echoDir,
  SPD_generalDir PT_IGNORE SPD_equalDir,
  SPD_generalDir PT_IGNORE SPD_equDir,
  SPD_generalDir PT_IGNORE SPD_publicDir,
  SPD_generalDir PT_IGNORE SPD_modelDir,
  SPD_generalDir PT_IGNORE SPD_ifDir,
  SPD_generalDir PT_IGNORE SPD_nameDir,
  SPD_generalDir PT_IGNORE SPD_groupDir,
  SPD_generalDir PT_IGNORE SPD_assumeDir,
  SPD_generalDir PT_IGNORE SPD_externDir,
  SPD_generalDir PT_IGNORE SPD_textEquDir,
  SPD_generalDir PT_IGNORE SPD_titleDir,
  SPD_generalDir PT_IGNORE SPD_pageDir,
  SPD_generalDir PT_IGNORE SPD_listDir,
  SPD_generalDir PT_IGNORE SPD_processorDir,
  SPD_generalDir PT_IGNORE SPD_optionDir,

  SPD_modelDir PT_EXECUTE STK_MODEL SPD_memOption STK_COMMA SPD_modelOptList STK_NEWLINE,
  SPD_modelDir PT_EXECUTE STK_MODEL SPD_memOption STK_NEWLINE,

  SPD_memOption PT_IGNORE STK_TINY,
  SPD_memOption PT_IGNORE STK_SMALL,
  SPD_memOption PT_IGNORE STK_MEDIUM,
  SPD_memOption PT_IGNORE STK_COMPACT,
  SPD_memOption PT_IGNORE STK_LARGE,
  SPD_memOption PT_IGNORE STK_HUGE,

  SPD_modelOptList PT_IGNORE SPD_modelOptList STK_COMMA SPD_modelOpt,
  SPD_modelOptList PT_IGNORE SPD_modelOpt,

  SPD_modelOpt PT_EXECUTE SPD_langType,
  SPD_modelOpt PT_EXECUTE SPD_stackOption,
  
  SPD_langType PT_IGNORE STK_C,
  SPD_langType PT_IGNORE STK_PASCAL,

  SPD_stackOption PT_IGNORE STK_NEARSTACK,
  SPD_stackOption PT_IGNORE STK_FARSTACK,

  SPD_nameDir PT_IGNORE STK_NAME STK_IDENTIFIER STK_NEWLINE,
  
  SPD_groupDir PT_IGNORE SPD_groupId STK_GROUP SPD_segIdList,
  
  SPD_groupId PT_IGNORE STK_IDENTIFIER,
  
  SPD_segIdList PT_IGNORE SPD_segIdList STK_COMMA SPD_segId,
  SPD_segIdList PT_IGNORE SPD_segId,

  SPD_assumeDir PT_IGNORE STK_ASSUME SPD_assumeList STK_NEWLINE,
  SPD_assumeDir PT_EXECUTE STK_ASSUME STK_NOTHING STK_NEWLINE,

  SPD_assumeList PT_IGNORE SPD_assumeList SPD_assumeRegister,
  SPD_assumeList PT_IGNORE SPD_assumeRegister,

  SPD_assumeRegister PT_IGNORE SPD_assumeSegReg,
  SPD_assumeRegister PT_IGNORE SPD_assumeReg,

  SPD_assumeSegReg PT_IGNORE SPD_segmentRegister STK_COLON SPD_assumeSegVal,
  
  SPD_assumeSegVal PT_IGNORE SPD_frameExpr,
  SPD_assumeSegVal PT_IGNORE STK_NOTHING,
  SPD_assumeSegVal PT_IGNORE STK_ERROR,
  
  SPD_frameExpr PT_IGNORE STK_SEG STK_IDENTIFIER,
  SPD_frameExpr PT_IGNORE STK_DGROUP STK_COLON STK_IDENTIFIER,
  SPD_frameExpr PT_IGNORE SPD_segmentRegister STK_COLON STK_IDENTIFIER,
  SPD_frameExpr PT_IGNORE STK_IDENTIFIER,

  SPD_segmentRegister PT_IGNORE STK_CS,
  SPD_segmentRegister PT_IGNORE STK_DS,
  SPD_segmentRegister PT_IGNORE STK_ES,
  SPD_segmentRegister PT_IGNORE STK_SS,
  
  SPD_assumeReg PT_IGNORE SPD_register STK_COLON SPD_assumeVal,
  
  SPD_register PT_IGNORE SPD_gpRegister,
  SPD_register PT_IGNORE SPD_byteRegister,

  SPD_gpRegister PT_IGNORE STK_AX,
  SPD_gpRegister PT_IGNORE STK_BX,
  SPD_gpRegister PT_IGNORE STK_CX,
  SPD_gpRegister PT_IGNORE STK_DX,
  SPD_gpRegister PT_IGNORE STK_BP,
  SPD_gpRegister PT_IGNORE STK_SP,
  SPD_gpRegister PT_IGNORE STK_DI,
  SPD_gpRegister PT_IGNORE STK_SI,
  
  SPD_byteRegister PT_IGNORE STK_AL,
  SPD_byteRegister PT_IGNORE STK_AH,
  SPD_byteRegister PT_IGNORE STK_BL,
  SPD_byteRegister PT_IGNORE STK_BH,
  SPD_byteRegister PT_IGNORE STK_CL,
  SPD_byteRegister PT_IGNORE STK_CH,
  SPD_byteRegister PT_IGNORE STK_DL,
  SPD_byteRegister PT_IGNORE STK_DH,

  SPD_assumeVal PT_IGNORE SPD_qualifiedType,
  SPD_assumeVal PT_IGNORE STK_NOTHING,
  SPD_assumeVal PT_IGNORE STK_ERROR,

  SPD_externDir PT_IGNORE STK_EXTERN SPD_externList STK_NEWLINE,
  
  SPD_externList PT_IGNORE SPD_externList SPD_commaOptNewline SPD_externDef,
  SPD_externList PT_IGNORE SPD_externDef,

  SPD_externDef PT_IGNORE SPD_langType STK_IDENTIFIER STK_COLON SPD_externType,
  SPD_externDef PT_IGNORE STK_IDENTIFIER STK_COLON SPD_externType,

  SPD_externType PT_IGNORE STK_ABS,
  SPD_externType PT_IGNORE SPD_qualifiedType,

  SPD_publicDir PT_IGNORE STK_PUBLIC SPD_pubList STK_NEWLINE,
  
  SPD_pubList PT_IGNORE SPD_pubList SPD_commaOptNewline SPD_pubDef,
  SPD_pubList PT_IGNORE SPD_pubDef,

  SPD_pubDef PT_EXECUTE SPD_langType STK_IDENTIFIER,
  SPD_pubDef PT_EXECUTE STK_IDENTIFIER,

  SPD_equDir PT_EXECUTE SPD_equId STK_EQU SPD_immExpr STK_NEWLINE,
  
  SPD_equId PT_DROP STK_IDENTIFIER,
  
  SPD_textEquDir PT_EXECUTE SPD_equId STK_TEXTEQU SPD_textList STK_NEWLINE,
  
  SPD_textList PT_EXECUTE SPD_textList SPD_commaOptNewline SPD_textItem,
  SPD_textList PT_DROP SPD_textItem,

  SPD_textItem PT_EXECUTE STK_PERCENT SPD_constExpr,
  SPD_textItem PT_DROP STK_STRING,
  SPD_textItem PT_EXECUTE SPD_equId,

  SPD_equalDir PT_EXECUTE STK_IDENTIFIER STK_EQUAL SPD_immExpr STK_NEWLINE,
  
  SPD_processor PT_IGNORE STK_8086,
  SPD_processor PT_IGNORE STK_186,
  SPD_processor PT_IGNORE STK_286P,
  SPD_processor PT_IGNORE STK_286,

  SPD_processorDir PT_EXECUTE SPD_processor STK_NEWLINE,
  SPD_processorDir PT_EXECUTE SPD_coprocessor STK_NEWLINE,

  SPD_coprocessor PT_IGNORE STK_NO87,
  SPD_coprocessor PT_IGNORE STK_8087,
  SPD_coprocessor PT_IGNORE STK_287,

  SPD_titleDir PT_IGNORE SPD_titleType SPD_arbitaryText STK_NEWLINE,
  
  SPD_titleType PT_IGNORE STK_TITLE,
  SPD_titleType PT_IGNORE STK_SUBTITLE,
  
  SPD_arbitaryText PT_DROP STK_TEXT,

  SPD_pageDir PT_IGNORE STK_PAGE SPD_pageExpr STK_NEWLINE,
  SPD_pageDir PT_IGNORE STK_PAGE STK_NEWLINE,

  SPD_pageExpr PT_IGNORE STK_PLUS,
  SPD_pageExpr PT_IGNORE SPD_pageLength STK_COMMA SPD_pageWidth,
  SPD_pageExpr PT_IGNORE SPD_pageLength,

  SPD_pageLength PT_IGNORE SPD_constExpr,
  
  SPD_pageWidth PT_IGNORE SPD_constExpr,
  
  SPD_listDir PT_IGNORE SPD_listOption STK_NEWLINE,
  
  SPD_listOption PT_IGNORE STK_LIST,
  SPD_listOption PT_IGNORE STK_NOLIST,

  SPD_echoDir PT_EXECUTE STK_ECHO SPD_textList STK_NEWLINE,
  SPD_echoDir PT_EXECUTE STK_ECHO SPD_arbitaryText STK_NEWLINE,

  SPD_e01 PT_EXECUTE SPD_e01 SPD_orOp SPD_e02,
  SPD_e01 PT_DROP SPD_e02,

  SPD_orOp PT_IGNORE STK_OR,
  SPD_orOp PT_IGNORE STK_XOR,

  SPD_e02 PT_EXECUTE SPD_e02 SPD_andOp SPD_e03,
  SPD_e02 PT_DROP SPD_e03,

  SPD_andOp PT_IGNORE STK_AND,

  SPD_e03 PT_EXECUTE STK_NOT SPD_e04,
  SPD_e03 PT_DROP SPD_e04,

  SPD_e04 PT_EXECUTE SPD_e04 SPD_relOp SPD_e05,
  SPD_e04 PT_DROP SPD_e05,
  
  SPD_relOp PT_IGNORE STK_EQ,
  SPD_relOp PT_IGNORE STK_NE,
  SPD_relOp PT_IGNORE STK_LT,
  SPD_relOp PT_IGNORE STK_LE,
  SPD_relOp PT_IGNORE STK_GT,
  SPD_relOp PT_IGNORE STK_GE,

  SPD_e05 PT_EXECUTE SPD_e05 SPD_addOp SPD_e06,
  SPD_e05 PT_DROP SPD_e06,
  
  SPD_addOp PT_IGNORE STK_PLUS,
  SPD_addOp PT_IGNORE STK_MINUS,

  SPD_e06 PT_EXECUTE SPD_e06 SPD_mulOp SPD_e07,
  SPD_e06 PT_DROP SPD_e07,
  
  SPD_mulOp PT_IGNORE STK_MULTIPLY,
  SPD_mulOp PT_IGNORE STK_DIVIDE,
  SPD_mulOp PT_IGNORE STK_SHR,
  SPD_mulOp PT_IGNORE STK_SHL,

  SPD_e07   PT_EXECUTE SPD_addOp SPD_e08,
  SPD_e07   PT_DROP SPD_e08,

  SPD_e08 PT_EXECUTE SPD_e08Op SPD_e09,
  SPD_e08 PT_DROP SPD_e09,
  
  SPD_e08Op PT_IGNORE STK_HIGH,
  SPD_e08Op PT_IGNORE STK_LOW,

  SPD_e09 PT_EXECUTE STK_OFFSET SPD_e10, // 0
  SPD_e09 PT_EXECUTE STK_SEG SPD_e10,    // 1
  SPD_e09 PT_EXECUTE STK_TYPE SPD_e10,   // 2
  SPD_e09 PT_EXECUTE STK_THIS SPD_e10,   // 3
  SPD_e09 PT_DROP SPD_e10,

  SPD_e10 PT_EXECUTE SPD_e10 STK_OBRACKET SPD_expr STK_CBRACKET,
  SPD_e10 PT_DROP SPD_e11,
  
  SPD_e11 PT_EXECUTE STK_WIDTH STK_IDENTIFIER,              // 0
  SPD_e11 PT_EXECUTE STK_MASK STK_IDENTIFIER,               // 1
  SPD_e11 PT_EXECUTE STK_SIZEOF SPD_sizeArg,                // 2
  SPD_e11 PT_EXECUTE STK_LENGTHOF STK_IDENTIFIER,           // 3
  SPD_e11 PT_EXECUTE STK_DEFINED STK_IDENTIFIER,            // 4
  SPD_e11 PT_DROP SPD_constant,                             // 5
  SPD_e11 PT_EXECUTE STK_DOLLAR,                            // 6
  SPD_e11 PT_EXECUTE SPD_segmentRegister,                   // 7
  SPD_e11 PT_EXECUTE SPD_register,                          // 8
  SPD_e11 PT_EXECUTE STK_ST STK_OBRACE SPD_expr STK_CBRACE, // 9
  SPD_e11 PT_EXECUTE STK_ST,                                // 10
  SPD_e11 PT_EXECUTE STK_IDENTIFIER,                        // 11
  SPD_e11 PT_EXECUTE STK_STRING,                            // 12
  SPD_e11 PT_DROP    SPD_e12,                               // 13

  SPD_e12 PT_EXECUTE STK_OBRACE SPD_expr STK_CBRACE,        // 0
  SPD_e12 PT_EXECUTE STK_OBRACKET SPD_expr STK_CBRACKET,    // 1

  SPD_expr PT_EXECUTE STK_SHORT SPD_e01,
  SPD_expr PT_DROP SPD_e01,

  SPD_constExpr PT_DROP SPD_expr,

  SPD_immExpr PT_DROP SPD_expr,

  SPD_sizeArg PT_DROP SPD_type,
  SPD_sizeArg PT_DROP STK_IDENTIFIER,

  SPD_constant PT_DROP STK_INTEGERHEX,
  SPD_constant PT_DROP STK_INTEGERBIN,
  SPD_constant PT_DROP STK_INTEGEROCT,
  SPD_constant PT_DROP STK_INTEGERDEC,
  SPD_constant PT_DROP SPD_bool,

  SPD_bool PT_EXECUTE STK_FALSE,
  SPD_bool PT_EXECUTE STK_TRUE,
  
  SPD_asmInstruction PT_DROP SPD_mnemonicTwo SPD_operand STK_COMMA SPD_operand STK_NEWLINE,
  SPD_asmInstruction PT_DROP SPD_mnemonicOne SPD_operand STK_NEWLINE,
  SPD_asmInstruction PT_DROP SPD_mnemonicZero STK_NEWLINE,

  SPD_fpInstruction PT_DROP SPD_fpMnemonicTwo SPD_operand STK_COMMA SPD_operand STK_NEWLINE,
  SPD_fpInstruction PT_DROP SPD_fpMnemonicOne SPD_operand STK_NEWLINE,
  SPD_fpInstruction PT_DROP SPD_fpMnemonicZero STK_NEWLINE,

  SPD_operand PT_DROP SPD_expr,
  
  SPD_mnemonicZero PT_EXECUTE STK_AAA,
  SPD_mnemonicZero PT_EXECUTE STK_AAD,
  SPD_mnemonicZero PT_EXECUTE STK_AAM,
  SPD_mnemonicZero PT_EXECUTE STK_AAS,
  SPD_mnemonicZero PT_EXECUTE STK_CBW,
  SPD_mnemonicZero PT_EXECUTE STK_CLC,
  SPD_mnemonicZero PT_EXECUTE STK_CLD,
  SPD_mnemonicZero PT_EXECUTE STK_CLI,
  SPD_mnemonicZero PT_EXECUTE STK_CLTS,
  SPD_mnemonicZero PT_EXECUTE STK_CMC,
  SPD_mnemonicZero PT_EXECUTE STK_CWD,
  SPD_mnemonicZero PT_EXECUTE STK_DAA,
  SPD_mnemonicZero PT_EXECUTE STK_DAS,
  SPD_mnemonicZero PT_EXECUTE STK_HLT,
  SPD_mnemonicZero PT_EXECUTE STK_INTO,
  SPD_mnemonicZero PT_EXECUTE STK_IRET,
  SPD_mnemonicZero PT_EXECUTE STK_LAHF,
  SPD_mnemonicZero PT_EXECUTE STK_LEAVE,
  SPD_mnemonicZero PT_EXECUTE STK_NOP,
  SPD_mnemonicZero PT_EXECUTE STK_POPA,
  SPD_mnemonicZero PT_EXECUTE STK_POPF,
  SPD_mnemonicZero PT_EXECUTE STK_PUSHA,
  SPD_mnemonicZero PT_EXECUTE STK_PUSHF,
  SPD_mnemonicZero PT_EXECUTE STK_RET,
  SPD_mnemonicZero PT_EXECUTE STK_SAHF,
  SPD_mnemonicZero PT_EXECUTE STK_STC,
  SPD_mnemonicZero PT_EXECUTE STK_STD,
  SPD_mnemonicZero PT_EXECUTE STK_STI,
  SPD_mnemonicZero PT_EXECUTE STK_WAIT,

  SPD_mnemonicOne PT_IGNORE STK_CALL,
  SPD_mnemonicOne PT_IGNORE STK_DEC,
  SPD_mnemonicOne PT_IGNORE STK_DIV,
  SPD_mnemonicOne PT_IGNORE STK_IDIV,
  SPD_mnemonicOne PT_IGNORE STK_IMUL,
  SPD_mnemonicOne PT_IGNORE STK_INC,
  SPD_mnemonicOne PT_IGNORE STK_INT,
  SPD_mnemonicOne PT_IGNORE STK_JAE,
  SPD_mnemonicOne PT_IGNORE STK_JA,
  SPD_mnemonicOne PT_IGNORE STK_JNBE,
  SPD_mnemonicOne PT_IGNORE STK_JNB,
  SPD_mnemonicOne PT_IGNORE STK_JBE,
  SPD_mnemonicOne PT_IGNORE STK_JB,
  SPD_mnemonicOne PT_IGNORE STK_JNAE,
  SPD_mnemonicOne PT_IGNORE STK_JNA,
  SPD_mnemonicOne PT_IGNORE STK_JCXZ,
  SPD_mnemonicOne PT_IGNORE STK_JC,
  SPD_mnemonicOne PT_IGNORE STK_JE,
  SPD_mnemonicOne PT_IGNORE STK_JZ,
  SPD_mnemonicOne PT_IGNORE STK_JG,
  SPD_mnemonicOne PT_IGNORE STK_JNLE,
  SPD_mnemonicOne PT_IGNORE STK_JGE,
  SPD_mnemonicOne PT_IGNORE STK_JNL,
  SPD_mnemonicOne PT_IGNORE STK_JLE,
  SPD_mnemonicOne PT_IGNORE STK_JL,
  SPD_mnemonicOne PT_IGNORE STK_JNGE,
  SPD_mnemonicOne PT_IGNORE STK_JNG,
  SPD_mnemonicOne PT_IGNORE STK_JMP,
  SPD_mnemonicOne PT_IGNORE STK_JNC,
  SPD_mnemonicOne PT_IGNORE STK_JNE,
  SPD_mnemonicOne PT_IGNORE STK_JNZ,
  SPD_mnemonicOne PT_IGNORE STK_JNO,
  SPD_mnemonicOne PT_IGNORE STK_JNS,
  SPD_mnemonicOne PT_IGNORE STK_JNP,
  SPD_mnemonicOne PT_IGNORE STK_JPO,
  SPD_mnemonicOne PT_IGNORE STK_JO,
  SPD_mnemonicOne PT_IGNORE STK_JPE,
  SPD_mnemonicOne PT_IGNORE STK_JP,
  SPD_mnemonicOne PT_IGNORE STK_JS,
  SPD_mnemonicOne PT_IGNORE STK_LGDT,
  SPD_mnemonicOne PT_IGNORE STK_LIDT,
  SPD_mnemonicOne PT_IGNORE STK_LLDT,
  SPD_mnemonicOne PT_IGNORE STK_LMSW,
  SPD_mnemonicOne PT_IGNORE STK_LODSB,
  SPD_mnemonicOne PT_IGNORE STK_LODSW,
  SPD_mnemonicOne PT_IGNORE STK_LODS,
  SPD_mnemonicOne PT_IGNORE STK_LOOPE,
  SPD_mnemonicOne PT_IGNORE STK_LOOPZ,
  SPD_mnemonicOne PT_IGNORE STK_LOOPNZ,
  SPD_mnemonicOne PT_IGNORE STK_LOOPNE,
  SPD_mnemonicOne PT_IGNORE STK_LOOP,
  SPD_mnemonicOne PT_IGNORE STK_LTR,
  SPD_mnemonicOne PT_IGNORE STK_MOVSB,
  SPD_mnemonicOne PT_IGNORE STK_MOVSW,
  SPD_mnemonicOne PT_IGNORE STK_MOVS,
  SPD_mnemonicOne PT_IGNORE STK_MUL,
  SPD_mnemonicOne PT_IGNORE STK_NEG,
  SPD_mnemonicOne PT_IGNORE STK_NOT,
  SPD_mnemonicOne PT_IGNORE STK_POP,
  SPD_mnemonicOne PT_IGNORE STK_PUSH,
  SPD_mnemonicOne PT_IGNORE STK_RET,
  SPD_mnemonicOne PT_IGNORE STK_SCASB,
  SPD_mnemonicOne PT_IGNORE STK_SCASW,
  SPD_mnemonicOne PT_IGNORE STK_SCAS,
  SPD_mnemonicOne PT_IGNORE STK_SGDT,
  SPD_mnemonicOne PT_IGNORE STK_SIDT,
  SPD_mnemonicOne PT_IGNORE STK_SLDT,
  SPD_mnemonicOne PT_IGNORE STK_SMSW,
  SPD_mnemonicOne PT_IGNORE STK_STOSB,
  SPD_mnemonicOne PT_IGNORE STK_STOSW,
  SPD_mnemonicOne PT_IGNORE STK_STOS,
  SPD_mnemonicOne PT_IGNORE STK_STR,
  SPD_mnemonicOne PT_IGNORE STK_VERR,
  SPD_mnemonicOne PT_IGNORE STK_VERW,
  SPD_mnemonicOne PT_IGNORE STK_XLAT,

  SPD_mnemonicTwo PT_IGNORE STK_ADC,
  SPD_mnemonicTwo PT_IGNORE STK_ADD,
  SPD_mnemonicTwo PT_IGNORE STK_AND,
  SPD_mnemonicTwo PT_IGNORE STK_ARPL,
  SPD_mnemonicTwo PT_IGNORE STK_BOUND,
  SPD_mnemonicTwo PT_IGNORE STK_CMPSW,
  SPD_mnemonicTwo PT_IGNORE STK_CMPSB,
  SPD_mnemonicTwo PT_IGNORE STK_CMPS,
  SPD_mnemonicTwo PT_IGNORE STK_CMP,
  SPD_mnemonicTwo PT_IGNORE STK_ENTER,
  SPD_mnemonicTwo PT_IGNORE STK_ESC,
  SPD_mnemonicTwo PT_IGNORE STK_INSB,
  SPD_mnemonicTwo PT_IGNORE STK_INSW,
  SPD_mnemonicTwo PT_IGNORE STK_INS,
  SPD_mnemonicTwo PT_IGNORE STK_IN,
  SPD_mnemonicTwo PT_IGNORE STK_LAR,
  SPD_mnemonicTwo PT_IGNORE STK_LDS,
  SPD_mnemonicTwo PT_IGNORE STK_LEA,
  SPD_mnemonicTwo PT_IGNORE STK_LES,
  SPD_mnemonicTwo PT_IGNORE STK_LSL,
  SPD_mnemonicTwo PT_IGNORE STK_MOV,
  SPD_mnemonicTwo PT_IGNORE STK_OR,
  SPD_mnemonicTwo PT_IGNORE STK_OUTSB,
  SPD_mnemonicTwo PT_IGNORE STK_OUTSW,
  SPD_mnemonicTwo PT_IGNORE STK_OUTS,
  SPD_mnemonicTwo PT_IGNORE STK_OUT,
  SPD_mnemonicTwo PT_IGNORE STK_RCL,
  SPD_mnemonicTwo PT_IGNORE STK_RCR,
  SPD_mnemonicTwo PT_IGNORE STK_ROL,
  SPD_mnemonicTwo PT_IGNORE STK_ROR,
  SPD_mnemonicTwo PT_IGNORE STK_SAL,
  SPD_mnemonicTwo PT_IGNORE STK_SHL,
  SPD_mnemonicTwo PT_IGNORE STK_SAR,
  SPD_mnemonicTwo PT_IGNORE STK_SBB,
  SPD_mnemonicTwo PT_IGNORE STK_SHR,
  SPD_mnemonicTwo PT_IGNORE STK_SUB,
  SPD_mnemonicTwo PT_IGNORE STK_TEST,
  SPD_mnemonicTwo PT_IGNORE STK_XCHG,
  SPD_mnemonicTwo PT_IGNORE STK_XOR,

  SPD_fpMnemonicZero PT_IGNORE STK_FABS,
  SPD_fpMnemonicZero PT_IGNORE STK_FCHS,
  SPD_fpMnemonicZero PT_IGNORE STK_FCLEX,
  SPD_fpMnemonicZero PT_IGNORE STK_FNCLEX,
  SPD_fpMnemonicZero PT_IGNORE STK_FCOMPP,
  SPD_fpMnemonicZero PT_IGNORE STK_FDECSTP,
  SPD_fpMnemonicZero PT_IGNORE STK_FDISI,
  SPD_fpMnemonicZero PT_IGNORE STK_FNDISI,
  SPD_fpMnemonicZero PT_IGNORE STK_FENI,
  SPD_fpMnemonicZero PT_IGNORE STK_FNENI,
  SPD_fpMnemonicZero PT_IGNORE STK_FINCSTP,
  SPD_fpMnemonicZero PT_IGNORE STK_FINIT,
  SPD_fpMnemonicZero PT_IGNORE STK_FNINIT,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDLG2,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDLN2,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDL2E,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDL2T,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDPI,
  SPD_fpMnemonicZero PT_IGNORE STK_FLDZ,
  SPD_fpMnemonicZero PT_IGNORE STK_FLD1,
  SPD_fpMnemonicZero PT_IGNORE STK_FNOP,
  SPD_fpMnemonicZero PT_IGNORE STK_FPATAN,
  SPD_fpMnemonicZero PT_IGNORE STK_FPREM,
  SPD_fpMnemonicZero PT_IGNORE STK_FPTAN,
  SPD_fpMnemonicZero PT_IGNORE STK_FRNDINT,
  SPD_fpMnemonicZero PT_IGNORE STK_FSCALE,
  SPD_fpMnemonicZero PT_IGNORE STK_FSETPM,
  SPD_fpMnemonicZero PT_IGNORE STK_FSQRT,
  SPD_fpMnemonicZero PT_IGNORE STK_FTST,
  SPD_fpMnemonicZero PT_IGNORE STK_FWAIT,
  SPD_fpMnemonicZero PT_IGNORE STK_FXAM,
  SPD_fpMnemonicZero PT_IGNORE STK_FXTRACT,
  SPD_fpMnemonicZero PT_IGNORE STK_FYL2X,
  SPD_fpMnemonicZero PT_IGNORE STK_FYL2XP1,
  SPD_fpMnemonicZero PT_IGNORE STK_F2XM1,

  SPD_fpMnemonicOne PT_IGNORE STK_FADD,
  SPD_fpMnemonicOne PT_IGNORE STK_FBLD,
  SPD_fpMnemonicOne PT_IGNORE STK_FBSTP,
  SPD_fpMnemonicOne PT_IGNORE STK_FCOMP,
  SPD_fpMnemonicOne PT_IGNORE STK_FCOM,
  SPD_fpMnemonicOne PT_IGNORE STK_FDIVR,
  SPD_fpMnemonicOne PT_IGNORE STK_FDIV,
  SPD_fpMnemonicOne PT_IGNORE STK_FFREE,
  SPD_fpMnemonicOne PT_IGNORE STK_FIADD,
  SPD_fpMnemonicOne PT_IGNORE STK_FICOMP,
  SPD_fpMnemonicOne PT_IGNORE STK_FICOM,
  SPD_fpMnemonicOne PT_IGNORE STK_FIDIVR,
  SPD_fpMnemonicOne PT_IGNORE STK_FIDIV,
  SPD_fpMnemonicOne PT_IGNORE STK_FILD,
  SPD_fpMnemonicOne PT_IGNORE STK_FIMUL,
  SPD_fpMnemonicOne PT_IGNORE STK_FISTP,
  SPD_fpMnemonicOne PT_IGNORE STK_FISUBR,
  SPD_fpMnemonicOne PT_IGNORE STK_FISUB,
  SPD_fpMnemonicOne PT_IGNORE STK_FLDCW,
  SPD_fpMnemonicOne PT_IGNORE STK_FLDENV,
  SPD_fpMnemonicOne PT_IGNORE STK_FLD,
  SPD_fpMnemonicOne PT_IGNORE STK_FMUL,
  SPD_fpMnemonicOne PT_IGNORE STK_FRSTOR,
  SPD_fpMnemonicOne PT_IGNORE STK_FSAVE,
  SPD_fpMnemonicOne PT_IGNORE STK_FNSAVE,
  SPD_fpMnemonicOne PT_IGNORE STK_FSTCW,
  SPD_fpMnemonicOne PT_IGNORE STK_FST,
  SPD_fpMnemonicOne PT_IGNORE STK_FNSTCW,
  SPD_fpMnemonicOne PT_IGNORE STK_FSTENV,
  SPD_fpMnemonicOne PT_IGNORE STK_FNSTENV,
  SPD_fpMnemonicOne PT_IGNORE STK_FSTP,
  SPD_fpMnemonicOne PT_IGNORE STK_FSTSW,
  SPD_fpMnemonicOne PT_IGNORE STK_FNSTSW,
  SPD_fpMnemonicOne PT_IGNORE STK_FSUBR,
  SPD_fpMnemonicOne PT_IGNORE STK_FSUB,
  SPD_fpMnemonicOne PT_IGNORE STK_FXCH,

  SPD_fpMnemonicTwo PT_IGNORE STK_FADDP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FADD,
  SPD_fpMnemonicTwo PT_IGNORE STK_FDIVP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FDIVRP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FDIVR,
  SPD_fpMnemonicTwo PT_IGNORE STK_FDIV,
  SPD_fpMnemonicTwo PT_IGNORE STK_FMULP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FMUL,
  SPD_fpMnemonicTwo PT_IGNORE STK_FSUBP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FSUBRP,
  SPD_fpMnemonicTwo PT_IGNORE STK_FSUBR,
  SPD_fpMnemonicTwo PT_IGNORE STK_FSUB,
  
  SPD_optionDir PT_IGNORE STK_OPTION SPD_optionList STK_NEWLINE,
  
  SPD_optionList PT_IGNORE SPD_optionList SPD_commaOptNewline SPD_optionItem,
  SPD_optionList PT_IGNORE SPD_optionItem,

  SPD_optionItem PT_EXECUTE STK_CASEMAP STK_COLON SPD_mapType,
  SPD_optionItem PT_EXECUTE STK_LJMP,
  SPD_optionItem PT_EXECUTE STK_NOLJMP,
  SPD_optionItem PT_EXECUTE STK_LANGUAGE STK_COLON SPD_langType,
  SPD_optionItem PT_EXECUTE STK_READONLY,
  SPD_optionItem PT_EXECUTE STK_NOREADONLY,

  SPD_mapType PT_IGNORE STK_ALL,
  SPD_mapType PT_IGNORE STK_NOTPUBLIC,
  SPD_mapType PT_IGNORE STK_NONE,
  
  SPD_ifDir PT_IGNORE SPD_ifBlock SPD_elseifList SPD_elseBlock SPD_endifStatement,
  SPD_ifDir PT_IGNORE SPD_ifBlock SPD_elseifList SPD_endifStatement,
  SPD_ifDir PT_IGNORE SPD_ifBlock SPD_elseBlock SPD_endifStatement,
  SPD_ifDir PT_IGNORE SPD_ifBlock SPD_endifStatement,

  SPD_ifBlock PT_IGNORE SPD_ifStatement SPD_inSegDirList,
  SPD_ifBlock PT_IGNORE SPD_ifStatement,
  
  SPD_ifStatement PT_IGNORE STK_IF SPD_constExpr STK_NEWLINE,

  SPD_elseifList PT_IGNORE SPD_elseifList SPD_elseifBlock,
  SPD_elseifList PT_IGNORE SPD_elseifBlock,

  SPD_elseifBlock PT_IGNORE SPD_elseifStatement SPD_inSegDirList,
  SPD_elseifBlock PT_IGNORE SPD_elseifStatement,

  SPD_elseifStatement PT_IGNORE STK_ELSEIF SPD_constExpr STK_NEWLINE,

  SPD_elseBlock PT_IGNORE SPD_elseStatement SPD_inSegDirList,
  SPD_elseBlock PT_IGNORE SPD_elseStatement,

  SPD_elseStatement PT_IGNORE STK_ELSE STK_NEWLINE,

  SPD_endifStatement PT_IGNORE STK_ENDIF STK_NEWLINE,

#ifdef WALLY
  
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
#endif
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
