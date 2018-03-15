
#include "int_queue.h"

#include <check.h>
#include <stdlib.h>

START_TEST(init) {
  int_queue_t queue;

  int_queue_init(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);

  int_queue_clear(&queue);

  ck_assert_ptr_null(queue.buffer_begin);
  ck_assert_ptr_null(queue.buffer_end);
  ck_assert_ptr_null(queue.getptr);
  ck_assert_ptr_null(queue.putptr);
  ck_assert_int_eq(queue.size, 0);
}
END_TEST

START_TEST(push_pop) {
  int_queue_t queue;

  int_queue_init(&queue);

  for(int k = 0 ; k < 100 ; k ++) {
    int N = rand() % 200;

    for(int i = 0 ; i < N ; i ++) {
      ck_assert_int_eq(int_queue_size(&queue), i);
      int_queue_push(&queue, i);
    }

    ck_assert_int_eq(int_queue_size(&queue), N);

    for(int i = 0 ; i < N ; i ++) {
      int value;
      ck_assert_int_eq(int_queue_peek(&queue, &value), 1);
      ck_assert_int_eq(value, i);
      ck_assert_int_eq(int_queue_pop(&queue), 1);

      ck_assert_int_eq(int_queue_size(&queue), N - i - 1);
    }

    ck_assert_int_eq(int_queue_pop(&queue), 0);
    ck_assert_int_eq(int_queue_size(&queue), 0);
  }

  int_queue_clear(&queue);
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

