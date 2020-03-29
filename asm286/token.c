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
  RESERVED         "_",
  RESERVED         "-",
  RESERVED         ";",
  RESERVED         ":",
  RESERVED         "?",
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
  KEYWORD          "AAA",
  KEYWORD          "AAD",
  KEYWORD          "AAM",
  KEYWORD          "AAS",
  RESERVED         "ABS",
  KEYWORD          "ADC",
  KEYWORD          "ADD",
  RESERVED         "AH",
  RESERVED         "AL",
  KEYWORD_RESERVED "AND",
  KEYWORD          "ARPL",
  RESERVED         "ASSUME",
  RESERVED         "AX",
  RESERVED         "BH",
  RESERVED         "BL",
  KEYWORD          "BOUND",
  RESERVED         "BP",
  KEYWORD          "BSWAP",
  RESERVED         "BX",
  RESERVED         "BYTE",
  KEYWORD          "CALL",
  KEYWORD          "CBW",
  RESERVED         "CH",
  RESERVED         "CL",
  KEYWORD          "CLC",
  KEYWORD          "CLD",
  KEYWORD          "CLI",
  KEYWORD          "CLTS",
  KEYWORD          "CMC",
  KEYWORD          "CMP",
  KEYWORD          "CMPS",
  KEYWORD          "CMPSB",
  KEYWORD          "CMPSW",
  KEYWORD          "CMPXCHG",
  RESERVED         "CODEMACRO",
  RESERVED         "COMMON",
  RESERVED         "CS",
  KEYWORD          "CWD",
  RESERVED         "CX",
  KEYWORD          "DAA",
  KEYWORD          "DAS",
  RESERVED         "DB",
  RESERVED         "DD",
  KEYWORD          "DEC",
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
  RESERVED         "END",
  RESERVED         "ENDM",
  RESERVED         "ENDP",
  RESERVED         "ENDS",
  KEYWORD          "ENTER",
  RESERVED         "EO",
  RESERVED         "EQ",
  RESERVED         "EQU",
  RESERVED         "ER",
  RESERVED         "ES",
  KEYWORD          "ESC",
  RESERVED         "EVEN",
  RESERVED         "EXTRN",
  KEYWORD          "F2XM1",
  KEYWORD          "FABS",
  KEYWORD          "FADD",
  KEYWORD          "FADDP",
  RESERVED         "FAR",
  KEYWORD          "FBLD",
  KEYWORD          "FBSTP",
  KEYWORD          "FCHS",
  KEYWORD          "FCLEX",
  KEYWORD          "FCOM",
  KEYWORD          "FCOMP",
  KEYWORD          "FCOMPP",
  KEYWORD          "FDECSTP",
  KEYWORD          "FDISI",
  KEYWORD          "FDIV",
  KEYWORD          "FDIVP",
  KEYWORD          "FDIVR",
  KEYWORD          "FDIVRP",
  KEYWORD          "FENI",
  KEYWORD          "FFREE",
  KEYWORD          "FIADD",
  KEYWORD          "FICOM",
  KEYWORD          "FICOMP",
  KEYWORD          "FIDIV",
  KEYWORD          "FIDIVR",
  KEYWORD          "FILD",
  KEYWORD          "FIMUL",
  KEYWORD          "FINCSTP",
  KEYWORD          "FINIT",
  KEYWORD          "FIST",
  KEYWORD          "FISTP",
  KEYWORD          "FISUB",
  KEYWORD          "FISUBR",
  KEYWORD          "FLD",
  KEYWORD          "FLD1",
  KEYWORD          "FLDCW",
  KEYWORD          "FLDENV",
  KEYWORD          "FLDL2E",
  KEYWORD          "FLDL2T",
  KEYWORD          "FLDLG2",
  KEYWORD          "FLDLN2",
  KEYWORD          "FLDPI",
  KEYWORD          "FLDZ",
  KEYWORD          "FMUL",
  KEYWORD          "FMULP",
  KEYWORD          "FNCLEX",
  KEYWORD          "FNDISI",
  KEYWORD          "FNENI",
  KEYWORD          "FNINIT",
  KEYWORD          "FNOP",
  KEYWORD          "FNSAVE",
  KEYWORD          "FNSTCW",
  KEYWORD          "FNSTENV",
  KEYWORD          "FNSTSW",
  KEYWORD          "FPATAN",
  KEYWORD          "FPREM",
  KEYWORD          "FPTAN",
  KEYWORD          "FRNDINT",
  KEYWORD          "FRSTOR",
  KEYWORD          "FSAVE",
  KEYWORD          "FSCALE",
  KEYWORD          "FSETPM",
  KEYWORD          "FSQRT",
  KEYWORD          "FST",
  KEYWORD          "FSTCW",
  KEYWORD          "FSTENV",
  KEYWORD          "FSTP",
  KEYWORD          "FSTSW",
  KEYWORD          "FSUB",
  KEYWORD          "FSUBP",
  KEYWORD          "FSUBR",
  KEYWORD          "FSUBRP",
  KEYWORD          "FTST",
  KEYWORD          "FWAIT",
  KEYWORD          "FXAM",
  KEYWORD          "FXCH",
  KEYWORD          "FXTRACT",
  KEYWORD          "FYL2X",
  KEYWORD          "FYL2XP1",
  RESERVED         "GE",
  RESERVED         "GT",
  RESERVED         "HIGH",
  KEYWORD          "HLT",
  KEYWORD          "IDIV",
  KEYWORD          "IMUL",
  KEYWORD          "IN",
  KEYWORD          "INC",
  KEYWORD          "INS",
  KEYWORD          "INSB",
  KEYWORD          "INSW",
  KEYWORD          "INT",
  KEYWORD          "INTO",
  KEYWORD          "INVD",
  KEYWORD          "INVLPG",
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
  KEYWORD          "LAR",
  KEYWORD          "LDS",
  RESERVED         "LE",
  KEYWORD          "LEA",
  KEYWORD          "LEAVE",
  RESERVED         "LENGTH",
  KEYWORD          "LES",
  KEYWORD          "LGDT",
  KEYWORD          "LIDT",
  KEYWORD          "LLDT",
  KEYWORD          "LMSW",
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
  KEYWORD          "LSL",
  RESERVED         "LT",
  RESERVED         "MASK",
  RESERVED         "MOD",
  RESERVED         "MODRM",
  KEYWORD          "MOV",
  KEYWORD          "MOVS",
  KEYWORD          "MOVSB",
  KEYWORD          "MOVSW",
  KEYWORD          "MUL",
  RESERVED         "NAME",
  RESERVED         "NE",
  RESERVED         "NEAR",
  KEYWORD          "NEG",
  KEYWORD          "NIL",
  KEYWORD          "NOP",
  RESERVED         "NOSEGFIX",
  KEYWORD_RESERVED "NOT",
  RESERVED         "NOTHING",
  RESERVED         "OFFSET",
  KEYWORD_RESERVED "OR",
  RESERVED         "ORG",
  KEYWORD          "OUT",
  KEYWORD          "OUTS",
  KEYWORD          "OUTSB",
  KEYWORD          "OUTSW",
  RESERVED         "PREFIX66",
  RESERVED         "PREFIX67",
  RESERVED         "PREFX",
  KEYWORD          "POP",
  KEYWORD          "POPA",
  KEYWORD          "POPF",
  RESERVED         "PROC",
  RESERVED         "PROCLEN",
  RESERVED         "PTR",
  RESERVED         "PUBLIC",
  RESERVED         "PURGE",
  KEYWORD          "PUSH",
  KEYWORD          "PUSHA",
  KEYWORD          "PUSHF",
  RESERVED         "QWORD",
  KEYWORD          "RCL",
  KEYWORD          "RCR",
  RESERVED         "RECORD",
  RESERVED         "RELB",
  RESERVED         "RELD",
  RESERVED         "RELW",
  KEYWORD          "REP",
  KEYWORD          "REPE",
  KEYWORD          "REPNE",
  KEYWORD          "REPNZ",
  KEYWORD          "RET",
  RESERVED         "RO",
  KEYWORD          "ROL",
  KEYWORD          "ROR",
  RESERVED         "RW",
  KEYWORD          "SAHF",
  KEYWORD          "SAL",
  KEYWORD          "SAR",
  KEYWORD          "SBB",
  KEYWORD          "SCAS",
  KEYWORD          "SCASB",
  KEYWORD          "SCASW",
  RESERVED         "SEG",
  RESERVED         "SEGMENT",
  KEYWORD          "SETBE",
  KEYWORD          "SGDT",
  KEYWORD_RESERVED "SHL",
  RESERVED         "SHORT",
  KEYWORD_RESERVED "SHR",
  RESERVED         "SI",
  KEYWORD          "SIDT",
  RESERVED         "SIZE",
  KEYWORD          "SLDT",
  KEYWORD          "SMSW",
  RESERVED         "SP",
  RESERVED         "SS",
  RESERVED         "ST",
  RESERVED         "STACKSEG",
  RESERVED         "STACKSTART",
  KEYWORD          "STC",
  KEYWORD          "STD",
  KEYWORD          "STI",
  KEYWORD          "STOS",
  KEYWORD          "STOSB",
  KEYWORD          "STOSW",
  KEYWORD          "STR",
  RESERVED         "STRUC",
  KEYWORD          "SUB",
  RESERVED         "TBYTE",
  KEYWORD          "TEST",
  RESERVED         "THIS",
  RESERVED         "TR3",
  RESERVED         "TR4",
  RESERVED         "TR5",
  RESERVED         "TYPE",
  KEYWORD          "VERR",
  KEYWORD          "VERW",
  KEYWORD          "WAIT",
  RESERVED         "WARNING",
  KEYWORD          "WBINVD",
  RESERVED         "WC",
  RESERVED         "WIDTH",
  RESERVED         "WORD",
  KEYWORD          "XADD",
  KEYWORD          "XCHG",
  KEYWORD          "XLAT",
  KEYWORD          "XLATB",
  KEYWORD_RESERVED "XOR",
  SPECIAL          "[0-1]+B",
  SPECIAL          "[0-7]+O|Q",
  SPECIAL          "(-|+)?[0-9]+D?",
  SPECIAL          "([0-9][0-9A-F]*H)|([0-9][0-9A-F]*R)",
  SPECIAL          "((-|+)?[0-9]+([.][0-9]*)?E(+|-)?[0-9]+)|((-|+)?[0-9]+[.][0-9]*)",
  SPECIAL          "(['][^']*['])+",
  SPECIAL          "[A-Z_][A-Z_0-9]*",
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
