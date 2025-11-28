#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

// File read buffer size in bytes, 64 KiB:
#define BUFFER_SIZE (1024 * 4)

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
    frequency_distribution_init(distribution);

    uint8_t* buffer = malloc(data_size);
    ABORT_ON(buffer == NULL)

    FILE* file = fopen(file_name, "rb");
    ABORT_ON(file == NULL)
    ABORT_ON(setvbuf(file, (char*) buffer, _IOFBF, data_size) != 0)

    const size_t bytes_read = fread(buffer, sizeof(uint8_t), data_size, file);
        
    ABORT_ON(bytes_read != data_size)

    for (size_t i = 0; i < data_size; ++i) {
        frequency_distribution_increment(distribution, (uint8_t) buffer[i]);
    }

    return distribution;
}
