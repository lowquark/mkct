#ifndef _INT_STACK_H_
#define _INT_STACK_H_

typedef unsigned long int_stack_size_t;

/*
 * FILO stack of `int`s. Values are copied, not referenced.
 */
typedef struct int_stack {
  int * buffer_begin;
  int * buffer_end;
  int * putptr;
  int_stack_size_t size;
} int_stack_t;

/*
 * Initializes the given `int_stack_t` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJMAP_METHOD_CLEAR to pop all values
 * from the stack.
 */
void int_stack_init(int_stack_t * stack);

/*
 * Pops all values present in the stack, and frees all allocated memory it
 * owns.
 */
void int_stack_clear(int_stack_t * stack);

/*
 * Pushes the given value onto the top of the stack, reallocating buffer space
 * if necessary. Returns 1 if successful, and 0 otherwise.
 */
int int_stack_push(int_stack_t * stack, int value);

/*
 * If the stack is non-empty, pops (erases) its top value and returns 1.
 * Otherwise, returns 0.
 */
int int_stack_pop(int_stack_t * stack);

/*
 * If the stack is non-empty, stores its top value into `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int int_stack_top(int_stack_t * stack, int * value_out);

/*
 * If a value exists at the given stack index, stores its value in `*value_out` and returns 1
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 *
 * Note:
 *   An index of 0 is the front of the stack. The top of the stack is indexed
 *   by the stack's size minus one.
 */
int int_stack_at(int_stack_t * stack, int * value_out, int_stack_size_t idx);

/*
 * Returns the number of elements in the stack
 */
#define int_stack_size(_stack_) (((const int_stack_t *)_stack_)->size)

#endif
