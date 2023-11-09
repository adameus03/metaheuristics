#include "../sa/sa_r2_r_basic.h"

typedef enum { VERBOSE, QUIET } BATCH_VERBOSITY_MODE;

typedef struct {
    sa_stat_t saStat;
    double solution_x;
    double solution_y;
    double extreme_val;
} batch_result_t;

typedef struct {
    const char* batch_name;
    batch_result_t result_minima;
    batch_result_t result_maxima;
    batch_result_t result_average;
    
} batch_stat_t;

batch_stat_t batch(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint, const BATCH_VERBOSITY_MODE verbose);

void print_stat(const batch_stat_t stats);