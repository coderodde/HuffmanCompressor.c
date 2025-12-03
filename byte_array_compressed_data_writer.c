#include "byte_array_compressed_data_writer.h"
#include "codeword.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

#define BITS_PER_BYTE 8

void byte_array_compressed_data_writer_init(
    ByteArrayCompressedDataWriter* writer,
    uint8_t* compressed_data,
    uint8_t* input_data,
    size_t compressed_data_length,
    size_t input_data_length,
    size_t starting_bit_index,
    CodeTable* code_table
)
{
    ABORT_ON(writer == NULL)
    ABORT_ON(compressed_data == NULL)
    ABORT_ON(input_data == NULL)
    ABORT_ON(code_table == NULL);
    ABORT_ON(compressed_data_length == 0)
    ABORT_ON(input_data_length == 0)

    *writer = (ByteArrayCompressedDataWriter) {
        .compressed_data        = compressed_data,
        .input_data             = input_data,
        .compressed_data_length = compressed_data_length,
        .input_data_length      = input_data_length,
        .starting_bit_index     = starting_bit_index,
        .code_table             = code_table
    };
}

static void write_codeword(
    uint8_t* data,
    size_t current_bit_index,
    Codeword* codeword
);

void byte_array_compressed_data_writer_write(
    ByteArrayCompressedDataWriter* const writer
)
{
    size_t current_bit_index = writer->starting_bit_index;

    for (size_t i = 0; i < writer->input_data_length; ++i) {
        uint8_t byte = writer->input_data[i];
        Codeword* codeword = codetable_get(writer->code_table, byte);
        codeword = codeword_reverse(codeword);
        size_t codeword_len = codeword_length(codeword);

        write_codeword(writer->compressed_data,
                       current_bit_index,
                       codeword);

        current_bit_index += codeword_len;
    }
}

static void set_bit(
    uint8_t* data,
    size_t byte_index,
    size_t bit_index
);

static void write_codeword(
    uint8_t* data,
    size_t current_bit_index,
    Codeword* codeword
)
{
    size_t byte_index = current_bit_index / BITS_PER_BYTE;
    size_t bit_index  = current_bit_index % BITS_PER_BYTE;
    size_t codeword_len = codeword_length(codeword);

    for (size_t i = 0; i < codeword_len; ++i) {

        if (codeword_get_bit(codeword, i)) {
            set_bit(data,
                    byte_index,
                    bit_index);
        }

        ++bit_index;

        if (bit_index == BITS_PER_BYTE) {
            bit_index = 0;
            byte_index++;
        }
    }
}

static void set_bit(
    uint8_t* data,
    size_t byte_index,
    size_t bit_index
)
{
    uint8_t mask = 1 << bit_index;
    data[byte_index] |= mask;
}