#pragma once

#include "char_list.h"
#include "tree.h"
#include "stdbool.h"

int move_to_next_block(struct c_node** index);

int move_to_previous_block(struct c_node** index);

int check_left_op(struct c_node* index, struct c_node* block_limit);

int check_right_op(struct c_node* index, struct c_node* block_limit);

int is_operator(char key);

int expand_string(struct string* str);

int splice(struct string* str);

int group(struct c_node* block_start, struct c_node* block_end, struct string* str, bool should_restart, struct c_node* index);

int group_string(struct string* str);

int load_tree(struct tree* out_tree, struct string* str);

int contract(struct string* str);