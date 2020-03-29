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
 *    2019 November 17 Paul Willmott Error codes added.
 *
 *  Revision History:
 *
 *    2019 November 10 Paul Willmott Production string constants added.
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
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
} ;

#define NUM_ERR ( 6 )

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

#define NUM_PATTERN ( 384 )  /* Number of token patterns */

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
#define STK_BIT         "\001\045"
#define STK_BITOFFSET   "\001\046"
#define STK_BL          "\001\047"
#define STK_BOUND       "\001\050"
#define STK_BP          "\001\051"
#define STK_BSF         "\001\052"
#define STK_BSR         "\001\053"
#define STK_BSWAP       "\001\054"
#define STK_BT          "\001\055"
#define STK_BTC         "\001\056"
#define STK_BTR         "\001\057"
#define STK_BTS         "\001\060"
#define STK_BX          "\001\061"
#define STK_BYTE        "\001\062"
#define STK_CALL        "\001\063"
#define STK_CBW         "\001\064"
#define STK_CDQ         "\001\065"
#define STK_CH          "\001\066"
#define STK_CL          "\001\067"
#define STK_CLC         "\001\070"
#define STK_CLD         "\001\071"
#define STK_CLI         "\001\072"
#define STK_CLTS        "\001\073"
#define STK_CMC         "\001\074"
#define STK_CMP         "\001\075"
#define STK_CMPS        "\001\076"
#define STK_CMPXCHG     "\001\077"
#define STK_COMM        "\001\100"
#define STK_COMMON      "\001\101"
#define STK_CR0         "\001\102"
#define STK_CR1         "\001\103"
#define STK_CR2         "\001\104"
#define STK_CS          "\001\105"
#define STK_CWD         "\001\106"
#define STK_CWDE        "\001\107"
#define STK_CX          "\001\110"
#define STK_DAA         "\001\111"
#define STK_DAS         "\001\112"
#define STK_DB          "\001\113"
#define STK_DBIT        "\001\114"
#define STK_DD          "\001\115"
#define STK_DEC         "\001\116"
#define STK_DH          "\001\117"
#define STK_DI          "\001\120"
#define STK_DIV         "\001\121"
#define STK_DL          "\001\122"
#define STK_DP          "\001\123"
#define STK_DQ          "\001\124"
#define STK_DR0         "\001\125"
#define STK_DR1         "\001\126"
#define STK_DR2         "\001\127"
#define STK_DR3         "\001\130"
#define STK_DR6         "\001\131"
#define STK_DR7         "\001\132"
#define STK_DS          "\001\133"
#define STK_DT          "\001\134"
#define STK_DUP         "\001\135"
#define STK_DW          "\001\136"
#define STK_DWORD       "\001\137"
#define STK_DX          "\001\140"
#define STK_EAX         "\001\141"
#define STK_EBP         "\001\142"
#define STK_EBX         "\001\143"
#define STK_ECX         "\001\144"
#define STK_EDI         "\001\145"
#define STK_EDX         "\001\146"
#define STK_END         "\001\147"
#define STK_ENDP        "\001\150"
#define STK_ENDS        "\001\151"
#define STK_ENTER       "\001\152"
#define STK_EO          "\001\153"
#define STK_EQ          "\001\154"
#define STK_EQU         "\001\155"
#define STK_ER          "\001\156"
#define STK_ES          "\001\157"
#define STK_ESC         "\001\160"
#define STK_ESI         "\001\161"
#define STK_ESP         "\001\162"
#define STK_EVEN        "\001\163"
#define STK_EXTRN       "\001\164"
#define STK_F2XM1       "\001\165"
#define STK_FABS        "\001\166"
#define STK_FADD        "\001\167"
#define STK_FADDP       "\001\170"
#define STK_FAR         "\001\171"
#define STK_FBLD        "\001\172"
#define STK_FBSTP       "\001\173"
#define STK_FCHS        "\001\174"
#define STK_FCLEX       "\001\175"
#define STK_FCOM        "\001\176"
#define STK_FCOMP       "\001\177"
#define STK_FCOMPP      "\002\001"
#define STK_FCOS        "\002\002"
#define STK_FDECSTP     "\002\003"
#define STK_FDISI       "\002\004"
#define STK_FDIV        "\002\005"
#define STK_FDIVP       "\002\006"
#define STK_FDIVR       "\002\007"
#define STK_FDIVRP      "\002\010"
#define STK_FENI        "\002\011"
#define STK_FFREE       "\002\012"
#define STK_FIADD       "\002\013"
#define STK_FICOM       "\002\014"
#define STK_FICOMP      "\002\015"
#define STK_FIDIV       "\002\016"
#define STK_FIDIVR      "\002\017"
#define STK_FILD        "\002\020"
#define STK_FIMUL       "\002\021"
#define STK_FINCSTP     "\002\022"
#define STK_FINIT       "\002\023"
#define STK_FIST        "\002\024"
#define STK_FISTP       "\002\025"
#define STK_FISUB       "\002\026"
#define STK_FISUBR      "\002\027"
#define STK_FLD         "\002\030"
#define STK_FLD1        "\002\031"
#define STK_FLDCW       "\002\032"
#define STK_FLDENV      "\002\033"
#define STK_FLDL2E      "\002\034"
#define STK_FLDL2T      "\002\035"
#define STK_FLDLG2      "\002\036"
#define STK_FLDLN2      "\002\037"
#define STK_FLDPI       "\002\040"
#define STK_FLDZ        "\002\041"
#define STK_FMUL        "\002\042"
#define STK_FMULP       "\002\043"
#define STK_FNCLEX      "\002\044"
#define STK_FNDISI      "\002\045"
#define STK_FNENI       "\002\046"
#define STK_FNINIT      "\002\047"
#define STK_FNOP        "\002\050"
#define STK_FNSAVE      "\002\051"
#define STK_FNSTCW      "\002\052"
#define STK_FNSTENV     "\002\053"
#define STK_FNSTSW      "\002\054"
#define STK_FPATAN      "\002\055"
#define STK_FPREM       "\002\056"
#define STK_FPREM1FPTAN "\002\057"
#define STK_FRNDINT     "\002\060"
#define STK_FRSTOR      "\002\061"
#define STK_FS          "\002\062"
#define STK_FSAVE       "\002\063"
#define STK_FSCALE      "\002\064"
#define STK_FSETPM      "\002\065"
#define STK_FSIN        "\002\066"
#define STK_FSINCOS     "\002\067"
#define STK_FSQRT       "\002\070"
#define STK_FST         "\002\071"
#define STK_FSTCW       "\002\072"
#define STK_FSTENV      "\002\073"
#define STK_FSTP        "\002\074"
#define STK_FSTSW       "\002\075"
#define STK_FSUB        "\002\076"
#define STK_FSUBP       "\002\077"
#define STK_FSUBR       "\002\100"
#define STK_FSUBRP      "\002\101"
#define STK_FTST        "\002\102"
#define STK_FUCOM       "\002\103"
#define STK_FUCOMP      "\002\104"
#define STK_FUCOMPP     "\002\105"
#define STK_FWAIT       "\002\106"
#define STK_FXAM        "\002\107"
#define STK_FXCH        "\002\110"
#define STK_FXTRACT     "\002\111"
#define STK_FYL2X       "\002\112"
#define STK_FYL2XP1     "\002\113"
#define STK_GE          "\002\114"
#define STK_GS          "\002\115"
#define STK_GT          "\002\116"
#define STK_HIGH        "\002\117"
#define STK_HIGHW       "\002\120"
#define STK_HLT         "\002\121"
#define STK_IDIV        "\002\122"
#define STK_IMUL        "\002\123"
#define STK_IN          "\002\124"
#define STK_INC         "\002\125"
#define STK_INS         "\002\126"
#define STK_INT         "\002\127"
#define STK_INTO        "\002\130"
#define STK_INVD        "\002\131"
#define STK_INVLPG      "\002\132"
#define STK_IRET        "\002\133"
#define STK_JA          "\002\134"
#define STK_JAE         "\002\135"
#define STK_JB          "\002\136"
#define STK_JBE         "\002\137"
#define STK_JC          "\002\140"
#define STK_JCXZ        "\002\141"
#define STK_JE          "\002\142"
#define STK_JECXZ       "\002\143"
#define STK_JG          "\002\144"
#define STK_JGE         "\002\145"
#define STK_JL          "\002\146"
#define STK_JLE         "\002\147"
#define STK_JMP         "\002\150"
#define STK_JNA         "\002\151"
#define STK_JNAE        "\002\152"
#define STK_JNB         "\002\153"
#define STK_JNBE        "\002\154"
#define STK_JNC         "\002\155"
#define STK_JNE         "\002\156"
#define STK_JNG         "\002\157"
#define STK_JNGE        "\002\160"
#define STK_JNL         "\002\161"
#define STK_JNLE        "\002\162"
#define STK_JNO         "\002\163"
#define STK_JNP         "\002\164"
#define STK_JNS         "\002\165"
#define STK_JNZ         "\002\166"
#define STK_JO          "\002\167"
#define STK_JP          "\002\170"
#define STK_JPE         "\002\171"
#define STK_JPO         "\002\172"
#define STK_JS          "\002\173"
#define STK_JZ          "\002\174"
#define STK_LABEL       "\002\175"
#define STK_LAHF        "\002\176"
#define STK_LAR         "\002\177"
#define STK_LDS         "\003\001"
#define STK_LE          "\003\002"
#define STK_LEA         "\003\003"
#define STK_LEAVE       "\003\004"
#define STK_LENGTH      "\003\005"
#define STK_LES         "\003\006"
#define STK_LFS         "\003\007"
#define STK_LGDT        "\003\010"
#define STK_LGS         "\003\011"
#define STK_LIDT        "\003\012"
#define STK_LLDT        "\003\013"
#define STK_LMSW        "\003\014"
#define STK_LOCK        "\003\015"
#define STK_LODS        "\003\016"
#define STK_LOOP        "\003\017"
#define STK_LOOPD       "\003\020"
#define STK_LOOPE       "\003\021"
#define STK_LOOPED      "\003\022"
#define STK_LOOPNE      "\003\023"
#define STK_LOOPNED     "\003\024"
#define STK_LOW         "\003\025"
#define STK_LOWW        "\003\026"
#define STK_LSL         "\003\027"
#define STK_LSS         "\003\030"
#define STK_LT          "\003\031"
#define STK_LTR         "\003\032"
#define STK_MASK        "\003\033"
#define STK_MOD         "\003\034"
#define STK_MOV         "\003\035"
#define STK_MOVS        "\003\036"
#define STK_MOVSX       "\003\037"
#define STK_MOVZX       "\003\040"
#define STK_MUL         "\003\041"
#define STK_NAME        "\003\042"
#define STK_NE          "\003\043"
#define STK_NEAR        "\003\044"
#define STK_NEG         "\003\045"
#define STK_NOP         "\003\046"
#define STK_NOT         "\003\047"
#define STK_NOTHING     "\003\050"
#define STK_OFFSET      "\003\051"
#define STK_OR          "\003\052"
#define STK_ORG         "\003\053"
#define STK_OUT         "\003\054"
#define STK_OUTS        "\003\055"
#define STK_POP         "\003\056"
#define STK_POPA        "\003\057"
#define STK_POPAD       "\003\060"
#define STK_POPF        "\003\061"
#define STK_POPFD       "\003\062"
#define STK_PROC        "\003\063"
#define STK_PROCLEN     "\003\064"
#define STK_PTR         "\003\065"
#define STK_PUBLIC      "\003\066"
#define STK_PURGE       "\003\067"
#define STK_PUSH        "\003\070"
#define STK_PUSHA       "\003\071"
#define STK_PUSHAD      "\003\072"
#define STK_PUSHF       "\003\073"
#define STK_PUSHFD      "\003\074"
#define STK_PWORD       "\003\075"
#define STK_QWORD       "\003\076"
#define STK_RCL         "\003\077"
#define STK_RCR         "\003\100"
#define STK_RECORD      "\003\101"
#define STK_REPE        "\003\102"
#define STK_REPNE       "\003\103"
#define STK_RET         "\003\104"
#define STK_RO          "\003\105"
#define STK_ROL         "\003\106"
#define STK_ROR         "\003\107"
#define STK_RW          "\003\110"
#define STK_SAHF        "\003\111"
#define STK_SAL         "\003\112"
#define STK_SAR         "\003\113"
#define STK_SCAS        "\003\114"
#define STK_SEG         "\003\115"
#define STK_SEGMENT     "\003\116"
#define STK_SET         "\003\117"
#define STK_SGDT        "\003\120"
#define STK_SHL         "\003\121"
#define STK_SHLD        "\003\122"
#define STK_SHORT       "\003\123"
#define STK_SHR         "\003\124"
#define STK_SHRD        "\003\125"
#define STK_SI          "\003\126"
#define STK_SIDT        "\003\127"
#define STK_SIZE        "\003\130"
#define STK_SLDT        "\003\131"
#define STK_SMSW        "\003\132"
#define STK_SP          "\003\133"
#define STK_SS          "\003\134"
#define STK_STACKSEG    "\003\135"
#define STK_STACKSTART  "\003\136"
#define STK_STC         "\003\137"
#define STK_STD         "\003\140"
#define STK_STI         "\003\141"
#define STK_STOS        "\003\142"
#define STK_STR         "\003\143"
#define STK_STRUC       "\003\144"
#define STK_SUB         "\003\145"
#define STK_TBYTE       "\003\146"
#define STK_TEST        "\003\147"
#define STK_THIS        "\003\150"
#define STK_TR3         "\003\151"
#define STK_TR4         "\003\152"
#define STK_TR5         "\003\153"
#define STK_TR6         "\003\154"
#define STK_TR7         "\003\155"
#define STK_TYPE        "\003\156"
#define STK_USE16       "\003\157"
#define STK_USE32       "\003\160"
#define STK_VERR        "\003\161"
#define STK_VERW        "\003\162"
#define STK_WAIT        "\003\163"
#define STK_WBINVD      "\003\164"
#define STK_WIDTH       "\003\165"
#define STK_WORD        "\003\166"
#define STK_XADD        "\003\167"
#define STK_XCHG        "\003\170"
#define STK_XLAT        "\003\171"
#define STK_XOR         "\003\172"
#define STK_SBB         "\003\173"

/*
 * END OF AUTONUBER SECTION
 */

#define STK_INTEGERBIN  "\003\174"
#define STK_INTEGEROCT  "\003\175"
#define STK_INTEGERDEC  "\003\176"
#define STK_INTEGERHEX  "\003\177"
#define STK_DOUBLE      "\004\001"
#define STK_STRING      "\004\002"
#define STK_IDENTIFIER  "\004\003"

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
  TOK_BIT         =  36,
  TOK_BITOFFSET   =  37,
  TOK_BL          =  38,
  TOK_BOUND       =  39,
  TOK_BP          =  40,
  TOK_BSF         =  41,
  TOK_BSR         =  42,
  TOK_BSWAP       =  43,
  TOK_BT          =  44,
  TOK_BTC         =  45,
  TOK_BTR         =  46,
  TOK_BTS         =  47,
  TOK_BX          =  48,
  TOK_BYTE        =  49,
  TOK_CALL        =  50,
  TOK_CBW         =  51,
  TOK_CDQ         =  52,
  TOK_CH          =  53,
  TOK_CL          =  54,
  TOK_CLC         =  55,
  TOK_CLD         =  56,
  TOK_CLI         =  57,
  TOK_CLTS        =  58,
  TOK_CMC         =  59,
  TOK_CMP         =  60,
  TOK_CMPS        =  61,
  TOK_CMPXCHG     =  62,
  TOK_COMM        =  63,
  TOK_COMMON      =  64,
  TOK_CR0         =  65,
  TOK_CR1         =  66,
  TOK_CR2         =  67,
  TOK_CS          =  68,
  TOK_CWD         =  69,
  TOK_CWDE        =  70,
  TOK_CX          =  71,
  TOK_DAA         =  72,
  TOK_DAS         =  73,
  TOK_DB          =  74,
  TOK_DBIT        =  75,
  TOK_DD          =  76,
  TOK_DEC         =  77,
  TOK_DH          =  78,
  TOK_DI          =  79,
  TOK_DIV         =  80,
  TOK_DL          =  81,
  TOK_DP          =  82,
  TOK_DQ          =  83,
  TOK_DR0         =  84,
  TOK_DR1         =  85,
  TOK_DR2         =  86,
  TOK_DR3         =  87,
  TOK_DR6         =  88,
  TOK_DR7         =  89,
  TOK_DS          =  90,
  TOK_DT          =  91,
  TOK_DUP         =  92,
  TOK_DW          =  93,
  TOK_DWORD       =  94,
  TOK_DX          =  95,
  TOK_EAX         =  96,
  TOK_EBP         =  97,
  TOK_EBX         =  98,
  TOK_ECX         =  99,
  TOK_EDI         = 100,
  TOK_EDX         = 101,
  TOK_END         = 102,
  TOK_ENDP        = 103,
  TOK_ENDS        = 104,
  TOK_ENTER       = 105,
  TOK_EO          = 106,
  TOK_EQ          = 107,
  TOK_EQU         = 108,
  TOK_ER          = 109,
  TOK_ES          = 110,
  TOK_ESC         = 111,
  TOK_ESI         = 112,
  TOK_ESP         = 113,
  TOK_EVEN        = 114,
  TOK_EXTRN       = 115,
  TOK_F2XM1       = 116,
  TOK_FABS        = 117,
  TOK_FADD        = 118,
  TOK_FADDP       = 119,
  TOK_FAR         = 120,
  TOK_FBLD        = 121,
  TOK_FBSTP       = 122,
  TOK_FCHS        = 123,
  TOK_FCLEX       = 124,
  TOK_FCOM        = 125,
  TOK_FCOMP       = 126,
  TOK_FCOMPP      = 127,
  TOK_FCOS        = 128,
  TOK_FDECSTP     = 129,
  TOK_FDISI       = 130,
  TOK_FDIV        = 131,
  TOK_FDIVP       = 132,
  TOK_FDIVR       = 133,
  TOK_FDIVRP      = 134,
  TOK_FENI        = 135,
  TOK_FFREE       = 136,
  TOK_FIADD       = 137,
  TOK_FICOM       = 138,
  TOK_FICOMP      = 139,
  TOK_FIDIV       = 140,
  TOK_FIDIVR      = 141,
  TOK_FILD        = 142,
  TOK_FIMUL       = 143,
  TOK_FINCSTP     = 144,
  TOK_FINIT       = 145,
  TOK_FIST        = 146,
  TOK_FISTP       = 147,
  TOK_FISUB       = 148,
  TOK_FISUBR      = 149,
  TOK_FLD         = 150,
  TOK_FLD1        = 151,
  TOK_FLDCW       = 152,
  TOK_FLDENV      = 153,
  TOK_FLDL2E      = 154,
  TOK_FLDL2T      = 155,
  TOK_FLDLG2      = 156,
  TOK_FLDLN2      = 157,
  TOK_FLDPI       = 158,
  TOK_FLDZ        = 159,
  TOK_FMUL        = 160,
  TOK_FMULP       = 161,
  TOK_FNCLEX      = 162,
  TOK_FNDISI      = 163,
  TOK_FNENI       = 164,
  TOK_FNINIT      = 165,
  TOK_FNOP        = 166,
  TOK_FNSAVE      = 167,
  TOK_FNSTCW      = 168,
  TOK_FNSTENV     = 169,
  TOK_FNSTSW      = 170,
  TOK_FPATAN      = 171,
  TOK_FPREM       = 172,
  TOK_FPREM1FPTAN = 173,
  TOK_FRNDINT     = 174,
  TOK_FRSTOR      = 175,
  TOK_FS          = 176,
  TOK_FSAVE       = 177,
  TOK_FSCALE      = 178,
  TOK_FSETPM      = 179,
  TOK_FSIN        = 180,
  TOK_FSINCOS     = 181,
  TOK_FSQRT       = 182,
  TOK_FST         = 183,
  TOK_FSTCW       = 184,
  TOK_FSTENV      = 185,
  TOK_FSTP        = 186,
  TOK_FSTSW       = 187,
  TOK_FSUB        = 188,
  TOK_FSUBP       = 189,
  TOK_FSUBR       = 190,
  TOK_FSUBRP      = 191,
  TOK_FTST        = 192,
  TOK_FUCOM       = 193,
  TOK_FUCOMP      = 194,
  TOK_FUCOMPP     = 195,
  TOK_FWAIT       = 196,
  TOK_FXAM        = 197,
  TOK_FXCH        = 198,
  TOK_FXTRACT     = 199,
  TOK_FYL2X       = 200,
  TOK_FYL2XP1     = 201,
  TOK_GE          = 202,
  TOK_GS          = 203,
  TOK_GT          = 204,
  TOK_HIGH        = 205,
  TOK_HIGHW       = 206,
  TOK_HLT         = 207,
  TOK_IDIV        = 208,
  TOK_IMUL        = 209,
  TOK_IN          = 210,
  TOK_INC         = 211,
  TOK_INS         = 212,
  TOK_INT         = 213,
  TOK_INTO        = 214,
  TOK_INVD        = 215,
  TOK_INVLPG      = 216,
  TOK_IRET        = 217,
  TOK_JA          = 218,
  TOK_JAE         = 219,
  TOK_JB          = 220,
  TOK_JBE         = 221,
  TOK_JC          = 222,
  TOK_JCXZ        = 223,
  TOK_JE          = 224,
  TOK_JECXZ       = 225,
  TOK_JG          = 226,
  TOK_JGE         = 227,
  TOK_JL          = 228,
  TOK_JLE         = 229,
  TOK_JMP         = 230,
  TOK_JNA         = 231,
  TOK_JNAE        = 232,
  TOK_JNB         = 233,
  TOK_JNBE        = 234,
  TOK_JNC         = 235,
  TOK_JNE         = 236,
  TOK_JNG         = 237,
  TOK_JNGE        = 238,
  TOK_JNL         = 239,
  TOK_JNLE        = 240,
  TOK_JNO         = 241,
  TOK_JNP         = 242,
  TOK_JNS         = 243,
  TOK_JNZ         = 244,
  TOK_JO          = 245,
  TOK_JP          = 246,
  TOK_JPE         = 247,
  TOK_JPO         = 248,
  TOK_JS          = 249,
  TOK_JZ          = 250,
  TOK_LABEL       = 251,
  TOK_LAHF        = 252,
  TOK_LAR         = 253,
  TOK_LDS         = 254,
  TOK_LE          = 255,
  TOK_LEA         = 256,
  TOK_LEAVE       = 257,
  TOK_LENGTH      = 258,
  TOK_LES         = 259,
  TOK_LFS         = 260,
  TOK_LGDT        = 261,
  TOK_LGS         = 262,
  TOK_LIDT        = 263,
  TOK_LLDT        = 264,
  TOK_LMSW        = 265,
  TOK_LOCK        = 266,
  TOK_LODS        = 267,
  TOK_LOOP        = 268,
  TOK_LOOPD       = 269,
  TOK_LOOPE       = 270,
  TOK_LOOPED      = 271,
  TOK_LOOPNE      = 272,
  TOK_LOOPNED     = 273,
  TOK_LOW         = 274,
  TOK_LOWW        = 275,
  TOK_LSL         = 276,
  TOK_LSS         = 277,
  TOK_LT          = 278,
  TOK_LTR         = 279,
  TOK_MASK        = 280,
  TOK_MOD         = 281,
  TOK_MOV         = 282,
  TOK_MOVS        = 283,
  TOK_MOVSX       = 284,
  TOK_MOVZX       = 285,
  TOK_MUL         = 286,
  TOK_NAME        = 287,
  TOK_NE          = 288,
  TOK_NEAR        = 289,
  TOK_NEG         = 290,
  TOK_NOP         = 291,
  TOK_NOT         = 292,
  TOK_NOTHING     = 293,
  TOK_OFFSET      = 294,
  TOK_OR          = 295,
  TOK_ORG         = 296,
  TOK_OUT         = 297,
  TOK_OUTS        = 298,
  TOK_POP         = 299,
  TOK_POPA        = 300,
  TOK_POPAD       = 301,
  TOK_POPF        = 302,
  TOK_POPFD       = 303,
  TOK_PROC        = 304,
  TOK_PROCLEN     = 305,
  TOK_PTR         = 306,
  TOK_PUBLIC      = 307,
  TOK_PURGE       = 308,
  TOK_PUSH        = 309,
  TOK_PUSHA       = 310,
  TOK_PUSHAD      = 311,
  TOK_PUSHF       = 312,
  TOK_PUSHFD      = 313,
  TOK_PWORD       = 314,
  TOK_QWORD       = 315,
  TOK_RCL         = 316,
  TOK_RCR         = 317,
  TOK_RECORD      = 318,
  TOK_REPE        = 319,
  TOK_REPNE       = 320,
  TOK_RET         = 321,
  TOK_RO          = 322,
  TOK_ROL         = 323,
  TOK_ROR         = 324,
  TOK_RW          = 325,
  TOK_SAHF        = 326,
  TOK_SAL         = 327,
  TOK_SAR         = 328,
  TOK_SCAS        = 329,
  TOK_SEG         = 330,
  TOK_SEGMENT     = 331,
  TOK_SET         = 332,
  TOK_SGDT        = 333,
  TOK_SHL         = 334,
  TOK_SHLD        = 335,
  TOK_SHORT       = 336,
  TOK_SHR         = 337,
  TOK_SHRD        = 338,
  TOK_SI          = 339,
  TOK_SIDT        = 340,
  TOK_SIZE        = 341,
  TOK_SLDT        = 342,
  TOK_SMSW        = 343,
  TOK_SP          = 344,
  TOK_SS          = 345,
  TOK_STACKSEG    = 346,
  TOK_STACKSTART  = 347,
  TOK_STC         = 348,
  TOK_STD         = 349,
  TOK_STI         = 350,
  TOK_STOS        = 351,
  TOK_STR         = 352,
  TOK_STRUC       = 353,
  TOK_SUB         = 354,
  TOK_TBYTE       = 355,
  TOK_TEST        = 356,
  TOK_THIS        = 357,
  TOK_TR3         = 358,
  TOK_TR4         = 359,
  TOK_TR5         = 360,
  TOK_TR6         = 361,
  TOK_TR7         = 362,
  TOK_TYPE        = 363,
  TOK_USE16       = 364,
  TOK_USE32       = 365,
  TOK_VERR        = 366,
  TOK_VERW        = 367,
  TOK_WAIT        = 368,
  TOK_WBINVD      = 369,
  TOK_WIDTH       = 370,
  TOK_WORD        = 371,
  TOK_XADD        = 372,
  TOK_XCHG        = 373,
  TOK_XLAT        = 374,
  TOK_XOR         = 375,
  TOK_SBB         = 376,
  
  TOK_INTEGERBIN  = 377,
  TOK_INTEGEROCT  = 378,
  TOK_INTEGERDEC  = 379,
  TOK_INTEGERHEX  = 380,
  TOK_DOUBLE      = 381,
  TOK_STRING      = 382,
  TOK_IDENTIFIER  = 383,

/*
 * Productions
 */
  
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
  PRD_STMT        = 8153,
  PRD_AAA         = 8154,
  PRD_AAD         = 8155,
  PRD_AAM         = 8156,
  PRD_AAS         = 8157,
  PRD_REG16       = 8158,
  PRD_ADC         = 8159,
  PRD_REG8        = 8160,
  PRD_REGSEG      = 8161,
  PRD_ALUOP       = 8162,
  PRD_JMPOP       = 8163,
  PRD_JR          = 8164,
  PRD_DB          = 8165,

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

#endif /* asm286_h */

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
