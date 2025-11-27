#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_WRITER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_WRITER_H

#include "codetable.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct ByteArrayHeaderWriter {
    uint8_t*   output_data;
    size_t     output_data_length;
    size_t     raw_data_length;
    size_t     data_start_bit_index;
    CodeTable* table;
} 
ByteArrayHeaderWriter;

void byte_array_header_writer_init(
    uint8_t* output_data,
    size_t output_data_length,
    size_t raw_data_length,
    CodeTable* table
);

size_t byte_array_header_writer_get_data_start_bit_index(
    const ByteArrayHeaderWriter *const writer
);

void byte_array_header_writer_perform_write(
    ByteArrayHeaderWriter *const writer
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_WRITER_H