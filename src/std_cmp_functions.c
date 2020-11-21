#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "util.h"

void std_cmp(const char *string1, const char *string2, off_t size) {
  struct timespec start;
  struct timespec end;

  clock_gettime(0, &start);
  if (strcmp(string1, string2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strcmp: %.4f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (strncmp(string1, string2, size) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strncmp: %.4f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (memcmp(string1, string2, size) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("memcmp: %.4f\n", get_time_taken(&start, &end));
}