/*
 *-------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    asm286_t.h       Custom Data Types
 *
 *  This revision:
 *
 *    2020 May 26 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2020 May 26 Paul Willmott Baseline.
 *
 *  Copyright (c) 2020 Paul C. L. Willmott. See license at end.
 *
 *-------------------------------------------------------------------------------
 */

#ifndef asm286_t_h
#define asm286_t_h

/*
 * Error codes.
 */

enum {
  ERR_INTERNAL_ERROR           =  1,
  ERR_OUT_OF_MEMORY            =  2,
  ERR_LINE_BUFFER_OVERFLOW     =  3,
  ERR_SYNTAX_ERROR             =  4,
  ERR_STRING_TOO_LONG          =  5,
  ERR_IDENTIFIER_TOO_LONG      =  6,
  ERR_IDENTIFIER_EXISTS        =  7,
  ERR_SYMBOL_TABLE_FULL        =  8,
  ERR_SEGMENT_STACK_OVERFLOW   =  9,
  ERR_TOO_MANY_SEGMENTS        = 10,
  ERR_NOT_IN_SEGMENT           = 11,
  ERR_SEGMENT_NESTING_FAULT    = 12,
  ERR_SEGMENT_NOT_ENDED        = 13,
  ERR_PRODUCTION_NOT_FOUND     = 14,
  ERR_INVALID_INSTRUCTION      = 15,
  ERR_IDENTIFIER_NOT_FOUND     = 16,
} ;

#define NUM_ERR ( 16 )

/*
 * Symbol Table stuff.
 */

enum SymbolType {
  ST_LABEL    = 0,
  ST_VARIABLE = 1,
  ST_CONSTANT = 2,
  ST_DATA     = 3,
};

enum DataType {
  DT_UNASSIGNED = -1,
  DT_BYTE   = 0,
  DT_SBYTE  = 1,
  DT_WORD   = 2,
  DT_SWORD  = 3,
  DT_DWORD  = 4,
  DT_SDWORD = 5,
  DT_FWORD  = 6,
  DT_QWORD  = 7,
  DT_TBYTE  = 8,
  DT_REAL4  = 9,
  DT_REAL8  = 10,
  DT_REAL10 = 11,
};

enum Distance {
  DIST_UNASSIGNED = -1,
  DIST_NEAR = 0,
  DIST_FAR = 1,
};

enum Visibility {
  VS_PRIVATE = 0,
  VS_PUBLIC = 1,
  VS_EXTERN = 2,
};

enum Language {
  LG_UNASSIGNED = -1,
  LG_C = 0,
  LG_PASCAL = 1,
};

enum MemoryModel {
  MODEL_TINY = 0,
  MODEL_SMALL = 1,
  MODEL_MEDIUM = 2,
  MODEL_COMPACT = 3,
  MODEL_LARGE = 4,
  MODEL_HUGE = 5,
};

enum ProcessorType {
  i86 = 0,
  i186 = 1,
  i286 = 2,
  i286p = 3,
};

enum CoProcessorType {
  none = 0,
  i87 = 1,
  i287 = 2,
};

/*
 * Value Type
 */

typedef union {
  int    i ;
  double d ;
  char  *s ;
} value_t ;

/*
 * Parse Tree Node
 */

typedef struct ptree_node_t ptree_node_t ;

struct ptree_node_t {
  unsigned int production_id ;
  unsigned int variant ;
  unsigned int num_args ;
  unsigned int exec_type ;
  ptree_node_t **args ;
  unsigned int value_type ;
  value_t value ;
} ;

/*
 * Token List Node
 */

typedef struct tlist_node_t tlist_node_t ;

struct tlist_node_t {
  unsigned int token_id ;
  char *token ;
  unsigned int value_type ;
  value_t value ;
  tlist_node_t *next ;
} ;

typedef struct segment_table_t segment_table_t;

enum CombineType {
  CT_PRIVATE = 0,
  CT_PUBLIC = 1,
  CT_STACK = 2,
  CT_COMMON = 3,
  CT_AT = 4,
};

enum SegAlign {
  SA_BYTE = 0,
  SA_WORD = 1,
  SA_DWORD = 2,
  SA_PARA = 3,
  SA_PAGE = 4,
};

struct segment_table_t {
  char *name ;
  signed char read_only;
  unsigned int position;
  enum SegAlign align;
  enum CombineType combine_type;
  unsigned int at_position;
  char *class_name;
};

#endif /* asm286_t_h */

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

