#pragma once

#include "char_list.h"

struct tree_node {
    string* data;
    struct tree_node* parent;
    struct tree_node* l_child;
    struct tree_node* r_child;
};
typedef struct tree_node tree_node;

struct d_tree_node {
    double data;
    int op;
    struct d_tree_node* parent;
    struct d_tree_node* l_child;
    struct d_tree_node* r_child;
};
typedef struct d_tree_node d_tree_node;

struct tree {
    tree_node* tree_head;
};
typedef struct tree tree;

struct d_tree {
    d_tree_node* d_tree_head;
};
typedef struct d_tree d_tree;

int create_tree(tree* out_tree);

int create_d_tree(d_tree* out_tree);

int is_leaf(c_node* block_start, c_node* block_end);

int load_tree(tree* out_tree, string* str);

int load_tree_block(c_node* block_start, c_node* block_end, tree_node* out_node, tree* out_tree, string* in_string);

int v_insert(tree_node* node, string* out_string);

int v_delete(tree_node* node);

int v_delete_char(tree_node* node, char key);

int inorder_i(tree_node* node, string* out_string);

int postorder_r(tree_node* node);

int postorder_rd(d_tree_node* node);

int remove_tree(tree* tree);

int remove_d_tree(d_tree* tree);

int v_substitute(tree_node* node, string* x);

int delete_from_tree(tree* tree, char key);

tree_node* get_head_tree(tree* in_tree);

c_node* get_head_cn_from_tree_node(tree_node* node);

string* get_data(tree_node* node);

tree_node* get_l_child(tree_node* in_tree);

tree_node* get_r_child(tree_node* in_tree);