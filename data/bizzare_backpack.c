#include "bizzare_backpack.h"
/*
    1 	Toporek,32252,68674
    2 	Moneta z brązu,225790,471010
    3 	Korona,468164,944620
    4 	Diamentowy posążek,489494,962094
    5 	Szmaragdowy pas,35384,78344
    6 	Skamieliny,265590,579152
    7 	Złota moneta,497911,902698
    8 	Hełm,800493,1686515
    9 	Tusz,823576,1688691
    10 	Szkatułka,552202,1056157
    11 	Nóż,323618,677562
    12 	Długi miecz,382846,833132
    13 	Maska,44676,99192
    14 	Naszyjnik,169738,376418
    15 	Opalowa zawieszka,610876,1253986
    16 	Perły,854190,1853562
    17 	Kołczan,671123,1320297
    18 	Rubinowy pierścień,698180,1301637
    19 	Srebrna bransoletka,446517,859835
    20 	Czasomierz,909620,1677534
    21 	Mundur,904818,1910501
    22 	Trucizna,730061,1528646
    23 	Wełniany szal,931932,1827477
    24 	Kusza,952360,2068204
    25 	Stara księga,926023,1746556
    26 	Puchar z cynku,978724,2100851
*/

const unsigned int BIZZARE_BACKPACK_MAX_WEIGHT = 6404180;
const unsigned int BIZZARE_BACKPACK_AVAILABLE = 26;

unsigned int weights[BIZZARE_BACKPACK_AVAILABLE] = {
    32252,
    225790,
    468164,
    489494,
    35384,
    265590,
    497911,
    800493,
    823576,
    552202,
    323618,
    382846,
    44676,
    169738,
    610876,
    854190,
    671123,
    698180,
    446517,
    909620,
    904818,
    730061,
    931932,
    952360,
    926023,
    978724
};

unsigned int values[BIZZARE_BACKPACK_AVAILABLE] = {
    68674,
    471010,
    944620,
    962094,
    78344,
    579152,
    902698,
    1686515,
    1688691,
    1056157,
    677562,
    833132,
    99192,
    376418,
    1253986,
    1853562,
    1320297,
    1301637,
    859835,
    1677534,
    1910501,
    1528646,
    1827477,
    2068204,
    1746556,
    2100851
};

char* names[BIZZARE_BACKPACK_AVAILABLE] = {
    "Toporek",
    "Moneta z brązu",
    "Korona",
    "Diamentowy posążek",
    "Szmaragdowy pas",
    "Skamieliny",
    "Złota moneta",
    "Hełm",
    "Tusz",
    "Szkatułka",
    "Nóż",
    "Długi miecz",
    "Maska",
    "Naszyjnik",
    "Opalowa zawieszka",
    "Perły",
    "Kołczan",
    "Rubinowy pierścień",
    "Srebrna bransoletka",
    "Czasomierz",
    "Mundur",
    "Trucizna",
    "Wełniany szal",
    "Kusza",
    "Stara księga",
    "Puchar z cynku"
};

static backpack_t BACKPACK;
backpack_t _init_data() {
    BACKPACK.available_weights = weights;
    BACKPACK.available_values = values;
    BACKPACK.available_names = names;
    BACKPACK.num_available = BIZZARE_BACKPACK_AVAILABLE;
    BACKPACK.backpack_capacity = BIZZARE_BACKPACK_MAX_WEIGHT;
}

backpack_t get_bizzare_backpack() {
    _init_data();
    return BACKPACK;
} 