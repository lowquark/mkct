
#include "obj_list.h"

#include <check.h>

START_TEST(init) {
  obj_list_t list;

  obj_list_init(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);

  obj_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);
}
END_TEST

START_TEST(push) {
  obj_list_t list;
  obj_list_node_t * node;

  obj_list_init(&list);


  node = obj_list_pushback(&list);

  ck_assert_ptr_nonnull(node);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  ck_assert_int_eq(obj_num(), 1);

  obj_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);

  ck_assert_int_eq(obj_num(), 0);


  node = obj_list_pushfront(&list);

  ck_assert_ptr_nonnull(node);

  ck_assert_ptr_eq(list.next, &node->list);
  ck_assert_ptr_eq(list.prev, &node->list);
  ck_assert_ptr_eq(list.head, &list);

  ck_assert_int_eq(obj_num(), 1);

  obj_list_clear(&list);

  ck_assert_ptr_eq(list.next, &list);
  ck_assert_ptr_eq(list.prev, &list);
  ck_assert_ptr_eq(list.head, &list);

  ck_assert_int_eq(obj_num(), 0);
}
END_TEST

START_TEST(iterate) {
  int i;
  obj_list_t list;
  obj_list_node_t * node;

  obj_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    node = obj_list_pushback(&list);
    obj_list_value(node)->a = i;
    obj_list_value(node)->b = i;
    obj_list_value(node)->c = i;
  }

  node = obj_list_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(obj_list_value(node)->a, i);
    ck_assert_int_eq(obj_list_value(node)->b, i);
    ck_assert_int_eq(obj_list_value(node)->c, i);

    node = obj_list_next(node);
  }

  ck_assert_ptr_null(node);

  obj_list_clear(&list);
}
END_TEST

START_TEST(iterate_reverse) {
  int i;
  obj_list_t list;
  obj_list_node_t * node;

  obj_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    node = obj_list_pushfront(&list);
    obj_list_value(node)->a = i;
    obj_list_value(node)->b = i;
    obj_list_value(node)->c = i;
  }

  node = obj_list_last(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(obj_list_value(node)->a, i);
    ck_assert_int_eq(obj_list_value(node)->b, i);
    ck_assert_int_eq(obj_list_value(node)->c, i);

    node = obj_list_prev(node);
  }

  ck_assert_ptr_null(node);

  obj_list_clear(&list);
}
END_TEST

START_TEST(iterate_inverted) {
  int i;
  obj_list_t list;
  obj_list_node_t * node;

  obj_list_init(&list);

  for(i = 0 ; i < 100 ; i ++) {
    node = obj_list_pushfront(&list);
    obj_list_value(node)->a = i;
    obj_list_value(node)->b = i;
    obj_list_value(node)->c = i;
  }

  node = obj_list_first(&list);

  for(i = 0 ; i < 100 ; i ++) {
    ck_assert_ptr_nonnull(node);

    ck_assert_int_eq(obj_list_value(node)->a, 100 - (i + 1));
    ck_assert_int_eq(obj_list_value(node)->b, 100 - (i + 1));
    ck_assert_int_eq(obj_list_value(node)->c, 100 - (i + 1));

    node = obj_list_next(node);
  }

  ck_assert_ptr_null(node);

  obj_list_clear(&list);
}
END_TEST

START_TEST(erase_even) {
  obj_list_node_t * node;

  // add values ints from 0 to 99 to a list
  obj_list_t list;

  obj_list_init(&list);

  for(int i = 0 ; i < 100 ; i ++) {
    node = obj_list_pushback(&list);
    obj_list_value(node)->a = i;
    obj_list_value(node)->b = i;
    obj_list_value(node)->c = i;
  }

  // erase even numbers
  node = obj_list_first(&list);

  while(node) {
    if((obj_list_value(node)->a % 2) == 0) {
      obj_list_node_t * next = obj_list_next(node);
      obj_list_erase(node);
      node = next;
    } else {
      node = obj_list_next(node);
    }
  }

  // should only contain odd numbers
  int i = 0;
  node = obj_list_first(&list);

  while(node) {
    ck_assert_int_eq(obj_list_value(node)->a, i * 2 + 1);
    ck_assert_int_eq(obj_list_value(node)->b, i * 2 + 1);
    ck_assert_int_eq(obj_list_value(node)->c, i * 2 + 1);

    i ++;
    node = obj_list_next(node);
  }

  obj_list_clear(&list);
}
END_TEST

Suite * objlist_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("objlist");

  tc = tcase_create("unnamed");

  tcase_add_test(tc, init);
  tcase_add_test(tc, push);
  tcase_add_test(tc, iterate);
  tcase_add_test(tc, iterate_reverse);
  tcase_add_test(tc, iterate_inverted);
  tcase_add_test(tc, erase_even);

  suite_add_tcase(s, tc);

  return s;
}

