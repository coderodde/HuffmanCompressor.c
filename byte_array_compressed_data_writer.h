#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_COMPRESSED_DATA_WRITER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_COMPRESSED_DATA_WRITER_H

#include "codetable.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct ByteArrayCompressedDataWriter {
    uint8_t* compressed_data;
    uint8_t* input_data;
    size_t compressed_data_length;
    size_t input_data_length;
    size_t starting_bit_index;
    CodeTable* code_table;
}
ByteArrayCompressedDataWriter;

void byte_array_compressed_data_writer_init(
    ByteArrayCompressedDataWriter *const writer,
    uint8_t* compressed_data,
    uint8_t* raw_data,
    size_t compressed_data_length,
    size_t raw_data_length,
    size_t starting_bit_index,
    const CodeTable *const code_table
);

void byte_array_compressed_data_writer_write(
    ByteArrayCompressedDataWriter *const writer
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_COMPRESSED_DATA_WRITER_H