#include "pqueue.h"
#include "utils.h"
#include "weighted_byte_list.h"
#include <stdlib.h>

#define PRIORITY_QUEUE_TABLE_CAPACITY 256

static size_t get_weight(
    WeightedByteList* wbl
)
{
    return weighted_bytelist_total_weight(wbl);
}

static void swap(
    WeightedByteList** a,
    WeightedByteList** b
)
{
    WeightedByteList* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(
    WeightedByteList** data, 
    size_t index
)
{
    while (index > 0) {
        size_t parent_index = index / 2;

        if (get_weight(data[index]) >= get_weight(data[parent_index])) {
            break;
        }

        swap(&data[index], 
             &data[parent_index]);

        index = parent_index;
    }
}

static void heap_sift_down(
    WeightedByteList** data,
    size_t size, 
    size_t index
)
{
    for (;;) {
        size_t left_index     = 2 * index + 1;
        size_t right_index    = 2 * index + 2;
        size_t smallest_index = index;

        if (left_index < size && 
            get_weight(data[left_index]) < 
            get_weight(data[smallest_index])) {
           
            smallest_index = left_index;
        }

        if (right_index < size && 
            get_weight(data[right_index]) <
            get_weight(data[smallest_index])) {
            
            smallest_index = right_index;
        }

        if (smallest_index == index) {
            break;
        }

        swap(&data[index], &data[smallest_index]);
        index = smallest_index;
    }
}

void priority_queue_init(
    PriorityQueue* pq
)
{
    ABORT_ON(pq == NULL);

    *pq = (PriorityQueue) {
        .data = malloc(sizeof(WeightedByteList*) * PRIORITY_QUEUE_TABLE_CAPACITY),
        .size = 0,
    };
}

void priority_queue_insert(
    PriorityQueue* pq, 
    WeightedByteList* wbl
)
{
    ABORT_ON(pq == NULL);
    ABORT_ON(pq->data == NULL);
    ABORT_ON(wbl == NULL);
    ABORT_ON(wbl->data == NULL);

    const size_t index = pq->size;

    pq->data[index] = wbl;
    pq->size++;
    heap_sift_up(pq->data, index);
}

WeightedByteList* priority_queue_extract_min(
    PriorityQueue* pq
)
{
    ABORT_ON(pq == NULL);
    ABORT_ON(pq->data == NULL);

    WeightedByteList* minimum_element = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heap_sift_down(pq->data, pq->size, 0);
    return minimum_element;
}

size_t priority_queue_size(
    PriorityQueue* pq
)
{
    ABORT_ON(pq == NULL);
    return pq->size;
}

void priority_queue_free(
    PriorityQueue* pq
)
{
    ABORT_ON(pq == NULL);
    ABORT_ON(pq->data == NULL);
    free(pq->data);
}