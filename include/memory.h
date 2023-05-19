#pragma once

#include "tree.h"

struct memory {
    struct memory_node* head;
    struct memory_node* tail;
};

struct memory_node {
    struct tree* data;
    struct memory_node* next;
    struct memory_node* previous;
};

int create_memory(struct memory* mem);

int free_memory(struct memory* mem);

int remove_mem_node(struct memory* mem, struct memory_node* target);

int add_tree(struct memory* mem, struct tree* out_tree);