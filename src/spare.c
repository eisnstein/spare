#define __USE_POSIX199309
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "custom_cmp_functions.h"
#include "std_cmp_functions.h"
#include "thread_cmp_functions.h"
#include "util.h"

int main(int argc, char const *argv[]) {
  FILE *f1 = NULL;
  FILE *f2 = NULL;

  die_(argc != 3, "Usage: ./build/spare <file1> <file2>");

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

  // standard compare
  printf("--- Standard compare\n");
  std_cmp(string1, string2, sizeFile1);

  // custom compare
  printf("\n--- Custom compare\n");
  custom_cmp(string1, string2, sizeFile1);

  // thread compare
  printf("\n--- Thread compare\n");
  thread_cmp(string1, string2, sizeFile1);

  free(string1);
  free(string2);

  return 0;

error:
  if (f1) fclose(f1);
  if (f2) fclose(f2);
  return -1;
}
