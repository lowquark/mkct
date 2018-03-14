#include "int_stack.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE 32


void int_stack_init(int_stack_t * stack) {
  stack->buffer_begin = NULL;
  stack->buffer_end   = NULL;
  stack->putptr = NULL;
  stack->size = 0;
}

void int_stack_clear(int_stack_t * stack) {
  /* free the buffer (may be NULL) */
  free(stack->buffer_begin);

  /* clean slate */
  int_stack_init(stack);
}

int int_stack_push(int_stack_t * stack, int value) {
  int * new_buffer_begin;
  int_stack_size_t new_buffer_size;

  if(!stack->buffer_begin) {
    /* this buffer has not been allocated */
    stack->buffer_begin = malloc(INITIAL_SIZE*sizeof(int));

    /* couldn't alloc, escape before anything breaks */
    if(!stack->buffer_begin) { return 0; }

    stack->buffer_end = stack->buffer_begin + INITIAL_SIZE;
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

int int_stack_pop(int_stack_t * stack) {
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

int int_stack_top(int_stack_t * stack, int * value_out) {
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

int int_stack_at(int_stack_t * stack, int * value_out, int_stack_size_t idx) {
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
