#include "pqueue.h"
#include "utils.h"
#include "weighted_byte_list..h"
#include <stdlib.h>

#define PRIORITY_QUEUE_TABLE_CAPACITY 256

static size_t get_weight(const WeightedByteList *const wbl) {
    return weighted_bytelist_total_weight(wbl);
}

static void swap(WeightedByteList** a, WeightedByteList** b) {
    WeightedByteList* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(WeightedByteList** data, size_t index) {
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
static void heap_sift_down(WeightedByteList** data,
                           size_t size, 
                           size_t index) {
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

void priority_queue_init(PriorityQueue* const pq) {
    if (pq == NULL) {
        errorf("priority_queue_init: pq is NULL.");
        abort();
    }

    pq->data = malloc(sizeof(WeightedByteList*) * PRIORITY_QUEUE_TABLE_CAPACITY);
    pq->size = 0;
}

void priority_queue_insert(PriorityQueue* const pq, 
                           WeightedByteList *const wbl) {
    if (pq == NULL) {
        errorf("priority_queue_insert: pq is NULL.");
        abort();
    }

    if (pq->data == NULL) {
        errorf("priority_queue_insert: pq->data is NULL.");
        abort();
    }

    if (wbl == NULL) {
        errorf("priority_queue_insert: wbl is NULL.");
        abort();
    }

    if (wbl->data == NULL) {
        errorf("priority_queue_insert: wbl->data is NULL.");
        abort();
    }

    const size_t index = pq->size;
    pq->data[index] = wbl;
    pq->size++;
    heap_sift_up(pq->data, index);
}

WeightedByteList* priority_queue_extract_min(PriorityQueue* const pq) {
    if (pq == NULL) {
        errorf("priority_queue_extract_min: pq is NULL.");
        abort();
    }

    if (pq->data == NULL) {
        errorf("priority_queue_extract_min: pq->data is NULL.");
        abort();
    }

    WeightedByteList* minimum_element = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heap_sift_down(pq->data, pq->size, 0);
    return minimum_element;
}

size_t priority_queue_size(const PriorityQueue* const pq) {
    if (pq == NULL) {
        errorf("priority_queue_size: pq is NULL.");
        abort();
    }

    return pq->size;
}

void priority_queue_free(PriorityQueue* const pq) {
    if (pq != NULL && pq->data != NULL) {
        free(pq->data);
    }
}