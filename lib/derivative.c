#include "derivative.h"
#include "char_list.h"
#include "tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int get_func_der(tree* tree, string* out_string) {
    c_node* index = out_string->head;
    derive_node(tree->tree_head, out_string, index);
    return 0;
}

int derive_node(tree_node* node, string* out_string, c_node* index) {
    if(node == NULL) return 0;

    if(index == NULL) {
        head_string_insert(out_string, '#');
        index = out_string->head;
    }

    if(node->data->head->character == '_') {
        if(node->data->head->next->character == 'x' && index->character == '#') {
            index->character = '1';
            insert_before(out_string, index, '_');
            insert_before(out_string, index->previous, '[');
            insert_after(out_string, index, ']');
        } else if(node->data->head->next->character == 'x' && index->character == '@') {
            index->character = 'x';
            insert_before(out_string, index, '_');
            insert_before(out_string, index->previous, '[');
            insert_after(out_string, index, ']');
        } else if(isdigit(node->data->head->next->character) && index->character == '#') {
            index->character = '0';
        } else if(isdigit(node->data->head->next->character) && index->character == '@') {
            index->character = '_';
            insert_before(out_string, index, '[');
            c_node* t_index = node->data->tail;
            while(t_index->character != '_') {
                insert_after(out_string, index, t_index->character);
                t_index = t_index->previous;
            }
        }
    }
    if(node->data->head->character == '*' && index->character == '#') {
        index->character = '+';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_after(out_string, index, '@');
            insert_before(out_string, index->previous, '*');
            insert_after(out_string, index->next, '*');
            insert_before(out_string, index->previous->previous, '#');
            insert_after(out_string, index->next->next, '#');
            insert_before(out_string, index->previous->previous->previous, '[');
            insert_after(out_string, index->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous;
            c_node* lr_index = index->previous;
            c_node* rl_index = index->next;
            c_node* rr_index = index->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_before(out_string, index->previous, '*');
            insert_before(out_string, index->previous->previous, ']');
            insert_before(out_string, index->previous->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            insert_after(out_string, index->next->next->next, '*');
            insert_after(out_string, index->next->next->next->next, '#');
            insert_after(out_string, index->next->next->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous->previous;
            c_node* lr_index = index->previous;
            c_node* rl_index = index->next->next;
            c_node* rr_index = index->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous, '*');
            insert_before(out_string, index->previous->previous->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index, '@');
            insert_after(out_string, index->next, '*');
            insert_after(out_string, index->next->next, '[');
            insert_after(out_string, index->next->next->next, '#');
            insert_after(out_string, index->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous->previous->previous;
            c_node* lr_index = index->previous->previous;
            c_node* rl_index = index->next;
            c_node* rr_index = index->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
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

            c_node* ll_index = index->previous->previous->previous->previous->previous->previous;
            c_node* lr_index = index->previous->previous;
            c_node* rl_index = index->next->next;
            c_node* rr_index = index->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
        } 
    } else if(node->data->head->character == '/' && index->character == '#') {
        index->character = '-';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_before(out_string, index->previous, '*');
            insert_before(out_string, index->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous, '[');
            insert_after(out_string, index, '@');
            insert_after(out_string, index->next, '*');
            insert_after(out_string, index->next->next, '#');
            insert_after(out_string, index->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next, '/');
            insert_after(out_string, index->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next, '@');
            insert_after(out_string, index->next->next->next->next->next->next->next, '^');
            insert_after(out_string, index->next->next->next->next->next->next->next->next, '2');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next, ']');
            

            c_node* ll_index = index->previous->previous->previous;
            c_node* lr_index = index->previous;
            c_node* rl_index = index->next;
            c_node* rr_index = index->next->next->next;
            c_node* d_index = index->next->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
            derive_node(node->r_child, out_string, d_index);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '@');
            insert_before(out_string, index->previous, '*');
            insert_before(out_string, index->previous->previous, ']');
            insert_before(out_string, index->previous->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            insert_after(out_string, index->next->next->next, '*');
            insert_after(out_string, index->next->next->next->next, '#');
            insert_after(out_string, index->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next, '/');
            insert_after(out_string, index->next->next->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next->next, '@');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next, '^');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next, '2');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous->previous;
            c_node* lr_index = index->previous;
            c_node* rl_index = index->next->next;
            c_node* rr_index = index->next->next->next->next->next;
            c_node* d_index = index->next->next->next->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
            derive_node(node->r_child, out_string, d_index);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous, '*');
            insert_before(out_string, index->previous->previous->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index, '@');
            insert_after(out_string, index->next, '*');
            insert_after(out_string, index->next->next, '[');
            insert_after(out_string, index->next->next->next, '#');
            insert_after(out_string, index->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next, '/');
            insert_after(out_string, index->next->next->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next, '@');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next, '^');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next, '2');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous->previous->previous;
            c_node* lr_index = index->previous->previous;
            c_node* rl_index = index->next;
            c_node* rr_index = index->next->next->next->next;
            c_node* d_index = index->next->next->next->next->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
            derive_node(node->r_child, out_string, d_index);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous, '*');
            insert_before(out_string, index->previous->previous->previous->previous, ']');
            insert_before(out_string, index->previous->previous->previous->previous->previous, '#');
            insert_before(out_string, index->previous->previous->previous->previous->previous->previous, '[');
            insert_before(out_string, index->previous->previous->previous->previous->previous->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            insert_after(out_string, index->next->next->next, '*');
            insert_after(out_string, index->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next, '#');
            insert_after(out_string, index->next->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next->next->next, '/');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next, '[');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next, '@');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next, ']');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next->next, '^');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next->next->next, '2');
            insert_after(out_string, index->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next, ']');

            c_node* ll_index = index->previous->previous->previous->previous->previous->previous;
            c_node* lr_index = index->previous->previous;
            c_node* rl_index = index->next->next;
            c_node* rr_index = index->next->next->next->next->next->next;
            c_node* d_index = index->next->next->next->next->next->next->next->next->next->next->next->next;

            derive_node(node->l_child, out_string, ll_index);
            derive_node(node->r_child, out_string, lr_index);
            derive_node(node->l_child, out_string, rl_index);
            derive_node(node->r_child, out_string, rr_index);
            derive_node(node->r_child, out_string, d_index);
        } 
    } else if((node->data->head->character == '+' || node->data->head->character == '-') && index->character == '#') {
        if(node->data->head->character == '+') {
            index->character = '+';
        }
        if(node->data->head->character == '-') {
            index->character = '-';
        }
            

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '#');
            insert_after(out_string, index, '#');

            c_node* l_index = index->previous;
            c_node* r_index = index->next;

            derive_node(node->l_child, out_string, l_index);
            derive_node(node->r_child, out_string, r_index);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, '#');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '#');
            insert_after(out_string, index->next->next, ']');

            c_node* l_index = index->previous;
            c_node* r_index = index->next->next;

            derive_node(node->l_child, out_string, l_index);
            derive_node(node->r_child, out_string, r_index);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '#');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '#');

            c_node* l_index = index->previous->previous;
            c_node* r_index = index->next;

            derive_node(node->l_child, out_string, l_index);
            derive_node(node->r_child, out_string, r_index);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '#');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '#');
            insert_after(out_string, index->next->next, ']');

            c_node* l_index = index->previous->previous;
            c_node* r_index = index->next->next;

            derive_node(node->l_child, out_string, l_index);
            derive_node(node->r_child, out_string, r_index);
        }
    } else if(node->data->head->character == '^' && index->character == '#' && (is_operator(node->l_child->data->head->character) || node->l_child->data->head->character == 'x')) {
        index->character = '^';

        insert_before(out_string, index, ']');
        insert_before(out_string, index->previous, '@');
        insert_before(out_string, index->previous->previous, '[');
        insert_before(out_string, index->previous->previous->previous, '*');

        c_node* t_index = node->r_child->data->head;
        int digits = 0;
        while(t_index != NULL) {
            insert_before(out_string, index->previous->previous->previous->previous, t_index->character);
            t_index = t_index->next;
            digits++;
        }
        t_index = index->previous->previous->previous->previous;
        for(int i = 0 ; i < digits ; i++) {
            t_index = t_index->previous;
        }
        insert_before(out_string, t_index, '[');
        insert_before(out_string, t_index->previous, '[');

        string* new_exponent_str = (string*)malloc(sizeof(string));
        create_string(new_exponent_str);
        int exponent = get_number(node->r_child->data);
        exponent--;
        to_string(new_exponent_str, exponent);

        digits = 0;
        t_index = new_exponent_str->tail;
        while(t_index != NULL) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
            digits++;
        }
        t_index = index;
        for(int i = 0 ; i < digits ; i++) {
            t_index = t_index->next;
        }
        insert_after(out_string, t_index, ']');
        insert_after(out_string, t_index->next, '*');
        insert_after(out_string, t_index->next->next, '[');
        insert_after(out_string, t_index->next->next->next, '#');
        insert_after(out_string, t_index->next->next->next->next, ']');
        insert_after(out_string, t_index->next->next->next->next->next, ']');

        c_node* r_index = t_index->next->next->next->next;
        c_node* l_index = index->previous->previous;
        delete_string(new_exponent_str);

        derive_node(node->l_child, out_string, l_index);
        derive_node(node->l_child, out_string, r_index);
    } else if(node->data->head->character == '^' && index->character == '@' && is_operator(node->l_child->data->head->character)) {
        index->character = '^';
        insert_before(out_string, index, ']');
        insert_before(out_string, index->previous, '@');
        insert_before(out_string, index->previous->previous, '[');
        c_node* t_index = node->r_child->data->tail;
        while(t_index != NULL) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
        }
        derive_node(node->l_child, out_string, index->previous->previous);
    } else if(node->data->head->character == '^' && index->character == '#' && isdigit(node->l_child->data->head->character)) {
        index->character = '0';
    } else if(node->data->head->character == '^' && index->character == '@' && isdigit(node->l_child->data->head->character)) {
        index->character = '^';
        c_node* t_index = node->r_child->data->tail;
        while(t_index != NULL) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
        }
        t_index = node->l_child->data->head;
        while(t_index != NULL) {
            insert_before(out_string, index, t_index->character);
            t_index = t_index->next;
        }
    } else if(node->data->head->character == '^' && index->character == '@' && node->l_child->data->head->character == 'x') {
        index->character = '^';
        insert_before(out_string, index, 'x');
        c_node* t_index = node->r_child->data->tail;
        while(t_index != NULL) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
        }
    } else if(index->character == '@' && node->data->head->character == '+') {
        index->character = '+';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, '@');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            derive_node(node->l_child, out_string, index->previous->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '[');
            insert_before(out_string, index, '@');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next->next);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->r_child, out_string, index->next->next);
            derive_node(node->l_child, out_string, index->previous->previous);
        }
    } else if(index->character == '@' && node->data->head->character == '-') {
        index->character = '-';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, '@');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            derive_node(node->l_child, out_string, index->previous->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '[');
            insert_before(out_string, index, '@');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next->next);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->r_child, out_string, index->next->next);
            derive_node(node->l_child, out_string, index->previous->previous);
        }
    } else if(index->character == '@' && node->data->head->character == '*') {
        index->character = '*';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, '@');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            derive_node(node->l_child, out_string, index->previous->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '[');
            insert_before(out_string, index, '@');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next->next);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->r_child, out_string, index->next->next);
            derive_node(node->l_child, out_string, index->previous->previous);
        }
    } else if(index->character == '@' && node->data->head->character == '/') {
        index->character = '/';

        if(!is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, '@');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(is_operator(node->l_child->data->head->character) && !is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            derive_node(node->l_child, out_string, index->previous->previous);
            derive_node(node->r_child, out_string, index->next);
        } else if(!is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_after(out_string, index, '[');
            insert_before(out_string, index, '@');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->l_child, out_string, index->previous);
            derive_node(node->r_child, out_string, index->next->next);
        } else if(is_operator(node->l_child->data->head->character) && is_operator(node->r_child->data->head->character)) {
            insert_before(out_string, index, ']');
            insert_before(out_string, index->previous, '@');
            insert_before(out_string, index->previous->previous, '[');
            insert_after(out_string, index, '[');
            insert_after(out_string, index->next, '@');
            insert_after(out_string, index->next->next, ']');
            derive_node(node->r_child, out_string, index->next->next);
            derive_node(node->l_child, out_string, index->previous->previous);
        }
    } else if(node->data->head->character == 'x' && index->character == '@') {
        index->character = 'x';
    } else if(node->data->head->character == 'x' && index->character == '#') {
        index->character = '1';
    } else if(isdigit(node->data->head->character) && index->character == '@') {
        index->character = node->data->head->character;
        c_node* t_index = node->data->tail;
        while(t_index != node->data->head) {
            insert_after(out_string, index, t_index->character);
            t_index = t_index->previous;
        }
    } else if(isdigit(node->data->head->character) && index->character == '#') {
        index->character = '0';
    }

    return 0;
}