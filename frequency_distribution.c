#include "frequency_distribution.h"
#include "utils.h"

void frequency_distribution_init(FrequencyDistribution *const distribution) {
    ABORT_ON("frequency_distribution_init", distribution == NULL);

    for (size_t i = 0; i < FREQUENCY_DISTRIBUTION_CAPACITY; ++i) {
        distribution->frequencies[i] = 0;
    }
}

void frequency_distribution_increment(FrequencyDistribution *const distribution,
                                      const uint8_t byte) {
    ABORT_ON("frequency_distribution_init", distribution == NULL);
    distribution->frequencies[byte]++;
}

size_t frequency_distribution_get_frequency(
    const FrequencyDistribution* const distribution,
    const uint8_t byte) {
    ABORT_ON("frequency_distribution_init", distribution == NULL);
    return distribution->frequencies[byte];
}

size_t frequency_distribution_size(const FrequencyDistribution* const distribution) {
    ABORT_ON("frequency_distribution_init", distribution == NULL);
    size_t size = 0;

    for (size_t i = 0; i < FREQUENCY_DISTRIBUTION_CAPACITY; ++i) {
        if (distribution->frequencies[i] != 0) {
            size++;
        }
    }

    return size;
}