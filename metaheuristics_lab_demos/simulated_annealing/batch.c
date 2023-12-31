#include "batch.h"
#include <stdio.h>



batch_stat_t batch(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint, const BATCH_VERBOSITY_MODE verbosity) {
    batch_result_t minStat;
    batch_result_t maxStat;
    batch_result_t sumStat;

    minStat.saStat.solution_improved_times = __UINT64_MAX__;
    minStat.saStat.solution_worsened_times = __UINT64_MAX__;
    minStat.saStat.best_solution_updated_times = __UINT64_MAX__;
    minStat.solution_x = __DBL_MAX__;
    minStat.solution_y = __DBL_MAX__;
    minStat.extreme_val = __DBL_MAX__;

    maxStat.saStat.solution_improved_times = 0;
    maxStat.saStat.solution_worsened_times = 0;
    maxStat.saStat.best_solution_updated_times = 0;
    maxStat.solution_x = -__DBL_MAX__;
    maxStat.solution_y = -__DBL_MAX__;
    maxStat.extreme_val = -__DBL_MAX__;

    sumStat.saStat.solution_improved_times = 0;
    sumStat.saStat.solution_worsened_times = 0;
    sumStat.saStat.best_solution_updated_times = 0;
    sumStat.solution_x = 0;
    sumStat.solution_y = 0;
    sumStat.extreme_val = 0;

    printf("\nRunning batch \"%s\" of size %d with params: T=%4.2f, k=%4.2f, a=%4.5f, epochs=%d, iters=%d, memory=%s, mode=%s\n\n", batch_name, runTimes, config.temperature, config.boltzmann_k, config.cooldown, config.epochs, config.epoch_iters, config.mem_mode==WITH_MEMORY?"yes":"no", config.emode==MAX?"max":"min");

    for (unsigned long int i = 0; i < runTimes; i++) {
        static r2_cart_t solution;
        static double extremeVal;
        static sa_stat_t solStat;

        solution = sa_r2_r_basic_extreme(f, config, constraint);
        extremeVal = *(double*)f(&solution);
        solStat = get_sa_stat();

        if (verbosity == QUIET) {
            printf("\33[2K\r(%d / %d) Annealing...", i + 1, runTimes);
            //fflush(stdout);
        }
        
        if (verbosity == VERBOSE) {
            printf("(%d / %d) X=%4.5f, Y=%4.5f, Fmax=%4.5f, SP=%d, SM=%d, SB=%d", i + 1, runTimes, solution.x, solution.y, extremeVal, solStat.solution_improved_times, solStat.solution_worsened_times, solStat.best_solution_updated_times);
            printf("\n");
        }

        sumStat.solution_x += solution.x;
        sumStat.solution_y += solution.y;
        sumStat.extreme_val += extremeVal;
        sumStat.saStat.solution_improved_times += solStat.solution_improved_times;
        sumStat.saStat.solution_worsened_times += solStat.solution_worsened_times;
        sumStat.saStat.best_solution_updated_times += solStat.best_solution_updated_times;

        if (solution.x > maxStat.solution_x) {
            maxStat.solution_x = solution.x;
        }
        if (solution.y > maxStat.solution_y) {
            maxStat.solution_y = solution.y;
        }
        if (extremeVal > maxStat.extreme_val) {
            maxStat.extreme_val = extremeVal;
        }
        if (solStat.solution_improved_times > maxStat.saStat.solution_improved_times) {
            maxStat.saStat.solution_improved_times = solStat.solution_improved_times;
        }
        if (solStat.solution_worsened_times > maxStat.saStat.solution_worsened_times) {
            maxStat.saStat.solution_worsened_times = solStat.solution_worsened_times;
        }
        if (solStat.best_solution_updated_times > maxStat.saStat.best_solution_updated_times) {
            maxStat.saStat.best_solution_updated_times = solStat.best_solution_updated_times;
        }

        if (solution.x < minStat.solution_x) {
            minStat.solution_x = solution.x;
        }
        if (solution.y < minStat.solution_y) {
            minStat.solution_y = solution.y;
        }
        if (extremeVal < minStat.extreme_val) {
            minStat.extreme_val = extremeVal;
        }
        if (solStat.solution_improved_times < minStat.saStat.solution_improved_times) {
            minStat.saStat.solution_improved_times = solStat.solution_improved_times;
        }
        if (solStat.solution_worsened_times < minStat.saStat.solution_worsened_times) {
            minStat.saStat.solution_worsened_times = solStat.solution_worsened_times;
        }
        if (solStat.best_solution_updated_times < minStat.saStat.best_solution_updated_times) {
            minStat.saStat.best_solution_updated_times = solStat.best_solution_updated_times;
        }
    }

    if (verbosity == QUIET) {
        printf("\n");
    }

    static batch_stat_t stats;
    
    stats.batch_name = batch_name;
    stats.result_average.saStat.solution_improved_times = sumStat.saStat.solution_improved_times/runTimes;
    stats.result_average.saStat.solution_worsened_times = sumStat.saStat.solution_worsened_times/runTimes;
    stats.result_average.saStat.best_solution_updated_times = sumStat.saStat.best_solution_updated_times/runTimes;
    stats.result_average.solution_x = ((double)sumStat.solution_x)/runTimes;
    stats.result_average.solution_y = ((double)sumStat.solution_y)/runTimes;
    stats.result_average.extreme_val = ((double)sumStat.extreme_val)/runTimes;

    stats.result_minima = minStat;
    stats.result_maxima = maxStat;

    return stats;

    /*printf("\n=========== %s global extreme obtained ===========\n", batch_name);
    printf("Solution improved times: avg %4.1f min %d max %d\n", ((double)sumStat.saStat.solution_improved_times)/runTimes, minStat.saStat.solution_improved_times, maxStat.saStat.solution_improved_times);
    printf("Solution worsened times: avg %4.1f min %d max %d\n", ((double)sumStat.saStat.solution_worsened_times)/runTimes, minStat.saStat.solution_worsened_times, maxStat.saStat.solution_worsened_times);
    printf("Best solution updated times: avg %4.1f min %d max %d\n", ((double)sumStat.saStat.best_solution_updated_times)/runTimes, minStat.saStat.best_solution_updated_times, maxStat.saStat.best_solution_updated_times);
    printf("Solution X: avg %4.5f min %4.5f max %4.5f\n", ((double)sumStat.solution_x)/runTimes, minStat.solution_x, maxStat.solution_x);
    printf("Solution Y: avg %4.5f min %4.5f max %4.5f\n", ((double)sumStat.solution_y)/runTimes, minStat.solution_y, maxStat.solution_y);
    printf("Extreme value: avg %4.5f min %4.5f max %4.5f\n\n", ((double)sumStat.extreme_val)/runTimes, minStat.extreme_val, maxStat.extreme_val);*/

}

void print_stat(const batch_stat_t stats) {
    printf("\n=========== %s global extreme obtained ===========\n", stats.batch_name);
    printf("Solution improved times: avg %d min %d max %d\n", stats.result_average.saStat.solution_improved_times, stats.result_minima.saStat.solution_improved_times, stats.result_maxima.saStat.solution_improved_times);
    printf("Solution worsened times: avg %d min %d max %d\n", stats.result_average.saStat.solution_worsened_times, stats.result_minima.saStat.solution_worsened_times, stats.result_maxima.saStat.solution_worsened_times);
    printf("Best solution updated times: avg %d min %d max %d\n", stats.result_average.saStat.best_solution_updated_times, stats.result_minima.saStat.best_solution_updated_times, stats.result_maxima.saStat.best_solution_updated_times);
    printf("Solution X: avg %4.5f min %4.5f max %4.5f\n", stats.result_average.solution_x, stats.result_minima.solution_x, stats.result_maxima.solution_x);
    printf("Solution Y: avg %4.5f min %4.5f max %4.5f\n", stats.result_average.solution_y, stats.result_minima.solution_y, stats.result_maxima.solution_y);
    printf("Extreme value: avg %4.5f min %4.5f max %4.5f\n\n", stats.result_average.extreme_val, stats.result_minima.extreme_val, stats.result_maxima.extreme_val);
}