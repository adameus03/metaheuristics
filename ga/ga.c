#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "ga_selections.h"

/* typedef struct {
    unsigned int epochs;
    double mutation_probability; 
    double dropout;
    gaFunc mutate; //maybe add multiple mutations (with respective probablities) later?
    gaPairFunc crossover; //maybe add multiple crossovers later?
    SELECTION_METHOD parentingPoolSelection;
    SELECTION_METHOD veteranSelection;
} ga_config_t; */

ga_population_t roulette_select(ga_population_t population, 
                               const gaFunc f, 
                               const unsigned int selectionSize) {
    // {{{implement}}}
    // use blob as in SA to store parents/veterans? Consistency?
    // No, just use the pointers to evolve's population parameter
    static ga_population_t selectedPopulation;
    return selectedPopulation;
}

ga_codomain_config_t __codomainConfig(const ga_codomain_config_t* codomainConfig) {
    static ga_codomain_config_t _codomainConfig;
    if (codomainConfig) {
        _codomainConfig = *codomainConfig;
    } 
    return _codomainConfig;
}
gaFunc __gaFunc(const gaFunc* f) {
    static gaFunc _gaFunc;
    if (f) {
        _gaFunc = *f;
    }
    return _gaFunc
}

int __segregationComparerF(const void* solA, const void* solB, const gaFunc f, const ga_codomain_config_t* codomainConfig) {
    static void* fa;
    static void* fb;
    fa = f(solA);
    fb = f(solB);
    static CMP_RESULT comparerResult;
    comparerResult = codomainConfig.comparer(fa, fb);
    if (comparerResult == LEFT) {
        return -1;
    }
    else {
        return 1;
    }
}

void __segregationComparer(const void* solA, const void* solB) {
    return __segregationComparerF(solA, solB, __gaFunc(NULL), __codomainConfig(NULL));
}

_blob _blob_loc(const _blob* blob) {
    static _blob __blob;
    if (blob == NULL) {
        return __blob;
    }
    else {
        __blob = *blob;
        return __blob;
    }
}

/**
 * @brief Moves the strongest solutions to the beginning of the population buffer
 * @param population The population to be segregated
*/
void segregate(const ga_population_t* population) { //
    qsort(*(population->members), population->size, _blob_loc(NULL).domainExtent, __segregationComparer);
}

ga_population_t elite_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    segregate(&population);
    static ga_population_t selectedPopulation;
    selectedPopulation.size = selectionSize;
    selectedPopulation.members = population.members;
    return selectedPopulation;
}

ga_population_t ranking_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    // {{{implement}}}
    static ga_population_t selectedPopulation;
    return selectedPopulation;
}

ga_population_t tournament_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    // {{{implement}}}
    static ga_population_t selectedPopulation;
    return selectedPopulation;
}

ga_population_t select(ga_population_t population, 
                       const gaFunc f, 
                       const unsigned int selectionSize,
                       const SELECTION_METHOD selectionMethod) {
    static ga_population_t selectedPool;
    switch (selectionMethod) {
        case ROULETTE:
            selectedPool = roulette_select(population, f, numAncestors);
            break;
        case RANKING:
            selectedPool = ranking_select(population, f, numAncestors);
            break;
        case TOURNAMENT:
            selectedPool = tournament_select(population, f, numAncestors);
            break;
        case ELITE:
            selectedPool = elite_select(population, f, numAncestors);
            break;
    }
    return selectedPool;
}






typedef enum { _S, _SM, _TPB } _blobID;
typedef enum { _READ, _WRITE } _blobAccess;

unsigned int _blob_TPBIndex(const unsigned int* index) {
    static unsigned int _index = 0;
    if (index) {
        _index = *index;
    }
    return _index;
}

void* _blob_access(const void* dataPtr, const _blobID blobId, const _blobAccess blobAccess) {
    if (blobId > 2 || blobId < 0) {
        return NULL;
    }
    static _blob blob;
    blob = _blob_loc(NULL);

    static unsigned char* blobPtr;
    static size_t blobCurrentUnitSize;
    static size_t blobUnitIndex;
    if (blobId == _S) {
        blobPtr = blob.blobDomainPtr;
        blobCurrentUnitSize = blob.domainExtent;
        // blobUnitIndex = blobId == _BS;
        blobUnitIndex = 0;
    }
    else if (blobId == _SM) {
        blobPtr = blob.blobCodomainPtr;
        blobCurrentUnitSize = blob.codomainExtent;
        // blobUnitIndex = blobId == _BSM;
        blobUnitIndex = 0;
    }
    else {
        blobUnitIndex = _blob_TPBIndex(NULL);
        blobPtr = blob.tempPopulationPtr;
        blobCurrentUnitSize = blob.domainExtent;
    }
    switch (blobAccess) {
        case _READ:
            return (void*)(blobPtr + blobUnitIndex * blobCurrentUnitSize);
        case _WRITE:
            unsigned char* ucDataPtr = (unsigned char*)dataPtr;
            for (size_t i = 0; i < blobCurrentUnitSize; i++) {
                blobPtr[blobUnitIndex * blobCurrentUnitSize + i] = ucDataPtr[i];
            }
            return NULL;
        default:
            return NULL;
    }
}

void* _blob_read(const _blobID blobId) {
    return _blob_access(NULL, blobId, _READ);
}

void _blob_write(const void* dataPtr, const _blobID blobId) {
    _blob_access(dataPtr, blobId, _WRITE);
}

void _random_guard() {
    static unsigned char guard = 0x1;
    if (guard) {
        guard = 0x0;
        srand((unsigned int)time(NULL));   
    }
}

void alabama(const ga_population_t parentingPool, const gaPairFunc crossover) {
    static unsigned int childPosition;
    childPosition = 0;
    for (unsigned int i = 0; i < parentingPool.size - 1; i++) {
        //__tempPopulationStorage_Set(i, )
        for (unsigned int j = i + 1; j < parentingPool.size; j++) {
            _blob_TPBIndex(&childPosition);
            _blob_write(crossover(parentingPool[i], parentingPool[j]), _TPB);
            childPosition++;
        }
    }
}

void fukushima(const double mutation_probability, const gaFunc mutation, const double fukushimaPopulationSize) {
    static unsigned int totalMutations;
    totalMutations = round(mutation_probability * fukushimaPopulationSize);

    for (unsigned int i = 0; i < totalMutations; i++) {
        static unsigned int fukushimaMemberPosition;
        fukushimaMemberPosition = rand() % totalMutations;
        _blob_TPBIndex(&fukushimaMemberPosition);
        static void* fukushimaMember;
        fukushimaMember = _blob_read(_TPB);
        _blob_write(mutation(fukushimaMember), _TPB);
    }
}

ga_population_t execute_births(const ga_population_t currentPopulation, const unsigned int numBirths) {
    static ga_population_t extendedPopulation;
    static void* populationBuffer;
    extendedPopulation.size = currentPopulation.size + numBirths;
    for (unsigned int i = 0; i < currentPopulation.size; i++) {
        static unsigned int existingMemberPosition;
        existingMemberPosition = numBirths + i;
        _blob_TPBIndex(&existingMemberPosition);
        _blob_write(currentPopulation.members + i, _TPB);
    }
    populationBuffer = _blob_loc(NULL).blobTempPopulationPtr;
    extendedPopulation.members = &populationBuffer;
    return extendedPopulation;
}

void/*ga_population_t*/ evolve(const ga_population_t* population, 
                       const gaFunc f, 
                       const ga_config_t config, 
                       const ga_codomain_config_t codomainConfig) {
    __gaFunc(&f);
    __codomainConfig(&codomainConfig);
    for (unsigned int generation = 0; i < config.epochs; i++) {
        static ga_population_t parentingPool;
        /*
            Minimum number of parents, so that that each child is an only child in his family is N*(N-1)/2 
            where N is the current population size

            A - number of ancestors
            D - number of descendants
            D=A*(A-1)/2
            2D=A^2 - A
            A^2 - A - 2D = 0
            (A - 1/2)^2 = 2D + 1/4
            A - 1/2 = +- sqrt(2D + 1/4)
            A = 1/2 +- sqrt(2D + 1/4)

            Asymptotic approximation: A = sqrt(2D) + 1/2
        */
        static unsigned int numDescendants;
        static unsigned int numAncestors;
        numDescendants = config.dropout * population->size;
        numAncestors = ceil(0.5 + sqrt(numDescendants << 1)); // {{{check if won't bug}}}

        // assert(numDescendants == numAscestors * (numAncestors - 1) / 2); //debug

        parentingPool = select(*population, f, numAncestors, config.parentingPoolSelection);// <<< ADD iteratively to POOOL
        // segregate(population); 
        alabama(parentingPool, config.crossover);
        fukushima(config.mutation_probability, config.mutate, numAncestors);
        // {{{cont}}}
        static ga_population_t extendedPopulation;
        extendedPopulation = execute_births(*population, numDescendants);

        static ga_population_t veterans;
        veterans = select(extendedPopulation, f, population->size, config.veteranSelection);
        *population = veterans;
    }
}

void* ga_extreme(const gaFunc f, 
                 const ga_config_t config, 
                 const ga_domain_config_t domainConfig,
                 const ga_codomain_config_t codomainConfig) {
    
    
    //initialize population
    ga_population_t* population = domainConfig.domainGenerator();
    evolve(population, f, config, codomainConfig);
    //{{{implement}}}
    // return best solution

    //{{{check if population is not empty?}}}
    _blob_write(population->members[0], _S);
    _blob_write(f(population->members[0]), _SM);
    // {{{maybe do this in each generation?}}} 
    for (unsigned int i = 1; i < population->size; i++) {
        static CMP_RESULT comparerResult;
        static void* candidate;
        static void* candidateMeasure;
        candidate = population->members[i];
        candidateMeasure = f(candidate);
        comparerResult = codomainConfig.comparer(_blob_read(_SM), candidateMeasure);
        if (comparerResult == RIGHT) {
            _blob_write(candidate, _S);
            _blob_write(candidateMeasure, _SM);
        }
    }
    return _blob_read(_S);
}

