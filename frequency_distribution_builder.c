#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// File read buffer size in bytes, 64 KiB:
#define BUFFER_SIZE (1024 * 64)

static size_t file_size(const char *const file_name) {
    FILE* file = fopen(file_name, "rb");
    ABORT_ON(file == NULL)
    const int res = _fseeki64(file, 0, SEEK_END);
    ABORT_ON(res != 0)
    const size_t size = (size_t) _ftelli64(file);
    fclose(file);
    return size;
}

FrequencyDistribution* frequency_distribution_builder_build(const char *const file_name) {
    const size_t data_size = file_size(file_name);

    FrequencyDistribution* distribution = malloc(sizeof *distribution);
    
    ABORT_ON(distribution == NULL)

    frequency_distribution_init(distribution);

    FILE* file = fopen(file_name, "rb");
    ABORT_ON(file == NULL)
    uint8_t* buffer = malloc(BUFFER_SIZE);
    ABORT_ON(buffer == NULL)
    ABORT_ON(setvbuf(file, (char*) buffer, _IOFBF, BUFFER_SIZE) != 0)

    size_t bytes_remaining = data_size;

    while (bytes_remaining > 0) {

        const size_t bytes_to_read = 
            (bytes_remaining > BUFFER_SIZE ? BUFFER_SIZE : bytes_remaining);

        const size_t bytes_read = fread(buffer, 
                                        sizeof(uint8_t), 
                                        bytes_to_read, 
                                        file);

        ABORT_ON(bytes_read != bytes_to_read)
        ABORT_ON(ferror(file) != 0)

        for (size_t i = 0; i < bytes_read; ++i) {
            frequency_distribution_increment(distribution, buffer[i]);
        }

        bytes_remaining -= bytes_read;
    }

    fclose(file);
    return distribution;
}
