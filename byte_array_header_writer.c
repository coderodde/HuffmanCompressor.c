#include "byte_array_header_writer.h"
#include "codetable.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const size_t BYTES_PER_CODEWORD_ENTRY = 6;
static const size_t BITS_PER_BYTE = 8;

void byte_array_header_writer_init(
    ByteArrayHeaderWriter* const writer,
    uint8_t* output_data,
    size_t output_data_length,
    size_t raw_data_length,
    CodeTable* table
) 
{
    ABORT_ON(writer == NULL)
    ABORT_ON(output_data == NULL)
    ABORT_ON(table == NULL)
    ABORT_ON(output_data_length == 0)
    ABORT_ON(raw_data_length == 0)

    *writer = (ByteArrayHeaderWriter)
    {
        .output_data          = output_data,
        .output_data_length   = output_data_length,
        .raw_data_length      = raw_data_length,
        .data_start_bit_index = 0,
        .table                = table,
    };

    // Clear away the junk data:
    memset(output_data,
           0,
           output_data_length);
}

size_t byte_array_header_writer_get_data_start_bit_index(
    const ByteArrayHeaderWriter* const writer
) 
{
    ABORT_ON(writer == NULL)
    ABORT_ON(writer->output_data == NULL)
    ABORT_ON(writer->table == NULL)
    return writer->data_start_bit_index;
}

static void byte_array_header_writer_write_code_size(
    ByteArrayHeaderWriter *const writer
) 
{
    const size_t code_table_size = codetable_size(writer->table);

    // Force little-endian encoding:
    for (size_t i = 0; i < sizeof(size_t); ++i) {
        writer->output_data[i] = (uint8_t) (code_table_size >> (8 * i));
    }
}

static void byte_array_header_writer_write_raw_data_length(
    ByteArrayHeaderWriter* const writer
) 
{
    const size_t raw_data_length = writer->raw_data_length;

    // Force little-endian encoding:
    for (size_t i = 0; i < sizeof(size_t); ++i) {
        writer->output_data[i + sizeof(size_t)] = 
            (uint8_t)(raw_data_length >> (8 * i));
    }
}

static void byte_array_header_writer_write_code_table(
    ByteArrayHeaderWriter* const writer
) 
{ 
    // Skip code size and raw data length:
    size_t current_byte_index = 2 * sizeof(size_t);

    for (size_t byte = 0; byte < CODE_TABLE_CAPACITY; ++byte) {
        const Codeword *const codeword = codetable_get(writer->table, (uint8_t) byte);
        
        if (codeword != NULL) {
            writer->output_data[current_byte_index++] = (uint8_t) byte;
            writer->output_data[current_byte_index++] = (uint8_t) 
                                                        (codeword->length);
            
            const size_t num_codeword_bytes = codeword_number_of_bytes(codeword);
            uint8_t* const codeword_bytes   = codeword_get_bytes(codeword);

            memcpy(&writer->output_data[current_byte_index],
                   codeword_bytes, 
                   num_codeword_bytes);

            free(codeword_bytes);

            current_byte_index += sizeof(uint32_t);
        }
    }
    
    writer->data_start_bit_index = current_byte_index * BITS_PER_BYTE;
}

void byte_array_header_writer_perform_write(
    ByteArrayHeaderWriter* const writer
) 
{
    ABORT_ON(writer == NULL)
    ABORT_ON(writer->output_data == NULL)
    ABORT_ON(writer->table == NULL)
   
    byte_array_header_writer_write_code_size      (writer);
    byte_array_header_writer_write_raw_data_length(writer);
    byte_array_header_writer_write_code_table     (writer);
}

size_t byte_array_header_writer_get_header_length(
    const size_t code_table_size
)
{
    return 2 * sizeof(size_t) + code_table_size * BYTES_PER_CODEWORD_ENTRY; 
}