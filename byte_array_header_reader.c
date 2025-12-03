#include "codetable.h"
#include "byte_array_header_reader.h"
#include "utils.h"
#include <stdlib.h>

#define BYTES_PER_CODEWORD_ENTRY 6

static size_t read_raw_data_length(
    uint8_t* compressed_data
);

static size_t read_code_table_size(
    uint8_t* compressed_data
);

static CodeTable* read_code_table(
    size_t code_table_size,
    uint8_t* compressed_data,
    size_t compressed_data_length
);

void byte_array_header_reader_init(
    ByteArrayHeaderReader* reader,
    uint8_t* compressed_data,
    size_t compressed_data_length
)
{
    ABORT_ON(reader == NULL)
    ABORT_ON(compressed_data == NULL)

    CodeTable* ct = malloc(sizeof *ct);
    ABORT_ON(ct == NULL)
    codetable_init(ct);

    reader->compressed_data        = compressed_data;
    reader->compressed_data_length = compressed_data_length;
    reader->code_table             = ct;

    reader->raw_data_length = read_raw_data_length(compressed_data);
    size_t code_table_size  = read_code_table_size(compressed_data);
    reader->code_table      = read_code_table(
                                         code_table_size,
                                         compressed_data,
                                         compressed_data_length);
}

CodeTable* byte_array_header_reader_get_code_table(
    ByteArrayHeaderReader* reader
)
{
    return reader->code_table;
}

const size_t byte_array_header_reader_get_raw_data_length(
    ByteArrayHeaderReader* reader
)
{
    return reader->raw_data_length;
}

static size_t read_raw_data_length(
    uint8_t* compressed_data
) 
{
    size_t raw_data_length = 0;

    for (size_t i = 0; i < sizeof(size_t); ++i) {
        raw_data_length |= ((size_t) compressed_data[sizeof(size_t) + i]) << (8 * i);
    }

    return raw_data_length;
}

static size_t read_code_table_size(
    uint8_t* compressed_data
)
{
    size_t code_table_size = 0;

    for (size_t i = 0; i < sizeof(size_t); ++i) {
        code_table_size |= ((size_t) compressed_data[i]) << (8 * i);
    }

    return code_table_size;
}

static Codeword* codeword_deserialize(
    uint8_t* data
)
{
    ABORT_ON(data == NULL)

    size_t length = 0;
    uint32_t bits = 0;

    length = data[1];

    bits |= (((uint8_t) data[5]) << 24);
    bits |= (((uint8_t) data[4]) << 16);
    bits |= (((uint8_t) data[3]) << 8 );
    bits |= (((uint8_t) data[2]) << 0 );

    Codeword* codeword = malloc(sizeof *codeword);
    ABORT_ON(codeword == NULL)

    *codeword = (Codeword) {
        .length = length,
        .bits   = bits
    };

    return codeword;
}

static CodeTable* read_code_table(
    size_t code_table_size,
    uint8_t* compressed_data,
    size_t compressed_data_length
) 
{
    CodeTable* table = malloc(sizeof * table);
    ABORT_ON(table == NULL)
    codetable_init(table);
    size_t offset = 2 * sizeof(size_t);

    for (size_t i = 0; i < code_table_size; ++i) {
        ABORT_ON(offset + BYTES_PER_CODEWORD_ENTRY > compressed_data_length)

        Codeword* codeword = codeword_deserialize(
            &compressed_data[offset]
        );

        ABORT_ON(codeword == NULL)

        codetable_put(table, (uint8_t)i, codeword);
        offset += BYTES_PER_CODEWORD_ENTRY;
    }

    return table;
}

const size_t byte_array_header_reader_get_header_length(
    ByteArrayHeaderReader* reader
)
{
    return 2 * sizeof(size_t) + 
               codetable_size(reader->code_table) * 
                    BYTES_PER_CODEWORD_ENTRY;
}