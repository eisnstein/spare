#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "util.h"

typedef struct cmp_data {
  bool result;
  size_t from;
  size_t count;
  char *string1;
  char *string2;
} cmp_data;

void *thread_strncmp(void *data) {
  cmp_data *d = data;
  const char *from1 = d->string1 + d->from;
  const char *from2 = d->string2 + d->from;

  if (strncmp(from1, from2, d->count) == 0) {
    d->result = true;
  } else {
    d->result = false;
  }

  return NULL;
}

void *thread_naive_compare(void *data) {
  cmp_data *d = data;
  const char *from1 = d->string1 + d->from;
  const char *from2 = d->string2 + d->from;

  for (size_t i = 0; i < d->count; ++i) {
    if (from1[i] == '\0' || from1[i] != from2[i]) {
      d->result = false;
      return NULL;
    }
  }

  d->result = true;

  return NULL;
}

void *thread_front_back_compare(void *data) {
  cmp_data *d = data;
  char *frontStr1 = d->string1 + d->from;
  char *backStr1 = frontStr1 + d->count;
  char *frontStr2 = d->string2 + d->from;
  char *backStr2 = frontStr2 + d->count;
  pthread_t id = pthread_self();

  size_t middle = d->count / 2;
  for (size_t i = 0; i < middle; ++i) {
    if (*frontStr1 != *frontStr2) {
      return NULL;
    }
    if (*backStr1 != *backStr2) {
      return NULL;
    }

    frontStr1++;
    backStr1--;
    frontStr2++;
    backStr2--;
  }

  d->result = true;

  return NULL;
}

void thread_cmp(const char *string1, const char *string2, off_t size) {
  struct timespec start;
  struct timespec end;

  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  cmp_data data_for_1 = {
      .result = false,
      .from = 0,
      .count = 1000000000,
      .string1 = string1,
      .string2 = string2,
  };
  cmp_data data_for_2 = {
      .result = false,
      .from = 1000000000,
      .count = 1000000000,
      .string1 = string1,
      .string2 = string2,
  };
  cmp_data data_for_3 = {
      .result = false,
      .from = 2000000000,
      .count = 1000000000,
      .string1 = string1,
      .string2 = string2,
  };

  clock_gettime(0, &start);

  pthread_create(&thread1, NULL, thread_strncmp, &data_for_1);
  pthread_create(&thread2, NULL, thread_strncmp, &data_for_2);
  pthread_create(&thread3, NULL, thread_strncmp, &data_for_3);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  clock_gettime(0, &end);

  if (data_for_1.result == true && data_for_2.result == true &&
      data_for_3.result == true) {
    printf("e : thread_strncmp: %.4f\n", get_time_taken(&start, &end));
  } else {
    printf("n : thread_strncmp: %.4f\n", get_time_taken(&start, &end));
  }

  data_for_1.result = false;
  data_for_2.result = false;
  data_for_3.result = false;

  clock_gettime(0, &start);

  pthread_create(&thread1, NULL, thread_naive_compare, &data_for_1);
  pthread_create(&thread2, NULL, thread_naive_compare, &data_for_2);
  pthread_create(&thread3, NULL, thread_naive_compare, &data_for_3);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  clock_gettime(0, &end);

  if (data_for_1.result == true && data_for_2.result == true &&
      data_for_3.result == true) {
    printf("e : thread_naive_compare: %.4f\n", get_time_taken(&start, &end));
  } else {
    printf("n : thread_naive_compare: %.4f\n", get_time_taken(&start, &end));
  }

  data_for_1.result = false;
  data_for_2.result = false;
  data_for_3.result = false;

  clock_gettime(0, &start);

  pthread_create(&thread1, NULL, thread_front_back_compare, &data_for_1);
  pthread_create(&thread2, NULL, thread_front_back_compare, &data_for_2);
  pthread_create(&thread3, NULL, thread_front_back_compare, &data_for_3);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  clock_gettime(0, &end);

  if (data_for_1.result == true && data_for_2.result == true &&
      data_for_3.result == true) {
    printf("e : thread_front_back_compare: %.4f\n",
           get_time_taken(&start, &end));
  } else {
    printf("n : thread_front_back_compare: %.4f\n",
           get_time_taken(&start, &end));
  }
}