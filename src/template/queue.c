
#include "H_FILE"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 32


static void deinit_value(VALUE_TYPE value) {
  /* TODO: Cleanup the VALUE_TYPE, if applicable. This function is called
   * when values are popped from the queue, and when the queue is cleared. */
}


void QUEUE_METHOD_INIT(QUEUE_TYPE * q) {
  q->buffer_begin = NULL;
  q->buffer_end = NULL;
  q->getptr = NULL;
  q->putptr = NULL;
  q->size = 0;
}

void QUEUE_METHOD_CLEAR(QUEUE_TYPE * q) {
  VALUE_TYPE * valptr;

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
  QUEUE_METHOD_INIT(q);
}

int QUEUE_METHOD_PUSH(QUEUE_TYPE * q, VALUE_TYPE value) {
  VALUE_TYPE * new_buffer_begin;
  VALUE_TYPE * wrap_point;
  long new_buffer_size;

  if(!q->buffer_begin) {
    /* this buffer is null */
    q->buffer_begin = malloc(INITIAL_SIZE*sizeof(VALUE_TYPE));

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
    new_buffer_begin = malloc(new_buffer_size*sizeof(VALUE_TYPE ));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return 0; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (q->buffer_end - q->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, q->putptr, sizeof(VALUE_TYPE)*(q->buffer_end - q->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, q->buffer_begin, sizeof(VALUE_TYPE)*(q->putptr - q->buffer_begin));

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

int QUEUE_METHOD_POP(QUEUE_TYPE * q) {
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

int QUEUE_METHOD_PEEK(QUEUE_TYPE * q, VALUE_TYPE * value_out) {
  if(q->size == 0) { return 0; }

  *value_out = *q->getptr;

  return 1;
}

int QUEUE_METHOD_AT(QUEUE_TYPE * q, VALUE_TYPE * value_out, int idx) {
  VALUE_TYPE * elem_ptr;

  if(idx < 0) { return 0; }

  if(idx >= q->size) { return 0; }

  elem_ptr = q->getptr + idx;

  if(elem_ptr >= q->buffer_end) {
    elem_ptr -= q->buffer_end - q->buffer_begin;
  }

  *value_out = *elem_ptr;

  return 1;
}

