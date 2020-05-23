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
    error(ERR_SEGMENT_STACK_OVERFLOW, lineno);
    return 1;
  }
  int index;
  if ((index = get_segment_index(name)) == -1) {
    if (segment_count == MAX_SEGMENT) {
      error(ERR_TOO_MANY_SEGMENTS, lineno);
      return 1;
    }
    index = segment_count++;
    segment_table_t *segment = &segment_table[index];
    segment->name = NULL;
    setstr(&segment->name, (char *)name);
    segment->attributes = attributes;
    segment->position = 0;
  }
  segment_stack[stack_count++] = index;
  return 0;
}

int close_segment(const char *name, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top(lineno)) == NULL) {
    return 1;
  }
  if (strcmp(name, segment->name)) {
    error(ERR_SEGMENT_NESTING_FAULT, lineno);
    return 1;
  }
  stack_count--;
  return 0;
}

int segment_stack_count() {
  return stack_count;
}

segment_table_t *segment_stack_top(int lineno)
{
  if (stack_count == 0) {
    error(ERR_NOT_IN_SEGMENT, lineno);
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
  processor = i86;
  coprocessor = none;
}

int get_current_position(unsigned int *position, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top(lineno)) == NULL) {
    return 1;
  }
  *position = segment->position;
  return 0;
}

int set_current_position(unsigned int position, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top(lineno)) == NULL) {
    return 1;
  }
  segment->position = position;
  return 0;
}

int dep(unsigned char db, int pass, int lineno)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top(lineno)) == NULL) {
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

