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
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "asm286.h"

const char *pattern[ NUM_PATTERN ] = {
  "_",
  "-",
  ";",
  ":",
  "?",
  ".",
  "'",
  "{(}",
  "{)}",
  "{[}",
  "{]}",
  "@",
  "*",
  "/",
  "\"",
  "&",
  "+",
  "<",
  ">",
  "$",
  ",",
  "AAA",
  "AAD",
  "AAM",
  "AAS",
 // "ABS",
  "ADC",
  "ADD",
  "AH",
  "AL",
//  "ALIGN", // ASM386
  "AND",
  "ARPL",
//  "ASSUME",
  "AX",
  "BH",
//  "BIT", // ASM386
//  "BITOFFSET", // ASM386
  "BL",
  "BOUND",
  "BP",
//  "BSF",   /* 386 */
//  "BSR",   /* 386 */
//  "BSWAP",
//  "BT",    /* 386 */
//  "BTC",   /* 386 */
//  "BTR",   /* 386 */
//  "BTS",   /* 386 */
  "BX",
//  "BYTE",
  "CALL",
  "CBW",
//  "CDQ",   /* 386 */
  "CH",
  "CL",
  "CLC",
  "CLD",
  "CLI",
  "CLTS",
  "CMC",
  "CMP",
  "CMPS",
  "CMPSB",
  "CMPSW",
//  "CMPSD",  /* 386 */
//  "CMPXCHG",
//  "COMM", // MAYBE ASM386
//  "COMMON",
//  "CR0",   /* 386 */
//  "CR1",   /* 386 */
//  "CR2",   /* 386 */
  "CS",
  "CWD",
//  "CWDE",  /* 386 */
  "CX",
  "DAA",
  "DAS",
  "DB",
//  "DBIT", // ASM386
  "DD",
  "DEC",
  "DH",
  "DI",
  "DIV",
  "DL",
//  "DP", // ASM386
//"DQ",
//  "DR0",  /* 386 */
//  "DR1",  /* 386 */
//  "DR2",  /* 386 */
//  "DR3",  /* 386 */
//  "DR6",  /* 386 */
//  "DR7",  /* 386 */
  "DS",
//  "DT",
  "DUP",
  "DW",
//  "DWORD",
  "DX",
//  "EAX", /* 386 */
//  "EBP", /* 386 */
//  "EBX", /* 386 */
//  "ECX", /* 386 */
//  "EDI", /* 386 */
//  "EDX", /* 386 */
  "END",
//  "ENDP",
//  "ENDS",
  "ENTER",
//  "EO",
  "EQ",
  "EQU",
//  "ER",
  "ES",
//  "ESC",
//  "ESI",
//  "ESP",
//  "EVEN", // ASM386
//  "EXTRN",
  "F2XM1",
  "FABS",
  "FADD",
  "FADDP",
//  "FAR",
  "FBLD",
  "FBSTP",
  "FCHS",
  "FCLEX",
  "FCOM",
  "FCOMP",
  "FCOMPP",
//  "FCOS", // 387
  "FDECSTP",
//  "FDISI",
  "FDIV",
  "FDIVP",
  "FDIVR",
  "FDIVRP",
//  "FENI",
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
//  "FNCLEX",
//  "FNDISI",
//  "FNENI",
 // "FNINIT",
  "FNOP",
  "FNSAVE",
  "FNSTCW",
  "FNSTENV",
  "FNSTSW",
  "FPATAN",
  "FPREM",
//  "FPREM1" // 387
  "FPTAN",
  "FRNDINT",
  "FRSTOR",
//  "FS",      /* 386 */
  "FSAVE",
  "FSCALE",
  "FSETPM",
//  "FSIN", // 387
//  "FSINCOS", // 387
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
//  "FUCOM", // 387
//  "FUCOMP", // 387
//  "FUCOMPP", // 387
  "FWAIT",
  "FXAM",
  "FXCH",
  "FXTRACT",
  "FYL2X",
  "FYL2XP1",
  "GE",
//  "GS",      /* 386 */
  "GT",
  "HIGH",
//  "HIGHW", // ASM386
  "HLT",
  "IDIV",
  "IMUL",
  "IN",
  "INC",
  "INS",
  "INSB",
  "INSW",
//  "INSD",  /* 386 */
  "INT",
  "INTO",
//  "INVD",
 // "INVLPG",
  "IRET",
//  "IRETD", /* 386 */
  "JA",
  "JAE",
  "JB",
  "JBE",
  "JC",
  "JCXZ",
  "JE",
//  "JECXZ", /* 386 */
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
//  "LFS",  /* 386 */
  "LGDT",
//  "LGDTD",  /* 386 */
//  "LGDTW", /* 386 */
//  "LGS",  /* 386 */
  "LIDT",
//  "LIDTD",  /* 386 */
//  "LIDTW",  /* 386 */
  "LLDT",
  "LMSW",
  "LOCK",
  "LODS",
  "LODSB",
  "LODSW",
//  "LODSD", /* 386 */
  "LOOP",
//  "LOOPD",
  "LOOPE",
//  "LOOPED",
  "LOOPNE",
  "LOOPNZ",
  "LOOPZ",
//  "LOOPNED",
  "LOW",
//  "LOWW", // ASM386
  "LSL",
//  "LSS", /* 386 */
  "LT",
  "LTR",
//  "MASK",
//  "MOD",
  "MOV",
  "MOVS",
  "MOVSB",
  "MOVSW",
//  "MOVSD", /* 386 */
//  "MOVSX", /* 386 */
//  "MOVZX",/* 386 */
  "MUL",
//  "NAME",
  "NE",
//  "NEAR",
  "NEG",
  "NOP",
  "NOT",
//  "NOTHING",
//  "OFFSET",
  "OR",
  "ORG",
  "OUT",
  "OUTS",
  "OUTSB",
  "OUTSW",
//  "OUTSD", /* 386 */
  "POP",
  "POPA",
//  "POPAD", /* 386 */
  "POPF",
//  "POPFD", /* 386 */
//  "PROC",
//  "PROCLEN",
//  "PTR",
//  "PUBLIC",
//  "PURGE",
  "PUSH",
  "PUSHA",
//  "PUSHAD", /* 386 */
  "PUSHF",
//  "PUSHFD", /* 386 */
//  "PWORD", // ASM386
//  "QWORD",
  "RCL",
  "RCR",
//  "RECORD",
  "REP",
  "REPE",
  "REPNE",
  "RET",
//  "RO",
  "ROL",
  "ROR",
//  "RW",
  "SAHF",
  "SAL",
  "SAR",
  "SBB",
  "SCAS",
  "SCASB",
  "SCASW",
//  "SCASD", /* 386 */
//  "SEG",
//  "SEGMENT",
//  "SET", /* 386 */
//  "SETAE", /* 386 */
//  "SETB", /* 386 */
//  "SETC", /* 386 */
//  "SETE", /* 386 */
//  "SETG", /* 386 */
//  "SETGE", /* 386 */
//  "SETL", /* 386 */
//  "SETLE", /* 386 */
//  "SETNA", /* 386 */
//  "SETNAE", /* 386 */
//  "SETNB", /* 386 */
//  "SETNBE", /* 386 */
//  "SETNC", /* 386 */
//  "SETNE", /* 386 */
//  "SETNG", /* 386 */
//  "SETNGE", /* 386 */
//  "SETNL", /* 386 */
//  "SETNLE", /* 386 */
//  "SETNO", /* 386 */
//  "SETNP", /* 386 */
//  "SETNS", /* 386 */
//  "SETNZ", /* 386 */
//  "SETO", /* 386 */
//  "SETP", /* 386 */
//  "SETPO", /* 386 */
//  "SETPE", /* 386 */
//  "SETS", /* 386 */
//  "SETZ", /* 386 */
  "SGDT",
//  "SGDTD", /* 386 */
//  "SGDTW", /* 386 */
  "SHL",
//  "SHLD", /* 386 */
//  "SHORT",
  "SHR",
//  "SHRD", /* 386 */
  "SI",
  "SIDT",
//  "SIDTD", /* 386 */
//  "STDTW", /* 386 */
//  "SIZE",
  "SLDT",
  "SMSW",
  "SP",
  "SS",
//  "STACKSEG",
//  "STACKSTART",
  "STC",
  "STD",
  "STI",
  "STOS",
  "STOSB",
  "STOSW",
//  "STOSD", /* 386 */
  "STR",
//  "STRUC",
  "SUB",
//  "TBYTE",
  "TEST",
//  "THIS",
  "TR3",
  "TR4",
  "TR5",
//  "TR6",  /* 386 */
//  "TR7",  /* 386 */
//  "TYPE",
//  "USE16", // ASM386
//  "USE32", // ASM386
  "VERR",
  "VERW",
  "WAIT",
//  "WBINVD",
//  "WIDTH",
//  "WORD",
//  "XADD",
  "XCHG",
  "XLAT",
  "XOR",
  "[0-1]+B",
  "[0-7]+O|Q",
  "(-|+)?[0-9]+D?",
  "([0-9][0-9A-F]*H)|([0-9][0-9A-F]*R)",
  "((-|+)?[0-9]+([.][0-9]*)?E(+|-)?[0-9]+)|((-|+)?[0-9]+[.][0-9]*)",
  "(['][^']*['])+",
  "[A-Z_][A-Z_0-9]*",
} ;

void dump_pattern() 
{
  return ;
  unsigned int idx ;
  
  for ( idx = 0; idx < NUM_PATTERN - 7; idx++ ) {
//    unsigned int B = idx % 127 + 1 ;
//    unsigned int A = idx / 127 + 1 ;
//    printf("#define STK_%-11s \"\\%03o\\%03o\"\n", pattern[idx], A, B ) ;
  }
  for ( idx = 0; idx < NUM_PATTERN - 7; idx++ ) {
 //   printf("  TOK_%-11s = %3i,\n", pattern[idx], idx ) ;
  }
//  printf("%i\n", 0177 * 127 + 0177 - 1) ;
}

int match_pattern
(
  const char *statement,
  const char *pattern,
  const char **match,
  unsigned long *matchlen
)
{
  
  int result = -1 ;
  
  const char *sptr = statement ;
  
  char cv[128] ;
  
  char *expression = NULL, *ptr ;
  
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
    
    const char *save = sptr ;
    
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
          error( ERR_OUT_OF_MEMORY, -1 ) ;
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
      
      char ct = toupper(*sptr) ;
      
      switch ( start ) {
        case '(':
          {
            const char *match ;
            unsigned long matchlen ;
            if ( ( is_match = ! match_pattern( sptr, expression, &match, &matchlen ) ) ) {
              sptr += matchlen;
            }
          }
          break ;
        case '[':
          ptr = cv ;
          while ( *ptr && ct != *ptr ) {
            ptr++ ;
          }
          if ( ( is_match = *ptr ) ) {
            sptr++;
          }
          break ;
        case '{':
          ptr = expression ;
          while ( *sptr && *ptr && *sptr == *ptr ) {
            sptr++ ;
            ptr++ ;
          }
          is_match = ! *ptr ;
          break ;
        default:
          if ( ( is_match = ( ct == start ) ) ) {
            sptr++ ;
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
      
    } while ( more_to_find && *sptr ) ;

/*
 * Handle Or.
 */
    
    if ( *pattern == '|' ) {
      skip_next = is_success ;
      if ( ! skip_next ) {
        sptr = save ;
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

  *matchlen = sptr - statement ;
  
  *match = statement ;
  
/*
 * Tidy-Up.
 */
  
  result = 0 ;
  
fail:
  
  if (expression != NULL ) {
    free(expression) ;
  }
  
/*
 * Finished.
 */
  
  return result ;
  
}

void delete_tlist( tlist_node_t **tlist )
{
  
  tlist_node_t *ptr = *tlist ;
  
/*
 *------------------------------------------------------------------------------
 */

  while ( ptr != NULL ) {
    tlist_node_t *temp = ptr ;
    if ( temp->token != NULL ) {
      free(temp->token) ;
    }
    free( temp ) ;
    ptr = ptr->next ;
  }
  
  *tlist = NULL ;
  
/*
 * Finished.
 */

}

tlist_node_t * tokenize( const char *statement, int lineno )
{
  
  int idx, result = -1 ;
  
  const char *sptr = statement, *best = NULL ;
  
  unsigned long bestlen = 0 ;
  
  int bestidx = -1 ;
  
  tlist_node_t *tlist = NULL, *last = NULL ;
  
/*
 *------------------------------------------------------------------------------
 */
  
/*
 * Keep going until all of statement has been tokenized.
 */
  
  while ( *sptr ) {
    
/*
 * Skip whitespace.
 */
    while ( *sptr && isspace( *sptr ) ) {
      sptr++ ;
    }
    
    if ( ! *sptr ) {
      break;
    }

/*
 * Check each pattern and find the longest match.
 */
    
    bestlen = 0 ;
    
    for ( idx = 0; idx < NUM_PATTERN; idx++ ) {
      
      const char *match = NULL ;
      
      unsigned long matchlen = 0 ;
      
      if ( ! match_pattern( sptr, pattern[idx], &match, &matchlen ) ) {
        if ( matchlen > bestlen ) {
          bestlen = matchlen ;
          bestidx = idx ;
          best = match ;
        }
      }
      
    }

/*
 * Fail if none found.
 */
    
    if ( ! bestlen ) {
      error( ERR_SYNTAX_ERROR, lineno ) ;
      goto fail ;
    }
    
    tlist_node_t *node ;
    
    if ( ( node = (tlist_node_t *) malloc( sizeof(tlist_node_t) ) ) == NULL ) {
      error( ERR_OUT_OF_MEMORY, -1 ) ;
      goto fail ;
    }
    
    node->token_id = bestidx ;
    
    if ( ( node->token = (char *) malloc( bestlen + 1 ) ) == NULL ) {
      error( ERR_OUT_OF_MEMORY, -1 ) ;
      goto fail ;
    }
    
    strncpy(node->token, best, bestlen) ;
    node->token[bestlen] = '\0' ;
    
    node->next = NULL ;
    
    if ( last == NULL ) {
      tlist = node ;
    }
    else {
      last->next = node ;
    }
    last = node ;
    
/*
 * Move onto the next part of the statement.
 */
    
    sptr += bestlen ;
    
  }
  
/*
 * Tidy-Up.
 */
  
  result = 0 ;
  
fail:
  
  if ( result ) {
    delete_tlist( &tlist ) ;
  }
  
  /*
   * Finished.
   */
  
  return tlist ;

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
