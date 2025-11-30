#include "byte_array_header_writer.h"
#include "codetable.h"
#include "codetable_builder.h"
#include "compressor.h"
#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (1024 * 64)

void compress(
    const char* const file_name
)
{
    const FrequencyDistribution *const fd = 
        frequency_distribution_builder_build(file_name);

    ABORT_ON(fd == NULL)

    CodeTable* code_table = codetable_builder_build(fd);

    ABORT_ON(code_table == NULL)

    const size_t code_table_size = codetable_size(code_table);
    const size_t header_length =
        byte_array_header_writer_get_header_length(code_table_size);

    FILE *const file = fopen(file_name, "wb");

    ABORT_ON(file == NULL)

    uint8_t* buffer = malloc(BUFFER_SIZE);
    uint8_t* header_byte_array = malloc(header_length);

    ABORT_ON(buffer == NULL)
    ABORT_ON(header_byte_array == NULL)
    ABORT_ON(setvbuf(file, (char*)buffer, _IOFBF, BUFFER_SIZE) != 0)

    ByteArrayHeaderWriter *const header_writer = malloc(sizeof *header_writer);

    ABORT_ON(header_writer == NULL)

    const size_t raw_data_length = get_file_length(file);

    byte_array_header_writer_init(
        header_writer, 
        header_byte_array,
        header_length, 
        raw_data_length, 
        code_table
    );

                                  
}