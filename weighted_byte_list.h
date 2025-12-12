#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H

#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

#define LIST_CAPACITY 256

typedef struct WeightedByteList {
    uint8_t* data;
    size_t size;
    size_t total_list_weight;
} 
WeightedByteList;

void weighted_bytelist_init(
    WeightedByteList* list
);

void weighted_bytelist_append(
    WeightedByteList* list,
    uint8_t byte
);

size_t weighted_bytelist_size(
    WeightedByteList* list
);

void weighted_bytelist_free(
    WeightedByteList* list
);

uint8_t weighted_bytelist_get(
    WeightedByteList* list, 
    size_t index
);

size_t weighted_bytelist_total_weight(
    WeightedByteList* list
);

void weighted_bytelist_merge(
    WeightedByteList* dest,
    WeightedByteList* src
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_WEIGHTED_BYTE_LIST_H