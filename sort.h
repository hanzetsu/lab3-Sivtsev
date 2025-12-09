#pragma once
#include <stdint.h>
#include "container.h"

typedef int (*comparator)(const void *, const void *);

void container_sort(struct container *cont, comparator cmp, int descending);