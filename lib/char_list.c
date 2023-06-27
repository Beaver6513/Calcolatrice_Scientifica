#include "char_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#ifdef __linux__ 
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>             //STDIN_FILENO
#elif _WIN32
#include <conio.h>
#else

#endif

int create_string(string* str) {
    str->head = NULL;
    str->tail = NULL;
    return 0;
}

int tail_string_insert(string* str, char data) {
    c_node* n = (c_node*)malloc(sizeof(c_node));
    n->next = NULL;

    n->character = data;

    if(str->head == NULL) {
        n->previous = NULL;
	    str->head = n;
	    str->tail = n;
    } else {
        n->previous = str->tail;
	    str->tail->next = n;
	    str->tail = n;
    }

    return 0;
}

int head_string_insert(string* str, char data) {
    c_node* n = (c_node*)malloc(sizeof(c_node));
    n->previous = NULL;

    n->character = data;

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

#ifdef __linux__ 
int scan_string(string* str) {
    char c = 'a';

    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    str->head = NULL;
    str->tail = NULL;

    int k;
    while ((k = getchar()) != '\n' && k != EOF);
    while (1) {
        c = getchar();
        if (c == '\n') break;
        tail_string_insert(str, c);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
#elif _WIN32
int scan_string(string* str) {
    char c = 'a';

    str->head = NULL;
    str->tail = NULL;

    flush_stdin();
    while (1) {
        c = getchar();
        if (c == '\n') break;
        tail_string_insert(str, c);
    }

    return 0;
}
#else

#endif

int print_string(string str) {
    c_node* index = str.head;
    while (index != NULL) {
        printf("%c", index->character);
        index = index->next;
    }
    printf("\n");
    return 0;
}

int delete(string* str, char key) {
    c_node* index = str->head;

    while (index != NULL) {
        if(index->character == key) {
            if(index == str->head) {
                str->head = (str->head)->next;
                c_node* t = index;
                index = str->head;
                free(t);
            } else if(index->next == NULL) {
                (index->previous)->next = NULL;
                c_node* t = index;
                str->tail = index->previous;
                free(t);
            } else {
                (index->next)->previous = index->previous;
                (index->previous)->next = index->next;
                c_node* t = index;
                index = index->next;
                free(t);
            }
        } else {
            index = index->next;
        }
    }
    return 0;
}

int modify(string* str, char key, char target) {
    c_node* index = str->head;
    while(index != NULL) {
        if(index->character == target) index->character = key;
        index = index->next;
    }
    return 0;
}

int delete_string(string* str) {
    if(str->head == NULL && str->tail == NULL) {
        free(str);
        return 0;
    }
    c_node* i = str->head;
    do {
        i = str->head;
        str->head = str->head->next;
        free(i);
    } while(str->head != NULL);
    free(str);
    return 0;
}

int compare(string* s_params, string* t_list) {
    int param_length = 0;
    int test_length = 0;
    int is_equal = 1;

    c_node* param_index = s_params->head;
    c_node* test_index = t_list->head;

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
    c_node* temp_ptr = t_list->head;
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

int get_number(string* string) {
    int res = 0;
    int exp = 0;
    c_node* index = string->tail;
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

int insert_before(string* str, c_node* index, char key) {
    if(index->previous == NULL) {
        head_string_insert(str, key);
    } else {
        c_node* temp = index->previous;
        c_node* n = (c_node*)malloc(sizeof(c_node));

        n->character = key;

        n->previous = temp;
        n->next = index;

        index->previous = n;
        temp->next = n;
    }
    return 0;
}

int insert_after(string* str, c_node* index, char key) {
    if(index->next == NULL) {
        tail_string_insert(str, key);
    } else {
        c_node* temp = index->next;
        c_node* n = (c_node*)malloc(sizeof(c_node));

        n->character = key;

        n->previous = index;
        n->next = temp;

        index->next = n;
        temp->previous = n;
    }
    return 0;
}

int insert_before_l(string* str, c_node* index, char* t) {
    for (int i = 0; i < strlen(t); i++) {
        insert_before(str, index, t[i]);
    }
    return 0;
}

int insert_after_l(string* str, c_node* index, char* t) {
    for (int i = strlen(t) - 1; i > -1; i--) {
        insert_after(str, index, t[i]);
    }
    return 0;
}

int to_string(string* str, int n) {
    int digits = log10(n) + 1;

    for(int i = 0 ; i < digits ; i++) {
        int digit = (n / pow(10, i));
        digit = digit % 10;
        char c = digit + '0';
        head_string_insert(str, c);
    }
    return 0;
}

int delete_between(string* string, c_node* block_start, c_node* block_end) {
    if(block_start->previous == NULL && block_end->next != NULL) {
        string->head = block_end->next;
        string->head->previous = NULL;
        do {
            c_node* t = block_start;
            block_start = block_start->next;
            free(t);
        } while(block_start != block_end);
        free(block_start);
    } else if(block_start->previous != NULL && block_end->next == NULL) {
        string->tail = block_start->previous;
        string->tail->next = NULL;
        do {
            c_node* t = block_start;
            block_start = block_start->next;
            free(t);
        } while(block_start != block_end);
        free(block_start);
    } else if(block_start->previous != NULL && block_end->next != NULL) {
        block_start->previous->next = block_end->next;
        block_end->next->previous = block_start->previous;
        do {
            c_node* t = block_start;
            block_start = block_start->next;
            free(t);
        } while(block_start != block_end);
        free(block_start);
    } else if(block_start->previous == NULL && block_end->next == NULL) {
        do {
            c_node* t = block_start;
            block_start = block_start->next;
            free(t);
        } while(block_start != block_end);
        free(block_start);
        string->head = NULL;
        string->tail = NULL;
        head_string_insert(string, '0');
    }

    return 0;
}

int fix(string* string) {
    string->head->previous = NULL;
    string->tail->next = NULL;
    return 0;
}

c_node* get_head_str(string* str) {
    return str->head;
}

c_node* get_tail_str(string* str) {
    return str->tail;
}

void set_char(c_node* index, char key) {
    index->character = key;
}

char get_char(c_node* index) {
    return index->character;
}

void to_next_list(c_node** index) {
    (*index) = (*index)->next;
}

void to_prev_list(c_node** index) {
    (*index) = (*index)->previous;
}

c_node* get_next_str(c_node* index) {
    return index->next;
}

c_node* get_prev_str(c_node* index) {
    return index->previous;
}

c_node* get_next_str_m(c_node* index, int i) {
    for(int j = i ; j > 0 ; j--) {
        index = index->next;
    }
    return index;
}

c_node* get_prev_str_m(c_node* index, int i) {
    for(int j = i ; j > 0 ; j--) {
        index = index->previous;
    }
    return index;
}

void set_c_node(c_node** node, c_node* index) {
    *node = index;
}

void set_c_node_prev(c_node** node, c_node* index) {
    (*node)->previous = index;
}

void set_c_node_next(c_node** node, c_node* index) {
    (*node)->next = index;
}