#include "aco.h"
#include "linked_list.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Type used to index node connectors */
typedef unsigned int aco_edge_numeric_t;
/* Type used to measure pheromone levels */
typedef double aco_pheromone_t;

#define ACO_MAX_EDGES ACO_MAX_NODES * ACO_MAX_NODES




/* Container for parameter set assigned to each edge / node connector */
typedef struct {
    aco_pheromone_t pheromones[ACO_MAX_EDGES];
    aco_pheromone_t pheromones_appendix[ACO_MAX_EDGES]; //neccessary, or the algorithm could be efficient without it? I.e. update pheromones after each ant...
    aco_measure_t metric_values[ACO_MAX_EDGES];
} aco_edge_data_t;

typedef aco_edge_data_t* aco_edge_data_ptr_t;

typedef struct {
    linked_list_node_t list_unvisited_node_buffer[ACO_MAX_NODES];
    aco_node_numeric_t list_unvisited_node_buffer_length;
} aco_exploration_data_t; //rename to exploration data? ; Done

typedef aco_exploration_data_t* aco_exploration_data_ptr_t;

typedef double aco_pherometric_t;

aco_pheromone_t get_pheromone_level(const aco_node_numeric_t index_1,
                                    const aco_node_numeric_t index_2, 
                                    const aco_edge_data_ptr_t reference) {
    return reference->pheromones[index_1 * ACO_MAX_NODES + index_2];
}

void set_pheromone_level(const aco_node_numeric_t index_1, 
                         const aco_node_numeric_t index_2, 
                         const aco_pheromone_t level,
                         const aco_edge_data_ptr_t reference) {
    reference->pheromones[index_1 * ACO_MAX_NODES + index_2] = level;
}

aco_pheromone_t get_pheromone_level_appendix(const aco_node_numeric_t index_1, 
                                    const aco_node_numeric_t index_2, 
                                    const aco_edge_data_ptr_t reference) {
    return reference->pheromones_appendix[index_1 * ACO_MAX_NODES + index_2];
}

void set_pheromone_level_appendix(const aco_node_numeric_t index_1, 
                         const aco_node_numeric_t index_2, 
                         const aco_pheromone_t level,
                         const aco_edge_data_ptr_t reference) {
    reference->pheromones_appendix[index_1 * ACO_MAX_NODES + index_2] = level;
}

aco_measure_t get_metric_value(const aco_node_numeric_t index_1, 
                               const aco_node_numeric_t index_2, 
                               const aco_edge_data_ptr_t reference) {
    return reference->metric_values[index_1 * ACO_MAX_NODES + index_2];
}

void set_metric_value(const aco_node_numeric_t index_1, 
                      const aco_node_numeric_t index_2, 
                      const aco_pheromone_t value,
                      const aco_edge_data_ptr_t reference) {
    reference->pheromones[index_1 * ACO_MAX_NODES + index_2] = value;
}

/**
 * @brief Inserts a node index into the end of the route
 * @param index In-buffer index of the node to be inserted
*/
void add_route_node_index(const aco_route_ptr_t route,
                          const aco_node_numeric_t index) {
    route->num_nodes++;
    aco_route_node_numeric_t route_last_index = route->num_nodes - 1;
    route->node_indices[route_last_index] = index;
}

aco_node_numeric_t get_last_route_node_index(const aco_route_ptr_t route) {
    aco_route_node_numeric_t route_last_index = route->num_nodes - 1;
    return route->node_indices[route_last_index];
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
aco_randomness_t get_floating_point_random() {
    return (aco_randomness_t)(((double)rand())/(double)RAND_MAX);
}

aco_node_numeric_t get_random_aco_node_index(const aco_node_numeric_t maxExclusive) {
    return rand() % maxExclusive;
}

aco_pherometric_t calc_pherometric(const aco_pheromone_t pheromone_level,
                                   const aco_measure_t metric_value,
                                   const aco_weight_t pheromone_weight,
                                   const aco_weight_t metric_weight) {
    return (aco_pherometric_t)(powl(pheromone_level, pheromone_weight) / powl(metric_value, metric_value));
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
            set_pheromone_level_appendix(i, j, 0, data);
            set_metric_value(i, j, metric_value, data);
        }
    }
}

//{{Ext: Try to optimize by constraining to local search at first}}
linked_list_node_ptr_t/*aco_node_numeric_t*/ aco_ant_decide(const aco_node_numeric_t current_node_index,
                                  const aco_exploration_data_ptr_t explorationData,
                                  const aco_edge_data_ptr_t edgeData,
                                  const aco_weight_t metricWeight,
                                  const aco_weight_t pheromoneWeight,
                                  const linked_list_element_numeric_t unvisited_buf_head_ix) {
    
    // {{declare & implement - based on pheromone levels, metrics}}
    aco_node_numeric_t num_unvisited = explorationData->list_unvisited_node_buffer_length;
    if (num_unvisited == 0) {
        return NULL/*(aco_node_numeric_t)-1*/;
    }
    else if (num_unvisited == 1) {
        //return (aco_node_numeric_t)explorationData->list_unvisited_node_buffer[unvisited_buf_head_ix].id;
        return explorationData->list_unvisited_node_buffer + unvisited_buf_head_ix/*(aco_node_numeric_t)unvisited_buf_head_ix*/;
    }

    aco_pherometric_t pherometric_total = calc_pherometric(
        //get_pheromone_level(current_node_index, (aco_node_numeric_t)explorationData->list_unvisited_node_buffer[unvisited_buf_head_ix].id, edgeData),
        //get_metric_value(current_node_index, (aco_node_numeric_t)explorationData->list_unvisited_node_buffer[unvisited_buf_head_ix].id, edgeData),
        get_pheromone_level(current_node_index, unvisited_buf_head_ix, edgeData),
        get_metric_value(current_node_index, unvisited_buf_head_ix, edgeData),
        metricWeight,
        pheromoneWeight
    );

    aco_node_numeric_t unvisited_counter = 1;
    linked_list_node_ptr_t iteration_node = &(explorationData->list_unvisited_node_buffer[unvisited_buf_head_ix]);
    while (unvisited_counter < num_unvisited && unvisited_counter < 3) {
        iteration_node = iteration_node->next;
        pherometric_total += calc_pherometric(
            //get_pheromone_level(current_node_index, (aco_node_numeric_t)explorationData->list_unvisited_node_buffer[0].id, edgeData),
            //get_metric_value(current_node_index, (aco_node_numeric_t)explorationData->list_unvisited_node_buffer[0].id, edgeData),
            get_pheromone_level(current_node_index, (aco_node_numeric_t)iteration_node->id, edgeData),
            get_metric_value(current_node_index, (aco_node_numeric_t)iteration_node->id, edgeData),
            metricWeight,
            pheromoneWeight
        );
        unvisited_counter++;
    }

    aco_randomness_t r = get_floating_point_random();
    aco_pherometric_t partial_sum = calc_pherometric(
        get_pheromone_level(current_node_index, unvisited_buf_head_ix, edgeData),
        get_metric_value(current_node_index, unvisited_buf_head_ix, edgeData),
        metricWeight,
        pheromoneWeight
    );
    if (r < 50 * ((aco_randomness_t)partial_sum) / (aco_randomness_t)pherometric_total) {
        return explorationData->list_unvisited_node_buffer + unvisited_buf_head_ix/*(aco_node_numeric_t)unvisited_buf_head_ix*/;
    }
    
    unvisited_counter = 1;
    iteration_node = &(explorationData->list_unvisited_node_buffer[unvisited_buf_head_ix]);
    while (unvisited_counter < num_unvisited) {
        iteration_node = iteration_node->next;
        partial_sum += calc_pherometric(
            get_pheromone_level(current_node_index, (aco_node_numeric_t)iteration_node->id, edgeData),
            get_metric_value(current_node_index, (aco_node_numeric_t)iteration_node->id, edgeData),
            metricWeight,
            pheromoneWeight
        );
        if (r < ((aco_randomness_t)partial_sum) / (aco_randomness_t)pherometric_total) {
            return iteration_node/*(aco_node_numeric_t)iteration_node->id*/;
        }
        unvisited_counter++;
    }
    return iteration_node/*(aco_node_numeric_t)iteration_node->id*/; //should never reach here in theory
}

aco_measure_t aco_route_length(const aco_route_ptr_t routePtr, aco_node_array_ptr_t nodes, const acoPairScalarFunc metric) {
    /*aco_measure_t L = 0;
    for (aco_route_node_numeric_t k = 0; k < route.num_nodes - 1; k++) { //calc route length
        L += config->node_metric(nodes->buffer[route.node_indices[k]], nodes->buffer[route.node_indices[k+1]]);
    }*/
    aco_measure_t L = 0;
    for (aco_route_node_numeric_t k = 0; k < routePtr->num_nodes - 1; k++) { //calc route length
        aco_node_numeric_t node1_index = routePtr->node_indices[k];
        aco_node_numeric_t node2_index = routePtr->node_indices[k + 1];
        //aco_measure_t metric_val = metric(nodes->buffer[routePtr->node_indices[k]], nodes->buffer[routePtr->node_indices[k+1]]);
        aco_measure_t metric_val = metric(nodes->buffer[node1_index], nodes->buffer[node2_index]);
        if (metric_val == INFINITY) {
            int a = 1;
            a += 1;
        }
        L += metric_val;
    }
    return L;
}

/**
 * @brief Optimize the route between abstract nodes using Ant Colony Optimization (ACO)
 * @param nodes An array of abstract nodes
 * @param config Configuration structure instance for the algorithm
 * @returns The ordering of nodes in an optimal route
*/
aco_result_t/*aco_route_t*//*aco_node_ordering_t*/ aco_optimize_route(const aco_node_array_ptr_t nodes, 
                                       const aco_config_ptr_t config) {
    _aco_random_guard();
    aco_route_t bestRoute;
    aco_measure_t bestRouteLength = -1;
    aco_edge_data_t edgeData;
    
    /*for (aco_node_numeric_t i = 0; i < nodes->size; i++) {
        explorationData.list_node_buffer[i] = nodes->buffer[i];
    }
    linked_list_from_buffer(explorationData.list_node_buffer, 0, )*/

    init_edge_data(&edgeData, nodes, config->node_metric);
    
    aco_randomness_t r;
    //aco_node_numeric_t selected_node_index;
    
    for (aco_iteration_t i = 0; i < config->num_iters; i++) {
        
        for (aco_edge_numeric_t j1 = 0; j1 < nodes->size; j1++) {
            for (aco_edge_numeric_t j2 = 0; j2 < nodes->size; j2++) {
                set_pheromone_level_appendix(j1, j2, 0, &edgeData);
            }
        }
        
        for (aco_ant_numeric_t j = 0; j < config->num_ants; j++) {

            //<<<remember to setup route and unvisited>>>
            aco_route_t route;
            route.num_nodes = 0;
            aco_exploration_data_t explorationData;
            linked_list_from_buffer(nodes->buffer, nodes->size, 
                                explorationData.list_unvisited_node_buffer);
            explorationData.list_unvisited_node_buffer_length = nodes->size;
            linked_list_element_numeric_t unvisited_buf_head_ix = 0;


            

            aco_node_numeric_t selected_node_index = get_random_aco_node_index(explorationData.list_unvisited_node_buffer_length);
            linked_list_node_ptr_t selected_list_node = linked_list_node_get(selected_node_index,
                                                            explorationData.list_unvisited_node_buffer, 
                                                            explorationData.list_unvisited_node_buffer_length,
                                                            0);
            if (selected_list_node->id == unvisited_buf_head_ix) {
                unvisited_buf_head_ix = selected_list_node->next->id;//
            }
            linked_list_element_remove(selected_list_node); //... ... do not forget to decrement length of unvisited nodes buffer
            explorationData.list_unvisited_node_buffer_length--;
            add_route_node_index(&route, selected_list_node->id);
            
                                                            

            while (route.num_nodes < config->num_nodes_to_visit) {
                r = get_floating_point_random();
                linked_list_node_ptr_t selected_list_node;
                
                if (r < config->ant_randomness_factor) {
                    selected_node_index = get_random_aco_node_index(explorationData.list_unvisited_node_buffer_length);
                    selected_list_node = linked_list_node_get(selected_node_index, 
                                                            explorationData.list_unvisited_node_buffer, 
                                                            explorationData.list_unvisited_node_buffer_length,
                                                            unvisited_buf_head_ix);
                }
                else {
                    // decide based on pheromones and metrics
                    // {{{implement}}}, roulette selection / different selection methods?
                    ////decision_node_index = ;
                    ////decision_list_node = ;
                    selected_list_node = aco_ant_decide(
                        get_last_route_node_index(&route),
                        &explorationData,
                        &edgeData,
                        config->metric_weight,
                        config->pheromone_weight,
                        unvisited_buf_head_ix
                    );
                }
                
                if (selected_list_node->id == unvisited_buf_head_ix && explorationData.list_unvisited_node_buffer_length != 1) {
                    unvisited_buf_head_ix = selected_list_node->next->id;//
                }
                linked_list_element_remove(selected_list_node); //... ... do not forget to decrement length of unvisited nodes buffer
                explorationData.list_unvisited_node_buffer_length--;
                add_route_node_index(&route, selected_list_node->id);
                
            }

            //update pheromones appendix
            
            /*aco_measure_t L = 0;
            for (aco_route_node_numeric_t k = 0; k < route.num_nodes - 1; k++) { //calc route length
                L += config->node_metric(nodes->buffer[route.node_indices[k]], nodes->buffer[route.node_indices[k+1]]);
            }*/
            aco_measure_t L = aco_route_length(&route, nodes, config->node_metric);

            aco_pheromone_t pheromone_level_appendix;
            for (aco_route_node_numeric_t k = 0; k < route.num_nodes - 1; k++) {
                pheromone_level_appendix = get_pheromone_level_appendix(route.node_indices[k], route.node_indices[k+1], &edgeData);
                pheromone_level_appendix += ((aco_pheromone_t)1) / ((aco_pheromone_t)L);
                set_pheromone_level_appendix(route.node_indices[k], route.node_indices[k+1], pheromone_level_appendix, &edgeData);
            }

            if (L < bestRouteLength || bestRouteLength == -1) {
                bestRouteLength = L;
                //bestRoute = route; //ok?;
                bestRoute.num_nodes = route.num_nodes;
                for (aco_node_numeric_t i = 0; i < route.num_nodes; i++) {
                    bestRoute.node_indices[i] = route.node_indices[i];
                }
                
                
            }
            
        }
        //update pheromones
        aco_pheromone_t pheromone_level;
        for (aco_edge_numeric_t j1 = 0; j1 < nodes->size; j1++) {
            for (aco_edge_numeric_t j2 = 0; j2 < nodes->size; j2++) {
                pheromone_level = get_pheromone_level(j1, j2, &edgeData);
                pheromone_level *= (aco_pheromone_t)((aco_evaporation_t)1 - config->evaporation_factor);
                pheromone_level += get_pheromone_level_appendix(j1, j2, &edgeData);

                set_pheromone_level(j1, j2, pheromone_level, &edgeData);
            }
        }
    }

    aco_result_t result;
    result.route = bestRoute;
    result.route_length = aco_route_length(&bestRoute, nodes, config->node_metric);
    //return bestRoute;
    return result;
}