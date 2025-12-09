#pragma once
#include <stdint.h>
#include "container.h"

void container_sort(struct container *cont, int (*cmp)(const void *, const void *), int descending);