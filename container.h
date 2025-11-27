#pragma once
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>   
#include <stdint.h>

struct node {
    void *value;         
    struct node *prev;     
    struct node *next;     
};

struct container {
    struct node *head;     
    struct node *tail;     
    unsigned short size;   
    uint64_t elem_size;      
};