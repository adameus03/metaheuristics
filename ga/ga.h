typedef void* (*gaFunc)(const void*);
typedef void* (*gaPairFunc)(const void*, const void*);
typedef double (*gaScalarFunc)(const void*);
typedef double (*gaPairScalarFunc)(const void*, const void*);
typedef struct {
    unsigned int size;
    void** members;
} ga_population_t;

typedef ga_population_t* (*gaGenerator)();
typedef enum { LEFT, RIGHT } CMP_RESULT;
typedef CMP_RESULT (*gaPredicate)(const void*, const void*);
typedef enum {
    ROULETTE,
    RANKING,
    TOURNAMENT,
    ELITE
} SELECTION_METHOD;

/**
 * Structure instance containing the algorithm's input conditions and operaton modes
*/
typedef struct {
    unsigned int epochs;
    double mutation_probability; 
    double dropout;
    gaFunc mutate;
    gaPairFunc crossover;
    SELECTION_METHOD parentingPoolSelection;
    SELECTION_METHOD veteranSelection;
} ga_config_t;

/**
 * Structure instance containing domain space configuration for the algorithm's initial population generator
*/
typedef struct {
    gaGenerator domainGenerator;
} ga_domain_config_t;

/**
 * Structure instance containing the optimized function's codomain configuration for the algorithm
*/
typedef struct {
    /* A comparing function that should be able to generate a CMP_RESULT comparison result 
       based on a given pair of optimized function codomain objects */
    gaPredicate comparer;
    /* A metric function that should be able to give the algoritm an idea
       of how far the 2 given objects are from each other in the optimized function's codomain space */
    gaPairScalarFunc metric;
    gaScalarFunc norm;
} ga_codomain_config_t;

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
                 const ga_codomain_config_t codomainConfig);

void set_tournament_group_size_factor(const double groupsizeFactor);

void set_tournament_determinism_factor(const double p);


typedef struct {
    unsigned char* blobDomainPtr;
    unsigned char* blobCodomainPtr;
    unsigned char* blobTempPopulationPtr;
    unsigned char* blobSelectionPtr;
    unsigned char* blobTournamentGroupPtr;
    unsigned long domainExtent;
    unsigned long codomainExtent;
    unsigned long tempPopulationBufferLength;
} _gaBlob;
_gaBlob _gaBlob_loc(const _gaBlob* blob);
/*
 @brief Macro for setting domain type and temporary population buffer size for the genetic algorithm {{{Elaborate?}}}
 @param domainType Optimized function domain datatype
 @Optimized function codomain datatype
 @param N Max size for GA's population
*/
#define ga_type(domainType, codomainType, N) ({ \
    static unsigned char domainStorage[sizeof(domainType)]; \
    static unsigned char codomainStorage[sizeof(domainType)]; \
    static unsigned char tempPopulationStorage[(sizeof(domainType) * N) << 1]; \
    static unsigned char selectionStorage[sizeof(domainType) * N]; \
    static unsigned char tournamentGroupStorage[sizeof(domainType) * N]; \
    static _gaBlob blob; \
    blob.blobDomainPtr = domainStorage; \
    blob.blobCodomainPtr = codomainStorage; \
    blob.blobTempPopulationPtr = tempPopulationStorage; \
    blob.blobSelectionPtr = selectionStorage; \
    blob.blobTournamentGroupPtr = tournamentGroupStorage; \
    blob.domainExtent = sizeof(domainType); \
    blob.codomainExtent = sizeof(codomainType); \
    blob.tempPopulationBufferLength = N; \
    _gaBlob_loc(&blob); \
})
