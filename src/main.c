#define __USE_POSIX199309
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "util.h"

double get_time_taken(struct timespec *start, struct timespec *end) {
  double time_taken;
  time_taken = (end->tv_sec - start->tv_sec) * 1e9;
  time_taken = (time_taken + (end->tv_nsec - start->tv_nsec)) * 1e-9;
  return time_taken;
}

bool naive_compare(const char *str1, const char *str2, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (str1[i] != str2[i]) return false;
  }

  return true;
}

bool front_back_compare(char *str1, char *str2, size_t len) {
  char *frontStr1 = str1;
  char *backStr1 = str1 + len - 1;
  char *frontStr2 = str2;
  char *backStr2 = str2 + len - 1;

  size_t middle = len / 2;
  for (size_t i = 0; i < middle; ++i) {
    if (*frontStr1 != *frontStr2) return false;
    if (*backStr1 != *backStr2) return false;

    frontStr1++;
    backStr1--;
    frontStr2++;
    backStr2--;
  }

  return false;
}

int main(int argc, char const *argv[]) {
  FILE *f1 = NULL;
  FILE *f2 = NULL;

  die_(argc != 3, "Usage: spare <file1> <file2>");

  debug_v_("File 1: %s", argv[1]);
  debug_v_("File 2: %s", argv[2]);

  f1 = fopen(argv[1], "r");
  throw_v_(f1 == NULL, "Could not open file: %s", argv[1]);

  f2 = fopen(argv[2], "r");
  throw_v_(f2 == NULL, "Could not open file: %s", argv[2]);

  fseek(f1, 0, SEEK_END);
  off_t sizeFile1 = ftell(f1);
  fseek(f2, 0, SEEK_END);
  off_t sizeFile2 = ftell(f2);

  debug_v_("File size: %ld", sizeFile1);
  debug_v_("File size: %ld", sizeFile2);

  fseek(f1, 0, SEEK_SET);
  fseek(f2, 0, SEEK_SET);

  char *string1 = malloc(sizeof(char) * sizeFile1 + 1);
  throw_mem_(string1);
  char *string2 = malloc(sizeof(char) * sizeFile2 + 1);
  throw_mem_(string2);

  fread(string1, sizeof(char), sizeFile1, f1);
  string1[sizeFile1] = '\0';
  fread(string2, sizeof(char), sizeFile2, f2);
  string2[sizeFile2] = '\0';

  fclose(f2);
  fclose(f1);

  struct timespec start;
  struct timespec end;

  clock_gettime(0, &start);
  if (strcmp(string1, string2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strcmp: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (strncmp(string1, string2, sizeFile2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strncmp: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (memcmp(string1, string2, sizeFile2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("memcmp: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (naive_compare(string1, string2, sizeFile2) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("naive_compare: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (front_back_compare(string1, string2, sizeFile2) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("front_back_compare: %f\n", get_time_taken(&start, &end));

  // last item of string2 is different
  printf("---\n");
  string2[sizeFile2 - 1] = 'x';

  clock_gettime(0, &start);
  if (strcmp(string1, string2) == 0) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("strcmp: %f\n", get_time_taken(&start, &end));

  clock_gettime(0, &start);
  if (front_back_compare(string1, string2, sizeFile2) == true) {
    printf("e : ");
  } else {
    printf("n : ");
  }
  clock_gettime(0, &end);
  printf("front_back_compare: %f\n", get_time_taken(&start, &end));

  free(string1);
  free(string2);

  return 0;

error:
  if (f2) fclose(f2);
  return -1;
}
