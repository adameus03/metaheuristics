#include "abatch.h"
typedef struct {
    char* logger_name;
    char* path;
    aco_r2_cart_euc_config_ptr_t ar2ceConfig;
} avarlogger_config_t;

typedef avarlogger_config_t* avarlogger_config_ptr_t;


/**
 * @brief Call this method before using other functions 
 * @param config Input data configuration
*/
void init_avarlogger(const avarlogger_config_ptr_t config);

void varlog_metric_weight(const aco_weight_t minInclusive, const aco_weight_t maxExclusive,
                    const aco_weight_t stepSize, const char* logFilePath);

void varlog_pheromone_weight(const aco_weight_t minInclusive, const aco_weight_t maxExclusive,
                    const aco_weight_t stepSize, const char* logFilePath);

void varlog_num_ants(const aco_ant_numeric_t minInclusive, const aco_ant_numeric_t maxExclusive,
                    const aco_ant_numeric_t stepSize, const char* logFilePath);

void varlog_ant_randomness_factor(const aco_randomness_t minInclusive, const aco_randomness_t maxExclusive,
                    const aco_randomness_t stepSize, const char* logFilePath);

void varlog_num_iters(const aco_iteration_t minInclusive, const aco_iteration_t maxExclusive,
                    const aco_iteration_t stepSize, const char* logFilePath);

void varlog_evaporation_factor(const aco_evaporation_t minInclusive, const aco_evaporation_t maxExclusive,
                    const aco_evaporation_t stepSize, const char* logFilePath);