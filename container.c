#include "container.h"

struct node
{
    void *value;
    struct node *prev;
    struct node *next;
};

struct container
{
    struct node *head;
    struct node *tail;
    unsigned short size;
    uint64_t elem_size;
};

struct Iterator
{
    struct node *element;
};

Iterator *iterator_begin(container *container)
{
    Iterator *iterator = malloc(sizeof(Iterator));
    if (!iterator)
    {
        fprintf(stderr, "Не удалось выделить память под итератор\n");
        exit(1);
    }
    iterator->element = container->head;
    return iterator;
}

Iterator *iterator_end(container *container)
{
    Iterator *iterator = malloc(sizeof(Iterator));
    if (!iterator)
    {
        fprintf(stderr, "Не удалось выделить память под итератор\n");
        exit(1);
    }
    iterator->element = container->tail;
    return iterator;
}

void iterator_destroy(Iterator *iterator)
{
    free(iterator);
}

void iterator_next(Iterator *iterator)
{
    if (iterator && iterator->element)
    {
        iterator->element = iterator->element->next;
    }
}

void iterator_prev(Iterator *iterator)
{
    if (iterator && iterator->element)
    {
        iterator->element = iterator->element->prev;
    }
}

int iterator_is_valid(const Iterator *iterator)
{
    return iterator != NULL && iterator->element != NULL;
}

void *iterator_get_value(Iterator *iterator)
{
    if (iterator_is_valid(iterator))
    {
        return iterator->element->value;
    }
    return NULL;
}

struct node *create_node(const void *value, uint64_t elem_size)
{
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL)
    {
        fprintf(stderr, "Не удалось выделить память\n");
        exit(1);
    }
    node->value = malloc(elem_size);
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

void container_delete_back(struct container *container)
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
        struct node *old_tail = container->tail;
        struct node *new_tail = container->tail->prev;
        container->tail = new_tail;
        container->tail->next = NULL;
        free(old_tail->value);
        free(old_tail);
    }
    container->size--;
}

void container_clear(struct container *container)
{
    struct node *current = container->head;
    while (current != NULL)
    {
        struct node *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    container->head = NULL;
    container->tail = NULL;
    container->size = 0;
}

unsigned short container_get_size(const container *container)
{
    return container->size;
}

void *container_get_index(container *c, unsigned short index)
{
    if (!c || index >= c->size)
    {
        fprintf(stderr, "Слишком большой индекс\n");
        exit(1);
    }

    Iterator *iterator = iterator_begin(c);
    for (unsigned short i = 0; i < index; ++i)
    {
        iterator_next(iterator);
    }

    void *value = iterator_get_value(iterator);
    iterator_destroy(iterator);
    return value;
}