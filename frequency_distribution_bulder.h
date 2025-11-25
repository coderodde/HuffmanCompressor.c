#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H

#include "frequency_distribution.h"

FrequencyDistribution* frequency_distribution_builder_build(const uint8_t* const data,
                                                            const size_t data_size);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H