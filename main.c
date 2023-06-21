#include "char_list.h"
#include "memory.h"
#include "parser.h"
#include "tree.h"
#include "tree_utils.h"
#include "derivative.h"
#include "files.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice = 0;
    memory mem;
    create_memory(&mem);
    load_mem(&mem);
    
    while(1) {
        //Resets
        system("clear");;
        choice = 0;
        //MAIN CHOICE INPUT---------
        while(choice < 1 || choice > 7) {
            system("clear");
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
                add_function(&mem);
            break;
            case 2:
                delete_function(&mem);
            break;
            case 3:
                search_function(&mem);
            break;
            case 4:
                duplicate_function(&mem);
            break;
            case 5:
                print_memory(&mem);
            break;
            case 6:
                apply_operation(&mem);
            break;
            case 7:
                system("clear");
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
