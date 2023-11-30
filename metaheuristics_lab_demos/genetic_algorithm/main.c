#include <stdio.h>
#include "bizzare_backpack.h"
#include "bvarlogger.h"



void bizzare_backpack_demo() {
    backpack_t backpack = get_bizzare_backpack();

    ga_bin_r_basic_config_t gbrConfig;
    gbrConfig.epochs = 10;
    gbrConfig.mutation_probability = 0.3;
    gbrConfig.dropout = 0.4;
    gbrConfig.mutation_method = ALLEL_FLIP;
    gbrConfig.crossover_method = MASK;
    gbrConfig.parentingPoolSelection = ROULETTE;
    gbrConfig.veteranSelection = ROULETTE;
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

    /*bbatch_stat_t stats = bbatch("bizzare_backpack", 10000, backpack, bpoConfig, QUIET);
    print_stat(stats); return;*/

    bvarlogger_config_t bvarloggerConfig;
    bvarloggerConfig.logger_name = "bizzare_backpack";
    bvarloggerConfig.backpack = backpack;
    bvarloggerConfig.bpoConfig = bpoConfig;


    ///<mask>
        ///<mask_elite_elite>
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___mask___elite___elite.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___mask___elite___elite.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___mask___elite___elite.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___lel_flip___mask___elite___elite.galog");
        ///</mask_elite_elite>
        
        ///<mask_roulette_roulette>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = ROULETTE;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___mask___roulette___roulette.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___mask___roulette___roulette.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___mask___roulette___roulette.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___mask___roulette___roulette.galog");
        ///</mask_roulette_roulette>

        ///<mask_roulette_tournament>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = TOURNAMENT;
        bpoConfig.gbrConfig = gbrConfig;
        double tournamnet_groupSizeCoefficient = 0.2;
        double tournament_determinismFactor = 1;
        bpoConfig.tournamentGroupSizePtr = &tournamnet_groupSizeCoefficient;
        bpoConfig.tournamentDeterminismFactorPtr = &tournament_determinismFactor;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___mask___roulette___tournament_0_2__prob_1.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___mask___roulette___tournament_0_2__prob_1.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___mask___roulette___roulette__tournament_0_2__prob_1.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___mask___roulette___roulette__tournament_0_2__prob_1.galog");
        ///</mask_roulette_tournament>


        ///<mask_ranking_ranking>
        gbrConfig.parentingPoolSelection = RANKING;
        gbrConfig.veteranSelection = RANKING;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 0.01, 0.0001, "bizarre_backpack_dropout___allel_flip___mask___ranking___ranking.galog");
        varlog_mutprob(0.0, 0.1, 0.001, "bizarre_backpack_mutprob___allel_flip___mask___ranking___ranking.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___mask___ranking___ranking.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___mask___ranking___ranking.galog");
        ///</mask_ranking_ranking>
    ///</mask>

    ///<singlecut>
        ///<singlecut_elite_elite>
        gbrConfig.crossover_method = SINGLE_CUT;
        gbrConfig.parentingPoolSelection = ELITE;
        gbrConfig.veteranSelection = ELITE;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___singlecut___elite___elite.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___singlecut___elite___elite.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___singlecut___elite___elite.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___singlecut___elite___elite.galog");
        ///</singlecut_elite_elite>
        
        ///<singlecut_roulette_roulette>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = ROULETTE;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___singlecut___roulette___roulette.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___singlecut___roulette___roulette.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___singlecut___roulette___roulette.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___singlecut___roulette___roulette.galog");
        ///</singlecut_roulette_roulette>

        ///<singlecut_roulette_tournament>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = TOURNAMENT;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___singlecut___roulette___tournament_0_2__prob_1.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___singlecut___roulette___tournament_0_2__prob_1.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___singlecut___roulette___roulette__tournament_0_2__prob_1.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___singlecut___roulette___roulette__tournament_0_2__prob_1.galog");
        ///</singlecut_roulette_tournament>


        ///<singlecut_ranking_ranking>
        gbrConfig.parentingPoolSelection = RANKING;
        gbrConfig.veteranSelection = RANKING;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 0.01, 0.0001, "bizarre_backpack_dropout___allel_flip___singlecut___ranking___ranking.galog");
        varlog_mutprob(0.0, 0.1, 0.001, "bizarre_backpack_mutprob___allel_flip___singlecut___ranking___ranking.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___singlecut___ranking___ranking.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___singlecut___ranking___ranking.galog");
        ///</singlecut_ranking_ranking>
    ///</singlecut>

    ///<doublecut>
        ///<doublecut_elite_elite>
        gbrConfig.crossover_method = DOUBLE_CUT;
        gbrConfig.parentingPoolSelection = ELITE;
        gbrConfig.veteranSelection = ELITE;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___doublecut___elite___elite.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___doublecut___elite___elite.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___doublecut___elite___elite.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___doublecut___elite___elite.galog");
        ///</doublecut_elite_elite>
        
        ///<doublecut_roulette_roulette>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = ROULETTE;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___doublecut___roulette___roulette.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___doublecut___roulette___roulette.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___doublecut___roulette___roulette.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___doublecut___roulette___roulette.galog");
        ///</doublecut_roulette_roulette>

        ///<doublecut_roulette_tournament>
        gbrConfig.parentingPoolSelection = ROULETTE;
        gbrConfig.veteranSelection = TOURNAMENT;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 1.0, 0.01, "bizarre_backpack_dropout___allel_flip___doublecut___roulette___tournament_0_2__prob_1.galog");
        varlog_mutprob(0.0, 1.0, 0.01, "bizarre_backpack_mutprob___allel_flip___doublecut___roulette___tournament_0_2__prob_1.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___doublecut___roulette___roulette__tournament_0_2__prob_1.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___doublecut___roulette___roulette__tournament_0_2__prob_1.galog");
        ///</doublecut_roulette_tournament>


        ///<doublecut_ranking_ranking>
        gbrConfig.parentingPoolSelection = RANKING;
        gbrConfig.veteranSelection = RANKING;
        bpoConfig.gbrConfig = gbrConfig;
        bvarloggerConfig.bpoConfig = bpoConfig;
        init_bvarlogger(bvarloggerConfig);
        varlog_dropout(0.0, 0.01, 0.0001, "bizarre_backpack_dropout___allel_flip___doublecut___ranking___ranking.galog");
        varlog_mutprob(0.0, 0.1, 0.001, "bizarre_backpack_mutprob___allel_flip___doublecut___ranking___ranking.galog");
        varlog_popsize(0, 200, 1, "bizarre_backpack_popsize___allel_flip___doublecut___ranking___ranking.galog");
        //varlog_epochs(0, 200, 1, "bizzare_backpack_epochs___allel_flip___doublecut___ranking___ranking.galog");
        ///</doublecut_ranking_ranking>
    ///</doublecut>

}


int main() {
    printf("Place for genetic algorighm demos batch run\n");
    bizzare_backpack_demo();
    return 0;
}