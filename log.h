#ifndef _LOG_H_
#define _LOG_H_

#ifdef DEBUG_ENABLED

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_MUST 1
#define LOG_LEVEL_NICE 2
#define LOG_LEVEL_ALL  3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_MUST
#endif

void debug_log(int level, const char* format, ...);

#define LOG_MUST(fmt, args...) debug_log(LOG_LEVEL_MUST, fmt, ##args)
#define LOG_NICE(fmt, args...) debug_log(LOG_LEVEL_NICE, fmt, ##args)
#define LOG_SPECIAL(fmt, args...) debug_log(LOG_LEVEL_ALL, fmt, ##args)

#else /* DEBUG_ENABLED */

#define LOG_MUST(fmt, args...)
#define LOG_NICE(fmt, args...)
#define LOG_SPECIAL(fmt, args...)

#endif /* DEBUG_ENABLED */

#endif /* _LOG_H_ */
