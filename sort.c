#include "sort.h"

void container_sort(struct container *cont, int (*cmp)(const void *, const void *), int descending)
{
    unsigned short size = container_get_size(cont);
    if (size <= 1) return;
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
