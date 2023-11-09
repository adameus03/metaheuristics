#include "varlogger.h"
#include <stdio.h>
//#include <stdlib.h>

/*void init_varlogger(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint) {
    
}*/

static varlogger_config_t logger_config;
void init_varlogger(const varlogger_config_t config) {
    logger_config = config;
} 

void oneline_print_stat(FILE* logFilePtr, batch_stat_t batchStat/*, const char* variable*/) {
    fprintf(logFilePtr, "%d %d %d %d %d %d %d %d %d %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f\n",
            //variable,
            batchStat.result_average.saStat.solution_improved_times, batchStat.result_minima.saStat.solution_improved_times, batchStat.result_maxima.saStat.solution_improved_times,
            batchStat.result_average.saStat.solution_worsened_times, batchStat.result_minima.saStat.solution_worsened_times, batchStat.result_maxima.saStat.solution_worsened_times,
            batchStat.result_average.saStat.best_solution_updated_times, batchStat.result_minima.saStat.best_solution_updated_times, batchStat.result_maxima.saStat.best_solution_updated_times,
            batchStat.result_average.solution_x, batchStat.result_minima.solution_x, batchStat.result_maxima.solution_x,
            batchStat.result_average.solution_y, batchStat.result_minima.solution_y, batchStat.result_maxima.solution_y,
            batchStat.result_average.extreme_val, batchStat.result_minima.extreme_val, batchStat.result_maxima.extreme_val);
}
/*void print_stat_f(FILE* logFilePtr, batch_stat_t batchStat, const double variable) {
    fprintf(logFilePtr, "%4.5f %4.1f %d %d %4.1f %d %d %4.1f %d %d %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f %4.5f\n",
            variable,
            batchStat.result_average.saStat.solution_improved_times, batchStat.result_minima.saStat.solution_improved_times, batchStat.result_maxima.saStat.solution_improved_times,
            batchStat.result_average.saStat.solution_worsened_times, batchStat.result_minima.saStat.solution_worsened_times, batchStat.result_maxima.saStat.solution_worsened_times,
            batchStat.result_average.saStat.best_solution_updated_times, batchStat.result_minima.saStat.best_solution_updated_times, batchStat.result_maxima.saStat.best_solution_updated_times,
            batchStat.result_average.solution_x, batchStat.result_minima.solution_x, batchStat.result_maxima.solution_x,
            batchStat.result_average.solution_y, batchStat.result_minima.solution_y, batchStat.result_maxima.solution_y,
            batchStat.result_average.extreme_val, batchStat.result_minima.extreme_val, batchStat.result_maxima.extreme_val);
}*/

void varlog_iters_per_epoch(const unsigned int minItersInclusive, const unsigned int maxItersExclusive, 
                            const unsigned int stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");

    sa_config_t saConfig = logger_config.saConfig;
    saConfig.epoch_iters = minItersInclusive;
    for (; saConfig.epoch_iters < maxItersExclusive; saConfig.epoch_iters += stepSize) {
        static batch_stat_t batchStat;
        //batchStat.batch_name ///HHHHHHEEEERRRREEEEE
        batchStat = batch(logger_config.logger_name, 10, logger_config.f, saConfig, logger_config.constraint, QUIET);
        fprintf(logFilePtr, "%d ", saConfig.epoch_iters);
        oneline_print_stat(logFilePtr, batchStat/*, saConfig.epoch_iters*/); ///HHHHHHHH<<<<<
    }
    fclose(logFilePtr);
}
void varlog_cooldown(const double minCooldownInclusive, const double maxCooldownExclusive, 
                     const double stepMultiplier, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");

    sa_config_t saConfig = logger_config.saConfig;
    saConfig.cooldown = minCooldownInclusive;
    for (; saConfig.cooldown < maxCooldownExclusive; saConfig.cooldown *= stepMultiplier) {
        static batch_stat_t batchStat;
        batchStat = batch(logger_config.logger_name, 10, logger_config.f, saConfig, logger_config.constraint, QUIET);
        fprintf(logFilePtr, "%4.5f ", saConfig.cooldown);
        oneline_print_stat(logFilePtr, batchStat);
    }
    fclose(logFilePtr);
}
void varlog_boltzmann_k(const double minKInclusive, const double maxKExclusive, 
                        const double stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");

    sa_config_t saConfig = logger_config.saConfig;
    saConfig.boltzmann_k = minKInclusive;
    for (; saConfig.boltzmann_k < maxKExclusive; saConfig.boltzmann_k += stepSize) {
        static batch_stat_t batchStat;
        batchStat = batch(logger_config.logger_name, 10, logger_config.f, saConfig, logger_config.constraint, QUIET);
        fprintf(logFilePtr, "%4.5f ", saConfig.boltzmann_k);
        oneline_print_stat(logFilePtr, batchStat);
    }
    fclose(logFilePtr);
}

void varlog_temperature(const double minTemperatureInclusive, const double maxTemperatureExclusive,
                        const double stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");

    sa_config_t saConfig = logger_config.saConfig;
    saConfig.temperature = minTemperatureInclusive;
    for (; saConfig.temperature < maxTemperatureExclusive; saConfig.temperature += stepSize) {
        static batch_stat_t batchStat;
        batchStat = batch(logger_config.logger_name, 10, logger_config.f, saConfig, logger_config.constraint, QUIET);
        fprintf(logFilePtr, "%4.5f ", saConfig.temperature);
        oneline_print_stat(logFilePtr, batchStat);
    }
    fclose(logFilePtr);
}

void comparison_memmode(const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    batch_stat_t batchStat;
    sa_config_t saConfig = logger_config.saConfig;
    
    saConfig.mem_mode = NO_MEMORY;
    batchStat = batch(logger_config.logger_name, 100, logger_config.f, saConfig, logger_config.constraint, QUIET);
    fprintf(logFilePtr, "%s ", "NO_MEMORY");
    oneline_print_stat(logFilePtr, batchStat);

    saConfig.mem_mode = WITH_MEMORY;
    batchStat = batch(logger_config.logger_name, 100, logger_config.f, saConfig, logger_config.constraint, QUIET);
    fprintf(logFilePtr, "%s ", "WITH_MEMORY");
    oneline_print_stat(logFilePtr, batchStat);

    fclose(logFilePtr);
}