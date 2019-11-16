/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    action.c       Production decoder
 *
 *  This revision:
 *
 *    2019 November 10 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2019 November 10 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include "asm286.h"

/*
 * Production Table
 */

#define PT_IGNORE      "\001"
#define PT_DROP        "\002"
#define PT_EXECUTE     "\003"

char *prdlst[] = {
  
  /*
   * Numerical Operators
   */

  SPD_GRP1_OP     PT_IGNORE       STK_LENGTH,
  SPD_GRP1_OP     PT_IGNORE       STK_SIZE,
  SPD_GRP1_OP     PT_IGNORE       STK_WIDTH,
  SPD_GRP1_OP     PT_IGNORE       STK_MASK,
  SPD_GRP1_OP     PT_IGNORE       STK_STACKSTART,

  
  SPD_GRP2_OP     PT_IGNORE       STK_PTR,
  SPD_GRP2_OP     PT_IGNORE       STK_OFFSET,
  SPD_GRP2_OP     PT_IGNORE       STK_BITOFFSET,
  SPD_GRP2_OP     PT_IGNORE       STK_SEG,
  SPD_GRP2_OP     PT_IGNORE       STK_TYPE,
  SPD_GRP2_OP     PT_IGNORE       STK_THIS,

  
  SPD_GRP3_OP     PT_IGNORE       STK_HIGHW,
  SPD_GRP3_OP     PT_IGNORE       STK_LOWW,
  SPD_GRP3_OP     PT_IGNORE       STK_HIGH,
  SPD_GRP3_OP     PT_IGNORE       STK_LOW,

  SPD_GRP4_OP     PT_IGNORE       STK_MULTIPLY,
  SPD_GRP4_OP     PT_IGNORE       STK_DIVIDE,
  SPD_GRP4_OP     PT_IGNORE       STK_MOD,
  SPD_GRP4_OP     PT_IGNORE       STK_SHL,
  SPD_GRP4_OP     PT_IGNORE       STK_SHR,
  
  SPD_GRP5_OP     PT_IGNORE       STK_PLUS,
  SPD_GRP5_OP     PT_IGNORE       STK_MINUS,

  SPD_GRP6_OP     PT_IGNORE       STK_EQ,
  SPD_GRP6_OP     PT_IGNORE       STK_NE,
  SPD_GRP6_OP     PT_IGNORE       STK_LT,
  SPD_GRP6_OP     PT_IGNORE       STK_GT,
  SPD_GRP6_OP     PT_IGNORE       STK_LE,
  SPD_GRP6_OP     PT_IGNORE       STK_GE,

  SPD_GRP7_OP     PT_IGNORE       STK_NOT,

  SPD_GRP8_OP     PT_IGNORE       STK_AND,
  
  SPD_GRP9_OP     PT_IGNORE       STK_OR,
  SPD_GRP9_OP     PT_IGNORE       STK_XOR,
  
  SPD_GRP10_OP    PT_IGNORE       STK_SHORT,

  /*
   * Variables and Constants
   */
  
  SPD_CON_NUM     PT_DROP         STK_DOUBLE,
  
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERBIN,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGEROCT,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERDEC,
  SPD_CON_NUM     PT_EXECUTE      STK_INTEGERHEX,

  SPD_CON_STR     PT_DROP         STK_STRING,
  
  /*
   * Numerical Expressions
   */
  
  SPD_GRP0_EXP    PT_DROP         SPD_CON_NUM,
  SPD_GRP0_EXP    PT_EXECUTE      STK_OBRACE      SPD_NUM_EXP     STK_CBRACE,
  
  SPD_GRP1_EXP    PT_EXECUTE      SPD_GRP1_EXP    SPD_GRP1_OP     SPD_GRP0_EXP,
  SPD_GRP1_EXP    PT_DROP         SPD_GRP0_EXP,
  
  SPD_GRP2_EXP    PT_EXECUTE      SPD_GRP2_EXP    SPD_GRP2_OP     SPD_GRP1_EXP,
  SPD_GRP2_EXP    PT_DROP         SPD_GRP1_EXP,
  
  SPD_GRP3_EXP    PT_EXECUTE      SPD_GRP3_OP     SPD_GRP2_EXP,
  SPD_GRP3_EXP    PT_DROP         SPD_GRP2_EXP,
  
  SPD_GRP4_EXP    PT_EXECUTE      SPD_GRP4_EXP    SPD_GRP4_OP     SPD_GRP3_EXP,
  SPD_GRP4_EXP    PT_DROP         SPD_GRP3_EXP,
  
  SPD_GRP5_EXP    PT_EXECUTE      SPD_GRP5_EXP    SPD_GRP5_OP     SPD_GRP4_EXP,
  SPD_GRP5_EXP    PT_DROP         SPD_GRP4_EXP,
  
  SPD_GRP6_EXP    PT_EXECUTE      SPD_GRP6_EXP    SPD_GRP6_OP     SPD_GRP5_EXP,
  SPD_GRP6_EXP    PT_DROP         SPD_GRP5_EXP,
  
  SPD_GRP7_EXP    PT_EXECUTE      SPD_GRP7_OP     SPD_GRP6_EXP,
  SPD_GRP7_EXP    PT_DROP         SPD_GRP6_EXP,
  
  SPD_GRP8_EXP    PT_EXECUTE      SPD_GRP8_EXP    SPD_GRP8_OP     SPD_GRP7_EXP,
  SPD_GRP8_EXP    PT_DROP         SPD_GRP7_EXP,
  
  SPD_GRP9_EXP    PT_EXECUTE      SPD_GRP9_EXP    SPD_GRP9_OP     SPD_GRP8_EXP,
  SPD_GRP9_EXP    PT_DROP         SPD_GRP8_EXP,
  
  SPD_NUM_EXP     PT_EXECUTE      SPD_NUM_EXP     SPD_GRP10_OP    SPD_GRP9_EXP,
  SPD_NUM_EXP     PT_DROP         SPD_GRP9_EXP,
  
  
  SPD_LAST
  
} ;

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
