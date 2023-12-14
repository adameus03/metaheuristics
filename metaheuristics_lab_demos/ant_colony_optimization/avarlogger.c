#include "avarlogger.h"
#include <stdio.h>

static avarlogger_config_ptr_t logger_config;
void init_avarlogger(const avarlogger_config_ptr_t config) {
    logger_config = config;
}

void oneline_print_stat(FILE* logFilePtr, abatch_stat_t abatchStat/*, const char* variable*/) {
    fprintf(logFilePtr, "%4.2f %4.2f %4.2f\n",
            //variable,
            abatchStat.result_average.route_length, abatchStat.result_minima.route_length, abatchStat.result_maxima.route_length
    );
}

void varlog_metric_weight(const aco_weight_t minInclusive, const aco_weight_t maxExclusive,
    const aco_weight_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.metric_weight = minInclusive;
    for (; ar2ceConfig.metric_weight < maxExclusive; ar2ceConfig.metric_weight += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%4.2f ", ar2ceConfig.metric_weight);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);
}

void varlog_pheromone_weight(const aco_weight_t minInclusive, const aco_weight_t maxExclusive,
                    const aco_weight_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.pheromone_weight = minInclusive;
    for (; ar2ceConfig.pheromone_weight < maxExclusive; ar2ceConfig.pheromone_weight += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%4.2f ", ar2ceConfig.pheromone_weight);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);
}

void varlog_num_ants(const aco_ant_numeric_t minInclusive, const aco_ant_numeric_t maxExclusive,
                    const aco_ant_numeric_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.num_ants = minInclusive;
    for (; ar2ceConfig.num_ants < maxExclusive; ar2ceConfig.num_ants += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%d ", ar2ceConfig.num_ants);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);
}

void varlog_ant_randomness_factor(const aco_randomness_t minInclusive, const aco_randomness_t maxExclusive,
                    const aco_randomness_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.ant_randomness_factor = minInclusive;
    for (; ar2ceConfig.ant_randomness_factor < maxExclusive; ar2ceConfig.ant_randomness_factor += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%4.2f ", ar2ceConfig.ant_randomness_factor);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);
}

void varlog_num_iters(const aco_iteration_t minInclusive, const aco_iteration_t maxExclusive,
                    const aco_iteration_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.num_iters = minInclusive;
    for (; ar2ceConfig.num_iters < maxExclusive; ar2ceConfig.num_iters += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%d ", ar2ceConfig.num_iters);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);

}
void varlog_evaporation_factor(const aco_evaporation_t minInclusive, const aco_evaporation_t maxExclusive,
                    const aco_evaporation_t stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    aco_r2_cart_euc_config_t ar2ceConfig = *(logger_config->ar2ceConfig);
    ar2ceConfig.evaporation_factor = minInclusive;
    for (; ar2ceConfig.evaporation_factor < maxExclusive; ar2ceConfig.evaporation_factor += stepSize) {
        static abatch_stat_t abatchStat;
        abatchStat = abatch(logger_config->logger_name, 10, logger_config->path, &ar2ceConfig, QUIET);
        fprintf(logFilePtr, "%4.2f ", ar2ceConfig.evaporation_factor);
        oneline_print_stat(logFilePtr, abatchStat);
    }
    fclose(logFilePtr);                        
}