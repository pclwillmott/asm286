/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    asm286.c       Main
 *
 *  This revision:
 *
 *    2020 April 10 Paul Willmott Two Passes added and test for unended segment.
 *
 *  Revision History:
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "asm286.h"

int main(int argc, const char * argv[]) {
  
  extern long int maxPos;
  
/*
 *------------------------------------------------------------------------------
 */

//  dump_pattern();

  processor = i86;
  coprocessor = none;
  
  for (int pass = 0; pass < 2; pass++) {
    maxPos = 0;
    check_instructions = pass == 1;
    printf("Pass %i\n",pass+1);
    reset_for_pass(pass);
    if ( process("/Users/paul/Documents/Projects/LEGACY/asm286/EXAMPLE 2.ASM", pass ) ) {
      goto fail;
    }
    if (segment_stack_count()) {
      errno = ERR_SEGMENT_NOT_ENDED;
      goto fail;
    }
  }
  
  dump_symbol_table();

fail:
  
/*
 * Finished.
 */
  
  return 0 ;
  
}

/*
 * This routine assembles a single file.
 * It returns 0 on success and -1 on failure.
 */

long int maxPos = 0L;

int process(const char *filename, int pass) {
  
  FILE *fp = NULL ;
  
  int result = -1;

  ptree_node_t *pt ;

/*
 *------------------------------------------------------------------------------
 */
  
  if ( ( fp = fopen(filename, "r")) == NULL ) {
    goto fail;
  }
/*
  fseek(fp, 0L, SEEK_END);
  long int end_of_file = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
*/
  if ( ( ( pt = match2 ( PRD_module, fp, 0 ) ) == NULL )/* || ( tp != NULL ) */ ) {
    if (errno == 0) {
      errno = ERR_SYNTAX_ERROR;
    }
    goto fail ;
  }
  
  /*
   * Execute command.
   */
  
  else if ( ! execute ( pt, pass, 0 ) ) {
    goto fail ;
  }
  
  /*
   * Tidy-Up
   */
  
  if ( pt != NULL ) {
    delete_ptree ( pt, 1, 1, 0 ) ;
  }

  /*
   * Tidy-Up.
   */
  
  result = 0 ;
  
fail:
  
  if (result) {
    error();
    char buffer[256];
    long int x = maxPos - 10;
    if (x <0L) {
      x = 0L;
    }
    fseek(fp, x, SEEK_SET);
    unsigned long n = fread(&buffer, 1, 20, fp);
    buffer[n] = '\0';
    printf(">%s<\n", buffer);
  }
  
  if ( fp != NULL ) {
    fclose(fp) ;
  }
  
  /*
   * Finished.
   */
  
  return result ;

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
