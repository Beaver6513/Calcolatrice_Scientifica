#include "files.h"
#include "memory.h"
#include "char_list.h"
#include "parser.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

bool file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}

int save_mem(memory mem) {
    FILE* file = fopen("functions.txt", "w");
    memory_node* mem_index = mem.head;

    while(mem_index != NULL) {
        string* str = (string*)malloc(sizeof(string));
        create_string(str);

        inorder_i(get_tree_head_from_mem_index(mem_index), str);
        fix(str);
        modify(str, '[', '(');
        modify(str, ']', ')');
        delete(str, ' ');

        c_node* list_index = get_head_str(str);
        while(list_index != NULL) {
            putc(get_char(list_index), file);
            to_next_list(list_index);
        }
        putc('\n', file);

        to_next_mem(&mem_index);
        delete_string(str);
    }

    fclose(file);
    return 0;
}

int load_mem(memory* mem) {
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
        tree* function = (tree*)malloc(sizeof(tree));
        string* str = (string*)malloc(sizeof(string));

        create_string(str);
        create_tree(function);

        do {
            c = fgetc(file);
            if(c == '\n') {
                break;
            } else if(c == EOF) {
                delete_string(str);
                remove_tree(function);
                fclose(file);
                return 0;
            }
                
            tail_string_insert(str, c);
        } while (c != '\n' || c != EOF);

        fix(str);
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