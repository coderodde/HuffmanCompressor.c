#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H

#include <stdint.h>
#include <stdlib.h>

typedef struct ByteList {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t total_list_weight;
} ByteList;

void          weighted_bytelist_init(ByteList* const list);
void          weighted_bytelist_append(ByteList* const list,
                                       const uint8_t byte);

void          weighted_bytelist_free(ByteList* const list);
const uint8_t weighted_bytelist_get(const ByteList* const list, 
                                    const size_t index);

const size_t  weighted_bytelist_total_weight(const ByteList* const list);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H