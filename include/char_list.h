#pragma once

struct c_node {
    char character;
    struct c_node* next;
    struct c_node* previous;
};
typedef struct c_node c_node;

struct string {
    c_node* head;
    c_node* tail;
};
typedef struct string string;

int create_string(string* str);

int delete_string(string* str);

int tail_string_insert(string* str, char data);

int head_string_insert(string* str, char data);

int scan_string(string* str);

int print_string(string str);

int delete(string* str, char key);

int modify(string* str, char key, char target);

int compare(string* s_params, string* t_list);

int get_number(string* string);

int insert_before(string* str, c_node* index, char key);

int insert_after(string* str, c_node* index, char key);

int insert_before_l(string* str, c_node* index, string* string);

int insert_after_l(string* str, c_node* index, string* string);

int to_string(string* str, int n);

int delete_between(string* string, c_node* block_start, c_node* block_end);

int fix(string* string);

c_node* get_head_str(string* str);

c_node* get_tail_str(string* str);

c_node* get_next_str(c_node* index);

c_node* get_prev_str(c_node* index);

void set_char(c_node* index, char key);

char get_char(c_node* index);

void to_next_list(c_node** index);

void to_prev_list(c_node** index);

void set_c_node(c_node** node, c_node* index);