#include "linked_list.h"

void linked_list_element_remove(linked_list_node_ptr_t linked_node) {
    linked_node->prev->next = linked_node->next;
    linked_node->next->prev = linked_node->prev;
}

void linked_list_from_buffer(const linked_list_element_buffer_t buffer, 
                             const linked_list_element_numeric_t buf_size,
                             linked_list_node_buffer_t nodes_buffer) {
    for (linked_list_element_numeric_t i = 0; i < buf_size; i++) {
        nodes_buffer[i].element = buffer[i];
        nodes_buffer[i].id = i;
    }

    for (linked_list_element_numeric_t i = 0; i < buf_size - 1; i++) {
        nodes_buffer[i].next = nodes_buffer + i + 1;
    }

    for (linked_list_element_numeric_t i = 1; i < buf_size; i++) {
        nodes_buffer[i].prev = nodes_buffer + i - 1;
    }
}

/* linked_list_element_t linked_list_element_get(linked_list_element_numeric_t index,
                                              const linked_list_node_buffer_t nodes_buffer,
                                              const linked_list_element_numeric_t buf_len) {
    // return nodes_buffer[index].element;
    linked_list_node_ptr_t target; //no empty verifiaction!

    if (index < buf_len - index) {
        target = (linked_list_node_ptr_t)nodes_buffer;
        while (--index) {
            target = target->next;
        }
    }
    else {
        target = (linked_list_node_ptr_t)(nodes_buffer + buf_len);
        while (--index) {
            target = target->prev;
        }
    }
    
    return target->element;
} */

/* Not at all */
linked_list_node_ptr_t linked_list_node_get(linked_list_element_numeric_t index,
                                            const linked_list_node_buffer_t nodes_buffer) {
    linked_list_node_ptr_t target; //no empty verifiaction!

    if (index < buf_len - index) {
        target = (linked_list_node_ptr_t)nodes_buffer;
        while (--index) {
            target = target->next;
        }
    }
    else {
        target = (linked_list_node_ptr_t)(nodes_buffer + buf_len);
        while (--index) {
            target = target->prev;
        }
    }
    
    // return target->element;
    return target;
}