#include "parser.h"
#include "char_list.h"
#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int expand_string(string* str) {
    c_node* index = get_head_str(str);

    while(get_next_str(index) != NULL) {
        if((isdigit(get_char(index)) || get_char(index) == 'x') && get_char(get_next_str(index)) == '[') insert_after(str, index, '*');

        else if(get_char(index) == ']' && (isdigit(get_char(get_next_str(index))) || get_char(get_next_str(index)) == 'x')) insert_after(str, index, '*');

        else if(get_char(index) == ']' && get_char(get_next_str(index)) == '[') insert_after(str, index, '*');

        else if(isdigit(get_char(index)) && get_char(get_next_str(index)) == 'x') insert_after(str, index, '*');

        else if(get_prev_str(index) == NULL) {
            if(get_char(index) == '-' && isdigit(get_char(get_next_str(index)))) {
                set_char(index, '_');
            }
            if(get_char(index) == '-' && get_char(get_next_str(index)) == 'x') {
                set_char(index, '_');
            }
        } else if(get_prev_str(index) != NULL) {
            if(get_char(get_prev_str(index)) == '[' && get_char(index) == '-' && isdigit(get_char(get_next_str(index)))) {
                set_char(index, '_');
            }
            if(get_char(get_prev_str(index)) == '[' && get_char(index) == '-' && get_char(get_next_str(index)) == 'x') {
                set_char(index, '_');
            }
        }

        to_next_list(&index);
    }
    return 0;
}

int splice(string* str) {
    c_node* block_start = get_head_str(str);
    c_node* block_end = get_head_str(str);
    
    
    while(block_end != NULL) {
        if(get_char(block_start) == '(' || get_char(block_start) == ')' || get_char(block_start) == '[' || get_char(block_start) == ']') {
            to_next_list(&block_start);
            block_end = block_start;
        } else if(isdigit(get_char(block_start))) {
            while(1) {
                if(get_next_str(block_end) == NULL) break;
                if(!isdigit(get_char(get_next_str(block_end)))) break;
                to_next_list(&block_end);
            }
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            to_next_list(&block_end);
            block_start = block_end;
        } else if(get_char(block_start) == '_' && isdigit(get_char(get_next_str(block_start)))) {
            to_next_list(&block_end);
            while(1) {
                if(get_next_str(block_end) == NULL) break;
                if(!isdigit(get_char(get_next_str(block_end)))) break;
                to_next_list(&block_end);
            }
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            to_next_list(&block_end);
            block_start = block_end;
        } else if(get_char(block_start) == '_' && get_char(get_next_str(block_start)) == 'x') {
            to_next_list(&block_end);
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            to_next_list(&block_end);
            block_start = block_end;
        } else if(get_char(block_start) == 'x') {
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            to_next_list(&block_end);
            block_start = block_end;
        } else if(is_operator(get_char(block_start))) {
            to_next_list(&block_start);
            block_end = block_start;
        }
    }

    c_node* index = get_head_str(str);
    int count = 0;
    do {
        if(get_char(index) == '(') count++;
        if(get_char(index) == ')') count--;
        if(get_char(index) == '[') count++;
        if(get_char(index) == ']') count--;
        to_next_list(&index);
    } while (count != 0);

    if(index != NULL) {
        head_string_insert(str, '(');
        tail_string_insert(str, ')');
    }

    return 0;
}

int is_operator(char key) {
    if (key == '+') return true;
    if (key == '-') return true;
    if (key == '*') return true;
    if (key == '/') return true;
    if (key == '^') return true;

    return false;
}

int is_operator_pow(char key) {
    if (key == '+') return true;
    if (key == '-') return true;
    if (key == '*') return true;
    if (key == '/') return true;

    return false;
}

int move_to_next_block(c_node** index) {
    int count = 0;

    do {
        to_next_list(index);

        if(get_char(*index) == '(' || get_char(*index) == '[') {
            count++;
        } else if(get_char(*index) == ')' || get_char(*index) == ']') {
            count--;
        }
    } while (count != 0);

    return 0;
}

int move_to_previous_block(c_node** index) {
    int count = 0;

    do {
        to_prev_list(index);

        if(get_char(*index) == '(' || get_char(*index) == '[') {
            count++;
        } else if(get_char(*index) == ')' || get_char(*index) == ']') {
            count--;
        }
    } while (count != 0);

    return 0;
}

int check_left_op(c_node* index, c_node* block_limit) {
    while(index != block_limit) {
        if(is_operator(get_char(index))) {
            return true;
        }
        to_prev_list(&index);
    }
    return false;
}

int check_right_op(c_node* index, c_node* block_limit) {
    while(index != block_limit) {
        if(is_operator(get_char(index))) {
            return true;
        }
        to_next_list(&index);
    }
    return false;
}

int group(c_node* block_start, c_node* block_end, string* str, bool should_restart, c_node* index) {
    if(is_leaf(block_start, block_end)) {
        return 0;
    } else {
        if(should_restart == true) {
            index = block_start;
            move_to_next_block(&index);
            to_next_list(&index);
        }

        c_node* l_block_end = get_prev_str(index);
        c_node* l_block_start = index;
        c_node* r_block_start = get_next_str(index);
        c_node* r_block_end = index;
        move_to_previous_block(&l_block_start);
        move_to_next_block(&r_block_end);

        if(!check_left_op(l_block_start, block_start) && !check_right_op(r_block_end, block_end)) {
            group(l_block_start, l_block_end, str, true, index);
            group(r_block_start, r_block_end, str, true, index);            
        } else if(check_left_op(l_block_start, block_start) && !check_right_op(r_block_end, block_end)) {
            insert_before(str, l_block_start, '(');
            insert_after(str, r_block_end, ')');
            group(block_start, block_end, str, true, index);
        } else {
            if(get_char(get_next_str(r_block_end)) == '+' || get_char(get_next_str(r_block_end)) == '-') {
                insert_before(str, l_block_start, '(');
                insert_after(str, r_block_end, ')');
                group(block_start, block_end, str, true, index);
            } else if(get_char(get_next_str(r_block_end)) == '*' || get_char(get_next_str(r_block_end)) == '/') {
                if(get_char(index) == '^' || get_char(index) == '+' || get_char(index) == '-') {
                    move_to_next_block(&index);
                    to_next_list(&index);
                    group(block_start, block_end, str, false, index);
                } else if(get_char(index) == '*' || get_char(index) == '/') {
                    insert_before(str, l_block_start, '(');
                    insert_after(str, r_block_end, ')');
                    group(block_start, block_end, str, true, index);
                }
            } else if(get_char(get_next_str(r_block_end)) == '^') {
                move_to_next_block(&index);
                to_next_list(&index);
                group(block_start, block_end, str, false, index);
            }
        }
    }
    return 0;
}

int group_string(string* str) {
    group(get_head_str(str), get_tail_str(str), str, true, get_head_str(str));
    return 0;
}

int load_tree(tree* out_tree, string* str) {
    load_tree_block(get_head_str(str), get_tail_str(str), get_head_tree(out_tree), out_tree, str);
    return 0;
}

int contract(string* str) {
    c_node* index = get_head_str(str);

    while(get_next_str(index) != NULL) {
        if(get_next_str(index) != NULL) {
            if(get_next_str(get_next_str(index)) != NULL) {
                if(((isdigit(get_char(index))) || (get_char(index) == 'x')) && (get_char(get_next_str(index)) == '*') && (get_char(get_next_str(get_next_str(index))) == '(')) {
                    c_node* t = get_next_str(index);
                    c_node* t_1 = get_next_str(get_next_str(index));
                    set_c_node_prev(&t_1, index);
                    set_c_node_next(&index, t_1);
                    free(t);
                }

                if((get_char(index) == ')') && ((isdigit(get_char(get_next_str(get_next_str(index))))) || (get_char(get_next_str(get_next_str(index))) == 'x')) && (get_char(get_next_str(index)) == '*')) {
                    c_node* t = get_next_str(index);
                    c_node* t_1 = get_next_str(get_next_str(index));
                    set_c_node_prev(&t_1, index);
                    set_c_node_next(&index, t_1);
                    free(t);
                }

                if((get_char(index) == ')') && (get_char(get_next_str(index)) == '*') && (get_char(get_next_str(get_next_str(index))) == '(')) {
                    c_node* t = get_next_str(index);
                    c_node* t_1 = get_next_str(get_next_str(index));
                    set_c_node_prev(&t_1, index);
                    set_c_node_next(&index, t_1);
                    free(t);
                }

                if((isdigit(get_char(index))) && (get_char(get_next_str(index)) == '*') && (get_char(get_next_str(get_next_str(index))) == 'x')) {
                    c_node* t = get_next_str(index);
                    c_node* t_1 = get_next_str(get_next_str(index));
                    set_c_node_prev(&t_1, index);
                    set_c_node_next(&index, t_1);
                    free(t);
                }
            }
        }
        to_next_list(&index);
    }

    return 0;
}

int par_check(string* string) {
    c_node* l_index = get_head_str(string);
    c_node* r_index = get_tail_str(string);

    if(!is_leaf(l_index, r_index)) {
        r_index = get_head_str(string);
        while(l_index != NULL) {
            while(get_char(l_index) != '[') {
                to_next_list(&l_index);
                if(l_index == NULL) goto cycle_exit;
            }
            r_index = l_index;
            int count = 0;

            do {
                if(get_char(r_index) == '(' || get_char(r_index) == '[') {
                    count++;
                } else if(get_char(r_index) == ')' || get_char(r_index) == ']') {
                    count--;
                }
                if(count != 0)
                    to_next_list(&r_index);
            } while (count != 0);

            cycle_start:

            if(get_char(get_next_str(l_index)) == '[' && get_char(get_prev_str(r_index)) == ']') {
                c_node* t_index = l_index;
                count = 0;
                move_to_next_block(&t_index);
                to_next_list(&t_index);

                if(t_index == r_index) {
                    c_node* t = get_next_str(l_index);
                    set_c_node_next(&l_index, get_next_str(get_next_str(l_index)));
                    c_node* c = get_next_str(l_index);
                    set_c_node_prev(&c, l_index);
                    free(t);
                    t = get_prev_str(r_index);
                    set_c_node_prev(&r_index, get_prev_str(get_prev_str(r_index)));
                    c = get_prev_str(r_index);
                    set_c_node_next(&c, r_index);
                    free(t);
                    goto cycle_start;
                }
            }
            to_next_list(&l_index);
        }
    }
    cycle_exit:
    return 0;
}

int mult_delete(string* string) {
    c_node* index = get_head_str(string);
    while(true) {
        c_node* tl_index = NULL;
        c_node* tr_index = NULL;
        
        if(index == NULL) goto cycle_exit;
        do {
            to_next_list(&index);
            if(index == NULL) goto cycle_exit;
        } while(!is_operator(get_char(index)));

        c_node* l_index = index;
        c_node* r_index = index;

        if(get_char(index) == '*') {
            if(get_char(get_prev_str(index)) == '0' && !isdigit(get_char(get_prev_str(get_prev_str(index))))) {
                to_prev_list(&l_index);
                if(get_char(get_next_str(r_index)) == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        to_next_list(&r_index);
                    } while(!is_operator(get_char(r_index)) && get_char(r_index) != ']');
                    to_prev_list(&r_index);
                }
                if(get_char(get_next_str(r_index)) == '+' || get_char(get_next_str(r_index)) == '-') {
                    to_next_list(&r_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_prev_str(index)) == ']' && get_char(get_prev_str(get_prev_str(index))) == '0' && get_char(get_prev_str(get_prev_str(get_prev_str(index)))) == '[') {
                l_index = get_prev_str(get_prev_str(get_prev_str(index)));
                if(get_char(get_next_str(r_index)) == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        to_next_list(&r_index);
                    } while(!is_operator(get_char(r_index)) && get_char(r_index) != ']');
                    to_prev_list(&r_index);
                }
                if(get_char(get_next_str(r_index)) == '+' || get_char(get_next_str(r_index)) == '-') {
                    to_next_list(&r_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_next_str(index)) == '[' && get_char(get_next_str(get_next_str(index))) == '0' && get_char(get_next_str(get_next_str(get_next_str(index)))) == ']') {
                r_index = get_next_str(get_next_str(get_next_str(index)));
                if(get_char(get_prev_str(l_index)) == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        to_prev_list(&l_index);
                    } while(!is_operator(get_char(l_index)) && get_char(l_index) != '[');
                    to_next_list(&l_index);
                }
                if(get_char(get_prev_str(l_index)) == '+' || get_char(get_prev_str(l_index)) == '-') {
                    to_prev_list(&l_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_next_str(index)) == '0' && !isdigit(get_char(get_next_str(get_next_str(index))))) {
                r_index = get_next_str(index);
                if(get_char(get_prev_str(l_index)) == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        to_prev_list(&l_index);
                    } while(!is_operator(get_char(l_index)) && get_char(l_index) != '[');
                    to_next_list(&l_index);
                }
                if(get_char(get_prev_str(l_index)) == '+' || get_char(get_prev_str(l_index)) == '-') {
                    to_prev_list(&l_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            }
        } else if(get_char(index) == '+' || get_char(index) == '-') {
            if(get_char(get_prev_str(index)) == '0') {
                l_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_prev_str(index)) == ']' && get_char(get_prev_str(get_prev_str(index))) == '0' && get_char(get_prev_str(get_prev_str(get_prev_str(index)))) == '[') {
                l_index = get_prev_str(get_prev_str(get_prev_str(l_index)));
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_next_str(index)) == '0') {
                r_index = get_next_str(r_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_next_str(index)) == '[' && get_char(get_next_str(get_next_str(index))) == '0' && get_char(get_next_str(get_next_str(get_next_str(index)))) == ']') {
                r_index = get_next_str(get_next_str(get_next_str(r_index)));
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            }
        } else if(get_char(index) == '^') {
            if(get_char(get_next_str(index)) == '1' && !isdigit(get_char(get_next_str(get_next_str(index))))) {
                r_index = get_next_str(index);
                l_index = index;
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            }
        }
    }
    cycle_exit:
    return false;
}                    

int mult_delete_r(string* string, c_node* index) {    
    while(true) {
        c_node* tl_index = NULL;
        c_node* tr_index = NULL;
        
        if(index == NULL) goto cycle_exit;
        do {
            to_next_list(&index);
            if(index == NULL) goto cycle_exit;
        } while(!is_operator(get_char(index)));

        c_node* l_index = index;
        c_node* r_index = index;

        if(get_char(index) == '*') {
            if(get_char(get_prev_str(index)) == '0' && !isdigit(get_char(get_prev_str(get_prev_str(index))))) {
                to_prev_list(&l_index);
                if(get_char(get_next_str(r_index)) == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        to_next_list(&r_index);
                    } while(!is_operator(get_char(r_index)) && get_char(r_index) != ']');
                    to_prev_list(&r_index);
                }
                if(get_char(get_next_str(r_index)) == '+' || get_char(get_next_str(r_index)) == '-') {
                    to_next_list(&r_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_prev_str(index)) == ']' && get_char(get_prev_str(get_prev_str(index))) == '0' && get_char(get_prev_str(get_prev_str(get_prev_str(index)))) == '[') {
                l_index = get_prev_str(get_prev_str(get_prev_str(index)));
                if(get_char(get_next_str(r_index)) == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        to_next_list(&r_index);
                    } while(!is_operator(get_char(r_index)) && get_char(r_index) != ']');
                    to_prev_list(&r_index);
                }
                if(get_char(get_next_str(r_index)) == '+' || get_char(get_next_str(r_index)) == '-') {
                    to_next_list(&r_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_next_str(index)) == '[' && get_char(get_next_str(get_next_str(index))) == '0' && get_char(get_next_str(get_next_str(get_next_str(index)))) == ']') {
                r_index = get_next_str(get_next_str(get_next_str(index)));
                if(get_char(get_prev_str(l_index)) == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        to_prev_list(&l_index);
                    } while(!is_operator(get_char(l_index)) && get_char(l_index) != '[');
                    to_next_list(&l_index);
                }
                if(get_char(get_prev_str(l_index)) == '+' || get_char(get_prev_str(l_index)) == '-') {
                    to_prev_list(&l_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            } else if(get_char(get_next_str(index)) == '0' && !isdigit(get_char(get_next_str(get_next_str(index))))) {
                r_index = get_next_str(index);
                if(get_char(get_prev_str(l_index)) == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        to_prev_list(&l_index);
                    } while(!is_operator(get_char(l_index)) && get_char(l_index) != '[');
                    to_next_list(&l_index);
                }
                if(get_char(get_prev_str(l_index)) == '+' || get_char(get_prev_str(l_index)) == '-') {
                    to_prev_list(&l_index);
                }
                c_node* tl_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                check_limits(tl_index, tr_index, string);
                mult_delete_r(string, tr_index);
                return true;
            }
        } else if(get_char(index) == '+' || get_char(index) == '-') {
            if(get_char(get_prev_str(index)) == '0') {
                l_index = get_prev_str(l_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_prev_str(index)) == ']' && get_char(get_prev_str(get_prev_str(index))) == '0' && get_char(get_prev_str(get_prev_str(get_prev_str(index)))) == '[') {
                l_index = get_prev_str(get_prev_str(get_prev_str(l_index)));
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_next_str(index)) == '0') {
                r_index = get_next_str(r_index);
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            } else if(get_char(get_next_str(index)) == '[' && get_char(get_next_str(get_next_str(index))) == '0' && get_char(get_next_str(get_next_str(get_next_str(index)))) == ']') {
                r_index = get_next_str(get_next_str(get_next_str(r_index)));
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            }
        } else if(get_char(index) == '^') {
            if(get_char(get_next_str(index)) == '1' && !isdigit(get_char(get_next_str(get_next_str(index))))) {
                r_index = get_next_str(index);
                l_index = index;
                c_node* tr_index = get_next_str(r_index);
                delete_between(string, l_index, r_index);
                index = tr_index;
                return true;
            }
        }
    }
    cycle_exit:
    return false;
}      

int check_limits(c_node* tl_index, c_node* tr_index, string* string) {
    if(tl_index != NULL && tr_index != NULL) {
        if(get_char(tl_index) == '[' && get_char(tr_index) == ']') {
            c_node* t_index = get_next_str(tr_index);
            delete_between(string, tl_index, tr_index);
            if(get_head_str(string) == get_tail_str(string)) {
                set_char(get_head_str(string), '0');
            } else {
                insert_before(string, t_index, '0');
            }
        }
    }
    return 0;
}