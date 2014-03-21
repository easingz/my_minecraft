#include "profiler.h"

#ifdef ENABLE_PROFILING

double getTimeDiff(const struct timeval* tv1, const struct timeval* tv2) {
  if (tv1->tv_sec == tv2->tv_sec)
    {
      return (tv2->tv_usec / 1000000.0f) - (tv1->tv_usec / 1000000.0f);
    }
  else
    {
      return ((double)tv2->tv_sec + (tv2->tv_usec / 1000000.0f)) -
	((double)tv1->tv_sec + (tv1->tv_usec / 1000000.0f));
    }
}
#endif /* ENABLE_PROFILING */
