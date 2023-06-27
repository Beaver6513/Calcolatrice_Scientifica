#include "tree.h"
#include "char_list.h"
#include "parser.h"
#include "memory.h"
#include "derivative.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIGIT 0
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define POW 5
#define VAR 6

tree_node* get_head_tree(tree* in_tree) {
    return in_tree->tree_head;
}

int inorder_s(tree_node* node, string* x) {
    if (node == NULL) return 0;
    inorder_s(node->l_child, x);
    v_substitute(node, x);
    inorder_s(node->r_child, x);
    return 0;
}

int inorder_dc(tree_node* node, char key) {
    if (node == NULL) return 0;
    inorder_dc(node->l_child, key);
    v_delete_char(node, key);
    inorder_dc(node->r_child, key);
    return 0;
}

int inorder_i(tree_node* node, string* out_string) {
    if (node == NULL) return 0;
    inorder_i(node->l_child, out_string);
    v_insert(node, out_string);
    inorder_i(node->r_child, out_string);
    return 0;
}

int postorder_r(tree_node* node) {
    if (node == NULL) return 0;
    postorder_r(node->l_child);
    postorder_r(node->r_child);
    v_delete(node);
    return 0;
}

int v_delete(tree_node* node) {
    delete_string(node->data);
    free(node);
    return 0;
}

int v_delete_char(tree_node* node, char key) {
    c_node* index = node->data->head;
    while(index != NULL) {
        if(index->character == key) {
            if(index->previous == NULL) {
                c_node* t = index;
                index->next->previous = NULL;
                index = index->next;
                node->data->head = index;
                free(t);
            } else if(index->next == NULL) {
                c_node* t = index;
                index->previous->next = NULL;
                index = index->previous;
                node->data->tail = index;
                free(t);
            } else if(index->previous != NULL && index->next != NULL) {
                c_node* t = index;
                index->previous->next = index->next;
                index->next->previous = index->previous;
                index = index->next;
                free(t);
            }
        } else {
            index = index->next;
        }
    }
    return 0;
}

int postorder_rd(d_tree_node* node) {
    if(node == NULL) return 0;
    postorder_rd(node->l_child);
    postorder_rd(node->r_child);
    free(node);
    return 0;
}

int v_substitute(tree_node* node, string* x) {
    c_node* index = node->data->head;
    while(index != NULL) {
        if(index->next != NULL) {
            if(index->character == '_' && index->next->character == 'x') goto cycle_exit;
        }
        if(index != NULL) {
            if(index->character == 'x') goto cycle_exit;
        }
        index = index->next;
    }
    cycle_exit:
    if(index != NULL) {
        if(index->character == '_' && x->head->character == '_') {
            if(index->previous == NULL) {
                c_node* t = index;
                node->data->head = node->data->head->next;
                index->next->previous = NULL;
                index = index->next;
                free(t);
                t = x->head;
                x->head = x->head->next;
                x->head->previous = NULL;
                free(t);
            } else if(index->previous != NULL) {
                c_node* t = index;
                index->previous->next = index->next;
                index->next = index->previous;
                index = index->next;
                free(t);
                t = x->head;
                x->head = x->head->next;
                x->head->previous = NULL;
                free(t);
            }
            c_node* x_index = x->head;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
            node->data->head->previous = NULL;
            node->data->tail->next = NULL;
            head_string_insert(x, '_');
        } else if(index->character == '_' && isdigit(x->head->character)) {
            c_node* x_index = x->head;
            index = index->next;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
        } else if(index->character == 'x' && isdigit(x->head->character)) {
            c_node* x_index = x->head;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
        } else if(index->character == 'x' && x->head->character == '_') {
            c_node* x_index = x->head;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
        }
        node->data->head->previous = NULL;
        node->data->tail->next = NULL;
    }

    return 0;
}

int v_insert(tree_node* node, string* out_string) {
    c_node* index = node->data->head;
    int s = 0;
    do {
        s = 0;
        tail_string_insert(out_string, index->character);
        if(index->next != NULL) {
            index = index->next;
        } else s = 1;
    } while(index != NULL && s == 0);
    return 0;
}

int create_tree(tree* out_tree) {
    out_tree->tree_head = NULL;
    return 0;
}

int create_d_tree(d_tree* out_tree) {
    out_tree->d_tree_head = NULL;
    return 0;
}

int is_leaf(c_node* block_start, c_node* block_end) {
    c_node* index = block_start;
    while(index != block_end) {
        if(is_operator(index->character))
            return false;
        index = index->next;
    }
    return true;
}

int load_tree_block(c_node* block_start, c_node* block_end, tree_node* out_node, tree* out_tree, string* in_string) {
    if(out_node == NULL) {
        tree_node* node = (tree_node*)malloc(sizeof(tree_node));
        string* data = (string*)malloc(sizeof(string));
        create_string(data);
        node->data = data;
        out_node = node;
        out_tree->tree_head = node;
        node->parent = NULL;
        node->l_child = NULL;
        node->r_child = NULL;
    }
    if(!(is_leaf(block_start, block_end))) {
        if(out_node->l_child == NULL) {
            tree_node* l_child = (tree_node*)malloc(sizeof(tree_node));
            string* data = (string*)malloc(sizeof(string));
            create_string(data);
            l_child->data = data;
            l_child->parent = out_node;
            l_child->l_child = NULL;
            l_child->r_child = NULL;
            out_node->l_child = l_child;
        }
        if(out_node->r_child == NULL) {
            tree_node* r_child = (tree_node*)malloc(sizeof(tree_node));
            string* data = (string*)malloc(sizeof(string));
            create_string(data);
            r_child->data = data;
            r_child->parent = out_node;
            r_child->l_child = NULL;
            r_child->r_child = NULL;
            out_node->r_child = r_child;
        }

        c_node* index = block_start;

        move_to_next_block(&index);
        index = index->next;

        if(out_node->data == NULL) {
            out_node->data = (string*)malloc(sizeof(string));
            create_string(out_node->data);
        }
        head_string_insert(out_node->data, index->character);

        c_node* l_block_end = index;
        c_node* l_block_start = index;
        c_node* r_block_start = index;
        c_node* r_block_end = index;

        move_to_previous_block(&l_block_start);
        move_to_next_block(&r_block_end);

        l_block_end = index->previous;
        r_block_start = index->next;

        load_tree_block(l_block_start, l_block_end, out_node->l_child, out_tree, in_string);
        load_tree_block(r_block_start, r_block_end, out_node->r_child, out_tree, in_string);
    } else if(is_leaf(block_start, block_end)) {
        c_node* index = block_start;
        do {
            if(index->previous != NULL) {
                if(index->previous->character == '(' || index->previous->character == '[') {
                    index = index->previous;
                } else goto cycle_end;
            } else goto cycle_end;
        } while(index->previous != NULL);
        cycle_end:

        if(out_node->data == NULL) {
            out_node->data = (string*)malloc(sizeof(string));
            create_string(out_node->data);
        }

        if(index->character == '[') {
            tail_string_insert(out_node->data, '[');
        }
        do {
            index = index->next;
            if(index->character != '(' && index->character != ')') {
                tail_string_insert(out_node->data, index->character);
            }
        } while(index->character != ')');
        while(index->character == ')' || index->character == ']') {
            index = index->next;
            if(index != NULL) {
                if(index->character == ']') {
                    tail_string_insert(out_node->data, index->character);
                }
            } else goto cycle2_end;
        }
    cycle2_end:;
    }
    return 0;
}

int remove_tree(tree* tree) {
    if(tree == NULL) {
        free(tree);
        return 0;
    }
    postorder_r(tree->tree_head);
    free(tree);
    return 0;
}

int remove_d_tree(d_tree* tree) {
    postorder_rd(tree->d_tree_head);
    free(tree);
    return 0;
}

double compute(d_tree_node* d_node) {
    if(d_node == NULL) return 0;
    compute(d_node->l_child);
    compute(d_node->r_child);

    if(d_node->op == ADD) {
        d_node->data = d_node->l_child->data + d_node->r_child->data;
    } else if(d_node->op == SUB) {
        d_node->data = d_node->l_child->data - d_node->r_child->data;
    } else if(d_node->op == MULT) {
        d_node->data = d_node->l_child->data * d_node->r_child->data;
    } else if(d_node->op == DIV) {
        d_node->data = d_node->l_child->data / d_node->r_child->data;
    } else if(d_node->op == POW) {
        d_node->data = pow(d_node->l_child->data, d_node->r_child->data);
    }

    if(d_node->parent == NULL) return d_node->data;
    return 0;
}

double get_func_in_x(tree* in_tree, string* x) {
    d_tree* digit_tree = (d_tree*)malloc(sizeof(d_tree));
    create_d_tree(digit_tree);
    inorder_s(in_tree->tree_head, x);
    transfer_to_d_tree(in_tree, digit_tree);
    double res = compute(digit_tree->d_tree_head);
    remove_d_tree(digit_tree);
    return res;
}

int copy_to_d_node(tree_node* node, d_tree_node* d_node, d_tree* d_tree) {
    if(node == NULL && d_node == NULL) return 0;

    if (d_node == NULL && node != NULL) {
        d_tree_node* new_d_node = (d_tree_node*)malloc(sizeof(d_tree_node));
        d_tree->d_tree_head = new_d_node;
        new_d_node->data = 0;
        new_d_node->l_child = NULL;
        new_d_node->r_child = NULL;
        new_d_node->op = DIGIT;
        new_d_node->parent = NULL;
        d_node = new_d_node;
    }

    if(is_operator(node->data->head->character) && d_node->l_child == NULL && d_node->r_child == NULL) {
        d_tree_node* new_l_d_node = (d_tree_node*)malloc(sizeof(d_tree_node));
        d_tree_node* new_r_d_node = (d_tree_node*)malloc(sizeof(d_tree_node));
        d_node->l_child = new_l_d_node;
        d_node->r_child = new_r_d_node;
        d_node->l_child->data = 0;
        d_node->r_child->data = 0;
        d_node->l_child->op = DIGIT;
        d_node->r_child->op = DIGIT;
        d_node->l_child->parent = d_node;
        d_node->r_child->parent = d_node;
        d_node->l_child->l_child = NULL;
        d_node->l_child->r_child = NULL;
        d_node->r_child->l_child = NULL;
        d_node->r_child->r_child = NULL;
    }

    if(node->l_child != NULL && d_node->l_child != NULL) {
        copy_to_d_node(node->l_child, d_node->l_child, d_tree);
    }
    if(node->r_child != NULL && d_node->r_child != NULL) {
        copy_to_d_node(node->r_child, d_node->r_child, d_tree);
    }
    
    if(node->data->head->character == '+') {
        d_node->op = ADD;
    } else if(node->data->head->character == '-') {
        d_node->op = SUB;
    } else if(node->data->head->character == '*') {
        d_node->op = MULT;
    } else if(node->data->head->character == '/') {
        d_node->op = DIV;
    } else if(node->data->head->character == '^') {
        d_node->op = POW;
    } else {
        d_node->op = DIGIT;
        int n = get_number(node->data);
        d_node->data = n;
    }

    return 0;
}

int transfer_to_d_tree(tree* tree, d_tree* d_tree) {
    copy_to_d_node(tree->tree_head, d_tree->d_tree_head, d_tree);
    return 0;
}

int delete_from_tree(tree* tree, char key) {
    inorder_dc(tree->tree_head, key);
    return 0;
}