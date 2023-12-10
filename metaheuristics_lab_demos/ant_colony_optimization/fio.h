#include "aco_r2_cart_euc.h"
#define FILE_LINE_MAX_LENGTH 100

typedef struct {
    aco_node_array_t node_array;
    r2_cart_point_storage_t point_storage;
} fio_out_nodes_t;

fio_out_nodes_t/*r2_cart_point_ptr_buffer_t*/ fio_get_points(const char* filePath);

