#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errorf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);

    // Add newline if missing:
    size_t len = strlen(fmt);

    if (len == 0 || fmt[len - 1] != '\n') {
        fputc('\n', stderr);
    }

    va_end(args);
}

void infof(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stdout, "[INFO] ");
    vfprintf(stdout, fmt, args);

    // Add newline if missing:
    size_t len = strlen(fmt);

    if (len == 0 || fmt[len - 1] != '\n') {
        fputc('\n', stdout);
    }

    va_end(args);
}

size_t get_ms() {
#ifdef _WIN32
    return (size_t) GetTickCount64();
#else
    return 0;
#endif
}

size_t get_number_of_cpus() {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (size_t) si.dwNumberOfProcessors;
#else
    const long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    return nprocs > 0 ? (size_t) nprocs : 1;
#endif
}