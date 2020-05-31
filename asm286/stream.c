/*
 *------------------------------------------------------------------------------
 *
 *  ASM286 - i286 (80286) Assembler
 *
 *  Description:
 *
 *    stream.c       Source file stream
 *
 *  This revision:
 *
 *    2020 May 31 Paul Willmott Baseline.
 *
 *  Revision History:
 *
 *    2020 May 31 Paul Willmott Baseline.
 *
 *  Copyright (c) 2020 Paul C. L. Willmott. See license at end.
 *
 *------------------------------------------------------------------------------
 */

#include <string.h>
#include "asm286.h"

int stream_count = 0;

stream_node_t stream_stack[MAX_INCLUDE_FILES];

int openStream(const char *filename) {
  
  printf("open: %s\n", filename);
  
  if (stream_count == MAX_INCLUDE_FILES) {
    errno = ERR_TOO_MANY_INCLUDE_FILES;
    return -1;
  }
  
  stream_node_t *top = &stream_stack[stream_count];
  
  if ((top->fp = fopen(filename, "r")) == NULL) {
    errno = ERR_FILE_OPEN_ERROR;
    return -1;
  }
  
  if (fseek(top->fp, 0L, SEEK_END)) {
    errno = ERR_FILE_POSITION_ERROR;
    return -1;
  }
  
  top->size = ftell(top->fp);
  
  if (fseek(top->fp, 0L, SEEK_SET)) {
    errno = ERR_FILE_POSITION_ERROR;
    return -1;
  }
  
  stream_count++;
  
  return 0;
  
}

int closeStreams() {
  
  while (stream_count) {
    stream_count--;
    if (fclose(stream_stack[stream_count].fp)) {
      errno = ERR_FILE_CLOSE_ERROR;
    }
  }
  return (errno) ? -1 : 0;
  
}

int saveStreamContext(stream_context_t *save) {
  
  save->node_id = stream_count - 1;
  
  if (stream_count > 0) {
    
    stream_node_t *top = &stream_stack[save->node_id];
    
    save->position = ftell(top->fp);
    
  }
  
  return 0;

}

int restoreStreamContext(stream_context_t *restore) {
  
  if (restore == NULL || restore->node_id >= stream_count) {
    printf("restore: id = %i count = %i\n", restore->node_id,  stream_count);
  }
  
  for (int i = restore->node_id + 1; i < stream_count; i++) {
    if (fclose(stream_stack[i].fp)) {
      errno = ERR_FILE_CLOSE_ERROR;
    }
  }
  
//  if (errno) {
//    error();
//    printf("eric");
//    return -1;
//  }

  stream_count = restore->node_id + 1;
  
  stream_node_t *top = &stream_stack[restore->node_id];
  
  if (fseek(top->fp, restore->position, SEEK_SET)) {
    errno = ERR_FILE_POSITION_ERROR;
    return -1;
  }
  
  return 0;
  
}

int seekStream(stream_context_t *context, long offset) {
  
  if (restoreStreamContext(context)) {
    return -1;
  }
  
  int ci;
  while (offset-- > 0) {
    if (readStream(&ci)) {
      return -1;
    }
  }
  
  return 0;
  
}

int readStream(int *ci) {

//  printf("in\n");
  if (stream_count > 0) {
    
    stream_node_t *top = &stream_stack[stream_count - 1];
    
    while ((stream_count) && (*ci = fgetc(top->fp)) == EOF) {
      printf("herez\n");
      if (fclose(top->fp)) {
        errno = ERR_FILE_CLOSE_ERROR;
        return  -1;
      }
      stream_count--;
      top = (stream_count) ? &stream_stack[stream_count - 1] : NULL;
    }
    
 //   printf("%c", (char)*ci);
    
  }
  
  if (stream_count == 0) {
    *ci = EOF;
  }
  
//  printf("stream_count: %i %c\n", stream_count, (char)*ci);

//  printf("out\n");
  return 0;
  
}

int endOfInput(void) {
  return (stream_count > 0) ? 0 : -1;
}

void copyStreamContext(stream_context_t *dest, stream_context_t *src) {
  memmove(dest, src, sizeof(stream_context_t));
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
