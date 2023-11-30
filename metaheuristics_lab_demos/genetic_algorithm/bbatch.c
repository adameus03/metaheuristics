#include "bbatch.h"
#include "backpack_helper.h"
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

void print_chromosome(const void* chromosome, const unsigned int num_octets) {
    unsigned char* uc_chromo = (unsigned char*)chromosome;
    for (unsigned int i = 0; i < num_octets; i++) {
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(uc_chromo[i]));
    }
}

bbatch_stat_t bbatch(const char* batch_name, const unsigned long int runTimes,
           const backpack_t backpack, const bpack_optimizer_config_t bpoConfig, const BBATCH_VERBOSITY_MODE verbosity) {
    bbatch_result_t minStat;
    bbatch_result_t maxStat;
    bbatch_result_t sumStat;

    minStat.bpack_content_fitness = __DBL_MAX__;
    minStat.bpack_content_weight = __UINT32_MAX__;
    
    maxStat.bpack_content_fitness = -__DBL_MAX__;
    maxStat.bpack_content_weight = 0;

    sumStat.bpack_content_fitness = 0;
    sumStat.bpack_content_weight = 0;
    
    //printf("\nRunning backpack batch \"%s\" of size %d with params: T=%4.2f, k=%4.2f, a=%4.5f, epochs=%d, iters=%d, memory=%s, mode=%s\n\n", batch_name, runTimes, config.temperature, config.boltzmann_k, config.cooldown, config.epochs, config.epoch_iters, config.mem_mode==WITH_MEMORY?"yes":"no", config.emode==MAX?"max":"min");
    char* mut_meth = bpoConfig.gbrConfig.mutation_method == ALLEL_FLIP ? "ALLEL_FLIP" : "REVERSE";
    char* cross_meth;
    switch (bpoConfig.gbrConfig.crossover_method) {
        case SINGLE_CUT:
            cross_meth = "SINGLE_CUT";
            break;
        case DOUBLE_CUT:
            cross_meth = "DOUBLE_CUT";
            break;
        default:
            cross_meth = "MASK";
            break;
    }
    char parsel[50]; //{{less}}
    switch (bpoConfig.gbrConfig.parentingPoolSelection) {
        case ROULETTE:
            snprintf(parsel, sizeof(parsel), "ROULETTE");
            break;
        case RANKING:
            snprintf(parsel, sizeof(parsel), "RANKING");
            break;
        case ELITE:
            snprintf(parsel, sizeof(parsel), "ELITE");
            break;
        default:
            snprintf(parsel, sizeof(parsel), "TOURNAMENT(prob=%4.5f, %%pop=%4.5f)", *(bpoConfig.tournamentDeterminismFactorPtr), *(bpoConfig.tournamentGroupSizePtr));
            break;
    }
    char vetsel[50];
    switch (bpoConfig.gbrConfig.veteranSelection) {
        case ROULETTE:
            snprintf(vetsel, sizeof(vetsel), "ROULETTE");
            break;
        case RANKING:
            snprintf(vetsel, sizeof(vetsel), "RANKING");
            break;
        case ELITE:
            snprintf(vetsel, sizeof(vetsel), "ELITE");
            break;
        default:
            snprintf(vetsel, sizeof(vetsel), "TOURNAMENT(prob=%4.5f, %%pop=%4.5f)", *(bpoConfig.tournamentDeterminismFactorPtr), *(bpoConfig.tournamentGroupSizePtr));
            break;
    }
    printf("\nRunning backpack batch \"%s\" of size %d with params: epochs=%d, popsize=%d, mut_prob=%4.5f, dropout=%4.5f, mut_meth=%s, cross_meth=%s, parsel=%s, vetsel=%s\n\n", 
        batch_name, runTimes, bpoConfig.gbrConfig.epochs, bpoConfig.gaPopulationSize, bpoConfig.gbrConfig.mutation_probability,
        bpoConfig.gbrConfig.dropout, mut_meth, cross_meth, parsel, vetsel);

    //unsigned int occurences // maybe the other day...
    for (unsigned long int i = 0; i < runTimes; i++) {
        static unsigned char* solutionMask;
        solutionMask = optimized_backpack_composition_mask(backpack, bpoConfig);
        
        if (verbosity == QUIET) {
            printf("\33[2K\r(%d / %d) Sampling evolutions...", i + 1, runTimes);
            //fflush(stdout);
        }
        if (verbosity == VERBOSE) {
            static unsigned int num_octets;
            num_octets = (backpack.num_available >> 0x3) + (backpack.num_available % 0x8 != 0x0);
            printf("(%d / %d) SOL=", i + 1, runTimes);
            print_chromosome((void*)solutionMask, num_octets);
            printf(", BPK_WEIGHT=%d, BPK_VALUE=%d, BPK_FITNESS=%d", 
                calc_backpack_composition_weight(backpack, solutionMask),
                calc_backpack_composition_value(backpack, solutionMask),
                calc_backpack_composition_fitness(backpack, solutionMask));
            printf("\n");
        }

        static unsigned int calculatedFitness;
        static unsigned int calculatedWeight;
        calculatedFitness = (double)calc_backpack_composition_fitness(backpack, solutionMask);
        calculatedWeight = calc_backpack_composition_weight(backpack, solutionMask);

        sumStat.bpack_content_fitness += calculatedFitness;
        sumStat.bpack_content_weight += calculatedWeight;

        if (calculatedFitness > maxStat.bpack_content_fitness) {
            maxStat.bpack_content_fitness = calculatedFitness;
        }
        if (calculatedWeight > maxStat.bpack_content_weight) {
            maxStat.bpack_content_weight = calculatedWeight;
        }

        if (calculatedFitness < minStat.bpack_content_fitness) {
            minStat.bpack_content_fitness = calculatedFitness;
        }
        if (calculatedWeight < minStat.bpack_content_weight) {
            minStat.bpack_content_weight = calculatedWeight;
        }
    }
    if (verbosity == QUIET) {
        printf("\n");
    }

    static bbatch_stat_t stats;
    stats.batch_name = batch_name;
    stats.result_average.bpack_content_fitness = sumStat.bpack_content_fitness / runTimes;
    stats.result_average.bpack_content_weight = sumStat.bpack_content_weight / runTimes;

    stats.result_minima = minStat;
    stats.result_maxima = maxStat;

    return stats;
}

void print_stat(const bbatch_stat_t stats) {
    printf("\n=========== %s final generation obtained ===========\n", stats.batch_name);
    printf("Weight: avg %d min %d max %d\n", stats.result_average.bpack_content_weight, stats.result_minima.bpack_content_weight, stats.result_maxima.bpack_content_weight);
    printf("Fitness: avg %4.0f min %4.0f max %4.0f\n", stats.result_average.bpack_content_fitness, stats.result_minima.bpack_content_fitness, stats.result_maxima.bpack_content_fitness);   
}