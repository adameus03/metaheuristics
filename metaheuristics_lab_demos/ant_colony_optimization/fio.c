#include "fio.h"
#include <stdio.h>
#include <stdlib.h> //exit

fio_out_nodes_t/*r2_cart_point_ptr_buffer_t*/ fio_get_points(const char* filePath) {
    fio_out_nodes_t output;
    
    //aco_node_array_t node_array;
    //r2_cart_point_ptr_t buffer[ACO_MAX_NODES];
    //node_array.buffer = buffer;
    //node_array.buffer = 
    //node_array.size = 0;
    output.node_array.size = 0;
    
    FILE *file;
    char line[FILE_LINE_MAX_LENGTH];
    file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1); //
        return output; //
    }

    r2_cart_point_t point;
    while(fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%4.0f %4.0f %4.0f", NULL, &point.x, &point.y);
        output.point_storage.buffer[output.node_array.size] = point;
        output.node_array.buffer[output.node_array.size] = output.point_storage.buffer + output.node_array.size;
        output.node_array.size++;
    }

    fclose(file);
    return output;
}