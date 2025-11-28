#include "codetable.h"
#include "byte_array_header_reader.h"
#include "utils.h"
#include <stdlib.h>

static size_t read_raw_data_length(const uint8_t *const compressed_data);
static size_t read_code_table_size(const uint8_t *const compressed_data);
static CodeTable* read_code_table(
    const size_t code_table_size,
    const uint8_t *const compressed_data,
    const size_t compressed_data_length
);

void byte_array_header_reader_init(
    ByteArrayHeaderReader* reader,
    uint8_t* const compressed_data,
    const size_t compressed_data_length
)
{
    ABORT_ON(reader == NULL)
    ABORT_ON(compressed_data == NULL)

    CodeTable *const ct = malloc(sizeof *ct);
    codetable_init(ct);

    reader->compressed_data = compressed_data;
    reader->compressed_data_length = compressed_data_length;
    reader->code_table = ct;

    reader->raw_data_length = read_raw_data_length(compressed_data);
    const size_t code_table_size = read_code_table_size(compressed_data);
    reader->code_table = read_code_table(code_table_size,
                                         compressed_data,
                                         compressed_data_length);
}

const CodeTable* const byte_array_header_reader_get_code_table(
    const ByteArrayHeaderReader* const reader
)
{
    return NULL;
}

const size_t byte_array_header_reader_get_raw_data_length(
    const ByteArrayHeaderReader* const reader
)
{
    return 0;
}

static size_t read_raw_data_length(const uint8_t* const compressed_data) {
    return 0;
}

static size_t read_code_table_size(const uint8_t* const compressed_data) {
    return 0;
}

static CodeTable* read_code_table(
    const size_t code_table_size,
    const uint8_t* const compressed_data,
    const size_t compressed_data_length
) {
    return NULL;
}