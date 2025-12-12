#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// File read buffer size in bytes, 64 KiB:
#define BUFFER_SIZE (1024 * 64)

FrequencyDistribution* frequency_distribution_builder_build(
    char* file_name
)
{
    ABORT_ON(file_name == NULL)

    size_t data_size = get_file_length_by_name(file_name);
    FILE* file = fopen(file_name, "rb");

    ABORT_ON(file == NULL)

    FrequencyDistribution* distribution = malloc(sizeof *distribution);
    
    ABORT_ON(distribution == NULL)

    frequency_distribution_init(distribution);

    uint8_t* buffer = malloc(BUFFER_SIZE);
    ABORT_ON(buffer == NULL)
    ABORT_ON(setvbuf(file, (char*) buffer, _IOFBF, BUFFER_SIZE) != 0)

    size_t bytes_remaining = data_size;

    while (bytes_remaining > 0) {

        size_t bytes_to_read = 
            (bytes_remaining > BUFFER_SIZE ? BUFFER_SIZE : bytes_remaining);

        size_t bytes_read = fread(buffer, 
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
    free(buffer);
    return distribution;
}
