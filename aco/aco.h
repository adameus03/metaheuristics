#include "config.h"
/* Abstract node type */
typedef void* aco_node_t;
/* Type used to index the nodes */
typedef unsigned int aco_node_numeric_t;
/* Type used to index the route nodes */
typedef unsigned int aco_route_node_numeric_t;
/* Type used to index the ants of the ant colony */
typedef unsigned int aco_ant_numeric_t;
/* Type used to express the distance between the nodes */
typedef double aco_measure_t;
/* Type used to express control parameter weight for the ACO */
typedef double aco_weight_t;
/* Type used to express probabilities */
typedef double aco_randomness_t;
/* Type used to index ACO internal iterations */
typedef unsigned int aco_iteration_t;
/* Type used to express the evaporation factor for ACO pheromones */
typedef double aco_evaporation_t;
/* Type used to denote a "(node, node) --> measure" function (typically for measuring distance between nodes)*/
typedef aco_measure_t (*acoPairScalarFunc)(const aco_node_t, 
                                           const aco_node_t);
/*
    Array structure for storing a pointer to a nodes buffer
*/
typedef struct {
    aco_node_t* buffer;
    aco_node_numeric_t size;
} aco_node_array_t;

typedef aco_node_array_t* aco_node_array_ptr_t;

/*
    Array structure for storing a pointer to the nodes ordering which should be an array of node indices 

typedef struct {
    //aco_node_numeric_t* buffer;
    aco_node_numeric_t buffer[ACO_MAX_ROUTE_LENGTH];
    aco_route_node_numeric_t size;
} aco_node_ordering_t;*/

/* Representation of a route passing through nodes */
typedef struct {
    /* Indices of the nodes present in the route so far */
    aco_node_numeric_t node_indices[ACO_MAX_ROUTE_LENGTH];
    /* Number of nodes present in the route so far */
    aco_route_node_numeric_t num_nodes;
} aco_route_t;

typedef aco_route_t* aco_route_ptr_t;
/*
    Configuration structure for the Ant Colony Optimization (ACO) algorithm.
    Contains the route optimization control parameters and the metric for calculating distance between the nodes.
*/
typedef struct {
    /* Metric function used to measure distance between the nodes */
    acoPairScalarFunc node_metric;
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
} aco_config_t;
typedef aco_config_t* aco_config_ptr_t;
/**
 * @brief Optimize the route between abstract nodes using Ant Colony Optimization (ACO)
 * @param nodes An array of abstract nodes
 * @param config Configuration structure instance for the algorithm
 * @returns The ordering of nodes in an optimal route
*/
aco_route_t/*aco_node_ordering_t*/ aco_optimize_route(const aco_node_array_ptr_t nodes, 
                                       const aco_config_ptr_t config);