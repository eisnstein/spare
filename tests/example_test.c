#include <stdio.h>
#include <string.h>

#include "minunit.h"

char *test_example() { return NULL; }

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_example);

  return NULL;
}

RUN_TESTS(all_tests)