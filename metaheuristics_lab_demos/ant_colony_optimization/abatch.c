#include "abatch.h"
#include <stdio.h>

abatch_stat_t abatch(const char* batch_name, const unsigned long int runTimes,
           const char* path, const aco_r2_cart_euc_config_ptr_t ar2ceConfig, const ABATCH_VERBOSITY_MODE verbosity) {
    abatch_result_t minStat;
    abatch_result_t maxStat;
    abatch_result_t sumStat;

    minStat.route_length = __DBL_MAX__;
    
    maxStat.route_length = 0;

    sumStat.route_length = 0;

    printf("\nRunning TSP batch \"%s\" of size %d with params: heurw=%4.2f, pherw=%4.2f, ants=%d, r=%4.2f, iters=%d, eva=%4.2f\n\n",
                batch_name, runTimes, ar2ceConfig->metric_weight, ar2ceConfig->pheromone_weight, 
                ar2ceConfig->num_ants, ar2ceConfig->ant_randomness_factor, ar2ceConfig->num_iters, 
                ar2ceConfig->evaporation_factor);

    for (unsigned long int i = 0; i < runTimes; i++) {
        aco_result_t result = find_best_route(path, ar2ceConfig);
        if (verbosity == QUIET) {
            printf("\33[2K\r(%d / %d) Sampling ACO executions...", i + 1, runTimes);
        }
        if (verbosity == VERBOSE) {
            printf("(%d / %d) ROUTE_LEN=%4.2f\n", i + 1, runTimes, result.route_length);
        }

        sumStat.route_length += result.route_length;

        if (result.route_length > maxStat.route_length) {
            maxStat.route_length = result.route_length;
        }
        if (result.route_length < minStat.route_length) {
            minStat.route_length = result.route_length;
        }
    }

    if (verbosity == QUIET) {
        printf("\n");
    }

    abatch_stat_t stats;
    stats.batch_name = batch_name;
    stats.result_average.route_length = sumStat.route_length / runTimes;

    stats.result_minima = minStat;
    stats.result_maxima = maxStat;

    return stats;
    
}

void print_stat(const abatch_stat_t stats) {
    printf("\n=========== %s final ant epoch obtained ===========\n", stats.batch_name);
    printf("Route length: avg %4.2f min %4.2f max %4.2f\n", stats.result_average.route_length, stats.result_minima.route_length, stats.result_maxima.route_length);
}