
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


void OBJSTACK_METHOD_INIT(OBJSTACK_TYPE * stack) {
  stack->buffer_begin = NULL;
  stack->buffer_end   = NULL;
  stack->putptr = NULL;
  stack->size = 0;
}

void OBJSTACK_METHOD_CLEAR(OBJSTACK_TYPE * stack) {
  OBJECT_TYPE ** valptr;

  /* iterate over [0, putptr), call clear and free */
  if(stack->size) {
    for(valptr = stack->buffer_begin ;
        valptr != stack->putptr ;
        valptr ++) {
      object_clear(*valptr);
      free(*valptr);
    }
  }

  /* free the buffer (may be NULL) */
  free(stack->buffer_begin);

  /* clean slate */
  OBJSTACK_METHOD_INIT(stack);
}

OBJECT_TYPE * OBJSTACK_METHOD_PUSH(OBJSTACK_TYPE * stack) {
  OBJECT_TYPE * new_object;
  OBJECT_TYPE ** new_buffer_begin;
  SIZE_TYPE new_buffer_size;

  if(!stack->buffer_begin) {
    /* this buffer has not been allocated */
    stack->buffer_begin = malloc(initial_size*sizeof(OBJECT_TYPE *));

    /* couldn't alloc, escape before anything breaks */
    if(!stack->buffer_begin) { return NULL; }

    stack->buffer_end = stack->buffer_begin + initial_size;
    stack->putptr     = stack->buffer_begin;
  } else if(stack->putptr == stack->buffer_end) {
    /* full buffer condition */

    /* double previous buffer size */
    new_buffer_size = 2*stack->size;

    /* realloc twice as large */
    new_buffer_begin = realloc(stack->buffer_begin, new_buffer_size*sizeof(OBJECT_TYPE *));

    /* couldn't realloc, escape before anything breaks */
    if(!new_buffer_begin) { return NULL; }

    stack->buffer_begin = new_buffer_begin;
    stack->buffer_end   = new_buffer_begin + new_buffer_size;
    stack->putptr       = new_buffer_begin + stack->size;
  }

  /* allocate + initialize */
  new_object = malloc(sizeof(OBJECT_TYPE));

  object_init(new_object);

  /* store at put pointer and advance */
  *stack->putptr++ = new_object;

  /* keep track of size */
  stack->size ++;

  return new_object;
}

int OBJSTACK_METHOD_POP(OBJSTACK_TYPE * stack) {
  /* If unallocated, but initialized, this won't try to pop, assuming
   * (stack->putptr == stack->buffer_end)
   */

  if(stack->putptr > stack->buffer_begin) {
    stack->putptr --;

    /* deinitialize + deallocate */
    object_clear(*stack->putptr);
    free(*stack->putptr);

    stack->size --;

    return 1;
  } else {
    return 0;
  }
}

OBJECT_TYPE * OBJSTACK_METHOD_PEEK(OBJSTACK_TYPE * stack) {
  /* If unallocated, but initialized, this will return 0, assuming
   * (stack->putptr == stack->buffer_begin)
   *
   * Will not return undefined memory assuming
   * (stack->putptr <= stack->buffer_end)
   */

  if(stack->putptr > stack->buffer_begin) {
    return *(stack->putptr - 1);
  } else {
    return NULL;
  }
}

OBJECT_TYPE * OBJSTACK_METHOD_AT(OBJSTACK_TYPE * stack, SIZE_TYPE idx) {
  /* If unallocated, but initialized, this will return 0, assuming
   * (stack->putptr == 0)
   */

  OBJECT_TYPE ** slot = stack->buffer_begin + idx;

  if(slot < stack->buffer_begin || slot >= stack->putptr) {
    return NULL;
  } else {
    return *slot;
  }
}

