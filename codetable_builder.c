#include "codetable_builder.h"
#include "codeword.h"
#include "pqueue.h"
#include "utils.h"

#define CODETABLE_CAPACITY 256

CodeTable* codetable_builder_build(
    FrequencyDistribution* frequency_distribution
) 
{
    ABORT_ON(frequency_distribution == NULL);
    ABORT_ON(frequency_distribution_size(frequency_distribution) == 0);

    CodeTable* code_table = malloc(sizeof(CodeTable));
    ABORT_ON(code_table == NULL);
    codetable_init(code_table);

    PriorityQueue pq;
    priority_queue_init(&pq);

    for (size_t byte = 0; byte < CODETABLE_CAPACITY; ++byte) {
        size_t frequency = frequency_distribution_get_frequency(
                    frequency_distribution,
                    (uint8_t) byte);

        if (frequency != 0) {
            WeightedByteList* wbl = malloc(sizeof(WeightedByteList));
            ABORT_ON(wbl == NULL);

            weighted_bytelist_init(wbl);
            weighted_bytelist_append(wbl, (uint8_t) byte);
            wbl->total_list_weight = frequency;
            priority_queue_insert(&pq, wbl);

            Codeword* codeword = codeword_alloc(0);

            ABORT_ON(codeword == NULL);

            codetable_put(code_table, 
                          (uint8_t) byte, 
                          codeword);
        }
    }

    while (priority_queue_size(&pq) > 1) {
        WeightedByteList* left  = priority_queue_extract_min(&pq);
        WeightedByteList* right = priority_queue_extract_min(&pq);

        for (size_t i = 0; i < weighted_bytelist_size(left); ++i) {
            uint8_t byte = weighted_bytelist_get(left,
                                                 i);

            Codeword* codeword = codetable_get(code_table,
                                               (uint8_t) byte);

            ABORT_ON(codeword == NULL);
            codeword_prepend_bit(codeword, true);
        }

        for (size_t i = 0; i < weighted_bytelist_size(right); ++i) {
            uint8_t byte = weighted_bytelist_get(right,
                                                 i);

            Codeword* codeword = codetable_get(code_table,
                                               byte);

            ABORT_ON(codeword == NULL);
            codeword_prepend_bit(codeword, false);
        }

        weighted_bytelist_append_all(left, right);
        weighted_bytelist_free(right);
        priority_queue_insert(&pq, left);
    }

    WeightedByteList* final_list = priority_queue_extract_min(&pq);
    weighted_bytelist_free(final_list);
    priority_queue_free(&pq);
    return code_table;
}