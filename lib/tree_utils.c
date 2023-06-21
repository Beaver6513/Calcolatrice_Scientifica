#include "tree_utils.h"
#include <stdlib.h>
#include "memory.h"
#include "tree.h"
#include "parser.h"

int print_tree(memory_node* node) {
    tree* out_tree = node->data;
    string* out_string = (string*)malloc(sizeof(string));
    create_string(out_string);
    inorder_i(out_tree->tree_head, out_string);
    modify(out_string, '(', '[');
    modify(out_string, ')', ']');
    modify(out_string, '-', '_');
    contract(out_string);
    print_string(*out_string);
    delete_string(out_string);
    return 0;
}


tree_node* get_parent(memory_node* mem_index) {
    return mem_index->data->tree_head;
}