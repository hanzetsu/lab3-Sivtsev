#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct container container;
typedef struct Iterator Iterator;

Iterator *iterator_begin(container *c);
Iterator *iterator_end(container *c);

void iterator_next(Iterator *it);
void iterator_prev(Iterator *it);

int iterator_is_valid(const Iterator *it);

void *iterator_get_value(Iterator *it);

void iterator_destroy(Iterator *it);

void *container_get_index(container *c, unsigned short index);

void container_init(container *c, uint64_t elem_size);

void container_push_back(const void *value, container *c);
void container_push_front(const void *value, container *c);

void container_delete_front(container *c);
void container_delete_back(container *c);

void container_clear(container *c);

unsigned short container_get_size(const container *c);