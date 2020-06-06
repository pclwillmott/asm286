/*
 *-------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    asm286.h       Definitions
 *
 *  This revision:
 *
 *    2020 March 29 Paul Willmott Token macros updated for i286.
 *
 *  Revision History:
 *
 *    2019 November 17 Paul Willmott Error codes added.
 *
 *    2019 November 10 Paul Willmott Production string constants added.
 *
 *    2019 November 9 Paul Willmott Baseline.
 *
 *  Copyright (c) 2019-2020 Paul C. L. Willmott. See license at end.
 *
 *-------------------------------------------------------------------------------
 */

#include <stdio.h>
#include "asm286_t.h"

#ifndef asm286_h
#define asm286_h

#define OLD 1

#define MAX_SYMBOLS 128

#define MAX_IF_STACK 16

/*
 * Token types.
 */

#define KEYWORD          "\001"
#define KEYWORD_186      "\011"
#define KEYWORD_286      "\021"
#define KEYWORD_286P     "\031"
#define KEYWORD_8087     "\101"
#define KEYWORD_287      "\201"
#define RESERVED         "\002"
#define KEYWORD_RESERVED "\003"
#define SPECIAL          "\004"

#define PROCESSOR_MASK 070
#define COPROCESSOR_MASK 0300

#define PROCESSOR(x) (((x) & PROCESSOR_MASK) >> 3)
#define COPROCESSOR(x) (((x) & COPROCESSOR_MASK) >> 5)

/*
 * Token string constants.
 */

#define NUM_PATTERN ( 403 )  /* Number of token patterns */

#define STK_UNDERSCORE  "\001\001"
#define STK_MINUS       "\001\002"
#define STK_SEMICOLON   "\001\003"
#define STK_COLON       "\001\004"
#define STK_QMARK       "\001\005"
#define STK_PERIOD      "\001\006"
#define STK_QUOTE       "\001\007"
#define STK_OBRACE      "\001\010"
#define STK_CBRACE      "\001\011"
#define STK_OBRACKET    "\001\012"
#define STK_CBRACKET    "\001\013"
#define STK_ATSIGN      "\001\014"
#define STK_MULTIPLY    "\001\015"
#define STK_DIVIDE      "\001\016"
#define STK_DBLQUOTE    "\001\017"
#define STK_AMPERSAND   "\001\020"
#define STK_PLUS        "\001\021"
#define STK_LESS        "\001\022"
#define STK_GREATER     "\001\023"
#define STK_DOLLAR      "\001\024"
#define STK_COMMA       "\001\025"
#define STK_EQUAL       "\001\026"
#define STK_PERCENT     "\001\027"

#define STK_8086        "\001\030"
#define STK_8087        "\001\031"
#define STK_186         "\001\032"
#define STK_286         "\001\033"
#define STK_286P        "\001\034"
#define STK_287         "\001\035"
#define STK_CODE        "\001\036"
#define STK_CONST       "\001\037"
#define STK_DATA        "\001\040"
#define STK_DATAQ       "\001\041"
#define STK_FARDATA     "\001\042"
#define STK_FARDATAQ    "\001\043"
#define STK_LIST        "\001\044"
#define STK_MODEL       "\001\045"
#define STK_NO87        "\001\046"
#define STK_NOLIST      "\001\047"
#define STK_DOTSTACK    "\001\050"

/*
 * BEGIN AUTONUMBER SECTION
 */

#include "STK.h"

/*
 * END OF AUTONUBER SECTION
 */

#define STK_INTEGERBIN  "\004\013"
#define STK_INTEGEROCT  "\004\014"
#define STK_INTEGERDEC  "\004\015"
#define STK_INTEGERHEX  "\004\016"
#define STK_DOUBLE      "\004\017"
#define STK_STRING      "\004\020"
#define STK_IDENTIFIER  "\004\021"
#define STK_INST_LABEL  "\004\022"
#define STK_NEWLINE     "\004\023"
#define STK_WHITESPACE  "\004\024"
#define STK_TEXT        "\004\025"
#define STK_TOKSEQUENCE "\004\026"

/*
 * Production string constants.
 */

#include "SPD.h"

#define SPD_LAST            "\177\177"

enum {
  
  TOK_UNDERSCORE  =   0,
  TOK_MINUS       =   1,
  TOK_SEMICOLON   =   2,
  TOK_COLON       =   3,
  TOK_QMARK       =   4,
  TOK_PERIOD      =   5,
  TOK_QUOTE       =   6,
  TOK_OBRACE      =   7,
  TOK_CBRACE      =   8,
  TOK_OBRACKET    =   9,
  TOK_CBRACKET    =  10,
  TOK_ATSIGN      =  11,
  TOK_MULTIPLY    =  12,
  TOK_DIVIDE      =  13,
  TOK_DBLQUOTE    =  14,
  TOK_AMPERSAND   =  15,
  TOK_PLUS        =  16,
  TOK_LESS        =  17,
  TOK_GREATER     =  18,
  TOK_DOLLAR      =  19,
  TOK_COMMA       =  20,
  TOK_EQUAL       =  21,
  TOK_PERCENT     =  22,

  TOK_8086        =  23,
  TOK_8087        =  24,
  TOK_186         =  25,
  TOK_286         =  26,
  TOK_286P        =  27,
  TOK_287         =  28,
  TOK_CODE        =  29,
  TOK_CONST       =  30,
  TOK_DATA        =  31,
  TOK_DATAQ       =  32,
  TOK_FARDATA     =  33,
  TOK_FARDATAQ    =  34,
  TOK_LIST        =  35,
  TOK_MODEL       =  36,
  TOK_NO87        =  37,
  TOK_NOLIST      =  38,
  TOK_DOTSTACK    =  39,
  
#include "TOK.h"
  
  TOK_INTEGERBIN  = 391,
  TOK_INTEGEROCT  = 392,
  TOK_INTEGERDEC  = 393,
  TOK_INTEGERHEX  = 394,
  TOK_DOUBLE      = 395,
  TOK_STRING      = 396,
  TOK_IDENTIFIER  = 397,
  TOK_INST_LABEL  = 398,
  TOK_NEWLINE     = 399,
  TOK_WHITESPACE  = 400,
  TOK_TEXT        = 401,
  TOK_TOKSEQUENCE = 402,

/*
 * Productions
 */

#include "PRD.h"

  PRD_LAST        = 16128,

  XT_IGNORE       = 0001,
  XT_DROP         = 0002,
  XT_EXECUTE      = 0003,

} ;

#define PRODUCTION_OFFSET (8128)

/*
 * Segment support.
 */

#define MAX_SEGMENT_STACK (8)  /* Size of Segment Stack            */
#define MAX_SEGMENT (8)        /* Maximum entries in Segment table */

/*
 * Helper Macros.
 */

#define CPYVALUE(TGT, SRC) memcpy( &TGT, &SRC, sizeof( value_t ) )

/*
 * Prototypes.
 */

int assemble(const char *, int, int );
void dump_pattern( void );
void error(void);
void delete_ptree( ptree_node_t *, int, int, int );
int execute ( ptree_node_t *, int, int );
unsigned int pid( const char * );
unsigned char to_byte( char );
int dep(unsigned char, int, int);
int depw(unsigned short, int, int);
int depd(unsigned int, int, int);
int setstr(char **, char *);
int get_symbol_value(const char *, int *);
int get_symbol_index(const char *);
int add_symbol(const char *, enum SymbolType, enum DataType, enum Distance, int, enum Language, int);
int set_variable(const char *, enum DataType, int);
int set_model(enum MemoryModel);

int add_label(const char *, enum Distance, int);
int segment_stack_top_index(void);
void dump_symbol_table(void);
int open_segment(const char *);
int open_segment_with_attributes(const char *, int, enum SegAlign, enum CombineType, int, char *);
int close_segment(const char *);
int get_current_position(unsigned int *);
void reset_for_pass(int);
segment_table_t *segment_stack_top(void);
int get_segment_index(const char *);
int segment_stack_count(void);
int set_current_position(unsigned int, int);
int dep_disp(unsigned char [], int, int, int, int);
unsigned char rm_disp_mod(ptree_node_t *, int *, int *);
int dep_opcodes(unsigned char [], int, int, int);
int is_word(int);
int process(const char *, int);

#ifdef OLD
ptree_node_t * match2(int, FILE *, int);
ptree_node_t *find_token(int, FILE *);
int match_pattern2(FILE *, const char *, char **, unsigned long *);
#else
ptree_node_t *match2(int, int);
ptree_node_t *find_token(int);
int match_pattern2(const char *, char **, long *);
#endif

void printProdName(int);
int checkInstruction(int);
char *get_segment_name(int);
int set_symbol_visibility(const char *, enum Visibility);
void trim(char *);
void dump_segment_table(void);
int set_strconstant(const char *, char *);
int set_constant(const char *, enum DataType, int);
int get_strsymbol_value(const char *, char **);
int get_number_of_elements(const char *, int *);
int get_array_size(const char *, int *);
int datatype_size(enum DataType);
int get_element_length(const char *, int *);
int set_assume(enum SegReg, const char *);
int set_segment_group(const char *, const char *);
segment_table_t * get_segment(const char *);
int close_all_segments(void);
enum MemoryModel get_model(void);
int add_to_segid_list(const char *);
int set_segment_group_from_list(const char *);
int add_numbers(char *[]);
int openStream(const char *);
int closeStreams(void);
int saveStreamContext(stream_context_t *);
int restoreStreamContext(stream_context_t *);
int readStream(int *);
int endOfInput(void);
void copyStreamContext(stream_context_t *, stream_context_t *);
int seekStream(stream_context_t *, long);
int processPP(void);

// COMMON DATA

extern int processor;
extern int coprocessor;
extern char *title;
extern char *subtitle;
extern int list;
extern int check_instructions;
extern int errno;
extern enum Language language_type;
extern enum Distance stack_distance;
extern int main_routine;
extern char *module_name;

#endif /* asm286_h */


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
