#include "codeword.h"
#include "utils.h"
#include <stdlib.h>

static const size_t BITS_IN_BYTE = 8;
static const uint32_t LSBYTE_MASK = 0xFF;

static void check_index(const Codeword* const codeword, const size_t index) {
    if (index >= codeword->length) {
        errorf("codeword_get_bit: index %zu out of bounds for codeword of length %zu.",
               index, 
               codeword->length);

        abort();
    }
}

Codeword* codeword_alloc(const size_t length) {
    Codeword* pcodeword = malloc(sizeof(Codeword));
    
    if (pcodeword == NULL) {
        return NULL;
    }

    pcodeword->length = length;
    pcodeword->bits = 0;
    return pcodeword;
}

Codeword* codeword_reverse(const Codeword *const pcodeword) {
    Codeword* p_reversed_codeword = malloc(sizeof(Codeword));

    if (p_reversed_codeword == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < pcodeword->length; ++i) {
        if (codeword_get_bit(pcodeword, i)) {
            codeword_set_bit(p_reversed_codeword, 
                             pcodeword->length - 1 - i);
        }
    }

    return p_reversed_codeword;
}

size_t codeword_number_of_bytes(const Codeword* const codeword) {
    if (codeword == NULL) {
        errorf("codeword_number_of_bytes: codeword is NULL.");
        abort();
    }

    return (codeword->length + 7) / BITS_IN_BYTE; // BITS_IN_BYTE = 8
}

uint8_t* codeword_get_bytes(const Codeword* const codeword) {
    if (codeword == NULL) {
        errorf("codeword_get_bytes: codeword is NULL.");
        return NULL;
    }

    const size_t num_bytes = codeword_number_of_bytes(codeword);
    uint8_t* const bytes = malloc(num_bytes);
    uint32_t bits = codeword->bits;

    if (bytes == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < num_bytes; ++i) {
        bytes[i] = (uint8_t)(bits & LSBYTE_MASK);
        bits >>= BITS_IN_BYTE;
    }

    return bytes;
}

bool codeword_prepend_bit(Codeword* const codeword, const bool bit) {
    if (codeword == NULL) {
        return false;
    }

    codeword->length++;

    if (bit) {
        codeword_set_bit(codeword, codeword->length - 1);
    }

    return true;
}

size_t codeword_length(const Codeword* const codeword) {
    if (codeword == NULL) {
        errorf("codeword_length: codeword is NULL.");
        abort();
    }

    return codeword->length;
}

bool codeword_get_bit(const Codeword* const codeword, const size_t index) {
    if (codeword == NULL) {
        errorf("codeword_get_bit: codeword is NULL.");
        abort();
    }

    check_index(codeword, index);

    const uint32_t mask = 1 << index;
    return (codeword->bits & mask) != 0;
}

void codeword_set_bit(Codeword* const codeword, const size_t index) {
    if (codeword == NULL) {
        errorf("codeword_set_bit: codeword is NULL.");
        abort();
    }

    check_index(codeword, index);

    const uint32_t mask = 1 << index;
    codeword->bits |= mask;
}

bool codeword_equals(const Codeword* const a, const Codeword* const b) {
    if (a == NULL || b == NULL) {
        errorf("codeword_equals: a = %s, b = %s",
               (a == NULL ? "NULL" : "non-NULL"), 
               (b == NULL ? "NULL" : "non-NULL"));

        abort();
    }

    if (a->length != b->length) {
        return false;
    }

    for (size_t i = 0; i < a->length; ++i) {
        const bool b1 = codeword_get_bit(a, i);
        const bool b2 = codeword_get_bit(b, i);

        if (b1 != b2) {
            return false;
        }
    }

    return true;
}