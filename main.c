#include "codetable.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    CodeTable code_table;
    Codeword* cw1 = codeword_alloc(3);
    Codeword* cw2 = codeword_alloc(5);

    codeword_set_bit(cw1, 0);
    codeword_set_bit(cw1, 2);

    codeword_set_bit(cw2, 1);
    codeword_set_bit(cw2, 4);

    codetable_init(&code_table);
    codetable_put(&code_table, 66, cw1); // Put codeword for 'B'

    codetable_put(&code_table, 65, cw2); // Put codeword for 'A'

    char* table_str = codetable_to_string(&code_table);

    printf("CodeTable: %s\n", table_str);

    free(table_str);
    return EXIT_SUCCESS;
}
