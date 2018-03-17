#include "obj_stack.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


static const unsigned long initial_size = 32;


void obj_stack_init(obj_stack_t * stack) {
  stack->buffer_begin = NULL;
  stack->buffer_end   = NULL;
  stack->putptr = NULL;
  stack->size = 0;
}

void obj_stack_clear(obj_stack_t * stack) {
  /* free the buffer (may be NULL) */
  free(stack->buffer_begin);

  /* clean slate */
  obj_stack_init(stack);
}

int obj_stack_push(obj_stack_t * stack, int value) {
  int * new_buffer_begin;
  obj_stack_size_t new_buffer_size;

  if(!stack->buffer_begin) {
    /* this buffer has not been allocated */
    stack->buffer_begin = malloc(initial_size*sizeof(int));

    /* couldn't alloc, escape before anything breaks */
    if(!stack->buffer_begin) { return 0; }

    stack->buffer_end = stack->buffer_begin + initial_size;
    stack->putptr     = stack->buffer_begin;
  } else if(stack->putptr == stack->buffer_end) {
    /* full buffer condition */

    /* double previous buffer size */
    new_buffer_size = 2*stack->size;

    /* realloc twice as large */
    new_buffer_begin = realloc(stack->buffer_begin, new_buffer_size*sizeof(int));

    /* couldn't realloc, escape before anything breaks */
    if(!new_buffer_begin) { return 0; }

    stack->buffer_begin = new_buffer_begin;
    stack->buffer_end   = new_buffer_begin + new_buffer_size;
    stack->putptr       = new_buffer_begin + stack->size;
  }

  /* store at put pointer and advance */
  *stack->putptr++ = value;

  /* keep track of size */
  stack->size ++;

  return 1;
}

int obj_stack_pop(obj_stack_t * stack) {
  /* If unallocated, but initialized, this won't try to pop, assuming
   * (stack->putptr == stack->buffer_end)
   */

  if(stack->putptr > stack->buffer_begin) {
    stack->putptr --;
    stack->size --;

    return 1;
  } else {
    return 0;
  }
}

int obj_stack_top(obj_stack_t * stack, int * value_out) {
  /* If unallocated, but initialized, this will return 0, assuming
   * (stack->putptr == stack->buffer_begin)
   *
   * Will not return undefined memory assuming
   * (stack->putptr <= stack->buffer_end)
   */

  if(stack->putptr > stack->buffer_begin) {
    *value_out = *(stack->putptr - 1);
    return 1;
  } else {
    return 0;
  }
}

int obj_stack_at(obj_stack_t * stack, int * value_out, obj_stack_size_t idx) {
  /* If unallocated, but initialized, this will return 0, assuming
   * (stack->putptr == 0)
   */

  int * slot = stack->buffer_begin + idx;

  if(slot < stack->buffer_begin || slot >= stack->putptr) {
    return 0;
  } else {
    *value_out = *slot;
    return 1;
  }
}
