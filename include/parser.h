#pragma once

#include "char_list.h"
#include "tree.h"
#include "stdbool.h"

int move_to_next_block(c_node** index);

int move_to_previous_block(c_node** index);

int check_left_op(c_node* index, c_node* block_limit);

int check_right_op(c_node* index, c_node* block_limit);

int is_operator(char key);

int is_operator_pow(char key);

int expand_string(string* str);

int splice(string* str);

int group(c_node* block_start, c_node* block_end, string* str, bool should_restart, c_node* index);

int group_string(string* str);

int contract(string* str);

int mult_delete(string* string);

int mult_delete_r(string* string, c_node* index);

int one_mult_delete(string* string);

int par_check(string* string);

int check_limits(c_node* tl_index, c_node* tr_index, string* string);