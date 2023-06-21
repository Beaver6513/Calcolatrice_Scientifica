#include "duplicate_function.h"
#include "memory.h"
#include "tree_utils.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void duplicate_function(memory* mem) {
    int choice = -1;
    int mem_lenght = 1;
    memory_node* index = mem->head;
    get_length(index, *mem, &mem_lenght);
    while(choice < 1 || choice > mem_lenght) {
        system("clear");

        if((choice < 0 || choice > mem_lenght) && choice != -1) {
            printf("Location unknown!\n\n");
        }
        printf("Insert the function index that you want to duplicate: ");
        scanf("%d", &choice);
    }
    index = mem->head;
    move_index(&index, choice);

    tree* function_2 = (tree*)malloc(sizeof(tree));
    create_tree(function_2);
    struct string* t_list = (struct string*)malloc(sizeof(struct string));
    create_string(t_list);
    inorder_i(get_parent(index), t_list);
    fix(t_list);
    splice(t_list);
    group_string(t_list);
    load_tree(function_2, t_list);
    delete_string(t_list);
    int pos = add_tree(mem, function_2);
    

    printf("\nFunction duplicated in position: %d\nPress enter to return to main menu...", pos + 1);
    getchar();
    getchar();
}