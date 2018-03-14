
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

extern Suite * list_check(void);
extern Suite * queue_check(void);
extern Suite * map_check(void);
//extern Suite * objmap_check(void);

int run_suite(Suite * suite) {
  int number_failed;
  SRunner * sr;

  sr = srunner_create(suite);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed;
}

int main(int argc, char ** argv) {
  int number_failed;

  number_failed += run_suite(list_check());
  number_failed += run_suite(queue_check());
  number_failed += run_suite(map_check());
  //number_failed += run_suite(objmap_check());

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

