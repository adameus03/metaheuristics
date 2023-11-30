#include <stdio.h>
#include "bvarlogger.h"

static bvarlogger_config_t logger_config;
void init_bvarlogger(const bvarlogger_config_t config) {
    logger_config = config;
}

void oneline_print_stat(FILE* logFilePtr, bbatch_stat_t bbatchStat/*, const char* variable*/) {
    fprintf(logFilePtr, "%d %d %d %4.0f %4.0f %4.0f\n",
            //variable,
            bbatchStat.result_average.bpack_content_weight, bbatchStat.result_minima.bpack_content_weight, bbatchStat.result_maxima.bpack_content_weight,
            bbatchStat.result_average.bpack_content_fitness, bbatchStat.result_minima.bpack_content_fitness, bbatchStat.result_maxima.bpack_content_fitness
    );
}

void varlog_dropout(const double minDropoutInclusive, const double maxDropoutExclusive,
                    const double stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    bpack_optimizer_config_t bpoConfig = logger_config.bpoConfig;
    bpoConfig.gbrConfig.dropout = minDropoutInclusive;
    for (; bpoConfig.gbrConfig.dropout < maxDropoutExclusive; bpoConfig.gbrConfig.dropout += stepSize) {
        static bbatch_stat_t bbatchStat;
        bbatchStat = bbatch(logger_config.logger_name, 100, logger_config.backpack, bpoConfig, QUIET);
        fprintf(logFilePtr, "%4.5f ", bpoConfig.gbrConfig.dropout);
        oneline_print_stat(logFilePtr, bbatchStat);
    }
    fclose(logFilePtr);
}

void varlog_mutprob(const double minMutprobInclusive, const double maxMutprobExclusive,
                    const double stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    bpack_optimizer_config_t bpoConfig = logger_config.bpoConfig;
    bpoConfig.gbrConfig.mutation_probability = minMutprobInclusive;
    for (; bpoConfig.gbrConfig.mutation_probability < maxMutprobExclusive; bpoConfig.gbrConfig.mutation_probability += stepSize) {
        static bbatch_stat_t bbatchStat;
        bbatchStat = bbatch(logger_config.logger_name, 100, logger_config.backpack, bpoConfig, QUIET);
        fprintf(logFilePtr, "%4.5f ", bpoConfig.gbrConfig.mutation_probability);
        oneline_print_stat(logFilePtr, bbatchStat);
    }
    fclose(logFilePtr);
}

void varlog_popsize(const unsigned int minPopsizeInclusive, const unsigned int maxPopsizeExclusive,
                    const unsigned int stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    bpack_optimizer_config_t bpoConfig = logger_config.bpoConfig;
    bpoConfig.gaPopulationSize= minPopsizeInclusive;
    for (; bpoConfig.gaPopulationSize < maxPopsizeExclusive; bpoConfig.gaPopulationSize += stepSize) {
        static bbatch_stat_t bbatchStat;
        bbatchStat = bbatch(logger_config.logger_name, 100, logger_config.backpack, bpoConfig, QUIET);
        fprintf(logFilePtr, "%d ", bpoConfig.gaPopulationSize);
        oneline_print_stat(logFilePtr, bbatchStat);
    }
    fclose(logFilePtr);
}

void varlog_epochs(const unsigned int minEpochsInclusive, const unsigned int maxEpochsExclusive,
                    const unsigned int stepSize, const char* logFilePath) {
    FILE* logFilePtr = fopen(logFilePath, "w");
    bpack_optimizer_config_t bpoConfig = logger_config.bpoConfig;
    bpoConfig.gbrConfig.epochs = minEpochsInclusive;
    for (; bpoConfig.gbrConfig.epochs < maxEpochsExclusive; bpoConfig.gbrConfig.epochs += stepSize) {
        static bbatch_stat_t bbatchStat;
        bbatchStat = bbatch(logger_config.logger_name, 100, logger_config.backpack, bpoConfig, QUIET);
        fprintf(logFilePtr, "%d ", bpoConfig.gbrConfig.epochs);
        oneline_print_stat(logFilePtr, bbatchStat);
    }
    fclose(logFilePtr);
}