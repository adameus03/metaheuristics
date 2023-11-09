//#include "../sa/sa_r2_r_basic.h"
#include "batch.h"

typedef struct {
    char* logger_name;
    saFunc f;
    sa_config_t saConfig;
    r2_cart_rect_constraint_t constraint;

} varlogger_config_t;

/**
 * @brief Call this method before using other functions 
 * @param config Input data configuration
*/
void init_varlogger(const varlogger_config_t config);

void varlog_iters_per_epoch(const unsigned int minItersInclusive, const unsigned int maxItersExclusive, 
                            const unsigned int stepSize, const char* logFilePath);

void varlog_cooldown(const double minCooldownInclusive, const double maxCooldownExclusive, 
                     const double stepMultiplier, const char* logFilePath);

void varlog_boltzmann_k(const double minKInclusive, const double maxKExclusive, 
                        const double stepSize, const char* logFilePath);

void varlog_temperature(const double minTemperatureInclusive, const double maxTemperatureExclusive,
                        const double stepSize, const char* logFilePath);
                        
void comparison_memmode(const char* logFilePath);
/* void comparison_inbound_constraining(); */