#include "derivative.h"
#include "char_list.h"
#include "tree.h"
#include "parser.h"
#include <stdio.h>

int get_func_der(struct tree* tree, struct string* out_string) {
    struct c_node* block_start = out_string->head;
    struct c_node* block_end = out_string->tail;

    derive_node(tree->tree_head, out_string, block_start, block_end);

    return 0;
}

int derive_node(struct tree_node* node, struct string* out_string, struct c_node* block_start, struct c_node* block_end) {
    if(block_start == NULL && block_end == NULL) {
        head_string_insert(out_string, '#');
        block_start = out_string->head;
        block_end = out_string->tail;
    }

    if(node->data->head->character == '*') {
        block_start->character = '+';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, block_start, '@');
            insert_after(out_string, block_end, '@');
            insert_before(out_string, block_start->previous, '*');
            insert_after(out_string, block_end->next, '*');
            insert_before(out_string, block_start->previous->previous, '#');
            insert_after(out_string, block_end->next->next, '#');
            insert_before(out_string, block_start->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next, ']');

            struct c_node* ll_block_start = block_start->previous->previous->previous;
            struct c_node* ll_block_end = block_start->previous->previous->previous;
            struct c_node* lr_block_start = block_start->previous;
            struct c_node* lr_block_end = block_start->previous;
            struct c_node* rl_block_start = block_end->next;
            struct c_node* rl_block_end = block_end->next;
            struct c_node* rr_block_start = block_end->next->next->next;
            struct c_node* rr_block_end = block_end->next->next->next;

            derive_node(node->l_child, out_string, ll_block_start, ll_block_end);
            derive_node(node->r_child, out_string, lr_block_start, lr_block_end);
            derive_node(node->l_child, out_string, rl_block_start, rl_block_end);
            derive_node(node->r_child, out_string, rr_block_start, rr_block_end);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, block_start, '@');
            insert_after(out_string, block_end, '[');
            insert_before(out_string, block_start->previous, '*');
            insert_after(out_string, block_end->next, '@');
            insert_before(out_string, block_start->previous->previous, ']');
            insert_after(out_string, block_end->next->next->next, ']');
            insert_before(out_string, block_start->previous->previous->previous, '#');
            insert_after(out_string, block_end->next->next->next->next, '*');
            insert_before(out_string, block_start->previous->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next->next->next, '#');
            insert_before(out_string, block_start->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next->next->next->next, ']');

            struct c_node* ll_block_start = block_start->previous->previous->previous->previous;
            struct c_node* ll_block_end = block_start->previous->previous->previous->previous;
            struct c_node* lr_block_start = block_start->previous;
            struct c_node* lr_block_end = block_start->previous;
            struct c_node* rl_block_start = block_end->next->next;
            struct c_node* rl_block_end = block_end->next->next;
            struct c_node* rr_block_start = block_end->next->next->next->next->next;
            struct c_node* rr_block_end = block_end->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_block_start, ll_block_end);
            derive_node(node->r_child, out_string, lr_block_start, lr_block_end);
            derive_node(node->l_child, out_string, rl_block_start, rl_block_end);
            derive_node(node->r_child, out_string, rr_block_start, rr_block_end);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, block_start, ']');
            insert_after(out_string, block_end, '@');
            insert_before(out_string, block_start->previous, '@');
            insert_after(out_string, block_end->next, '*');
            insert_before(out_string, block_start->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next, '[');
            insert_before(out_string, block_start->previous->previous->previous, '*');
            insert_after(out_string, block_end->next->next->next->next, '#');
            insert_before(out_string, block_start->previous->previous->previous->previous, '#');
            insert_after(out_string, block_end->next->next->next->next->next, ']');
            insert_before(out_string, block_start->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next->next->next->next, ']');

            struct c_node* ll_block_start = block_start->previous->previous->previous->previous->previous;
            struct c_node* ll_block_end = block_start->previous->previous->previous->previous->previous;
            struct c_node* lr_block_start = block_start->previous->previous;
            struct c_node* lr_block_end = block_start->previous->previous;
            struct c_node* rl_block_start = block_end->next;
            struct c_node* rl_block_end = block_end->next;
            struct c_node* rr_block_start = block_end->next->next->next->next;
            struct c_node* rr_block_end = block_end->next->next->next->next;

            derive_node(node->l_child, out_string, ll_block_start, ll_block_end);
            derive_node(node->r_child, out_string, lr_block_start, lr_block_end);
            derive_node(node->l_child, out_string, rl_block_start, rl_block_end);
            derive_node(node->r_child, out_string, rr_block_start, rr_block_end);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, block_start, ']');
            insert_after(out_string, block_end, '[');
            insert_before(out_string, block_start->previous, '@');
            insert_after(out_string, block_end->next, '@');
            insert_before(out_string, block_start->previous->previous, '[');
            insert_after(out_string, block_end->next->next, ']');
            insert_before(out_string, block_start->previous->previous->previous, '*');
            insert_after(out_string, block_end->next->next->next, '*');
            insert_before(out_string, block_start->previous->previous->previous->previous, ']');
            insert_after(out_string, block_end->next->next->next->next, '[');
            insert_before(out_string, block_start->previous->previous->previous->previous->previous, '#');
            insert_after(out_string, block_end->next->next->next->next->next, '#');
            insert_before(out_string, block_start->previous->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next->next->next->next, ']');
            insert_before(out_string, block_start->previous->previous->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, block_end->next->next->next->next->next->next->next, ']');

            struct c_node* ll_block_start = block_start->previous->previous->previous->previous->previous->previous;
            struct c_node* ll_block_end = block_start->previous->previous->previous->previous->previous->previous;
            struct c_node* lr_block_start = block_start->previous->previous;
            struct c_node* lr_block_end = block_start->previous->previous;
            struct c_node* rl_block_start = block_end->next->next;
            struct c_node* rl_block_end = block_end->next->next;
            struct c_node* rr_block_start = block_end->next->next->next->next->next->next;
            struct c_node* rr_block_end = block_end->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_block_start, ll_block_end);
            derive_node(node->r_child, out_string, lr_block_start, lr_block_end);
            derive_node(node->l_child, out_string, rl_block_start, rl_block_end);
            derive_node(node->r_child, out_string, rr_block_start, rr_block_end);
        } 
    } else if(node->data->head->character == '/') {
        block_start->character == '-';
        insert_before(out_string, block_start, '@');
        insert_after(out_string, block_end, '@');
        insert_before(out_string, block_start->previous, '*');
        insert_after(out_string, block_end->next, '*');
        insert_before(out_string, block_start->previous->previous, '#');
        insert_after(out_string, block_end->next->next, '#');
        insert_before(out_string, block_start->previous->previous->previous, '[');
        insert_after(out_string, block_end->next->next->next, ']');
        insert_after(out_string, block_end->next->next->next->next, '/');
        insert_after(out_string, block_end->next->next->next->next->next, '[');
        insert_after(out_string, block_end->next->next->next->next->next->next, '@');
        insert_after(out_string, block_end->next->next->next->next->next->next->next, ']');
        insert_after(out_string, block_end->next->next->next->next->next->next->next->next, '^');
        insert_after(out_string, block_end->next->next->next->next->next->next->next->next->next, '2');

        struct c_node* ll_block_start = block_start->previous->previous->previous;
        struct c_node* ll_block_end = block_start->previous->previous->previous;
        struct c_node* lr_block_start = block_start->previous;
        struct c_node* lr_block_end = block_start->previous;
        struct c_node* rl_block_start = block_end->next;
        struct c_node* rl_block_end = block_end->next;
        struct c_node* rr_block_start = block_end->next->next->next;
        struct c_node* rr_block_end = block_end->next->next->next;
        struct c_node* d_block_start = block_end->next->next->next->next->next->next->next;
        struct c_node* d_block_end = block_end->next->next->next->next->next->next->next;

    } else if(node->data->head->character == '+') {
        block_start->character == '+';
    } else if(node->data->head->character == '-') {
        block_start->character == '-';
    }

    if(!is_operator(node->data->head->character)) {
        
    }
    return 0;
}