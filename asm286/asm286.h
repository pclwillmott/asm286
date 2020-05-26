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
#include "asm286_t.h"

#ifndef asm286_h
#define asm286_h

//#define DEBUG2 1

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

/*
 * Token string constants.
 */

#define NUM_PATTERN ( 387 )  /* Number of token patterns */

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
#define STK_ALL         "\001\061"
#define STK_ALIGN       "\001\062"
#define STK_AND         "\001\063"
#define STK_ARPL        "\001\064"
#define STK_ASSUME      "\001\065"
#define STK_AT          "\001\066"
#define STK_AX          "\001\067"
#define STK_BH          "\001\070"
#define STK_BL          "\001\071"
#define STK_BOUND       "\001\072"
#define STK_BP          "\001\073"
#define STK_BX          "\001\074"
#define STK_BYTE        "\001\075"
#define STK_C           "\001\076"
#define STK_CALL        "\001\077"
#define STK_CASEMAP     "\001\100"
#define STK_CBW         "\001\101"
#define STK_CH          "\001\102"
#define STK_CL          "\001\103"
#define STK_CLC         "\001\104"
#define STK_CLD         "\001\105"
#define STK_CLI         "\001\106"
#define STK_CLTS        "\001\107"
#define STK_CMC         "\001\110"
#define STK_CMP         "\001\111"
#define STK_CMPS        "\001\112"
#define STK_CMPSB       "\001\113"
#define STK_CMPSW       "\001\114"
#define STK_COMMON      "\001\115"
#define STK_COMPACT     "\001\116"
#define STK_CS          "\001\117"
#define STK_CWD         "\001\120"
#define STK_CX          "\001\121"
#define STK_DAA         "\001\122"
#define STK_DAS         "\001\123"
#define STK_DB          "\001\124"
#define STK_DD          "\001\125"
#define STK_DEC         "\001\126"
#define STK_DF          "\001\127"
#define STK_DGROUP      "\001\130"
#define STK_DH          "\001\131"
#define STK_DI          "\001\132"
#define STK_DIV         "\001\133"
#define STK_DL          "\001\134"
#define STK_DQ          "\001\135"
#define STK_DS          "\001\136"
#define STK_DT          "\001\137"
#define STK_DUP         "\001\140"
#define STK_DW          "\001\141"
#define STK_DWORD       "\001\142"
#define STK_DX          "\001\143"
#define STK_ECHO        "\001\144"
#define STK_END         "\001\145"
#define STK_ENDS        "\001\146"
#define STK_ENTER       "\001\147"
#define STK_EQ          "\001\150"
#define STK_EQU         "\001\151"
#define STK_ERROR       "\001\152"
#define STK_ES          "\001\153"
#define STK_ESC         "\001\154"
#define STK_EVEN        "\001\155"
#define STK_EXTERN      "\001\156"
#define STK_F2XM1       "\001\157"
#define STK_FABS        "\001\160"
#define STK_FADD        "\001\161"
#define STK_FADDP       "\001\162"
#define STK_FAR         "\001\163"
#define STK_FARSTACK    "\001\164"
#define STK_FBLD        "\001\165"
#define STK_FBSTP       "\001\166"
#define STK_FCHS        "\001\167"
#define STK_FCLEX       "\001\170"
#define STK_FCOM        "\001\171"
#define STK_FCOMP       "\001\172"
#define STK_FCOMPP      "\001\173"
#define STK_FDECSTP     "\001\174"
#define STK_FDISI       "\001\175"
#define STK_FDIV        "\001\176"
#define STK_FDIVP       "\001\177"
#define STK_FDIVR       "\002\001"
#define STK_FDIVRP      "\002\002"
#define STK_FENI        "\002\003"
#define STK_FFREE       "\002\004"
#define STK_FIADD       "\002\005"
#define STK_FICOM       "\002\006"
#define STK_FICOMP      "\002\007"
#define STK_FIDIV       "\002\010"
#define STK_FIDIVR      "\002\011"
#define STK_FILD        "\002\012"
#define STK_FIMUL       "\002\013"
#define STK_FINCSTP     "\002\014"
#define STK_FINIT       "\002\015"
#define STK_FIST        "\002\016"
#define STK_FISTP       "\002\017"
#define STK_FISUB       "\002\020"
#define STK_FISUBR      "\002\021"
#define STK_FLD         "\002\022"
#define STK_FLD1        "\002\023"
#define STK_FLDCW       "\002\024"
#define STK_FLDENV      "\002\025"
#define STK_FLDL2E      "\002\026"
#define STK_FLDL2T      "\002\027"
#define STK_FLDLG2      "\002\030"
#define STK_FLDLN2      "\002\031"
#define STK_FLDPI       "\002\032"
#define STK_FLDZ        "\002\033"
#define STK_FMUL        "\002\034"
#define STK_FMULP       "\002\035"
#define STK_FNCLEX      "\002\036"
#define STK_FNDISI      "\002\037"
#define STK_FNENI       "\002\040"
#define STK_FNINIT      "\002\041"
#define STK_FNOP        "\002\042"
#define STK_FNSAVE      "\002\043"
#define STK_FNSTCW      "\002\044"
#define STK_FNSTENV     "\002\045"
#define STK_FNSTSW      "\002\046"
#define STK_FPATAN      "\002\047"
#define STK_FPREM       "\002\050"
#define STK_FPTAN       "\002\051"
#define STK_FRNDINT     "\002\052"
#define STK_FRSTOR      "\002\053"
#define STK_FSAVE       "\002\054"
#define STK_FSCALE      "\002\055"
#define STK_FSETPM      "\002\056"
#define STK_FSQRT       "\002\057"
#define STK_FST         "\002\060"
#define STK_FSTCW       "\002\061"
#define STK_FSTENV      "\002\062"
#define STK_FSTP        "\002\063"
#define STK_FSTSW       "\002\064"
#define STK_FSUB        "\002\065"
#define STK_FSUBP       "\002\066"
#define STK_FSUBR       "\002\067"
#define STK_FSUBRP      "\002\070"
#define STK_FTST        "\002\071"
#define STK_FWAIT       "\002\072"
#define STK_FWORD       "\002\073"
#define STK_FXAM        "\002\074"
#define STK_FXCH        "\002\075"
#define STK_FXTRACT     "\002\076"
#define STK_FYL2X       "\002\077"
#define STK_FYL2XP1     "\002\100"
#define STK_GE          "\002\101"
#define STK_GROUP       "\002\102"
#define STK_GT          "\002\103"
#define STK_HIGH        "\002\104"
#define STK_HUGE        "\002\105"
#define STK_HLT         "\002\106"
#define STK_IDIV        "\002\107"
#define STK_IMUL        "\002\110"
#define STK_IN          "\002\111"
#define STK_INC         "\002\112"
#define STK_INS         "\002\113"
#define STK_INSB        "\002\114"
#define STK_INSW        "\002\115"
#define STK_INT         "\002\116"
#define STK_INTO        "\002\117"
#define STK_IRET        "\002\120"
#define STK_JA          "\002\121"
#define STK_JAE         "\002\122"
#define STK_JB          "\002\123"
#define STK_JBE         "\002\124"
#define STK_JC          "\002\125"
#define STK_JCXZ        "\002\126"
#define STK_JE          "\002\127"
#define STK_JG          "\002\130"
#define STK_JGE         "\002\131"
#define STK_JL          "\002\132"
#define STK_JLE         "\002\133"
#define STK_JMP         "\002\134"
#define STK_JNA         "\002\135"
#define STK_JNAE        "\002\136"
#define STK_JNB         "\002\137"
#define STK_JNBE        "\002\140"
#define STK_JNC         "\002\141"
#define STK_JNE         "\002\142"
#define STK_JNG         "\002\143"
#define STK_JNGE        "\002\144"
#define STK_JNL         "\002\145"
#define STK_JNLE        "\002\146"
#define STK_JNO         "\002\147"
#define STK_JNP         "\002\150"
#define STK_JNS         "\002\151"
#define STK_JNZ         "\002\152"
#define STK_JO          "\002\153"
#define STK_JP          "\002\154"
#define STK_JPE         "\002\155"
#define STK_JPO         "\002\156"
#define STK_JS          "\002\157"
#define STK_JZ          "\002\160"
#define STK_LABEL       "\002\161"
#define STK_LAHF        "\002\162"
#define STK_LANGUAGE    "\002\163"
#define STK_LAR         "\002\164"
#define STK_LARGE       "\002\165"
#define STK_LDS         "\002\166"
#define STK_LE          "\002\167"
#define STK_LEA         "\002\170"
#define STK_LEAVE       "\002\171"
#define STK_LENGTH      "\002\172"
#define STK_LENGTHOF    "\002\173"
#define STK_LES         "\002\174"
#define STK_LGDT        "\002\175"
#define STK_LIDT        "\002\176"
#define STK_LJMP        "\002\177"
#define STK_LLDT        "\003\001"
#define STK_LMSW        "\003\002"
#define STK_LOCK        "\003\003"
#define STK_LODS        "\003\004"
#define STK_LODSB       "\003\005"
#define STK_LODSW       "\003\006"
#define STK_LOOP        "\003\007"
#define STK_LOOPE       "\003\010"
#define STK_LOOPNE      "\003\011"
#define STK_LOOPNZ      "\003\012"
#define STK_LOOPZ       "\003\013"
#define STK_LOW         "\003\014"
#define STK_LSL         "\003\015"
#define STK_LT          "\003\016"
#define STK_LTR         "\003\017"
#define STK_MASK        "\003\020"
#define STK_MEDIUM      "\003\021"
#define STK_MEMORY      "\003\022"
#define STK_MOD         "\003\023"
#define STK_MOV         "\003\024"
#define STK_MOVS        "\003\025"
#define STK_MOVSB       "\003\026"
#define STK_MOVSW       "\003\027"
#define STK_MUL         "\003\030"
#define STK_NAME        "\003\031"
#define STK_NE          "\003\032"
#define STK_NEAR        "\003\033"
#define STK_NEARSTACK   "\003\034"
#define STK_NEG         "\003\035"
#define STK_NOLJMP      "\003\036"
#define STK_NONE        "\003\037"
#define STK_NOP         "\003\040"
#define STK_NOREADONLY  "\003\041"
#define STK_NOT         "\003\042"
#define STK_NOTHING     "\003\043"
#define STK_NOTPUBLIC   "\003\044"
#define STK_OFFSET      "\003\045"
#define STK_OPTION      "\003\046"
#define STK_OR          "\003\047"
#define STK_ORG         "\003\050"
#define STK_OUT         "\003\051"
#define STK_OUTS        "\003\052"
#define STK_OUTSB       "\003\053"
#define STK_OUTSW       "\003\054"
#define STK_PARA        "\003\055"
#define STK_PAGE        "\003\056"
#define STK_PASCAL      "\003\057"
#define STK_POP         "\003\060"
#define STK_POPA        "\003\061"
#define STK_POPF        "\003\062"
#define STK_PRIVATE     "\003\063"
#define STK_PTR         "\003\064"
#define STK_PUBLIC      "\003\065"
#define STK_PUSH        "\003\066"
#define STK_PUSHA       "\003\067"
#define STK_PUSHF       "\003\070"
#define STK_QWORD       "\003\071"
#define STK_RCL         "\003\072"
#define STK_RCR         "\003\073"
#define STK_READONLY    "\003\074"
#define STK_REAL4       "\003\075"
#define STK_REAL8       "\003\076"
#define STK_REAL10      "\003\077"
#define STK_REP         "\003\100"
#define STK_REPE        "\003\101"
#define STK_REPNE       "\003\102"
#define STK_REPNZ       "\003\103"
#define STK_REPZ        "\003\104"
#define STK_RET         "\003\105"
#define STK_ROL         "\003\106"
#define STK_ROR         "\003\107"
#define STK_SAHF        "\003\110"
#define STK_SAL         "\003\111"
#define STK_SAR         "\003\112"
#define STK_SBB         "\003\113"
#define STK_SBYTE       "\003\114"
#define STK_SCAS        "\003\115"
#define STK_SCASB       "\003\116"
#define STK_SCASW       "\003\117"
#define STK_SDWORD      "\003\120"
#define STK_SEG         "\003\121"
#define STK_SEGMENT     "\003\122"
#define STK_SGDT        "\003\123"
#define STK_SHL         "\003\124"
#define STK_SHORT       "\003\125"
#define STK_SHR         "\003\126"
#define STK_SI          "\003\127"
#define STK_SIDT        "\003\130"
#define STK_SIZE        "\003\131"
#define STK_SIZEOF      "\003\132"
#define STK_SLDT        "\003\133"
#define STK_SMALL       "\003\134"
#define STK_SMSW        "\003\135"
#define STK_SP          "\003\136"
#define STK_SS          "\003\137"
#define STK_ST          "\003\140"
#define STK_STACK       "\003\141"
#define STK_STC         "\003\142"
#define STK_STD         "\003\143"
#define STK_STI         "\003\144"
#define STK_STOS        "\003\145"
#define STK_STOSB       "\003\146"
#define STK_STOSW       "\003\147"
#define STK_STR         "\003\150"
#define STK_SUB         "\003\151"
#define STK_SUBTITLE    "\003\152"
#define STK_SWORD       "\003\153"
#define STK_TBYTE       "\003\154"
#define STK_TEST        "\003\155"
#define STK_THIS        "\003\156"
#define STK_TINY        "\003\157"
#define STK_TITLE       "\003\160"
#define STK_TYPE        "\003\161"
#define STK_VERR        "\003\162"
#define STK_VERW        "\003\163"
#define STK_WAIT        "\003\164"
#define STK_WIDTH       "\003\165"
#define STK_WORD        "\003\166"
#define STK_XCHG        "\003\167"
#define STK_XLAT        "\003\170"
#define STK_XLATB       "\003\171"
#define STK_XOR         "\003\172"

/*
 * END OF AUTONUBER SECTION
 */

#define STK_INTEGERBIN  "\003\173"
#define STK_INTEGEROCT  "\003\174"
#define STK_INTEGERDEC  "\003\175"
#define STK_INTEGERHEX  "\003\176"
#define STK_DOUBLE      "\003\177"
#define STK_STRING      "\004\001"
#define STK_IDENTIFIER  "\004\002"
#define STK_INST_LABEL  "\004\003"
#define STK_NEWLINE     "\004\004"
#define STK_WHITESPACE  "\004\005"
#define STK_TEXT        "\004\006"

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
#define SPD_e08Op           "\101\040"
#define SPD_e08             "\101\041"
#define SPD_e09             "\101\042"
#define SPD_e10             "\101\043"
#define SPD_e11             "\101\044"
#define SPD_e12             "\101\045"
#define SPD_echoDir         "\101\046"
#define SPD_endDir          "\101\047"
#define SPD_endsDir         "\101\050"
#define SPD_equalDir        "\101\051"
#define SPD_equDir          "\101\052"
#define SPD_equType         "\101\053"
#define SPD_expr            "\101\054"
#define SPD_externDef       "\101\055"
#define SPD_externDir       "\101\056"
#define SPD_externList      "\101\057"
#define SPD_externType      "\101\060"
#define SPD_fpInstruction   "\101\061"
#define SPD_fpMnemonicOne   "\101\062"
#define SPD_fpMnemonicTwo   "\101\063"
#define SPD_fpMnemonicZero  "\101\064"
#define SPD_frameExpr       "\101\065"
#define SPD_generalDir      "\101\066"
#define SPD_gpRegister      "\101\067"
#define SPD_groupDir        "\101\070"
#define SPD_groupId         "\101\071"
#define SPD_immExpr         "\101\072"
#define SPD_initValue       "\101\073"
#define SPD_inSegDir        "\101\074"
#define SPD_inSegDirList    "\101\075"
#define SPD_inSegmentDir    "\101\076"
#define SPD_instPrefix      "\101\077"
#define SPD_instruction     "\101\100"
#define SPD_labelDef        "\101\101"
#define SPD_labelDir        "\101\102"
#define SPD_langType        "\101\103"
#define SPD_listDir         "\101\104"
#define SPD_listOption      "\101\105"
#define SPD_mapType         "\101\106"
#define SPD_memOption       "\101\107"
#define SPD_mnemonicOne     "\101\110"
#define SPD_mnemonicTwo     "\101\111"
#define SPD_mnemonicZero    "\101\112"
#define SPD_modelDir        "\101\113"
#define SPD_modelOpt        "\101\114"
#define SPD_modelOptList    "\101\115"
#define SPD_module          "\101\116"
#define SPD_mulOp           "\101\117"
#define SPD_nameDir         "\101\120"
#define SPD_nearfar         "\101\121"
#define SPD_offsetDir       "\101\122"
#define SPD_offsetDirType   "\101\123"
#define SPD_operand         "\101\124"
#define SPD_optionDir       "\101\125"
#define SPD_optionItem      "\101\126"
#define SPD_optionList      "\101\127"
#define SPD_orOp            "\101\130"
#define SPD_pageDir         "\101\131"
#define SPD_pageExpr        "\101\132"
#define SPD_pageLength      "\101\133"
#define SPD_pageWidth       "\101\134"
#define SPD_processor       "\101\135"
#define SPD_processorDir    "\101\136"
#define SPD_pubDef          "\101\137"
#define SPD_publicDir       "\101\140"
#define SPD_pubList         "\101\141"
#define SPD_qualifiedType   "\101\142"
#define SPD_register        "\101\143"
#define SPD_relOp           "\101\144"
#define SPD_scalarInstList  "\101\145"
#define SPD_segAlign        "\101\146"
#define SPD_segAttrib       "\101\147"
#define SPD_segDir          "\101\150"
#define SPD_segId           "\101\151"
#define SPD_segIdList       "\101\152"
#define SPD_segmentDef      "\101\153"
#define SPD_segmentDir      "\101\154"
#define SPD_segmentRegister "\101\155"
#define SPD_segOption       "\101\156"
#define SPD_segOptionList   "\101\157"
#define SPD_simpleSegDir    "\101\160"
#define SPD_sizeArg         "\101\161"
#define SPD_stackOption     "\101\162"
#define SPD_textLiteral     "\101\163"
#define SPD_textMacroId     "\101\164"
#define SPD_titleDir        "\101\165"
#define SPD_titleType       "\101\166"
#define SPD_type            "\101\167"

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
  TOK_ALL         =  48,
  TOK_ALIGN       =  49,
  TOK_AND         =  50,
  TOK_ARPL        =  51,
  TOK_ASSUME      =  52,
  TOK_AT          =  53,
  TOK_AX          =  54,
  TOK_BH          =  55,
  TOK_BL          =  56,
  TOK_BOUND       =  57,
  TOK_BP          =  58,
  TOK_BX          =  59,
  TOK_BYTE        =  60,
  TOK_C           =  61,
  TOK_CALL        =  62,
  TOK_CASEMAP     =  63,
  TOK_CBW         =  64,
  TOK_CH          =  65,
  TOK_CL          =  66,
  TOK_CLC         =  67,
  TOK_CLD         =  68,
  TOK_CLI         =  69,
  TOK_CLTS        =  70,
  TOK_CMC         =  71,
  TOK_CMP         =  72,
  TOK_CMPS        =  73,
  TOK_CMPSB       =  74,
  TOK_CMPSW       =  75,
  TOK_COMMON      =  76,
  TOK_COMPACT     =  77,
  TOK_CS          =  78,
  TOK_CWD         =  79,
  TOK_CX          =  80,
  TOK_DAA         =  81,
  TOK_DAS         =  82,
  TOK_DB          =  83,
  TOK_DD          =  84,
  TOK_DEC         =  85,
  TOK_DF          =  86,
  TOK_DGROUP      =  87,
  TOK_DH          =  88,
  TOK_DI          =  89,
  TOK_DIV         =  90,
  TOK_DL          =  91,
  TOK_DQ          =  92,
  TOK_DS          =  93,
  TOK_DT          =  94,
  TOK_DUP         =  95,
  TOK_DW          =  96,
  TOK_DWORD       =  97,
  TOK_DX          =  98,
  TOK_ECHO        =  99,
  TOK_END         = 100,
  TOK_ENDS        = 101,
  TOK_ENTER       = 102,
  TOK_EQ          = 103,
  TOK_EQU         = 104,
  TOK_ERROR       = 105,
  TOK_ES          = 106,
  TOK_ESC         = 107,
  TOK_EVEN        = 108,
  TOK_EXTERN      = 109,
  TOK_F2XM1       = 110,
  TOK_FABS        = 111,
  TOK_FADD        = 112,
  TOK_FADDP       = 113,
  TOK_FAR         = 114,
  TOK_FARSTACK    = 115,
  TOK_FBLD        = 116,
  TOK_FBSTP       = 117,
  TOK_FCHS        = 118,
  TOK_FCLEX       = 119,
  TOK_FCOM        = 120,
  TOK_FCOMP       = 121,
  TOK_FCOMPP      = 122,
  TOK_FDECSTP     = 123,
  TOK_FDISI       = 124,
  TOK_FDIV        = 125,
  TOK_FDIVP       = 126,
  TOK_FDIVR       = 127,
  TOK_FDIVRP      = 128,
  TOK_FENI        = 129,
  TOK_FFREE       = 130,
  TOK_FIADD       = 131,
  TOK_FICOM       = 132,
  TOK_FICOMP      = 133,
  TOK_FIDIV       = 134,
  TOK_FIDIVR      = 135,
  TOK_FILD        = 136,
  TOK_FIMUL       = 137,
  TOK_FINCSTP     = 138,
  TOK_FINIT       = 139,
  TOK_FIST        = 140,
  TOK_FISTP       = 141,
  TOK_FISUB       = 142,
  TOK_FISUBR      = 143,
  TOK_FLD         = 144,
  TOK_FLD1        = 145,
  TOK_FLDCW       = 146,
  TOK_FLDENV      = 147,
  TOK_FLDL2E      = 148,
  TOK_FLDL2T      = 149,
  TOK_FLDLG2      = 150,
  TOK_FLDLN2      = 151,
  TOK_FLDPI       = 152,
  TOK_FLDZ        = 153,
  TOK_FMUL        = 154,
  TOK_FMULP       = 155,
  TOK_FNCLEX      = 156,
  TOK_FNDISI      = 157,
  TOK_FNENI       = 158,
  TOK_FNINIT      = 159,
  TOK_FNOP        = 160,
  TOK_FNSAVE      = 161,
  TOK_FNSTCW      = 162,
  TOK_FNSTENV     = 163,
  TOK_FNSTSW      = 164,
  TOK_FPATAN      = 165,
  TOK_FPREM       = 166,
  TOK_FPTAN       = 167,
  TOK_FRNDINT     = 168,
  TOK_FRSTOR      = 169,
  TOK_FSAVE       = 170,
  TOK_FSCALE      = 171,
  TOK_FSETPM      = 172,
  TOK_FSQRT       = 173,
  TOK_FST         = 174,
  TOK_FSTCW       = 175,
  TOK_FSTENV      = 176,
  TOK_FSTP        = 177,
  TOK_FSTSW       = 178,
  TOK_FSUB        = 179,
  TOK_FSUBP       = 180,
  TOK_FSUBR       = 181,
  TOK_FSUBRP      = 182,
  TOK_FTST        = 183,
  TOK_FWAIT       = 184,
  TOK_FWORD       = 185,
  TOK_FXAM        = 186,
  TOK_FXCH        = 187,
  TOK_FXTRACT     = 188,
  TOK_FYL2X       = 189,
  TOK_FYL2XP1     = 190,
  TOK_GE          = 191,
  TOK_GROUP       = 192,
  TOK_GT          = 193,
  TOK_HIGH        = 194,
  TOK_HUGE        = 195,
  TOK_HLT         = 196,
  TOK_IDIV        = 197,
  TOK_IMUL        = 198,
  TOK_IN          = 199,
  TOK_INC         = 200,
  TOK_INS         = 201,
  TOK_INSB        = 202,
  TOK_INSW        = 203,
  TOK_INT         = 204,
  TOK_INTO        = 205,
  TOK_IRET        = 206,
  TOK_JA          = 207,
  TOK_JAE         = 208,
  TOK_JB          = 209,
  TOK_JBE         = 210,
  TOK_JC          = 211,
  TOK_JCXZ        = 212,
  TOK_JE          = 213,
  TOK_JG          = 214,
  TOK_JGE         = 215,
  TOK_JL          = 216,
  TOK_JLE         = 217,
  TOK_JMP         = 218,
  TOK_JNA         = 219,
  TOK_JNAE        = 220,
  TOK_JNB         = 221,
  TOK_JNBE        = 222,
  TOK_JNC         = 223,
  TOK_JNE         = 224,
  TOK_JNG         = 225,
  TOK_JNGE        = 226,
  TOK_JNL         = 227,
  TOK_JNLE        = 228,
  TOK_JNO         = 229,
  TOK_JNP         = 230,
  TOK_JNS         = 231,
  TOK_JNZ         = 232,
  TOK_JO          = 233,
  TOK_JP          = 234,
  TOK_JPE         = 235,
  TOK_JPO         = 236,
  TOK_JS          = 237,
  TOK_JZ          = 238,
  TOK_LABEL       = 239,
  TOK_LAHF        = 240,
  TOK_LANGUAGE    = 241,
  TOK_LAR         = 242,
  TOK_LARGE       = 243,
  TOK_LDS         = 244,
  TOK_LE          = 245,
  TOK_LEA         = 246,
  TOK_LEAVE       = 247,
  TOK_LENGTH      = 248,
  TOK_LENGTHOF    = 249,
  TOK_LES         = 250,
  TOK_LGDT        = 251,
  TOK_LIDT        = 252,
  TOK_LJMP        = 253,
  TOK_LLDT        = 254,
  TOK_LMSW        = 255,
  TOK_LOCK        = 256,
  TOK_LODS        = 257,
  TOK_LODSB       = 258,
  TOK_LODSW       = 259,
  TOK_LOOP        = 260,
  TOK_LOOPE       = 261,
  TOK_LOOPNE      = 262,
  TOK_LOOPNZ      = 263,
  TOK_LOOPZ       = 264,
  TOK_LOW         = 265,
  TOK_LSL         = 266,
  TOK_LT          = 267,
  TOK_LTR         = 268,
  TOK_MASK        = 269,
  TOK_MEDIUM      = 270,
  TOK_MEMORY      = 271,
  TOK_MOD         = 272,
  TOK_MOV         = 273,
  TOK_MOVS        = 274,
  TOK_MOVSB       = 275,
  TOK_MOVSW       = 276,
  TOK_MUL         = 277,
  TOK_NAME        = 278,
  TOK_NE          = 279,
  TOK_NEAR        = 280,
  TOK_NEARSTACK   = 281,
  TOK_NEG         = 282,
  TOK_NOLJMP      = 283,
  TOK_NONE        = 284,
  TOK_NOP         = 285,
  TOK_NOREADONLY  = 286,
  TOK_NOT         = 287,
  TOK_NOTHING     = 288,
  TOK_NOTPUBLIC   = 289,
  TOK_OFFSET      = 290,
  TOK_OPTION      = 291,
  TOK_OR          = 292,
  TOK_ORG         = 293,
  TOK_OUT         = 294,
  TOK_OUTS        = 295,
  TOK_OUTSB       = 296,
  TOK_OUTSW       = 297,
  TOK_PARA        = 298,
  TOK_PAGE        = 299,
  TOK_PASCAL      = 300,
  TOK_POP         = 301,
  TOK_POPA        = 302,
  TOK_POPF        = 303,
  TOK_PRIVATE     = 304,
  TOK_PTR         = 305,
  TOK_PUBLIC      = 306,
  TOK_PUSH        = 307,
  TOK_PUSHA       = 308,
  TOK_PUSHF       = 309,
  TOK_QWORD       = 310,
  TOK_RCL         = 311,
  TOK_RCR         = 312,
  TOK_READONLY    = 313,
  TOK_REAL4       = 314,
  TOK_REAL8       = 315,
  TOK_REAL10      = 316,
  TOK_REP         = 317,
  TOK_REPE        = 318,
  TOK_REPNE       = 319,
  TOK_REPNZ       = 320,
  TOK_REPZ        = 321,
  TOK_RET         = 322,
  TOK_ROL         = 323,
  TOK_ROR         = 324,
  TOK_SAHF        = 325,
  TOK_SAL         = 326,
  TOK_SAR         = 327,
  TOK_SBB         = 328,
  TOK_SBYTE       = 329,
  TOK_SCAS        = 330,
  TOK_SCASB       = 331,
  TOK_SCASW       = 332,
  TOK_SDWORD      = 333,
  TOK_SEG         = 334,
  TOK_SEGMENT     = 335,
  TOK_SGDT        = 336,
  TOK_SHL         = 337,
  TOK_SHORT       = 338,
  TOK_SHR         = 339,
  TOK_SI          = 340,
  TOK_SIDT        = 341,
  TOK_SIZE        = 342,
  TOK_SIZEOF      = 343,
  TOK_SLDT        = 344,
  TOK_SMALL       = 345,
  TOK_SMSW        = 346,
  TOK_SP          = 347,
  TOK_SS          = 348,
  TOK_ST          = 349,
  TOK_STACK       = 350,
  TOK_STC         = 351,
  TOK_STD         = 352,
  TOK_STI         = 353,
  TOK_STOS        = 354,
  TOK_STOSB       = 355,
  TOK_STOSW       = 356,
  TOK_STR         = 357,
  TOK_SUB         = 358,
  TOK_SUBTITLE    = 359,
  TOK_SWORD       = 360,
  TOK_TBYTE       = 361,
  TOK_TEST        = 362,
  TOK_THIS        = 363,
  TOK_TINY        = 364,
  TOK_TITLE       = 365,
  TOK_TYPE        = 366,
  TOK_VERR        = 367,
  TOK_VERW        = 368,
  TOK_WAIT        = 369,
  TOK_WIDTH       = 370,
  TOK_WORD        = 371,
  TOK_XCHG        = 372,
  TOK_XLAT        = 373,
  TOK_XLATB       = 374,
  TOK_XOR         = 375,

  TOK_INTEGERBIN  = 376,
  TOK_INTEGEROCT  = 377,
  TOK_INTEGERDEC  = 378,
  TOK_INTEGERHEX  = 379,
  TOK_DOUBLE      = 380,
  TOK_STRING      = 381,
  TOK_IDENTIFIER  = 382,
  TOK_INST_LABEL  = 383,
  TOK_NEWLINE     = 384,
  TOK_WHITESPACE  = 385,
  TOK_TEXT        = 386,
  
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
  PRD_e08Op           = 8159,
  PRD_e08             = 8160,
  PRD_e09             = 8161,
  PRD_e10             = 8162,
  PRD_e11             = 8163,
  PRD_e12             = 8164,
  PRD_echoDir         = 8165,
  PRD_endDir          = 8166,
  PRD_endsDir         = 8167,
  PRD_equalDir        = 8168,
  PRD_equDir          = 8169,
  PRD_equType         = 8170,
  PRD_expr            = 8171,
  PRD_externDef       = 8172,
  PRD_externDir       = 8173,
  PRD_externList      = 8174,
  PRD_externType      = 8175,
  PRD_fpInstruction   = 8176,
  PRD_fpMnemonicOne   = 8177,
  PRD_fpMnemonicTwo   = 8178,
  PRD_fpMnemonicZero  = 8179,
  PRD_frameExpr       = 8180,
  PRD_generalDir      = 8181,
  PRD_gpRegister      = 8182,
  PRD_groupDir        = 8183,
  PRD_groupId         = 8184,
  PRD_immExpr         = 8185,
  PRD_initValue       = 8186,
  PRD_inSegDir        = 8187,
  PRD_inSegDirList    = 8188,
  PRD_inSegmentDir    = 8189,
  PRD_instPrefix      = 8190,
  PRD_instruction     = 8191,
  PRD_labelDef        = 8192,
  PRD_labelDir        = 8193,
  PRD_langType        = 8194,
  PRD_listDir         = 8195,
  PRD_listOption      = 8196,
  PRD_mapType         = 8197,
  PRD_memOption       = 8198,
  PRD_mnemonicOne     = 8199,
  PRD_mnemonicTwo     = 8200,
  PRD_mnemonicZero    = 8201,
  PRD_modelDir        = 8202,
  PRD_modelOpt        = 8203,
  PRD_modelOptList    = 8204,
  PRD_module          = 8205,
  PRD_mulOp           = 8206,
  PRD_nameDir         = 8207,
  PRD_nearfar         = 8208,
  PRD_offsetDir       = 8209,
  PRD_offsetDirType   = 8210,
  PRD_operand         = 8211,
  PRD_optionDir       = 8212,
  PRD_optionItem      = 8213,
  PRD_optionList      = 8214,
  PRD_orOp            = 8215,
  PRD_pageDir         = 8216,
  PRD_pageExpr        = 8217,
  PRD_pageLength      = 8218,
  PRD_pageWidth       = 8219,
  PRD_processor       = 8220,
  PRD_processorDir    = 8221,
  PRD_pubDef          = 8222,
  PRD_publicDir       = 8223,
  PRD_pubList         = 8224,
  PRD_qualifiedType   = 8225,
  PRD_register        = 8226,
  PRD_relOp           = 8227,
  PRD_scalarInstList  = 8228,
  PRD_segAlign        = 8229,
  PRD_segAttrib       = 8230,
  PRD_segDir          = 8231,
  PRD_segId           = 8232,
  PRD_segIdList       = 8233,
  PRD_segmentDef      = 8234,
  PRD_segmentDir      = 8235,
  PRD_segmentRegister = 8236,
  PRD_segOption       = 8237,
  PRD_segOptionList   = 8238,
  PRD_simpleSegDir    = 8239,
  PRD_sizeArg         = 8240,
  PRD_stackOption     = 8241,
  PRD_textLiteral     = 8242,
  PRD_textMacroId     = 8243,
  PRD_titleDir        = 8244,
  PRD_titleType       = 8245,
  PRD_type            = 8246,

  PRD_LAST            = 16128,

  XT_IGNORE       = 0001,
  XT_DROP         = 0002,
  XT_EXECUTE      = 0003,

} ;

#define PRODUCTION_OFFSET (8128)

/*
 * Segment support.
 */

#define MAX_SEGMENT_STACK (8)  /* Size of Segment Stack            */
#define MAX_SEGMENT (8)        /* Maximum entries in Segment table */

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
int set_variable(const char *, enum DataType, int);
int set_model(enum MemoryModel);

int add_label(const char *, enum Distance, int);
int segment_stack_top_index(void);
void dump_symbol_table(void);
int open_segment(const char *, int);
int open_segment_with_attributes(const char *, int, int);
int close_segment(const char *, int);
int get_current_position(unsigned int *);
void reset_for_pass(int);
segment_table_t *segment_stack_top(void);
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
void dump_segment_table(void);

// COMMON DATA

extern int processor;
extern int coprocessor;
extern char *title;
extern char *subtitle;
extern int list;
extern int check_instructions;
extern int errno;
extern enum Language language_type;
extern enum Distance stack_distance;
extern int main_routine;

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
