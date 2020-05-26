/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    segment.c    Segment and code deposit support.
 *
 *  This revision:
 *
 *    2020 April 10 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2020 April 10 Paul Willmott Baseline.
 *
 *  Copyright (c) 2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asm286.h"

segment_table_t segment_table[MAX_SEGMENT];
unsigned int segment_count = 0;

unsigned int segment_stack[MAX_SEGMENT_STACK];
unsigned int stack_count = 0;

enum MemoryModel model = MODEL_TINY;

enum Distance default_distance[6][2] = {
  { DIST_NEAR, DIST_NEAR },
  { DIST_NEAR, DIST_NEAR },
  { DIST_FAR,  DIST_NEAR },
  { DIST_NEAR, DIST_FAR },
  { DIST_FAR,  DIST_FAR },
  { DIST_FAR,  DIST_FAR },
};


int set_model(enum MemoryModel mod) {
  model = mod;
  int res;
  if ((res = set_variable("@CodeSize", DT_BYTE, default_distance[model][0]))) {
    return res;
  }
  if ((res = set_variable("@DataSize", DT_BYTE, default_distance[model][1]))) {
    return res;
  }
  return set_variable("@Model", DT_BYTE, mod+1);
}

char *get_segment_name(int idx) {
  return segment_table[idx].name;
}

int get_segment_index(const char *name) {
  for (int i = 0; i < segment_count; i++) {
    if (strcmp(name, segment_table[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

int open_segment(const char *name, int lineno)
{
  return open_segment_with_attributes(name, 0, lineno);
}

int open_segment_with_attributes(const char *name, int attributes, int lineno)
{
  if (stack_count == MAX_SEGMENT_STACK) {
    errno = ERR_SEGMENT_STACK_OVERFLOW;
    return 1;
  }
  int index;
  if ((index = get_segment_index(name)) == -1) {
    if (segment_count == MAX_SEGMENT) {
      errno = ERR_TOO_MANY_SEGMENTS;
      return 1;
    }
    index = segment_count++;
    segment_table_t *segment = &segment_table[index];
    segment->name = NULL;
    setstr(&segment->name, (char *)name);
    segment->position = 0;
  }
  segment_stack[stack_count++] = index;
  return 0;
}

int close_segment(const char *name, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top()) == NULL) {
    return 1;
  }
  if (strcmp(name, segment->name)) {
    errno = ERR_SEGMENT_NESTING_FAULT;
    return 1;
  }
  stack_count--;
  return 0;
}

int segment_stack_count() {
  return stack_count;
}

int segment_stack_top_index() {
  if (stack_count == 0) {
    errno = ERR_NOT_IN_SEGMENT;
    return -1;
  }
  return segment_stack[stack_count-1];
}

segment_table_t *segment_stack_top()
{
  if (stack_count == 0) {
    errno = ERR_NOT_IN_SEGMENT;
    return NULL;
  }
  return &segment_table[segment_stack[stack_count-1]];
}

void reset_for_pass(int pass) {
  for (int i = 0; i < segment_count; i++) {
    free(segment_table[i].name);
  }
  stack_count = 0;
  segment_count = 0;
}

int get_current_position(unsigned int *position)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top()) == NULL) {
    return 1;
  }
  *position = segment->position;
  return 0;
}

int set_current_position(unsigned int position, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top()) == NULL) {
    return 1;
  }
  segment->position = position;
  return 0;
}

int dep(unsigned char db, int pass, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top()) == NULL) {
    return 1;
  }
  if (pass == 1) {
    printf("%s: %04x %02x\n",segment->name, segment->position, db);
  }
  segment->position++;
  return 0;
}

int depw(unsigned short dw, int pass, int lineno)
{
  return (dep( dw & 0xff, pass, lineno) ||
          dep( dw >> 8,   pass, lineno)) ;
}

int depd(unsigned int dd, int pass, int lineno)
{
  return (dep((dd)       & 0xff, pass, lineno) ||
          dep((dd >>  8) & 0xff, pass, lineno) ||
          dep((dd >> 16) & 0xff, pass, lineno) ||
          dep((dd >> 24) & 0xff, pass, lineno));
}

void dump_segment_table() {
  
  char *align[] = {
    "BYTE",
    "WORD",
    "DWORD",
    "PARA",
    "PAGE",
  };
  
  char *combine[] = {
    "PUBLIC",
    "STACK",
    "COMMON",
    "AT",
    "PRIVATE",
  };
  
  printf("\n%-32s %-10s %-10s %-10s %-10s\n", "segment name", "access", "alignment", "combine", "at");
  printf(  "%-32s %-10s %-10s %-10s %-10s\n", "------------", "------", "---------", "-------", "--");
  char buffer[16];
  for (int i = 0; i < segment_count; i++) {
    struct segment_table_t *t = &segment_table[i];
    sprintf(buffer,"%04x", t->at_position );
    printf("%-32s %-10s %-10s %-10s %-10s\n", t->name,
           (t->read_only) ? "read only" : "read/write",
           align[t->align],
           combine[t->combine_type],
           (t->combine_type == CT_AT) ? buffer : "");
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

