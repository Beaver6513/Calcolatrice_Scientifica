#include "tree.h"
#include "char_list.h"
#include "parser.h"
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

int create_tree(struct tree* out_tree) {
    out_tree->tree_head = NULL;
    return 0;
}

int create_d_tree(struct d_tree* out_tree) {
    out_tree->d_tree_head = NULL;
    return 0;
}

int is_leaf(struct c_node* block_start, struct c_node* block_end) {
    struct c_node* index = block_start;
    while(index != block_end) {
        if(is_operator(index->character))
            return false;
        index = index->next;
    }
    return true;
}

int load_tree_block(struct c_node* block_start, struct c_node* block_end, struct tree_node* out_node, struct tree* tree, struct string* in_string) {
    if(out_node == NULL) {
        struct tree_node* node = (struct tree_node*)malloc(sizeof(struct tree_node));
        struct string* data = (struct string*)malloc(sizeof(struct string));
        create_string(data);
        node->data = data;
        out_node = node;
        tree->tree_head = node;
        node->parent = NULL;
        node->l_child = NULL;
        node->r_child = NULL;
    }
    if(!(is_leaf(block_start, block_end))) {
        if(out_node->l_child == NULL) {
            struct tree_node* l_child = (struct tree_node*)malloc(sizeof(struct tree_node));
            struct string* data = (struct string*)malloc(sizeof(struct string));
            create_string(data);
            l_child->data = data;
            l_child->parent = out_node;
            l_child->l_child = NULL;
            l_child->r_child = NULL;
            out_node->l_child = l_child;
        }
        if(out_node->r_child == NULL) {
            struct tree_node* r_child = (struct tree_node*)malloc(sizeof(struct tree_node));
            struct string* data = (struct string*)malloc(sizeof(struct string));
            create_string(data);
            r_child->data = data;
            r_child->parent = out_node;
            r_child->l_child = NULL;
            r_child->r_child = NULL;
            out_node->r_child = r_child;
        }

        struct c_node* index = block_start;

        move_to_next_block(&index);
        index = index->next;

        if(out_node->data == NULL) {
            out_node->data = (struct string*)malloc(sizeof(struct string));
            create_string(out_node->data);
        }
        head_string_insert(out_node->data, index->character);

        struct c_node* l_block_end = index;
        struct c_node* l_block_start = index;
        struct c_node* r_block_start = index;
        struct c_node* r_block_end = index;

        move_to_previous_block(&l_block_start);
        move_to_next_block(&r_block_end);

        l_block_end = index->previous;
        r_block_start = index->next;

        load_tree_block(l_block_start, l_block_end, out_node->l_child, tree, in_string);
        load_tree_block(r_block_start, r_block_end, out_node->r_child, tree, in_string);
    } else if(is_leaf(block_start, block_end)) {
        struct c_node* index = block_start;
        do {
            if(index->previous != NULL) {
                if(index->previous->character == '(' || index->previous->character == '[') {
                    index = index->previous;
                } else goto cycle_end;
            } else goto cycle_end;
        } while(index->previous != NULL);
        cycle_end:

        if(out_node->data == NULL) {
            out_node->data = (struct string*)malloc(sizeof(struct string));
            create_string(out_node->data);
        }

        if(index->character == '[') {
            tail_string_insert(out_node->data, index->character);
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
        cycle2_end:
    }
    return 0;
}

int v_insert(struct tree_node* node, struct string* out_string) {
    struct c_node* index = node->data->head;
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

int inorder_i(struct tree_node* node, struct string* out_string) {
    if (node == NULL) return 0;
    inorder_i(node->l_child, out_string);
    v_insert(node, out_string);
    inorder_i(node->r_child, out_string);
    return 0;
}

int postorder_r(struct tree_node* node) {
    if (node == NULL) return 0;
    postorder_r(node->l_child);
    postorder_r(node->r_child);
    v_delete(node);
    return 0;
}

int print_tree(struct tree* out_tree) {
    struct string* out_string = (struct string*)malloc(sizeof(struct string));
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

int v_delete(struct tree_node* node) {
    delete_string(node->data);
    free(node);
    return 0;
}

int remove_tree(struct tree* tree) {
    postorder_r(tree->tree_head);
    free(tree);
    return 0;
}

int postorder_rd(struct d_tree_node* node) {
    if(node == NULL) return 0;
    postorder_rd(node->l_child);
    postorder_rd(node->r_child);
    free(node);
    return 0;
}

int remove_d_tree(struct d_tree* tree) {
    postorder_rd(tree->d_tree_head);
    free(tree);
    return 0;
}

int get_der_func(struct tree* in_tree, struct string* der_func) {

    return 0;
}

int v_substitute(struct tree_node* node, struct string* x) {
    struct c_node* index = node->data->head;
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
                struct c_node* t = index;
                node->data->head = node->data->head->next;
                index->next->previous = NULL;
                index = index->next;
                free(t);
                t = x->head;
                x->head = x->head->next;
                x->head->previous = NULL;
                free(t);
            } else if(index->previous != NULL) {
                struct c_node* t = index;
                index->previous->next = index->next;
                index->next = index->previous;
                index = index->next;
                free(t);
                t = x->head;
                x->head = x->head->next;
                x->head->previous = NULL;
                free(t);
            }
            struct c_node* x_index = x->head;

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
            struct c_node* x_index = x->head;
            index = index->next;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
        } else if(index->character == 'x' && isdigit(x->head->character)) {
            struct c_node* x_index = x->head;

            while(x_index != NULL) {
                index->character = x_index->character;
                if(x_index->next != NULL) insert_after(node->data, index, '0');
                index = index->next;
                x_index = x_index->next;
            }
        } else if(index->character == 'x' && x->head->character == '_') {
            struct c_node* x_index = x->head;

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
    


int inorder_s(struct tree_node* node, struct string* x) {
    if (node == NULL) return 0;
    inorder_s(node->l_child, x);
    v_substitute(node, x);
    inorder_s(node->r_child, x);
    return 0;
}

double compute(struct d_tree_node* d_node) {
    if(d_node == NULL) return 0;
    compute(d_node->l_child);
    compute(d_node->r_child);

    if(d_node->operator == ADD) {
        d_node->data = d_node->l_child->data + d_node->r_child->data;
    } else if(d_node->operator == SUB) {
        d_node->data = d_node->l_child->data - d_node->r_child->data;
    } else if(d_node->operator == MULT) {
        d_node->data = d_node->l_child->data * d_node->r_child->data;
    } else if(d_node->operator == DIV) {
        d_node->data = d_node->l_child->data / d_node->r_child->data;
    } else if(d_node->operator == POW) {
        d_node->data = pow(d_node->l_child->data, d_node->r_child->data);
    }

    if(d_node->parent == NULL) return d_node->data;
    return 0;
}

double get_func_in_x(struct tree* in_tree, struct string* x) {
    struct d_tree* d_tree = (struct d_tree*)malloc(sizeof(struct d_tree));
    create_d_tree(d_tree);
    inorder_s(in_tree->tree_head, x);
    transfer_to_d_tree(in_tree, d_tree);
    double res = compute(d_tree->d_tree_head);
    remove_d_tree(d_tree);
    return res;
}

int copy_to_d_node(struct tree_node* node, struct d_tree_node* d_node, struct d_tree* d_tree) {
    if(node == NULL && d_node == NULL) return 0;

    if (d_node == NULL && node != NULL) {
        struct d_tree_node* new_d_node = (struct d_tree_node*)malloc(sizeof(struct d_tree_node));
        d_tree->d_tree_head = new_d_node;
        new_d_node->data = 0;
        new_d_node->l_child = NULL;
        new_d_node->r_child = NULL;
        new_d_node->operator = DIGIT;
        new_d_node->parent = NULL;
        d_node = new_d_node;
    }

    if(is_operator(node->data->head->character) && d_node->l_child == NULL && d_node->r_child == NULL) {
        struct d_tree_node* new_l_d_node = (struct d_tree_node*)malloc(sizeof(struct d_tree_node));
        struct d_tree_node* new_r_d_node = (struct d_tree_node*)malloc(sizeof(struct d_tree_node));
        d_node->l_child = new_l_d_node;
        d_node->r_child = new_r_d_node;
        d_node->l_child->data = 0;
        d_node->r_child->data = 0;
        d_node->l_child->operator = DIGIT;
        d_node->r_child->operator = DIGIT;
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
        d_node->operator = ADD;
    } else if(node->data->head->character == '-') {
        d_node->operator = SUB;
    } else if(node->data->head->character == '*') {
        d_node->operator = MULT;
    } else if(node->data->head->character == '/') {
        d_node->operator = DIV;
    } else if(node->data->head->character == '^') {
        d_node->operator = POW;
    } else {
        d_node->operator = DIGIT;
        int n = get_number(node->data);
        d_node->data = n;
    }

    return 0;
}

int transfer_to_d_tree(struct tree* tree, struct d_tree* d_tree) {
    copy_to_d_node(tree->tree_head, d_tree->d_tree_head, d_tree);
    return 0;
}
