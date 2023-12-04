#include "aco.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

/* Type used to index node connectors */
typedef unsigned int aco_edge_numeric_t;
/* Type used to measure pheromone levels */
typedef double aco_pheromone_t;

#define ACO_MAX_EDGES ACO_MAX_NODES * ACO_MAX_NODES

/* Representation of a route passing through nodes */
typedef struct {
    /* Indices of the nodes present in the route so far */
    aco_route_node_numeric_t node_indices[ACO_MAX_ROUTE_LENGTH];
    /* Number of nodes present in the route so far */
    aco_route_node_numeric_t num_nodes;
} aco_route_t;

/* Container for parameter set assigned to each edge / node connector */
typedef struct {
    aco_pheromone_t pheromones[ACO_MAX_EDGES];
    aco_measure_t metric_values[ACO_MAX_EDGES];
} aco_edge_data_t;

typedef aco_edge_data_t* aco_edge_data_ptr_t;

aco_pheromone_t get_pheromone_level(const aco_node_numeric_t index_1, 
                                    const aco_node_numeric_t index_2, 
                                    const aco_edge_data_ptr_t reference) {
    return reference->pheromones[index_1 * ACO_MAX_EDGES + index_2];
}

void set_pheromone_level(const aco_node_numeric_t index_1, 
                         const aco_node_numeric_t index_2, 
                         const aco_pheromone_t level,
                         const aco_edge_data_ptr_t reference) {
    reference->pheromones[index_1 * ACO_MAX_EDGES + index_2] = level;
}

aco_measure_t get_metric_value(const aco_node_numeric_t index_1, 
                               const aco_node_numeric_t index_2, 
                               const aco_edge_data_ptr_t reference) {
    return reference->metric_values[index_1 * ACO_MAX_EDGES + index_2];
}

void set_metric_value(const aco_node_numeric_t index_1, 
                      const aco_node_numeric_t index_2, 
                      const aco_pheromone_t value,
                      const aco_edge_data_ptr_t reference) {
    reference->pheromones[index_1 * ACO_MAX_EDGES + index_2] = value;
}

/**
 * @brief Make sure that the random number generator is initialized
*/
void _aco_random_guard() {
    static unsigned char guard = 0x1;
    if (guard) {
        guard = 0x0;
        srand((unsigned int)time(NULL));
    }
}

/**
 * @brief Generate random floating-point number
 * @returns Floating-point number in range [0; 1)
*/
aco_randomness_t get_random() {
    return (aco_randomness_t)(((double)rand())/(double)RAND_MAX);
}

/**
 * @brief Initialize pheromones and calculate metric values
 * @param data Edges data structure which stores the data buffers
*/
void init_edge_data(const aco_edge_data_ptr_t data, 
                    const aco_node_array_ptr_t nodes,
                    const acoPairScalarFunc node_metric) {
    aco_measure_t metric_value;
    for (aco_edge_numeric_t i = 0; i < nodes->size; i++) {
        for (aco_edge_numeric_t j = 0; j < nodes->size; j++) {
            metric_value = node_metric(nodes->buffer[i], nodes->buffer[j]);
            set_pheromone_level(i, j, 1, data);
            set_metric_value(i, j, metric_value, data);
        }
    }
}

/**
 * @brief Optimize the route between abstract nodes using Ant Colony Optimization (ACO)
 * @param nodes An array of abstract nodes
 * @param config Configuration structure instance for the algorithm
 * @returns The ordering of nodes in an optimal route
*/
aco_node_ordering_t aco_optimize_route(const aco_node_array_ptr_t nodes, 
                                       const aco_config_ptr_t config) {
    _aco_random_guard();
    aco_route_t route;
    aco_edge_data_t edgeData;

    init_edge_data(&edgeData, nodes, config->node_metric);
    
    aco_randomness_t r;
    for (aco_iteration_t i = 0; i < config->num_iters; i++) {
        for (aco_ant_numeric_t j = 0; j < config->num_ants; j++) {
            r = get_random();
            if (r < config->ant_randomness_factor) {
                
            }
        }
    }
}