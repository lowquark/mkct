
#include "obj_stack.h"

#include <check.h>
#include <stdlib.h>

START_TEST(init) {
  obj_stack_t stack;

  obj_stack_init(&stack);

  ck_assert_ptr_null(stack.buffer_begin);
  ck_assert_ptr_null(stack.buffer_end);
  ck_assert_ptr_null(stack.putptr);
  ck_assert_int_eq(stack.size, 0);

  obj_stack_clear(&stack);

  ck_assert_ptr_null(stack.buffer_begin);
  ck_assert_ptr_null(stack.buffer_end);
  ck_assert_ptr_null(stack.putptr);
  ck_assert_int_eq(stack.size, 0);
}
END_TEST

START_TEST(push_pop) {
  int N;
  obj_t * objects[200];

  obj_stack_t stack;

  obj_stack_init(&stack);

  for(int k = 0 ; k < 100 ; k ++) {
    N = rand() % 200;

    for(int i = 0 ; i < N ; i ++) {
      /* push an object */
      objects[i] = obj_stack_push(&stack);
      /* an object should have been created */
      ck_assert_ptr_nonnull(objects[i]);
      /* this many objects should exist */
      ck_assert_int_eq(obj_num(), i + 1);
    }

    for(int i = 0 ; i < N ; i ++) {
      /* should match what we saved, in reverse order */
      ck_assert_ptr_eq(obj_stack_peek(&stack), objects[N - i - 1]);
      /* popping should work */
      ck_assert_int_eq(obj_stack_pop(&stack), 1);
      /* this many objects should exist */
      ck_assert_int_eq(obj_num(), N - i - 1);
    }

    /* popping shouldn't work anymore */
    ck_assert_int_eq(obj_stack_pop(&stack), 0);
  }

  obj_stack_clear(&stack);
}
END_TEST

Suite * objstack_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("objstack");

  tc = tcase_create("unnamed");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push_pop);

  suite_add_tcase(s, tc);

  return s;
}

