#pragma once

#include "char_list.h"
#include "tree.h"

int get_func_der(struct tree* tree, struct string* out_string);

int derive_node(struct tree_node* node, struct string* out_string, struct c_node* index);

double get_func_in_x(struct tree* in_tree, struct string* x);

double compute(struct d_tree_node* d_node);

int transfer_to_d_tree(struct tree* tree, struct d_tree* d_tree);

int copy_to_d_node(struct tree_node* node, struct d_tree_node* d_node, struct d_tree* d_tree);