#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * FIFO queue of `VALUE_TYPE`s
 */
typedef struct QUEUE_STRUCT {
  VALUE_TYPE * buffer_begin;
  VALUE_TYPE * buffer_end;

  VALUE_TYPE * getptr;
  VALUE_TYPE * putptr;

  long size;
} QUEUE_TYPE;

/*
 * Initializes the given queue to a valid state.
 */
void QUEUE_METHOD_INIT(QUEUE_TYPE * q);

/*
 * Deletes all values present in the queue.
 */
void QUEUE_METHOD_CLEAR(QUEUE_TYPE * q);

/*
 * Pushes a value onto the back of the queue. Reallocates buffer space if necessary.
 *
 * Return Value:
 *   Returns 1 if successful, and 0 if memory allocation failed.
 */
int QUEUE_METHOD_PUSH(QUEUE_TYPE * q, VALUE_TYPE value);

/*
 * Pops a value from the front of the queue.
 *
 * Return Value:
 *   Returns 1 if a value was popped, and 0 if the queue was empty.
 */
int QUEUE_METHOD_POP(QUEUE_TYPE * q);

/*
 * Copies the value from the front of the queue into `value_out`.
 *
 * Return Value:
 *   If the queue is empty, returns 0 and leaves `value_out` unmodified.
 *   Returns 1 otherwise.
 */
int QUEUE_METHOD_PEEK(QUEUE_TYPE * q, VALUE_TYPE * value_out);

/*
 * Copies the value from the queue at the given index `idx` into `value_out`.
 *
 * Return Value:
 *   If the queue is empty, or if idx is larger than the queue's size, returns
 *   0 and leaves `value_out` unmodified. Returns 1 otherwise.
 *
 * Note:
 *   An index of 0 is the front of the queue. The back of the queue is indexed
 *   by the queue's size minus one.
 */
int QUEUE_METHOD_AT(QUEUE_TYPE * q, VALUE_TYPE * value_out, int idx);

#endif
