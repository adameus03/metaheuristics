#include "backpack.h"

void print_backpack_composition_mask(const backpack_t backpack, unsigned char* compositionMask);
void print_backpack(const backpack_t backpack, unsigned char* compositionMask);
unsigned int calc_backpack_composition_weight(const backpack_t backpack, unsigned char* compositionMask);
unsigned int calc_backpack_composition_value(const backpack_t backpack, unsigned char* compositionMask);
unsigned int calc_backpack_composition_fitness(const backpack_t backpack, unsigned char* compositionMask);