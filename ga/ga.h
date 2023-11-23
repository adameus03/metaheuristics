typedef void* (*gaFunc)(const void*);
typedef void* (*gaPairFunc)(const void*, const void*);
typedef double (*gaScalarFunc)(const void*, const void*);
typedef CMP_RESULT (*gaPredicate)(const void*, const void*);
typedef enum { LEFT, RIGHT } CMP_RESULT;
typedef enum {
    ROULETTE,
    RANKING,
    TOURNAMENT,
    ELITE
} SELECTION_METHOD;

typedef struct {
    unsigned int epochs;
    double mutation_probability; 
    double dropout;
    gaFunc mutate; //maybe add multiple mutations (with respective probablities) later?
    gaPairFunc crossover; //maybe add multiple crossovers later?
    SELECTION_METHOD parentingPoolSelection;
    SELECTION_METHOD veteranSelection;
} ga_config_t;

typedef struct {
    unsigned int size;
    void** members;
} ga_population_t; //sa_type consistency?

/**
 * Structure instance containing the optimized function's codomain configuration for the algorithm
*/
typedef struct {
    /* A comparing function that should be able to generate a CMP_RESULT comparison result 
       based on a given pair of optimized function codomain objects */
    gaPredicate comparer;
    /* A metric function that should be able to give the algoritm an idea
       of how far the 2 given objects are from each other in the optimized function's codomain space */
    gaScalarFunc metric;
} ga_codomain_config_t;


ga_population_t evolve(const ga_population_t* population, 
                       const gaFunc f, 
                       const ga_config_t config, 
                       const ga_codomain_config_t codomainConfig); //maybe instead of returning, update a variable which could be read by different process during GA's execution?




/* typedef struct {
    unsigned char* blobPopulationPtr;
    unsigned long domainExtent;
    unsigned long codomainExtent;
} _blob;
_blob _blob_loc(const _blob* blob);
//
// @brief Macro for setting domain & codomain types for the genetic algorithm
// @param domainType Optimized function domain datatype
// @param codomainType Optimized function codomain datatype
//
#define sa_type(domainType, codomainType) ({ \
    static unsigned char domainStorage[sizeof(domainType) << 1]; \
    static unsigned char codomainStorage[sizeof(domainType) << 1]; \
    static _blob blob; \
    blob.blobDomainPtr = domainStorage; \
    blob.blobCodomainPtr = codomainStorage; \
    blob.domainExtent = sizeof(domainType); \
    blob.codomainExtent = sizeof(codomainType); \
    _blob_loc(&blob); \
}) */