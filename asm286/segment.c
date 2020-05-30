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

char *segid_list[MAX_SEGMENT];
unsigned int segid_list_count = 0;

int add_to_segid_list(const char * segId) {
  if (segid_list_count == MAX_SEGMENT) {
    errno = ERR_TOO_MANY_SEGMENTS;
    return -1;
  }
  setstr(&segid_list[segid_list_count++], (char *)segId);
  return 0;
}

int set_segment_group_from_list(const char *group) {
  for (int i = 0; i < segid_list_count; i++) {
    if (set_segment_group(segid_list[i], group)) {
      return -1;
    }
    free(segid_list[i]);
  }
  segid_list_count = 0;
  return 0;
}

enum MemoryModel model = MODEL_TINY;

enum Distance default_distance[6][2] = {
  { DIST_NEAR, DIST_NEAR },
  { DIST_NEAR, DIST_NEAR },
  { DIST_FAR,  DIST_NEAR },
  { DIST_NEAR, DIST_FAR },
  { DIST_FAR,  DIST_FAR },
  { DIST_FAR,  DIST_FAR },
};

enum MemoryModel get_model() {
  return model;
}

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

segment_table_t * get_segment(const char *name) {
  int idx ;
  if ((idx = get_segment_index(name)) == -1) {
    return NULL;
  }
  return &segment_table[idx];
}

int get_segment_index(const char *name) {
  for (int i = 0; i < segment_count; i++) {
    if (strcmp(name, segment_table[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

int open_segment(const char *name)
{
  return open_segment_with_attributes(name, 0, SA_PARA, CT_PRIVATE, 0, NULL);
}

int open_segment_with_attributes(const char *name, int readonly, enum SegAlign segAlign, enum CombineType combineType, int at, char *className)
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
    segment->read_only = readonly;
    segment->combine_type = combineType;
    segment->align = segAlign;
    segment->at_position = 0;
    if (combineType == CT_AT) {
      segment->at_position = at;
    }
    segment->size = 0;
    if (combineType == CT_STACK) {
      segment->size = at;
    }
    if (className != NULL) {
      setstr(&segment->class_name, className);
    }
    segment->name = NULL;
    setstr(&segment->name, (char *)name);
    segment->position = 0;
    for (int i = 0; i < 4; i++) {
      setstr(&segment->assume[i], "NOTHING");
    }
    segment->group = NULL;
  }
  segment_stack[stack_count++] = index;
  return 0;
}

int set_assume(enum SegReg seg, const char *value)
{
  segment_table_t *segment;
  if ((segment = segment_stack_top()) == NULL) {
    return -1;
  }
  free(segment->assume[seg]);
  setstr(&segment->assume[seg], (char *)value);
  return 0;
}

int set_segment_group(const char *name, const char *group)
{
  segment_table_t *segment;
  if ((segment = get_segment(name)) == NULL) {
    return -1;
  }
  if (segment->group != NULL) {
    free(segment->group);
  }
  setstr(&segment->group, (char *)group);
  return 0;
}

int close_segment(const char *name)
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

int close_all_segments()
{
  stack_count = 0;
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
    "PRIVATE",
    "PUBLIC",
    "STACK",
    "COMMON",
    "AT",
  };
  
  printf("\n%-32s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "segment name", "access", "alignment", "combine", "at", "class","group", "size", "CS","DS","ES","SS");
  printf(  "%-32s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "------------", "------", "---------", "-------", "--", "-----","-----", "----", "--","--","--","--");
  char buffer[16], buffer2[16];
  for (int i = 0; i < segment_count; i++) {
    struct segment_table_t *t = &segment_table[i];
    sprintf(buffer,"%04x", t->at_position);
    sprintf(buffer2,"%04i", t->size);
    printf("%-32s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", t->name,
           (t->read_only) ? "read only" : "read/write",
           align[t->align],
           combine[t->combine_type],
           (t->combine_type == CT_AT) ? buffer : "",
           (t->class_name == NULL) ? "" : t->class_name,
           (t->group == NULL) ? "" : t->group,
           buffer2,
           t->assume[0],
           t->assume[1],
           t->assume[2],
           t->assume[3]
           );
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

