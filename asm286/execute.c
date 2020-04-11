/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    execute.c       Execute a production
 *
 *  This revision:
 *
 *    2019 November 17 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2019 November 17 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm286.h"

enum {
  SUCCESS = 1,
  FAIL = 0,
} ;

int execute( ptree_node_t *ptree, int pass, int lineno )
{
  
  unsigned int arg_index ;
  
//  extern unsigned int current_position, dep_count;
  
/*
 *-----------------------------------------------------------------------------
 */
  
/*
 * Only process productions.
 */
  
  if ( ptree->production_id < PRODUCTION_OFFSET )  {
    return SUCCESS ;
  }
  
/*
 * Evaluate all the nodes from the bottom up and left to right.
 */
  
  for ( arg_index = 0; arg_index < ptree->num_args; arg_index++ ) {
    if ( ! execute ( ptree->args[ arg_index ], pass, lineno ) ) {
      return FAIL ;
    }
  }
  
/*
 * Skip non-action productions.
 */
  
  if ( ptree->exec_type == XT_IGNORE ) {
    return SUCCESS ;
  }
  
/*
 * If production is not an action production then drop through
 * value of the first argument.
 */
  
  if ( ptree->exec_type == XT_DROP ) {
    
    ptree->value_type = ptree->args[ 0 ]->value_type ;
    
    if ( ptree->value_type == TOK_STRING ) {
      ptree->value.s = NULL ;
      if ( ! setstr ( &ptree->value.s, ptree->args[ 0 ]->value.s ) ) {
        return FAIL ;
      }
      
    }
    else {
      CPYVALUE( ptree->value, ptree->args[ 0 ]->value ) ;
    }
    
    return SUCCESS ;
    
  }
  
/*
 * Jump Table
 */

//  printf("execute: pid = %i ", ptree->production_id);
  
  int operator, result, i1 = 0, i2 = 0;
  ptree_node_t *arg1, *arg2 ;
  char *str ;
  int pass1 = (pass == 0);
  int pass2 = (pass == 1);
  unsigned int pos;

  switch ( ptree->production_id ) {
    case PRD_WARNING:
      if (pass2) {
        ptree->value.i = ptree->args[1]->value.i;
        printf("PRD_WARNING: %i\n", ptree->value.i);
      }
      break;
    case PRD_DBITEM:
      operator = ptree->args[ 0 ]->value_type ;
      switch (operator) {
        case TOK_INTEGERDEC:
          if (dep((unsigned char)ptree->args[ 0 ]->value.i, pass, lineno)) {
            return FAIL;
          }
          break;
        case TOK_STRING:
          for (str = ptree->args[ 0 ]->value.s; *str; str++) {
            if (dep((unsigned char)*str, pass, lineno)) {
              return FAIL;
            }
          }
          break;
        default:
          if ( ptree->args[ 0 ]->production_id == TOK_QMARK) {
            if (dep(0x00, pass, lineno)) {
              return FAIL;
            }
          }
          break;
      }
      break;
    case PRD_DWITEM:
      if ( ptree->args[ 0 ]->production_id == TOK_QMARK) {
        if (depw(0x0000, pass, lineno)) {
          return FAIL;
        }
      }
      else {
        if (depw((unsigned short)ptree->args[ 0 ]->value.i, pass, lineno)) {
          return FAIL;
        }
      }
      break;
    case PRD_DDITEM:
      if ( ptree->args[ 0 ]->production_id == TOK_QMARK) {
        if (depd(0x00000000, pass, lineno)) {
          return FAIL;
        }
      }
      else {
        if (depd((unsigned int)ptree->args[ 0 ]->value.i, pass, lineno)) {
          return FAIL;
        }
      }
      break;
    case PRD_VAR_NAME:
      if (pass1) {
        if (get_current_position(&pos, lineno)) {
          return FAIL;
        }
        if (add_symbol(ptree->args[0]->value.s, SYMBOL_VARIABLE, pos)) {
          return FAIL;
        }
      }
      break;
    case PRD_SEGMENT:
      if (open_segment(ptree->args[0]->value.s, lineno)) {
        return FAIL;
      }
      break;
    case PRD_ENDS:
      if (close_segment(ptree->args[0]->value.s, lineno)) {
        return FAIL;
      }
      break;
    case PRD_ORG:
      if (set_current_position(ptree->args[1]->value.i, lineno)) {
        return FAIL;
      }
      break;
    case PRD_INST_LABEL:
      if (pass1) {
        if (get_current_position(&pos, lineno)) {
          return FAIL;
        }
        if (add_symbol(ptree->args[0]->value.s, SYMBOL_LABEL, pos)) {
          return FAIL;
        }
      }
      break;
    case PRD_CON_NUM:
      ptree->value_type = ptree->args[0]->value_type;
      ptree->value.i = ptree->args[0]->value.i;
      break;
    case PRD_GRP0_EXP:
      ptree->value_type = TOK_INTEGERDEC;
      switch (ptree->variant) {
        case 1:
          ptree->value.i = ptree->args[1]->value.i;
          break;
        case 2:
          if (pass2) {
            if (get_symbol_value(ptree->args[0]->value.s, &i1)) {
              return FAIL;
            }
          }
          ptree->value.i = i1;
          break;
        case 3:
          if (get_current_position((unsigned int *)&ptree->value.i, lineno)) {
            return FAIL;
          }
          break;
      }
      break;
    case PRD_GRP7_EXP:
      ptree->value_type = ptree->args[1]->value_type;
      ptree->value.i = ~ ptree->args[1]->value.i;
      break;
    case PRD_GRP4_EXP:
    case PRD_GRP5_EXP:
    case PRD_GRP6_EXP:
    case PRD_GRP8_EXP:
    case PRD_GRP9_EXP:
      operator = ptree->args[ 1 ]->args[ 0 ]->production_id ;
      arg1 = ptree->args[ 0 ] ;
      arg2 = ptree->args[ 2 ] ;
      i1 = arg1->value.i;
      i2 = arg2->value.i;
      switch (operator) {
        case TOK_MULTIPLY:
          result = i1 * i2;
          break;
        case TOK_DIVIDE:
          result = i1 / i2;
          break;
        case TOK_MOD:
          result = i1 % i2;
          break;
        case TOK_SHL:
          result = i1 << i2;
          break;
        case TOK_SHR:
          result = i1 >> i2;
          break;
        case TOK_PLUS:
          result = i1 + i2;
          break;
        case TOK_MINUS:
          result = i1 - i2;
          break;
        case TOK_AND:
          result = i1 & i2;
          break;
        case TOK_OR:
          result = i1 | i2;
          break;
        case TOK_XOR:
          result = i1 ^ i2;
          break;
        case TOK_EQ:
          result = (i1 == i2) ? 0 : -1;
          break;
        case TOK_NE:
          result = (i1 != i2) ? 0 : -1;
          break;
        case TOK_LT:
          result = (i1 < i2) ? 0 : -1;
          break;
        case TOK_GT:
          result = (i1 > i2) ? 0 : -1;
          break;
        case TOK_LE:
          result = (i1 <= i2) ? 0 : -1;
          break;
        case TOK_GE:
          result = (i1 >= i2) ? 0 : -1;
          break;
        default:
          result = 0;
          break;
      }
      ptree->value_type = arg1->value_type;
      ptree->value.i = result;
//      printf("value %i\n", ptree->value.i);
      break;
    case PRD_SIMPLE:
      {
        const unsigned char simple[][3] = {
          {1, 0x37, 0x00}, // AAA
          {2, 0xd5, 0x0a}, // AAD
          {2, 0xd4, 0x0a}, // AAM
          {1, 0x3f, 0x00}, // AAS
          {1, 0x98, 0x00}, // CBW
          {1, 0xf8, 0x00}, // CLC
          {1, 0xfc, 0x00}, // CLD
          {1, 0xfa, 0x00}, // CLI
          {2, 0x0f, 0x06}, // CLTS
          {1, 0xf5, 0x00}, // CMC
          {1, 0x99, 0x00}, // CWD
          {1, 0x27, 0x00}, // DAA
          {1, 0x2f, 0x00}, // DAS
          {1, 0xf4, 0x00}, // HLT
          {1, 0xcf, 0x00}, // IRET
          {1, 0x9f, 0x00}, // LAHF
          {1, 0xc9, 0x00}, // LEAVE
          {1, 0x90, 0x00}, // NOP
          {1, 0x61, 0x00}, // POPA
          {1, 0x9d, 0x00}, // POPF
          {1, 0x60, 0x00}, // PUSHA
          {1, 0x9c, 0x00}, // PUSHF
          {1, 0x9e, 0x00}, // SAHF
          {1, 0xf9, 0x00}, // STC
          {1, 0xfd, 0x00}, // STD
          {1, 0xfb, 0x00}, // STI
          {1, 0x9b, 0x00}, // WAIT
        };
        for (int i = 1; i < simple[ptree->variant][0] + 1; i++) {
          if (dep(simple[ptree->variant][i], pass, lineno)) {
            return FAIL;
          }
        }
 //       printf("\n");
      }
      break;
  }

  return SUCCESS ;
  
/*
 * Finished
 */
  
}

/*
 *-----------------------------------------------------------------------------
 * This routine sets a string value.
 *-----------------------------------------------------------------------------
 */

int setstr
(
 char **str,
 char *value
)
{
  
  /*
   *-----------------------------------------------------------------------------
   */
  
  if ( (*str) != NULL ) {
//    free ( *str ) ;
  }
  
  if ( ( (*str) = ( char * ) malloc ( strlen ( value ) + 1 ) ) == NULL ) {
    error ( ERR_OUT_OF_MEMORY, 0 ) ;
    return FAIL ;
  }
  
  strcpy ( (*str), value ) ;
  
  return SUCCESS ;
  
  /*
   * Finished
   */
  
}


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
