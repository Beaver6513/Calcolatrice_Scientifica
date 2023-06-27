#include "search_function.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "tree_utils.h"
#include "parser.h"

void search_function(memory* mem) {
    int choice = 0;
    while(choice < 1 || choice > 2) {
        system("clear");
        printf("Search Method:\n1) Bt string\n2) Return to main menu\n\n");
        if((choice < 1 || choice > 2) && choice != 0) {
            printf("Select a valid number!\n\n");
        }
        printf("Choice: ");
        scanf("%d", &choice);
    }
    switch(choice) {
        case 1:
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

            printf("\nPress enter to return to main menu...");
            break;
        }
        case 2:

        break;
        default:
            exit(1);
    }
    int k;
    while ((k = getchar()) != '\n' && k != EOF);
}