#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H

#define ABORT_ON(func_name, expr)      \
    if (expr) {                        \
        errorf(#func_name ": " #expr); \
        abort();                       \
    }                                  

void errorf(const char* fmt, ...);
void infof (const char* fmt, ...);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_UTILS_H