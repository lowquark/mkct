
#include "H_FILE"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*  ========  object functionaility  ========  */


/* This function is called after an object's memory has been allocated. */
static void object_init(OBJECT_TYPE * obj) {
  memset(obj, 0, sizeof(OBJECT_TYPE));
}

/* This function is called before an object's memory is freed. */
static void object_clear(OBJECT_TYPE * obj) {
}


/*  ========  general functionaility  ========  */


static const unsigned long initial_size = 32;


void OBJQUEUE_METHOD_INIT(OBJQUEUE_TYPE * queue) {
  queue->buffer_begin = NULL;
  queue->buffer_end = NULL;
  queue->getptr = NULL;
  queue->putptr = NULL;
  queue->size = 0;
}

void OBJQUEUE_METHOD_CLEAR(OBJQUEUE_TYPE * queue) {
  OBJECT_TYPE ** valptr;

  /* iterate over [getptr, putptr), call clear and free */
  if(queue->size) {
    do {
      valptr = queue->getptr;

      object_clear(*valptr);
      free(*valptr);

      valptr ++;
      if(valptr == queue->buffer_end) {
        valptr = queue->buffer_begin;
      }
    } while(valptr != queue->putptr);
  }

  /* free the buffer (may be NULL) */
  free(queue->buffer_begin);

  /* clean slate */
  OBJQUEUE_METHOD_INIT(queue);
}

OBJECT_TYPE * OBJQUEUE_METHOD_PUSH(OBJQUEUE_TYPE * queue) {
  OBJECT_TYPE * new_object;
  OBJECT_TYPE ** new_buffer_begin;
  OBJECT_TYPE ** wrap_point;
  long new_buffer_size;

  if(!queue->buffer_begin) {
    /* this buffer is null */
    queue->buffer_begin = malloc(initial_size*sizeof(OBJECT_TYPE *));

    /* couldn't alloc, escape before anything breaks */
    if(!queue->buffer_begin) { return NULL; }

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
    new_buffer_begin = malloc(new_buffer_size*sizeof(OBJECT_TYPE *));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return NULL; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (queue->buffer_end - queue->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, queue->putptr, sizeof(OBJECT_TYPE *)*(queue->buffer_end - queue->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, queue->buffer_begin, sizeof(OBJECT_TYPE *)*(queue->putptr - queue->buffer_begin));

    /* new buffer has been initialized, replace old buffer */
    free(queue->buffer_begin);

    queue->buffer_begin = new_buffer_begin;
    queue->buffer_end   = new_buffer_begin + new_buffer_size;
    queue->getptr       = new_buffer_begin;
    queue->putptr       = new_buffer_begin + queue->size;
  }

  /* allocate + initialize */
  new_object = malloc(sizeof(OBJECT_TYPE));
  object_init(new_object);

  /* store at put pointer and advance */
  *queue->putptr++ = new_object;

  /* wrap put pointer at end */
  if(queue->putptr == queue->buffer_end) {
    queue->putptr = queue->buffer_begin;
  }

  /* keep track of size */
  queue->size ++;

  /* return success */
  return new_object;
}

int OBJQUEUE_METHOD_POP(OBJQUEUE_TYPE * queue) {
  if(queue->size == 0) { return 0; }

  object_clear(*queue->getptr);
  free(*queue->getptr);

  queue->getptr++;

  /* wrap get pointer at end */
  if(queue->getptr == queue->buffer_end) {
    queue->getptr = queue->buffer_begin;
  }

  /* keep track of size */
  queue->size --;

  return 1;
}

OBJECT_TYPE * OBJQUEUE_METHOD_PEEK(OBJQUEUE_TYPE * queue) {
  if(queue->size == 0) { return NULL; }

  return *queue->getptr;
}

OBJECT_TYPE * OBJQUEUE_METHOD_AT(OBJQUEUE_TYPE * queue, long idx) {
  OBJECT_TYPE ** elem_ptr;

  if(idx < 0) { return NULL; }

  if(idx >= queue->size) { return NULL; }

  elem_ptr = queue->getptr + idx;

  if(elem_ptr >= queue->buffer_end) {
    elem_ptr -= queue->buffer_end - queue->buffer_begin;
  }

  return *elem_ptr;
}

