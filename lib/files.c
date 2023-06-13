#include "files.h"
#include "memory.h"
#include "char_list.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

bool file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}

int save_mem(struct memory mem) {
    FILE* file = fopen("functions.txt", "w");
    struct memory_node* mem_index = mem.head;

    while(mem_index != NULL) {
        struct string* string = (struct string*)malloc(sizeof(struct string));
        create_string(string);

        inorder_i(mem_index->data->tree_head, string);
        string->head->previous = NULL;
        string->tail->next = NULL;
        modify(string, '[', '(');
        modify(string, ']', ')');
        delete(string, ' ');

        struct c_node* list_index = string->head;
        while(list_index != NULL) {
            putc(list_index->character, file);
            list_index = list_index->next;
        }
        putc('\n', file);

        mem_index = mem_index->next;
        delete_string(string);
    }

    fclose(file);
    return 0;
}

int load_mem(struct memory* mem) {
    if(!file_exists("functions.txt")) {
        return 0;
    }
    FILE* file = fopen("functions.txt", "r");
    char c;

    c = fgetc(file);
    if (c == EOF) {
        fclose(file);
        return 1;
    }
    ungetc(c, file);

    while (c != '\n' || c != EOF) {
        struct tree* function = (struct tree*)malloc(sizeof(struct tree));
        struct string* str = (struct string*)malloc(sizeof(struct string));

        create_string(str);
        create_tree(function);

        do {
            c = fgetc(file);
            if(c == '\n') {
                break;
            } else if(c == EOF) {
                delete_string(str);
                remove_tree(function);
                return 0;
            }
                
            tail_string_insert(str, c);
        } while (c != '\n' || c != EOF);

        str->head->previous = NULL;
        str->tail->next = NULL;
        modify(str, '[', '(');
        modify(str, ']', ')');
        delete(str, ' ');
        expand_string(str);
        splice(str);
        group_string(str);
        load_tree(function, str);
        add_tree(mem, function);
        delete_string(str);
    }
    
    

    fclose(file);
    return 0;
}