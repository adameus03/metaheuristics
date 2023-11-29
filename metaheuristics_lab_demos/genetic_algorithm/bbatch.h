#include "../ga/ga_bin_r_basic.h"

typedef struct {
    double bpack_content_fitness;
    unsigned int bpack_content_weight;
} bbatch_result_t;

typedef struct {
    double* frequencies;
    unsigned int num_genes;
} gene_spectrum;

typedef struct {
    const char* batch_name;
    batch_result_t result_minima;
    batch_result_t result_maxima;
    batch_result_t result_average;
    gene_spectrum allel_stat;
} bbatch_stat_t;

batch_stat_t bbatch(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint, const BATCH_VERBOSITY_MODE verbose);

/* void print_stat(const batch_stat_t stats); */