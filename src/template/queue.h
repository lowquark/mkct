#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * FIFO queue of `VALUE_TYPE`s. Values are copied, not referenced.
 */
typedef struct QUEUE_STRUCT {
  VALUE_TYPE * buffer_begin;
  VALUE_TYPE * buffer_end;

  VALUE_TYPE * getptr;
  VALUE_TYPE * putptr;

  long size;
} QUEUE_TYPE;

/*
 * Initializes the given `QUEUE_TYPE` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use QUEUE_METHOD_CLEAR to pop all values
 * from the queue.
 */
void QUEUE_METHOD_INIT(QUEUE_TYPE * q);

/*
 * Pops all values present in the queue, and frees all allocated memory it
 * owns.
 */
void QUEUE_METHOD_CLEAR(QUEUE_TYPE * q);

/*
 * Pushes the given value onto the back of the queue, reallocating buffer space
 * if necessary. Returns 1 if successful, and 0 otherwise.
 */
int QUEUE_METHOD_PUSH(QUEUE_TYPE * q, VALUE_TYPE value);

/*
 * If the queue is non-empty, pops (erases) its front value and returns 1.
 * Otherwise, returns 0.
 */
int QUEUE_METHOD_POP(QUEUE_TYPE * q);

/*
 * If the queue is non-empty, stores its top value into `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int QUEUE_METHOD_PEEK(QUEUE_TYPE * q, VALUE_TYPE * value_out);

/*
 * If a value exists at the given queue index, stores its value in `*value_out` and returns 1
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 *
 * Note:
 *   An index of 0 is the front of the queue. The back of the queue is indexed
 *   by the queue's size minus one.
 */
int QUEUE_METHOD_AT(QUEUE_TYPE * q, VALUE_TYPE * value_out, int idx);

/*
 * Returns the number of elements in the queue
 */
#define QUEUE_METHOD_SIZE(_queue_) (((const QUEUE_TYPE *)_queue_)->size)

#endif
