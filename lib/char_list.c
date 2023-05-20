#include "char_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>             //STDIN_FILENO

int create_string(struct string* str) {
    str->head = NULL;
    str->tail = NULL;
    return 0;
}

int tail_string_insert(struct string* str, char data) {
    struct c_node* n = (struct c_node*)malloc(sizeof(struct c_node));
    n->next = NULL;

    n->character = data;
    n->next = NULL;

    if(str->head == NULL) {
	    str->head = n;
	    str->tail = n;
    } else {
        n->previous = str->tail;
	    str->tail->next = n;
	    str->tail = n;
    }

    return 0;
}

int head_string_insert(struct string* str, char data) {
    struct c_node* n = (struct c_node*)malloc(sizeof(struct c_node));
    n->next = NULL;

    n->character = data;
    n->previous = NULL;

    if(str->head == NULL) {
        n->next = NULL;
        str->tail = n;
        str->head = n;
    } else {
        str->head->previous = n;
        n->next = str->head;
        str->head = n;
    }
    return 0;
}

int scan_string(struct string* str) {
    char c = 'a';

    static struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    str->head = NULL;
    str->tail = NULL;

    while ((c = getchar()) != '\n' && c != EOF);
    while(1) {
        c = getchar();
        if(c == '\n') break;
        tail_string_insert(str, c);
    }

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

int print_string(struct string str) {
    struct c_node* index = str.head;
    while (index != NULL) {
        printf("%c", index->character);
        index = index->next;
    }
    printf("\n");
    return 0;
}

int delete(struct string* str, char key) {
    struct c_node* index = str->head;

    while (index != NULL) {
        if(index->character == key) {
            if(index == str->head) {
                str->head = (str->head)->next;
                struct c_node* t = index;
                index = str->head;
                free(t);
            } else if(index->next == NULL) {
                (index->previous)->next = NULL;
                struct c_node* t = index;
                str->tail = index->previous;
                free(t);
            } else {
                (index->next)->previous = index->previous;
                (index->previous)->next = index->next;
                struct c_node* t = index;
                index = index->next;
                free(t);
            }
        } else {
            index = index->next;
        }
    }
    return 0;
}

int modify(struct string* str, char key, char target) {
    struct c_node* index = str->head;
    while(index != NULL) {
        if(index->character == target) index->character = key;
        index = index->next;
    }
    return 0;
}

int delete_string(struct string* str) {
    struct c_node* i = str->head;
    do {
        i = str->head;
        str->head = str->head->next;
        free(i);
    } while(str->head != NULL);
    free(str);
    return 0;
}

int compare(struct string* s_params, struct string* t_list) {
    int param_length = 0;
    int test_length = 0;
    int is_equal = 1;

    struct c_node* param_index = s_params->head;
    struct c_node* test_index = t_list->head;

    while(param_index != NULL) {
        param_length++;
        param_index = param_index->next;
    }

    while(test_index != NULL) {
        test_length++;
        test_index = test_index->next;
    }

    if(param_length > test_length) return 2;

    int max_offset = (test_length + 1) - param_length;
    struct c_node* temp_ptr = t_list->head;
    int i = 0;
    for(i = 0 ; i < max_offset ; i++) {
        is_equal = 1;
        param_index = s_params->head;
        test_index = temp_ptr;
        while(param_index != NULL) {
            if(param_index->character != test_index->character) is_equal = 0;
            param_index = param_index->next;
            test_index = test_index->next;
        }
        if(is_equal == 1) break;
        temp_ptr = temp_ptr->next;
    }
    
    if(i == max_offset) is_equal = 0;

    return is_equal;
}

int get_number(struct string* string) {
    int res = 0;
    int exp = 0;
    struct c_node* index = string->tail;
    while(index->character == ']') {
        index = index->previous;
    }
    while(isdigit(index->character)) {
        int n = index->character - '0';
        res += n*(pow(10, exp));
        index = index->previous;
        if(index == NULL) goto cycle_end;
        exp++;
    }
    cycle_end:
    if(index != NULL) {
        if(index->character == '_') {
            res *= -1;
        }
    }
    return res;
}

int insert_before(struct string* str, struct c_node* index, char key) {
    if(index->previous == NULL) {
        head_string_insert(str, key);
    } else {
        struct c_node* temp = index->previous;
        struct c_node* n = (struct c_node*)malloc(sizeof(struct c_node));

        n->character = key;

        n->previous = temp;
        n->next = index;

        index->previous = n;
        temp->next = n;
    }
    return 0;
}

int insert_after(struct string* str, struct c_node* index, char key) {
    if(index->next == NULL) {
        tail_string_insert(str, key);
    } else {
        struct c_node* temp = index->next;
        struct c_node* n = (struct c_node*)malloc(sizeof(struct c_node));

        n->character = key;

        n->previous = index;
        n->next = temp;

        index->next = n;
        temp->previous = n;
    }
    return 0;
}