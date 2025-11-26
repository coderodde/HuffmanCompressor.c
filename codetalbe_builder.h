#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_BUILDER_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_BUILDER_H

#include "codetable.h"
#include "frequency_distribution.h"

CodeTable* codetable_builder_build(
    const FrequencyDistribution* const frequency_distribution);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_CODETABLE_BUILDER_H