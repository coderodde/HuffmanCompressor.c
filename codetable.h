#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H

#include "codeword.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

#define CODE_TABLE_CAPACITY 256

typedef struct CodeTable {
    Codeword* table[CODE_TABLE_CAPACITY];
    size_t size;
}
CodeTable;

void codetable_init(
    CodeTable* table
);

void codetable_put(
    CodeTable* table,
    uint8_t byte,
    Codeword* codeword
);

Codeword* codetable_get(
    CodeTable* table,
    uint8_t byte
);

size_t codetable_size(
    CodeTable* table
);

char* codetable_to_string(
    CodeTable* table
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_H