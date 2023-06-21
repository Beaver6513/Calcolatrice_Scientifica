#include "add_function.h"
#include "tree.h"
#include "memory.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void add_function(memory* mem) {
    tree* function = (tree*)malloc(sizeof(tree));
    struct string* str = (struct string*)malloc(sizeof(struct string));

    create_string(str);
    create_tree(function);

    system("clear");
    printf("Insert function: ");
    scan_string(str);
    fix(str);
    modify(str, '[', '(');
    modify(str, ']', ')');
    delete(str, ' ');
    expand_string(str);
    splice(str);
    group_string(str);
    load_tree(function, str);
    delete_string(str);
    int pos = add_tree(mem, function);
    printf("\nAdded Function in position: %d\nPress enter to return to main menu...", pos + 1);
    int k;
    while ((k = getchar()) != '\n' && k != EOF);
}