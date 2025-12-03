#include "codetable.h"
#include "codetable_builder.h"
#include "compressor.h"
#include "decompressor.h"
#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s <-c|-d> <INPUT_FILE> <OUTPUT_FILE>\n", 
               extract_file_name_only(argv[0]));

        return EXIT_FAILURE;
    }

    const char* const mode = argv[1];

    if (strcmp(mode, "-c") == 0) {
        const char* const input_file  = argv[2];
        const char* const output_file = argv[3];

        infof("Compressing file '%s' to '%s'.", input_file, output_file);

        compress(input_file, output_file);

        infof("Compression completed.");

    } else if (strcmp(mode, "-d") == 0) {
        const char* const input_file  = argv[2];
        const char* const output_file = argv[3];

        infof("Decompressing file '%s' to '%s'.", input_file, output_file);
        
        decompress(input_file, output_file);

        infof("Decompression completed.");
    }
    else {
        printf("Unknown mode '%s'. Use 'c' for compression and 'd' for decompression.\n", mode);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
