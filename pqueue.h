#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H

#include "weighted_byte_list..h"
#include <stdlib.h>

typedef struct PriorityQueue {
    WeightedByteList** data;
    size_t size;
} PriorityQueue;

void priority_queue_init(PriorityQueue *const pq);
void priority_queue_insert(PriorityQueue *const pq, 
                           WeightedByteList *const wbl);

WeightedByteList* priority_queue_extract_min(PriorityQueue *const pq);
size_t priority_queue_size(const PriorityQueue *const pq);
void priority_queue_free(PriorityQueue *const pq);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_PRIORITY_QUEUE_H