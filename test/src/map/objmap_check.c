
#include "int_obj_map.h"

#include <check.h>
#include <stdlib.h>
#include <time.h>


START_TEST(init_deinit) {
  int_obj_map_t map;

  int_obj_map_init(&map);

  // create a slew of objects
  for(int i = 0 ; i < 100 ; i ++) {
    obj_t * o = int_obj_map_create(&map, i);

    // i + 1 `obj_t`s in existence
    ck_assert_int_eq(obj_num(), i + 1);

    // object state valid
    ck_assert_int_eq(o->a, OBJ_INITIAL_A);
    ck_assert_int_eq(o->b, OBJ_INITIAL_B);
    ck_assert_int_eq(o->c, OBJ_INITIAL_C);

    o->a = i*50;
    o->b = i*500;
    o->c = i;
  }

  for(int i = 0 ; i < 100 ; i ++) {
    obj_t * o = int_obj_map_find(&map, i);

    ck_assert_ptr_nonnull(o);

    // object state still valid
    ck_assert_int_eq(o->a, i*50);
    ck_assert_int_eq(o->b, i*500);
    ck_assert_int_eq(o->c, i);
  }

  int_obj_map_clear(&map);

  // no obj_t in existence
  ck_assert_int_eq(obj_num(), 0);
}
END_TEST

START_TEST(unique_values) {
  // Algorithm is O(N*N), watch out
  static const int N = 100;

  int_obj_map_t map;

  obj_t ** objs = calloc(N, sizeof(obj_t *));


  int_obj_map_init(&map);

  for(int i = 0 ; i < N ; i ++) {
    objs[i] = int_obj_map_create(&map, i);

    // pointer shouldn't match any other
    for(int k = 0 ; k < i ; k ++) {
      ck_assert_ptr_ne(objs[i], objs[k]);
    }
  }

  free(objs);

  int_obj_map_clear(&map);
}
END_TEST

START_TEST(consistent_create_and_find) {
  static const int N = 10000;

  srand((unsigned int)time(NULL));

  int_obj_map_t map;

  int_obj_map_init(&map);

  for(int i = 0 ; i < N ; i ++) {
    int key = rand();

    obj_t * new_obj = int_obj_map_create(&map, key);

    obj_t * found_obj = int_obj_map_find(&map, key);

    ck_assert_ptr_eq(new_obj, found_obj);
  }

  int_obj_map_clear(&map);
}
END_TEST

START_TEST(consistent_clear_and_find) {
  static const int N = 10000;

  srand((unsigned int)time(NULL));

  int_obj_map_t map;

  int_obj_map_init(&map);

  // create a slew of objects
  for(int i = 0 ; i < N ; i ++) {
    obj_t * new_obj = int_obj_map_create(&map, i*507);

    obj_t * found_obj = int_obj_map_find(&map, i*507);

    ck_assert_ptr_eq(new_obj, found_obj);
  }

  int_obj_map_clear(&map);

  // shouldn't find any
  for(int i = 0 ; i < N ; i ++) {
    obj_t * found_obj = int_obj_map_find(&map, i*507);

    ck_assert_ptr_null(found_obj);
  }
}
END_TEST

START_TEST(consistent_destroy_and_find) {
  static const int N = 10000;

  srand((unsigned int)time(NULL));

  int_obj_map_t map;

  int_obj_map_init(&map);

  int    * keys    = calloc(N, sizeof(int));
  int    * indices = calloc(N, sizeof(int));
  obj_t ** objs    = calloc(N, sizeof(obj_t *));

  for(int i = 0 ; i < N ; i ++) {
    keys[i]    = rand();
    indices[i] = i;
    objs[i]    = int_obj_map_create(&map, keys[i]);

    ck_assert_ptr_nonnull(objs[i]);
  }

  // generate random permutation of [0, N)
  for(int k = N - 1 ; k >= 1 ; k --) {
    int rand_idx = rand() % (k + 1);

    // swap indices[rand_idx] and indices[k]
    int tmp_idx = indices[rand_idx];
    indices[rand_idx] = indices[k];
    indices[k] = tmp_idx;
  }

  // erase half in a random order
  for(int i = 0 ; i < N / 2 ; i ++) {
    int idx = indices[i];

    // should successfully erase
    ck_assert_int_eq(int_obj_map_destroy(&map, keys[idx]), 1);

    // try to find it, should fail
    ck_assert_ptr_null(int_obj_map_find(&map, keys[idx]));
  }

  // verify remaining still intact
  for(int i = N / 2 ; i < N ; i ++) {
    int idx = indices[i];

    // try to find it, should match
    ck_assert_ptr_eq(int_obj_map_find(&map, keys[idx]), objs[idx]);
  }

  // erase remaining in a random order
  for(int i = N / 2 ; i < N ; i ++) {
    int idx = indices[i];

    // should successfully erase
    ck_assert_int_eq(int_obj_map_destroy(&map, keys[idx]), 1);

    // try to find it, should fail
    ck_assert_ptr_null(int_obj_map_find(&map, keys[idx]));
  }

  free(keys);
  free(indices);
  free(objs);

  int_obj_map_clear(&map);
}
END_TEST


Suite * objmap_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("objmap");

  tc = tcase_create("unnamed");

  // correct object handling
  tcase_add_test(tc, init_deinit);

  // correct map functionality
  tcase_add_test(tc, unique_values);
  tcase_add_test(tc, consistent_create_and_find);
  tcase_add_test(tc, consistent_destroy_and_find);
  tcase_add_test(tc, consistent_clear_and_find);

  suite_add_tcase(s, tc);

  return s;
}

