#ifndef STACK_H
#define STACK_H

typedef unsigned long SIZE_TYPE;

typedef struct STACK_STRUCT {
  VALUE_TYPE * buffer_begin;
  VALUE_TYPE * buffer_end;
  VALUE_TYPE * putptr;
  SIZE_TYPE size;
} STACK_TYPE;


void STACK_METHOD_INIT(STACK_TYPE * stack);
void STACK_METHOD_CLEAR(STACK_TYPE * stack);

int STACK_METHOD_PUSH(STACK_TYPE * stack, VALUE_TYPE value);
int STACK_METHOD_POP(STACK_TYPE * stack);

int STACK_METHOD_TOP(STACK_TYPE * stack, VALUE_TYPE * value_out);
int STACK_METHOD_AT(STACK_TYPE * stack, VALUE_TYPE * value_out, SIZE_TYPE idx);

#define STACK_METHOD_SIZE(stack) (((const stack_t *)stack)->size)

#endif
