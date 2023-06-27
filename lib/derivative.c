#include "derivative.h"
#include "char_list.h"
#include "tree.h"
#include "parser.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int get_func_der(tree* tree, string* out_string) {
    c_node* index = get_head_str(out_string);
    derive_node(get_head_tree(tree), out_string, index);
    return 0;
}

int derive_node(tree_node* node, string* out_string, c_node* index) {
    if(node == NULL) return 0;

    if(index == NULL) {
        head_string_insert(out_string, '#');
        index = get_head_str(out_string);
    }

    if(get_char(get_head_cn_from_tree_node(node)) == '_') {
        if(get_char(get_next_str(get_head_cn_from_tree_node(node))) == 'x' && get_char(index) == '#') {
            set_char(index, '1');
            insert_before_l(out_string, index, "[_");
            insert_after(out_string, index, ']');
        } else if(get_char(get_next_str(get_head_cn_from_tree_node(node))) == 'x' && get_char(index) == '@') {
            set_char(index, 'x');
            insert_before_l(out_string, index, "[_");
            insert_after(out_string, index, ']');
        } else if(isdigit(get_char(get_next_str(get_head_cn_from_tree_node(node)))) && get_char(index) == '#') {
            set_char(index, '0');
        } else if(isdigit(get_char(get_next_str(get_head_cn_from_tree_node(node)))) && get_char(index) == '@') {
            set_char(index, '_');
            insert_before(out_string, index, '[');
            c_node* t_index = get_tail_str(get_data(node));
            while(get_char(t_index) != '_') {
                insert_after(out_string, index, get_char(t_index));
                to_prev_list(&t_index);
            }
        }
    }
    if(get_char(get_head_cn_from_tree_node(node)) == '*' && get_char(index) == '#') {
        set_char(index, '+');

        if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#*@");
            insert_after_l(out_string, index, "@*#]");

            c_node* ll_index = get_prev_str_m(index, 3);
            c_node* lr_index = get_prev_str_m(index, 1);
            c_node* rl_index = get_next_str_m(index, 1);
            c_node* rr_index = get_next_str_m(index, 3);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[[#]*@");
            insert_after_l(out_string, index, "[@]*#]");

            c_node* ll_index = get_prev_str_m(index, 4);
            c_node* lr_index = get_prev_str_m(index, 1);
            c_node* rl_index = get_next_str_m(index, 2);
            c_node* rr_index = get_next_str_m(index, 5);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
        } else if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#*[@]");
            insert_after_l(out_string, index, "@*[#]]");

            c_node* ll_index = get_prev_str_m(index, 5);
            c_node* lr_index = get_prev_str_m(index, 2);
            c_node* rl_index = get_next_str_m(index, 1);
            c_node* rr_index = get_next_str_m(index, 4);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[[#]*[@]");
            insert_after_l(out_string, index, "[@]*[#]]");

            c_node* ll_index = get_prev_str_m(index, 6);
            c_node* lr_index = get_prev_str_m(index, 2);
            c_node* rl_index = get_next_str_m(index, 2);
            c_node* rr_index = get_next_str_m(index, 6);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
        } 
    } else if(get_char(get_head_cn_from_tree_node(node)) == '/' && get_char(index) == '#') {
        set_char(index, '-');

        if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#*@");
            insert_after_l(out_string, index, "@*#]/[@^2]");

            c_node* ll_index = get_prev_str_m(index, 3);
            c_node* lr_index = get_prev_str_m(index, 1);
            c_node* rl_index = get_next_str_m(index, 1);
            c_node* rr_index = get_next_str_m(index, 3);
            c_node* d_index = get_next_str_m(index, 7);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
            derive_node(get_r_child(node), out_string, d_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[[#]*@");
            insert_after_l(out_string, index, "[@]*#]/[@^2]");

            c_node* ll_index = get_prev_str_m(index, 4);
            c_node* lr_index = get_prev_str_m(index, 1);
            c_node* rl_index = get_next_str_m(index, 2);
            c_node* rr_index = get_next_str_m(index, 5);
            c_node* d_index = get_next_str_m(index, 9);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
            derive_node(get_r_child(node), out_string, d_index);
        } else if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#*[@]");
            insert_after_l(out_string, index, "@*[#]]/[[@]^2]");

            c_node* ll_index = get_prev_str_m(index, 5);
            c_node* lr_index = get_prev_str_m(index, 2);
            c_node* rl_index = get_next_str_m(index, 1);
            c_node* rr_index = get_next_str_m(index, 4);
            c_node* d_index = get_next_str_m(index, 10);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
            derive_node(get_r_child(node), out_string, d_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[[#]*[@]");
            insert_after_l(out_string, index, "[@]*[#]]/[[@]^2]");

            c_node* ll_index = get_prev_str_m(index, 6);
            c_node* lr_index = get_prev_str_m(index, 2);
            c_node* rl_index = get_next_str_m(index, 2);
            c_node* rr_index = get_next_str_m(index, 6);
            c_node* d_index = get_next_str_m(index, 12);

            derive_node(get_l_child(node), out_string, ll_index);
            derive_node(get_r_child(node), out_string, lr_index);
            derive_node(get_l_child(node), out_string, rl_index);
            derive_node(get_r_child(node), out_string, rr_index);
            derive_node(get_r_child(node), out_string, d_index);
        } 
    } else if((get_char(get_head_cn_from_tree_node(node)) == '+' || get_char(get_head_cn_from_tree_node(node)) == '-') && get_char(index) == '#') {
        if(get_char(get_head_cn_from_tree_node(node)) == '+') {
            set_char(index, '+');
        }
        if(get_char(get_head_cn_from_tree_node(node)) == '-') {
            set_char(index, '-');
        }
            

        if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before(out_string, index, '#');
            insert_after(out_string, index, '#');

            c_node* l_index = get_prev_str(index);
            c_node* r_index = get_next_str(index);

            derive_node(get_l_child(node), out_string, l_index);
            derive_node(get_r_child(node), out_string, r_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before(out_string, index, '#');
            insert_after_l(out_string, index, "[#]");

            c_node* l_index = get_prev_str(index);
            c_node* r_index = get_next_str(get_next_str(index));

            derive_node(get_l_child(node), out_string, l_index);
            derive_node(get_r_child(node), out_string, r_index);
        } else if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#]");
            insert_after(out_string, index, '#');

            c_node* l_index = get_prev_str(get_prev_str(index));
            c_node* r_index = get_next_str(index);

            derive_node(get_l_child(node), out_string, l_index);
            derive_node(get_r_child(node), out_string, r_index);
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[#]");
            insert_after_l(out_string, index, "[#]");

            c_node* l_index = get_prev_str(get_prev_str(index));
            c_node* r_index = get_next_str(get_next_str(index));

            derive_node(get_l_child(node), out_string, l_index);
            derive_node(get_r_child(node), out_string, r_index);
        }
    } else if(get_char(get_head_cn_from_tree_node(node)) == '^' && get_char(index) == '#' && (is_operator(get_char(get_head_str(get_data(get_l_child(node))))) || get_char(get_head_str(get_data(get_l_child(node)))) == 'x')) {
        set_char(index, '^');

        insert_before_l(out_string, index, "*[@]");

        c_node* t_index = get_head_str(get_data(get_r_child(node)));
        int digits = 0;
        while(t_index != NULL) {
            insert_before(out_string, get_prev_str(get_prev_str(get_prev_str(get_prev_str(index)))), get_char(t_index));
            to_next_list(&t_index);
            digits++;
        }
        t_index = get_prev_str(get_prev_str(get_prev_str(get_prev_str(index))));
        for(int i = 0 ; i < digits ; i++) {
            to_prev_list(&t_index);
        }
        insert_before_l(out_string, t_index, "[[");

        string* new_exponent_str = (string*)malloc(sizeof(string));
        create_string(new_exponent_str);
        int exponent = get_number(get_data(get_r_child(node)));
        exponent--;
        to_string(new_exponent_str, exponent);

        digits = 0;
        t_index = get_tail_str(new_exponent_str);
        while(t_index != NULL) {
            insert_after(out_string, index, get_char(t_index));
            to_prev_list(&t_index);
            digits++;
        }
        t_index = index;
        for(int i = 0 ; i < digits ; i++) {
            to_next_list(&t_index);
        }
        insert_after_l(out_string, t_index, "*[#]]");

        c_node* r_index = get_next_str(get_next_str(get_next_str(get_next_str(t_index))));
        c_node* l_index = get_prev_str(get_prev_str(index));
        delete_string(new_exponent_str);

        derive_node(get_l_child(node), out_string, l_index);
        derive_node(get_r_child(node), out_string, r_index);
    } else if(get_char(get_head_cn_from_tree_node(node)) == '^' && get_char(index) == '@' && is_operator(get_char(get_head_str(get_data(get_l_child(node)))))) {
        set_char(index, '^');
        insert_before_l(out_string, index, "[@]");
        c_node* t_index = get_tail_str(get_data(get_r_child(node)));
        while(t_index != NULL) {
            insert_after(out_string, index, get_char(t_index));
            to_prev_list(&t_index);
        }
        derive_node(get_l_child(node), out_string, get_prev_str(get_prev_str(index)));
    } else if(get_char(get_head_cn_from_tree_node(node)) == '^' && get_char(index) == '#' && isdigit(get_char(get_head_str(get_data(get_l_child(node)))))) {
        set_char(index, '0');
    } else if(get_char(get_head_cn_from_tree_node(node)) == '^' && get_char(index) == '@' && isdigit(get_char(get_head_str(get_data(get_l_child(node)))))) {
        set_char(index, '^');
        c_node* t_index = get_tail_str(get_data(get_r_child(node)));
        while(t_index != NULL) {
            insert_after(out_string, index, get_char(t_index));
            to_prev_list(&t_index);
        }
        t_index = get_head_str(get_data(get_l_child(node)));
        while(t_index != NULL) {
            insert_before(out_string, index, get_char(t_index));
            to_next_list(&t_index);
        }
    } else if(get_char(get_head_cn_from_tree_node(node)) == '^' && get_char(index) == '@' && get_char(get_head_str(get_data(get_l_child(node)))) == 'x') {
        set_char(index, '^');
        insert_before(out_string, index, 'x');
        c_node* t_index = get_tail_str(get_data(get_r_child(node)));
        while(t_index != NULL) {
            insert_after(out_string, index, get_char(t_index));
            to_prev_list(&t_index);
        }
    } else if(get_char(index) == '@' && (get_char(get_head_cn_from_tree_node(node)) == '+' || get_char(get_head_cn_from_tree_node(node)) == '-' || get_char(get_head_cn_from_tree_node(node)) == '*' || get_char(get_head_cn_from_tree_node(node)) == '/')) {
        if(get_char(get_head_cn_from_tree_node(node)) == '+') {
            set_char(index, '+');
        } else if(get_char(get_head_cn_from_tree_node(node)) == '-') {
            set_char(index, '-');
        } else if(get_char(get_head_cn_from_tree_node(node)) == '*') {
            set_char(index, '*');
        } else if(get_char(get_head_cn_from_tree_node(node)) == '/') {
            set_char(index, '/');
        }

        if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_after(out_string, index, '@');
            insert_before(out_string, index, '@');
            derive_node(get_l_child(node), out_string, get_prev_str(index));
            derive_node(get_r_child(node), out_string, get_next_str(index));
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && !is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_after(out_string, index, '@');
            insert_before_l(out_string, index, "[@]");
            derive_node(get_l_child(node), out_string, get_prev_str(get_prev_str(index)));
            derive_node(get_r_child(node), out_string, get_next_str(index));
        } else if(!is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_after_l(out_string, index, "[@]");
            insert_before(out_string, index, '@');
            derive_node(get_l_child(node), out_string, get_prev_str(index));
            derive_node(get_r_child(node), out_string, get_next_str(get_next_str(index)));
        } else if(is_operator(get_char(get_head_str(get_data(get_l_child(node))))) && is_operator(get_char(get_head_str(get_data(get_r_child(node)))))) {
            insert_before_l(out_string, index, "[@]");
            insert_after_l(out_string, index, "[@]");
            derive_node(get_r_child(node), out_string, get_next_str(get_next_str(index)));
            derive_node(get_l_child(node), out_string, get_prev_str(get_prev_str(index)));
        }
    } else if(get_char(get_head_cn_from_tree_node(node)) == 'x' && get_char(index) == '@') {
        set_char(index, 'x');
    } else if(get_char(get_head_cn_from_tree_node(node)) == 'x' && get_char(index) == '#') {
        set_char(index, '1');
    } else if(isdigit(get_char(get_head_cn_from_tree_node(node))) && get_char(index) == '@') {
        set_char(index, get_char(get_head_cn_from_tree_node(node)));
        c_node* t_index = get_tail_str(get_data(node));
        while(t_index != get_head_str(get_data(node))) {
            insert_after(out_string, index, get_char(t_index));
            to_prev_list(&t_index);
        }
    } else if(isdigit(get_char(get_head_cn_from_tree_node(node))) && get_char(index) == '#') {
        set_char(index, '0');
    }

    return 0;
}