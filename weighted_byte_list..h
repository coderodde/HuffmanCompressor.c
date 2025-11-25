#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H

#include <stdint.h>
#include <stdlib.h>

#define LIST_CAPACITY 256

typedef struct WeightedByteList {
    uint8_t* data;
    size_t size;
    size_t total_list_weight;
} WeightedByteList;

void          weighted_bytelist_init(WeightedByteList* const list);
void          weighted_bytelist_append(WeightedByteList* const list,
                                       const uint8_t byte);

void          weighted_bytelist_append_all(
                        WeightedByteList* const target_list,
                        const WeightedByteList* const source_list);

void          weighted_bytelist_free(WeightedByteList* const list);
const uint8_t weighted_bytelist_get(const WeightedByteList* const list, 
                                    const size_t index);

const size_t  weighted_bytelist_total_weight(const WeightedByteList* const list);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H