#include "ga.h"

/* typedef struct {
    unsigned int epochs;
    double mutation_probability; 
    double dropout;
    gaFunc mutate; //maybe add multiple mutations (with respective probablities) later?
    gaPairFunc crossover; //maybe add multiple crossovers later?
    SELECTION_METHOD parentingPoolSelection;
    SELECTION_METHOD veteranSelection;
} ga_config_t; */

ga_population_t rouletteSelect(ga_population_t population, 
                               const gaFunc f, 
                               const unsigned int selectionSize) {
    // {{{implement}}}
    // use blob as in SA to store parents/veterans? Consistency?
    // No, just use the pointers to evolve's population parameter
}

ga_population_t evolve(const ga_population_t* population, 
                       const gaFunc f, 
                       const ga_config_t config, 
                       const ga_codomain_config_t codomainConfig) {
    for (unsigned int generation = 0; i < config.epochs; i++) {
        static ga_population_t parentingPool;
        switch(config.parentingPoolSelection) {
            /* case ROULETTE:
                parentingPool = */
        }
    }
}


