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

#ifndef asm286_h
#define asm286_h

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
} ;

#define NUM_ERR ( 7 )

/*
 * Token types.
 */

#define KEYWORD          "\001"
#define RESERVED         "\002"
#define KEYWORD_RESERVED "\003"
#define SPECIAL          "\004"

/*
 * Token string constants.
 */

#define NUM_PATTERN ( 355 )  /* Number of token patterns */

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

/*
 * BEGIN AUTONUMBER SECTION
 */

#define STK_AAA         "\001\026"
#define STK_AAD         "\001\027"
#define STK_AAM         "\001\030"
#define STK_AAS         "\001\031"
#define STK_ABS         "\001\032"
#define STK_ADC         "\001\033"
#define STK_ADD         "\001\034"
#define STK_AH          "\001\035"
#define STK_AL          "\001\036"
#define STK_ALIGN       "\001\037"
#define STK_AND         "\001\040"
#define STK_ARPL        "\001\041"
#define STK_ASSUME      "\001\042"
#define STK_AX          "\001\043"
#define STK_BH          "\001\044"
#define STK_BL          "\001\045"
#define STK_BOUND       "\001\046"
#define STK_BP          "\001\047"
#define STK_BSWAP       "\001\050"
#define STK_BX          "\001\051"
#define STK_BYTE        "\001\052"
#define STK_CALL        "\001\053"
#define STK_CBW         "\001\054"
#define STK_CH          "\001\055"
#define STK_CL          "\001\056"
#define STK_CLC         "\001\057"
#define STK_CLD         "\001\060"
#define STK_CLI         "\001\061"
#define STK_CLTS        "\001\062"
#define STK_CMC         "\001\063"
#define STK_CMP         "\001\064"
#define STK_CMPS        "\001\065"
#define STK_CMPSB       "\001\066"
#define STK_CMPSW       "\001\067"
#define STK_CMPXCHG     "\001\070"
#define STK_CODEMACRO   "\001\071"
#define STK_COMMON      "\001\072"
#define STK_CS          "\001\073"
#define STK_CWD         "\001\074"
#define STK_CX          "\001\075"
#define STK_DAA         "\001\076"
#define STK_DAS         "\001\077"
#define STK_DB          "\001\100"
#define STK_DD          "\001\101"
#define STK_DEC         "\001\102"
#define STK_DH          "\001\103"
#define STK_DI          "\001\104"
#define STK_DIV         "\001\105"
#define STK_DL          "\001\106"
#define STK_DQ          "\001\107"
#define STK_DS          "\001\110"
#define STK_DT          "\001\111"
#define STK_DUP         "\001\112"
#define STK_DW          "\001\113"
#define STK_DWORD       "\001\114"
#define STK_DX          "\001\115"
#define STK_END         "\001\116"
#define STK_ENDM        "\001\117"
#define STK_ENDP        "\001\120"
#define STK_ENDS        "\001\121"
#define STK_ENTER       "\001\122"
#define STK_EO          "\001\123"
#define STK_EQ          "\001\124"
#define STK_EQU         "\001\125"
#define STK_ER          "\001\126"
#define STK_ES          "\001\127"
#define STK_ESC         "\001\130"
#define STK_EVEN        "\001\131"
#define STK_EXTRN       "\001\132"
#define STK_F2XM1       "\001\133"
#define STK_FABS        "\001\134"
#define STK_FADD        "\001\135"
#define STK_FADDP       "\001\136"
#define STK_FAR         "\001\137"
#define STK_FBLD        "\001\140"
#define STK_FBSTP       "\001\141"
#define STK_FCHS        "\001\142"
#define STK_FCLEX       "\001\143"
#define STK_FCOM        "\001\144"
#define STK_FCOMP       "\001\145"
#define STK_FCOMPP      "\001\146"
#define STK_FDECSTP     "\001\147"
#define STK_FDISI       "\001\150"
#define STK_FDIV        "\001\151"
#define STK_FDIVP       "\001\152"
#define STK_FDIVR       "\001\153"
#define STK_FDIVRP      "\001\154"
#define STK_FENI        "\001\155"
#define STK_FFREE       "\001\156"
#define STK_FIADD       "\001\157"
#define STK_FICOM       "\001\160"
#define STK_FICOMP      "\001\161"
#define STK_FIDIV       "\001\162"
#define STK_FIDIVR      "\001\163"
#define STK_FILD        "\001\164"
#define STK_FIMUL       "\001\165"
#define STK_FINCSTP     "\001\166"
#define STK_FINIT       "\001\167"
#define STK_FIST        "\001\170"
#define STK_FISTP       "\001\171"
#define STK_FISUB       "\001\172"
#define STK_FISUBR      "\001\173"
#define STK_FLD         "\001\174"
#define STK_FLD1        "\001\175"
#define STK_FLDCW       "\001\176"
#define STK_FLDENV      "\001\177"
#define STK_FLDL2E      "\002\001"
#define STK_FLDL2T      "\002\002"
#define STK_FLDLG2      "\002\003"
#define STK_FLDLN2      "\002\004"
#define STK_FLDPI       "\002\005"
#define STK_FLDZ        "\002\006"
#define STK_FMUL        "\002\007"
#define STK_FMULP       "\002\010"
#define STK_FNCLEX      "\002\011"
#define STK_FNDISI      "\002\012"
#define STK_FNENI       "\002\013"
#define STK_FNINIT      "\002\014"
#define STK_FNOP        "\002\015"
#define STK_FNSAVE      "\002\016"
#define STK_FNSTCW      "\002\017"
#define STK_FNSTENV     "\002\020"
#define STK_FNSTSW      "\002\021"
#define STK_FPATAN      "\002\022"
#define STK_FPREM       "\002\023"
#define STK_FPTAN       "\002\024"
#define STK_FRNDINT     "\002\025"
#define STK_FRSTOR      "\002\026"
#define STK_FSAVE       "\002\027"
#define STK_FSCALE      "\002\030"
#define STK_FSETPM      "\002\031"
#define STK_FSQRT       "\002\032"
#define STK_FST         "\002\033"
#define STK_FSTCW       "\002\034"
#define STK_FSTENV      "\002\035"
#define STK_FSTP        "\002\036"
#define STK_FSTSW       "\002\037"
#define STK_FSUB        "\002\040"
#define STK_FSUBP       "\002\041"
#define STK_FSUBR       "\002\042"
#define STK_FSUBRP      "\002\043"
#define STK_FTST        "\002\044"
#define STK_FWAIT       "\002\045"
#define STK_FXAM        "\002\046"
#define STK_FXCH        "\002\047"
#define STK_FXTRACT     "\002\050"
#define STK_FYL2X       "\002\051"
#define STK_FYL2XP1     "\002\052"
#define STK_GE          "\002\053"
#define STK_GT          "\002\054"
#define STK_HIGH        "\002\055"
#define STK_HLT         "\002\056"
#define STK_IDIV        "\002\057"
#define STK_IMUL        "\002\060"
#define STK_IN          "\002\061"
#define STK_INC         "\002\062"
#define STK_INS         "\002\063"
#define STK_INSB        "\002\064"
#define STK_INSW        "\002\065"
#define STK_INT         "\002\066"
#define STK_INTO        "\002\067"
#define STK_INVD        "\002\070"
#define STK_INVLPG      "\002\071"
#define STK_IRET        "\002\072"
#define STK_JA          "\002\073"
#define STK_JAE         "\002\074"
#define STK_JB          "\002\075"
#define STK_JBE         "\002\076"
#define STK_JC          "\002\077"
#define STK_JCXZ        "\002\100"
#define STK_JE          "\002\101"
#define STK_JG          "\002\102"
#define STK_JGE         "\002\103"
#define STK_JL          "\002\104"
#define STK_JLE         "\002\105"
#define STK_JMP         "\002\106"
#define STK_JNA         "\002\107"
#define STK_JNAE        "\002\110"
#define STK_JNB         "\002\111"
#define STK_JNBE        "\002\112"
#define STK_JNC         "\002\113"
#define STK_JNE         "\002\114"
#define STK_JNG         "\002\115"
#define STK_JNGE        "\002\116"
#define STK_JNL         "\002\117"
#define STK_JNLE        "\002\120"
#define STK_JNO         "\002\121"
#define STK_JNP         "\002\122"
#define STK_JNS         "\002\123"
#define STK_JNZ         "\002\124"
#define STK_JO          "\002\125"
#define STK_JP          "\002\126"
#define STK_JPE         "\002\127"
#define STK_JPO         "\002\130"
#define STK_JS          "\002\131"
#define STK_JZ          "\002\132"
#define STK_LABEL       "\002\133"
#define STK_LAHF        "\002\134"
#define STK_LAR         "\002\135"
#define STK_LDS         "\002\136"
#define STK_LE          "\002\137"
#define STK_LEA         "\002\140"
#define STK_LEAVE       "\002\141"
#define STK_LENGTH      "\002\142"
#define STK_LES         "\002\143"
#define STK_LGDT        "\002\144"
#define STK_LIDT        "\002\145"
#define STK_LLDT        "\002\146"
#define STK_LMSW        "\002\147"
#define STK_LOCK        "\002\150"
#define STK_LODS        "\002\151"
#define STK_LODSB       "\002\152"
#define STK_LODSW       "\002\153"
#define STK_LOOP        "\002\154"
#define STK_LOOPE       "\002\155"
#define STK_LOOPNE      "\002\156"
#define STK_LOOPNZ      "\002\157"
#define STK_LOOPZ       "\002\160"
#define STK_LOW         "\002\161"
#define STK_LSL         "\002\162"
#define STK_LT          "\002\163"
#define STK_MASK        "\002\164"
#define STK_MOD         "\002\165"
#define STK_MODRM       "\002\166"
#define STK_MOV         "\002\167"
#define STK_MOVS        "\002\170"
#define STK_MOVSB       "\002\171"
#define STK_MOVSW       "\002\172"
#define STK_MUL         "\002\173"
#define STK_NAME        "\002\174"
#define STK_NE          "\002\175"
#define STK_NEAR        "\002\176"
#define STK_NEG         "\002\177"
#define STK_NIL         "\003\001"
#define STK_NOP         "\003\002"
#define STK_NOSEGFIX    "\003\003"
#define STK_NOT         "\003\004"
#define STK_NOTHING     "\003\005"
#define STK_OFFSET      "\003\006"
#define STK_OR          "\003\007"
#define STK_ORG         "\003\010"
#define STK_OUT         "\003\011"
#define STK_OUTS        "\003\012"
#define STK_OUTSB       "\003\013"
#define STK_OUTSW       "\003\014"
#define STK_PREFIX66    "\003\015"
#define STK_PREFIX67    "\003\016"
#define STK_PREFX       "\003\017"
#define STK_POP         "\003\020"
#define STK_POPA        "\003\021"
#define STK_POPF        "\003\022"
#define STK_PROC        "\003\023"
#define STK_PROCLEN     "\003\024"
#define STK_PTR         "\003\025"
#define STK_PUBLIC      "\003\026"
#define STK_PUSH        "\003\027"
#define STK_PUSHA       "\003\030"
#define STK_PUSHF       "\003\031"
#define STK_QWORD       "\003\032"
#define STK_RCL         "\003\033"
#define STK_RCR         "\003\034"
#define STK_RECORD      "\003\035"
#define STK_RELB        "\003\036"
#define STK_RELD        "\003\037"
#define STK_RELW        "\003\040"
#define STK_REP         "\003\041"
#define STK_REPE        "\003\042"
#define STK_REPNE       "\003\043"
#define STK_REPNZ       "\003\044"
#define STK_RET         "\003\045"
#define STK_RO          "\003\046"
#define STK_ROL         "\003\047"
#define STK_ROR         "\003\050"
#define STK_RW          "\003\051"
#define STK_SAHF        "\003\052"
#define STK_SAL         "\003\053"
#define STK_SAR         "\003\054"
#define STK_SBB         "\003\055"
#define STK_SCAS        "\003\056"
#define STK_SCASB       "\003\057"
#define STK_SCASW       "\003\060"
#define STK_SEG         "\003\061"
#define STK_SEGMENT     "\003\062"
#define STK_SETBE       "\003\063"
#define STK_SGDT        "\003\064"
#define STK_SHL         "\003\065"
#define STK_SHORT       "\003\066"
#define STK_SHR         "\003\067"
#define STK_SI          "\003\070"
#define STK_SIDT        "\003\071"
#define STK_SIZE        "\003\072"
#define STK_SLDT        "\003\073"
#define STK_SMSW        "\003\074"
#define STK_SP          "\003\075"
#define STK_SS          "\003\076"
#define STK_ST          "\003\077"
#define STK_STACKSEG    "\003\100"
#define STK_STACKSTART  "\003\101"
#define STK_STC         "\003\102"
#define STK_STD         "\003\103"
#define STK_STI         "\003\104"
#define STK_STOS        "\003\105"
#define STK_STOSB       "\003\106"
#define STK_STOSW       "\003\107"
#define STK_STR         "\003\110"
#define STK_STRUC       "\003\111"
#define STK_SUB         "\003\112"
#define STK_TBYTE       "\003\113"
#define STK_TEST        "\003\114"
#define STK_THIS        "\003\115"
#define STK_TR3         "\003\116"
#define STK_TR4         "\003\117"
#define STK_TR5         "\003\120"
#define STK_TYPE        "\003\121"
#define STK_VERR        "\003\122"
#define STK_VERW        "\003\123"
#define STK_WAIT        "\003\124"
#define STK_WARNING     "\003\125"
#define STK_WBINVD      "\003\126"
#define STK_WC          "\003\127"
#define STK_WIDTH       "\003\130"
#define STK_WORD        "\003\131"
#define STK_XADD        "\003\132"
#define STK_XCHG        "\003\133"
#define STK_XLAT        "\003\134"
#define STK_XLATB       "\003\135"
#define STK_XOR         "\003\136"

/*
 * END OF AUTONUBER SECTION
 */

#define STK_INTEGERBIN  "\003\137"
#define STK_INTEGEROCT  "\003\140"
#define STK_INTEGERDEC  "\003\141"
#define STK_INTEGERHEX  "\003\142"
#define STK_DOUBLE      "\003\143"
#define STK_STRING      "\003\144"
#define STK_IDENTIFIER  "\003\145"

/*
 * Production string constants.
 */

/*
 * Number Operators
 */

#define SPD_GRP1_OP    "\101\001"
#define SPD_GRP2_OP    "\101\002"
#define SPD_GRP3_OP    "\101\003"
#define SPD_GRP4_OP    "\101\004"
#define SPD_GRP5_OP    "\101\005"
#define SPD_GRP6_OP    "\101\006"
#define SPD_GRP7_OP    "\101\007"
#define SPD_GRP8_OP    "\101\010"
#define SPD_GRP9_OP    "\101\011"
#define SPD_GRP10_OP   "\101\012"

/*
 * Variables and Constants
 */

#define SPD_CON_NUM    "\101\013"
#define SPD_CON_STR    "\101\014"

/*
 * Number Expressions
 */

#define SPD_GRP0_EXP   "\101\015"  /* EXECUTE */
#define SPD_GRP1_EXP   "\101\016"  /* EXECUTE */
#define SPD_GRP2_EXP   "\101\017"  /* EXECUTE */
#define SPD_GRP3_EXP   "\101\020"  /* EXECUTE */
#define SPD_GRP4_EXP   "\101\021"  /* EXECUTE */
#define SPD_GRP5_EXP   "\101\022"  /* EXECUTE */
#define SPD_GRP6_EXP   "\101\023"  /* EXECUTE */
#define SPD_GRP7_EXP   "\101\024"  /* EXECUTE */
#define SPD_GRP8_EXP   "\101\025"  /* EXECUTE */
#define SPD_GRP9_EXP   "\101\026"  /* EXECUTE */
#define SPD_NUM_EXP    "\101\027"  /* EXECUTE */

/*
 * String Expressions
 */

#define SPD_STRING     "\101\030"
#define SPD_STR_EXP    "\101\031"  /* EXECUTE */

/*
 * Statements
 */

#define SPD_STMT       "\101\032"   
#define SPD_AAA        "\101\033"
#define SPD_AAD        "\101\034"
#define SPD_AAM        "\101\035"
#define SPD_AAS        "\101\036"
#define SPD_REG16      "\101\037"
#define SPD_ADC        "\101\040"
#define SPD_REG8       "\101\041"
#define SPD_REGSEG     "\101\042"
#define SPD_ALUOP      "\101\043"
#define SPD_JMPOP      "\101\044"
#define SPD_JR         "\101\045"
#define SPD_DB         "\101\046"
#define SPD_SIMPLE     "\101\047"
#define SPD_rw         "\101\050"
#define SPD_rb         "\101\051"
#define SPD_IND_BX     "\101\052"
#define SPD_IND_BP     "\101\053"
#define SPD_IND_SI     "\101\054"
#define SPD_IND_DI     "\101\055"
#define SPD_rm         "\101\056"
#define SPD_eb         "\101\057"
#define SPD_ew         "\101\060"
#define SPD_RS         "\101\061"
#define SPD_db         "\101\062"
#define SPD_dw         "\101\063"
#define SPD_ALU        "\101\064"
#define SPD_VARIABLE   "\101\065"
#define SPD_VAR_TYPE   "\101\066"
#define SPD_INITLIST   "\101\067"
#define SPD_INITITEM   "\101\070"
#define SPD_DISP       "\101\071"
#define SPD_WARNING    "\101\072"
#define SPD_DBVARIABLE "\101\073"
#define SPD_DBLIST     "\101\074"
#define SPD_DBITEM     "\101\075"
#define SPD_DWITEM     "\101\076"
#define SPD_DWVARIABLE "\101\077"
#define SPD_DWLIST     "\101\100"
#define SPD_DDVARIABLE "\101\101"
#define SPD_DDLIST     "\101\102"
#define SPD_DDITEM     "\101\103"

#define SPD_LAST       "\177\177"

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

  TOK_AAA         =  21,
  TOK_AAD         =  22,
  TOK_AAM         =  23,
  TOK_AAS         =  24,
  TOK_ABS         =  25,
  TOK_ADC         =  26,
  TOK_ADD         =  27,
  TOK_AH          =  28,
  TOK_AL          =  29,
  TOK_ALIGN       =  30,
  TOK_AND         =  31,
  TOK_ARPL        =  32,
  TOK_ASSUME      =  33,
  TOK_AX          =  34,
  TOK_BH          =  35,
  TOK_BL          =  36,
  TOK_BOUND       =  37,
  TOK_BP          =  38,
  TOK_BSWAP       =  39,
  TOK_BX          =  40,
  TOK_BYTE        =  41,
  TOK_CALL        =  42,
  TOK_CBW         =  43,
  TOK_CH          =  44,
  TOK_CL          =  45,
  TOK_CLC         =  46,
  TOK_CLD         =  47,
  TOK_CLI         =  48,
  TOK_CLTS        =  49,
  TOK_CMC         =  50,
  TOK_CMP         =  51,
  TOK_CMPS        =  52,
  TOK_CMPSB       =  53,
  TOK_CMPSW       =  54,
  TOK_CMPXCHG     =  55,
  TOK_CODEMACRO   =  56,
  TOK_COMMON      =  57,
  TOK_CS          =  58,
  TOK_CWD         =  59,
  TOK_CX          =  60,
  TOK_DAA         =  61,
  TOK_DAS         =  62,
  TOK_DB          =  63,
  TOK_DD          =  64,
  TOK_DEC         =  65,
  TOK_DH          =  66,
  TOK_DI          =  67,
  TOK_DIV         =  68,
  TOK_DL          =  69,
  TOK_DQ          =  70,
  TOK_DS          =  71,
  TOK_DT          =  72,
  TOK_DUP         =  73,
  TOK_DW          =  74,
  TOK_DWORD       =  75,
  TOK_DX          =  76,
  TOK_END         =  77,
  TOK_ENDM        =  78,
  TOK_ENDP        =  79,
  TOK_ENDS        =  80,
  TOK_ENTER       =  81,
  TOK_EO          =  82,
  TOK_EQ          =  83,
  TOK_EQU         =  84,
  TOK_ER          =  85,
  TOK_ES          =  86,
  TOK_ESC         =  87,
  TOK_EVEN        =  88,
  TOK_EXTRN       =  89,
  TOK_F2XM1       =  90,
  TOK_FABS        =  91,
  TOK_FADD        =  92,
  TOK_FADDP       =  93,
  TOK_FAR         =  94,
  TOK_FBLD        =  95,
  TOK_FBSTP       =  96,
  TOK_FCHS        =  97,
  TOK_FCLEX       =  98,
  TOK_FCOM        =  99,
  TOK_FCOMP       = 100,
  TOK_FCOMPP      = 101,
  TOK_FDECSTP     = 102,
  TOK_FDISI       = 103,
  TOK_FDIV        = 104,
  TOK_FDIVP       = 105,
  TOK_FDIVR       = 106,
  TOK_FDIVRP      = 107,
  TOK_FENI        = 108,
  TOK_FFREE       = 109,
  TOK_FIADD       = 110,
  TOK_FICOM       = 111,
  TOK_FICOMP      = 112,
  TOK_FIDIV       = 113,
  TOK_FIDIVR      = 114,
  TOK_FILD        = 115,
  TOK_FIMUL       = 116,
  TOK_FINCSTP     = 117,
  TOK_FINIT       = 118,
  TOK_FIST        = 119,
  TOK_FISTP       = 120,
  TOK_FISUB       = 121,
  TOK_FISUBR      = 122,
  TOK_FLD         = 123,
  TOK_FLD1        = 124,
  TOK_FLDCW       = 125,
  TOK_FLDENV      = 126,
  TOK_FLDL2E      = 127,
  TOK_FLDL2T      = 128,
  TOK_FLDLG2      = 129,
  TOK_FLDLN2      = 130,
  TOK_FLDPI       = 131,
  TOK_FLDZ        = 132,
  TOK_FMUL        = 133,
  TOK_FMULP       = 134,
  TOK_FNCLEX      = 135,
  TOK_FNDISI      = 136,
  TOK_FNENI       = 137,
  TOK_FNINIT      = 138,
  TOK_FNOP        = 139,
  TOK_FNSAVE      = 140,
  TOK_FNSTCW      = 141,
  TOK_FNSTENV     = 142,
  TOK_FNSTSW      = 143,
  TOK_FPATAN      = 144,
  TOK_FPREM       = 145,
  TOK_FPTAN       = 146,
  TOK_FRNDINT     = 147,
  TOK_FRSTOR      = 148,
  TOK_FSAVE       = 149,
  TOK_FSCALE      = 150,
  TOK_FSETPM      = 151,
  TOK_FSQRT       = 152,
  TOK_FST         = 153,
  TOK_FSTCW       = 154,
  TOK_FSTENV      = 155,
  TOK_FSTP        = 156,
  TOK_FSTSW       = 157,
  TOK_FSUB        = 158,
  TOK_FSUBP       = 159,
  TOK_FSUBR       = 160,
  TOK_FSUBRP      = 161,
  TOK_FTST        = 162,
  TOK_FWAIT       = 163,
  TOK_FXAM        = 164,
  TOK_FXCH        = 165,
  TOK_FXTRACT     = 166,
  TOK_FYL2X       = 167,
  TOK_FYL2XP1     = 168,
  TOK_GE          = 169,
  TOK_GT          = 170,
  TOK_HIGH        = 171,
  TOK_HLT         = 172,
  TOK_IDIV        = 173,
  TOK_IMUL        = 174,
  TOK_IN          = 175,
  TOK_INC         = 176,
  TOK_INS         = 177,
  TOK_INSB        = 178,
  TOK_INSW        = 179,
  TOK_INT         = 180,
  TOK_INTO        = 181,
  TOK_INVD        = 182,
  TOK_INVLPG      = 183,
  TOK_IRET        = 184,
  TOK_JA          = 185,
  TOK_JAE         = 186,
  TOK_JB          = 187,
  TOK_JBE         = 188,
  TOK_JC          = 189,
  TOK_JCXZ        = 190,
  TOK_JE          = 191,
  TOK_JG          = 192,
  TOK_JGE         = 193,
  TOK_JL          = 194,
  TOK_JLE         = 195,
  TOK_JMP         = 196,
  TOK_JNA         = 197,
  TOK_JNAE        = 198,
  TOK_JNB         = 199,
  TOK_JNBE        = 200,
  TOK_JNC         = 201,
  TOK_JNE         = 202,
  TOK_JNG         = 203,
  TOK_JNGE        = 204,
  TOK_JNL         = 205,
  TOK_JNLE        = 206,
  TOK_JNO         = 207,
  TOK_JNP         = 208,
  TOK_JNS         = 209,
  TOK_JNZ         = 210,
  TOK_JO          = 211,
  TOK_JP          = 212,
  TOK_JPE         = 213,
  TOK_JPO         = 214,
  TOK_JS          = 215,
  TOK_JZ          = 216,
  TOK_LABEL       = 217,
  TOK_LAHF        = 218,
  TOK_LAR         = 219,
  TOK_LDS         = 220,
  TOK_LE          = 221,
  TOK_LEA         = 222,
  TOK_LEAVE       = 223,
  TOK_LENGTH      = 224,
  TOK_LES         = 225,
  TOK_LGDT        = 226,
  TOK_LIDT        = 227,
  TOK_LLDT        = 228,
  TOK_LMSW        = 229,
  TOK_LOCK        = 230,
  TOK_LODS        = 231,
  TOK_LODSB       = 232,
  TOK_LODSW       = 233,
  TOK_LOOP        = 234,
  TOK_LOOPE       = 235,
  TOK_LOOPNE      = 236,
  TOK_LOOPNZ      = 237,
  TOK_LOOPZ       = 238,
  TOK_LOW         = 239,
  TOK_LSL         = 240,
  TOK_LT          = 241,
  TOK_MASK        = 242,
  TOK_MOD         = 243,
  TOK_MODRM       = 244,
  TOK_MOV         = 245,
  TOK_MOVS        = 246,
  TOK_MOVSB       = 247,
  TOK_MOVSW       = 248,
  TOK_MUL         = 249,
  TOK_NAME        = 250,
  TOK_NE          = 251,
  TOK_NEAR        = 252,
  TOK_NEG         = 253,
  TOK_NIL         = 254,
  TOK_NOP         = 255,
  TOK_NOSEGFIX    = 256,
  TOK_NOT         = 257,
  TOK_NOTHING     = 258,
  TOK_OFFSET      = 259,
  TOK_OR          = 260,
  TOK_ORG         = 261,
  TOK_OUT         = 262,
  TOK_OUTS        = 263,
  TOK_OUTSB       = 264,
  TOK_OUTSW       = 265,
  TOK_PREFIX66    = 266,
  TOK_PREFIX67    = 267,
  TOK_PREFX       = 268,
  TOK_POP         = 269,
  TOK_POPA        = 270,
  TOK_POPF        = 271,
  TOK_PROC        = 272,
  TOK_PROCLEN     = 273,
  TOK_PTR         = 274,
  TOK_PUBLIC      = 275,
  TOK_PUSH        = 276,
  TOK_PUSHA       = 277,
  TOK_PUSHF       = 278,
  TOK_QWORD       = 279,
  TOK_RCL         = 280,
  TOK_RCR         = 281,
  TOK_RECORD      = 282,
  TOK_RELB        = 283,
  TOK_RELD        = 284,
  TOK_RELW        = 285,
  TOK_REP         = 286,
  TOK_REPE        = 287,
  TOK_REPNE       = 288,
  TOK_REPNZ       = 289,
  TOK_RET         = 290,
  TOK_RO          = 291,
  TOK_ROL         = 292,
  TOK_ROR         = 293,
  TOK_RW          = 294,
  TOK_SAHF        = 295,
  TOK_SAL         = 296,
  TOK_SAR         = 297,
  TOK_SBB         = 298,
  TOK_SCAS        = 299,
  TOK_SCASB       = 300,
  TOK_SCASW       = 301,
  TOK_SEG         = 302,
  TOK_SEGMENT     = 303,
  TOK_SETBE       = 304,
  TOK_SGDT        = 305,
  TOK_SHL         = 306,
  TOK_SHORT       = 307,
  TOK_SHR         = 308,
  TOK_SI          = 309,
  TOK_SIDT        = 310,
  TOK_SIZE        = 311,
  TOK_SLDT        = 312,
  TOK_SMSW        = 313,
  TOK_SP          = 314,
  TOK_SS          = 315,
  TOK_ST          = 316,
  TOK_STACKSEG    = 317,
  TOK_STACKSTART  = 318,
  TOK_STC         = 319,
  TOK_STD         = 320,
  TOK_STI         = 321,
  TOK_STOS        = 322,
  TOK_STOSB       = 323,
  TOK_STOSW       = 324,
  TOK_STR         = 325,
  TOK_STRUC       = 326,
  TOK_SUB         = 327,
  TOK_TBYTE       = 328,
  TOK_TEST        = 329,
  TOK_THIS        = 330,
  TOK_TR3         = 331,
  TOK_TR4         = 332,
  TOK_TR5         = 333,
  TOK_TYPE        = 334,
  TOK_VERR        = 335,
  TOK_VERW        = 336,
  TOK_WAIT        = 337,
  TOK_WARNING     = 338,
  TOK_WBINVD      = 339,
  TOK_WC          = 340,
  TOK_WIDTH       = 341,
  TOK_WORD        = 342,
  TOK_XADD        = 343,
  TOK_XCHG        = 344,
  TOK_XLAT        = 345,
  TOK_XLATB       = 346,
  TOK_XOR         = 347,

  TOK_INTEGERBIN  = 348,
  TOK_INTEGEROCT  = 349,
  TOK_INTEGERDEC  = 350,
  TOK_INTEGERHEX  = 351,
  TOK_DOUBLE      = 352,
  TOK_STRING      = 353,
  TOK_IDENTIFIER  = 354,

/*
 * Productions
 */

  PRD_STMT       = 8153,
  PRD_SIMPLE     = 8166,
  PRD_WARNING    = 8185,
  PRD_CON_NUM    = 8138,
  PRD_GRP0_EXP   = 8140,
  PRD_GRP1_EXP   = 8141,
  PRD_GRP2_EXP   = 8142,
  PRD_GRP3_EXP   = 8143,
  PRD_GRP4_EXP   = 8144,
  PRD_GRP5_EXP   = 8145,
  PRD_GRP6_EXP   = 8146,
  PRD_GRP7_EXP   = 8147,
  PRD_GRP8_EXP   = 8148,
  PRD_GRP9_EXP   = 8149,
  PRD_VARIABLE   = 8180,
  PRD_INITITEM   = 8183,
  PRD_DBVARIABLE = 8186,
  PRD_DBLIST     = 8187,
  PRD_DBITEM     = 8188,
  PRD_DWITEM     = 8189,
  PRD_DWVARIABLE = 8190,
  PRD_DWLIST     = 8191,
  PRD_DDVARIABLE = 8192,
  PRD_DDLIST     = 8193,
  PRD_DDITEM     = 8194,


//  PRD_REG16       = 8158,
//  PRD_ADC         = 8159,
//  PRD_REG8        = 8160,
//  PRD_REGSEG      = 8161,
//  PRD_ALUOP       = 8162,
//  PRD_JMPOP       = 8163,
//  PRD_JR          = 8164,
//  PRD_DB          = 8165,

  /*
  PRD_GRP1_OP     = 8128,
  PRD_GRP2_OP     = 8129,
  PRD_GRP3_OP     = 8130,
  PRD_GRP4_OP     = 8131,
  PRD_GRP5_OP     = 8132,
  PRD_GRP6_OP     = 8133,
  PRD_GRP7_OP     = 8134,
  PRD_GRP8_OP     = 8135,
  PRD_GRP9_OP     = 8136,
  PRD_GRP10_OP    = 8137,
  PRD_CON_NUM     = 8138,
  PRD_CON_STR     = 8139,
  PRD_GRP0_EXP    = 8140,
  PRD_GRP1_EXP    = 8141,
  PRD_GRP2_EXP    = 8142,
  PRD_GRP3_EXP    = 8143,
  PRD_GRP4_EXP    = 8144,
  PRD_GRP5_EXP    = 8145,
  PRD_GRP6_EXP    = 8146,
  PRD_GRP7_EXP    = 8147,
  PRD_GRP8_EXP    = 8148,
  PRD_GRP9_EXP    = 8149,
  PRD_NUM_EXP     = 8150,
  PRD_STRING      = 8151,
  PRD_STR_EXP     = 8152,
*/
  
  PRD_LAST        = 16128,

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
 * Helper Macros.
 */

#define CPYVALUE(TGT, SRC) memcpy( &TGT, &SRC, sizeof( value_t ) )

/*
 * Prototypes.
 */

int process(const char *) ;
int assemble(const char *, int ) ;
tlist_node_t * tokenize(const char *, int) ;
void delete_tlist( tlist_node_t ** ) ;
void dump_pattern( void ) ;
void error( int, int ) ;
ptree_node_t * match( int, tlist_node_t **, int ) ;
void delete_ptree( ptree_node_t *, int, int, int ) ;
int execute ( ptree_node_t * ) ;
unsigned int pid( const char * ) ;
unsigned char to_byte( char ) ;
void dep(unsigned char) ;
void depw(unsigned short) ;
void depd(unsigned int) ;
int setstr(char **, char *) ;

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
