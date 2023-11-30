#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "ga.h"

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
    return _gaFunc;
}

int __segregationComparerF(const void* solA, const void* solB, const gaFunc f, const ga_codomain_config_t codomainConfig) {
    static void* fa;
    static void* fb;
    fa = f(solA);
    fb = f(solB);
    static CMP_RESULT comparerResult;
    comparerResult = codomainConfig.comparer(fa, fb);
    if (comparerResult == LEFT) {
        return 1;
    }
    else {
        return -1;
    }
}

int __segregationComparer(const void* solA, const void* solB) {
    return __segregationComparerF(solA, solB, __gaFunc(NULL), __codomainConfig(NULL));
}

_gaBlob _gaBlob_loc(const _gaBlob* blob) {
    static _gaBlob __blob;
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
    qsort((population->members), population->size, _gaBlob_loc(NULL).domainExtent, __segregationComparer);
}

/*
    _S - solution
    _SM - solution measure
    _TPB - temp population buf
    _SELBUF - selection buf
    _TOURGRP - tournament group buf
*/
typedef enum { _S, _SM, _TPB, _SELBUF, _TOURGRP } _gaBlobID;
typedef enum { _READ, _WRITE } _gaBlobAccess;

unsigned int _gaBlob_TPBIndex(const unsigned int* index) {
    static unsigned int _index = 0;
    if (index) {
        _index = *index;
    }
    return _index;
}

unsigned int _gaBlob_SELBUFIndex(const unsigned int* index) {
    static unsigned int _index = 0;
    if (index) {
        _index = *index;
    }
    return _index;
}

unsigned int _gaBlob_TOURGRPIndex(const unsigned int* index) {
    static unsigned int _index = 0;
    if (index) {
        _index = *index;
    }
    return _index;
}

void* _gaBlob_access(const void* dataPtr, const _gaBlobID blobId, const _gaBlobAccess blobAccess) {
    if (blobId > 4 || blobId < 0) {
        return NULL;
    }
    static _gaBlob blob;
    blob = _gaBlob_loc(NULL);

    static unsigned char* blobPtr;
    static size_t blobCurrentUnitSize;
    static size_t blobUnitIndex;
    if (blobId == _S) {
        blobPtr = blob.blobDomainPtr;
        blobCurrentUnitSize = blob.domainExtent;
        blobUnitIndex = 0;
    }
    else if (blobId == _SM) {
        blobPtr = blob.blobCodomainPtr;
        blobCurrentUnitSize = blob.codomainExtent;
        blobUnitIndex = 0;
    }
    else if (blobId == _TPB) {
        blobUnitIndex = _gaBlob_TPBIndex(NULL);
        blobPtr = blob.blobTempPopulationPtr;
        blobCurrentUnitSize = blob.domainExtent;
    }
    else if (blobId == _SELBUF){
        blobUnitIndex = _gaBlob_SELBUFIndex(NULL);
        blobPtr = blob.blobSelectionPtr;
        blobCurrentUnitSize = blob.domainExtent;
    }
    else {
        blobUnitIndex = _gaBlob_TOURGRPIndex(NULL);
        blobPtr = blob.blobTournamentGroupPtr;
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

void* _gaBlob_read(const _gaBlobID blobId) {
    return _gaBlob_access(NULL, blobId, _READ);
}

void _gaBlob_write(const void* dataPtr, const _gaBlobID blobId) {
    _gaBlob_access(dataPtr, blobId, _WRITE);
}

/**
 * Elite selection for the genetic algorithm
*/
ga_population_t elite_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    segregate(&population);
    static ga_population_t selectedPopulation;
    selectedPopulation.size = selectionSize;
    selectedPopulation.members = population.members;
    return selectedPopulation;
}

/**
 * Roulette selection for the genetic algorithm
*/
ga_population_t roulette_select(ga_population_t population,
                               const gaFunc f, 
                               const unsigned int selectionSize) {
    static ga_population_t selectedPopulation;
    static ga_codomain_config_t codomainConfig;
    codomainConfig = __codomainConfig(NULL);
    static double r;

    selectedPopulation.size = selectionSize;
    // selectedPopulation.members = population.members; // move selected members to beginning of buffer
    static void** selectedPopulationBuffer;
    selectedPopulationBuffer = (void**)(_gaBlob_loc(NULL).blobSelectionPtr);
    selectedPopulation.members = selectedPopulationBuffer;

    double totalFitness = 0;
    // Could be optimized in cost of using additional memory (storing members fitness)
    for (unsigned int i = 0; i < population.size; i++) {
        totalFitness += codomainConfig.norm(f(population.members[i]));
    }

    if (totalFitness == 0) {
        ga_population_t trimmedPopulation;
        trimmedPopulation.size = selectionSize;
        trimmedPopulation.members = population.members;
        return trimmedPopulation;
    }

    for (unsigned int i = 0; i < selectionSize; i++) {
        r = ((double)rand()) / ((double)RAND_MAX);
        static double sumFitness;
        sumFitness = 0;
        for (unsigned int j = 0; j < population.size; j++) {
            sumFitness += codomainConfig.norm(f(population.members[j]));
            if (r < ((double)sumFitness) / ((double)totalFitness)) {
                _gaBlob_SELBUFIndex(&i);
                _gaBlob_write(population.members + j, _SELBUF);
                break;
            }
        }
    }
    return selectedPopulation;
}

/**
 * Rank selection for the genetic algorithm
*/
ga_population_t ranking_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    segregate(&population);
    static ga_population_t selectedPopulation;
    static ga_codomain_config_t codomainConfig;
    codomainConfig = __codomainConfig(NULL);
    static double r;

    selectedPopulation.size = selectionSize;
    static void** selectedPopulationBuffer;
    selectedPopulationBuffer = (void**)(_gaBlob_loc(NULL).blobSelectionPtr);
    selectedPopulation.members = selectedPopulationBuffer;

    double totalRanking = 0;
    for (unsigned int i = 0; i < population.size; i++) {
        totalRanking += ((double)1) / ((double)(i + 1));
    }
    for (unsigned int i = 0; i < selectionSize; i++) {
        r = ((double)rand()) / ((double)RAND_MAX);
        static double sumRanking;
        sumRanking = 0;
        for (unsigned int j = 0; j < population.size; j++) {
            sumRanking += ((double)1) / ((double)(j + 1));
            if (r < ((double)sumRanking) / ((double)totalRanking)) {
                _gaBlob_SELBUFIndex(&i);
                _gaBlob_write(population.members + j, _SELBUF);
                break;
            }
        }
    }

    return selectedPopulation;
}

double _tournament_group_size_factor(const double* factor) {
    static double _factor = 0.1;
    if (factor) {
        _factor = *factor;
    }
    return _factor;
}

double _tournament_determinism_factor(const double* p) {
    static double _p = 1;
    if (p) {
        _p = *p;
    }
    return _p;
}

/**
 * @brief Function used to update the tournament group size for the tournament selection
 * @param groupsizeFactor
 * @note The default groupsize factor is 0.1
*/
void set_tournament_group_size_factor(const double groupsizeFactor) {
    static double _groupsizeFactor;
    _groupsizeFactor = groupsizeFactor;
    _tournament_group_size_factor(&_groupsizeFactor);
}

/**
 * @brief Function used to update the tournament determinism factor (p)
 * @param p
 * @note The default value used is 1 (fully deterministic tournaments)
*/
void set_tournament_determinism_factor(const double p) {
    static double _determinismFactor;
    _determinismFactor = p;
    _tournament_determinism_factor(&_determinismFactor);
}

/**
 * Tournamnet selection for the genetic algorithm
*/
ga_population_t tournament_select(ga_population_t population,
                             const gaFunc f,
                             const unsigned int selectionSize) {
    static ga_population_t selectedPopulation;
    static ga_codomain_config_t codomainConfig;
    codomainConfig = __codomainConfig(NULL);

    selectedPopulation.size = selectionSize;
    static void** selectedPopulationBuffer;
    selectedPopulationBuffer = (void**)(_gaBlob_loc(NULL).blobSelectionPtr);
    selectedPopulation.members = selectedPopulationBuffer;

    unsigned int tournament_groupsize = _tournament_group_size_factor(NULL) * population.size;
    double p = _tournament_determinism_factor(NULL);
    
    for (unsigned int i = 0; i < selectionSize; i++) {
        static ga_population_t tournamentGroup;
        static void** tournamentMembersBuf;
        tournamentMembersBuf = (void**)(_gaBlob_loc(NULL).blobTournamentGroupPtr);
        tournamentGroup.size= tournament_groupsize;
        tournamentGroup.members = tournamentMembersBuf;   

        for (unsigned int j = 0; j < tournament_groupsize; j++) {
            static unsigned int tournamentMemberIndex;
            tournamentMemberIndex = rand() % population.size;
            _gaBlob_TOURGRPIndex(&j);
            _gaBlob_write(population.members + tournamentMemberIndex, _TOURGRP);
        }

        segregate(&tournamentGroup);
        static double r;
        static unsigned char selected;
        selected = 0x0;
        for (unsigned int j = 0; j < tournament_groupsize - 1; j++) {
            r = ((double)rand()) / ((double)RAND_MAX);
            if (r < p) {
                _gaBlob_SELBUFIndex(&i);
                _gaBlob_write(tournamentGroup.members + j, _SELBUF);
                selected = 0x1;
                break;
            }
        }
        if (!selected) {
            _gaBlob_SELBUFIndex(&i);
            _gaBlob_write(tournamentGroup.members + tournament_groupsize - 1, _SELBUF);
        }
    }

    return selectedPopulation;
}

/**
 * @brief Filter population using a chosen selection method
 * @param population Population to filter
 * @param f Fitness function used for the population filtering
 * @param selectionSize Size of the subpopulation to be selected
 * @param selectionMethod The selection type used to perform the subpopulation selection
 * @returns The subpopulation based on the input population filtering
*/
ga_population_t select_subpopulation(ga_population_t population, 
                       const gaFunc f, 
                       const unsigned int selectionSize,
                       const SELECTION_METHOD selectionMethod) {
    static ga_population_t selectedPool;
    switch (selectionMethod) {
        case ROULETTE:
            selectedPool = roulette_select(population, f, selectionSize);
            break;
        case RANKING:
            selectedPool = ranking_select(population, f, selectionSize);
            break;
        case TOURNAMENT:
            selectedPool = tournament_select(population, f, selectionSize);
            break;
        case ELITE:
            selectedPool = elite_select(population, f, selectionSize);
            break;
    }
    return selectedPool;
}

void _gaRandom_guard() {
    static unsigned char guard = 0x1;
    if (guard) {
        guard = 0x0;
        srand((unsigned int)time(NULL));   
    }
}

/**
 * @brief Execute crossovers on the provided subpopulation
 * @param parentingPool The subpopulation to execute crossovers on
 * @param crossover The crossover function to be executed
*/
void crossoverHub(const ga_population_t parentingPool, const gaPairFunc crossover) {
    static unsigned int childPosition;
    childPosition = 0;
    for (unsigned int i = 0; i < parentingPool.size - 1; i++) {
        for (unsigned int j = i + 1; j < parentingPool.size; j++) {
            _gaBlob_TPBIndex(&childPosition);
            static void* crossoverResult;
            crossoverResult = crossover(parentingPool.members[i], parentingPool.members[j]);
            _gaBlob_write(&crossoverResult, _TPB);
            childPosition++;
        }
    }
}

/**
 * @brief Execute mutations on a part of population
 * @param mutation_probability The probability that the mutation hub will issue a mutation on the target subpopulation
 * @param mutation The crossover function to be executed
 * @param mutationHubPopulationSize The size of the target subpopulation
 * @note For this function to mutate only the descendants subpopulation, it is assumed that the descendants are stored at the beginning of the backend population buffer
*/
void mutationHub(const double mutation_probability, const gaFunc mutation, const unsigned int mutationHubPopulationSize) {
    static unsigned int totalMutations;
    totalMutations = round(mutation_probability * mutationHubPopulationSize);

    for (unsigned int i = 0; i < totalMutations; i++) {
        static unsigned int mutationHubMemberPosition;
        mutationHubMemberPosition = rand() % mutationHubPopulationSize;
        _gaBlob_TPBIndex(&mutationHubMemberPosition);
        static void* mutationHubMember;
        mutationHubMember = *(void**)_gaBlob_read(_TPB);
        static void* mutationResult;
        mutationResult = mutation(mutationHubMember);
        _gaBlob_write(&mutationResult, _TPB);
    }
}

/**
 * @brief Extend the population by the descendants using members stored in a backend buffer
 * @param currentPopulation Population to be extended
 * @param numBirths Number of descendants to have births executed
 * @returns The extended population
*/
ga_population_t execute_births(const ga_population_t currentPopulation, const unsigned int numBirths) {
    static ga_population_t extendedPopulation;
    static void** populationBuffer;
    extendedPopulation.size = currentPopulation.size + numBirths;
    populationBuffer = (void**)(_gaBlob_loc(NULL).blobTempPopulationPtr);
    extendedPopulation.members = populationBuffer;
    for (unsigned int i = 0; i < currentPopulation.size; i++) {
        static unsigned int existingMemberPosition;
        existingMemberPosition = numBirths + i;
        _gaBlob_TPBIndex(&existingMemberPosition);
        _gaBlob_write(currentPopulation.members + i, _TPB);
    }
    return extendedPopulation;
}

/**
 * @brief The function used to execute the evolution for the genetic algorithm.
 * @param population - pointer to the population structure instance which value will be updated by this procedure
 * @param f The fitness function to be optimized
 * @param config Structure instance containing the algorithm's input conditions and operaton modes
 * @param domainConfig Structure instance containing domain space configuration for the algorithm's initial population generator
 * @param codomainConfig Structure instance containing f function's codomain configuration for the algorithm
*/
void evolve(ga_population_t* population, 
                       const gaFunc f, 
                       const ga_config_t config, 
                       const ga_codomain_config_t codomainConfig) {
    __gaFunc(&f);
    __codomainConfig(&codomainConfig);
    for (unsigned int generation = 0; generation < config.epochs; generation++) {
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
        numAncestors = ceil(0.5 + sqrt(numDescendants << 1));
        parentingPool = select_subpopulation(*population, f, numAncestors, config.parentingPoolSelection);// <<< ADD iteratively to POOOL
        
        crossoverHub(parentingPool, config.crossover);
        mutationHub(config.mutation_probability, config.mutate, numAncestors);
        
        static ga_population_t extendedPopulation;
        extendedPopulation = execute_births(*population, numDescendants);

        static ga_population_t veterans;
        veterans = select_subpopulation(extendedPopulation, f, population->size, config.veteranSelection);

        population->size = veterans.size;
        for (unsigned int i = 0; i < population->size; i++) {
            population->members[i] = veterans.members[i];
        }
    }
}

/**
 * @brief The core general function used to run the genetic algorithm.
 * @param f The fitness function to be optimized
 * @param config Structure instance containing the algorithm's input conditions and operaton modes
 * @param domainConfig Structure instance containing domain space configuration for the algorithm's initial population generator
 * @param codomainConfig Structure instance containing f function's codomain configuration for the algorithm
 * @returns The best solution from the final generation of the population, depending on the algorithms configuration
*/
void* ga_extreme(const gaFunc f, 
                 const ga_config_t config, 
                 const ga_domain_config_t domainConfig,
                 const ga_codomain_config_t codomainConfig) {
    
    _gaRandom_guard();
    //initialize population
    ga_population_t* population = domainConfig.domainGenerator();
    evolve(population, f, config, codomainConfig);

    _gaBlob_write(population->members[0], _S);
    _gaBlob_write(f(population->members[0]), _SM);
    
    for (unsigned int i = 1; i < population->size; i++) {
        static CMP_RESULT comparerResult;
        static void* candidate;
        static void* candidateMeasure;
        candidate = population->members[i];
        candidateMeasure = f(candidate);
        comparerResult = codomainConfig.comparer(_gaBlob_read(_SM), candidateMeasure);
       
        if (comparerResult == RIGHT) {
            _gaBlob_write(candidate, _S);
            _gaBlob_write(candidateMeasure, _SM);
        }
    }

    return (void*)_gaBlob_read(_S);
}

