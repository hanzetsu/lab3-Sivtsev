#pragma once
#include <math.h>
#include <string.h>

enum type_of_house {panel,brick,monolithic};

struct house {
    
    char *name_of_the_developer;
    char *name_of_microdistrict;
    enum type_of_house house_type;
    unsigned short year_built;
    int elevator;
    int trash_chute;
    unsigned short number_of_apartments;
    unsigned short number_of_floors;
    float average_apartment_area;
};

int comparator(const void *a, const void *b);