#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

typedef unsigned long SIZE_TYPE;

/*
 * FILO stack of `OBJECT_TYPE`s. Grows dynamically, and manages object
 * initialization / allocation.
 */
typedef struct OBJSTACK_STRUCT {
  OBJECT_TYPE ** buffer_begin;
  OBJECT_TYPE ** buffer_end;
  OBJECT_TYPE ** putptr;
  SIZE_TYPE size;
} OBJSTACK_TYPE;

/*
 * Initializes the stack object to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJQUEUE_METHOD_CLEAR to clear an
 * initialized stack.
 */
void OBJSTACK_METHOD_INIT(OBJSTACK_TYPE * stack);

/*
 * Pops all values present in the stack. Frees all allocated memory.
 */
void OBJSTACK_METHOD_CLEAR(OBJSTACK_TYPE * stack);

/*
 * Creates a new object and places it at the top of the stack.
 * Returns NULL upon memory allocation failure.
 */
OBJECT_TYPE * OBJSTACK_METHOD_PUSH(OBJSTACK_TYPE * stack);

/*
 * Destroys and removes the object at the top of the stack. Does nothing if
 * the stack is empty. Returns whether an element was popped.
 */
int OBJSTACK_METHOD_POP(OBJSTACK_TYPE * stack);

/*
 * Returns the element at the top of the stack, or NULL if the stack is empty.
 */
OBJECT_TYPE * OBJSTACK_METHOD_PEEK(OBJSTACK_TYPE * stack);

/*
 * If an object exists at the given stack index, returns it
 * Otherwise, returns NULL.
 *
 * Note:
 *   An index of 0 is the bottom of the stack. The top of the stack is indexed
 *   by the stack's size minus one.
 */
OBJECT_TYPE * OBJSTACK_METHOD_AT(OBJSTACK_TYPE * stack, SIZE_TYPE idx);

/*
 * Returns the number of elements in the stack.
 */
#define OBJSTACK_METHOD_SIZE(_stack_) (((const OBJSTACK_TYPE *)_stack_)->size)

#endif
