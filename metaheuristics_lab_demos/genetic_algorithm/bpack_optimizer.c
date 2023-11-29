#include <stdlib.h>
#include "../ga/ga_bin_r_basic.h"
#include "bpack_optimizer.h"

// #define NULL ((void*) 0)

double fitness(const binary_chromosome chromosome, const backpack_t backpack) {
    double total_weight = 0;
    double total_value = 0;
    for (unsigned int i = 0; i < backpack.num_available; i++) {
        if (chromosome[i >> 0x3] & (((unsigned char)0x80U) >> (i % 0x8))) {
            total_weight += backpack.available_weights[i];
            total_value += backpack.available_values[i];
        }
    }
    // if (total_weight > backpack.num_available) {
    if (total_weight > backpack.backpack_capacity) {
        return 0;
    }
    else {
        return total_value;
    }
}

backpack_t __backpack (const backpack_t* backpack) {
    static backpack_t _backpack;
    if (backpack) {
        _backpack = *backpack;
    }
    return _backpack;
}

void* _fitness(const void* chromosome) {
    static double fitnessVal;
    fitnessVal = fitness((binary_chromosome)chromosome, __backpack(NULL));
    return (void*)&fitnessVal;
}

unsigned char* _genes_storage(unsigned char* loc) {
    static unsigned char* _loc;
    if (loc) {
        _loc = loc;
    }
    return _loc;
}
#define genes_loc(memtotal) ({ \
    static unsigned char genes[memtotal]; \
    _genes_storage(genes); \
})

ga_bin_r_basic_population_t* population_generator() {
    static ga_bin_r_basic_population_t population;
    population.size = 200;
    static binary_chromosome _members[200];
    // unsigned int chromo_octet_num = __backpack(NULL).num_available >> 0x3 + (__backpack(NULL).num_available % 0x8 != 0x0);
    unsigned int chromo_octet_num = 4;
    // static unsigned char genes[40 * chromo_octet_num];
    // genes_loc(40 * chromo_octet_num);
    genes_loc( 200 * 4 );
    unsigned char* genes = _genes_storage(NULL);

    for (unsigned int i = 0; i < 200 * chromo_octet_num; i++) {
        genes[i] = (unsigned char)(rand() % 0x100);
        genes[i] &= (unsigned char)(rand() % 0x100);
        /* genes[i] &= (unsigned char)(rand() % 0x100);
        genes[i] &= (unsigned char)(rand() % 0x100);*/
        // genes[i] = 0x0;
        // genes[i] = 0x1;
        // genes[i] = 0x80;
    }
    for (unsigned int i = 0; i < population.size; i++) {
        _members[i] = genes + chromo_octet_num * i;
    }
    population.members = (binary_chromosome*)&_members;
    return &population;
}
// optimized_backpack_element_ids

unsigned char* optimized_backpack_composition_mask(const backpack_t backpack) {
    // const gaFunc f, 
    // const ga_bin_r_basic_config_t config,
    // const ga_bin_r_basic_startup_config_t startupConfig
    __backpack(&backpack);
    ga_bin_set_octet_num(4);
    ga_bin_r_init(200);
    ga_bin_r_basic_config_t config; //MAKE BATCH LATER
    config.epochs = /*1000*/10;
    config.mutation_probability = 0.3;
    config.dropout = 0.4;
    config.mutation_method = ALLEL_FLIP;
    config.crossover_method = /*SINGLE_CUT*//*DOUBLE_CUT*/MASK;
    config.parentingPoolSelection = ELITE/*TOURNAMENT*/;
    config.veteranSelection = /*ROULETTE*/ /*RANKING*/ ELITE;
    /* config.veteranSelection = TOURNAMENT; */
    set_tournament_group_size_factor(0.1);
    set_tournament_determinism_factor(1); 
    config.chromosome_length = backpack.num_available;

    ga_bin_r_basic_startup_config_t startupConfig;
    startupConfig.initialPopulationGenerator = population_generator;
    binary_chromosome solution = ga_bin_r_basic_extreme(_fitness, config, startupConfig);

    // static unsigned int[backpack.num_available];
    //printf("DEBUG FROM bpack_optimizer | FITNESS = %lf\n", *(double*)_fitness((void*)solution));
    return (unsigned char*)solution;
}