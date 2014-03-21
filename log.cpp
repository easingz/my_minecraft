#include "log.h"

#ifdef DEBUG_ENABLED
#include <stdio.h>
#include <stdarg.h>

void debug_log(int level, const char* format, ...) {
  if (level <= LOG_LEVEL) {
    va_list vl;
    char buf[1024];
    va_start(vl, format);
    vsnprintf(buf, 1024, format, vl);
    printf("%s", buf);
    fflush(stdout);
    va_end(vl);
  }
}
#endif 
