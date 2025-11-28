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

// Thread chunk size in bytes, 64 KiB:
#define CHUNK_SIZE (1024 * 64)
#define BUFFER_SIZE (1024 * 8)

typedef struct ThreadArguments {
    const uint8_t* data;
    size_t start_byte_index;
    size_t end_byte_index;
    const char *const file_name;
}
ThreadArguments;

#ifdef _WIN32

DWORD WINAPI thread_function(LPVOID lpParam) {
    ThreadArguments *const args = (ThreadArguments *const) lpParam;
    FrequencyDistribution* local_distribution = malloc(sizeof(FrequencyDistribution));
    
    ABORT_ON(local_distribution == NULL);

    frequency_distribution_init(local_distribution);

    FILE* file = fopen(args->file_name, "rb");
   
    ABORT_ON(file == NULL);

#ifdef _WIN32
    const int res = _fseeki64(file, args->start_byte_index, SEEK_SET);
    ABORT_ON(res != 0)
#else
    
#endif

    uint8_t buffer[BUFFER_SIZE];

    for (size_t i = args->start_byte_index; i < args->end_byte_index; ++i) {
        frequency_distribution_increment(local_distribution, args->data[i]);
    }

    return (DWORD)(uintptr_t)local_distribution;
}

#else

#endif

FrequencyDistribution* frequency_distribution_builder_build(const uint8_t* const data,
                                                            const size_t data_size) {

    ABORT_ON(data == NULL)
    ABORT_ON(data_size == 0)

    const size_t number_of_chunks = (data_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    ThreadArguments* *const thread_arguments_array = 
        malloc(sizeof(ThreadArguments*) * number_of_chunks);

    for (size_t i = 0; i < number_of_chunks; ++i) {
        const size_t start_byte_index = i * CHUNK_SIZE;
        size_t end_byte_index = start_byte_index + CHUNK_SIZE;

        if (end_byte_index > data_size) {
            end_byte_index = data_size;
        }

        ThreadArguments *const thread_arguments = 
            malloc(sizeof(ThreadArguments));

        thread_arguments->data = data;
        thread_arguments->start_byte_index = start_byte_index;
        thread_arguments->end_byte_index = end_byte_index;
        thread_arguments_array[i] = thread_arguments;
    }

#ifdef _WIN32
    HANDLE* thread_handles = malloc(sizeof(HANDLE) * number_of_chunks);

    // Windows threading code would go here:
    for (size_t i = 0; i < number_of_chunks; ++i) {
        HANDLE thread_handle = CreateThread(
            NULL,
            0,
            thread_function,
            (LPVOID) thread_arguments_array[i],
            0,
            NULL
        );

        ABORT_ON(thread_handle == NULL);
        thread_handles[i] = thread_handle;
    }

    for (size_t i = 0; i < number_of_chunks; ++i) {
        // Wait for thread to finish and collect results:
        WaitForSingleObject(thread_handles[i], INFINITE);
    }
#else

#endif

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