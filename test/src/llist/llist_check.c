
#include "ilist.h"

#include <check.h>

START_TEST(init) {
  ilist_t list;

  ilist_init(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);

  ilist_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);
}
END_TEST

START_TEST(push) {
  int i;
  ilist_t list;
  ilist_node_t * node;

  ilist_init(&list);


  node = ilist_pushback(&list, 0);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  ilist_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);


  node = ilist_pushfront(&list, 0);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  ilist_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);
}
END_TEST

START_TEST(iterate) {
  int i;
  ilist_t list;
  ilist_node_t * node;

  ilist_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ilist_pushback(&list, i);
  }

  node = ilist_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, i);

    node = ilist_next(node);
  }

  ck_assert_ptr_null(node);

  ilist_clear(&list);
}
END_TEST

START_TEST(iterate_reverse) {
  int i;
  ilist_t list;
  ilist_node_t * node;

  ilist_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ilist_pushfront(&list, i);
  }

  node = ilist_last(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, i);

    node = ilist_prev(node);
  }

  ck_assert_ptr_null(node);

  ilist_clear(&list);
}
END_TEST

START_TEST(iterate_inverted) {
  int i;
  ilist_t list;
  ilist_node_t * node;

  ilist_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ilist_pushfront(&list, i);
  }

  node = ilist_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(node->value, 100 - (i + 1));

    node = ilist_next(node);
  }

  ck_assert_ptr_null(node);

  ilist_clear(&list);
}
END_TEST

START_TEST(erase_even) {
  // add values ints from 0 to 99 to a list
  ilist_t list;

  ilist_init(&list);

  for(int i = 0 ; i < 100 ; i ++) {
    ilist_pushback(&list, i);
  }

  // erase even numbers
  ilist_node_t * node = ilist_first(&list);

  while(node) {
    if((node->value % 2) == 0) {
      ilist_node_t * next = ilist_next(node);
      ilist_erase(node);
      node = next;
    } else {
      node = ilist_next(node);
    }
  }

  // should only contain odd numbers
  int i = 0;
  node = ilist_first(&list);

  while(node) {
    ck_assert_int_eq(node->value, i * 2 + 1);

    i ++;
    node = ilist_next(node);
  }

  ilist_clear(&list);
}
END_TEST

Suite * llist_check(void) {
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

