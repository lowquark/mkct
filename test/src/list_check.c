
#include "int_list.h"

#include <check.h>

START_TEST(init) {
  int_list_t list;

  int_list_init(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);

  int_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);
}
END_TEST

START_TEST(push) {
  int i;
  int_list_t list;
  int_list_node_t * node;

  int_list_init(&list);


  node = int_list_pushback(&list, 0);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  int_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);


  node = int_list_pushfront(&list, 0);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  int_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);
}
END_TEST

START_TEST(iterate) {
  int i;
  int_list_t list;
  int_list_node_t * node;

  int_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    int_list_pushback(&list, i);
  }

  node = int_list_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, i);

    node = int_list_next(node);
  }

  ck_assert_ptr_null(node);

  int_list_clear(&list);
}
END_TEST

START_TEST(iterate_reverse) {
  int i;
  int_list_t list;
  int_list_node_t * node;

  int_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    int_list_pushfront(&list, i);
  }

  node = int_list_last(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, i);

    node = int_list_prev(node);
  }

  ck_assert_ptr_null(node);

  int_list_clear(&list);
}
END_TEST

START_TEST(iterate_inverted) {
  int i;
  int_list_t list;
  int_list_node_t * node;

  int_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    int_list_pushfront(&list, i);
  }

  node = int_list_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, 100 - (i + 1));

    node = int_list_next(node);
  }

  ck_assert_ptr_null(node);

  int_list_clear(&list);
}
END_TEST

START_TEST(erase_even) {
  // add values ints from 0 to 99 to a list
  int_list_t list;

  int_list_init(&list);

  for(int i = 0 ; i < 100 ; i ++) {
    int_list_pushback(&list, i);
  }

  // erase even numbers
  int_list_node_t * node = int_list_first(&list);

  while(node) {
    if((node->value % 2) == 0) {
      int_list_node_t * next = int_list_next(node);
      int_list_erase(node);
      node = next;
    } else {
      node = int_list_next(node);
    }
  }

  // should only contain odd numbers
  int i = 0;
  node = int_list_first(&list);

  while(node) {
    ck_assert_int_eq(node->value, i * 2 + 1);

    i ++;
    node = int_list_next(node);
  }

  int_list_clear(&list);
}
END_TEST

Suite * list_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("list");

  tc = tcase_create("simple int");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push);
  tcase_add_test(tc, iterate);
  tcase_add_test(tc, iterate_reverse);
  tcase_add_test(tc, iterate_inverted);
  tcase_add_test(tc, erase_even);

  suite_add_tcase(s, tc);

  return s;
}

