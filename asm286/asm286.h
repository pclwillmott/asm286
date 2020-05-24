/*
 *-------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    asm286.h       Definitions
 *
 *  This revision:
 *
 *    2020 March 29 Paul Willmott Token macros updated for i286.
 *
 *  Revision History:
 *
 *    2019 November 17 Paul Willmott Error codes added.
 *
 *    2019 November 10 Paul Willmott Production string constants added.
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *-------------------------------------------------------------------------------
 */

#include <stdio.h>

#ifndef asm286_h
#define asm286_h

//#define DEBUG2 1

/*
 * Error codes.
 */

enum {
  ERR_INTERNAL_ERROR           =  1,
  ERR_OUT_OF_MEMORY            =  2,
  ERR_LINE_BUFFER_OVERFLOW     =  3,
  ERR_SYNTAX_ERROR             =  4,
  ERR_STRING_TOO_LONG          =  5,
  ERR_IDENTIFIER_TOO_LONG      =  6,
  ERR_IDENTIFIER_EXISTS        =  7,
  ERR_SYMBOL_TABLE_FULL        =  8,
  ERR_SEGMENT_STACK_OVERFLOW   =  9,
  ERR_TOO_MANY_SEGMENTS        = 10,
  ERR_NOT_IN_SEGMENT           = 11,
  ERR_SEGMENT_NESTING_FAULT    = 12,
  ERR_SEGMENT_NOT_ENDED        = 13,
  ERR_PRODUCTION_NOT_FOUND     = 14,
  ERR_INVALID_INSTRUCTION      = 15,
  ERR_IDENTIFIER_NOT_FOUND     = 16,
} ;

#define NUM_ERR ( 16 )

/*
 * Symbol Table stuff.
 */

enum SymbolType {
  ST_LABEL    = 0,
  ST_VARIABLE = 1,
  ST_CONSTANT = 2,
};

enum DataType {
  DT_UNASSIGNED = -1,
  DT_BYTE   = 0,
  DT_SBYTE  = 1,
  DT_WORD   = 2,
  DT_SWORD  = 3,
  DT_DWORD  = 4,
  DT_SDWORD = 5,
  DT_FWORD  = 6,
  DT_QWORD  = 7,
  DT_TBYTE  = 8,
  DT_REAL4  = 9,
  DT_REAL8  = 10,
  DT_REAL10 = 11,
};

enum Distance {
  DIST_UNASSIGNED = -1,
  DIST_NEAR = 0,
  DIST_FAR = 1,
};

enum Visibility {
  VS_PRIVATE = 0,
  VS_PUBLIC = 1,
  VS_EXTERN = 2,
};

enum Language {
  LG_UNASSIGNED = -1,
  LG_C = 0,
};

#define MAX_SYMBOLS 128

/*
 * Token types.
 */

#define KEYWORD          "\001"
#define KEYWORD_186      "\011"
#define KEYWORD_286      "\021"
#define KEYWORD_286P     "\031"
#define KEYWORD_8087     "\101"
#define KEYWORD_287      "\201"
#define RESERVED         "\002"
#define KEYWORD_RESERVED "\003"
#define SPECIAL          "\004"

#define PROCESSOR_MASK 070
#define COPROCESSOR_MASK 0300

#define PROCESSOR(x) (((x) & PROCESSOR_MASK) >> 3)
#define COPROCESSOR(x) (((x) & COPROCESSOR_MASK) >> 5)

enum ProcessorType {
  i86 = 0,
  i186 = 1,
  i286 = 2,
  i286p = 3,
};

enum CoProcessorType {
  none = 0,
  i87 = 1,
  i287 = 2,
};

/*
 * Token string constants.
 */

#define NUM_PATTERN ( 376 )  /* Number of token patterns */

#define STK_UNDERSCORE  "\001\001"
#define STK_MINUS       "\001\002"
#define STK_SEMICOLON   "\001\003"
#define STK_COLON       "\001\004"
#define STK_QMARK       "\001\005"
#define STK_PERIOD      "\001\006"
#define STK_QUOTE       "\001\007"
#define STK_OBRACE      "\001\010"
#define STK_CBRACE      "\001\011"
#define STK_OBRACKET    "\001\012"
#define STK_CBRACKET    "\001\013"
#define STK_ATSIGN      "\001\014"
#define STK_MULTIPLY    "\001\015"
#define STK_DIVIDE      "\001\016"
#define STK_DBLQUOTE    "\001\017"
#define STK_AMPERSAND   "\001\020"
#define STK_PLUS        "\001\021"
#define STK_LESS        "\001\022"
#define STK_GREATER     "\001\023"
#define STK_DOLLAR      "\001\024"
#define STK_COMMA       "\001\025"
#define STK_EQUAL       "\001\026"

/*
 * BEGIN AUTONUMBER SECTION
 */

#define STK_8086        "\001\027"
#define STK_8087        "\001\030"
#define STK_186         "\001\031"
#define STK_286         "\001\032"
#define STK_286P        "\001\033"
#define STK_287         "\001\034"
#define STK_CODE        "\001\035"
#define STK_CONST       "\001\036"
#define STK_DATA        "\001\037"
#define STK_DATAQ       "\001\040"
#define STK_FARDATA     "\001\041"
#define STK_FARDATAQ    "\001\042"
#define STK_LIST        "\001\043"
#define STK_MODEL       "\001\044"
#define STK_NO87        "\001\045"
#define STK_NOLIST      "\001\046"
#define STK_DOTSTACK    "\001\047"
#define STK_AAA         "\001\050"
#define STK_AAD         "\001\051"
#define STK_AAM         "\001\052"
#define STK_AAS         "\001\053"
#define STK_ABS         "\001\054"
#define STK_ADC         "\001\055"
#define STK_ADD         "\001\056"
#define STK_AH          "\001\057"
#define STK_AL          "\001\060"
#define STK_ALIGN       "\001\061"
#define STK_AND         "\001\062"
#define STK_ARPL        "\001\063"
#define STK_ASSUME      "\001\064"
#define STK_AT          "\001\065"
#define STK_AX          "\001\066"
#define STK_BH          "\001\067"
#define STK_BL          "\001\070"
#define STK_BOUND       "\001\071"
#define STK_BP          "\001\072"
#define STK_BX          "\001\073"
#define STK_BYTE        "\001\074"
#define STK_C           "\001\075"
#define STK_CALL        "\001\076"
#define STK_CBW         "\001\077"
#define STK_CH          "\001\100"
#define STK_CL          "\001\101"
#define STK_CLC         "\001\102"
#define STK_CLD         "\001\103"
#define STK_CLI         "\001\104"
#define STK_CLTS        "\001\105"
#define STK_CMC         "\001\106"
#define STK_CMP         "\001\107"
#define STK_CMPS        "\001\110"
#define STK_CMPSB       "\001\111"
#define STK_CMPSW       "\001\112"
#define STK_COMMON      "\001\113"
#define STK_COMPACT     "\001\114"
#define STK_CS          "\001\115"
#define STK_CWD         "\001\116"
#define STK_CX          "\001\117"
#define STK_DAA         "\001\120"
#define STK_DAS         "\001\121"
#define STK_DB          "\001\122"
#define STK_DD          "\001\123"
#define STK_DEC         "\001\124"
#define STK_DF          "\001\125"
#define STK_DGROUP      "\001\126"
#define STK_DH          "\001\127"
#define STK_DI          "\001\130"
#define STK_DIV         "\001\131"
#define STK_DL          "\001\132"
#define STK_DQ          "\001\133"
#define STK_DS          "\001\134"
#define STK_DT          "\001\135"
#define STK_DUP         "\001\136"
#define STK_DW          "\001\137"
#define STK_DWORD       "\001\140"
#define STK_DX          "\001\141"
#define STK_ECHO        "\001\142"
#define STK_END         "\001\143"
#define STK_ENDS        "\001\144"
#define STK_ENTER       "\001\145"
#define STK_EQ          "\001\146"
#define STK_EQU         "\001\147"
#define STK_ERROR       "\001\150"
#define STK_ES          "\001\151"
#define STK_ESC         "\001\152"
#define STK_EVEN        "\001\153"
#define STK_EXTERN      "\001\154"
#define STK_F2XM1       "\001\155"
#define STK_FABS        "\001\156"
#define STK_FADD        "\001\157"
#define STK_FADDP       "\001\160"
#define STK_FAR         "\001\161"
#define STK_FARSTACK    "\001\162"
#define STK_FBLD        "\001\163"
#define STK_FBSTP       "\001\164"
#define STK_FCHS        "\001\165"
#define STK_FCLEX       "\001\166"
#define STK_FCOM        "\001\167"
#define STK_FCOMP       "\001\170"
#define STK_FCOMPP      "\001\171"
#define STK_FDECSTP     "\001\172"
#define STK_FDISI       "\001\173"
#define STK_FDIV        "\001\174"
#define STK_FDIVP       "\001\175"
#define STK_FDIVR       "\001\176"
#define STK_FDIVRP      "\001\177"
#define STK_FENI        "\002\001"
#define STK_FFREE       "\002\002"
#define STK_FIADD       "\002\003"
#define STK_FICOM       "\002\004"
#define STK_FICOMP      "\002\005"
#define STK_FIDIV       "\002\006"
#define STK_FIDIVR      "\002\007"
#define STK_FILD        "\002\010"
#define STK_FIMUL       "\002\011"
#define STK_FINCSTP     "\002\012"
#define STK_FINIT       "\002\013"
#define STK_FIST        "\002\014"
#define STK_FISTP       "\002\015"
#define STK_FISUB       "\002\016"
#define STK_FISUBR      "\002\017"
#define STK_FLD         "\002\020"
#define STK_FLD1        "\002\021"
#define STK_FLDCW       "\002\022"
#define STK_FLDENV      "\002\023"
#define STK_FLDL2E      "\002\024"
#define STK_FLDL2T      "\002\025"
#define STK_FLDLG2      "\002\026"
#define STK_FLDLN2      "\002\027"
#define STK_FLDPI       "\002\030"
#define STK_FLDZ        "\002\031"
#define STK_FMUL        "\002\032"
#define STK_FMULP       "\002\033"
#define STK_FNCLEX      "\002\034"
#define STK_FNDISI      "\002\035"
#define STK_FNENI       "\002\036"
#define STK_FNINIT      "\002\037"
#define STK_FNOP        "\002\040"
#define STK_FNSAVE      "\002\041"
#define STK_FNSTCW      "\002\042"
#define STK_FNSTENV     "\002\043"
#define STK_FNSTSW      "\002\044"
#define STK_FPATAN      "\002\045"
#define STK_FPREM       "\002\046"
#define STK_FPTAN       "\002\047"
#define STK_FRNDINT     "\002\050"
#define STK_FRSTOR      "\002\051"
#define STK_FSAVE       "\002\052"
#define STK_FSCALE      "\002\053"
#define STK_FSETPM      "\002\054"
#define STK_FSQRT       "\002\055"
#define STK_FST         "\002\056"
#define STK_FSTCW       "\002\057"
#define STK_FSTENV      "\002\060"
#define STK_FSTP        "\002\061"
#define STK_FSTSW       "\002\062"
#define STK_FSUB        "\002\063"
#define STK_FSUBP       "\002\064"
#define STK_FSUBR       "\002\065"
#define STK_FSUBRP      "\002\066"
#define STK_FTST        "\002\067"
#define STK_FWAIT       "\002\070"
#define STK_FWORD       "\002\071"
#define STK_FXAM        "\002\072"
#define STK_FXCH        "\002\073"
#define STK_FXTRACT     "\002\074"
#define STK_FYL2X       "\002\075"
#define STK_FYL2XP1     "\002\076"
#define STK_GE          "\002\077"
#define STK_GROUP       "\002\100"
#define STK_GT          "\002\101"
#define STK_HIGH        "\002\102"
#define STK_HUGE        "\002\103"
#define STK_HLT         "\002\104"
#define STK_IDIV        "\002\105"
#define STK_IMUL        "\002\106"
#define STK_IN          "\002\107"
#define STK_INC         "\002\110"
#define STK_INS         "\002\111"
#define STK_INSB        "\002\112"
#define STK_INSW        "\002\113"
#define STK_INT         "\002\114"
#define STK_INTO        "\002\115"
#define STK_IRET        "\002\116"
#define STK_JA          "\002\117"
#define STK_JAE         "\002\120"
#define STK_JB          "\002\121"
#define STK_JBE         "\002\122"
#define STK_JC          "\002\123"
#define STK_JCXZ        "\002\124"
#define STK_JE          "\002\125"
#define STK_JG          "\002\126"
#define STK_JGE         "\002\127"
#define STK_JL          "\002\130"
#define STK_JLE         "\002\131"
#define STK_JMP         "\002\132"
#define STK_JNA         "\002\133"
#define STK_JNAE        "\002\134"
#define STK_JNB         "\002\135"
#define STK_JNBE        "\002\136"
#define STK_JNC         "\002\137"
#define STK_JNE         "\002\140"
#define STK_JNG         "\002\141"
#define STK_JNGE        "\002\142"
#define STK_JNL         "\002\143"
#define STK_JNLE        "\002\144"
#define STK_JNO         "\002\145"
#define STK_JNP         "\002\146"
#define STK_JNS         "\002\147"
#define STK_JNZ         "\002\150"
#define STK_JO          "\002\151"
#define STK_JP          "\002\152"
#define STK_JPE         "\002\153"
#define STK_JPO         "\002\154"
#define STK_JS          "\002\155"
#define STK_JZ          "\002\156"
#define STK_LABEL       "\002\157"
#define STK_LAHF        "\002\160"
#define STK_LAR         "\002\161"
#define STK_LARGE       "\002\162"
#define STK_LDS         "\002\163"
#define STK_LE          "\002\164"
#define STK_LEA         "\002\165"
#define STK_LEAVE       "\002\166"
#define STK_LENGTH      "\002\167"
#define STK_LENGTHOF    "\002\170"
#define STK_LES         "\002\171"
#define STK_LGDT        "\002\172"
#define STK_LIDT        "\002\173"
#define STK_LLDT        "\002\174"
#define STK_LMSW        "\002\175"
#define STK_LOCK        "\002\176"
#define STK_LODS        "\002\177"
#define STK_LODSB       "\003\001"
#define STK_LODSW       "\003\002"
#define STK_LOOP        "\003\003"
#define STK_LOOPE       "\003\004"
#define STK_LOOPNE      "\003\005"
#define STK_LOOPNZ      "\003\006"
#define STK_LOOPZ       "\003\007"
#define STK_LOW         "\003\010"
#define STK_LSL         "\003\011"
#define STK_LT          "\003\012"
#define STK_LTR         "\003\013"
#define STK_MASK        "\003\014"
#define STK_MEDIUM      "\003\015"
#define STK_MEMORY      "\003\016"
#define STK_MOD         "\003\017"
#define STK_MOV         "\003\020"
#define STK_MOVS        "\003\021"
#define STK_MOVSB       "\003\022"
#define STK_MOVSW       "\003\023"
#define STK_MUL         "\003\024"
#define STK_NAME        "\003\025"
#define STK_NE          "\003\026"
#define STK_NEAR        "\003\027"
#define STK_NEARSTACK   "\003\030"
#define STK_NEG         "\003\031"
#define STK_NOP         "\003\032"
#define STK_NOT         "\003\033"
#define STK_NOTHING     "\003\034"
#define STK_OFFSET      "\003\035"
#define STK_OR          "\003\036"
#define STK_ORG         "\003\037"
#define STK_OUT         "\003\040"
#define STK_OUTS        "\003\041"
#define STK_OUTSB       "\003\042"
#define STK_OUTSW       "\003\043"
#define STK_PARA        "\003\044"
#define STK_PAGE        "\003\045"
#define STK_POP         "\003\046"
#define STK_POPA        "\003\047"
#define STK_POPF        "\003\050"
#define STK_PRIVATE     "\003\051"
#define STK_PTR         "\003\052"
#define STK_PUBLIC      "\003\053"
#define STK_PUSH        "\003\054"
#define STK_PUSHA       "\003\055"
#define STK_PUSHF       "\003\056"
#define STK_QWORD       "\003\057"
#define STK_RCL         "\003\060"
#define STK_RCR         "\003\061"
#define STK_REAL4       "\003\062"
#define STK_REAL8       "\003\063"
#define STK_REAL10      "\003\064"
#define STK_REP         "\003\065"
#define STK_REPE        "\003\066"
#define STK_REPNE       "\003\067"
#define STK_REPNZ       "\003\070"
#define STK_REPZ        "\003\071"
#define STK_RET         "\003\072"
#define STK_ROL         "\003\073"
#define STK_ROR         "\003\074"
#define STK_SAHF        "\003\075"
#define STK_SAL         "\003\076"
#define STK_SAR         "\003\077"
#define STK_SBB         "\003\100"
#define STK_SBYTE       "\003\101"
#define STK_SCAS        "\003\102"
#define STK_SCASB       "\003\103"
#define STK_SCASW       "\003\104"
#define STK_SDWORD      "\003\105"
#define STK_SEG         "\003\106"
#define STK_SEGMENT     "\003\107"
#define STK_SGDT        "\003\110"
#define STK_SHL         "\003\111"
#define STK_SHORT       "\003\112"
#define STK_SHR         "\003\113"
#define STK_SI          "\003\114"
#define STK_SIDT        "\003\115"
#define STK_SIZE        "\003\116"
#define STK_SIZEOF      "\003\117"
#define STK_SLDT        "\003\120"
#define STK_SMALL       "\003\121"
#define STK_SMSW        "\003\122"
#define STK_SP          "\003\123"
#define STK_SS          "\003\124"
#define STK_ST          "\003\125"
#define STK_STACK       "\003\126"
#define STK_STC         "\003\127"
#define STK_STD         "\003\130"
#define STK_STI         "\003\131"
#define STK_STOS        "\003\132"
#define STK_STOSB       "\003\133"
#define STK_STOSW       "\003\134"
#define STK_STR         "\003\135"
#define STK_SUB         "\003\136"
#define STK_SUBTITLE    "\003\137"
#define STK_SWORD       "\003\140"
#define STK_TBYTE       "\003\141"
#define STK_TEST        "\003\142"
#define STK_THIS        "\003\143"
#define STK_TINY        "\003\144"
#define STK_TITLE       "\003\145"
#define STK_TYPE        "\003\146"
#define STK_VERR        "\003\147"
#define STK_VERW        "\003\150"
#define STK_WAIT        "\003\151"
#define STK_WIDTH       "\003\152"
#define STK_WORD        "\003\153"
#define STK_XCHG        "\003\154"
#define STK_XLAT        "\003\155"
#define STK_XLATB       "\003\156"
#define STK_XOR         "\003\157"

/*
 * END OF AUTONUBER SECTION
 */

#define STK_INTEGERBIN  "\003\160"
#define STK_INTEGEROCT  "\003\161"
#define STK_INTEGERDEC  "\003\162"
#define STK_INTEGERHEX  "\003\163"
#define STK_DOUBLE      "\003\164"
#define STK_STRING      "\003\165"
#define STK_IDENTIFIER  "\003\166"
#define STK_INST_LABEL  "\003\167"
#define STK_NEWLINE     "\003\170"
#define STK_WHITESPACE  "\003\171"
#define STK_TEXT        "\003\172"

/*
 * Production string constants.
 */

#define SPD_addOp           "\101\001"
#define SPD_altId           "\101\002"
#define SPD_andOp           "\101\003"
#define SPD_arbitaryText    "\101\004"
#define SPD_asmInstruction  "\101\005"
#define SPD_assumeDir       "\101\006"
#define SPD_assumeList      "\101\007"
#define SPD_assumeReg       "\101\010"
#define SPD_assumeRegister  "\101\011"
#define SPD_assumeSegReg    "\101\012"
#define SPD_assumeSegVal    "\101\013"
#define SPD_assumeVal       "\101\014"
#define SPD_byteRegister    "\101\015"
#define SPD_className       "\101\016"
#define SPD_commaOptNewline "\101\017"
#define SPD_constant        "\101\020"
#define SPD_constExpr       "\101\021"
#define SPD_coprocessor     "\101\022"
#define SPD_dataDecl        "\101\023"
#define SPD_dataDir         "\101\024"
#define SPD_dataItem        "\101\025"
#define SPD_dataType        "\101\026"
#define SPD_directive       "\101\027"
#define SPD_directiveList   "\101\030"
#define SPD_e01             "\101\031"
#define SPD_e02             "\101\032"
#define SPD_e03             "\101\033"
#define SPD_e04             "\101\034"
#define SPD_e05             "\101\035"
#define SPD_e06             "\101\036"
#define SPD_e07             "\101\037"
#define SPD_e07Op           "\101\040"
#define SPD_e08             "\101\041"
#define SPD_e09             "\101\042"
#define SPD_e10             "\101\043"
#define SPD_echoDir         "\101\044"
#define SPD_endDir          "\101\045"
#define SPD_endsDir         "\101\046"
#define SPD_equalDir        "\101\047"
#define SPD_equDir          "\101\050"
#define SPD_equType         "\101\051"
#define SPD_expr            "\101\052"
#define SPD_externDef       "\101\053"
#define SPD_externDir       "\101\054"
#define SPD_externList      "\101\055"
#define SPD_externType      "\101\056"
#define SPD_fpInstruction   "\101\057"
#define SPD_fpMnemonicOne   "\101\060"
#define SPD_fpMnemonicTwo   "\101\061"
#define SPD_fpMnemonicZero  "\101\062"
#define SPD_frameExpr       "\101\063"
#define SPD_generalDir      "\101\064"
#define SPD_gpRegister      "\101\065"
#define SPD_groupDir        "\101\066"
#define SPD_groupId         "\101\067"
#define SPD_immExpr         "\101\070"
#define SPD_initValue       "\101\071"
#define SPD_inSegDir        "\101\072"
#define SPD_inSegDirList    "\101\073"
#define SPD_inSegmentDir    "\101\074"
#define SPD_instPrefix      "\101\075"
#define SPD_instruction     "\101\076"
#define SPD_labelDef        "\101\077"
#define SPD_labelDir        "\101\100"
#define SPD_langType        "\101\101"
#define SPD_listDir         "\101\102"
#define SPD_listOption      "\101\103"
#define SPD_memOption       "\101\104"
#define SPD_mnemonicOne     "\101\105"
#define SPD_mnemonicTwo     "\101\106"
#define SPD_mnemonicZero    "\101\107"
#define SPD_modelDir        "\101\110"
#define SPD_modelOpt        "\101\111"
#define SPD_modelOptList    "\101\112"
#define SPD_module          "\101\113"
#define SPD_mulOp           "\101\114"
#define SPD_nameDir         "\101\115"
#define SPD_nearfar         "\101\116"
#define SPD_offsetDir       "\101\117"
#define SPD_offsetDirType   "\101\120"
#define SPD_operand         "\101\121"
#define SPD_orOp            "\101\122"
#define SPD_pageDir         "\101\123"
#define SPD_pageExpr        "\101\124"
#define SPD_pageLength      "\101\125"
#define SPD_pageWidth       "\101\126"
#define SPD_processor       "\101\127"
#define SPD_processorDir    "\101\130"
#define SPD_pubDef          "\101\131"
#define SPD_publicDir       "\101\132"
#define SPD_pubList         "\101\133"
#define SPD_qualifiedType   "\101\134"
#define SPD_register        "\101\135"
#define SPD_relOp           "\101\136"
#define SPD_scalarInstList  "\101\137"
#define SPD_segAlign        "\101\140"
#define SPD_segAttrib       "\101\141"
#define SPD_segDir          "\101\142"
#define SPD_segId           "\101\143"
#define SPD_segIdList       "\101\144"
#define SPD_segmentDef      "\101\145"
#define SPD_segmentDir      "\101\146"
#define SPD_segmentRegister "\101\147"
#define SPD_segOption       "\101\150"
#define SPD_segOptionList   "\101\151"
#define SPD_simpleSegDir    "\101\152"
#define SPD_sizeArg         "\101\153"
#define SPD_stackOption     "\101\154"
#define SPD_textLiteral     "\101\155"
#define SPD_textMacroId     "\101\156"
#define SPD_titleDir        "\101\157"
#define SPD_titleType       "\101\160"
#define SPD_type            "\101\161"

#define SPD_LAST            "\177\177"

enum {
  
  TOK_UNDERSCORE  =   0,
  TOK_MINUS       =   1,
  TOK_SEMICOLON   =   2,
  TOK_COLON       =   3,
  TOK_QMARK       =   4,
  TOK_PERIOD      =   5,
  TOK_QUOTE       =   6,
  TOK_OBRACE      =   7,
  TOK_CBRACE      =   8,
  TOK_OBRACKET    =   9,
  TOK_CBRACKET    =  10,
  TOK_ATSIGN      =  11,
  TOK_MULTIPLY    =  12,
  TOK_DIVIDE      =  13,
  TOK_DBLQUOTE    =  14,
  TOK_AMPERSAND   =  15,
  TOK_PLUS        =  16,
  TOK_LESS        =  17,
  TOK_GREATER     =  18,
  TOK_DOLLAR      =  19,
  TOK_COMMA       =  20,
  TOK_EQUAL       =  21,
  
  TOK_8086        =  22,
  TOK_8087        =  23,
  TOK_186         =  24,
  TOK_286         =  25,
  TOK_286P        =  26,
  TOK_287         =  27,
  TOK_CODE        =  28,
  TOK_CONST       =  29,
  TOK_DATA        =  30,
  TOK_DATAQ       =  31,
  TOK_FARDATA     =  32,
  TOK_FARDATAQ    =  33,
  TOK_LIST        =  34,
  TOK_MODEL       =  35,
  TOK_NO87        =  36,
  TOK_NOLIST      =  37,
  TOK_DOTSTACK    =  38,
  TOK_AAA         =  39,
  TOK_AAD         =  40,
  TOK_AAM         =  41,
  TOK_AAS         =  42,
  TOK_ABS         =  43,
  TOK_ADC         =  44,
  TOK_ADD         =  45,
  TOK_AH          =  46,
  TOK_AL          =  47,
  TOK_ALIGN       =  48,
  TOK_AND         =  49,
  TOK_ARPL        =  50,
  TOK_ASSUME      =  51,
  TOK_AT          =  52,
  TOK_AX          =  53,
  TOK_BH          =  54,
  TOK_BL          =  55,
  TOK_BOUND       =  56,
  TOK_BP          =  57,
  TOK_BX          =  58,
  TOK_BYTE        =  59,
  TOK_C           =  60,
  TOK_CALL        =  61,
  TOK_CBW         =  62,
  TOK_CH          =  63,
  TOK_CL          =  64,
  TOK_CLC         =  65,
  TOK_CLD         =  66,
  TOK_CLI         =  67,
  TOK_CLTS        =  68,
  TOK_CMC         =  69,
  TOK_CMP         =  70,
  TOK_CMPS        =  71,
  TOK_CMPSB       =  72,
  TOK_CMPSW       =  73,
  TOK_COMMON      =  74,
  TOK_COMPACT     =  75,
  TOK_CS          =  76,
  TOK_CWD         =  77,
  TOK_CX          =  78,
  TOK_DAA         =  79,
  TOK_DAS         =  80,
  TOK_DB          =  81,
  TOK_DD          =  82,
  TOK_DEC         =  83,
  TOK_DF          =  84,
  TOK_DGROUP      =  85,
  TOK_DH          =  86,
  TOK_DI          =  87,
  TOK_DIV         =  88,
  TOK_DL          =  89,
  TOK_DQ          =  90,
  TOK_DS          =  91,
  TOK_DT          =  92,
  TOK_DUP         =  93,
  TOK_DW          =  94,
  TOK_DWORD       =  95,
  TOK_DX          =  96,
  TOK_ECHO        =  97,
  TOK_END         =  98,
  TOK_ENDS        =  99,
  TOK_ENTER       = 100,
  TOK_EQ          = 101,
  TOK_EQU         = 102,
  TOK_ERROR       = 103,
  TOK_ES          = 104,
  TOK_ESC         = 105,
  TOK_EVEN        = 106,
  TOK_EXTERN      = 107,
  TOK_F2XM1       = 108,
  TOK_FABS        = 109,
  TOK_FADD        = 110,
  TOK_FADDP       = 111,
  TOK_FAR         = 112,
  TOK_FARSTACK    = 113,
  TOK_FBLD        = 114,
  TOK_FBSTP       = 115,
  TOK_FCHS        = 116,
  TOK_FCLEX       = 117,
  TOK_FCOM        = 118,
  TOK_FCOMP       = 119,
  TOK_FCOMPP      = 120,
  TOK_FDECSTP     = 121,
  TOK_FDISI       = 122,
  TOK_FDIV        = 123,
  TOK_FDIVP       = 124,
  TOK_FDIVR       = 125,
  TOK_FDIVRP      = 126,
  TOK_FENI        = 127,
  TOK_FFREE       = 128,
  TOK_FIADD       = 129,
  TOK_FICOM       = 130,
  TOK_FICOMP      = 131,
  TOK_FIDIV       = 132,
  TOK_FIDIVR      = 133,
  TOK_FILD        = 134,
  TOK_FIMUL       = 135,
  TOK_FINCSTP     = 136,
  TOK_FINIT       = 137,
  TOK_FIST        = 138,
  TOK_FISTP       = 139,
  TOK_FISUB       = 140,
  TOK_FISUBR      = 141,
  TOK_FLD         = 142,
  TOK_FLD1        = 143,
  TOK_FLDCW       = 144,
  TOK_FLDENV      = 145,
  TOK_FLDL2E      = 146,
  TOK_FLDL2T      = 147,
  TOK_FLDLG2      = 148,
  TOK_FLDLN2      = 149,
  TOK_FLDPI       = 150,
  TOK_FLDZ        = 151,
  TOK_FMUL        = 152,
  TOK_FMULP       = 153,
  TOK_FNCLEX      = 154,
  TOK_FNDISI      = 155,
  TOK_FNENI       = 156,
  TOK_FNINIT      = 157,
  TOK_FNOP        = 158,
  TOK_FNSAVE      = 159,
  TOK_FNSTCW      = 160,
  TOK_FNSTENV     = 161,
  TOK_FNSTSW      = 162,
  TOK_FPATAN      = 163,
  TOK_FPREM       = 164,
  TOK_FPTAN       = 165,
  TOK_FRNDINT     = 166,
  TOK_FRSTOR      = 167,
  TOK_FSAVE       = 168,
  TOK_FSCALE      = 169,
  TOK_FSETPM      = 170,
  TOK_FSQRT       = 171,
  TOK_FST         = 172,
  TOK_FSTCW       = 173,
  TOK_FSTENV      = 174,
  TOK_FSTP        = 175,
  TOK_FSTSW       = 176,
  TOK_FSUB        = 177,
  TOK_FSUBP       = 178,
  TOK_FSUBR       = 179,
  TOK_FSUBRP      = 180,
  TOK_FTST        = 181,
  TOK_FWAIT       = 182,
  TOK_FWORD       = 183,
  TOK_FXAM        = 184,
  TOK_FXCH        = 185,
  TOK_FXTRACT     = 186,
  TOK_FYL2X       = 187,
  TOK_FYL2XP1     = 188,
  TOK_GE          = 189,
  TOK_GROUP       = 190,
  TOK_GT          = 191,
  TOK_HIGH        = 192,
  TOK_HUGE        = 193,
  TOK_HLT         = 194,
  TOK_IDIV        = 195,
  TOK_IMUL        = 196,
  TOK_IN          = 197,
  TOK_INC         = 198,
  TOK_INS         = 199,
  TOK_INSB        = 200,
  TOK_INSW        = 201,
  TOK_INT         = 202,
  TOK_INTO        = 203,
  TOK_IRET        = 204,
  TOK_JA          = 205,
  TOK_JAE         = 206,
  TOK_JB          = 207,
  TOK_JBE         = 208,
  TOK_JC          = 209,
  TOK_JCXZ        = 210,
  TOK_JE          = 211,
  TOK_JG          = 212,
  TOK_JGE         = 213,
  TOK_JL          = 214,
  TOK_JLE         = 215,
  TOK_JMP         = 216,
  TOK_JNA         = 217,
  TOK_JNAE        = 218,
  TOK_JNB         = 219,
  TOK_JNBE        = 220,
  TOK_JNC         = 221,
  TOK_JNE         = 222,
  TOK_JNG         = 223,
  TOK_JNGE        = 224,
  TOK_JNL         = 225,
  TOK_JNLE        = 226,
  TOK_JNO         = 227,
  TOK_JNP         = 228,
  TOK_JNS         = 229,
  TOK_JNZ         = 230,
  TOK_JO          = 231,
  TOK_JP          = 232,
  TOK_JPE         = 233,
  TOK_JPO         = 234,
  TOK_JS          = 235,
  TOK_JZ          = 236,
  TOK_LABEL       = 237,
  TOK_LAHF        = 238,
  TOK_LAR         = 239,
  TOK_LARGE       = 240,
  TOK_LDS         = 241,
  TOK_LE          = 242,
  TOK_LEA         = 243,
  TOK_LEAVE       = 244,
  TOK_LENGTH      = 245,
  TOK_LENGTHOF    = 246,
  TOK_LES         = 247,
  TOK_LGDT        = 248,
  TOK_LIDT        = 249,
  TOK_LLDT        = 250,
  TOK_LMSW        = 251,
  TOK_LOCK        = 252,
  TOK_LODS        = 253,
  TOK_LODSB       = 254,
  TOK_LODSW       = 255,
  TOK_LOOP        = 256,
  TOK_LOOPE       = 257,
  TOK_LOOPNE      = 258,
  TOK_LOOPNZ      = 259,
  TOK_LOOPZ       = 260,
  TOK_LOW         = 261,
  TOK_LSL         = 262,
  TOK_LT          = 263,
  TOK_LTR         = 264,
  TOK_MASK        = 265,
  TOK_MEDIUM      = 266,
  TOK_MEMORY      = 267,
  TOK_MOD         = 268,
  TOK_MOV         = 269,
  TOK_MOVS        = 270,
  TOK_MOVSB       = 271,
  TOK_MOVSW       = 272,
  TOK_MUL         = 273,
  TOK_NAME        = 274,
  TOK_NE          = 275,
  TOK_NEAR        = 276,
  TOK_NEARSTACK   = 277,
  TOK_NEG         = 278,
  TOK_NOP         = 279,
  TOK_NOT         = 280,
  TOK_NOTHING     = 281,
  TOK_OFFSET      = 282,
  TOK_OR          = 283,
  TOK_ORG         = 284,
  TOK_OUT         = 285,
  TOK_OUTS        = 286,
  TOK_OUTSB       = 287,
  TOK_OUTSW       = 288,
  TOK_PARA        = 289,
  TOK_PAGE        = 290,
  TOK_POP         = 291,
  TOK_POPA        = 292,
  TOK_POPF        = 293,
  TOK_PRIVATE     = 294,
  TOK_PTR         = 295,
  TOK_PUBLIC      = 296,
  TOK_PUSH        = 297,
  TOK_PUSHA       = 298,
  TOK_PUSHF       = 299,
  TOK_QWORD       = 300,
  TOK_RCL         = 301,
  TOK_RCR         = 302,
  TOK_REAL4       = 303,
  TOK_REAL8       = 304,
  TOK_REAL10      = 305,
  TOK_REP         = 306,
  TOK_REPE        = 307,
  TOK_REPNE       = 308,
  TOK_REPNZ       = 309,
  TOK_REPZ        = 310,
  TOK_RET         = 311,
  TOK_ROL         = 312,
  TOK_ROR         = 313,
  TOK_SAHF        = 314,
  TOK_SAL         = 315,
  TOK_SAR         = 316,
  TOK_SBB         = 317,
  TOK_SBYTE       = 318,
  TOK_SCAS        = 319,
  TOK_SCASB       = 320,
  TOK_SCASW       = 321,
  TOK_SDWORD      = 322,
  TOK_SEG         = 323,
  TOK_SEGMENT     = 324,
  TOK_SGDT        = 325,
  TOK_SHL         = 326,
  TOK_SHORT       = 327,
  TOK_SHR         = 328,
  TOK_SI          = 329,
  TOK_SIDT        = 330,
  TOK_SIZE        = 331,
  TOK_SIZEOF      = 332,
  TOK_SLDT        = 333,
  TOK_SMALL       = 334,
  TOK_SMSW        = 335,
  TOK_SP          = 336,
  TOK_SS          = 337,
  TOK_ST          = 338,
  TOK_STACK       = 339,
  TOK_STC         = 340,
  TOK_STD         = 341,
  TOK_STI         = 342,
  TOK_STOS        = 343,
  TOK_STOSB       = 344,
  TOK_STOSW       = 345,
  TOK_STR         = 346,
  TOK_SUB         = 347,
  TOK_SUBTITLE    = 348,
  TOK_SWORD       = 349,
  TOK_TBYTE       = 350,
  TOK_TEST        = 351,
  TOK_THIS        = 352,
  TOK_TINY        = 353,
  TOK_TITLE       = 354,
  TOK_TYPE        = 355,
  TOK_VERR        = 356,
  TOK_VERW        = 357,
  TOK_WAIT        = 358,
  TOK_WIDTH       = 359,
  TOK_WORD        = 360,
  TOK_XCHG        = 361,
  TOK_XLAT        = 362,
  TOK_XLATB       = 363,
  TOK_XOR         = 364,

  TOK_INTEGERBIN  = 365,
  TOK_INTEGEROCT  = 366,
  TOK_INTEGERDEC  = 367,
  TOK_INTEGERHEX  = 368,
  TOK_DOUBLE      = 369,
  TOK_STRING      = 370,
  TOK_IDENTIFIER  = 371,
  TOK_INST_LABEL  = 372,
  TOK_NEWLINE     = 373,
  TOK_WHITESPACE  = 374,
  TOK_TEXT        = 375,
  
/*
 * Productions
 */

  PRD_addOp           = 8128,
  PRD_altId           = 8129,
  PRD_andOp           = 8130,
  PRD_arbitaryText    = 8131,
  PRD_asmInstruction  = 8132,
  PRD_assumeDir       = 8133,
  PRD_assumeList      = 8134,
  PRD_assumeReg       = 8135,
  PRD_assumeRegister  = 8136,
  PRD_assumeSegReg    = 8137,
  PRD_assumeSegVal    = 8138,
  PRD_assumeVal       = 8139,
  PRD_byteRegister    = 8140,
  PRD_className       = 8141,
  PRD_commaOptNewline = 8142,
  PRD_constant        = 8143,
  PRD_constExpr       = 8144,
  PRD_coprocessor     = 8145,
  PRD_dataDecl        = 8146,
  PRD_dataDir         = 8147,
  PRD_dataItem        = 8148,
  PRD_dataType        = 8149,
  PRD_directive       = 8150,
  PRD_directiveList   = 8151,
  PRD_e01             = 8152,
  PRD_e02             = 8153,
  PRD_e03             = 8154,
  PRD_e04             = 8155,
  PRD_e05             = 8156,
  PRD_e06             = 8157,
  PRD_e07             = 8158,
  PRD_e07Op           = 8159,
  PRD_e08             = 8160,
  PRD_e09             = 8161,
  PRD_e10             = 8162,
  PRD_echoDir         = 8163,
  PRD_endDir          = 8164,
  PRD_endsDir         = 8165,
  PRD_equalDir        = 8166,
  PRD_equDir          = 8167,
  PRD_equType         = 8168,
  PRD_expr            = 8169,
  PRD_externDef       = 8170,
  PRD_externDir       = 8171,
  PRD_externList      = 8172,
  PRD_externType      = 8173,
  PRD_fpInstruction   = 8174,
  PRD_fpMnemonicOne   = 8175,
  PRD_fpMnemonicTwo   = 8176,
  PRD_fpMnemonicZero  = 8177,
  PRD_frameExpr       = 8178,
  PRD_generalDir      = 8179,
  PRD_gpRegister      = 8180,
  PRD_groupDir        = 8181,
  PRD_groupId         = 8182,
  PRD_immExpr         = 8183,
  PRD_initValue       = 8184,
  PRD_inSegDir        = 8185,
  PRD_inSegDirList    = 8186,
  PRD_inSegmentDir    = 8187,
  PRD_instPrefix      = 8188,
  PRD_instruction     = 8189,
  PRD_labelDef        = 8190,
  PRD_labelDir        = 8191,
  PRD_langType        = 8192,
  PRD_listDir         = 8193,
  PRD_listOption      = 8194,
  PRD_memOption       = 8195,
  PRD_mnemonicOne     = 8196,
  PRD_mnemonicTwo     = 8197,
  PRD_mnemonicZero    = 8198,
  PRD_modelDir        = 8199,
  PRD_modelOpt        = 8200,
  PRD_modelOptList    = 8201,
  PRD_module          = 8202,
  PRD_mulOp           = 8203,
  PRD_nameDir         = 8204,
  PRD_nearfar         = 8205,
  PRD_offsetDir       = 8206,
  PRD_offsetDirType   = 8207,
  PRD_operand         = 8208,
  PRD_orOp            = 8209,
  PRD_pageDir         = 8210,
  PRD_pageExpr        = 8211,
  PRD_pageLength      = 8212,
  PRD_pageWidth       = 8213,
  PRD_processor       = 8214,
  PRD_processorDir    = 8215,
  PRD_pubDef          = 8216,
  PRD_publicDir       = 8217,
  PRD_pubList         = 8218,
  PRD_qualifiedType   = 8219,
  PRD_register        = 8220,
  PRD_relOp           = 8221,
  PRD_scalarInstList  = 8222,
  PRD_segAlign        = 8223,
  PRD_segAttrib       = 8224,
  PRD_segDir          = 8225,
  PRD_segId           = 8226,
  PRD_segIdList       = 8227,
  PRD_segmentDef      = 8228,
  PRD_segmentDir      = 8229,
  PRD_segmentRegister = 8230,
  PRD_segOption       = 8231,
  PRD_segOptionList   = 8232,
  PRD_simpleSegDir    = 8233,
  PRD_sizeArg         = 8234,
  PRD_stackOption     = 8235,
  PRD_textLiteral     = 8236,
  PRD_textMacroId     = 8237,
  PRD_titleDir        = 8238,
  PRD_titleType       = 8239,
  PRD_type            = 8240,

  PRD_LAST            = 16128,

  XT_IGNORE       = 0001,
  XT_DROP         = 0002,
  XT_EXECUTE      = 0003,

} ;

#define PRODUCTION_OFFSET (8128)

/*
 * Value Type
 */

typedef union {
  int    i ;
  double d ;
  char  *s ;
} value_t ;

/*
 * Parse Tree Node
 */

typedef struct ptree_node_t ptree_node_t ;

struct ptree_node_t {
  unsigned int production_id ;
  unsigned int variant ;
  unsigned int num_args ;
  unsigned int exec_type ;
  ptree_node_t **args ;
  unsigned int value_type ;
  value_t value ;
} ;

/*
 * Token List Node
 */

typedef struct tlist_node_t tlist_node_t ;

struct tlist_node_t {
  unsigned int token_id ;
  char *token ;
  unsigned int value_type ;
  value_t value ;
  tlist_node_t *next ;
} ;

/*
 * Segment support.
 */

#define MAX_SEGMENT_STACK (8)  /* Size of Segment Stack            */
#define MAX_SEGMENT (8)        /* Maximum entries in Segment table */

#define ATTRIBUTE_RO  001  /* Read Only    */
#define ATTRIBUTE_EO  002  /* Execute Only */
#define ATTRIBUTE_COM 004  /* Common       */
#define ATTRIBUTE_PUB 010  /* Public       */

typedef struct segment_table_t segment_table_t;

struct segment_table_t {
  char *name ;
  unsigned int attributes;
  unsigned int position;
};

/*
 * Helper Macros.
 */

#define CPYVALUE(TGT, SRC) memcpy( &TGT, &SRC, sizeof( value_t ) )

/*
 * Prototypes.
 */

int assemble(const char *, int, int );
tlist_node_t * tokenize(const char *, int);
void delete_tlist( tlist_node_t ** );
void dump_pattern( void );
void error(void);
ptree_node_t * match( int, tlist_node_t **, int );
void delete_ptree( ptree_node_t *, int, int, int );
int execute ( ptree_node_t *, int, int );
unsigned int pid( const char * );
unsigned char to_byte( char );
int dep(unsigned char, int, int);
int depw(unsigned short, int, int);
int depd(unsigned int, int, int);
int setstr(char **, char *);
int get_symbol_value(const char *, int *);
int get_symbol_index(const char *);
int add_symbol(const char *, enum SymbolType, enum DataType, enum Distance, int, enum Language, int);
int add_label(const char *, enum Distance, int);
int segment_stack_top_index(void);
void dump_symbol_table(void);
int open_segment(const char *, int);
int open_segment_with_attributes(const char *, int, int);
int close_segment(const char *, int);
int get_current_position(unsigned int *, int);
void reset_for_pass(int);
segment_table_t *segment_stack_top(int);
int get_segment_index(const char *);
int segment_stack_count(void);
int set_current_position(unsigned int, int);
int dep_disp(unsigned char [], int, int, int, int);
unsigned char rm_disp_mod(ptree_node_t *, int *, int *);
int dep_opcodes(unsigned char [], int, int, int);
int is_word(int);
int process(const char *, int);
ptree_node_t * match2(int, FILE *, int);
ptree_node_t *find_token(int, FILE *);
int match_pattern2(FILE *, const char *, char **, unsigned long *);
void printProdName(int);
int checkInstruction(int);
char *get_segment_name(int);
int set_sumbol_visibility(const char *, enum Visibility);
void trim(char *);
extern int processor;
extern int coprocessor;
extern char *title;
extern char *subtitle;
extern int list;
extern int check_instructions;
extern int errno;
#endif /* asm286_h */


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
