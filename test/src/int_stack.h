#ifndef _INT_STACK_H_
#define _INT_STACK_H_

typedef unsigned long int_stack_size_t;

typedef struct int_stack {
  int * buffer_begin;
  int * buffer_end;
  int * putptr;
  int_stack_size_t size;
} int_stack_t;

void int_stack_init(int_stack_t * stack);
void int_stack_clear(int_stack_t * stack);

int int_stack_push(int_stack_t * stack, int value);
int int_stack_pop(int_stack_t * stack);

int int_stack_top(int_stack_t * stack, int * value_out);
int int_stack_at(int_stack_t * stack, int * value_out, int_stack_size_t idx);

#define int_stack_size(stack) (((const stack_t *)stack)->size)

#endif
