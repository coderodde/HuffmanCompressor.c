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

size_t get_file_length_by_name(const char *const filename) {
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
    return (size_t) GetTickCount64();
#else
    return 0;
#endif
}

const char* const extract_file_name_only(const char* const path_name) {
    const size_t len = strlen(path_name);
    SSIZE_T i;

    for (i = len - 1; i >= 0; --i) {
        const char ch = path_name[i];

        if (ch == '\\' || ch == '/') {
            return &path_name[i + 1];
        }
    }

    ABORT_ON(i < 0)
    return NULL; // Make the compiler happy by returning anything.
                 // This is an unreachable statement.
}