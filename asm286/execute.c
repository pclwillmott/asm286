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
#include "asm286.h"

enum {
  SUCCESS = 1,
  FAIL = 0,
} ;

int execute( ptree_node_t *ptree )
{
  
  unsigned int arg_index ;
  
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
    if ( ! execute ( ptree->args[ arg_index ] ) ) {
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
//      if ( ! setstr ( &ptree->value.s, ptree->args[ 0 ]->value.s ) ) {
//        return FAIL ;
//      }
      
    }
    else {
      CPYVALUE( ptree->value, ptree->args[ 0 ]->value ) ;
    }
    
    return SUCCESS ;
    
  }
  
/*
 * Jump Table
 */

  printf("execute: pid = %i ", ptree->production_id);
  
  switch ( ptree->production_id ) {
    case PRD_WARNING:
      printf("PRD_WARNING: Result = %i\n", ptree->value.i);
      break;
    case PRD_CON_NUM:
      printf("PRD_CON_NUM: Result = %i\n", ptree->args[0]->value.s);
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
          dep(simple[ptree->variant][i]);
        }
        printf("\n");
      }
      break;
  }

  return SUCCESS ;
  
/*
 * Finished
 */
  
}

void dep(unsigned char db)
{
  printf("%02x ",db);
}

void depw(unsigned short dw)
{
  dep( dw & 0xff ) ;
  dep( dw >> 8 ) ;
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
