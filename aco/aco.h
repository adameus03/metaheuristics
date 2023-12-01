/* Abstract node type */
typedef void* aco_node_t;
/* Type used to index the nodes */
typedef unsigned int aco_numeric_t;
/* Type used to express the distance between the nodes */
typedef double aco_measure_t;
/* Type used to denote a "(node, node) --> measure" function */
typedef aco_measure_t (*acoPairScalarFunc)(const aco_node_t, const aco_node_t);

/*
    Array structure for storing a pointer to a nodes buffer
*/
typedef struct {
    aco_node_t* buffer;
    aco_numeric_t size;
} aco_node_array_t;

/*
    Array structure for storing a pointer to the nodes ordering which should be an array of node indices 
*/
typedef struct {
    aco_numeric_t* buffer;
    aco_numeric_t size;
} aco_node_ordering_t;

/*
    Configuration structure for the Ant Colony Optimization (ACO) algorithm.
    Contains the route optimization control parameters and the metric for calculating distance between the nodes.
*/
typedef struct {
    /* Metric function used to measure distance between the nodes */
    acoPairScalarFunc node_metric;
    /* Importance of metric in the ACO optimization process*/
    aco_measure_t metric_weight;
    /* Importance of ant pheromones in the ACO optimization process */
    aco_measure_t pheromone_weight;
    /* Number of nodes in the optimal route */
    aco_numeric_t num_nodes_to_visit;
} aco_config_t;

/**
 * @brief Optimize the route between abstract nodes using Ant Colony Optimization (ACO)
 * @param nodes An array of abstract nodes
 * @param config Configuration structure instance for the algorithm
 * @returns The ordering of nodes in an optimal route
*/
aco_node_ordering_t aco_optimize_route(aco_node_array_t nodes, aco_config_t config); 