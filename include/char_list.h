#pragma once

struct c_node {
    char character;
    struct c_node* next;
    struct c_node* previous;
};
typedef struct c_node c_node;

struct string {
    struct c_node* head;
    struct c_node* tail;
};
typedef struct string string;

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

int insert_before(struct string* str, struct c_node* index, char key);

int insert_after(struct string* str, struct c_node* index, char key);

int insert_before_l(struct string* str, struct c_node* index, struct string* string);

int insert_after_l(struct string* str, struct c_node* index, struct string* string);

int to_string(struct string* str, int n);

int delete_between(struct string* string, struct c_node* block_start, struct c_node* block_end);