#include "../ga/ga_bin_r_basic.h"
#include "bpack_optimizer.h"

double fitness(const binary_chromosome chromosome, const backpack_t backpack) {
    
}

unsigned int* optimized_backpack_element_ids(const backpack_t backpack) {
    // const gaFunc f, 
    // const ga_bin_r_basic_config_t config, 
    // const ga_bin_r_basic_startup_config_t startupConfig
    binary_chromosome solution = ga_bin_r_basic_extreme();
}