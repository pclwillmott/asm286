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
#include <string.h>
#include <stdio.h>
#include "asm286.h"

/*
 * This routine converts a char to an unsigned char.
 */

unsigned char to_byte( char sc )
{
  
  union {
    unsigned char uc ;
    char sc ;
  } value ;
  
/*
 *-----------------------------------------------------------------------------
 */
  
  value.sc = sc ;
  
  return value.uc ;
  
/*
 * Finished
 */
  
}

/*
 * This function decodes the string form of the token/production id
 * into the unsigned integer form.
 */

unsigned int pid( const char *str )
{
  
  unsigned int A, B ;
  
/*
 *-----------------------------------------------------------------------------
 */
  
  A = (unsigned int) to_byte( *str ) ;
  B = (unsigned int) to_byte( *(str+1) ) ;
  
  return ( A - 1 ) * 127 + ( B - 1 ) ;
  
/*
 * Finished
 */
  
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

int indent = 0;
int *prdIndex = NULL;

int build_index() {
 
  int count = 0;
  
  extern char * prdlst[] ;
  extern char * prod_list[];
  
  for (int i = 0; *prod_list[i]; i++ ) {
    count++ ;
  }
  
  if ((prdIndex = (int *) malloc (count * sizeof(int))) == NULL) {
    errno = ERR_OUT_OF_MEMORY;
    return -1;
  }

  for (int i = 0; i < count; i++ ) {
    prdIndex[i] = -1 ;
  }

  int last = -1;
  for (int i = 0; pid(prdlst[i]) != PRD_LAST; i++ ) {
    int j = pid(prdlst[i]);
    if (last != j) {
      last = j;
      prdIndex[j-PRODUCTION_OFFSET] = i;
    }
  }
  
  for (int i = 0; i < count; i++ ) {
    if (prdIndex[i] == -1) {
      printf("%s undefined\n", prod_list[i]);
    }
  }

  return 0;
  
}

#ifdef OLD
ptree_node_t * match2
(
 int production_id,
 FILE *fp,
 int start_arg
)
#else
ptree_node_t * match2
(
 int production_id,
 int start_arg
 )
#endif

{
  
  extern char * prdlst[] ;
  
  extern int *prdIndex;
  
  unsigned int prd_base = 0 ;
  int prd_index = 0 ;
  unsigned long num_args = 0L ;
  int arg_index ;
  int islist ;
  
  ptree_node_t *ptree, *safe_ptree ;
  
  if (prdIndex == NULL) {
    build_index();
  }
  
  int arg ;
  
  unsigned int j ;

  extern long int maxPos;
  
#ifdef OLD
  long int safe_tp, tp ;
  long int toklst = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  long int end_of_file = ftell(fp);
#else
  stream_context_t toklst, safe_tp, tp;
  if (saveStreamContext(&toklst)) {
    return NULL;
  }
#endif
  
  /*
   *-----------------------------------------------------------------------------
   */

  /*
   * Find production code in production list.
   */
  
  prd_base = prdIndex[production_id-PRODUCTION_OFFSET];
  prd_index = 0;
  
  /*
   * If the first production variant is a recursive list and
   * a search from the first arg is requested then skip to the next one.
   * Note: recursive lists are always listed first in the
   * production list.
   */
  
  islist = ( production_id == pid ( &prdlst [ prd_base ][ 3 ] ) ) ;
  
  if ( ( start_arg == 0 ) && ( islist ) ) {
    prd_index++ ;
  }
  
  /*
   * Create production tree node.
   */
  
  if ( ( ptree = ( ptree_node_t * ) malloc ( sizeof ( ptree_node_t ) ) ) == NULL ) {
    errno = ERR_OUT_OF_MEMORY ;
    return NULL ;
  }
  
  ptree->production_id = production_id ;
  ptree->value_type = production_id ;
  memset ( &ptree->value, 0, sizeof ( value_t ) ) ;
  
  /*
   * Try each production variant, return if found.
   */
  
  while ( pid ( prdlst [ prd_base + prd_index ] ) == production_id ) {
    
    /*
     * Store production variant for "execute".
     */
    
    ptree->variant = prd_index ;
    ptree->num_args = 0 ;
    ptree->exec_type = to_byte ( prdlst [ prd_base + prd_index ][ 2 ] ) ;
    
    /*
     * Find number of arguments (tokens/productions) to match.
     */
    
    num_args = ( strlen ( prdlst [ prd_base + prd_index ] ) - 3 ) >> 1 ;
    
    /*
     * Create argument pointers.
     */
    
    if ( ( ptree->args = ( ptree_node_t ** ) malloc ( sizeof ( ptree_node_t * ) * num_args ) ) == NULL ) {
      errno = ERR_OUT_OF_MEMORY ;
      free ( ptree ) ;
      return NULL ;
    }
    
    /*
     * Try and find each symbol in turn.
     */
    
#ifdef OLD
    tp = toklst ;
    fseek(fp, tp, SEEK_SET);
#else
    copyStreamContext(&tp, &toklst);
    if (restoreStreamContext(&tp)) {
      return NULL;
    }
#endif
    
    ptree->num_args = ( prd_index == 0 ) ? start_arg : 0 ;
    arg_index = ptree->num_args ;

#ifdef OLD
    while ( ( !feof(fp) ) && ( arg_index < num_args ) ) {
#else
    while (!endOfInput() && (arg_index < num_args)) {
#endif
      /*
       * Get target argument.
       */
      
      arg = pid ( &prdlst [ prd_base + prd_index ][ arg_index * 2 + 3 ] ) ;
      
      /*
       * Handle terminals.
       */
      
      if ( arg < PRODUCTION_OFFSET ) {

#ifdef OLD
        if ( (ptree->args[ arg_index ] = find_token(arg, fp)) != NULL ) {
#else
        if ( (ptree->args[ arg_index ] = find_token(arg)) != NULL ) {
#endif
          
          ptree->num_args++ ;
        }
        else {
#ifdef OLD
          long int x = ftell(fp);
          if (x>maxPos) {
            maxPos = x;
          }
#endif
          break ;
        }
        
      }
      
      /*
       * Handle productions.
       */
      
      else {

#ifdef OLD
        if ( ( ptree->args[ arg_index ] = match2 ( arg, fp, 0 ) ) != NULL ) {
#else
        if ( ( ptree->args[ arg_index ] = match2 ( arg, 0 ) ) != NULL ) {
#endif
          ptree->num_args++ ;
        }
        else {
          break ;
        }
      }
      
      arg_index++ ;
      
    }
    
    /*
     * All arguments found, so return.
     */
    
    if ( arg_index == num_args ) {
      
      /*
       * Check for first element in recursive list.
       */
 
#ifdef OLD
      safe_tp = ftell(fp) ;
#else
      if (saveStreamContext(&safe_tp)) {
        return NULL;
      }
#endif
      
      if ( ( islist ) && ( prd_index != 0 ) ) {

#ifdef OLD
        while ( ( safe_tp < end_of_file ) && ( ( safe_ptree = match2 ( production_id, fp, 1 ) ) != NULL ) ) {
#else
        while ( !endOfInput() && ( ( safe_ptree = match2 ( production_id, 1 ) ) != NULL ) ) {
#endif
          safe_ptree->args[ 0 ] = ptree ;
          
          ptree = safe_ptree ;

#ifdef OLD
          toklst = ftell(fp) ;
          safe_tp = toklst;
#else
          if (saveStreamContext(&toklst)) {
            return NULL;
          }
          copyStreamContext(&safe_tp, &toklst);
#endif
          
        }
        
      }
#ifdef OLD
      toklst = safe_tp ;
      fseek(fp, toklst, SEEK_SET);
#else
      copyStreamContext(&toklst, &safe_tp);
      if (restoreStreamContext(&toklst)) {
        return NULL;
      }
#endif
        
      indent--;
        
        // AT THIS POINT HANDLE INCLUDE FILES
#ifndef OLD
        if (ptree->production_id == PRD_includeDir) {
          if (openStream(ptree->args[1]->args[0]->value.s)) {
            return NULL;
          }
        }
#endif
        
        

        return ptree ;
    }
    
    /*
     * Tidy-Up.
     */
    
    delete_ptree ( ptree, 0, 1, ( prd_index == 0 ) ? start_arg : 0 ) ;
    
    if ( start_arg ) {
      
      free ( ptree ) ;
      indent--;
      
#ifdef OLD
      fseek(fp, toklst, SEEK_SET);
#else
      restoreStreamContext(&toklst);
#endif
      return NULL ;
      
    }
    
    prd_index++ ;
    
  }
  
  /*
   * Tidy-Up.
   */
  
  free ( ptree ) ;
  
  indent--;

#ifdef OLD
  fseek(fp, toklst, SEEK_SET);
#else
        restoreStreamContext(&toklst);
#endif
        
  return NULL ;
  
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
