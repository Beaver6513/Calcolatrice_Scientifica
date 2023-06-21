#pragma once

#include "char_list.h"
#include "tree.h"

int get_func_der(tree* tree, string* out_string);

int derive_node(tree_node* node, string* out_string, c_node* index);

double get_func_in_x(tree* in_tree, string* x);

double compute(d_tree_node* d_node);

int transfer_to_d_tree(tree* tree, d_tree* d_tree);

int copy_to_d_node(tree_node* node, d_tree_node* d_node, d_tree* d_tree);