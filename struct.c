#include "struct.h"

int comparator(const void *a, const void *b)
{
    const struct house *house_a = (const struct house *)a;
    const struct house *house_b = (const struct house *)b;

    if (house_a->year_built > house_b->year_built)
    {
        return 1;
    }
    else if (house_a->year_built < house_b->year_built)
    {
        return -1;
    }

    if (house_a->house_type > house_b->house_type)
    {
        return 1;
    }
    else if (house_a->house_type < house_b->house_type)
    {
        return -1;
    }

    if (house_a->elevator > house_b->elevator)
    {
        return 1;
    }
    else if (house_a->elevator < house_b->elevator)
    {
        return -1;
    }

    if (house_a->number_of_floors > house_b->number_of_floors)
    {
        return 1;
    }
    else if (house_a->number_of_floors < house_b->number_of_floors)
    {
        return -1;
    }

    if (house_a->number_of_apartments > house_b->number_of_apartments)
    {
        return 1;
    }
    else if (house_a->number_of_apartments < house_b->number_of_apartments)
    {
        return -1;
    }

    if (fabs(house_a->average_apartment_area - house_b->average_apartment_area) > 0.1)
    {
        if (house_a->average_apartment_area > house_b->average_apartment_area)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    if (house_a->trash > house_b->trash)
    {
        return 1;
    }
    else if (house_a->trash < house_b->trash)
    {
        return -1;
    }

    int cmp_microdistrict = strcmp(house_a->name_of_microdistrict, house_b->name_of_microdistrict);
    if (cmp_microdistrict != 0)
    {
        return cmp_microdistrict;
    }

    return strcmp(house_a->name_of_the_developer, house_b->name_of_the_developer);
}

static const char *developers[] = {"ПИК", "Самолёт", "ЛСР", "Донстрой", "Группа Эталон"};
static const char *districts[] = {"ЦАО", "САО", "ЮАО", "ЗАО", "ЮЗАО"};

struct house generate_random_house(void)
{
    struct house h;
    strncpy(h.name_of_the_developer, developers[rand() % 5], MAX_STRING_LEN - 1);
    h.name_of_the_developer[MAX_STRING_LEN - 1] = '\0';
    strncpy(h.name_of_microdistrict, districts[rand() % 5], MAX_STRING_LEN - 1);
    h.name_of_microdistrict[MAX_STRING_LEN - 1] = '\0';
    h.house_type = rand() % 3;
    h.year_built = year_built_min + rand() % 75;
    h.elevator = rand() % 2;
    h.trash = rand() % 2;
    h.number_of_apartments = number_of_apartments_min + rand() % 451;
    h.number_of_floors = number_of_floors_min + rand() % 26;
    h.average_apartment_area = average_apartment_area_min + (rand() % 6000) / 100.0f;
    return h;
}
