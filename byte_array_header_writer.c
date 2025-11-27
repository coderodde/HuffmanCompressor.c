#include "byte_array_header_writer.h"
#include "codetable.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

void byte_array_header_writer_init(
    uint8_t* output_data,
    size_t output_data_length,
    size_t raw_data_length,
    CodeTable* table
) {

}

size_t byte_array_header_writer_get_data_start_bit_index(
    const ByteArrayHeaderWriter* const writer
) {
    return 1;
}

void byte_array_header_writer_perform_write(
    ByteArrayHeaderWriter* const writer
) {

}