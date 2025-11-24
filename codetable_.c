#include "codetable.h"
#include "utils.h"
#include <stdlib.h>

void codetable_init(CodeTable *const code_table) {
    if (code_table == NULL) {
        errorf("codetable_init: code_table is NULL.");
        abort();
    }

    code_table->size = 0;

    for (size_t i = 0; i < CODE_TABLE_CAPACITY; ++i) {
        code_table->table[i] = NULL;
    }
}

void codetable_put(CodeTable* table,
                   const uint8_t byte,
                   Codeword* const codeword) {
    if (table == NULL) {
        errorf("codetable_put: table == NULL.");
        abort();
    }

    if (codeword == NULL) {
        errorf("codetable_put: codeword == NULL.");
        abort();
    }

    table->table[byte] = codeword;
    table->size++;
}

const Codeword* const codetable_get(const CodeTable* const table,
                                    const uint8_t byte) {
    if (table == NULL) {
        errorf("codetable_get: table == NULL.");
        abort();
    }

    return table->table[byte];
}

size_t codetable_size(const CodeTable* const table) {
    if (table == NULL) {
        errorf("codetable_size: table == NULL.");
        abort();
    }

    return table->size;
}