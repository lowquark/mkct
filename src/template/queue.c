
#include "H_FILE"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


static const unsigned long initial_size = 32;


void QUEUE_METHOD_INIT(QUEUE_TYPE * queue) {
  queue->buffer_begin = NULL;
  queue->buffer_end = NULL;
  queue->getptr = NULL;
  queue->putptr = NULL;
  queue->size = 0;
}

void QUEUE_METHOD_CLEAR(QUEUE_TYPE * queue) {
  /* free the buffer (may be NULL) */
  free(queue->buffer_begin);

  /* clean slate */
  QUEUE_METHOD_INIT(queue);
}

int QUEUE_METHOD_PUSH(QUEUE_TYPE * queue, VALUE_TYPE value) {
  VALUE_TYPE * new_buffer_begin;
  VALUE_TYPE * wrap_point;
  long new_buffer_size;

  if(!queue->buffer_begin) {
    /* this buffer is null */
    queue->buffer_begin = malloc(initial_size*sizeof(VALUE_TYPE));

    /* couldn't alloc, escape before anything breaks */
    if(!queue->buffer_begin) { return 0; }

    queue->buffer_end   = queue->buffer_begin + initial_size;
    queue->getptr       = queue->buffer_begin;
    queue->putptr       = queue->buffer_begin;
  } else if(queue->getptr == queue->putptr && queue->size != 0) {
    /* full buffer condition */

    /* sanity check */
    assert(queue->buffer_end - queue->buffer_begin == queue->size);

    /* double previous buffer size */
    new_buffer_size = 2*queue->size;

    /* alloc new buffer twice as large */
    new_buffer_begin = malloc(new_buffer_size*sizeof(VALUE_TYPE));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return 0; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (queue->buffer_end - queue->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, queue->putptr, sizeof(VALUE_TYPE)*(queue->buffer_end - queue->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, queue->buffer_begin, sizeof(VALUE_TYPE)*(queue->putptr - queue->buffer_begin));

    /* new buffer has been initialized, replace old buffer */
    free(queue->buffer_begin);

    queue->buffer_begin = new_buffer_begin;
    queue->buffer_end   = new_buffer_begin + new_buffer_size;
    queue->getptr       = new_buffer_begin;
    queue->putptr       = new_buffer_begin + queue->size;
  }

  /* store at put pointer and advance */
  *queue->putptr++ = value;

  /* wrap put pointer at end */
  if(queue->putptr == queue->buffer_end) {
    queue->putptr = queue->buffer_begin;
  }

  /* keep track of size */
  queue->size ++;

  /* return success */
  return 1;
}

int QUEUE_METHOD_POP(QUEUE_TYPE * queue) {
  if(queue->size == 0) { return 0; }

  queue->getptr++;

  /* wrap get pointer at end */
  if(queue->getptr == queue->buffer_end) {
    queue->getptr = queue->buffer_begin;
  }

  /* keep track of size */
  queue->size --;

  return 1;
}

int QUEUE_METHOD_PEEK(QUEUE_TYPE * queue, VALUE_TYPE * value_out) {
  if(queue->size == 0) { return 0; }

  *value_out = *queue->getptr;

  return 1;
}

int QUEUE_METHOD_AT(QUEUE_TYPE * queue, VALUE_TYPE * value_out, int idx) {
  VALUE_TYPE * elem_ptr;

  if(idx < 0) { return 0; }

  if(idx >= queue->size) { return 0; }

  elem_ptr = queue->getptr + idx;

  if(elem_ptr >= queue->buffer_end) {
    elem_ptr -= queue->buffer_end - queue->buffer_begin;
  }

  *value_out = *elem_ptr;

  return 1;
}

