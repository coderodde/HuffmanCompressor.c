#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char* extract_file_name_only(char* path_name);

#define ABORT_ON(expr)                                     \
    if (expr) {                                            \
        errorf("%s in function '%s', line %zu, file '%s'", \
               #expr,                                      \
               __FUNCTION__,                               \
               __LINE__,                                   \
               extract_file_name_only(__FILE__));          \
        exit(EXIT_FAILURE);                                \
    }                                  

void errorf(
    char* fmt, 
    ...
);

void infof(
    char* fmt, 
    ...
);

size_t get_ms();

size_t get_file_length_by_name(char* filename);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H