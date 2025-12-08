#include "container.h"

struct node *create_node(const void *value, uint64_t elem_size)
{
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL)
    {
        fprintf(stderr, "Не удалось выделить память\n");
        exit(1);
    }
    node->value = malloc(sizeof(elem_size));
    if (node->value == NULL)
    {
        fprintf(stderr, "Не удалось выделить память\n");
        exit(1);
    }
    value = memcpy(node->value, value, elem_size);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void container_init(struct container *container, uint64_t elem_size)
{
    container->head = NULL;
    container->tail = NULL;
    container->elem_size = elem_size;
    container->size = 0;
}

void container_push_back(const void *value, struct container *container)
{
    struct node *node = create_node(value, container->elem_size);
    if (container->size == 0)
    {
        container->tail = node;
        container->head = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        container->tail->next = node;
        node->prev = container->tail;
        container->tail = node;
        node->next = NULL;
    }
    container->size++;
}

void container_push_front(const void *value, struct container *container)
{
    struct node *node = create_node(value, container->elem_size);

    if (container->size == 0)
    {
        container->head = node;
        container->tail = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        container->head->prev = node;
        node->next = container->head;
        container->head = node;
        node->prev = NULL;
    }
    container->size++;
}

void container_delete_front(struct container *container)
{
    if (container->size == 0)
    {
        fprintf(stderr, "Нет элементов в контейнере\n");
        exit(1);
    }
    if (container->size == 1)
    {
        struct node *p = container->head;
        container->head = NULL;
        container->tail = NULL;
        free(p->value);
        free(p);
    }
    else
    {
        struct node *old_head = container->head;
        struct node *new_head = container->head->next;
        new_head->prev = NULL;
        container->head = new_head;
        free(old_head->value);
        free(old_head);
    }
    container->size--;
}