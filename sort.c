#include "sort.h"

void container_sort(struct container *cont, int (*cmp)(const void *, const void *), int descending)
{
    unsigned short size = container_get_size(cont);
    if (size <= 1)
        return;
    for (unsigned short i = 1; i < size; ++i)
    {
        unsigned short j = i;
        while (j > 0)
        {
            void *val_j = container_get_index(cont, j);
            void *val_j_minus_1 = container_get_index(cont, j - 1);

            int cmp_result = cmp(val_j, val_j_minus_1);
            if ((cmp_result < 0 && !descending) || (cmp_result > 0 && descending))
            {
                container_swap(cont, j, j - 1);
            }
            else
            {
                break;
            }
            --j;
        }
    }
}

static int partition(struct container *cont, int low, int high,
                     int (*cmp)(const void *, const void *), int descending)
{
    void *pivot = container_get_index(cont, high);
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        void *current = container_get_index(cont, j);
        int cmp_result = cmp(current, pivot);
        int should_swap;

        if (descending == 0)
        {
            if (cmp_result <= 0)
            {
                should_swap = 1;
            }
            else
            {
                should_swap = 0;
            }
        }
        else
        {
            if (cmp_result >= 0)
            {
                should_swap = 1;
            }
            else
            {
                should_swap = 0;
            }
        }

        if (should_swap == 1)
        {
            i++;
            if (i != j)
            {
                container_swap(cont, i, j);
            }
        }
    }

    container_swap(cont, i + 1, high);
    return i + 1;
}

static void quick_sort_recursive(struct container *cont, int low, int high,
                                 int (*cmp)(const void *, const void *), int descending)
{
    if (low < high)
    {
        int pi = partition(cont, low, high, cmp, descending);
        quick_sort_recursive(cont, low, pi - 1, cmp, descending);
        quick_sort_recursive(cont, pi + 1, high, cmp, descending);
    }
}

void container_sort_quick(struct container *cont, int (*cmp)(const void *, const void *), int descending)
{
    unsigned short size = container_get_size(cont);
    if (size <= 1)
    {
        return;
    }

    quick_sort_recursive(cont, 0, size - 1, cmp, descending);
}