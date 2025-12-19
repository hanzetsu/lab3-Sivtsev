#pragma once
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING_LEN 100
#define year_built_min 1950
#define number_of_apartments_min 50
#define number_of_floors_min 5
#define average_apartment_area_min 20
enum type_of_house
{
    panel,
    brick,
    monolithic
};

struct house
{

    char name_of_the_developer[MAX_STRING_LEN];
    char name_of_microdistrict[MAX_STRING_LEN];
    enum type_of_house house_type;
    unsigned short year_built;
    int elevator;
    int trash;
    unsigned short number_of_apartments;
    unsigned short number_of_floors;
    float average_apartment_area;
};

int comparator_asc(const void *a, const void *b);
int comparator_desc(const void *a, const void *b);

struct house generate_random_house(void);