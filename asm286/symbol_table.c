/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    symbol_table.c   Symbol table and support routines.
 *
 *  This revision:
 *
 *    2020 April 5 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2020 April 5 Paul Willmott Baseline.
 *
 *  Copyright (c) 2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <string.h>
#include <stdlib.h>
#include "asm286.h"
#include <stdio.h>

static char * symbol_table[MAX_SYMBOLS];

static int symbol_value[MAX_SYMBOLS];

static int symbol_type[MAX_SYMBOLS];

static int symbol_count = 0;

int get_symbol_value(const char *symbol_name, int *value) {
  int idx = get_symbol_index(symbol_name);
  if (idx != -1) {
    *value = symbol_value[idx];
    return 0;
  }
  return 1;
}

int get_symbol_index(const char *symbol_name) {
  for (int idx = 0; idx < symbol_count; idx++) {
    if (strcmp(symbol_name, symbol_table[idx]) == 0) {
      return idx ;
    }
  }
  return -1;
}

int add_symbol(const char *symbol_name, int type, int value) {
  if (symbol_count == MAX_SYMBOLS) {
    error(ERR_SYMBOL_TABLE_FULL, 0);
    return 1;
  }
  if (get_symbol_index(symbol_name) != -1) {
    error(ERR_IDENTIFIER_EXISTS, 0);
    return 1;
  }
  if ( (symbol_table[symbol_count] = malloc(strlen(symbol_name) + 1)) == NULL ) {
    error(ERR_OUT_OF_MEMORY, 0) ;
    return 1;
  }
  strcpy(symbol_table[symbol_count], symbol_name) ;
  symbol_type[symbol_count] = type;
  symbol_value[symbol_count++] = value;
  return 0;
}

void dump_symbol_table() {
  printf("\n");
  for (int idx = 0; idx < symbol_count; idx++) {
    printf("%-32s %3i %3i\n",symbol_table[idx],symbol_type[idx], symbol_value[idx]);
  }
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
