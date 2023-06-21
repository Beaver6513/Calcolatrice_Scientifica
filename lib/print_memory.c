#include "print_memory.h"
#include "memory.h"
#include "tree_utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_memory(memory* mem) {
    int choice = 0;
    while(choice < 1 || choice > 3) {
        system("clear");
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
            memory_node* index = mem->head;
            get_length(index, *mem, &mem_lenght);
            while(choice < 1 || choice > mem_lenght) {
                system("clear");

                if((choice < 0 || choice > mem_lenght) && choice != -1) {
                    printf("Location unknown!\n\n");
                }
                printf("Insert function index: ");
                scanf("%d", &choice);
            }
            printf("\n");
            index = mem->head;
            move_index(&index, choice);
            printf("Function :   ");
            print_tree(index);
            int k;
            while ((k = getchar()) != '\n' && k != EOF);
            printf("\nPress enter to return to main menu...");
        break;
        case 2:
            index = mem->head;
            int count = 1;
            system("clear");
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
}