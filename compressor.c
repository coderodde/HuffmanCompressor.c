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

typedef struct BitWriter {
    FILE*    file;
    uint8_t* buffer;
    size_t   byte_pos;  // next free byte in buffer
    uint8_t  bit_pos;   // next free bit position in current byte [0..7]
} 
BitWriter;

static void bit_writer_init(
    BitWriter* bw, 
    FILE*      file
)
{
    *bw = (BitWriter){
        .file = file,
        .byte_pos = 0,
        .bit_pos = 0,
        .buffer = malloc(BUFFER_SIZE),
    };
}

static void bit_writer_free(
    BitWriter* bw
)
{
    free(bw->buffer);
}

static void bit_writer_flush(
    BitWriter* bw
)
{
    if (bw->byte_pos == 0) {
        return;
    }

    size_t written = fwrite(bw->buffer, 1, bw->byte_pos, bw->file);
    ABORT_ON(written != bw->byte_pos);

    bw->byte_pos = 0;
    bw->bit_pos = 0;
}

// Write 'nbits' least significant bits of 'code' into the stream.
static void bit_writer_put_bits(
    BitWriter* bw, 
    uint32_t   code, 
    size_t     nbits
)
{
    while (nbits--) {
        // Take the next bit from MSB side or LSB side depending on your convention.
        uint8_t bit = (code >> nbits) & 1U;

        if (bw->bit_pos == 0) {
            // Start a new byte
            if (bw->byte_pos == BUFFER_SIZE) {
                bit_writer_flush(bw);
            }

            bw->buffer[bw->byte_pos] = 0;
        }

        bw->buffer[bw->byte_pos] |= (bit << (7 - bw->bit_pos));
        bw->bit_pos++;

        if (bw->bit_pos == 8) {
            bw->bit_pos = 0;
            bw->byte_pos++;
        }
    }
}

static void bit_writer_finish(
    BitWriter* bw
)
{
    if (bw->bit_pos != 0) {
        bw->byte_pos++;  // last partially filled byte is valid
    }

    bit_writer_flush(bw);
}

void compress(
    char* input_file_name,
    char* output_file_name
)
{
    // 1. Build frequency distribution from INPUT file:
    FrequencyDistribution* fd =
        frequency_distribution_builder_build(input_file_name);

    ABORT_ON(fd == NULL);

    // 2. Build code table:
    CodeTable* code_table = codetable_builder_build(fd);
    ABORT_ON(code_table == NULL);

    size_t code_table_size = codetable_size(code_table);
    size_t header_length =
        byte_array_header_writer_get_header_length(code_table_size);

    // 3. Open OUTPUT file:
    FILE* out = fopen(output_file_name, "wb");
    ABORT_ON(out == NULL);

    // 4. Prepare header
    uint8_t* header_byte_array = malloc(header_length);
    ABORT_ON(header_byte_array == NULL);

    const size_t raw_data_length = get_file_length_by_name(input_file_name);

    ByteArrayHeaderWriter* header_writer = malloc(sizeof * header_writer);
    ABORT_ON(header_writer == NULL);

    byte_array_header_writer_init(
        header_writer,
        header_byte_array,
        header_length,
        raw_data_length,
        code_table
    );

    byte_array_header_writer_perform_write(header_writer);

    // 5. Write header in one or more chunks (here: single fwrite):
    size_t written = fwrite(header_byte_array, 1, header_length, out);
    ABORT_ON(written != header_length);

    // 6. Now write the actual compressed codes, 64KiB at a time:
    FILE* in = fopen(input_file_name, "rb");
    ABORT_ON(in == NULL)
    
    BitWriter bw;
    bit_writer_init(&bw, out);

    uint8_t* inbuf = malloc(BUFFER_SIZE);
    ABORT_ON(inbuf == NULL);

    for (;;) {
        size_t read = fread(inbuf,
                            1, 
                            BUFFER_SIZE, 
                            in);

        if (read == 0) {
            ABORT_ON(ferror(in));   // error?
            break;                  // EOF
        }

        for (size_t i = 0; i < read; ++i) {
            uint8_t symbol = inbuf[i];

            Codeword* c = codetable_get(code_table, symbol);

            bit_writer_put_bits(&bw, c->bits, c->length);
        }
    }

    bit_writer_finish(&bw);
    bit_writer_free(&bw);

    fclose(in);
    fclose(out);

	free(inbuf);
    free(header_writer);
    free(header_byte_array);
    codetable_free(code_table);
    free(code_table);
    free(fd);
}
