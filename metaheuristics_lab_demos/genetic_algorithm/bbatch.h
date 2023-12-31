#include "bpack_optimizer.h"

typedef enum { VERBOSE, QUIET } BBATCH_VERBOSITY_MODE;

typedef struct {
    double bpack_content_fitness;
    unsigned int bpack_content_weight;
} bbatch_result_t;

/* typedef struct {
    double* frequencies;
    unsigned int num_genes;
} gene_spectrum; */

typedef struct {
    const char* batch_name;
    bbatch_result_t result_minima;
    bbatch_result_t result_maxima;
    bbatch_result_t result_average;
    // gene_spectrum allel_stat;
} bbatch_stat_t;

bbatch_stat_t bbatch(const char* batch_name, const unsigned long int runTimes,
           const backpack_t backpack, const bpack_optimizer_config_t bpoConfig, const BBATCH_VERBOSITY_MODE verbosity);

void print_stat(const bbatch_stat_t stats);