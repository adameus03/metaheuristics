#include <stdlib.h>
#include <stdio.h> //debug
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

unsigned int _chromo_octet_num(const unsigned int* octet_num) {
    static unsigned int _octet_num;
    if (octet_num) {
        _octet_num = *octet_num;
    }
    return _octet_num;
}

unsigned int _population_initsize(const unsigned int* initsize) {
    static unsigned int _initsize;
    if (initsize) {
        _initsize = *initsize;
    }
    return _initsize;
}

unsigned int _population_maxsize(const unsigned int* maxsize) {
    static unsigned int _maxsize;
    if (maxsize) {
        _maxsize = *maxsize;
    }
    return _maxsize;
}

binary_chromosome* _population(binary_chromosome* pop) {
    static binary_chromosome* _pop;
    if (pop) {
        _pop = pop;
    }
    return _pop;
}

#define population_buffer_setup(size) ({ \
    static binary_chromosome _pop_members[size]; \
    _population(_pop_members); \
})

ga_bin_r_basic_population_t* population_generator() {
    static ga_bin_r_basic_population_t population;
    population.size = _population_initsize(NULL);

    static binary_chromosome* _members;
    _members = _population(NULL);

    unsigned int chromo_octet_num = _chromo_octet_num(NULL);
    unsigned int pop_initsize = _population_initsize(NULL);
    unsigned char* genes = _genes_storage(NULL);

    for (unsigned int i = 0; i < pop_initsize * chromo_octet_num; i++) {
        genes[i] = (unsigned char)(rand() % 0x100);
        genes[i] &= (unsigned char)(rand() % 0x100);
    }
    for (unsigned int i = 0; i < population.size; i++) {
        _members[i] = genes + chromo_octet_num * i;
    }
    population.members = _members;
    return &population;
}

#define bpack_optimizer_init(popmax, chromo_octets_num) ({ \
    static unsigned int ___chromo_octet_num; \
    static unsigned int ___population_maxsize; \
    ___chromo_octet_num = chromo_octets_num; \
    ___population_maxsize = popmax; \
    ga_bin_set_octet_num(chromo_octets_num); \
    ga_bin_r_init(popmax); \
    _chromo_octet_num(&___chromo_octet_num); \
    _population_maxsize(&___population_maxsize); \
    population_buffer_setup(popmax); \
    genes_loc( popmax * chromo_octets_num ); \
})

void _initialization_guard() {
    static unsigned char guard_val = 0x0;
    if (guard_val == 0x0) {
        bpack_optimizer_init(200, 4);
        guard_val = 0x1;
    }
}

unsigned char* optimized_backpack_composition_mask(const backpack_t backpack, const bpack_optimizer_config_t config) {
    __backpack(&backpack);
    _initialization_guard();
    if (config.tournamentGroupSizePtr) {
        set_tournament_group_size_factor(*(config.tournamentGroupSizePtr));
    }
    if (config.tournamentDeterminismFactorPtr) {
        set_tournament_determinism_factor(*(config.tournamentDeterminismFactorPtr));
    }
    static unsigned int popInitSize;
    popInitSize = config.gaPopulationSize;
    _population_initsize(&popInitSize);
    ga_bin_r_basic_startup_config_t startupConfig;
    startupConfig.initialPopulationGenerator = population_generator;
    binary_chromosome solution = ga_bin_r_basic_extreme(_fitness, config.gbrConfig, startupConfig);

    return (unsigned char*)solution;
}