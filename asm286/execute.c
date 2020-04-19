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
 *    2020 April 11 Paul Willmott Relative Jumps added.
 *
 *  Revision History:
 *
 *    2020 April 10 Paul Willmott ORG and $ added.
 *
 *    2019 November 17 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm286.h"

enum {
  SUCCESS = 1,
  FAIL    = 0,
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

  int result, i1 = 0, i2 = 0;
  char *str ;
  int pass1 = (pass == 0);
  int pass2 = (pass == 1);
  unsigned int pos;

  unsigned char op2 = 0;
  
  unsigned char opcode[6] ;
  int opcode_count = 0;
  
  int literal[2] ;
  int literal_word[2] ;
  int literal_count = 0;
  
  int disp = 0;

  switch ( ptree->production_id ) {
    case PRD_ARPL:
      {
        opcode[opcode_count++] = 0x63;
        if (ptree->variant == 0) {
          op2 = rm_disp_mod(ptree->args[1], &disp);
        }
        else {
          op2 = 0xc0 | ptree->args[1]->variant;
        }
        opcode[opcode_count++] = op2 | (ptree->args[3]->variant << 3);
      }
      break;
    case PRD_BOUND:
      {
        opcode[opcode_count++] = 0x62;
        op2 = rm_disp_mod(ptree->args[3], &disp) | (ptree->args[1]->variant << 3) ;
        if (op2 == 0x0e) {
          op2 = 0x4e;
          disp = 0;
        }
        opcode[opcode_count++] = op2;
      }
      break;
    case PRD_ALU:
      {
        unsigned char w = 0x01;
        switch (ptree->variant) {
          case 0: // rb, num
            w = 0x00;
          case 1: // rw, num
            opcode[opcode_count++] = (ptree->args[0]->variant << 3) | 0x04 | w;
            literal_word[literal_count] = w;
            literal[literal_count++] = ptree->args[3]->value.i;
           break;
          case 2: // rb, rb
            w = 0x00;
          case 3: // rw, rw
            opcode[opcode_count++] = (ptree->args[0]->variant << 3) | 0x02 | w;
            opcode[opcode_count++] = 0xc0 | (ptree->args[1]->variant << 3) | ptree->args[3]->variant ;
            break;
          case 4: // rb, rm_disp
            w = 0x00;
          case 5: // rw, rm_disp
            {
              opcode[opcode_count++] = (ptree->args[0]->variant << 3) | 0x02 | w;
              op2 = rm_disp_mod(ptree->args[3], &disp) | (ptree->args[1]->variant << 3);
              if (op2 == 0x06) {
                op2 = 0x46;
                disp = 0;
              }
              opcode[opcode_count++] = op2;
            }
            break;
          case 6: // rm_disp, rb
            w = 0x00;
          case 7: // rm_disp, rw
            opcode[opcode_count++] = (ptree->args[0]->variant << 3) | 0x00 | w;
            op2 = rm_disp_mod(ptree->args[1], &disp) | (ptree->args[3]->variant << 3);
            if (op2 == 0x06) {
              op2 = 0x46;
              disp = 0;
            }
            opcode[opcode_count++] = op2;
           break;
          case 8: // rb, num
            w = 0x00;
          case 9: // rw, num
            opcode[opcode_count++] = 0x80 | w;
            opcode[opcode_count++] = 0xc0 | (ptree->args[0]->variant << 3) | ptree->args[1]->variant ;
            literal_word[literal_count] = w;
            literal[literal_count++] = ptree->args[3]->value.i;
            break;
          case 10: // rm_disp, num
            {
              literal[literal_count] = ptree->args[3]->value.i;
              w = is_word(literal[literal_count]);
              literal_word[literal_count++] = w;
              opcode[opcode_count++] = 0x80 | ((w) ? 0x01 : 0x03 );
              op2 = rm_disp_mod(ptree->args[1], &disp) | (ptree->args[0]->variant << 3);
              if (op2 == 0x36) {
                op2 = 0x76;
                disp = 0;
              }
              opcode[opcode_count++] = op2;
            }
            break;
        }
      }
      break;
    case PRD_WARNING:
      if (pass2) {
        ptree->value.i = ptree->args[1]->value.i;
        printf("PRD_WARNING: %i\n", ptree->value.i);
      }
      break;
    case PRD_DBITEM:
      switch (ptree->args[ 0 ]->value_type) {
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
      i1 = ptree->args[0]->value.i;
      i2 = ptree->args[2]->value.i;
      switch (ptree->args[1]->args[0]->production_id) {
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
      ptree->value_type = TOK_INTEGERDEC;
      ptree->value.i = result;
      break;
    case PRD_JR:
      {
        const unsigned char jr[] = {
          0x70,
          0x71,
          0x72,
          0x72,
          0x72,
          0x73,
          0x73,
          0x73,
          0x74,
          0x74,
          0x75,
          0x75,
          0x76,
          0x76,
          0x77,
          0x77,
          0x78,
          0x79,
          0x7a,
          0x7a,
          0x7b,
          0x7b,
          0x7c,
          0x7c,
          0x7d,
          0x7d,
          0x7e,
          0x7e,
          0x7f,
          0x7f,
          0xe3,
        };
        opcode[opcode_count++] = jr[ptree->args[0]->variant];
        int cp = 0;
        if (get_current_position((unsigned int *)&cp, lineno)) {
          return FAIL;
        }
        opcode[opcode_count++] = (unsigned char)((signed char)(ptree->args[1]->value.i - cp - 2));
      }
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
          opcode[opcode_count++] = simple[ptree->variant][i];
        }
      }
      break;
  }

  // Do the code deposits
  
  if (dep_disp(opcode, opcode_count, disp, pass, lineno)) {
    return FAIL;
  }
  
  for (int i = 0; i < literal_count; i++) {
    if (literal_word[i]) {
      if (depw((unsigned short)literal[i], pass, lineno)) {
        return FAIL;
      }
    }
    else {
      if (dep((unsigned char)literal[i], pass, lineno)) {
        return FAIL;
      }
    }
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

int dep_disp(unsigned char opcodes[], int opcode_count, int disp, int pass, int lineno)
{
  if (dep_opcodes(opcodes, opcode_count, pass, lineno)) {
    return 1;
  }
  if (opcode_count > 1) {
    switch ((opcodes[1] >> 6) & 0x03) {
      case 0x01:
        return dep((unsigned char)disp, pass, lineno);
      case 0x02:
        return depw((unsigned short)disp, pass, lineno);
    }
  }
  return 0;
}

unsigned char rm_disp_mod(ptree_node_t *ptree, int *disp)
{
  unsigned char opcode2 = 0;
  if (ptree->variant == 0) {
    *disp = ptree->args[2]->value.i;
    if (is_word(*disp)) {
      opcode2 = 0x02 << 6;
    }
    else {
      opcode2 = 0x01 << 6;
    }
  }
  opcode2 |= ptree->args[0]->variant;
  return opcode2;
}

int dep_opcodes(unsigned char opcodes[], int opcode_count, int pass, int lineno)
{
  for (int i = 0; i < opcode_count; i++) {
    if (dep(opcodes[i], pass, lineno)) {
      return 1;
    }
  }
  return 0;
}

int is_word(int value)
{
  return ( value < -128 || value > 255);
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
