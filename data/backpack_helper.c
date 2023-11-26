#include "backpack_helper.h"
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 


void print_backpack_composition_mask(const backpack_t backpack, unsigned char* compositionMask) {
    unsigned int num_octets_mask = (backpack.num_available >> 0x3) + (backpack.num_available % 0x8 != 0x0);
    for (unsigned int i = 0; i < num_octets_mask; i++) {
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(compositionMask[i]));
    }
}
void print_backpack(const backpack_t backpack, unsigned char* compositionMask) {
    unsigned int content_weight = 0;
    unsigned int content_value = 0;
    for (unsigned int i = 0; i < backpack.num_available; i++) {
        if (compositionMask[i >> 0x3] & (((unsigned char)0x80U) >> (i % 0x8))) {
            printf("%d %s weight %u val %u\n", i, backpack.available_names[i], backpack.available_weights[i], backpack.available_values[i]);
            content_weight += backpack.available_weights[i];
            content_value += backpack.available_values[i];
        }
    }
    if (content_weight > backpack.backpack_capacity) {
        printf("\nBackpack capacity exceeded\n");
    }
    else {
        printf("\nCollected weight: %u / %u\n", content_weight, backpack.backpack_capacity);
        printf("Collected value: %u\n", content_value);
    }
}