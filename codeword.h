#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODEWORD_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODEWORD_H

#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Codeword {
    size_t length;
    uint32_t bits;
} 
Codeword;

Codeword* codeword_alloc(
    size_t length
);

Codeword* codeword_reverse(
    Codeword* codeword
);

size_t codeword_number_of_bytes(
    size_t bit_length
);

uint8_t* codeword_get_bytes(
    Codeword* codeword
);

bool codeword_prepend_bit(
    Codeword* codeword, 
    bool bit
);

size_t codeword_length(
    Codeword* codeword
);

bool codeword_get_bit(
    Codeword* codeword, 
    size_t index
);

void codeword_set_bit(
    Codeword* codeword, 
    size_t index
);

bool codeword_equals(
    Codeword* a,
    Codeword* b
);

char* codeword_to_string(
    Codeword* codeword
);

#endif 