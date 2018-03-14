#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * FIFO queue of `OBJECT_TYPE`s
 */
typedef struct OBJQUEUE_STRUCT {
  OBJECT_TYPE ** buffer_begin;
  OBJECT_TYPE ** buffer_end;

  OBJECT_TYPE ** getptr;
  OBJECT_TYPE ** putptr;

  long size;
} OBJQUEUE_TYPE;

/*
 * Initializes the given queue to a valid state.
 */
void OBJQUEUE_METHOD_INIT(OBJQUEUE_TYPE * q);

/*
 * Deletes all values present in the queue. Frees all associated memory.
 */
void OBJQUEUE_METHOD_CLEAR(OBJQUEUE_TYPE * q);

OBJECT_TYPE * OBJQUEUE_METHOD_PUSH(OBJQUEUE_TYPE * q);

int OBJQUEUE_METHOD_POP(OBJQUEUE_TYPE * q);

OBJECT_TYPE * OBJQUEUE_METHOD_PEEK(OBJQUEUE_TYPE * q);

OBJECT_TYPE * OBJQUEUE_METHOD_AT(OBJQUEUE_TYPE * q, long idx);

long OBJQUEUE_METHOD_SIZE(OBJQUEUE_TYPE * q);

#endif
