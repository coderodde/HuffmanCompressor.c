#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_READER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_READER_H

#include "codetable.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct ByteArrayHeaderReader {
    uint8_t*   compressed_data;
    size_t     compressed_data_length;
    size_t     raw_data_length;
    size_t     header_length;
    CodeTable* code_table;
} 
ByteArrayHeaderReader;

void byte_array_header_reader_init(
    ByteArrayHeaderReader* reader,
    uint8_t* compressed_data,
    size_t compressed_data_length
);

size_t byte_array_header_reader_get_header_length(
    ByteArrayHeaderReader* reader
);

CodeTable* byte_array_header_reader_get_code_table(
    ByteArrayHeaderReader* reader
);

size_t byte_array_header_reader_get_raw_data_length(
    ByteArrayHeaderReader* reader
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_BYTE_ARRAY_HEADER_READER_H