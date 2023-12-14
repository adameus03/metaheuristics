#include "optimizer_fio.h"

typedef enum { VERBOSE, QUIET } ABATCH_VERBOSITY_MODE;

typedef struct {
    aco_measure_t route_length;
} abatch_result_t;

/* typedef struct {
    double* frequencies;
    unsigned int num_genes;
} gene_spectrum; */

typedef struct {
    const char* batch_name;
    abatch_result_t result_minima;
    abatch_result_t result_maxima;
    abatch_result_t result_average;
    // gene_spectrum allel_stat;
} abatch_stat_t;

abatch_stat_t abatch(const char* batch_name, const unsigned long int runTimes,
           const char* path, const aco_r2_cart_euc_config_ptr_t ar2ceConfig, const ABATCH_VERBOSITY_MODE verbosity);

void print_stat(const abatch_stat_t stats);