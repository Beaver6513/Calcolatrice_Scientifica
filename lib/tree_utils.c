#include "tree_utils.h"
#include <stdlib.h>
#include "memory.h"
#include "tree.h"
#include "parser.h"

int print_tree(memory_node* node) {
    tree* out_tree = get_tree_from_mem_index(node);
    string* out_string = (string*)malloc(sizeof(string));
    create_string(out_string);
    inorder_i(get_head_tree(out_tree), out_string);
    modify(out_string, '(', '[');
    modify(out_string, ')', ']');
    modify(out_string, '-', '_');
    contract(out_string);
    print_string(*out_string);
    delete_string(out_string);
    return 0;
}