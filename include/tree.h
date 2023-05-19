#pragma once

#include "char_list.h"

struct tree_node {
    struct string* data;
    struct tree_node* parent;
    struct tree_node* l_child;
    struct tree_node* r_child;
};

struct d_tree_node {
    double data;
    int operator;
    struct d_tree_node* parent;
    struct d_tree_node* l_child;
    struct d_tree_node* r_child;
};

struct tree {
    struct tree_node* tree_head;
};

struct d_tree {
    struct d_tree_node* d_tree_head;
};

int create_tree(struct tree* out_tree);

int create_d_tree(struct d_tree* out_tree);

int is_leaf(struct c_node* block_start, struct c_node* block_end);

int load_tree_block(struct c_node* block_start, struct c_node* block_end, struct tree_node* out_node, struct tree* out_tree, struct string* in_string);

int print_tree(struct tree* out_tree);

int v_insert(struct tree_node* node, struct string* out_string);

int v_delete(struct tree_node* node);

int inorder_i(struct tree_node* node, struct string* out_string);

int postorder_r(struct tree_node* node);

int copy_to_d_node(struct tree_node* node, struct d_tree_node* d_node, struct d_tree* d_tree);

int postorder_rd(struct d_tree_node* node);

int remove_tree(struct tree* tree);

int remove_d_tree(struct d_tree* tree);

int get_der_func(struct tree* in_tree, struct string* der_func);

int v_substitute(struct tree_node* node, struct string* x);

double get_func_in_x(struct tree* in_tree, struct string* x);

double compute(struct d_tree_node* d_node);

int transfer_to_d_tree(struct tree* tree, struct d_tree* d_tree);