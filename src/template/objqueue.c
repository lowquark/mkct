
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


void OBJQUEUE_METHOD_INIT(OBJQUEUE_TYPE * q) {
  q->buffer_begin = NULL;
  q->buffer_end = NULL;
  q->getptr = NULL;
  q->putptr = NULL;
  q->size = 0;
}

void OBJQUEUE_METHOD_CLEAR(OBJQUEUE_TYPE * q) {
  OBJECT_TYPE ** valptr;

  free(q->buffer_begin);

  /* iterate over [getptr, putptr), call clear */
  if(q->size) {
    do {
      valptr = q->getptr;

      object_clear(*valptr);

      valptr ++;
      if(valptr == q->buffer_end) {
        valptr = q->buffer_begin;
      }
    } while(valptr != q->putptr);
  }

  /* clean slate */
  OBJQUEUE_METHOD_INIT(q);
}

OBJECT_TYPE * OBJQUEUE_METHOD_PUSH(OBJQUEUE_TYPE * q) {
  OBJECT_TYPE * new_object;
  OBJECT_TYPE ** new_buffer_begin;
  OBJECT_TYPE ** wrap_point;
  long new_buffer_size;

  if(!q->buffer_begin) {
    /* this buffer is null */
    q->buffer_begin = malloc(initial_size*sizeof(OBJECT_TYPE *));

    /* couldn't alloc, escape before anything breaks */
    if(!q->buffer_begin) { return NULL; }

    q->buffer_end   = q->buffer_begin + initial_size;
    q->getptr       = q->buffer_begin;
    q->putptr       = q->buffer_begin;
  } else if(q->getptr == q->putptr && q->size != 0) {
    /* full buffer condition */

    /* sanity check */
    assert(q->buffer_end - q->buffer_begin == q->size);

    /* double previous buffer size */
    new_buffer_size = 2*q->size;

    /* alloc new buffer twice as large */
    new_buffer_begin = malloc(new_buffer_size*sizeof(OBJECT_TYPE *));

    /* couldn't alloc, escape before anything breaks */
    if(!new_buffer_begin) { return NULL; }

    /* pointer within new_buffer where buffer_end lines up with */
    wrap_point = new_buffer_begin + (q->buffer_end - q->putptr);

    /* copy first part [putptr, buffer_end) to new_buffer_begin */
    memcpy(new_buffer_begin, q->putptr, sizeof(OBJECT_TYPE *)*(q->buffer_end - q->putptr));

    /* copy second part [buffer_begin, putptr) to wrap_point */
    memcpy(wrap_point, q->buffer_begin, sizeof(OBJECT_TYPE *)*(q->putptr - q->buffer_begin));

    /* new buffer has been initialized, replace old buffer */
    free(q->buffer_begin);

    q->buffer_begin = new_buffer_begin;
    q->buffer_end   = new_buffer_begin + new_buffer_size;
    q->getptr       = new_buffer_begin;
    q->putptr       = new_buffer_begin + q->size;
  }

  new_object = malloc(sizeof(OBJECT_TYPE));

  /* store at put pointer and advance */
  *q->putptr++ = new_object;

  object_init(new_object);

  /* wrap put pointer at end */
  if(q->putptr == q->buffer_end) {
    q->putptr = q->buffer_begin;
  }

  /* keep track of size */
  q->size ++;

  /* return success */
  return new_object;
}

int OBJQUEUE_METHOD_POP(OBJQUEUE_TYPE * q) {
  if(q->size == 0) { return 0; }

  object_clear(*q->getptr);
  free(*q->getptr);

  q->getptr++;

  /* wrap get pointer at end */
  if(q->getptr == q->buffer_end) {
    q->getptr = q->buffer_begin;
  }

  /* keep track of size */
  q->size --;

  return 1;
}

OBJECT_TYPE * OBJQUEUE_METHOD_PEEK(OBJQUEUE_TYPE * q) {
  if(q->size == 0) { return NULL; }

  return *q->getptr;
}

OBJECT_TYPE * OBJQUEUE_METHOD_AT(OBJQUEUE_TYPE * q, long idx) {
  OBJECT_TYPE ** elem_ptr;

  if(idx < 0) { return NULL; }

  if(idx >= q->size) { return NULL; }

  elem_ptr = q->getptr + idx;

  if(elem_ptr >= q->buffer_end) {
    elem_ptr -= q->buffer_end - q->buffer_begin;
  }

  return *elem_ptr;
}

