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

ptree_node_t * match
(
 int production_id,
 tlist_node_t **toklst,
 int start_arg
)
{

  extern char * prdlst[] ;
  
  unsigned int prd_base = 0 ;
  int prd_index = 0 ;
  unsigned long num_args = 0L ;
  int arg_index ;
  int islist ;
  
  ptree_node_t *ptree, *safe_ptree ;
  
  tlist_node_t *safe_tp, *tp ;
  
  int arg ;
  
  unsigned int j ;
  
/*
 *-----------------------------------------------------------------------------
 */

/*
 * Find production code in production list.
 */
  
  while ( ( ( j = pid ( prdlst [ prd_base ] ) ) != PRD_LAST ) && ( j != production_id ) ) {
    prd_base++ ;
  }
  
/*
 * Throw error if production not found. This should never occur
 * outside of development.
 */
  
  if ( j == PRD_LAST ) {
    error ( ERR_PRODUCTION_NOT_FOUND, -1 ) ;
    return NULL ;
  }
  
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
    error ( ERR_OUT_OF_MEMORY, -1 ) ;
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
      error ( ERR_OUT_OF_MEMORY, -1 ) ;
      free ( ptree ) ;
      return NULL ;
    }
    
/*
 * Try and find each symbol in turn.
 */
    
    tp = (*toklst) ;
    
    ptree->num_args = ( prd_index == 0 ) ? start_arg : 0 ;
    arg_index = ptree->num_args ;
    
    while ( ( tp != NULL ) && ( arg_index < num_args ) ) {
      
      /*
       * Get target argument.
       */
      
      arg = pid ( &prdlst [ prd_base + prd_index ][ arg_index * 2 + 3 ] ) ;
      
      /*
       * Handle terminals.
       */
      
      if ( arg < PRODUCTION_OFFSET ) {
        
        if ( arg == tp->token_id ) {
          
          if ( ( ptree->args[ arg_index ] = ( ptree_node_t * ) malloc ( sizeof ( ptree_node_t ) ) ) == NULL ) {
            error ( ERR_OUT_OF_MEMORY, -1 ) ;
            delete_ptree ( ptree, 1, 1, start_arg ) ;
            return NULL ;
          }
          
          ptree->args[ arg_index ]->production_id = arg ;
          ptree->args[ arg_index ]->value_type = tp->value_type ;
          ptree->args[ arg_index ]->num_args = 0 ;
          
          ptree->num_args++ ;
          
          if ( ptree->args[ arg_index ]->value_type == TOK_STRING ) {
            
            if ( ( ptree->args[ arg_index ]->value.s = ( char * ) malloc ( strlen ( tp->value.s ) + 1 ) ) == NULL ) {
              error ( ERR_OUT_OF_MEMORY, -1 ) ;
              delete_ptree ( ptree, 1, 1, start_arg ) ;
              return NULL ;
            }
            
            strcpy ( ptree->args[ arg_index ]->value.s, tp->value.s ) ;
            
          }
          else {
            CPYVALUE( ptree->args[ arg_index ]->value, tp->value ) ;
          }
          
          tp = tp->next ;
          
        }
        else {
          break ;
        }
        
      }
      
/*
 * Handle productions.
 */
      
      else {
        
        if ( ( ptree->args[ arg_index ] = match ( arg, &tp, 0 ) ) != NULL ) {
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
      
      safe_tp = tp ;
      
      if ( ( islist ) && ( prd_index != 0 ) ) {
        
        while ( ( tp != NULL ) && ( ( safe_ptree = match ( production_id, &tp, 1 ) ) != NULL ) ) {
          
          safe_ptree->args[ 0 ] = ptree ;
          
          ptree = safe_ptree ;
          
          (*toklst) = tp ;
          
          safe_tp = tp ;
          
        }
        
      }
      
      (*toklst) = safe_tp ;
      
      return ptree ;
      
    }
    
/*
 * Tidy-Up.
 */
    
    delete_ptree ( ptree, 0, 1, ( prd_index == 0 ) ? start_arg : 0 ) ;
    
    if ( start_arg ) {
      
      free ( ptree ) ;
      
      return NULL ;
    }
    
    prd_index++ ;
    
  }
  
/*
 * Tidy-Up.
 */
  
  free ( ptree ) ;
  
  return NULL ;
  
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

ptree_node_t * match2
(
 int production_id,
 FILE *fp,
 int start_arg
)
{
  
  extern char * prdlst[] ;
  
  unsigned int prd_base = 0 ;
  int prd_index = 0 ;
  unsigned long num_args = 0L ;
  int arg_index ;
  int islist ;
  
  ptree_node_t *ptree, *safe_ptree ;
  
  long int safe_tp, tp ;
  
  int arg ;
  
  unsigned int j ;
  
  long int toklst = ftell(fp);
  
  /*
   *-----------------------------------------------------------------------------
   */
#ifndef FRED
  for (int i=0; i<indent; i++) {
    printf(" ");
  }
  indent++;
  switch (production_id) {
    case PRD_TOP:
      printf("Searching for: PRD_TOP\n");
      break;
    case PRD_DIRECTIVE_LIST:
      printf("Searching for: PRD_DIRECTIVE_LIST\n");
      break;
    case PRD_DIRECTIVE:
      printf("Searching for: PRD_DIRECTIVE\n");
      break;
    case PRD_SEGMENT_DEF:
      printf("Searching for: PRD_SEGMENT_DEF\n");
      break;
    case PRD_SEGMENT_DIR:
      printf("Searching for: PRD_SEGMENT_DIR\n");
      break;
    case PRD_SEG_ID:
      printf("Searching for: PRD_SEG_ID\n");
      break;
    case PRD_NEWLINE:
      printf("Searching for: PRD_NEWLINE\n");
      break;
    case PRD_IN_SEG_DIR_LIST:
      printf("Searching for: PRD_IN_SEG_DIR_LIST\n");
      break;
    case PRD_IN_SEG_DIR:
      printf("Searching for: PRD_IN_SEG_DIR\n");
      break;
    case PRD_IN_SEGMENT_DIR:
      printf("Searching for: PRD_IN_SEGMENT_DIR\n");
      break;
    case PRD_INSTRUCTION:
      printf("Searching for: PRD_INSTRUCTION\n");
      break;
    case PRD_INST_PREFIX:
      printf("Searching for: PRD_INST_PREFIX\n");
      break;
    case PRD_XINST_PREFIX:
      printf("Searching for: PRD_INST_XPREFIX\n");
      break;
    case PRD_ASM_INSTRUCTION:
      printf("Searching for: PRD_ASM_INSTRUCTION\n");
      break;
    case PRD_MNEMONIC:
      printf("Searching for: PRD_MNEMONIC\n");
      break;
    case PRD_XMNEMONIC:
      printf("Searching for: PRD_XMNEMONIC\n");
      break;
    case PRD_ENDS_DIR:
      printf("Searching for: PRD_ENDS_DIR\n");
      break;
    default:
      printf("Searching for: %o %o\n", production_id / 127 + 1, production_id % 127 + 1);
      break;
  }
#endif
  /*
   * Find production code in production list.
   */
  
  while ( ( ( j = pid ( prdlst [ prd_base ] ) ) != PRD_LAST ) && ( j != production_id ) ) {
    prd_base++ ;
  }
  
  /*
   * Throw error if production not found. This should never occur
   * outside of development.
   */
  
  if ( j == PRD_LAST ) {
    error ( ERR_PRODUCTION_NOT_FOUND, -1 ) ;
    return NULL ;
  }
  
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
    error ( ERR_OUT_OF_MEMORY, -1 ) ;
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
      error ( ERR_OUT_OF_MEMORY, -1 ) ;
      free ( ptree ) ;
      return NULL ;
    }
    
    /*
     * Try and find each symbol in turn.
     */
    
    tp = toklst ;
    fseek(fp, tp, SEEK_SET);
    
    ptree->num_args = ( prd_index == 0 ) ? start_arg : 0 ;
    arg_index = ptree->num_args ;
    
    while ( ( !feof(fp) ) && ( arg_index < num_args ) ) {
      
      /*
       * Get target argument.
       */
      
      arg = pid ( &prdlst [ prd_base + prd_index ][ arg_index * 2 + 3 ] ) ;
      
      /*
       * Handle terminals.
       */
      
      if ( arg < PRODUCTION_OFFSET ) {
        
        if ( (ptree->args[ arg_index ] = find_token(arg, fp)) != NULL ) {
          ptree->num_args++ ;
        }
        else {
          break ;
        }
        
      }
      
      /*
       * Handle productions.
       */
      
      else {
        
        if ( ( ptree->args[ arg_index ] = match2 ( arg, fp, 0 ) ) != NULL ) {
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
      
      safe_tp = ftell(fp) ;
      
      if ( ( islist ) && ( prd_index != 0 ) ) {
        
        while ( ( !feof(fp) ) && ( ( safe_ptree = match2 ( production_id, fp, 1 ) ) != NULL ) ) {
          
          safe_ptree->args[ 0 ] = ptree ;
          
          ptree = safe_ptree ;
          
          toklst = ftell(fp) ;
          
          safe_tp = tp ;
          
        }
        
      }
      
      toklst = safe_tp ;
      fseek(fp, toklst, SEEK_SET);
      indent--;
      return ptree ;
      
    }
    
    /*
     * Tidy-Up.
     */
    
    delete_ptree ( ptree, 0, 1, ( prd_index == 0 ) ? start_arg : 0 ) ;
    
    if ( start_arg ) {
      
      free ( ptree ) ;
      indent--;
      return NULL ;
    }
    
    prd_index++ ;
    
  }
  
  /*
   * Tidy-Up.
   */
  
  free ( ptree ) ;
  
  indent--;
  
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
