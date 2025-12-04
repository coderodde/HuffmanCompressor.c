#include "frequency_distribution.h"
#include "utils.h"

#define FREQUENCY_DISTRIBUTION_CAPACITY 256

void frequency_distribution_init(
    FrequencyDistribution* distribution
)
{
    ABORT_ON(distribution == NULL);

    for (size_t i = 0; i < FREQUENCY_DISTRIBUTION_CAPACITY; ++i) {
        distribution->frequencies[i] = 0;
    }
}

void frequency_distribution_increment(
    FrequencyDistribution* distribution,
    uint8_t byte
)
{
    ABORT_ON(distribution == NULL);
    distribution->frequencies[byte]++;
}

size_t frequency_distribution_get_frequency(
    FrequencyDistribution* distribution,
    uint8_t byte
)
{
    ABORT_ON(distribution == NULL);
    return distribution->frequencies[byte];
}

size_t frequency_distribution_size(
    FrequencyDistribution* distribution
)
{
    ABORT_ON(distribution == NULL);
    size_t size = 0;

    for (size_t i = 0; i < FREQUENCY_DISTRIBUTION_CAPACITY; ++i) {
        if (distribution->frequencies[i] != 0) {
            size++;
        }
    }

    return size;
}
