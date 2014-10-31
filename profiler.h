#ifndef _PROFILER_H_
#define _PROFILER_H_

#define CONCAT_(A, B) A##B
#define CONCAT(A, B) CONCAT_(A, B)
#define QUOTE_(A) #A
#define QUOTE(A) QUOTE_(A)

#define PROFILER_ENTRY_NAME(NAME) CONCAT(__profile_entry_, NAME)

#ifdef ENABLE_PROFILING

#include <sys/time.h>
#include "log.h"

double getTimeDiff(const struct timeval* tv1, const struct timeval* tv2);

/*
 * Simple profiling.
 *
 * To use, simply add PROFILE_START(someName); and PROFILE_STOP(someName);
 * around the area that is supposed to be profiled. For example:
 *
 * PROFILER_START(fooBar);
 * foo((void*)bar);
 * PROFILER_STOP(fooBar);
 *
 * When PROFILER_STOP(...) is called, the time spent since PROFILER_START(...)
 * will be logged.
 */

#define PROFILER_NAME(NAME, N) CONCAT(__profile_, CONCAT(NAME, CONCAT(_, N)))
#define LOG_PROFILER(...) LOG_MUST(__VA_ARGS__)

#define PROFILE_START(NAME) \
    struct timeval PROFILER_NAME(NAME, 1);\
    gettimeofday(&PROFILER_NAME(NAME, 1), NULL)

#define PROFILE_STOP(NAME) \
    do {\
        struct timeval PROFILER_NAME(NAME, 2);			 \
        gettimeofday(&PROFILER_NAME(NAME, 2), NULL);		   \
        double d = getTimeDiff(&PROFILER_NAME(NAME, 1),		\
			       &PROFILER_NAME(NAME, 2));	       \
        LOG_PROFILER("Profiler result for %s: %f s", QUOTE(NAME), d);  \
    } while(0)

#define PROFILE_START_TMP() PROFILE_START_N(tmp)
#define PROFILE_STOP_TMP() PROFILE_STOP_N(tmp)

#endif /* ENABLE_PROFILING */

#endif /* _PROFILER_H_ */
