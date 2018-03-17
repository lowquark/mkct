#ifndef _OBJ_QUEUE_H_
#define _OBJ_QUEUE_H_

/*
 * FIFO queue of `int`s. Values are copied, not referenced.
 */
typedef struct obj_queue {
  int * buffer_begin;
  int * buffer_end;

  int * getptr;
  int * putptr;

  long size;
} obj_queue_t;

/*
 * Initializes the given `obj_queue_t` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use obj_queue_clear to pop all values
 * from the queue.
 */
void obj_queue_init(obj_queue_t * q);

/*
 * Pops all values present in the queue, and frees all allocated memory it
 * owns.
 */
void obj_queue_clear(obj_queue_t * q);

/*
 * Pushes the given value onto the back of the queue, reallocating buffer space
 * if necessary. Returns 1 if successful, and 0 otherwise.
 */
int obj_queue_push(obj_queue_t * q, int value);

/*
 * If the queue is non-empty, pops (erases) its front value and returns 1.
 * Otherwise, returns 0.
 */
int obj_queue_pop(obj_queue_t * q);

/*
 * If the queue is non-empty, stores its top value into `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int obj_queue_peek(obj_queue_t * q, int * value_out);

/*
 * If a value exists at the given queue index, stores its value in `*value_out` and returns 1
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 *
 * Note:
 *   An index of 0 is the front of the queue. The back of the queue is indexed
 *   by the queue's size minus one.
 */
int obj_queue_at(obj_queue_t * q, int * value_out, int idx);

/*
 * Returns the number of elements in the queue
 */
#define obj_queue_size(_queue_) (((const obj_queue_t *)_queue_)->size)

#endif
