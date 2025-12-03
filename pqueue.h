#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H

#include "weighted_byte_list..h"
#include <stdlib.h>

typedef struct PriorityQueue {
    WeightedByteList** data;
    size_t size;
} 
PriorityQueue;

void priority_queue_init(
    PriorityQueue* pq
);

void priority_queue_insert(
    PriorityQueue* pq, 
    WeightedByteList* wbl
);

WeightedByteList* priority_queue_extract_min(
    PriorityQueue* pq
);

size_t priority_queue_size(
    PriorityQueue* pq
);

void priority_queue_free(
    PriorityQueue* pq
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H