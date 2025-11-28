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

// File read buffer size in bytes, 8 KiB:
#define BUFFER_SIZE (1024 * 8)

typedef struct ThreadArguments {
    size_t start_byte_index;
    size_t end_byte_index;
    const char* file_name;
    FrequencyDistribution* result_distribution;
}
ThreadArguments;

#ifdef _WIN32

DWORD WINAPI thread_function(LPVOID lpParam) {
    ThreadArguments *const args = (ThreadArguments *const) lpParam;
    FrequencyDistribution* local_distribution = 
        malloc(sizeof *local_distribution);
    
    ABORT_ON(local_distribution == NULL);

    frequency_distribution_init(local_distribution);

    FILE* file = fopen(args->file_name, "rb");
   
    ABORT_ON(file == NULL);

    const int res = _fseeki64(file, args->start_byte_index, SEEK_SET);

    ABORT_ON(res != 0)

    uint8_t buffer[BUFFER_SIZE];

    const size_t number_of_write_chunks = 
        (args->end_byte_index - args->start_byte_index + BUFFER_SIZE - 1) 
        / BUFFER_SIZE;

    for (size_t i = 0; i < number_of_write_chunks; ++i) {
        size_t bytes_to_read = BUFFER_SIZE;

        if (i == number_of_write_chunks - 1) {
            bytes_to_read = args->end_byte_index - args->start_byte_index - i * BUFFER_SIZE;
        }

        const size_t bytes_read = fread(buffer, 
                                        sizeof(uint8_t),
                                        bytes_to_read,
                                        file);

        ABORT_ON(bytes_read != bytes_to_read);

        for (size_t j = 0; j < bytes_read; ++j) {
            frequency_distribution_increment(local_distribution, buffer[j]);
        }
    }

    fclose(file);

    args->result_distribution = local_distribution;
    return 0;
}

#else

#endif

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
    const size_t number_of_chunks = (data_size + CHUNK_SIZE - 1) / CHUNK_SIZE;

    ThreadArguments* *const thread_arguments_array = 
        malloc(sizeof(ThreadArguments*) * number_of_chunks);

    ABORT_ON(thread_arguments_array == NULL)

    for (size_t i = 0; i < number_of_chunks; ++i) {
        const size_t start_byte_index = i * CHUNK_SIZE;
        size_t end_byte_index = start_byte_index + CHUNK_SIZE;

        if (end_byte_index > data_size) {
            end_byte_index = data_size;
        }

        ThreadArguments *const thread_arguments = 
            malloc(sizeof *thread_arguments);

        ABORT_ON(thread_arguments == NULL)

        thread_arguments->start_byte_index = start_byte_index;
        thread_arguments->end_byte_index = end_byte_index;
        thread_arguments->file_name = file_name;
        thread_arguments->result_distribution = NULL;
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
    ABORT_ON(distribution == NULL)

    frequency_distribution_init(distribution);

    for (size_t i = 0; i < number_of_chunks; ++i) {
        ABORT_ON(thread_arguments_array[i]->result_distribution == NULL);

        FrequencyDistribution* local_distribution = 
            thread_arguments_array[i]->result_distribution;

        frequency_distribution_add(distribution, local_distribution);
        free(thread_arguments_array[i]);
    }

    return distribution;
}