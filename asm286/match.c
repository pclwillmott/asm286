/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    match.c       Match a production to a list of tokens.
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

#include <stdlib.h>
#include "asm286.h"

ptree_node_t * match
(
 int production_id,
 tlist_node_t **toklst,
 int start_arg
)
{
  
  return NULL ;
  
}

void delete_ptree
(
 ptree_node_t *ptree,
 int kill_root,
 int kill_strings,
 int start_arg
)
{
  
  int arg_index ;
  
  /*
   *-----------------------------------------------------------------------------
   */
  
  if ( ptree->production_id >= PRODUCTION_OFFSET ) {
    
    for ( arg_index = start_arg; arg_index < ptree->num_args; arg_index++ ) {
      delete_ptree ( ptree->args[ arg_index ], 1, kill_strings, 0 ) ;
    }
    /*
     if ( ( kill_strings ) && ( ptree->value_type == to_char ( TOK_STRING ) ) ) {
     free ( ptree->value.s ) ;
     }
     */
    
    if ( ptree->num_args ) {
      free ( ptree->args ) ;
    }
    
  }
  
  if ( kill_root ) {
    free ( ptree ) ;
  }
  
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
