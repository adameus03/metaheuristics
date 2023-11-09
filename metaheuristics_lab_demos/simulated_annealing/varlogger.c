#include "varlogger.h"

/*void init_varlogger(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint) {
    
}*/

static varlogger_config_t logger_config;
void init_varlogger(const varlogger_config_t config) {
    logger_config = config;
} 


