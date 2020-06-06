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
 *    2020 May 17 Paul Willmott Processor type tags added.
 *
 *  Revision History:
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "asm286.h"

/*
 * Note: The RESERVED, KEYWORD, and KEYWORD_RESERVED flags are placeholders
 * in case EQU/PURGE function is added in the future.
 */

const char *pattern[ NUM_PATTERN ] = {
  RESERVED         "_",
  RESERVED         "-",
  RESERVED         ";",
  RESERVED         ":",
  RESERVED         "[{?}]",
  RESERVED         ".",
  RESERVED         "'",
  RESERVED         "{(}",
  RESERVED         "{)}",
  RESERVED         "{[}",
  RESERVED         "{]}",
  RESERVED         "@",
  RESERVED         "*",
  RESERVED         "/",
  RESERVED         "\"",
  RESERVED         "&",
  RESERVED         "+",
  RESERVED         "<",
  RESERVED         ">",
  RESERVED         "$",
  RESERVED         ",",
  RESERVED         "=",
  RESERVED         "%",
  RESERVED         ".8086",
  RESERVED         ".8087",
  RESERVED         ".186",
  RESERVED         ".286",
  RESERVED         ".286P",
  RESERVED         ".287",
  RESERVED         ".CODE",
  RESERVED         ".CONST",
  RESERVED         ".DATA",
  RESERVED         ".DATA[{?}]",
  RESERVED         ".FARDATA",
  RESERVED         ".FARDATA[{?}]",
  RESERVED         ".LIST",
  RESERVED         ".MODEL",
  RESERVED         ".NO87",
  RESERVED         ".NOLIST",
  RESERVED         ".STACK",
  KEYWORD          "AAA",
  KEYWORD          "AAD",
  KEYWORD          "AAM",
  KEYWORD          "AAS",
  RESERVED         "ABS",
  KEYWORD          "ADC",
  KEYWORD          "ADD",
  RESERVED         "AH",
  RESERVED         "AL",
  RESERVED         "ALL",
  RESERVED         "ALIGN",
  KEYWORD_RESERVED "AND",
  KEYWORD_286      "ARPL",
  RESERVED         "ASSUME",
  RESERVED         "AT",
  RESERVED         "AX",
  RESERVED         "BH",
  RESERVED         "BL",
  KEYWORD          "BOUND",
  RESERVED         "BP",
  RESERVED         "BX",
  RESERVED         "BYTE",
  RESERVED         "C",
  KEYWORD          "CALL",
  RESERVED         "CASEMAP",
  KEYWORD          "CBW",
  RESERVED         "CH",
  RESERVED         "CL",
  KEYWORD          "CLC",
  KEYWORD          "CLD",
  KEYWORD          "CLI",
  KEYWORD_286P     "CLTS",
  KEYWORD          "CMC",
  KEYWORD          "CMP",
  KEYWORD          "CMPS",
  KEYWORD          "CMPSB",
  KEYWORD          "CMPSW",
  RESERVED         "COMMON",
  RESERVED         "COMPACT",
  RESERVED         "CS",
  KEYWORD          "CWD",
  RESERVED         "CX",
  KEYWORD          "DAA",
  KEYWORD          "DAS",
  RESERVED         "DB",
  RESERVED         "DD",
  KEYWORD          "DEC",
  RESERVED         "DEFINED",
  RESERVED         "DF",
  RESERVED         "DGROUP",
  RESERVED         "DH",
  RESERVED         "DI",
  KEYWORD          "DIV",
  RESERVED         "DL",
  RESERVED         "DQ",
  RESERVED         "DS",
  RESERVED         "DT",
  RESERVED         "DUP",
  RESERVED         "DW",
  RESERVED         "DWORD",
  RESERVED         "DX",
  RESERVED         "ECHO",
  RESERVED         "ELSE",
  RESERVED         "ELSEIF",
  RESERVED         "END",
  RESERVED         "ENDIF",
  RESERVED         "ENDS",
  KEYWORD          "ENTER",
  RESERVED         "EQ",
  RESERVED         "EQU",
  RESERVED         "ERROR",
  RESERVED         "ES",
  KEYWORD          "ESC",
  RESERVED         "EVEN",
  RESERVED         "EXTERN",
  KEYWORD_8087     "F2XM1",
  KEYWORD_8087     "FABS",
  RESERVED         "FALSE",
  KEYWORD_8087     "FADD",
  KEYWORD_8087     "FADDP",
  RESERVED         "FAR",
  RESERVED         "FARSTACK",
  KEYWORD_8087     "FBLD",
  KEYWORD_8087     "FBSTP",
  KEYWORD_8087     "FCHS",
  KEYWORD_8087     "FCLEX",
  KEYWORD_8087     "FCOM",
  KEYWORD_8087     "FCOMP",
  KEYWORD_8087     "FCOMPP",
  KEYWORD_8087     "FDECSTP",
  KEYWORD_8087     "FDISI",
  KEYWORD_8087     "FDIV",
  KEYWORD_8087     "FDIVP",
  KEYWORD_8087     "FDIVR",
  KEYWORD_8087     "FDIVRP",
  KEYWORD_8087     "FENI",
  KEYWORD_8087     "FFREE",
  KEYWORD_8087     "FIADD",
  KEYWORD_8087     "FICOM",
  KEYWORD_8087     "FICOMP",
  KEYWORD_8087     "FIDIV",
  KEYWORD_8087     "FIDIVR",
  KEYWORD_8087     "FILD",
  KEYWORD_8087     "FIMUL",
  KEYWORD_8087     "FINCSTP",
  KEYWORD_8087     "FINIT",
  KEYWORD_8087     "FIST",
  KEYWORD_8087     "FISTP",
  KEYWORD_8087     "FISUB",
  KEYWORD_8087     "FISUBR",
  KEYWORD_8087     "FLD",
  KEYWORD_8087     "FLD1",
  KEYWORD_8087     "FLDCW",
  KEYWORD_8087     "FLDENV",
  KEYWORD_8087     "FLDL2E",
  KEYWORD_8087     "FLDL2T",
  KEYWORD_8087     "FLDLG2",
  KEYWORD_8087     "FLDLN2",
  KEYWORD_8087     "FLDPI",
  KEYWORD_8087     "FLDZ",
  KEYWORD_8087     "FMUL",
  KEYWORD_8087     "FMULP",
  KEYWORD_8087     "FNCLEX",
  KEYWORD_8087     "FNDISI",
  KEYWORD_8087     "FNENI",
  KEYWORD_8087     "FNINIT",
  KEYWORD_8087     "FNOP",
  KEYWORD_8087     "FNSAVE",
  KEYWORD_8087     "FNSTCW",
  KEYWORD_8087     "FNSTENV",
  KEYWORD_8087     "FNSTSW",
  KEYWORD_8087     "FPATAN",
  KEYWORD_8087     "FPREM",
  KEYWORD_8087     "FPTAN",
  KEYWORD_8087     "FRNDINT",
  KEYWORD_8087     "FRSTOR",
  KEYWORD_8087     "FSAVE",
  KEYWORD_8087     "FSCALE",
  KEYWORD_287      "FSETPM",
  KEYWORD_8087     "FSQRT",
  KEYWORD_8087     "FST",
  KEYWORD_8087     "FSTCW",
  KEYWORD_8087     "FSTENV",
  KEYWORD_8087     "FSTP",
  KEYWORD_8087     "FSTSW",
  KEYWORD_8087     "FSUB",
  KEYWORD_8087     "FSUBP",
  KEYWORD_8087     "FSUBR",
  KEYWORD_8087     "FSUBRP",
  KEYWORD_8087     "FTST",
  KEYWORD_8087     "FWAIT",
  RESERVED         "FWORD",
  KEYWORD_8087     "FXAM",
  KEYWORD_8087     "FXCH",
  KEYWORD_8087     "FXTRACT",
  KEYWORD_8087     "FYL2X",
  KEYWORD_8087     "FYL2XP1",
  RESERVED         "GE",
  RESERVED         "GROUP",
  RESERVED         "GT",
  RESERVED         "HIGH",
  RESERVED         "HUGE",
  KEYWORD          "HLT",
  KEYWORD          "IDIV",
  RESERVED         "IF",
  KEYWORD          "IMUL",
  KEYWORD          "IN",
  KEYWORD          "INC",
  KEYWORD          "INS",
  KEYWORD          "INSB",
  KEYWORD          "INSW",
  KEYWORD          "INT",
  KEYWORD          "INTO",
  KEYWORD          "IRET",
  KEYWORD          "JA",
  KEYWORD          "JAE",
  KEYWORD          "JB",
  KEYWORD          "JBE",
  KEYWORD          "JC",
  KEYWORD          "JCXZ",
  KEYWORD          "JE",
  KEYWORD          "JG",
  KEYWORD          "JGE",
  KEYWORD          "JL",
  KEYWORD          "JLE",
  KEYWORD          "JMP",
  KEYWORD          "JNA",
  KEYWORD          "JNAE",
  KEYWORD          "JNB",
  KEYWORD          "JNBE",
  KEYWORD          "JNC",
  KEYWORD          "JNE",
  KEYWORD          "JNG",
  KEYWORD          "JNGE",
  KEYWORD          "JNL",
  KEYWORD          "JNLE",
  KEYWORD          "JNO",
  KEYWORD          "JNP",
  KEYWORD          "JNS",
  KEYWORD          "JNZ",
  KEYWORD          "JO",
  KEYWORD          "JP",
  KEYWORD          "JPE",
  KEYWORD          "JPO",
  KEYWORD          "JS",
  KEYWORD          "JZ",
  RESERVED         "LABEL",
  KEYWORD          "LAHF",
  RESERVED         "LANGUAGE",
  KEYWORD_286      "LAR",
  RESERVED         "LARGE",
  KEYWORD          "LDS",
  RESERVED         "LE",
  KEYWORD          "LEA",
  KEYWORD_186      "LEAVE",
  RESERVED         "LENGTHOF",
  KEYWORD          "LES",
  KEYWORD_286P     "LGDT",
  KEYWORD_286P     "LIDT",
  RESERVED         "LINE",
  RESERVED         "LJMP",
  KEYWORD_286P     "LLDT",
  KEYWORD_286P     "LMSW",
  KEYWORD          "LOCK",
  KEYWORD          "LODS",
  KEYWORD          "LODSB",
  KEYWORD          "LODSW",
  KEYWORD          "LOOP",
  KEYWORD          "LOOPE",
  KEYWORD          "LOOPNE",
  KEYWORD          "LOOPNZ",
  KEYWORD          "LOOPZ",
  RESERVED         "LOW",
  KEYWORD_286      "LSL",
  RESERVED         "LT",
  KEYWORD_286P     "LTR",
  RESERVED         "MASK",
  RESERVED         "MEDIUM",
  RESERVED         "MEMORY",
  RESERVED         "MOD",
  KEYWORD          "MOV",
  KEYWORD          "MOVS",
  KEYWORD          "MOVSB",
  KEYWORD          "MOVSW",
  KEYWORD          "MUL",
  RESERVED         "NAME",
  RESERVED         "NE",
  RESERVED         "NEAR",
  RESERVED         "NEARSTACK",
  KEYWORD          "NEG",
  RESERVED         "NOLJMP",
  RESERVED         "NONE",
  KEYWORD          "NOP",
  RESERVED         "NOREADONLY",
  KEYWORD_RESERVED "NOT",
  RESERVED         "NOTHING",
  RESERVED         "NOTPUBLIC",
  RESERVED         "OFFSET",
  RESERVED         "OPTION",
  KEYWORD_RESERVED "OR",
  RESERVED         "ORG",
  KEYWORD          "OUT",
  KEYWORD          "OUTS",
  KEYWORD          "OUTSB",
  KEYWORD          "OUTSW",
  RESERVED         "PARA",
  RESERVED         "PAGE",
  RESERVED         "PASCAL",
  KEYWORD          "POP",
  KEYWORD          "POPA",
  KEYWORD          "POPF",
  RESERVED         "PRIVATE",
  RESERVED         "PTR",
  RESERVED         "PUBLIC",
  KEYWORD          "PUSH",
  KEYWORD          "PUSHA",
  KEYWORD          "PUSHF",
  RESERVED         "QWORD",
  KEYWORD          "RCL",
  KEYWORD          "RCR",
  RESERVED         "READONLY",
  RESERVED         "REAL4",
  RESERVED         "REAL8",
  RESERVED         "REAL10",
  KEYWORD          "REP",
  KEYWORD          "REPE",
  KEYWORD          "REPNE",
  KEYWORD          "REPNZ",
  KEYWORD          "REPZ",
  KEYWORD          "RET",
  KEYWORD          "ROL",
  KEYWORD          "ROR",
  KEYWORD          "SAHF",
  KEYWORD          "SAL",
  KEYWORD          "SAR",
  KEYWORD          "SBB",
  RESERVED         "SBYTE",
  KEYWORD          "SCAS",
  KEYWORD          "SCASB",
  KEYWORD          "SCASW",
  RESERVED         "SDWORD",
  RESERVED         "SEG",
  RESERVED         "SEGMENT",
  KEYWORD_286      "SGDT",
  KEYWORD_RESERVED "SHL",
  RESERVED         "SHORT",
  KEYWORD_RESERVED "SHR",
  RESERVED         "SI",
  KEYWORD_286      "SIDT",
  RESERVED         "SIZEOF",
  KEYWORD_286      "SLDT",
  RESERVED         "SMALL",
  KEYWORD_286      "SMSW",
  RESERVED         "SP",
  RESERVED         "SS",
  RESERVED         "ST",
  RESERVED         "STACK",
  KEYWORD          "STC",
  KEYWORD          "STD",
  KEYWORD          "STI",
  KEYWORD          "STOS",
  KEYWORD          "STOSB",
  KEYWORD          "STOSW",
  KEYWORD_286      "STR",
  KEYWORD          "SUB",
  RESERVED         "SUBTITLE",
  RESERVED         "SWORD",
  RESERVED         "TBYTE",
  KEYWORD          "TEST",
  RESERVED         "TEXTEQU",
  RESERVED         "THIS",
  RESERVED         "TINY",
  RESERVED         "TITLE",
  RESERVED         "TRUE",
  RESERVED         "TYPE",
  KEYWORD_286      "VERR",
  KEYWORD_286      "VERW",
  KEYWORD          "WAIT",
  RESERVED         "WIDTH",
  RESERVED         "WORD",
  KEYWORD          "XCHG",
  KEYWORD          "XLAT",
  KEYWORD          "XLATB",
  KEYWORD_RESERVED "XOR",
  RESERVED         "#DEFINE",
  RESERVED         "#ELSE",
  RESERVED         "#ELSEIF",
  RESERVED         "#ENDIF",
  RESERVED         "#IF",
  RESERVED         "#INCLUDE",
  RESERVED         "#UNDEF",
  SPECIAL          "[0-1]+B",
  SPECIAL          "[0-7]+O|Q",
  SPECIAL          "(-|+)?[0-9]+D?",
  SPECIAL          "([0-9][0-9A-F]*H)|([0-9][0-9A-F]*R)",
  SPECIAL          "((-|+)?[0-9]+([.][0-9]*)?E(+|-)?[0-9]+)|((-|+)?[0-9]+[.][0-9]*)",
  SPECIAL          "([']([^']|(''))*['])+",
  SPECIAL          "[A-Z_@{?}][A-Z_@{?}0-9]*",
  SPECIAL          "[A-Z_@{?}][A-Z_@{?}0-9]*::?",
  SPECIAL          "(\n|(;[^\n]*\n))+",
  SPECIAL          "(\010|\011|\013|\014|\015|\032|\040|(\\[^\n]*\n))*",
  SPECIAL          "[^\n]*",
  SPECIAL          "[^#\n]*\n",
} ;

int checkInstruction(int tokenId) {
  return (check_instructions && tokenId < PRODUCTION_OFFSET) && ((PROCESSOR(*pattern[tokenId]) > processor) || (COPROCESSOR(*pattern[tokenId]) > coprocessor)) ;
}


char * prod_list[] = {
  "addOp",
  "andOp",
  "arbitaryText",
  "asmInstruction",
  "assumeDir",
  "assumeList",
  "assumeReg",
  "assumeRegister",
  "assumeSegReg",
  "assumeSegVal",
  "assumeVal",
  "bool",
  "byteRegister",
  "className",
  "commaOptNewline",
  "constant",
  "constExpr",
  "coprocessor",
  "dataDecl",
  "dataDir",
  "dataItem",
  "dataType",
  "directive",
  "directiveList",
  "e01",
  "e02",
  "e03",
  "e04",
  "e05",
  "e06",
  "e07",
  "e08Op",
  "e08",
  "e09",
  "e10",
  "e11",
  "e12",
  "echoDir",
  "elseifBlock",
  "elseifList",
  "elseifStatement",
  "elseBlock",
  "elseStatement",
  "endDir",
  "endifStatement",
  "endsDir",
  "equalDir",
  "equDir",
  "equId",
  "expr",
  "externDef",
  "externDir",
  "externList",
  "externType",
  "fileSpec",
  "fpInstruction",
  "fpMnemonicOne",
  "fpMnemonicTwo",
  "fpMnemonicZero",
  "frameExpr",
  "generalDir",
  "gpRegister",
  "groupDir",
  "groupId",
  "ifBlock",
  "ifDir",
  "ifStatement",
  "immExpr",
  "initValue",
  "inSegDir",
  "inSegDirList",
  "inSegmentDir",
  "instPrefix",
  "instruction",
  "labelDef",
  "labelDir",
  "langType",
  "lineDir",
  "listDir",
  "listOption",
  "mapType",
  "memOption",
  "mnemonicOne",
  "mnemonicTwo",
  "mnemonicZero",
  "modelDir",
  "modelOpt",
  "modelOptList",
  "module",
  "mulOp",
  "nameDir",
  "nearfar",
  "offsetDir",
  "offsetDirType",
  "operand",
  "optionDir",
  "optionItem",
  "optionList",
  "orOp",
  "pageDir",
  "pageExpr",
  "pageLength",
  "pageWidth",
  "ppDefineDir",
  "ppDir",
  "ppDirectiveList",
  "ppelseBlock",
  "ppelseifBlock",
  "ppelseifList",
  "ppelseifStatement",
  "ppelseStatement",
  "ppendifStatement",
  "ppifBlock",
  "ppifDir",
  "ppifStatement",
  "ppIncludeDir",
  "ppModule",
  "ppUndefDir",
  "processor",
  "processorDir",
  "pubDef",
  "publicDir",
  "pubList",
  "qualifiedType",
  "register",
  "relOp",
  "scalarInstList",
  "segAlign",
  "segAttrib",
  "segDir",
  "segId",
  "segIdList",
  "segmentDef",
  "segmentDir",
  "segmentRegister",
  "segOption",
  "segOptionList",
  "simpleEndsDir",
  "simpleSegDir",
  "sizeArg",
  "stackOption",
  "textEquDir",
  "textList",
  "textItem",
  "titleDir",
  "titleType",
  "tokenSequence",
  "tokenSequenceList",
  "type",
  "",
} ;

void printProdName(int prodId) {
  printf("SPD_%s", prod_list[prodId-8128]);
}

void dump_productions()
{
  for (unsigned int idx = 0; strlen(prod_list[idx]) > 0; idx++ ) {
    unsigned int B = (idx + PRODUCTION_OFFSET) % 127 + 1 ;
    unsigned int A = (idx + PRODUCTION_OFFSET) / 127 + 1 ;
    printf("#define SPD_%-17s \"\\%03o\\%03o\"\n", prod_list[idx], A, B ) ;
  }

  for ( unsigned int idx = 0; strlen(prod_list[idx]) > 0; idx++  ) {
    printf("  PRD_%-17s = %3i,\n", prod_list[idx], idx + PRODUCTION_OFFSET) ;
  }
}

void dump_pattern() 
{
  dump_productions();
  
  unsigned int idx, line ;

  for ( idx = 0; idx < NUM_PATTERN  ; idx++ ) {
    if (*(pattern[idx]+1)=='#') {
      printf("  TOK_HASH%-7s = %3i,\n", pattern[idx]+2, idx ) ;
    }
    else {
      printf("  TOK_%-11s = %3i,\n", pattern[idx]+1, idx ) ;
    }
  }

  for ( idx = 0; idx < NUM_PATTERN ; idx++ ) {
    unsigned int B = idx % 127 + 1 ;
    unsigned int A = idx / 127 + 1 ;
    if (*(pattern[idx]+1)=='#') {
      printf("#define STK_HASH%-7s \"\\%03o\\%03o\"\n", pattern[idx]+2, A, B ) ;
    }
    else {
      printf("#define STK_%-11s \"\\%03o\\%03o\"\n", pattern[idx]+1, A, B ) ;
    }
  }
 
  
  return;
  const char *keywords[500];
  int num_keywords = 0;
  for ( idx = 21; idx < NUM_PATTERN - 7; idx++ ) {
    if (*pattern[idx] == 1 || *pattern[idx] == 3) {
      keywords[num_keywords++] = pattern[idx];
    }
  }
  
  int NUM_LINES = 47;
  int NUM_COLUMNS = 5;
  int last_print = 0;
  int first_print = 0;
  for (int i = 0; i<1;i++) {
    printf("\\begin{table}\n");
    printf("\\caption{Assembler Keywords}\n\n");
    printf("\\begin{tabular}{p{2.5cm} p{2.5cm} p{2.5cm} p{2.5cm} p{2.5cm}}\n\\\\\n");
    for ( line = 0; line < NUM_LINES; line++ ) {
      int printed = 0;
      int item;
      for (item = 0; item < NUM_COLUMNS;item ++) {
        idx = first_print + line + item * NUM_LINES;
        if (idx < num_keywords) {
          if (item > 0) {
            printf(" & ");
          }
          printf("%s", keywords[idx]+1);
          last_print = idx;
          printed = 1;
        }
      }
      if (printed) {
        printf("\\\\\n") ;
      }
    }
    printf("\\end{tabular}\n");
    printf("\\end{table}\n\n");
//    printf("%i\n", last_print);
    first_print = last_print + 1;
  }
// RESERVED WORDS
  
  num_keywords = 0;
  for ( idx = 21; idx < NUM_PATTERN - 7; idx++ ) {
    if (*pattern[idx] == 2 || *pattern[idx] == 3) {
      keywords[num_keywords++] = pattern[idx];
    }
  }
  
  NUM_LINES = 20;
  last_print = 0;
  first_print = 0;
  for (int i = 0; i<1;i++) {
    printf("\\begin{table}\n");
    printf("\\caption{Assembler Reserved Words}\n\n");
    printf("\\begin{tabular}{p{2.5cm} p{2.5cm} p{2.5cm} p{2.5cm} p{2.5cm}}\n\\\\\n");
    for ( line = 0; line < NUM_LINES; line++ ) {
      int printed = 0;
      int item;
      for (item = 0; item < NUM_COLUMNS;item ++) {
        idx = first_print + line + item * NUM_LINES;
        if (idx < num_keywords) {
          if (item > 0) {
            printf(" & ");
          }
          printf("%s", keywords[idx]+1);
          last_print = idx;
          printed = 1;
        }
      }
      if (printed) {
        printf("\\\\\n") ;
      }
    }
    printf("\\end{tabular}\n");
    printf("\\end{table}\n\n");
    //    printf("%i\n", last_print);
    first_print = last_print + 1;
  }

}

#ifdef OLD
ptree_node_t *find_token(int id, FILE *fp)
#else
ptree_node_t *find_token(int id)
#endif

{
  
  char *match;
#ifdef OLD
  unsigned long matchlen;
#else
  long matchlen;
#endif
  
  ptree_node_t *ptree = NULL;
  
#ifdef OLD
  long pos = ftell(fp);
#else
  stream_context_t pos;
  if (saveStreamContext(&pos)) {
    return NULL;
  }
#endif
  
  char *pat;
  
  if ((pat = (char *) malloc (strlen(pattern[id]+1) + strlen(pattern[TOK_WHITESPACE]+1) + 1)) == NULL ) {
    errno = ERR_OUT_OF_MEMORY;
    return NULL;
  }
  
  strcpy(pat, pattern[TOK_WHITESPACE]+1);
  strcat(pat, pattern[id]+1);

#ifdef OLD
  if ( ! match_pattern2( fp, pat, &match, &matchlen ) ) {
#else
    if ( ! match_pattern2( pat, &match, &matchlen ) ) {
#endif
    
    if ((ptree = (ptree_node_t *) malloc(sizeof(ptree_node_t))) == NULL) {
      errno = ERR_OUT_OF_MEMORY;
      free(pat);
      return NULL;
    }
    
    ptree->production_id = id ;
    
    if (checkInstruction(id)) {
      errno = ERR_INVALID_INSTRUCTION;
      free(pat);
      // should remove ptree here
      return NULL;
    }

    if (*pattern[id] == '\004') { // SPECIAL
      
      trim(match);
      
      switch (id) {
        case TOK_INTEGERBIN:
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = (int) strtol(match, NULL, 2) ;
          break;
        case TOK_INTEGERDEC:
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = (int) strtol(match, NULL, 10) ;
          break;
        case TOK_INTEGERHEX:
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = (int) strtol(match, NULL, 16) ;
          break;
        case TOK_INTEGEROCT:
          ptree->value_type = TOK_INTEGERDEC;
          ptree->value.i = (int) strtol(match, NULL, 8) ;
          break;
        case TOK_IDENTIFIER:
          ptree->value_type = TOK_STRING;
          ptree->value.s = match;
          break;
        case TOK_INST_LABEL:
          ptree->value_type = TOK_STRING;
          ptree->value.s = match;
          break;
        case TOK_DOUBLE:
          ptree->value_type = TOK_DOUBLE;
          break;
        case TOK_STRING:
        {
          ptree->value_type = TOK_STRING;
          unsigned long l = strlen(match), lim = l - 1;
          match[l-1] = '\0';
          for (int y = 2; y < lim; y++) {
            if ( match[y] == '\'' && match[y-1] == '\'') {
              for (int x = y-1; x < lim; x++) {
                match[x] = match[x+1];
              }
              lim--;
            }
          }
          ptree->value.s = match+1;
          break;
        }
        case TOK_TEXT:
          ptree->value_type = TOK_STRING;
          ptree->value.s = match;
          break;
      }
      
    }

    if (ptree->value_type != TOK_STRING) {
      free(match);
    }

#ifdef OLD
    fseek(fp, pos + matchlen, SEEK_SET);
#else
    if (seekStream(&pos, matchlen)) {
      return NULL;
    }
#endif
      
  }
  else {
  }
  
  free(pat);
  
  return ptree;
}

#ifdef OLD
int match_pattern2
(
 FILE *fp,
 const char *pattern,
 char **match,
 unsigned long *matchlen
)
#else
int match_pattern2
(
  const char *pattern,
  char **match,
  long *matchlen
)
#endif
  
{
  
  int result = -1 ;

#ifdef OLD
  long int tp, end_of_file;
  long int sptr = ftell(fp) ;
#else
  stream_context_t sptr;
  if (saveStreamContext(&sptr)) {
    goto fail;
  }
#endif
  
  char cv[128] ;
  
  char *expression = NULL, *ptr ;
  
  if ((*match = (char *) malloc(256)) == NULL) {
    errno = ERR_OUT_OF_MEMORY ;
    goto fail;
  }
  **match = '\0';

#ifdef OLD
  tp = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  end_of_file = ftell(fp);
  fseek(fp, tp, SEEK_SET);
#endif
  
  /*
   *------------------------------------------------------------------------------
   */
  
  /*
   * Keep going until all of the pattern has been matched.
   */
  
  int skip_next = 0 ;
  
  while ( *pattern ) {
    
    unsigned long seglen = 0 ;
    
    char start = *pattern, stop = '\0' ;
    
#ifdef OLD
    long int save = sptr ;
    fseek(fp, sptr, SEEK_SET);
#else
    stream_context_t save;
    copyStreamContext(&save, &sptr);
    if (restoreStreamContext(&sptr)) {
      goto fail;
    }
#endif
    
    switch ( *pattern ) {
      case '(':
        stop = ')' ;
        break ;
      case '[':
        stop = ']' ;
        break ;
      case '{':
        stop = '}' ;
        break ;
      default:
        seglen = 1 ;
        break ;
    }
    
    if ( ! seglen ) {
      
      int count = 1 ;
      
      const char *temp = pattern + 1 ;
      
      while ( *temp && count ) {
        if ( *temp == start ) {
          count++ ;
        }
        else if ( *temp == stop ) {
          count-- ;
        }
        temp++ ;
      }
      
      seglen = temp - pattern ;
      
      if ( start == '(' || start == '{' ) {
        if (expression != NULL ) {
          free(expression) ;
        }
        if ( ( expression = (char *) malloc( seglen + 1 - 2 ) ) == NULL ) {
          errno = ERR_OUT_OF_MEMORY ;
          goto fail;
        }
        strncpy( expression, pattern + 1 , seglen - 2 ) ;
        expression[seglen - 2] = '\0' ;
      }
      else if ( start == '[' ) {
        
        temp = pattern + 1 ;
        
        char lc = *temp ;
        
        ptr = cv ;
        
        if ( lc == '^' ) {
          
          unsigned int cc ;
          
          for ( cc = 1; cc < 128; cc++ ) {
            temp = pattern + 1 ;
            while ( ( lc = *temp++ ) != stop ) {
              if ( lc == cc ) {
                break ;
              }
            }
            if ( lc == stop ) {
              *ptr++ = (char) cc ;
            }
          }
          
        }
        else {
          
          while ( ( lc = *temp ) != stop ) {
            
            if ( *(temp+1) == '-' ) {
              char cc ;
              for ( cc = lc; cc <= *(temp+2); cc++ ) {
                *ptr++ = cc ;
              }
              temp += 3 ;
            }
            else {
              *ptr++ = lc ;
              temp++ ;
            }
            
          }
          
        }
        
        *ptr = '\0' ;
        
      }
      
    }
    
    char operator = *(pattern+seglen) ;
    
    switch ( operator ) {
      case '?':
      case '*':
      case '+':
        seglen++ ;
        break;
    }
    
    pattern += seglen ;
    
    if ( skip_next ) {
      skip_next = 0 ;
      continue;
    }
    
    int found_count = 0 ;
    
    int more_to_find ;
    
    int is_success ;
    
    do {
      
      int is_match ;
      
      int cti ;
#ifdef OLD
      fseek(fp, sptr, SEEK_SET);
      if ((cti = fgetc(fp)) == EOF) {
        sptr++;
        goto fail;
      }
#else
      if (restoreStreamContext(&sptr)) {
        goto fail;
      }
      if (readStream(&cti) || cti == EOF) {
        saveStreamContext(&sptr);
        goto fail;
      }
#endif
      char ct = toupper((char)cti) ;
      
      switch ( start ) {
        case '(':
        {
          char *temp ;
#ifdef OLD
          unsigned long matchlen ;
          fseek(fp,sptr, SEEK_SET);
          if ( ( is_match = ! match_pattern2( fp, expression, &temp, &matchlen ) ) ) {
            strcat(*match, temp);
            free(temp);
            sptr = ftell(fp);//sptr + matchlen; //ftell(fp);
          }
#else
          long matchlen ;
          if (restoreStreamContext(&sptr)) {
            goto fail;
          }
          if ( ( is_match = ! match_pattern2( expression, &temp, &matchlen ) ) ) {
            strcat(*match, temp);
            free(temp);
            if (saveStreamContext(&sptr)) {
              goto fail;
            }
          }
#endif
          
        }
          break ;
        case '[':
          ptr = cv ;
          while ( *ptr && ct != *ptr ) {
            ptr++ ;
          }
          if ( ( is_match = *ptr ) ) {
            strncat(*match, (const char *)&cti, 1);
#ifdef OLD
            sptr++;
#else
            if (saveStreamContext(&sptr)) {
              goto fail;
            }
#endif
          }
          break ;
        case '{':
          {
          int ci;
          ptr = expression ;
#ifdef OLD
          while ( ((ci = fgetc(fp)) != EOF) && *ptr && (char)ci == *ptr ) {
            strncat(*match, (char *)&ci, 1);
            sptr++ ;
            ptr++ ;
          }
#else
          while ( !readStream(&ci) && (ci != EOF) && *ptr && (char)ci == *ptr ) {
            strncat(*match, (char *)&ci, 1);
            if (saveStreamContext(&sptr)) {
              goto fail;
            }
            ptr++ ;
          }
#endif
          is_match = ! *ptr ;
          }
          break ;
        default:
          if ( ( is_match = ( ct == start ) ) ) {
            strncat(*match, (char *)&cti, 1);
#ifdef OLD
            sptr++ ;
#else
            if (saveStreamContext(&sptr)) {
              goto fail;
            }
#endif
          }
          break ;
      }
      
      if ( is_match ) {
        found_count++ ;
      }
      
      switch ( operator ) {
        case '?':
          is_success = 1 ;
          more_to_find = 0 ;
          break;
        case '*':
          is_success = 1 ;
          more_to_find = is_match ;
          break;
        case '+':
          is_success = ( found_count > 0 ) ;
          more_to_find = is_match ;
          break;
        default:
          is_success = is_match ;
          more_to_find = 0 ;
          break;
      }

#ifdef OLD
    } while ( more_to_find && sptr < end_of_file ) ;
#else
  } while ( more_to_find && ! endOfInput()) ;
#endif
    
    /*
     * Handle Or.
     */
    
    if ( *pattern == '|' ) {
      skip_next = is_success ;
      if ( ! skip_next ) {
#ifdef OLD
        sptr = save ;
#else
        copyStreamContext(&sptr, &save);
#endif
     //   fseek(fp, sptr, SEEK_SET);
      }
      pattern++;
    }
    
    /*
     * Bomb if no match.
     */
    
    else if ( ! is_success ) {
      goto fail ;
    }
    
  }
  
  *matchlen = strlen(*match) ;
  
  /*
   * Tidy-Up.
   */
  
  result = 0 ;
  
fail:
  
  if (result) {
    free(*match);
    *match = NULL;
  }
  
  if (expression != NULL ) {
    free(expression) ;
  }
  
  /*
   * Finished.
   */
  
  return result ;
  
}

void trim(char * s){
  char *white = "\010\011\013\014\015\032\040";
  unsigned long i;
  for (i = 0; s[i]; i++) {
    int w = 0;
    for (int j = 0; white[j]; j++) {
      if (s[i] == white[j]) {
        w = 1;
        break;
      }
    }
    if (!w) {
      break;
    }
  }
  if (s[i]) {
    unsigned long l = strlen(s) - i;
    for (unsigned long j = 0; j <= l; j++) {
      s[j] = s[j+i];
    }
  }
}

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
