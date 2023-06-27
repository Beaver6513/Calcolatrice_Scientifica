#include "delete_function.h"
#include "memory.h"
#include "char_list.h"
#include "tree_utils.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void delete_function(memory* mem) {
    int choice = 0;
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

            remove_mem_node(mem, index);
            printf("\nFunction deleted!\nPress enter to return to main menu...");
            getchar();
        break;
        case 2:
        {
            string* s_params = (string*)malloc(sizeof(string));
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
                modify(t_list, '-', '_');
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

            remove_mem_node(mem, index);
            printf("\nFunction deleted!\nPress enter to return to main menu...");
            getchar();
        }
        break;
        case 3:

        break;
        default:
            exit(1);
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}