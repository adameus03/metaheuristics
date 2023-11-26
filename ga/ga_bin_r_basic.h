#include "ga.h"

typedef enum {
    SINGLE_CUT,
    DOUBLE_CUT,
    MASK
} CROSSOVER_METHOD;

typedef enum {
    ALLEL_FLIP,
    REVERSE
} MUTATION_METHOD;

/* typedef struct {
    unsigned char* genes;
    // unsigned int num_genes;
} binary_chromosome_t; */

typedef unsigned char* binary_chromosome;

typedef struct {
    unsigned int size;
    binary_chromosome* members;
} ga_bin_r_basic_population_t;

typedef ga_bin_r_basic_population_t* (*gaBinRBasicGenerator)();

typedef struct {
    gaBinRBasicGenerator initialPopulationGenerator;
} ga_bin_r_basic_startup_config_t;

typedef struct {
    unsigned int epochs;
    double mutation_probability; 
    double dropout;
    MUTATION_METHOD mutation_method;
    CROSSOVER_METHOD crossover_method;
    SELECTION_METHOD parentingPoolSelection;
    SELECTION_METHOD veteranSelection;
    unsigned int chromosome_length;
} ga_bin_r_basic_config_t;


unsigned char* _octets_storage(unsigned char* loc);

#define ga_bin_set_octet_num(oct_num) ({ \
    static unsigned char octets[oct_num]; \
    _octets_storage(octets); \
})

binary_chromosome ga_bin_r_basic_extreme(const gaFunc f, 
                                      const ga_bin_r_basic_config_t config, 
                                      const ga_bin_r_basic_startup_config_t startupConfig);

#define ga_bin_r_init(max_popsize) ({\
    ga_type(unsigned char*, double, max_popsize);\
})
