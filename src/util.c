#include <time.h>

double get_time_taken(struct timespec *start, struct timespec *end) {
  double time_taken;
  time_taken = (end->tv_sec - start->tv_sec) * 1e9;
  time_taken = (time_taken + (end->tv_nsec - start->tv_nsec)) * 1e-9;
  return time_taken;
}