#include "bbatch.h"
typedef struct {
    char* logger_name;
    backpack_t backpack;
    bpack_optimizer_config_t bpoConfig;
} bvarlogger_config_t;

/**
 * @brief Call this method before using other functions 
 * @param config Input data configuration
*/
void init_bvarlogger(const bvarlogger_config_t config);

void varlog_dropout(const double minDropoutInclusive, const double maxDropoutExclusive,
                    const double stepSize, const char* logFilePath);

void varlog_mutprob(const double minMutprobInclusive, const double maxMutprobExclusive,
                    const double stepSize, const char* logFilePath);

void varlog_popsize(const unsigned int minPopsize, const unsigned int maxPopsize,
                    const double stepSize, const char* logFilePath);