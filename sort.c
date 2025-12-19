#include "sort.h"

void container_sort(struct container *cont, int (*cmp)(const void *, const void *))
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
            if (cmp_result < 0)
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
                     int (*cmp)(const void *, const void *))
{
    void *pivot = container_get_index(cont, high);
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        void *current = container_get_index(cont, j);
        int cmp_result = cmp(current, pivot);
        
        if (cmp_result <= 0)  // Убрали проверку descending
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
                                 int (*cmp)(const void *, const void *))
{
    if (low < high)
    {
        int i = partition(cont, low, high, cmp);
        quick_sort_recursive(cont, low, i - 1, cmp);
        quick_sort_recursive(cont, i + 1, high, cmp);
    }
}

void container_sort_quick(struct container *cont, int (*cmp)(const void *, const void *))
{
    unsigned short size = container_get_size(cont);
    if (size <= 1)
    {
        return;
    }

    quick_sort_recursive(cont, 0, size - 1, cmp);
}