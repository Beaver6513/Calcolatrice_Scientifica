#pragma once

struct c_node {
    char character;
    struct c_node* next;
    struct c_node* previous;
};

struct string {
    struct c_node* head;
    struct c_node* tail;
};

int create_string(struct string* str);

int delete_string(struct string* str);

int tail_string_insert(struct string* str, char data);

int head_string_insert(struct string* str, char data);

int scan_string(struct string* str);

int print_string(struct string str);

int delete(struct string* str, char key);

int modify(struct string* str, char key, char target);

int compare(struct string* s_params, struct string* t_list);

int get_number(struct string* string);