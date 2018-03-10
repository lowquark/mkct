
#include "iimap.h"

#include <check.h>
#include <stdlib.h>
#include <time.h>

static void erase_randomly_cycle(iimap_t * map) {
  struct test_entry_t {
    int salt;
    int key;
    _Bool set;
  };

  struct test_entry_t tests[100];

  // set several entries in the map
  for(int i = 0 ; i < 100 ; i ++) {
    int salt = rand();

    // note: impossible to have duplicates, but still very sparse
    int key = (int)(100 * i + (rand() % 100));

    iimap_set(map, key, salt);

    tests[i].salt = salt;
    tests[i].key = key;
    tests[i].set = 1;
  }

  // affirm that data is intact, values match those set
  for(int i = 0 ; i < 100 ; i ++) {
    int key = tests[i].key;
    int value;

    ck_assert_int_eq(iimap_get(map, key, &value), 1);

    ck_assert_int_eq(value, tests[i].salt);
  }

  // erase several entries
  for(int i = 0 ; i < 100 ; i ++) {
    int key = tests[i].key;

    if((rand() % 10) == 0) {
      iimap_erase(map, key);
      tests[i].set = 0;
    }
  }

  // affirm that those that were erased are gone,
  // affirm that those that remain are still valid
  for(int i = 0 ; i < 100 ; i ++) {
    int key = tests[i].key;
    int value;

    if(iimap_get(map, key, &value)) {
      ck_assert_int_eq(tests[i].set, 1);
      ck_assert_int_eq(tests[i].salt, value);
    } else {
      ck_assert_int_eq(tests[i].set, 0);
    }
  }

  // erase remaining entries
  for(int i = 0 ; i < 100 ; i ++) {
    if(tests[i].set) {
      int key = tests[i].key;

      iimap_erase(map, key);

      tests[i].set = 0;
    }
  }

  // affirm that all entries have been erased
  for(int i = 0 ; i < 100 ; i ++) {
    int key = tests[i].key;
    int value;

    ck_assert_int_eq(iimap_get(map, key, &value), 0);
  }
}

START_TEST(erase_randomly) {
  srand((unsigned int)time(NULL));

  iimap_t map;

  iimap_init(&map);

  for(int i = 0 ; i < 100 ; i ++) {
    erase_randomly_cycle(&map);
  }

  iimap_clear(&map);
}
END_TEST

Suite * map_check(void) {
  Suite * s;
  TCase * tc;

  s = suite_create("map");

  tc = tcase_create("int->int map");

  tcase_add_test(tc, erase_randomly);

  //tcase_add_test(tc, push);
  //tcase_add_test(tc, iterate);
  //tcase_add_test(tc, erase_even);

  suite_add_tcase(s, tc);

  return s;
}

