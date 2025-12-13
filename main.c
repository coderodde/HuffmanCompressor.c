#include "codetable.h"
#include "codetable_builder.h"
#include "compressor.h"
#include "decompressor.h"
#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(size_t) == 8, "Expected 64-bit size_t.");

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s <-c|-d> INPUT_FILE OUTPUT_FILE\n", 
               extract_file_name_only(argv[0]));
		puts("              -c for compressing,");
		puts("              -d for decompressing.");
        return EXIT_FAILURE;
    }

    char* mode = argv[1];

    if (strcmp(mode, "-c") == 0) {
        char* input_file  = argv[2];
        char* output_file = argv[3];

        infof("Compressing file '%s' to '%s'.", input_file, output_file);

        size_t ta = get_ms();
        compress(input_file, output_file);
        size_t tb = get_ms();

        infof("Compression completed in %zu ms.", tb - ta);

    } else if (strcmp(mode, "-d") == 0) {
        char* input_file  = argv[2];
        char* output_file = argv[3];

        infof("Decompressing file '%s' to '%s'.", input_file, output_file);
        
        size_t ta = get_ms();
        decompress(input_file, output_file);
        size_t tb = get_ms();

        infof("Decompression completed in %zu ms.", tb - ta);
    } else {
        printf("Unknown mode '%s'. Use 'c' for compression and 'd' for decompression.\n", mode);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
