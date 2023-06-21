#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int create_memory(memory* mem) {
    mem->head = NULL;
    mem->tail = NULL;
    return 0;
}

int to_next(memory_node* index) {
    index = index->next;
    return 0;
}

int move_index(memory_node* index, int pos) {
    for(int i = 0 ; i < (pos - 1) ; i++) {
        index = index->next;
    }
    return 0;
}

int get_length(memory_node* index, memory mem, int* mem_length) {
    while(index != mem.tail) {
        *mem_length = *mem_length + 1;
        index = index->next;
    }
    return 0;    
}

int add_tree(memory* mem, tree* out_tree) {
    memory_node* node = (memory_node*)malloc(sizeof(memory_node));
    node->next = NULL;
    node->data = out_tree;

    if(mem->head == NULL && mem->tail == NULL) {
        mem->head = node;
        mem->tail = node;
        node->previous = NULL;
    } else {
        mem->tail->next = node;
        node->previous = mem->tail;
        mem->tail = node;
    }

    int count = 0;
    memory_node* index = mem->head;
    while(index != mem->tail) {
        count++;
        index = index->next;
    }

    return count;
}

int remove_mem_node(memory* mem, memory_node* target) {
    remove_tree(target->data);
    if(target->next == NULL && target->previous == NULL) {
        mem->head = NULL;
        mem->tail = NULL;
    } else if(target->previous == NULL && target->next != NULL) {
        mem->head = mem->head->next;
        target->next->previous = NULL;
    } else if(target->previous != NULL && target->next == NULL) {
        mem->tail = mem->tail->previous;
        target->previous->next = NULL;
    } else {
        target->next->previous = target->previous;
        target->previous->next = target->next;
    }
    free(target);
    return 0;
}

int free_memory(memory* mem) {
    memory_node* i = mem->head;
    memory_node* j = mem->head;
    if(i != NULL) {
        do {
            j = j->next;
            if(j != NULL) j->previous = NULL;
            remove_mem_node(mem, i);
            mem->head = j;
            i = mem->head;
            if(j == NULL) mem->tail = j;
        } while(j != NULL);
    }
    return 0;
}