#include <math.h>
#include <stdlib.h>
#include "ga_bin_r_basic.h"

gaBinRBasicGenerator _initial_population_generator(const gaBinRBasicGenerator* generator) {
    static gaBinRBasicGenerator _generator;
    if (generator) {
        _generator = *generator;
    }
    return _generator;
}

ga_population_t* domain_generator() {
    static ga_population_t population;
    gaBinRBasicGenerator generator = _initial_population_generator(NULL);
    ga_bin_r_basic_population_t* binPopulation;
    binPopulation = generator();
    
    population.size = binPopulation->size;
    // population.members = (void**)&(binPopulation->members);
    population.members = (void**)(binPopulation->members);
    return &population;
}

CMP_RESULT comparer(const void* ra, const void* rb) {
    return (*(double*)rb) > *((double*)ra);
}

double metric(const void* ra, const void* rb) {
    return fabs(*(double*)(rb) - *(double*)(ra));
}

unsigned int _chromosome_length(const unsigned int* chromo_length) {
    static unsigned int _chromo_length = 0;
    if (chromo_length) {
        _chromo_length = *chromo_length;
    }
    return _chromo_length;
}

void* reverse_mutate(const void* chromosome) {
    return NULL;
}

void* allel_flip_mutate(const void* chromosome) {
    unsigned int allel_pos = rand() % _chromosome_length(NULL);
    unsigned int allel_octet_pos = allel_pos >> 0x3;
    unsigned char allel_offset = allel_pos % 0x8;
    
    *(((unsigned char*)chromosome) + allel_octet_pos) ^= 0x80U >> allel_offset;
    return (void*)chromosome; // should be ok
}

unsigned char* _octets_storage(unsigned char* loc) {
    static unsigned char* _loc;
    if (loc) {
        _loc = loc;
    }
    return _loc;
} 

void* single_cut_crossover(const void* chromosomeA, const void* chromosomeB) {
    unsigned int allel_pos = rand() % _chromosome_length(NULL);
    unsigned int allel_octet_pos = allel_pos >> 0x3;
    unsigned char allel_offset = allel_pos % 0x8;

    // unsigned int crossover_octet_pos = rand() % (_chromosome_length(NULL) >> 0x3);
    unsigned char* ucca = (unsigned char*)chromosomeA;
    unsigned char* uccb = (unsigned char*)chromosomeB;
    
    unsigned int num_octets = (_chromosome_length(NULL) >> 0x3) + (_chromosome_length(NULL) % 0x8 != 0x0); //move to macro?
    // static unsigned char ucr[num_octets] = {0}; //result //if problems, move to static in blob + macro? ; Done 
    unsigned char* ucr = _octets_storage(NULL);
    
    for (unsigned int i = 0; i < allel_octet_pos; i++) {
        *(ucr + i) = *(ucca + i);
    }
    for (unsigned int i = allel_octet_pos + 1; i < num_octets; i++) {
        unsigned char v = *(uccb + i);
        // *(ucr + i) = *(uccb + i);
        *(ucr + i) = v;
    }

    // *(ucr + allel_octet_pos) = 0x0;
    unsigned char msb_mask = 0xffU << (0x7U - allel_offset);
    unsigned char lsb_mask = 0xffU >> allel_offset;
    *(ucr + allel_octet_pos) = msb_mask & *(ucca + allel_octet_pos);
    *(ucr + allel_octet_pos) |= lsb_mask & *(uccb + allel_octet_pos);
    
    return (void*)ucr;
}

void* double_cut_crossover(const void* chromosomeA, const void* chromosomeB) {
    return NULL;
}

void* mask_crossover(const void* chromosomeA, const void* chromosomeB) {
    return NULL;
}

binary_chromosome ga_bin_r_basic_extreme(const gaFunc f, 
                                           const ga_bin_r_basic_config_t config, 
                                           const ga_bin_r_basic_startup_config_t startupConfig) {
    _initial_population_generator(&startupConfig.initialPopulationGenerator);
    _chromosome_length(&config.chromosome_length);

    ga_config_t gaConfig;
    unsigned int epochs;
    gaConfig.mutation_probability = config.mutation_probability;
    gaConfig.dropout = config.dropout;
    gaConfig.parentingPoolSelection = config.parentingPoolSelection;
    gaConfig.veteranSelection = config.veteranSelection;

    //gaConfig.mutate = co
    //gaConfig.crossover

    switch (config.mutation_method) {
        case REVERSE:
            gaConfig.mutate = reverse_mutate;
            break;
        default:
            gaConfig.mutate = allel_flip_mutate;
            break;
    }

    switch (config.crossover_method) {
        case DOUBLE_CUT:
            gaConfig.crossover = double_cut_crossover;
            break;
        case MASK:
            gaConfig.crossover = mask_crossover;
            break;
        default:
            gaConfig.crossover = single_cut_crossover;
            break;
    }


    ga_domain_config_t gaDomainConfig;
    gaDomainConfig.domainGenerator = domain_generator;

    ga_codomain_config_t gaCodomainConfig;
    gaCodomainConfig.comparer = comparer;
    gaCodomainConfig.metric = metric;
    
    // binary_chromosome_t solution;
    // solution.genes = (unsigned char*)ga_extreme(f, gaConfig, domainConfig, codomainConfig);
    // solution.num_genes = config.chromosome_length;

    // return solution;
    return (binary_chromosome)ga_extreme(f, gaConfig, gaDomainConfig, gaCodomainConfig);
}