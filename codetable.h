#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H

#include "codeword.h"
#include <stdint.h>
#include <stdlib.h>

#define CODE_TABLE_CAPACITY 256

typedef struct CodeTable {
    Codeword* table[CODE_TABLE_CAPACITY];
    size_t size;
} CodeTable;

void codetable_init(CodeTable *const table);

void codetable_put(CodeTable *const table,
                   const uint8_t byte,
                   Codeword *const codeword);

const Codeword *const codetable_get(const CodeTable *const table,
                                    const uint8_t byte);

size_t codetable_size(const CodeTable *const table);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H