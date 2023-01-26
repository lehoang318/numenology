#include "util.hpp"

#include <stdarg.h>
#include <stdio.h>

void logi(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
}

void loge(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vfprintf(stderr, format, args);

    va_end(args);
}


void logd(const char *format, ...) {
    va_list args;
    va_start(args, format);
#ifdef DEBUG
    vprintf(format, args);
#endif  // DEBUG
    va_end(args);
}
