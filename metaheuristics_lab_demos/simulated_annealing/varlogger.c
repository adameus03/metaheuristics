#include "varlogger.h"

/*void init_varlogger(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint) {
    
}*/

static varlogger_config_t logger_config;
void init_varlogger(const varlogger_config_t config) {
    logger_config = config;
} 


void varlog_iters_per_epoch(const unsigned int minIters, const unsigned int maxIters){
    
}
void varlog_cooldown(const double minCooldown, const double maxCooldown) {

}
void varlog_boltzmann_k(const double minK, const double maxK) {

}
void comparison_memmode() {

}