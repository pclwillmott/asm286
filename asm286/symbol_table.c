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

static value_t symbol_value[MAX_SYMBOLS];

static enum SymbolType symbol_type[MAX_SYMBOLS];
static enum DataType symbol_datatype[MAX_SYMBOLS];
static enum Distance symbol_distance[MAX_SYMBOLS];
static enum Visibility symbol_visibility[MAX_SYMBOLS];
static enum Language symbol_language[MAX_SYMBOLS];
static int symbol_segment[MAX_SYMBOLS];
static int symbol_length[MAX_SYMBOLS];

static int symbol_count = 0;

int get_symbol_value(const char *symbol_name, int *value) {
  int idx = get_symbol_index(symbol_name);
  if (idx != -1) {
    *value = symbol_value[idx].i;
    return 0;
  }
  return 1;
}

int get_strsymbol_value(const char *symbol_name, char **value) {
  int idx = get_symbol_index(symbol_name);
  if (idx == -1) {
    return -1;
  }
  else if (symbol_type[idx] != ST_CONSTANT || symbol_datatype[idx] != DT_STRING) {
    return -1;
  }
  setstr(value, symbol_value[idx].s);
  return 0;
}

int get_symbol_index(const char *symbol_name) {
  for (int idx = 0; idx < symbol_count; idx++) {
    if (strcmp(symbol_name, symbol_table[idx]) == 0) {
      return idx ;
    }
  }
  return -1;
}

int get_number_of_elements(const char *symbol_name, int *number) {
  int idx ;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return -1;
  }
  else if (symbol_type[idx] != ST_DATA) {
    errno = ERR_NOT_DATA_SYMBOL;
    return -1;
  }
  *number = symbol_length[idx];
  return 0;
}

int get_array_size(const char *symbol_name, int *size) {
  int idx ;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return -1;
  }
  else if (symbol_type[idx] != ST_DATA) {
    errno = ERR_NOT_DATA_SYMBOL;
    return -1;
  }
  *size = symbol_length[idx] * datatype_size(symbol_datatype[idx]);
  return 0;
}

int datatype_size(enum DataType dt) {
  if (dt < 0 || dt > DT_REAL10) {
    return -1;
  }
  int sizes[] = {1, 1, 2, 2, 4, 4, 6, 8, 10, 4, 8, 10};
  return sizes[dt];
}

int get_element_length(const char *symbol_name, int *length) {
  int idx ;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return -1;
  }
  else if (symbol_type[idx] != ST_DATA) {
    errno = ERR_NOT_DATA_SYMBOL;
    return -1;
  }
  *length = datatype_size(symbol_datatype[idx]);
  return 0;
}

int set_symbol_visibility(const char *symbol_name, enum Visibility visibility) {
  int idx ;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return idx;
  }
  symbol_visibility[idx] = visibility;
  return 0;
}

int add_label(const char *symbol_name, enum Distance dist, int value) {
  return add_symbol(
    symbol_name,
    ST_LABEL,
    DT_UNASSIGNED,
    dist,
    segment_stack_top_index(),
    LG_UNASSIGNED,
    value
  );
}

int set_variable(const char *symbol_name, enum DataType dtype, int value) {
  int idx;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return add_symbol(symbol_name, ST_VARIABLE, dtype, DIST_UNASSIGNED, -1, LG_UNASSIGNED, value);
  }
  else if (symbol_type[idx] != ST_VARIABLE || symbol_datatype[idx] != dtype) {
    errno = ERR_SYMBOL_REDEFINED;
    return -1;
  }
  symbol_value[idx].i = value;
  return 0;
}

int set_constant(const char *symbol_name, enum DataType dtype, int value) {
  int idx;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    return add_symbol(symbol_name, ST_CONSTANT, dtype, DIST_UNASSIGNED, -1, LG_UNASSIGNED, value);
  }
  else if (symbol_type[idx] != ST_CONSTANT || symbol_datatype[idx] != dtype) {
    errno = ERR_SYMBOL_REDEFINED;
    return -1;
  }
  else if (symbol_value[idx].i != value) {
    errno = ERR_CONSTANT_REDEFINED;
    return -1;
  }
  return 0;
}

int set_strconstant(const char *symbol_name, char *value) {
  int idx;
  if ((idx = get_symbol_index(symbol_name)) == -1) {
    if (add_symbol(symbol_name, ST_CONSTANT, DT_STRING, DIST_UNASSIGNED, -1, LG_UNASSIGNED, -1)) {
      return -1;
    }
    setstr(&symbol_value[get_symbol_index(symbol_name)].s,value);
  }
  else if (symbol_datatype[idx] != DT_STRING || symbol_type[idx] != ST_CONSTANT) {
    errno = ERR_SYMBOL_REDEFINED;
    return -1;
  }
  else if (strcmp(symbol_value[idx].s,value) != 0) {
    errno = ERR_CONSTANT_REDEFINED;
    return -1;
  }
  return 0;
}

int add_symbol(const char *symbol_name, enum SymbolType type, enum DataType dtype, enum Distance dist, int segment, enum Language lang, int value) {
  if (symbol_count == MAX_SYMBOLS) {
    errno = ERR_SYMBOL_TABLE_FULL;
    return 1;
  }
  if (get_symbol_index(symbol_name) != -1) {
    errno = ERR_IDENTIFIER_EXISTS;
    return 1;
  }
  if ( (symbol_table[symbol_count] = malloc(strlen(symbol_name) + 1)) == NULL ) {
    errno = ERR_OUT_OF_MEMORY ;
    return 1;
  }
  strcpy(symbol_table[symbol_count], symbol_name) ;
  symbol_type[symbol_count] = type;
  symbol_datatype[symbol_count] = dtype;
  symbol_segment[symbol_count] = segment;
  symbol_distance[symbol_count] = dist;
  symbol_visibility[symbol_count] = VS_PRIVATE;
  symbol_language[symbol_count] = lang;
  symbol_value[symbol_count++].i = value;
  return 0;
}

void dump_symbol_table() {
  char *st[] = {
    "LABEL",
    "VAR",
    "CONST",
  };
  char *dt[] = {
    "UNASSIGNED",
    "BYTE",
    "SBYTE",
    "WORD",
    "SWORD",
    "DWORD",
    "SDWORD",
    "FWORD",
    "QWORD",
    "TBYTE",
    "REAL4",
    "REAL8",
    "REAL10",
    "STRING",
  };
  char *dist[] = {
    "UNASSIGNED",
    "NEAR",
    "FAR",
  };
  char *vs[] = {
    "PRIVATE",
    "PUBLIC",
    "EXTERN",
  };
  char *lg[] = {
    "UNASSIGNED",
    "C",
  };
  
  printf("\n");
  printf("%-32s %-10s %-10s %-10s %-10s %-10s %-10s %s\n","id", "type", "data Type", "distance", "segment", "visibility", "language", "value");
  printf("%-32s %-10s %-10s %-10s %-10s %-10s %-10s %s\n","--", "----", "---------", "--------", "-------", "----------", "--------", "-----");
  for (int idx = 0; idx < symbol_count; idx++) {
    char *name = "";
    if (symbol_segment[idx] != -1) {
      name = get_segment_name(symbol_segment[idx]);
    }
    if (symbol_datatype[idx] == DT_STRING) {
      printf("%-32s %-10s %-10s %-10s %-10s %-10s %-10s %s\n",symbol_table[idx], st[symbol_type[idx]], dt[symbol_datatype[idx]+1], dist[symbol_distance[idx]+1], name, vs[symbol_visibility[idx]], lg[symbol_language[idx]+1], symbol_value[idx].s);
    }
    else {
      printf("%-32s %-10s %-10s %-10s %-10s %-10s %-10s %i\n",symbol_table[idx], st[symbol_type[idx]], dt[symbol_datatype[idx]+1], dist[symbol_distance[idx]+1], name, vs[symbol_visibility[idx]], lg[symbol_language[idx]+1], symbol_value[idx].i);
    }
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
