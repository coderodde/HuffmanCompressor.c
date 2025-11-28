#include "codetable.h"
#include "codetable_builder.h"
#include "frequency_distribution.h"
#include "frequency_distribution_bulder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const size_t ta = get_ms();
    FrequencyDistribution* fd = frequency_distribution_builder_build("C:\\Users\\rodio\\Documents\\WarAndPeace.html");
    const size_t tb = get_ms();

    printf("Duration: %zu ms\n", tb - ta);

    CodeTable* ct = codetable_builder_build(fd);

    printf("%s\n", codetable_to_string(ct));
    printf("%s\n", __FUNCTION__);
    printf("%s\n", __FILE__);
    ABORT_ON(NULL == 0)

    return EXIT_SUCCESS;
}
