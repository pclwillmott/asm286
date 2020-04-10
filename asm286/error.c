/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    error.c       Display error message
 *
 *  This revision:
 *
 *    2020 April 10 Paul Willmott Segment Stack Errors added.
 *
 *  Revision History:
 *
 *    2019 November 17 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include "asm286.h"

void error( int err, int lineno )
{
  
  const char *message[ NUM_ERR ] = {
    "Internal error",
    "Out of memory",
    "Line buffer overflow",
    "Syntax error",
    "String too long",
    "Identifier too long",
    "Identifier already exists",
    "Symbol table full",
    "Segment Stack Overflow",
    "Too Many Segments",
    "Not In Segment",
    "Segment Nesting Fault",
  } ;
  
  char tmp_str[ 128 ] ;
  
/*
 *-----------------------------------------------------------------------------
 */
  
/*
 * Output error message.
 */
  
  if ( ( err > 0 ) && ( err <= NUM_ERR ) ) {
    sprintf ( tmp_str, "%s", message[ err - 1 ] ) ;
  }
  else {
    sprintf ( tmp_str, "Error %i", err ) ;
  }
  
  fprintf ( stderr, "%s", tmp_str ) ;
  
  if ( lineno > 0 ) {
    fprintf ( stderr, " at line %i", lineno ) ;
  }
    
  fprintf( stderr, "\n" ) ;
  
  return ;
  
/*
 * Finished
 */
  
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
