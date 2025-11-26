#define _CRT_SECURE_NO_WARNINGS // Ignore _s functions

#include "codetable.h"
#include "codeword.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 5000
#define CODETABLE_CAPACITY 256

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

Codeword* const codetable_get(const CodeTable* const table,
                              const uint8_t byte) {
    ABORT_ON("codetable_get", table == NULL);

    return table->table[byte];
}

size_t codetable_size(const CodeTable* const table) {
    ABORT_ON("codetable_size", table == NULL);
    return table->size;
}

#define BYTE_HEX_STRING_LEN 4
#define BYTE_CODEWORD_SEPARATOR " = "
#define SPACE_CHARS ", "

// Converts a byte value to its hexadecimal represesntation.
// For example, the byte 0x42 will be converted to "0x42" 
// and 0x5 will be converted to "0x05".
static char *const convert_byte_to_string(const uint8_t byte) {
    char *const str = malloc(BYTE_HEX_STRING_LEN + 1);
    ABORT_ON("convert_byte_to_string", str == NULL);
    snprintf(str, BYTE_HEX_STRING_LEN + 1, "0x%02X", byte);
    return str;
}

char* codetable_to_string(const CodeTable* const table) {
    ABORT_ON("codetable_to_string", table == NULL);
    ABORT_ON("codetable_to_string", codetable_size(table) == 0);
    char* str = malloc(sizeof(char) * STRING_LENGTH);
    ABORT_ON("codetable_to_string", str == NULL);

    str[0] = '{';
    size_t index = 1;
    
    for (size_t byte = 0; byte < CODETABLE_CAPACITY; ++byte) {
        const Codeword *const codeword = codetable_get(table, (uint8_t) byte);

        if (codeword == NULL) {
            // 'byte' is not stored in the argument code table:
            continue;
        }

        // Print the byte value:
        char* const byte_str = convert_byte_to_string((uint8_t) byte);
        strncpy(str + index, byte_str, BYTE_HEX_STRING_LEN);
        free(byte_str);
        index += BYTE_HEX_STRING_LEN;

        // Print the key/value separator:
        strcpy(str + index, BYTE_CODEWORD_SEPARATOR);
        index += strlen(BYTE_CODEWORD_SEPARATOR);

        // Print the codeword:
        char* codeword_str = codeword_to_string(codeword);
        const size_t codeword_str_len = strlen(codeword_str);
        strcpy(str + index, codeword_str);
        index += codeword_str_len;
        free(codeword_str);

        // Print a comma and a space:
        strcpy(str + index, SPACE_CHARS);
        index += strlen(SPACE_CHARS);
    }

    // Finish the string with an '}' and zero-terminate:
    str[index - 2] = '}';
    str[index - 1] = '\0';
    return str;
}