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

    if (house_a->trash_chute > house_b->trash_chute)
    {
        return 1;
    }
    else if (house_a->trash_chute < house_b->trash_chute)
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

static enum type_of_house random_house_type(void)
{
    return rand() % 3;
}

struct house generate_random_house(void)
{
    struct house h;
    h.name_of_the_developer = strdup(developers[rand() % 5]);
    h.name_of_microdistrict = strdup(districts[rand() % 5]);
    h.house_type = random_house_type();
    h.year_built = 1950 + rand() % 55;
    h.elevator = rand() % 2;
    h.trash_chute = rand() % 2;
    h.number_of_apartments = 50 + rand() % 451;
    h.number_of_floors = 5 + rand() % 26;
    h.average_apartment_area = 20 + (rand() % 6000) / 100.0f;
    return h;
}
