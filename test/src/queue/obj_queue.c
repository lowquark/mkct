#include "obj_queue.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const unsigned long initial_size = 32;

void obj_queue_init(obj_queue_t * queue) {
  queue->buffer_begin = NULL;
  queue->buffer_end = NULL;
  queue->getptr = NULL;
  queue->putptr = NULL;
  queue->size = 0;
}

void obj_queue_clear(obj_queue_t * queue) {
  /* free the buffer (may be NULL) */
  free(queue->buffer_begin);

  /* clean slate */
  obj_queue_init(queue);
}

int obj_queue_push(obj_queue_t * queue, int value) {
  int * new_buffer_begin;
  int * wrap_point;
  long new_buffer_size;

  if(!queue->buffer_begin) {
    /* this buffer is null */
    queue->buffer_begin = malloc(initial_size*sizeof(int));

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
    new_buffer_begin = malloc(new_buffer_size*sizeof(int));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return 0; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (queue->buffer_end - queue->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, queue->putptr, sizeof(int)*(queue->buffer_end - queue->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, queue->buffer_begin, sizeof(int)*(queue->putptr - queue->buffer_begin));

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

int obj_queue_pop(obj_queue_t * queue) {
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

int obj_queue_peek(obj_queue_t * queue, int * value_out) {
  if(queue->size == 0) { return 0; }

  *value_out = *queue->getptr;

  return 1;
}

int obj_queue_at(obj_queue_t * queue, int * value_out, int idx) {
  int * elem_ptr;

  if(idx < 0) { return 0; }

  if(idx >= queue->size) { return 0; }

  elem_ptr = queue->getptr + idx;

  if(elem_ptr >= queue->buffer_end) {
    elem_ptr -= queue->buffer_end - queue->buffer_begin;
  }

  *value_out = *elem_ptr;

  return 1;
}
