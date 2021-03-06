#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

typedef unsigned long SIZE_TYPE;

/*
 * FILO stack of `VALUE_TYPE`s. Values are copied, not referenced.
 */
typedef struct STACK_STRUCT {
  VALUE_TYPE * buffer_begin;
  VALUE_TYPE * buffer_end;
  VALUE_TYPE * putptr;
  SIZE_TYPE size;
} STACK_TYPE;

/*
 * Initializes the given `STACK_TYPE` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJMAP_METHOD_CLEAR to pop all values
 * from the stack.
 */
void STACK_METHOD_INIT(STACK_TYPE * stack);

/*
 * Pops all values present in the stack, and frees all allocated memory it
 * owns.
 */
void STACK_METHOD_CLEAR(STACK_TYPE * stack);

/*
 * Pushes the given value onto the top of the stack, reallocating buffer space
 * if necessary. Returns 1 if successful, and 0 otherwise.
 */
int STACK_METHOD_PUSH(STACK_TYPE * stack, VALUE_TYPE value);

/*
 * If the stack is non-empty, pops (erases) its top value and returns 1.
 * Otherwise, returns 0.
 */
int STACK_METHOD_POP(STACK_TYPE * stack);

/*
 * If the stack is non-empty, stores its top value into `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int STACK_METHOD_TOP(STACK_TYPE * stack, VALUE_TYPE * value_out);

/*
 * If a value exists at the given stack index, stores its value in `*value_out` and returns 1
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 *
 * Note:
 *   An index of 0 is the front of the stack. The top of the stack is indexed
 *   by the stack's size minus one.
 */
int STACK_METHOD_AT(STACK_TYPE * stack, VALUE_TYPE * value_out, SIZE_TYPE idx);

/*
 * Returns the number of elements in the stack
 */
#define STACK_METHOD_SIZE(_stack_) (((const STACK_TYPE *)_stack_)->size)

#endif
