#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODEWORD_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODEWORD_H

#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Codeword {
    size_t length;
    uint32_t bits;
} Codeword;

Codeword* codeword_alloc(const size_t length);
Codeword* codeword_reverse(const Codeword *const codeword);
size_t    codeword_number_of_bytes(const Codeword *const codeword);
uint8_t*  codeword_get_bytes(const Codeword *const codeword);
bool      codeword_prepend_bit(Codeword* const codeword, const bool bit);
size_t    codeword_length(const Codeword* const codeword);
bool      codeword_get_bit(const Codeword* const codeword, const size_t index);
void      codeword_set_bit(Codeword* const codeword, const size_t index);
bool      codeword_equals(const Codeword* const a, const Codeword* const b);

#endif 