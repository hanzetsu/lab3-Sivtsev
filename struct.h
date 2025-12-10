#pragma once
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING_LEN 100

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
    int trash_chute;
    unsigned short number_of_apartments;
    unsigned short number_of_floors;
    float average_apartment_area;
};

int comparator(const void *a, const void *b);

struct house generate_random_house(void);