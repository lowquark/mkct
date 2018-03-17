#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * FIFO queue of `OBJECT_TYPE`s. Grows dynamically, and manages object
 * initialization / allocation.
 */
typedef struct OBJQUEUE_STRUCT {
  OBJECT_TYPE ** buffer_begin;
  OBJECT_TYPE ** buffer_end;

  OBJECT_TYPE ** getptr;
  OBJECT_TYPE ** putptr;

  long size;
} OBJQUEUE_TYPE;

/*
 * Initializes the queue object to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJQUEUE_METHOD_CLEAR to clear an
 * initialized queue.
 */
void OBJQUEUE_METHOD_INIT(OBJQUEUE_TYPE * q);

/*
 * Pops all values present in the queue. Frees all allocated memory.
 */
void OBJQUEUE_METHOD_CLEAR(OBJQUEUE_TYPE * q);

/*
 * Creates a new object and places it at the back of the queue.
 * Returns NULL upon memory allocation failure.
 */
OBJECT_TYPE * OBJQUEUE_METHOD_PUSH(OBJQUEUE_TYPE * q);

/*
 * Destroys and removes the object at the front of the queue. Does nothing if
 * the queue is empty. Returns whether an element was popped.
 */
int OBJQUEUE_METHOD_POP(OBJQUEUE_TYPE * q);

/*
 * Returns the element at the front of the queue. Returns NULL if the queue is
 * empty.
 */
OBJECT_TYPE * OBJQUEUE_METHOD_PEEK(OBJQUEUE_TYPE * q);

/*
 * If an object exists at the given queue index, returns it
 * Otherwise, returns NULL.
 *
 * Note:
 *   An index of 0 is the front of the queue. The back of the queue is indexed
 *   by the queue's size minus one.
 */
OBJECT_TYPE * OBJQUEUE_METHOD_AT(OBJQUEUE_TYPE * q, long idx);

/*
 * Returns the number of elements in the queue
 */
long OBJQUEUE_METHOD_SIZE(OBJQUEUE_TYPE * q);

#endif
