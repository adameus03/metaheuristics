#include "aco_r2_cart_euc.h"
#include <math.h>

typedef double coordinate_diff_t;

aco_measure_t r2_cart_euc_metric(const aco_node_t n1, const aco_node_t n2) {
    r2_cart_point_ptr_t p1 = (r2_cart_point_ptr_t)n1;
    r2_cart_point_ptr_t p2 = (r2_cart_point_ptr_t)n2;
    coordinate_diff_t diff_x = ((coordinate_diff_t)p2->x) - (coordinate_diff_t)p1->x; // unsigned??;
    coordinate_diff_t diff_y = ((coordinate_diff_t)p2->y) - (coordinate_diff_t)p1->y;
    return sqrt(diff_x * diff_x + diff_y * diff_y);
}

aco_route_t/*aco_node_ordering_t*/ aco_r2_cart_euc_optimize_route(const aco_node_array_ptr_t nodes,
                                                                  const aco_r2_cart_euc_config_ptr_t config) {
    aco_config_t acoConfig;
    acoConfig.node_metric = r2_cart_euc_metric;
    acoConfig.metric_weight = config->metric_weight;
    acoConfig.pheromone_weight = config->pheromone_weight;
    acoConfig.num_nodes_to_visit = config->num_nodes_to_visit;
    acoConfig.num_ants = config->num_ants;
    acoConfig.ant_randomness_factor = config->ant_randomness_factor;
    acoConfig.num_iters = config->num_iters;
    acoConfig.evaporation_factor = config->evaporation_factor;

    return aco_optimize_route(nodes, &acoConfig);
}