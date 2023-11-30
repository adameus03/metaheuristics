#include "../data/backpack.h"
#include "../ga/ga_bin_r_basic.h"


typedef struct {
    ga_bin_r_basic_config_t gbrConfig;
    unsigned int gaPopulationSize;
    double* tournamentGroupSizePtr;
    double* tournamentDeterminismFactorPtr;

} bpack_optimizer_config_t;

unsigned char* optimized_backpack_composition_mask(const backpack_t backpack, const bpack_optimizer_config_t config);