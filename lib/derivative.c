#include "derivative.h"
#include "char_list.h"
#include "tree.h"
#include "parser.h"
#include <stdio.h>

int get_func_der(struct tree* tree, struct string* out_string) {
    struct c_node* index = out_string->head;
    derive_node(tree->tree_head, out_string, index);
    return 0;
}

int derive_node(struct tree_node* node, struct string* out_string, struct c_node* index) {
    if(node == NULL) return 0;

    if(index == NULL) {
        head_string_insert(out_string, '#');
        index = out_string->head;
    }

    if(node->data->head->character == '*') {
        index->character = '+';

        if(!is_operator_pow(node->l_child->data->head->character) && !is_operator_pow(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_after(out_string, index, '@');
            insert_before(out_string, index->previous, '*');
            insert_after(out_string, index->next, '*');
            insert_before(out_string, index->previous->previous, '#');
            insert_after(out_string, index->next->next, '#');
            insert_before(out_string, index->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next, ']');

            struct c_node* ll_index = index->previous->previous->previous;
            struct c_node* lr_index = index->previous;
            struct c_node* rl_index = index->next;
            struct c_node* rr_index = index->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(is_operator_pow(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_after(out_string, index, '[');
            insert_before(out_string, index->previous, '*');
            insert_after(out_string, index->next, '@');
            insert_before(out_string, index->previous->previous, ']');
            insert_after(out_string, index->next->next->next, ']');
            insert_before(out_string, index->previous->previous->previous, '#');
            insert_after(out_string, index->next->next->next->next, '*');
            insert_before(out_string, index->previous->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next->next->next, '#');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next->next->next->next, ']');

            struct c_node* ll_index = index->previous->previous->previous->previous;
            struct c_node* lr_index = index->previous;
            struct c_node* rl_index = index->next->next;
            struct c_node* rr_index = index->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(!is_operator_pow(node->l_child->data->head->character) && is_operator_pow(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_after(out_string, index, '@');
            insert_before(out_string, index->previous, '@');
            insert_after(out_string, index->next, '*');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index->next->next->next, '[');
            insert_before(out_string, index->previous->previous->previous, '*');
            insert_after(out_string, index->next->next->next->next, '#');
            insert_before(out_string, index->previous->previous->previous->previous, '#');
            insert_after(out_string, index->next->next->next->next->next, ']');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next->next->next->next, ']');

            struct c_node* ll_index = index->previous->previous->previous->previous->previous;
            struct c_node* lr_index = index->previous->previous;
            struct c_node* rl_index = index->next;
            struct c_node* rr_index = index->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(is_operator_pow(node->l_child->data->head->character) && is_operator_pow(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_after(out_string, index, '[');
            insert_before(out_string, index->previous, '@');
            insert_after(out_string, index->next, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index->next->next, ']');
            insert_before(out_string, index->previous->previous->previous, '*');
            insert_after(out_string, index->next->next->next, '*');
            insert_before(out_string, index->previous->previous->previous->previous, ']');
            insert_after(out_string, index->next->next->next->next, '[');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '#');
            insert_after(out_string, index->next->next->next->next->next, '#');
            insert_before(out_string, index->previous->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next->next->next->next, ']');
            insert_before(out_string, index->previous->previous->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next, ']');

            struct c_node* ll_index = index->previous->previous->previous->previous->previous->previous;
            struct c_node* lr_index = index->previous->previous;
            struct c_node* rl_index = index->next->next;
            struct c_node* rr_index = index->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
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
        index->character == '+';
    } else if(node->data->head->character == '-') {
        index->character == '-';
    }

    if(node->data->head->character == '^' && index->character == '@') {
        index->character = '^';
        insert_before(out_string, index, 'x');
        struct c_node* t_index = node->r_child->data->tail;
        while(t_index != NULL) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
        }
    }

    return 0;
}