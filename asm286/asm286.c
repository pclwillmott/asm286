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

char *fname[2];

int ppModified = 0;

int main(int argc, const char * argv[]) {
  
  extern long int maxPos;
  
  extern char *fname[];
  
  extern int ppModified;

/*
 *------------------------------------------------------------------------------
 */

//  dump_pattern();

  processor = i86;
  coprocessor = none;
  
  fname[0] = "/Users/paul/Documents/Projects/LEGACY/asm286/EXAMPLE 2.ASM";
  fname[1] = "/Users/paul/Documents/Projects/LEGACY/asm286/TEMP1.ASM";
  
  add_numbers(fname);
  
  fname[0] = fname[1];
  fname[1] = "/Users/paul/Documents/Projects/LEGACY/asm286/TEMP2.ASM";
  
  do {
    
    ppModified = 0;
    
    in_preprocessor = -1;
    
    if (processPP(PRD_ppItemList)) {
      goto fail;
    }
  
    char *temp = fname[0];
    fname[0] = fname[1];
    fname[1] = temp;
    
    in_preprocessor = 0;
    ifCount = 0;
    assembleIt = 1;

    if (processPP(PRD_ppTokenSequenceList)) {
      goto fail;
    }
    
    if (ppModified) {
      char *temp = fname[0];
      fname[0] = fname[1];
      fname[1] = temp;
    }
    
  } while (ppModified);
  
  in_preprocessor = 0;
  
  for (int pass = 0; pass < 2; pass++) {
    ifCount = 0;
    assembleIt = 1;
    maxPos = 0;
    check_instructions = pass == 1;
    printf("Pass %i\n",pass+1);
    reset_for_pass(pass);
    if ( process(fname[1], pass ) ) {
      goto fail;
    }
  }
  
  if (list) {
    
    if (module_name != NULL) {
      printf("MODULE: %s\n", module_name); 
    }
    if (title != NULL) {
      printf("%s\n", title);
    }
    if (subtitle != NULL) {
      printf("%s\n", subtitle);
    }
    dump_symbol_table();
    
    dump_segment_table();
    
  }
  
fail:
  
/*
 * Finished.
 */
  
  return 0 ;
  
}

FILE *ppFP2 = NULL ;

int processPP(int production_id) {
  
  extern char *fname[];
  
  int result = -1;
  
  FILE *ppFP1 = NULL;
  
  extern FILE *ppFP2 ;
  

  ptree_node_t *pt ;
  
  /*
   *------------------------------------------------------------------------------
   */
  
  if ( ( ppFP1 = fopen(fname[0], "r")) == NULL ) {
    goto fail;
  }
  
  if ( ( ppFP2 = fopen(fname[1], "w")) == NULL ) {
    goto fail;
  }
  
  if ((pt = match2 (production_id, ppFP1, 0)) == NULL) {
    
    if (errno == 0) {
      errno = ERR_SYNTAX_ERROR;
    }
    goto fail ;
  }
  
  /*
   * Execute command.
   */
  
  else if ( ! execute ( pt, -1, 0 ) ) {
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
  
  if ( ppFP1 != NULL ) {
    fclose(ppFP1) ;
  }

  if ( ppFP2 != NULL ) {
    fclose(ppFP2) ;
  }
  
  return result;
  
}

int add_numbers(char *fname[]){
  
  if (fname[0] == NULL || fname[1] == NULL) {
    return -1;
  }
  
  FILE *fp1 = NULL, *fp2 = NULL;
  
  if ((fp1 = fopen(fname[0], "r")) == NULL) {
    goto fail;
  }

  if ((fp2 = fopen(fname[1], "w")) == NULL) {
    goto fail;
  }

  char buffer[512], buffer2[512];
  int lineno = 1;
  while (fgets(buffer, sizeof(buffer), fp1) != NULL) {
    sprintf(buffer2, "LINE %i '%s'\n", lineno++, fname[0]);
    fputs(buffer2, fp2);
    fputs(buffer, fp2);
  }
  
fail:
  
  if (fp1 != NULL) {
    fclose(fp1);
  }
  if (fp2 != NULL) {
    fclose(fp2);
  }
  
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

  if ((pt = match2 (PRD_module, fp, 0)) == NULL) {
    
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
