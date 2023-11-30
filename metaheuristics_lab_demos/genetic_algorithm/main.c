#include <stdio.h>
#include "bizzare_backpack.h"
//#include "bpack_optimizer.h"
//#include "backpack_helper.h"
//#include "bbatch.h"
#include "bvarlogger.h"



void bizzare_backpack_demo() {
    backpack_t backpack = get_bizzare_backpack();
    // genes_loc(40 * 4);

    ga_bin_r_basic_config_t gbrConfig;
    gbrConfig.epochs = 10;
    gbrConfig.mutation_probability = 0.3;
    gbrConfig.dropout = 0.4;
    gbrConfig.mutation_method = ALLEL_FLIP;
    gbrConfig.crossover_method = MASK;
    gbrConfig.parentingPoolSelection = ELITE;
    gbrConfig.veteranSelection = ELITE;
    gbrConfig.chromosome_length = backpack.num_available;

    bpack_optimizer_config_t bpoConfig;
    bpoConfig.gbrConfig = gbrConfig;
    bpoConfig.gaPopulationSize = 200;
    bpoConfig.tournamentGroupSizePtr = NULL;
    bpoConfig.tournamentDeterminismFactorPtr = NULL;

    

    /*unsigned char* solutionMask = optimized_backpack_composition_mask(backpack, bpoConfig);
    
    printf("\n");
    print_backpack_composition_mask(backpack, solutionMask);
    printf("\n");
    print_backpack(backpack, solutionMask);*/

    /*bbatch_stat_t stats = bbatch("bizzare_backpack", 10, backpack, bpoConfig, VERBOSE);
    print_stat(stats);*/

    bvarlogger_config_t bvarloggerConfig;
    bvarloggerConfig.logger_name = "bizzare_backpack";
    bvarloggerConfig.backpack = backpack;
    bvarloggerConfig.bpoConfig = bpoConfig;

    init_bvarlogger(bvarloggerConfig);
    varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___mask___elite___elite.galog");
    varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___mask___elite___elite.galog");
    varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___mask___elite___elite.galog");
        
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