#pragma once

#include "tree.h"

struct memory_node {
    tree* data;
    struct memory_node* next;
    struct memory_node* previous;
};
typedef struct memory_node memory_node; 

struct memory {
    memory_node* head;
    memory_node* tail;
};
typedef struct memory memory;

int create_memory(memory* mem);

int free_memory(memory* mem);

int remove_mem_node(memory* mem, memory_node* target);

int add_tree(memory* mem, tree* out_tree);

int move_index(memory_node** index, int pos);

int get_length(memory_node* index, memory mem, int* mem_length);

int to_next_mem(memory_node** index);

memory_node* get_head_mem(memory* mem);

tree_node* get_tree_head_from_mem_index(memory_node* index);

tree* get_tree_from_mem_index(memory_node* index);