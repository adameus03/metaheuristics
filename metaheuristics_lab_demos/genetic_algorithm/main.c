#include <stdio.h>
#include "bizzare_backpack.h"
#include "bpack_optimizer.h"
#include "backpack_helper.h"



void bizzare_backpack_demo() {
    backpack_t backpack = get_bizzare_backpack();
    // genes_loc(40 * 4);
    unsigned char* solutionMask = optimized_backpack_composition_mask(backpack);
    // print_backpack(backpack, solutionMask);
    printf("\n");
    print_backpack_composition_mask(backpack, solutionMask);
    printf("\n");
    print_backpack(backpack, solutionMask);
}


int main() {
    printf("Place for genetic algorighm demos batch run\n");
    /* backpack_t bizzareBackpack = get_bizzare_backpack();
    for (unsigned int i = 0; i < bizzareBackpack.num_available; i++) {
        printf("%d %s\n", i, bizzareBackpack.available_names[i]);
    } */
    bizzare_backpack_demo();
    return 0;
}