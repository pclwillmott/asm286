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
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "asm286.h"

int main(int argc, const char * argv[]) {

/*
 *------------------------------------------------------------------------------
 */

  dump_pattern();

  if ( process("/Users/paul/Documents/Projects/LEGACY/asm286/EXAMPLE.ASM" ) ) {
    goto fail;
  }

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

int process(const char *filename) {
  
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
        if ( assemble(statement, stmt_line ) ) {
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
    if ( assemble(statement, stmt_line ) ) {
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
