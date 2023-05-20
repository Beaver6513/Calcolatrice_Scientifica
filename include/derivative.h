#pragma once

#include "char_list.h"
#include "tree.h"

int get_func_der(struct tree* tree, struct string* out_string);

int derive_node(struct tree_node* node, struct string* out_string, struct c_node* block_start, struct c_node* block_end);