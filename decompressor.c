#include "byte_array_header_reader.h"
#include "codetable.h"
#include "decompressor.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE (1024 * 64)
#define HEADER_MAX_SIZE (16 + 256 * 6)

void decompress(
    const char* const input_file_name,
    const char* const output_file_name
)
{
    FILE* const in = fopen(input_file_name, "rb");
    ABORT_ON(in == NULL)

    uint8_t* header_buffer = malloc(HEADER_MAX_SIZE);
    ABORT_ON(header_buffer == NULL)

    const size_t header_bytes_read =
        fread(header_buffer, 1, HEADER_MAX_SIZE, in);

    ByteArrayHeaderReader reader;
    byte_array_header_reader_init(&reader, 
                                  header_buffer, 
                                  HEADER_MAX_SIZE);

    const CodeTable *const ct = byte_array_header_reader_get_code_table(&reader);

    ABORT_ON(ct == NULL)
    
    puts(codetable_to_string(ct));

    const size_t code_table_size = codetable_size(ct);
    const size_t expected_header_length =
        byte_array_header_writer_get_header_length(code_table_size);

    ABORT_ON(expected_header_length != header_bytes_read)

    const size_t raw_data_length = 
        byte_array_header_reader_get_raw_data_length(&reader);

    fclose(in);
    free(header_buffer);
    free(ct);
}