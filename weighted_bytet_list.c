#include "utils.h"
#include "weighted_byte_list.h"
#include <stdint.h>
#include <stdlib.h>

void weighted_bytelist_init(
    WeightedByteList* list
)
{
    ABORT_ON(list == NULL);

    *list = (WeightedByteList){ 
        .data = malloc(sizeof(uint8_t) * LIST_CAPACITY),
        .size = 0,
        .total_list_weight = 0,
    };
}

void weighted_bytelist_append(
    WeightedByteList* list,
    uint8_t byte
)
{
    ABORT_ON(list == NULL);
    list->data[list->size++] = byte;
}

void weighted_bytelist_merge(
    WeightedByteList* target_list,
    WeightedByteList* source_list
)
{
    ABORT_ON(target_list == NULL);
    ABORT_ON(source_list == NULL);

    for (size_t i = 0; i != source_list->size; ++i) {
        target_list->data[target_list->size + i] = source_list->data[i];
    }

    target_list->size += source_list->size;
    target_list->total_list_weight += source_list->total_list_weight;
}

void weighted_bytelist_free(
    WeightedByteList* list
)
{
    ABORT_ON(list == NULL);
    ABORT_ON(list->data == NULL);

    free(list->data);
}

uint8_t weighted_bytelist_get(
    WeightedByteList* list,
    size_t index
) 
{

    ABORT_ON(list == NULL);

    if (index >= list->size) {
        errorf("weighted_bytelist_get: index(%zu) is too large. "
               "Bytelist size: (%zu)",
               index,
               list->size);
        abort();
    }

    return list->data[index];
}

size_t weighted_bytelist_size(
    WeightedByteList* list
)
{
    ABORT_ON(list == NULL);
    return list->size;
}

size_t weighted_bytelist_total_weight(
    WeightedByteList* list
)
{
    ABORT_ON(list == NULL);
    return list->total_list_weight;
}