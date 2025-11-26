#include "codetable.h"
#include "utils.h"
#include <stdlib.h>

void codetable_init(CodeTable *const code_table) {
    ABORT_ON("codetable_init", code_table == NULL);

    code_table->size = 0;

    for (size_t i = 0; i < CODE_TABLE_CAPACITY; ++i) {
        code_table->table[i] = NULL;
    }
}

void codetable_put(CodeTable* table,
                   const uint8_t byte,
                   Codeword* const codeword) {
    ABORT_ON("codetable_put", table == NULL);
    ABORT_ON("codetable_put", codeword == NULL);

    table->table[byte] = codeword;
    table->size++;
}

const Codeword* const codetable_get(const CodeTable* const table,
                                    const uint8_t byte) {
    ABORT_ON("codetable_get", table == NULL);

    return table->table[byte];
}

size_t codetable_size(const CodeTable* const table) {
    ABORT_ON("codetable_size", table == NULL);
    return table->size;
}