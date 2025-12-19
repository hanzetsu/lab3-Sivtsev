#include "container.h"
#include "log.h"
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
    if (iterator == NULL)
    {
        logg(1, "Не удалось выделить память под итератор\n");
    }
    iterator->element = container->head;
    return iterator;
}

Iterator *iterator_end(container *container)
{
    Iterator *iterator = malloc(sizeof(Iterator));
    if (iterator == NULL)
    {
        logg(1, "Не удалось выделить память под итератор\n");
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

static struct node *create_node(const void *value, uint64_t elem_size)
{
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL)
    {
        logg(1, "Не удалось выделить память\n");
    }
    node->value = malloc(elem_size);
    if (node->value == NULL)
    {
        logg(1, "Не удалось выделить память\n");
    }
    memcpy(node->value, value, elem_size);
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

container *container_create(uint64_t elem_size)
{
    container *c = malloc(sizeof(container));
    if (c == NULL)
    {
        logg(1, "Не удалось выделить память под контейнер\n");
    }
    container_init(c, elem_size);
    return c;
}

void container_destroy(container *c)
{
    if (!c)
        return;
    container_clear(c);
    free(c);
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
        logg(1, "Нет элементов в контейнере\n");
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
        logg(1, "Нет элементов в контейнере\n");
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

void *container_get_index(container *container, unsigned short index)
{
    if (!container || index >= container->size)
    {
        logg(1, "Слишком большой индекс\n");
    }

    Iterator *iterator = iterator_begin(container);
    for (unsigned short i = 0; i < index; ++i)
    {
        iterator_next(iterator);
    }

    void *value = iterator_get_value(iterator);
    iterator_destroy(iterator);
    return value;
}

static void swap_nodes(struct container *c, struct node *a, struct node *b)
{
    if (a == b)
        return;

    if (b->next == a)
    {
        struct node *tmp = a;
        a = b;
        b = tmp;
    }

    if (a->next == b)
    {
        struct node *a_prev = a->prev;
        struct node *b_next = b->next;

        if (a_prev)
            a_prev->next = b;
        else
            c->head = b;
        if (b_next)
            b_next->prev = a;
        else
            c->tail = a;

        b->prev = a_prev;
        a->next = b_next;

        b->next = a;
        a->prev = b;
        return;
    }

    struct node *a_prev = a->prev;
    struct node *a_next = a->next;
    struct node *b_prev = b->prev;
    struct node *b_next = b->next;

    if (a_prev)
        a_prev->next = b;
    else
        c->head = b;
    if (a_next)
        a_next->prev = b;
    else
        c->tail = b;

    if (b_prev)
        b_prev->next = a;
    else
        c->head = a;
    if (b_next)
        b_next->prev = a;
    else
        c->tail = a;

    a->prev = b_prev;
    a->next = b_next;

    b->prev = a_prev;
    b->next = a_next;
}

void container_swap(container *cont, unsigned short i, unsigned short j)
{
    struct container *c = (struct container *)cont;
    if (!c || i >= c->size || j >= c->size)
    {
        logg(0, "Нет таких индексов\n");
        return;
    }
    if (i == j)
        return;

    if (i > j)
    {
        unsigned short t = i;
        i = j;
        j = t;
    }

    struct node *node_i = c->head;
    struct node *node_j = c->head;
    for (unsigned short k = 0; k < i; ++k)
        node_i = node_i->next;
    for (unsigned short k = 0; k < j; ++k)
        node_j = node_j->next;

    swap_nodes(c, node_i, node_j);
}

void container_insert_at(struct container *c, unsigned short index, const void *value)
{
    if (index > c->size)
    {
        logg(1, "Индекс слишком большой\n");
    }

    if (index == 0)
    {
        container_push_front(value, c);
        return;
    }
    if (index == c->size)
    {
        container_push_back(value, c);
        return;
    }

    struct node *new_node = create_node(value, c->elem_size);
    struct node *current = c->head;
    for (unsigned short i = 0; i < index; ++i)
    {
        current = current->next;
    }

    new_node->prev = current->prev;
    new_node->next = current;
    current->prev->next = new_node;
    current->prev = new_node;
    c->size++;
}

void container_delete_at(struct container *c, unsigned short index)
{
    if (index >= c->size)
    {
        logg(1, "Индекс слишком большой\n");
    }

    if (index == 0)
    {
        container_delete_front(c);
        return;
    }
    if (index == c->size - 1)
    {
        container_delete_back(c);
        return;
    }

    struct node *current = c->head;
    for (unsigned short i = 0; i++ < index; current = current->next)

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current->value);
    free(current);
    c->size--;
}

void container_from_array(struct container *c, const void *array, unsigned short count)
{
    const char *data = (const char *)array;
    for (unsigned short i = 0; i < count; ++i)
    {
        container_push_back(data + i * c->elem_size, c);
    }
}

void container_to_array(const struct container *c, void *array)
{
    char *data = (char *)array;
    struct node *current = c->head;
    unsigned short i = 0;
    while (current != NULL)
    {
        memcpy(data + i * c->elem_size, current->value, c->elem_size);
        current = current->next;
        i++;
    }
}