#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int create_memory(struct memory* mem) {
    mem->head = NULL;
    mem->tail = NULL;
    return 0;
}

int add_tree(struct memory* mem, struct tree* out_tree) {
    struct memory_node* node = (struct memory_node*)malloc(sizeof(struct memory_node));
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
    struct memory_node* index = mem->head;
    while(index != mem->tail) {
        count++;
        index = index->next;
    }

    return count;
}

int remove_mem_node(struct memory* mem, struct memory_node* target) {
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

int free_memory(struct memory* mem) {
    struct memory_node* i = mem->head;
    struct memory_node* j = mem->head;
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