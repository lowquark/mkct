#ifndef _OBJ_STACK_H_
#define _OBJ_STACK_H_

typedef unsigned long obj_stack_size_t;

typedef struct obj_stack {
  int * buffer_begin;
  int * buffer_end;
  int * putptr;
  obj_stack_size_t size;
} obj_stack_t;

void obj_stack_init(obj_stack_t * stack);
void obj_stack_clear(obj_stack_t * stack);

int obj_stack_push(obj_stack_t * stack, int value);
int obj_stack_pop(obj_stack_t * stack);

int obj_stack_top(obj_stack_t * stack, int * value_out);
int obj_stack_at(obj_stack_t * stack, int * value_out, obj_stack_size_t idx);

#define obj_stack_size(stack) (((const stack_t *)stack)->size)

#endif
