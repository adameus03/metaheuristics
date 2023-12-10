#include "optimizer_fio.h"
#include <stdio.h>

aco_result_t find_best_route(const char* data_file_path, const aco_r2_cart_euc_config_ptr_t config) {
    //aco_node_array_t nodes = fio_get_points(data_file_path).node_array;
    //aco_node_array_t nodes;
    fio_out_nodes_t fioOutput;
    fio_get_points(data_file_path, &fioOutput);
    aco_node_array_t nodes = fioOutput.node_array;
    
    config->num_nodes_to_visit = nodes.size;
    //optimizer_result_t result;
    //result.route = aco_r2_cart_euc_optimize_route(&nodes, config);
    //result.route_length = aco_route_length(&result.route, nodes, config->)
    //return result;
    return aco_r2_cart_euc_optimize_route(&nodes, config);
}

void print_route(const aco_route_t route) {
    for (aco_route_node_numeric_t i = 0; i < route.num_nodes; i++) {
        printf("%u ", route.node_indices[i]);
    }
}