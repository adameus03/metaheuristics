#include "fio.h"

/*typedef struct {
    aco_route_t route;
    aco_measure_t route_length;
} optimizer_result_t;*/

aco_result_t find_best_route(const char* data_file_path, const aco_r2_cart_euc_config_ptr_t config);

void print_route(const aco_route_t route);