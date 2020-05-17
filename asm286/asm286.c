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

  dump_pattern();

  for (int pass = 0; pass < 2; pass++) {
    maxPos = 0;
    printf("Pass %i\n",pass+1);
    reset_for_pass(pass);
    if ( process2("/Users/paul/Documents/Projects/LEGACY/asm286/EXAMPLE 2.ASM", pass ) ) {
      goto fail;
    }
    if (segment_stack_count()) {
      error(ERR_SEGMENT_NOT_ENDED,0);
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

#ifdef ROCKY

int process(const char *filename, int pass) {
  
  int result = -1, is_continue = 0, in_string, has_string ;
  
  unsigned long idx, len ;
  
  FILE *fp = NULL ;
  
  #define MAX_LINE (256)
  
  char linebuf[MAX_LINE], statement[MAX_LINE] ;
  
  char *wptr ;
  
  unsigned int line_number = 1, stmt_line = 1 ;

/*
 *------------------------------------------------------------------------------
 */
  
  if ( ( fp = fopen(filename, "r")) == NULL ) {
    goto fail;
  }
  
  strcpy(statement, "") ;
  
  while ( fgets(linebuf, sizeof(linebuf), fp) ) {
    
    char *ptr = linebuf ;
    
/*
 * If it is not a continuation line then assemble the statement.
 */
    
    if ( ! ( is_continue = ( linebuf[0] == '&' ) ) ) {
      if ( strlen(statement) > 0 ) {
        if ( assemble(statement, stmt_line, pass ) ) {
          goto fail ;
        }
        strcpy(statement, "") ;
        stmt_line = line_number ;
      }
    }
    
/*
 * Remove newline.
 */
    
    len = strlen(linebuf) ;
    
    if ( linebuf[len-1] == '\n' ) {
      linebuf[--len] = '\0' ;
    }
    
 /*
  * Strip comments from new line.
  */
    
    in_string = 0 ;
    has_string = 0 ;
    
    for ( idx = 0; idx < len; idx++ ) {
      
      char c = linebuf[idx];
      
      int is_quote = ( c == '\'' ) ;
      
      if ( in_string ) {
        if ( is_quote ) {
          in_string = 0 ;
        }
      }
      else if ( is_quote ) {
        in_string = 1 ;
        has_string = 1 ;
      }
      else if ( c == ';' ) {
        linebuf[idx] = '\0' ;
        break;
      }
      
    }
    
/*
 * Line continuation and string concatenation.
 */
    
    if ( is_continue ) {
      
      char *eptr = &statement[strlen(statement)-1] ;
      
      ptr++ ;

      if ( *ptr == ',' ) {
        
        ptr++ ;
        
        while ( *ptr && isspace(*ptr) ) {
          ptr++ ;
        }
        
        if ( *eptr == '\'' && *ptr == '\'' ) {
          *eptr = '\0' ;
          ptr++ ;
        }
        
      }
      
    }
    
/*
 * Remove trailing whitespace.
 */
    
    wptr = &linebuf[strlen(linebuf)-1] ;
    
    while ( wptr > ptr && isspace(*wptr)) {
      *wptr-- = '\0' ;
    }
    
    strcat( statement, ptr ) ;
    
    line_number++ ;
    
  }

  if ( strlen(statement) > 0 ) {
    if ( assemble(statement, stmt_line, pass ) ) {
      goto fail ;
    }
  }

/*
 * Tidy-Up.
 */
  
  result = 0 ;
  
fail:
  
  if ( fp != NULL ) {
    fclose(fp) ;
  }
  
/*
 * Finished.
 */
  
  return result ;
  
}

#endif

/*
 * This routine assembles a single file.
 * It returns 0 on success and -1 on failure.
 */

long int maxPos = 0L;

int process2(const char *filename, int pass) {
  
  FILE *fp = NULL ;
  
  int result = -1;

  ptree_node_t *pt ;

/*
 *------------------------------------------------------------------------------
 */
  
  if ( ( fp = fopen(filename, "r")) == NULL ) {
    goto fail;
  }
  
  fseek(fp, 0L, SEEK_END);
  long int end_of_file = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  
  while (ftell(fp)<end_of_file) {
    
//    printf("process2\n");
    if ( ( ( pt = match2 ( PRD_module, fp, 0 ) ) == NULL )/* || ( tp != NULL ) */ ) {
      if (ftell(fp)==end_of_file) {
        break;
      }
      error ( ERR_SYNTAX_ERROR, 0 ) ;
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

  }
  
  /*
   * Tidy-Up.
   */
  
  result = 0 ;
  
fail:
  
  if (result) {
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
