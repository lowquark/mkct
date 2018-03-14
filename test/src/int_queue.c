#include "int_queue.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 32


static void deinit_value(int value) {
  /* TODO: Cleanup the int, if applicable. This function is called
   * when values are popped from the queue, and when the queue is cleared. */
}


void int_queue_init(int_queue_t * q) {
  q->buffer_begin = NULL;
  q->buffer_end = NULL;
  q->getptr = NULL;
  q->putptr = NULL;
  q->size = 0;
}

void int_queue_clear(int_queue_t * q) {
  int * valptr;

  free(q->buffer_begin);

  /* iterate over [getptr, putptr), call deinit */
  if(q->size) {
    do {
      valptr = q->getptr;

      deinit_value(*valptr);

      valptr ++;
      if(valptr == q->buffer_end) {
        valptr = q->buffer_begin;
      }
    } while(valptr != q->putptr);
  }

  /* clean slate */
  int_queue_init(q);
}

int int_queue_push(int_queue_t * q, int value) {
  int * new_buffer_begin;
  int * wrap_point;
  long new_buffer_size;

  if(!q->buffer_begin) {
    /* this buffer is null */
    q->buffer_begin = malloc(INITIAL_SIZE*sizeof(int));

    /* couldn't alloc, escape before anything breaks */
    if(!q->buffer_begin) { return 0; }

    q->buffer_end   = q->buffer_begin + INITIAL_SIZE;
    q->getptr       = q->buffer_begin;
    q->putptr       = q->buffer_begin;
  } else if(q->getptr == q->putptr && q->size != 0) {
    /* full buffer condition */

    /* sanity check */
    assert(q->buffer_end - q->buffer_begin == q->size);

    /* double previous buffer size */
    new_buffer_size = 2*q->size;

    /* alloc new buffer twice as large */
    new_buffer_begin = malloc(new_buffer_size*sizeof(int ));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return 0; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (q->buffer_end - q->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, q->putptr, sizeof(int)*(q->buffer_end - q->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, q->buffer_begin, sizeof(int)*(q->putptr - q->buffer_begin));

    /* new buffer has been initialized, replace old buffer */
    free(q->buffer_begin);

    q->buffer_begin = new_buffer_begin;
    q->buffer_end   = new_buffer_begin + new_buffer_size;
    q->getptr       = new_buffer_begin;
    q->putptr       = new_buffer_begin + q->size;
  }

  /* store at put pointer and advance */
  *q->putptr++ = value;

  /* wrap put pointer at end */
  if(q->putptr == q->buffer_end) {
    q->putptr = q->buffer_begin;
  }

  /* keep track of size */
  q->size ++;

  /* return success */
  return 1;
}

int int_queue_pop(int_queue_t * q) {
  if(q->size == 0) { return 0; }

  deinit_value(*q->getptr);

  q->getptr++;

  /* wrap get pointer at end */
  if(q->getptr == q->buffer_end) {
    q->getptr = q->buffer_begin;
  }

  /* keep track of size */
  q->size --;

  return 1;
}

int int_queue_peek(int_queue_t * q, int * value_out) {
  if(q->size == 0) { return 0; }

  *value_out = *q->getptr;

  return 1;
}

int int_queue_at(int_queue_t * q, int * value_out, int idx) {
  int * elem_ptr;

  if(idx < 0) { return 0; }

  if(idx >= q->size) { return 0; }

  elem_ptr = q->getptr + idx;

  if(elem_ptr >= q->buffer_end) {
    elem_ptr -= q->buffer_end - q->buffer_begin;
  }

  *value_out = *elem_ptr;

  return 1;
}
