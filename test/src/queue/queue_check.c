
#include "iqueue.h"

#include <check.h>
#include <stdlib.h>

START_TEST(init) {
  iqueue_t queue;

  iqueue_init(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);

  iqueue_clear(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);
}
END_TEST

START_TEST(push_pop) {
  iqueue_t queue;

  iqueue_init(&queue);

  for(int k = 0 ; k < 100 ; k ++) {
    int N = rand() % 200;

    for(int i = 0 ; i < N ; i ++) {
      iqueue_push(&queue, i);
    }

    for(int i = 0 ; i < N ; i ++) {
      int value;
      ck_assert_int_eq(iqueue_peek(&queue, &value), 1);
      ck_assert_int_eq(value, i);
      ck_assert_int_eq(iqueue_pop(&queue), 1);
    }

    ck_assert_int_eq(iqueue_pop(&queue), 0);
    ck_assert_int_eq(queue.size, 0);
  }

  iqueue_clear(&queue);
}
END_TEST

Suite * queue_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("queue");

  tc = tcase_create("simple int");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push_pop);

  suite_add_tcase(s, tc);

  return s;
}

