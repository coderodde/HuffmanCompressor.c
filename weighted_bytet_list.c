#include "utils.h"
#include "weighted_byte_list..h"
#include <stdint.h>
#include <stdlib.h>

void weighted_bytelist_init(WeightedByteList* const list) {
    ABORT_ON("weighted_bytelist_init", list == NULL);

    *list = (WeightedByteList){ 
        .data = malloc(sizeof(uint8_t) * LIST_CAPACITY),
        .size = 0,
        .total_list_weight = 0,
    };
}

void weighted_bytelist_append(WeightedByteList* const list,
                              const uint8_t byte) {
    ABORT_ON("weighted_bytelist_append", list == NULL);
    list->data[list->size++] = byte;
}

void weighted_bytelist_append_all(
    WeightedByteList* const target_list,
    const WeightedByteList* const source_list) {

    ABORT_ON("weighted_bytelist_append_all", target_list == NULL);
    ABORT_ON("weighted_bytelist_append_all", source_list == NULL);

    for (size_t i = 0; i != source_list->size; ++i) {
        target_list->data[target_list->size + i] = source_list->data[i];
    }

    target_list->size += source_list->size;
    target_list->total_list_weight += source_list->total_list_weight;
}

void weighted_bytelist_free(WeightedByteList* const list) {
    ABORT_ON("weighted_bytelist_free", list == NULL);
    ABORT_ON("weighted_bytelist_free", list->data == NULL);

    free(list->data);
}

const uint8_t weighted_bytelist_get(const WeightedByteList* const list,
                                    const size_t index) {

    ABORT_ON("weighted_bytelist_get", list == NULL);

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
    ABORT_ON("weighted_bytelist_total_weight", list == NULL);
    return list->total_list_weight;
}