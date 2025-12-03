#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H

#include "frequency_distribution.h"
#include "utils.h"

FrequencyDistribution* frequency_distribution_builder_build(
    char* file_name
);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_BUILDER_H