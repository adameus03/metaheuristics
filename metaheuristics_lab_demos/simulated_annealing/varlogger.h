#include "../sa/sa_r2_r_basic.h"

typedef struct {
    char* logger_name;
    saFunc f;
    sa_config_t saConfig;
    r2_cart_rect_constraint_t constraint;

} varlogger_config_t;

void init_varlogger(const varlogger_config_t config);

void varlog_iters_per_epoch(const unsigned int minIters, const unsigned int maxIters);
void varlog_cooldown(const double minCooldown, const double maxCooldown);
void varlog_boltzmann_k(const double minK, const double maxK);
void comparison_memmode();
/* void comparison_inbound_constraining(); */