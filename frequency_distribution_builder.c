#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"

FrequencyDistribution* frequency_distribution_builder_build(const uint8_t* const data,
                                                            const size_t data_size) {

    ABORT_ON("frequency_distribution_builder_build", data == NULL);

    FrequencyDistribution* distribution = malloc(sizeof(FrequencyDistribution));

    if (distribution == NULL) {
        return NULL;
    }

    frequency_distribution_init(distribution);

    for (size_t i = 0; i < data_size; ++i) {
        frequency_distribution_increment(distribution, data[i]);
    }

    return distribution;
}