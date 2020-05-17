/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    assemble.c       Assemble a single statement
 *
 *  This revision:
 *
 *    2019 November 17 Paul Willmott Error handling added.
 *
 *  Revision History:
 *
 *    2019 November 10 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include "asm286.h" 

#ifdef ROCKY
int assemble( const char *statement, int lineno, int pass ) {

  int result = -1 ;
  
  tlist_node_t *tlist = NULL, *tp ;

  ptree_node_t *pt ;
  
//  extern unsigned int dep_count;
  
/*
 *------------------------------------------------------------------------------
 */

//  dep_count = 0;
  
//  printf("assemble: 1\n");
  if ( ( tlist = tokenize( statement, lineno ) ) == NULL ) {
    goto fail ;
  }

/*
 * Try and find a match.
 */
  
//  printf("assemble: 2\n");
  tp = tlist ;
  
  if ( ( ( pt = match ( PRD_STMT, &tp, 0 ) ) == NULL ) || ( tp != NULL ) ) {
    error ( ERR_SYNTAX_ERROR, lineno ) ;
    goto fail ;
  }
  
/*
 * Execute command.
 */
  
  else if ( ! execute ( pt, pass, lineno ) ) {
    goto fail ;
  }
//  printf("assemble: 3\n");

/*
 * Tidy-Up
 */
  
  if ( pt != NULL ) {
    delete_ptree ( pt, 1, 1, 0 ) ;
  }
  
  delete_tlist ( &tlist ) ;

/*
 * Tidy-Up.
 */
  
  result = 0 ;
  
fail:
  
  delete_tlist( &tlist ) ;
  
/*
 * Finished.
 */
  
  return result ;
  
}

#endif

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
