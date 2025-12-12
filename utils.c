#define _POSIX_C_SOURCE 200809L
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
typedef ptrdiff_t ssize_t;
#else
#include <sys/types.h>
#endif

void errorf(
    char* fmt, 
    ...
)
{
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

void infof(
    char* fmt, 
    ...
)
{
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

size_t get_file_length_by_name(
    char* filename
)
{
    FILE* f = fopen(filename, "rb");
    ABORT_ON(f == NULL)

#if defined(_WIN32)
    // Windows-safe: use 64-bit seek/tell
    if (_fseeki64(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }

    __int64 pos = _ftelli64(f);

    fclose(f);

    ABORT_ON(pos < 0)

    return (size_t)pos;

#else
#include <sys/types.h>
    // POSIX: fseeko / ftello (64-bit)
    if (fseeko(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }

    off_t pos = ftello(f);
    fclose(f);

    ABORT_ON(pos < 0)

    return (size_t)pos;
#endif
}

size_t get_ms() {
#ifdef _WIN32
#include <windows.h>
    return (size_t) GetTickCount64();
#else
#include <time.h>
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	
	return (size_t) ts.tv_sec * 1000 + 
                    ts.tv_nsec / 1000000;	
#endif
}

char* extract_file_name_only(
    char* path_name
)
{
    const size_t len = strlen(path_name);
    ssize_t i;

    for (i = len - 1; i >= 0; --i) {
        const char ch = path_name[i];

        if (ch == '\\' || ch == '/') {
            return &path_name[i + 1];
        }
    }

    return path_name; // No path separator found.
}
