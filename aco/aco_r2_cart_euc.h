#include "aco.h"

typedef double coordinate_t;
typedef struct {
    coordinate_t x;
    coordinate_t y;
} r2_cart_point_t;

typedef r2_cart_point_t* r2_cart_point_ptr_t;
typedef r2_cart_point_t* r2_cart_point_buffer_t;
//typedef r2_cart_point_ptr_t* r2_cart_point_ptr_buffer_t;


typedef struct {
    r2_cart_point_t buffer[ACO_MAX_NODES];
} r2_cart_point_storage_t;

/*typedef struct {
    r2_cart_point_ptr_t ptr_buffer[ACO_MAX_NODES];
} r2_cart_point_ptr_storage_t;*/





typedef struct {
    /* Importance of metric in the ACO optimization process*/
    aco_weight_t metric_weight;
    /* Importance of ant pheromones in the ACO optimization process */
    aco_weight_t pheromone_weight;
    /* Number of nodes in the optimal route */
    aco_route_node_numeric_t num_nodes_to_visit;
    /* Number of ants used in the ACO optimization process */
    aco_ant_numeric_t num_ants;
    /* Probability that an ACO ant will choose to randomly move between nodes */
    aco_randomness_t ant_randomness_factor;
    /* Number of ACO iterations */
    aco_iteration_t num_iters;
    /** 
     * The ACO ant pheromones will evaporate according to this factor 
       @note The evaporation rate for pheromones given evaporation factor `v` is given by `rate = 1-v`)
    */
    aco_evaporation_t evaporation_factor;
} aco_r2_cart_euc_config_t;

typedef aco_r2_cart_euc_config_t* aco_r2_cart_euc_config_ptr_t;

aco_result_t/*aco_route_t*//*aco_node_ordering_t*/ aco_r2_cart_euc_optimize_route(const aco_node_array_ptr_t nodes,
                                                                  const aco_r2_cart_euc_config_ptr_t config);