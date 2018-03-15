
#include "int_stack.h"

#include <check.h>
#include <stdlib.h>

START_TEST(init) {
  int_stack_t stack;

  int_stack_init(&stack);

  ck_assert_ptr_null(stack.buffer_begin);
  ck_assert_ptr_null(stack.buffer_end);
  ck_assert_ptr_null(stack.putptr);
  ck_assert_int_eq(stack.size, 0);

  int_stack_clear(&stack);

  ck_assert_ptr_null(stack.buffer_begin);
  ck_assert_ptr_null(stack.buffer_end);
  ck_assert_ptr_null(stack.putptr);
  ck_assert_int_eq(stack.size, 0);
}
END_TEST

START_TEST(push_pop) {
  int_stack_t stack;

  int_stack_init(&stack);

  for(int k = 0 ; k < 100 ; k ++) {
    int N = rand() % 200;

    for(int i = 0 ; i < N ; i ++) {
      int_stack_push(&stack, i);
    }

    for(int i = N - 1 ; i >= 0 ; i --) {
      int value;
      ck_assert_int_eq(int_stack_top(&stack, &value), 1);
      ck_assert_int_eq(value, i);
      ck_assert_int_eq(int_stack_pop(&stack), 1);
    }

    ck_assert_int_eq(int_stack_pop(&stack), 0);
    ck_assert_int_eq(int_stack_size(&stack), 0);
  }

  int_stack_clear(&stack);
}
END_TEST

Suite * stack_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("stack");

  tc = tcase_create("simple int");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push_pop);

  suite_add_tcase(s, tc);

  return s;
}

