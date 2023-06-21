#include "char_list.h"
#include "memory.h"
#include "parser.h"
#include "tree.h"
#include "tree_utils.h"
#include "derivative.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__ 
void clear_screen() {
	system("clear");
}
#elif _WIN32
void clear_screen() {
	system("cls");
}
#else

#endif

int main() {
    int choice = 0;
    memory mem;
    create_memory(&mem);
    load_mem(&mem);
    
    while(1) {
        //Resets
        clear_screen();
        choice = 0;
        //MAIN CHOICE INPUT---------
        while(choice < 1 || choice > 7) {
            clear_screen();
            printf("Solver V1.0\n\n");
            printf("1) Add function to memory\n2) Delete function from memory\n3) Search function in memory\n4) Duplicate function\n5) Print function memory\n6) Select operation\n7) Exit\n\n");
            if(choice == 8) {
                printf("Memory is empty!\n\n");
                choice = 0;
            }
            if((choice < 1 || choice > 7) && choice != 0) {
                printf("Select a valid number!\n\n");
            }
            printf("Choice: ");
            scanf("%d", &choice);
            if(mem.head == NULL && (choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6)) choice = 8;
        }
        switch(choice) {
            case 1:
            {
                tree* function = (tree*)malloc(sizeof(tree));
                struct string* str = (struct string*)malloc(sizeof(struct string));

                create_string(str);
                create_tree(function);

                clear_screen();
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
                int pos = add_tree(&mem, function);
                printf("\nAdded Function in position: %d\nPress enter to return to main menu...", pos + 1);
                int k;
                while ((k = getchar()) != '\n' && k != EOF);
            }
            break;
            case 2:
            {
                choice = 0;
                while(choice < 1 || choice > 3) {
                    clear_screen();
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
                        memory_node* index = mem.head;
                        get_length(index, mem, &mem_lenght);
                        while(choice < 1 || choice > mem_lenght) {
                            clear_screen();

                            if((choice < 0 || choice > mem_lenght) && choice != -1) {
                                printf("Location unknown!\n\n");
                            }
                            printf("Insert function index: ");
                            scanf("%d", &choice);
                        }
                        index = mem.head;
                        move_index(&index, choice);

                        remove_mem_node(&mem, index);
                        printf("\nFunction deleted!\nPress enter to return to main menu...");
                        getchar();
                    break;
                    case 2:
                    {
                        struct string* s_params = (struct string*)malloc(sizeof(struct string));
                        create_string(s_params);

                        clear_screen();
                        printf("Input search parameters :  ");
                        scan_string(s_params);
                        printf("\n\n");
                        fix(s_params);

                        memory_node* mem_index = mem.head;
                        int is_equal = 0;
                        int func_found = 0;
                        int func_index = 1;
                        while (mem_index != NULL) {
                            struct string* t_list = (struct string*)malloc(sizeof(struct string));
                            create_string(t_list);
                            inorder_i(get_parent(mem_index), t_list);
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

                            to_next(&mem_index);
                            func_index++;
                            delete_string(t_list);
                        }

                        if (func_found == 0) {
                            printf("No function found!");
                        }
                        delete_string(s_params);


                        choice = -1;
                        mem_lenght = 1;
                        index = mem.head;
                        get_length(index, mem, &mem_lenght);
                        while (choice < 1 || choice > mem_lenght) {

                            if ((choice < 0 || choice > mem_lenght) && choice != -1) {
                                printf("Location unknown!\n\n");
                            }
                            printf("Insert function index: ");
                            scanf("%d", &choice);
                        }
                        index = mem.head;
                        move_index(&index, choice);

                        remove_mem_node(&mem, index);
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
            break;
            case 3:
                choice = 0;
                while(choice < 1 || choice > 2) {
                    clear_screen();
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

                        clear_screen();
                        printf("Input search parameters :  ");
                        scan_string(s_params);
                        printf("\n\n");
                        fix(s_params);

                        memory_node* mem_index = mem.head;
                        int is_equal = 0;
                        int func_found = 0;
                        int func_index = 1;
                        while (mem_index != NULL) {
                            struct string* t_list = (struct string*)malloc(sizeof(struct string));
                            create_string(t_list);
                            inorder_i(get_parent(mem_index), t_list);
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

                            to_next(&mem_index);
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
            break;
            case 4:
                choice = -1;
                int mem_lenght = 1;
                memory_node* index = mem.head;
                get_length(index, mem, &mem_lenght);
                while(choice < 1 || choice > mem_lenght) {
                    clear_screen();

                    if((choice < 0 || choice > mem_lenght) && choice != -1) {
                        printf("Location unknown!\n\n");
                    }
                    printf("Insert the function index that you want to duplicate: ");
                    scanf("%d", &choice);
                }
                index = mem.head;
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
                int pos = add_tree(&mem, function_2);
                

                printf("\nFunction duplicated in position: %d\nPress enter to return to main menu...", pos + 1);
                getchar();
                getchar();
            break;
            case 5:
                choice = 0;
                while(choice < 1 || choice > 3) {
                    clear_screen();
                    printf("Print Method:\n1) By index\n2) All of memory\n3) Return to main menu\n\n");
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
                        memory_node* index = mem.head;
                        get_length(index, mem, &mem_lenght);
                        while(choice < 1 || choice > mem_lenght) {
                            clear_screen();

                            if((choice < 0 || choice > mem_lenght) && choice != -1) {
                                printf("Location unknown!\n\n");
                            }
                            printf("Insert function index: ");
                            scanf("%d", &choice);
                        }
                        printf("\n");
                        index = mem.head;
                        move_index(&index, choice + 1);
                        printf("Function :   ");
                        print_tree(index);
                        int k;
                        while ((k = getchar()) != '\n' && k != EOF);
                        printf("\nPress enter to return to main menu...");
                    break;
                    case 2:
                        index = mem.head;
                        int count = 1;
                        clear_screen();
                        while(index != NULL) {
                            printf("Index : %d\n", count);
                            printf("Function :   ");
                            print_tree(index);
                            printf("\n");
                            to_next(&index);
                            count++;
                        }
                        while ((k = getchar()) != '\n' && k != EOF);
                        printf("\nPress enter to return to main menu...");
                    break;
                    case 3:
                    break;
                    default:
                        exit(1);
                }
                getchar();

            break;
            case 6:
                choice = 0;
                while(choice < 1 || choice > 3) {
                    clear_screen();
                    printf("Select operation:\n1) Function derivative\n2) Function in x\n3) Return to main menu\n\n");
                    if((choice < 1 || choice > 3) && choice != 0) {
                        printf("Select a valid number!\n\n");
                    }
                    printf("Choice: ");
                    scanf("%d", &choice);
                }
                switch(choice) {
                    case 1:
                        clear_screen();
                        choice = 0;
                        while(choice < 1 || choice > 3) {
                            clear_screen();
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
                                memory_node* index = mem.head;
                                get_length(index, mem, &mem_lenght);
                                while(choice < 1 || choice > mem_lenght) {
                                    clear_screen();

                                    if((choice < 0 || choice > mem_lenght) && choice != -1) {
                                        printf("Location unknown!\n\n");
                                    }
                                    printf("Insert function index: ");
                                    scanf("%d", &choice);
                                }
                                index = mem.head;
                                move_index(&index, choice);

                                struct string* t_string = (struct string*)malloc(sizeof(struct string));
                                create_string(t_string);
                                tree* t_tree = (tree*)malloc(sizeof(tree));
                                create_tree(t_tree);
                                tree* out_tree = (tree*)malloc(sizeof(tree));
                                create_tree(out_tree);

                                inorder_i(get_parent(index), t_string);
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
                                int pos = add_tree(&mem, out_tree);
                                

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

                                clear_screen();
                                printf("Input search parameters :  ");
                                scan_string(s_params);
                                printf("\n\n");
                                fix(s_params);

                                memory_node* mem_index = mem.head;
                                int func_found = 0;
                                int func_index = 1;
                                while (mem_index != NULL) {
                                    struct string* t_list = (struct string*)malloc(sizeof(struct string));
                                    create_string(t_list);
                                    inorder_i(get_parent(mem_index), t_list);
                                    modify(t_list, '(', '[');
                                    modify(t_list, ')', ']');
                                    contract(t_list);

                                    if (compare(s_params, t_list)) {
                                        func_found++;
                                        printf("Index: %d\nFunction :   ", func_index);
                                        print_string(*t_list);
                                        printf("\n");
                                    }

                                    to_next(&mem_index);
                                    func_index++;
                                    delete_string(t_list);
                                }

                                if (func_found == 0) {
                                    printf("No function found!");
                                }
                                delete_string(s_params);

                                choice = -1;
                                mem_lenght = 1;
                                index = mem.head;
                                get_length(index, mem, &mem_lenght);
                                while (choice < 1 || choice > mem_lenght) {

                                    if ((choice < 0 || choice > mem_lenght) && choice != -1) {
                                        printf("Location unknown!\n\n");
                                    }
                                    printf("Insert function index: ");
                                    scanf("%d", &choice);
                                }
                                index = mem.head;
                                move_index(&index, choice);

                                struct string* t_string = (struct string*)malloc(sizeof(struct string));
                                create_string(t_string);
                                tree* t_tree = (tree*)malloc(sizeof(tree));
                                create_tree(t_tree);
                                tree* out_tree = (tree*)malloc(sizeof(tree));
                                create_tree(out_tree);

                                inorder_i(get_parent(index), t_string);
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
                                int pos = add_tree(&mem, out_tree);
                                

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
                        clear_screen();
                        choice = 0;
                        while(choice < 1 || choice > 3) {
                            clear_screen();
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
                                memory_node* index = mem.head;
                                get_length(index, mem, &mem_lenght);
                                while(choice < 1 || choice > mem_lenght) {
                                    clear_screen();

                                    if((choice < 0 || choice > mem_lenght) && choice != -1) {
                                        printf("Location unknown!\n\n");
                                    }
                                    printf("Insert function index: ");
                                    scanf("%d", &choice);
                                }
                                index = mem.head;
                                move_index(&index, choice);
                                printf("Insert x value: ");

                                struct string* x = (struct string*)malloc(sizeof(struct string));
                                create_string(x);
                                struct string* t_string = (struct string*)malloc(sizeof(struct string));
                                create_string(t_string);
                                tree* t_tree = (tree*)malloc(sizeof(tree));
                                create_tree(t_tree);

                                inorder_i(get_parent(index), t_string);
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

                                clear_screen();
                                printf("Input search parameters :  ");
                                scan_string(s_params);
                                printf("\n\n");
                                fix(s_params);

                                memory_node* mem_index = mem.head;
                                int is_equal = 0;
                                int func_found = 0;
                                int func_index = 1;
                                while (mem_index != NULL) {
                                    struct string* t_list = (struct string*)malloc(sizeof(struct string));
                                    create_string(t_list);
                                    inorder_i(get_parent(mem_index), t_list);
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

                                    to_next(&mem_index);
                                    func_index++;
                                    delete_string(t_list);
                                }

                                if (func_found == 0) {
                                    printf("No function found!");
                                }
                                delete_string(s_params);


                                choice = -1;
                                mem_lenght = 1;
                                index = mem.head;
                                get_length(index, mem, &mem_lenght);
                                while (choice < 1 || choice > mem_lenght) {

                                    if ((choice < 0 || choice > mem_lenght) && choice != -1) {
                                        printf("Location unknown!\n\n");
                                    }
                                    printf("Insert function index: ");
                                    scanf("%d", &choice);
                                }
                                index = mem.head;
                                move_index(&index, choice);

                                printf("Insert x value: ");

                                struct string* x2 = (struct string*)malloc(sizeof(struct string));
                                create_string(x2);
                                struct string* t_string2 = (struct string*)malloc(sizeof(struct string));
                                create_string(t_string2);
                                tree* t_tree2 = (tree*)malloc(sizeof(tree));
                                create_tree(t_tree2);

                                inorder_i(get_parent(index), t_string2);
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
            break;
            case 7:
                clear_screen();
                save_mem(mem);
                free_memory(&mem);
                exit(0);
            break;
            default:
                exit(1);
        }
    }
    
    
    return 0; 
}
