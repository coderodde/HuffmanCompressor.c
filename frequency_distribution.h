#ifndef IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_H
#define IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_H

#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

#define FREQUENCY_DISTRIBUTION_CAPACITY 256

typedef struct FrequencyDistribution {
    size_t frequencies[FREQUENCY_DISTRIBUTION_CAPACITY];
} FrequencyDistribution;

void frequency_distribution_init(FrequencyDistribution *const distribution);
void frequency_distribution_increment(
    FrequencyDistribution *const distribution,
    const uint8_t byte);

size_t frequency_distribution_get_frequency(
    const FrequencyDistribution* const distribution,
    const uint8_t byte);

size_t frequency_distribution_size(
    const FrequencyDistribution* const distribution);

#endif // IO_GITHUB_CODERODDE_HUFFMAN_COMPRESSOR_C_FREQUENCY_DISTRIBUTION_H