typedef void* (*gaFunc)(const void*);
typedef void* (*gaPairFunc)(const void*, const void*);
typedef double (*gaScalarFunc)(const void*, const void*);
typedef struct {
    unsigned int size;
    void** members;
} ga_population_t; //sa_type consistency?

typedef ga_population_t* (*gaGenerator)();
typedef enum { LEFT, RIGHT } CMP_RESULT;
typedef CMP_RESULT (*gaPredicate)(const void*, const void*);
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

/* typedef struct {
    unsigned int size;
    void** members;
} ga_population_t; */ //sa_type consistency?


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
    gaScalarFunc metric;
} ga_codomain_config_t;


/* void /^ga_population_t^/ evolve(const ga_population_t* population, 
                       const gaFunc f, 
                       const ga_config_t config, 
                       const ga_codomain_config_t codomainConfig);*/ //maybe instead of returning, update a variable which could be read by different process during GA's execution?

void* ga_extreme(const gaFunc f, 
                 const ga_config_t config, 
                 const ga_domain_config_t domainConfig,
                 const ga_codomain_config_t codomainConfig);


typedef struct {
    unsigned char* blobDomainPtr;
    unsigned char* blobCodomainPtr;
    unsigned char* blobTempPopulationPtr;
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
    static _gaBlob blob; \
    blob.blobDomainPtr = domainStorage; \
    blob.blobCodomainPtr = codomainStorage; \
    blob.blobTempPopulationPtr = tempPopulationStorage; \
    blob.domainExtent = sizeof(domainType); \
    blob.codomainExtent = sizeof(codomainType); \
    blob.tempPopulationBufferLength = N; \
    _gaBlob_loc(&blob); \
})

/*static domainType tempPopulationStorage[N];
void __tempPopulationStorage_Set(unsigned int index, domainType value) { 
    tempPopulationStorage[index] = value; 
} 
unsigned int __tempPopulationStorage_Size() { 
    return N; 
} 
void* __tempPopulationStorage_Get(unsigned int index) { 
    return (void*)(tempPopulationStorage + index); 
} */