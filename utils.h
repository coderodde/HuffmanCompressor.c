#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H

#include <stdlib.h>

#define ABORT_ON(expr)                           \
    if (expr) {                                  \
        errorf("%s in function '%s', file '%s'", \
               #expr,                            \
               __FUNCTION__,                     \
               __FILE__);                        \
        exit(EXIT_FAILURE);                      \
    }                                  

void errorf(const char* fmt, ...);
void infof (const char* fmt, ...);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H