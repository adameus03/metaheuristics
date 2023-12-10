typedef void* linked_list_element_t;
typedef linked_list_element_t* linked_list_element_buffer_t;
typedef unsigned int linked_list_element_numeric_t;

typedef struct _linked_list_node {
    linked_list_element_t element;
    linked_list_element_numeric_t id;
    struct _linked_list_node* next;
    struct _linked_list_node* prev;
} linked_list_node_t;

typedef linked_list_node_t* linked_list_node_ptr_t;
typedef linked_list_node_ptr_t linked_list_node_buffer_t;
typedef linked_list_node_t linked_list_t;
typedef linked_list_t* linked_list_ptr_t;

void linked_list_element_remove(linked_list_node_ptr_t linked_node);

void linked_list_from_buffer(const linked_list_element_buffer_t buffer, 
                             const linked_list_element_numeric_t buf_size,
                             linked_list_node_buffer_t nodes_buffer);

/* linked_list_element_t linked_list_element_get(linked_list_element_numeric_t index,
                                              const linked_list_node_buffer_t nodes_buffer,
                                              const linked_list_element_numeric_t buf_len); */

linked_list_node_ptr_t linked_list_node_get(linked_list_element_numeric_t index,
                                            const linked_list_node_buffer_t nodes_buffer,
                                            const linked_list_element_numeric_t buf_len,
                                            const linked_list_element_numeric_t buf_head_ix);