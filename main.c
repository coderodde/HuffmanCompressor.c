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

    uint8_t data[4];
    data[0] = 0x45;
    data[1] = 0x46;
    data[2] = 0x47;
    data[3] = 0x45;

    FrequencyDistribution* fd = frequency_distribution_builder_build(data, 4);

    CodeTable* ct = codetable_builder_build(fd);

    printf("%s\n", codetable_to_string(ct));
    printf("%s\n", __FUNCTION__);
    printf("%s\n", __FILE__);
    ABORT_ON(NULL == 0)

    return EXIT_SUCCESS;
}
