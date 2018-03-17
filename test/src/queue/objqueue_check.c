
#include "obj_queue.h"

#include <check.h>
#include <stdlib.h>

START_TEST(init) {
  obj_queue_t queue;

  obj_queue_init(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);

  obj_queue_clear(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);
}
END_TEST

START_TEST(push_pop) {
  int N;
  obj_t * objects[200];

  obj_queue_t queue;

  obj_queue_init(&queue);

  for(int k = 0 ; k < 100 ; k ++) {
    N = rand() % 200;

    for(int i = 0 ; i < N ; i ++) {
      /* push an object */
      objects[i] = obj_queue_push(&queue);
      /* an object should have been created */
      ck_assert_ptr_nonnull(objects[i]);
      /* this many objects should exist */
      ck_assert_int_eq(obj_num(), i + 1);
    }

    for(int i = 0 ; i < N ; i ++) {
      /* should match what we saved */
      ck_assert_ptr_eq(obj_queue_peek(&queue), objects[i]);
      /* should been popped */
      ck_assert_int_eq(obj_queue_pop(&queue), 1);
      /* this many objects should exist */
      ck_assert_int_eq(obj_num(), N - i - 1);
    }

    /* popping shouldn't work anymore */
    ck_assert_int_eq(obj_queue_pop(&queue), 0);
  }

  obj_queue_clear(&queue);
}
END_TEST

Suite * objqueue_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("objqueue");

  tc = tcase_create("unnamed");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push_pop);

  suite_add_tcase(s, tc);

  return s;
}

