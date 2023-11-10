#include <stdio.h>
#include "bizzare_backpack.h"

int main() {
    printf("Place for genetic algorighm demos batch run\n");
    backpack_t bizzareBackpack = get_bizzare_backpack();
    for (unsigned int i = 0; i < bizzareBackpack.num_available; i++) {
        printf("%d %s\n", i, bizzareBackpack.available_names[i]);
    }
    return 0;
}