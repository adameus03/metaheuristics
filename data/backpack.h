#ifndef __BACKPACK_T
#define __BACKPACK_T
typedef struct {
    unsigned int* available_weights;
    unsigned int* available_values;
    char** available_names;
    unsigned int num_available;
    unsigned int backpack_capacity;
} backpack_t;
#endif