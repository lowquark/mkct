#ifndef _INT_QUEUE_H_
#define _INT_QUEUE_H_

/*
 * FIFO queue of `int`s
 */
typedef struct int_queue {
  int * buffer_begin;
  int * buffer_end;

  int * getptr;
  int * putptr;

  long size;
} int_queue_t;

/*
 * Initializes the given queue to a valid state.
 */
void int_queue_init(int_queue_t * q);

/*
 * Deletes all values present in the queue.
 */
void int_queue_clear(int_queue_t * q);

/*
 * Pushes a value onto the back of the queue. Reallocates buffer space if necessary.
 *
 * Return Value:
 *   Returns 1 if successful, and 0 if memory allocation failed.
 */
int int_queue_push(int_queue_t * q, int value);

/*
 * Pops a value from the front of the queue.
 *
 * Return Value:
 *   Returns 1 if a value was popped, and 0 if the queue was empty.
 */
int int_queue_pop(int_queue_t * q);

/*
 * Copies the value from the front of the queue into `value_out`.
 *
 * Return Value:
 *   If the queue is empty, returns 0 and leaves `value_out` unmodified.
 *   Returns 1 otherwise.
 */
int int_queue_peek(int_queue_t * q, int * value_out);

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
int int_queue_at(int_queue_t * q, int * value_out, int idx);

#endif
