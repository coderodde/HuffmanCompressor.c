#include "utils.h"
#include "weighted_byte_list..h"
#include <stdint.h>
#include <stdlib.h>

void weighted_bytelist_init(WeightedByteList* const list) {
    if (list == NULL) {
        errorf("weighted_bytelist_init: list is NULL.");
        abort();
    }

    list->data = malloc(sizeof(uint8_t) * LIST_CAPACITY);
    list->size = 0;
    list->total_list_weight;
}

void weighted_bytelist_append(WeightedByteList* const list,
                              const uint8_t byte) {
    if (list == NULL) {
        errorf("weighted_bytelist_append: list is NULL.");
        abort();
    }

    list->data[list->size++] = byte;
}

void weighted_bytelist_append_all(
    WeightedByteList* const target_list,
    const WeightedByteList* const source_list) {

    if (target_list == NULL) {
        errorf("weighted_bytelist_append_all: target_list is NULL.");
        abort();
    }

    if (source_list == NULL) {
        errorf("weighted_bytelist_append_all: source_list is NULL.");
        abort();
    }

    for (size_t i = 0; i != source_list->size; ++i) {
        target_list->data[target_list->size + i] = source_list->data[i];
    }

    target_list->size += source_list->size;
    target_list->total_list_weight += source_list->total_list_weight;
}

void weighted_bytelist_free(WeightedByteList* const list) {
    if (list == NULL) {
        errorf("weighted_bytelist_free: list is NULL.");
        abort();
    }

    free(list->data);
}

const uint8_t weighted_bytelist_get(const WeightedByteList* const list,
                                    const size_t index) {
    if (list == NULL) {
        errorf("weighted_bytelist_get: list is NULL.");
        abort();
    }

    if (index >= list->size) {
        errorf("weighted_bytelist_get: index(%zu) is too large. "
               "Bytelist size: (%zu)",
               index,
               list->size);
        abort();
    }

    return list->data[index];
}

const size_t weighted_bytelist_total_weight(
            const WeightedByteList* const list) {
    if (list == NULL) {
        errorf("weighted_bytelist_total_weight: list is NULL.");
        abort();
    }

    return list->total_list_weight;
}