#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "util.h"

bool naive_compare(const char *str1, const char *str2, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (str1[i] == '\0' || str1[i] != str2[i]) return false;
  }

  return true;
}

int my_strcmp(const char *p1, const char *p2) {
  const unsigned char *s1 = (const unsigned char *)p1;
  const unsigned char *s2 = (const unsigned char *)p2;
  unsigned char c1, c2;

  do {
    c1 = (unsigned char)*s1++;
    c2 = (unsigned char)*s2++;
    if (c1 == '\0') {
      return c1 - c2;
    }
  } while (c1 == c2);

  return c1 - c2;
}

bool front_back_compare(char *str1, char *str2, off_t len) {
  char *frontStr1 = str1;
  char *backStr1 = str1 + len - 1;
  char *frontStr2 = str2;
  char *backStr2 = str2 + len - 1;

  off_t middle = len / 2;
  for (off_t i = 0; i < middle; ++i) {
    if (*frontStr1 != *frontStr2) return false;
    if (*backStr1 != *backStr2) return false;

    frontStr1++;
    backStr1--;
    frontStr2++;
    backStr2--;
  }

  return true;
}

void custom_cmp(const char *string1, const char *string2, off_t size) {
  struct timespec start;
  struct timespec end;

  clock_gettime(0, &start);
  if (naive_compare(string1, string2, size) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("naive_compare: %.4f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (my_strcmp(string1, string2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("my_strcmp: %.4f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (front_back_compare(string1, string2, size) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("front_back_compare: %.4f\n", get_time_taken(&start, &end));

  // last item of string2 is different
  /*printf("---\n");
  char save = string2[size - 1];
  string2[size - 1] = 'x';

  clock_gettime(0, &start);
  if (strcmp(string1, string2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strcmp: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (front_back_compare(string1, string2, size) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("front_back_compare: %f\n", get_time_taken(&start, &end));*/
}