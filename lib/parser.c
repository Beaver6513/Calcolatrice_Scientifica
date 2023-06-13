#include "parser.h"
#include "char_list.h"
#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int expand_string(struct string* str) {
    struct c_node* index = str->head;

    while(index->next != NULL) {
        if((isdigit(index->character) || index->character == 'x') && index->next->character == '[') insert_after(str, index, '*');

        else if(index->character == ']' && (isdigit(index->next->character) || index->next->character == 'x')) insert_after(str, index, '*');

        else if(index->character == ']' && index->next->character == '[') insert_after(str, index, '*');

        else if(isdigit(index->character) && index->next->character == 'x') insert_after(str, index, '*');

        else if(index->previous == NULL) {
            if(index->character == '-' && isdigit(index->next->character)) {
                index->character = '_';
            }
            if(index->character == '-' && index->next->character == 'x') {
                index->character = '_';
            }
        } else if(index->previous != NULL) {
            if(index->previous->character == '[' && index->character == '-' && isdigit(index->next->character)) {
                index->character = '_';
            }
            if(index->previous->character == '[' && index->character == '-' && index->next->character == 'x') {
                index->character = '_';
            }
        }

        index = index->next;
    }
    return 0;
}

int splice(struct string* str) {
    struct c_node* block_start = str->head;
    struct c_node* block_end = str->head;
    
    
    while(block_end != NULL) {
        if(block_start->character == '(' || block_start->character == ')' || block_start->character == '[' || block_start->character == ']') {
            block_start = block_start->next;
            block_end = block_start;
        } else if(isdigit(block_start->character)) {
            while(1) {
                if(block_end->next == NULL) break;
                if(!isdigit(block_end->next->character)) break;
                block_end = block_end->next;
            }
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            block_end = block_end->next;
            block_start = block_end;
        } else if(block_start->character == '_' && isdigit(block_start->next->character)) {
            block_end = block_end->next;
            while(1) {
                if(block_end->next == NULL) break;
                if(!isdigit(block_end->next->character)) break;
                block_end = block_end->next;
            }
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            block_end = block_end->next;
            block_start = block_end;
        } else if(block_start->character == '_' && block_start->next->character == 'x') {
            block_end = block_end->next;
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            block_end = block_end->next;
            block_start = block_end;
        } else if(block_start->character == 'x') {
            insert_before(str, block_start, '(');
            insert_after(str, block_end, ')');
            block_end = block_end->next;
            block_start = block_end;
        } else if(is_operator(block_start->character)) {
            block_start = block_start->next;
            block_end = block_start;
        }
    }

    struct c_node* index = str->head;
    int count = 0;
    do {
        if(index->character == '(') count++;
        if(index->character == ')') count--;
        if(index->character == '[') count++;
        if(index->character == ']') count--;
        index = index->next;
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

int move_to_next_block(struct c_node** index) {
    int count = 0;

    do {
        (*index) = (*index)->next;

        if((*index)->character == '(' || (*index)->character == '[') {
            count++;
        } else if((*index)->character == ')' || (*index)->character == ']') {
            count--;
        }
    } while (count != 0);

    return 0;
}

int move_to_previous_block(struct c_node** index) {
    int count = 0;

    do {
        (*index) = (*index)->previous;

        if((*index)->character == '(' || (*index)->character == '[') {
            count++;
        } else if((*index)->character == ')' || (*index)->character == ']') {
            count--;
        }
    } while (count != 0);

    return 0;
}

int check_left_op(struct c_node* index, struct c_node* block_limit) {
    while(index != block_limit) {
        if(is_operator(index->character)) {
            return true;
        }
        index = index->previous;
    }
    return false;
}

int check_right_op(struct c_node* index, struct c_node* block_limit) {
    while(index != block_limit) {
        if(is_operator(index->character)) {
            return true;
        }
        index = index->next;
    }
    return false;
}

int group(struct c_node* block_start, struct c_node* block_end, struct string* str, bool should_restart, struct c_node* index) {
    if(is_leaf(block_start, block_end)) {
        return 0;
    } else {
        if(should_restart == true) {
            index = block_start;
            move_to_next_block(&index);
            index = index->next;
        }

        struct c_node* l_block_end = index->previous;
        struct c_node* l_block_start = index;
        struct c_node* r_block_start = index->next;
        struct c_node* r_block_end = index;
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
            if(r_block_end->next->character == '+' || r_block_end->next->character == '-') {
                insert_before(str, l_block_start, '(');
                insert_after(str, r_block_end, ')');
                group(block_start, block_end, str, true, index);
            } else if(r_block_end->next->character == '*' || r_block_end->next->character == '/') {
                if(index->character == '^' || index->character == '+' || index->character == '-') {
                    move_to_next_block(&index);
                    index = index->next;
                    group(block_start, block_end, str, false, index);
                } else if(index->character == '*' || index->character == '/') {
                    insert_before(str, l_block_start, '(');
                    insert_after(str, r_block_end, ')');
                    group(block_start, block_end, str, true, index);
                }
            } else if(r_block_end->next->character == '^') {
                move_to_next_block(&index);
                index = index->next;
                group(block_start, block_end, str, false, index);
            }
        }
    }
    return 0;
}

int group_string(struct string* str) {
    group(str->head, str->tail, str, true, str->head);
    return 0;
}

int load_tree(struct tree* out_tree, struct string* str) {
    load_tree_block(str->head, str->tail, out_tree->tree_head, out_tree, str);
    return 0;
}

int contract(struct string* str) {
    struct c_node* index = str->head;

    while(index->next != NULL) {
        if(index->next != NULL) {
            if(index->next->next != NULL) {
                if(((isdigit(index->character)) || (index->character == 'x')) && (index->next->character == '*') && (index->next->next->character == '(')) {
                    struct c_node* t = index->next;
                    index->next->next->previous = index;
                    index->next = index->next->next;
                    free(t);
                }

                if((index->character == ')') && ((isdigit(index->next->next->character)) || (index->next->next->character == 'x')) && (index->next->character == '*')) {
                    struct c_node* t = index->next;
                    index->next->next->previous = index;
                    index->next = index->next->next;
                    free(t);
                }

                if((index->character == ')') && (index->next->character == '*') && (index->next->next->character == '(')) {
                    struct c_node* t = index->next;
                    index->next->next->previous = index;
                    index->next = index->next->next;
                    free(t);
                }

                if((isdigit(index->character)) && (index->next->character == '*') && (index->next->next->character == 'x')) {
                    struct c_node* t = index->next;
                    index->next->next->previous = index;
                    index->next = index->next->next;
                    free(t);
                }
            }
        }
        index = index->next;
    }

    return 0;
}

int one_mult_delete(struct string* string) {
    struct c_node* index = string->head;
    
    while(1) {
        do {
            index = index->next;
            if(index == NULL) goto cycle_exit;
        } while(index->character != '*');

        struct c_node* l_index = index;
        struct c_node* r_index = index;

        if(index->next->next->character == '1') {
            r_index = index->next->next->next;
            l_index->previous->next = r_index->next;
            r_index->next->previous = l_index->previous;

            do {
                struct c_node* t = l_index;
                l_index = l_index->next;
                free(t);
            } while(l_index != r_index);
        }
        index = r_index;
    }
    cycle_exit:
    return 0;
}

int par_check(struct string* string) {
    struct c_node* l_index = string->head;
    struct c_node* r_index = string->tail;

    if(!is_leaf(l_index, r_index)) {
        r_index = string->head;
        while(l_index != NULL) {
            while(l_index->character != '[') {
                l_index = l_index->next;
                if(l_index == NULL) goto cycle_exit;
            }
            r_index = l_index;
            int count = 0;

            do {
                if(r_index->character == '(' || r_index->character == '[') {
                    count++;
                } else if(r_index->character == ')' || r_index->character == ']') {
                    count--;
                }
                if(count != 0)
                    r_index = r_index->next;
            } while (count != 0);

            cycle_start:

            if(l_index->next->character == '[' && r_index->previous->character == ']') {
                struct c_node* t_index = l_index;
                count = 0;
                move_to_next_block(&t_index);
                t_index = t_index->next;

                if(t_index == r_index) {
                    struct c_node* c = l_index->next;
                    l_index->next = l_index->next->next;
                    l_index->next->previous = l_index;
                    free(c);
                    c = r_index->previous;
                    r_index->previous = r_index->previous->previous;
                    r_index->previous->next = r_index;
                    free(c);
                    goto cycle_start;
                }
            }
            l_index = l_index->next;
        }
    }
    cycle_exit:
    return 0;
}

int mult_delete(struct string* string) {
    bool changed = false;
    struct c_node* index = string->head;
    
    while(1) {
        do {
            index = index->next;
            if(index == NULL) goto cycle_exit;
        } while(!is_operator(index->character));

        struct c_node* l_index = index;
        struct c_node* r_index = index;

        if(index->character == '*') {
            if(index->previous->character == '0' && !isdigit(index->previous->previous->character)) {
                l_index = l_index->previous;
                if(r_index->next->character == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        r_index = r_index->next;
                    } while(!is_operator(r_index->character) && r_index->character != ']');
                    r_index = r_index->previous;
                }
                if(r_index->next->character == '+' || r_index->next->character == '-') {
                    r_index = r_index->next;
                }
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->previous->character == ']' && index->previous->previous->character == '0' && index->previous->previous->previous->character == '[') {
                l_index = index->previous->previous->previous;
                if(r_index->next->character == '[') {
                    move_to_next_block(&r_index);
                } else {
                    do {
                        r_index = r_index->next;
                    } while(!is_operator(r_index->character) && r_index->character != ']');
                    r_index = r_index->previous;
                }
                if(r_index->next->character == '+' || r_index->next->character == '-') {
                    r_index = r_index->next;
                }
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->next->character == '[' && index->next->next->character == '0' && index->next->next->next->character == ']') {
                r_index = index->next->next->next;
                if(l_index->previous->character == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        l_index = l_index->previous;
                    } while(!is_operator(l_index->character) && l_index->character != '[');
                    l_index = l_index->next;
                }
                if(l_index->previous->character == '+' || l_index->previous->character == '-') {
                    l_index = l_index->previous;
                }
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->next->character == '0' && !isdigit(index->next->next->character)) {
                r_index = index->next;
                if(l_index->previous->character == ']') {
                    move_to_previous_block(&l_index);
                } else {
                    do {
                        l_index = l_index->previous;
                    } while(!is_operator(l_index->character) && l_index->character != '[');
                    l_index = l_index->next;
                }
                if(l_index->previous->character == '+' || l_index->previous->character == '-') {
                    l_index = l_index->previous;
                }
                delete_between(string, l_index, r_index);
                changed = true;
            }
            if(l_index->previous->character == '[' && r_index->next->character == ']') {
                l_index = l_index->previous;
                r_index = r_index->next;
                delete_between(string, l_index, r_index);
                if(string->head == string->tail) {
                    string->head->character = '0';
                } else {
                    insert_before(string, r_index->next, '0');
                }
            }
        } else if(index->character == '+' || index->character == '-') {
            if(index->previous->character == '0') {
                l_index = l_index->previous;
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->previous->character == ']' && index->previous->previous->character == '0' && index->previous->previous->previous->character == '[') {
                l_index = index->previous->previous->previous;
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->next->character == '0') {
                r_index = r_index->next;
                delete_between(string, l_index, r_index);
                changed = true;
            } else if(index->next->character == '[' && index->next->next->character == '0' && index->next->next->next->character == ']') {
                r_index = r_index->next->next->next;
                delete_between(string, l_index, r_index);
                changed = true;
            }
        } else if(index->character == '^') {
            if(index->next->character == '1' && !isdigit(index->next->next->character)) {
                r_index = index->next;
                l_index = index;
                delete_between(string, l_index, r_index);
                changed = true;
            }
        }
        index = r_index;
    }
    cycle_exit:
    return changed;
}                    
