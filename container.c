#include "container.h"

struct node *create_node (const void *value, uint64_t elem_size) {
    struct node *node = malloc (sizeof(struct node));
    if (node == NULL) {
        fprintf (stderr, "Не удалось выделить память\n");
        exit(1);
    }
    node->value = malloc (sizeof(elem_size));
    if (node->value == NULL) {
        fprintf (stderr, "Не удалось выделить память\n");
        exit(1);
    }
    value = memcpy (node->value, value, elem_size);
    node->prev = NULL;
    node->next = NULL;
    return node;    
}

void container_init (struct container *container, uint64_t elem_size) {
    container->head = NULL;
    container->tail = NULL;
    container->elem_size = elem_size;
    container->size = 0;
}