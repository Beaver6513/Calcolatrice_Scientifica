#include "apply_operation.h"
#include "memory.h"
#include "tree_utils.h"
#include "parser.h"
#include "derivative.h"
#include <stdio.h>
#include <stdlib.h>

void apply_operation(memory* mem) {
    int choice = 0;
    while(choice < 1 || choice > 3) {
        system("clear");
        printf("Select operation:\n1) Function derivative\n2) Function in x\n3) Return to main menu\n\n");
        if((choice < 1 || choice > 3) && choice != 0) {
            printf("Select a valid number!\n\n");
        }
        printf("Choice: ");
        scanf("%d", &choice);
    }
    switch(choice) {
        case 1:
            system("clear");
            choice = 0;
            while(choice < 1 || choice > 3) {
                system("clear");
                printf("Search Method:\n1) By index\n2) By string\n3) Return to main menu\n\n");
                if((choice < 1 || choice > 3) && choice != 0) {
                    printf("Select a valid number!\n\n");
                }
                printf("Choice: ");
                scanf("%d", &choice);
            }
            switch(choice) {
                case 1:
                    choice = -1;
                    int mem_lenght = 1;
                    memory_node* index = get_head_mem(mem);
                    get_length(index, *mem, &mem_lenght);
                    while(choice < 1 || choice > mem_lenght) {
                        system("clear");

                        if((choice < 0 || choice > mem_lenght) && choice != -1) {
                            printf("Location unknown!\n\n");
                        }
                        printf("Insert function index: ");
                        scanf("%d", &choice);
                    }
                    index = get_head_mem(mem);
                    move_index(&index, choice);

                    struct string* t_string = (struct string*)malloc(sizeof(struct string));
                    create_string(t_string);
                    tree* t_tree = (tree*)malloc(sizeof(tree));
                    create_tree(t_tree);
                    tree* out_tree = (tree*)malloc(sizeof(tree));
                    create_tree(out_tree);

                    inorder_i(get_tree_head_from_mem_index(index), t_string);
                    fix(t_string);
                    modify(t_string, '[', '(');
                    modify(t_string, ']', ')');
                    delete(t_string, ' ');
                    expand_string(t_string);
                    splice(t_string);
                    group_string(t_string);
                    par_check(t_string);
                    load_tree(t_tree, t_string);
                    delete_from_tree(t_tree, '[');
                    delete_from_tree(t_tree, ']');

                    struct string* der_string = (struct string*)malloc(sizeof(struct string));
                    create_string(der_string);

                    get_func_der(t_tree, der_string);
                    fix(der_string);

                    while(mult_delete(der_string));
                    par_check(der_string);
                    modify(der_string, '-', '_');

                    printf("Function derivative :  ");
                    print_string(*der_string);
                    printf("\n");

                    fix(der_string);
                    modify(der_string, '[', '(');
                    modify(der_string, ']', ')');
                    delete(der_string, ' ');
                    splice(der_string);
                    group_string(der_string);
                    load_tree(out_tree, der_string);
                    int pos = add_tree(mem, out_tree);
                    

                    printf("\nResult added in position: %d\nPress enter to return to main menu...", pos + 1);

                    delete_string(der_string);
                    delete_string(t_string);
                    remove_tree(t_tree);
                    int k;
                    while ((k = getchar()) != '\n' && k != EOF);
                    getchar();
                break;
                case 2:
                {
                    struct string* s_params = (struct string*)malloc(sizeof(struct string));
                    create_string(s_params);

                    system("clear");
                    printf("Input search parameters :  ");
                    scan_string(s_params);
                    printf("\n\n");
                    fix(s_params);

                    memory_node* mem_index = get_head_mem(mem);
                    int func_found = 0;
                    int func_index = 1;
                    while (mem_index != NULL) {
                        struct string* t_list = (struct string*)malloc(sizeof(struct string));
                        create_string(t_list);
                        inorder_i(get_tree_head_from_mem_index(mem_index), t_list);
                        modify(t_list, '(', '[');
                        modify(t_list, ')', ']');
                        contract(t_list);

                        if (compare(s_params, t_list)) {
                            func_found++;
                            printf("Index: %d\nFunction :   ", func_index);
                            print_string(*t_list);
                            printf("\n");
                        }

                        to_next_mem(&mem_index);
                        func_index++;
                        delete_string(t_list);
                    }

                    if (func_found == 0) {
                        printf("No function found!");
                    }
                    delete_string(s_params);

                    choice = -1;
                    mem_lenght = 1;
                    index = get_head_mem(mem);
                    get_length(index, *mem, &mem_lenght);
                    while (choice < 1 || choice > mem_lenght) {

                        if ((choice < 0 || choice > mem_lenght) && choice != -1) {
                            printf("Location unknown!\n\n");
                        }
                        printf("Insert function index: ");
                        scanf("%d", &choice);
                    }
                    index = get_head_mem(mem);
                    move_index(&index, choice);

                    struct string* t_string = (struct string*)malloc(sizeof(struct string));
                    create_string(t_string);
                    tree* t_tree = (tree*)malloc(sizeof(tree));
                    create_tree(t_tree);
                    tree* out_tree = (tree*)malloc(sizeof(tree));
                    create_tree(out_tree);

                    inorder_i(get_tree_head_from_mem_index(index), t_string);
                    fix(t_string);
                    modify(t_string, '[', '(');
                    modify(t_string, ']', ')');
                    delete(t_string, ' ');
                    expand_string(t_string);
                    splice(t_string);
                    group_string(t_string);
                    par_check(t_string);
                    load_tree(t_tree, t_string);
                    delete_from_tree(t_tree, '[');
                    delete_from_tree(t_tree, ']');

                    struct string* der_string = (struct string*)malloc(sizeof(struct string));
                    create_string(der_string);

                    get_func_der(t_tree, der_string);
                    fix(der_string);
                    
                    while(mult_delete(der_string));
                    par_check(der_string);
                    modify(der_string, '-', '_');

                    printf("Function derivative :  ");
                    print_string(*der_string);
                    printf("\n");
                    fix(der_string);

                    modify(der_string, '[', '(');
                    modify(der_string, ']', ')');
                    delete(der_string, ' ');
                    splice(der_string);
                    group_string(der_string);
                    load_tree(out_tree, der_string);
                    int pos = add_tree(mem, out_tree);
                    

                    printf("\nResult added in position: %d\nPress enter to return to main menu...", pos + 1);

                    delete_string(der_string);
                    delete_string(t_string);
                    remove_tree(t_tree);
                    int k;
                    while ((k = getchar()) != '\n' && k != EOF);
                    getchar();
                }
                break;
                case 3:
                break;
                default:
                    exit(1);
            }
        break;
        case 2:
            system("clear");
            choice = 0;
            while(choice < 1 || choice > 3) {
                system("clear");
                printf("Search Method:\n1) By index\n2) By string\n3) Return to main menu\n\n");
                if((choice < 1 || choice > 3) && choice != 0) {
                    printf("Select a valid number!\n\n");
                }
                printf("Choice: ");
                scanf("%d", &choice);
            }
            switch(choice) {
                case 1:
                    choice = -1;
                    int mem_lenght = 1;
                    memory_node* index = get_head_mem(mem);
                    get_length(index, *mem, &mem_lenght);
                    while(choice < 1 || choice > mem_lenght) {
                        system("clear");

                        if((choice < 0 || choice > mem_lenght) && choice != -1) {
                            printf("Location unknown!\n\n");
                        }
                        printf("Insert function index: ");
                        scanf("%d", &choice);
                    }
                    index = get_head_mem(mem);
                    move_index(&index, choice);
                    printf("Insert x value: ");

                    struct string* x = (struct string*)malloc(sizeof(struct string));
                    create_string(x);
                    struct string* t_string = (struct string*)malloc(sizeof(struct string));
                    create_string(t_string);
                    tree* t_tree = (tree*)malloc(sizeof(tree));
                    create_tree(t_tree);

                    inorder_i(get_tree_head_from_mem_index(index), t_string);
                    fix(t_string);
                    modify(t_string, '[', '(');
                    modify(t_string, ']', ')');
                    delete(t_string, ' ');
                    expand_string(t_string);
                    splice(t_string);
                    group_string(t_string);
                    load_tree(t_tree, t_string);

                    scan_string(x);
                    modify(x, '_', '-');

                    double res = get_func_in_x(t_tree, x);

                    printf("\n\nFunction in point x is :  %g\n\n", res);
                    printf("Press enter to return to main menu...");
                    delete_string(x);
                    delete_string(t_string);
                    remove_tree(t_tree);
                    int k;
                    while ((k = getchar()) != '\n' && k != EOF);
                break;
                case 2:
                {
                    struct string* s_params = (struct string*)malloc(sizeof(struct string));
                    create_string(s_params);

                    system("clear");
                    printf("Input search parameters :  ");
                    scan_string(s_params);
                    printf("\n\n");
                    fix(s_params);

                    memory_node* mem_index = get_head_mem(mem);
                    int is_equal = 0;
                    int func_found = 0;
                    int func_index = 1;
                    while (mem_index != NULL) {
                        struct string* t_list = (struct string*)malloc(sizeof(struct string));
                        create_string(t_list);
                        inorder_i(get_tree_head_from_mem_index(mem_index), t_list);
                        modify(t_list, '(', '[');
                        modify(t_list, ')', ']');
                        contract(t_list);

                        is_equal = compare(s_params, t_list);
                        if (is_equal == 1) {
                            func_found++;
                            printf("Index: %d\nFunction :   ", func_index);
                            print_string(*t_list);
                            printf("\n");
                        }

                        to_next_mem(&mem_index);
                        func_index++;
                        delete_string(t_list);
                    }

                    if (func_found == 0) {
                        printf("No function found!");
                    }
                    delete_string(s_params);


                    choice = -1;
                    mem_lenght = 1;
                    index = get_head_mem(mem);
                    get_length(index, *mem, &mem_lenght);
                    while (choice < 1 || choice > mem_lenght) {

                        if ((choice < 0 || choice > mem_lenght) && choice != -1) {
                            printf("Location unknown!\n\n");
                        }
                        printf("Insert function index: ");
                        scanf("%d", &choice);
                    }
                    index = get_head_mem(mem);
                    move_index(&index, choice);

                    printf("Insert x value: ");

                    struct string* x2 = (struct string*)malloc(sizeof(struct string));
                    create_string(x2);
                    struct string* t_string2 = (struct string*)malloc(sizeof(struct string));
                    create_string(t_string2);
                    tree* t_tree2 = (tree*)malloc(sizeof(tree));
                    create_tree(t_tree2);

                    inorder_i(get_tree_head_from_mem_index(index), t_string2);
                    fix(t_string2);
                    modify(t_string2, '[', '(');
                    modify(t_string2, ']', ')');
                    delete(t_string2, ' ');
                    expand_string(t_string2);
                    splice(t_string2);
                    group_string(t_string2);
                    load_tree(t_tree2, t_string2);

                    scan_string(x2);
                    modify(x2, '_', '-');

                    res = get_func_in_x(t_tree2, x2);

                    printf("\n\nFunction in point x is :  %g\n\n", res);
                    printf("Press enter to return to main menu...");
                    delete_string(x2);
                    delete_string(t_string2);
                    remove_tree(t_tree2);
                    int k;
                    while ((k = getchar()) != '\n' && k != EOF);
                }
                break;
                case 3:

                break;
                default:
                    exit(1);
            }
        break;
        case 3:
        break;
        default:
            exit(1);
    }
}