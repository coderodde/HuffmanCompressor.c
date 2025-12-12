#include "byte_array_header_reader.h"
#include "codetable.h"
#include "decompressor.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t BUFFER_SIZE = 1024 * 64;
static const size_t HEADER_MAX_SIZE  = 16 + 256 * 6;
static const size_t CODE_ENTRY_LENGTH = 6;

typedef struct DecoderTreeNode {
    struct DecoderTreeNode* zero_child;
    struct DecoderTreeNode* one_child;
    uint8_t*                byte;
} 
DecoderTreeNode;

typedef struct DecoderTree {
    DecoderTreeNode* root;
}
DecoderTree;

static void decoder_tree_init(
    DecoderTree* tree
)
{
    ABORT_ON(tree == NULL)

    tree->root = malloc(sizeof(DecoderTreeNode));

    ABORT_ON(tree->root == NULL)

    *(tree->root) = (DecoderTreeNode) {
        .zero_child = NULL,
        .one_child  = NULL,
        .byte       = NULL
    };
}

static void decoder_tree_free(
    DecoderTreeNode* node
)
{
    if (node == NULL) {
        return;
    }

    decoder_tree_free(node->zero_child);
    decoder_tree_free(node->one_child);

    free(node->byte);
    free(node);
}

static void decoder_tree_insert(
    DecoderTree* tree,
    uint8_t      value,
    Codeword*    codeword
)
{
    ABORT_ON(tree == NULL)
    ABORT_ON(codeword == NULL)

    DecoderTreeNode* node = tree->root;
    size_t length = codeword_length(codeword);

    SSIZE_T current_index = (SSIZE_T) length - 1;

    for (; current_index >= 0; --current_index) {

        bool bit = codeword_get_bit(codeword, 
                                    (size_t) current_index);

        if (bit) {
            // bit == 1:
            if (node->one_child == NULL) {
                node->one_child = malloc(sizeof(DecoderTreeNode));

                ABORT_ON(node->one_child == NULL)

                *(node->one_child) = (DecoderTreeNode) {
                    .zero_child = NULL,
                    .one_child  = NULL,
                    .byte       = NULL
                };
            }

            node = node->one_child;
        } else {
            // bit == 0:
            if (node->zero_child == NULL) {
                node->zero_child = malloc(sizeof(DecoderTreeNode));

                ABORT_ON(node->zero_child == NULL)

                *(node->zero_child) = (DecoderTreeNode) {
                    .zero_child = NULL,
                    .one_child  = NULL,
                    .byte       = NULL
                };
            }

            node = node->zero_child;
        }
    }

    ABORT_ON(node->byte != NULL)
    node->byte = malloc(sizeof(uint8_t));
    *(node->byte) = value;
}

static DecoderTreeNode* decoder_tree_decode(
    DecoderTreeNode* starting_node,
    uint8_t          bits,
    size_t           starting_bit_index,
    size_t           length
)
{
    ABORT_ON(starting_node == NULL)
    size_t i = 0;

    while (i < length) {
        bool bit = (bits >> (starting_bit_index - i)) & 1;
        if (bit) {
            // bit == 1:
            starting_node = starting_node->one_child;
        } else {
            // bit == 0:
            starting_node = starting_node->zero_child;
        }

        if (starting_node->byte != NULL) {
            return starting_node;
        }

        ++i;
    }

    return starting_node;
}

void decompress(
    char* input_file_name,
    char* output_file_name
)
{
    FILE* in = fopen(input_file_name, "rb");
    ABORT_ON(in == NULL)

    FILE* out = fopen(output_file_name, "wb");
    ABORT_ON(out == NULL)

    uint8_t* input_buffer = malloc(BUFFER_SIZE);
    ABORT_ON(input_buffer == NULL)

    uint8_t* output_buffer = malloc(BUFFER_SIZE);
    ABORT_ON(output_buffer == NULL)

    // Read the code table size and raw data length first:
    size_t bytes_read = fread(input_buffer, 1, 2 * sizeof(size_t), in);

    ABORT_ON(bytes_read != 2 * sizeof(size_t))

    size_t code_table_size = 0;
    size_t raw_data_length = 0;

    for (size_t i = 0; i < sizeof(size_t); ++i) {
        code_table_size |= ((size_t) input_buffer[i]) << (8 * i);
        raw_data_length |= ((size_t) input_buffer[sizeof(size_t) + i]) << (8 * i);
    }

    CodeTable* ct = malloc(sizeof *ct);
    ABORT_ON(ct == NULL);

    DecoderTree tree;
    decoder_tree_init(&tree);

    size_t current_offset = 2 * sizeof(size_t);

    for (size_t i = 0; i < code_table_size; ++i) {
        bytes_read = fread(input_buffer + current_offset,
                           1,
                           CODE_ENTRY_LENGTH, 
                           in);

        ABORT_ON(bytes_read != CODE_ENTRY_LENGTH)

        uint8_t byte   = input_buffer[current_offset++];
        uint8_t length = input_buffer[current_offset++];

        size_t number_of_bytes = codeword_number_of_bytes(length);
        uint8_t codeword_bytes[4];

        memcpy(codeword_bytes,
               &input_buffer[current_offset], 
               4);

        Codeword* codeword = malloc(sizeof *codeword);
        ABORT_ON(codeword == NULL)

        *codeword = (Codeword) {
            .length = length,
            .bits   = 0
        };

        for (size_t j = 0; j < number_of_bytes; ++j) {
            codeword->bits |= ((uint32_t) codeword_bytes[j]) << (8 * j);
        }

        codetable_put(ct,
                      byte, 
                      codeword);

        decoder_tree_insert(&tree,
                            byte,
                            codeword);

        current_offset += CODE_ENTRY_LENGTH - 2;
    }

    DecoderTreeNode* node = tree.root;

    size_t produced = 0;
    size_t out_pos  = 0;

    while (produced < raw_data_length) {
        bytes_read = fread(input_buffer, 1, BUFFER_SIZE, in);

        ABORT_ON(bytes_read == 0 && ferror(in))

        for (size_t bi = 0; bi < bytes_read && produced < raw_data_length; ++bi) {
            uint8_t b = input_buffer[bi];

            for (int bitpos = 7; bitpos >= 0 && produced < raw_data_length; --bitpos) {
                bool bit = (b >> bitpos) & 1;

                if (bit) {
                    node = node->one_child;
                } else {
                    node = node->zero_child;
                }

                ABORT_ON(node == NULL)

                if (node->byte != NULL) {
                    // We found a byte!
                    output_buffer[out_pos++] = *(node->byte);
                    produced++;
                    node = tree.root;

                    if (out_pos == BUFFER_SIZE) {
                        size_t bytes_written = fwrite(output_buffer, 
                                                      1, 
                                                      out_pos, 
                                                      out);

                        ABORT_ON(bytes_written != out_pos)
                        out_pos = 0;
                    }
                }
            }
        }
    }

    if (out_pos > 0) {
        size_t bytes_written = fwrite(output_buffer, 
                                      1, 
                                      out_pos, 
                                      out);

        ABORT_ON(bytes_written != out_pos)
    }

    decoder_tree_free(tree.root);
    fclose(in);
    fclose(out);
    free(input_buffer);
    free(output_buffer);
}