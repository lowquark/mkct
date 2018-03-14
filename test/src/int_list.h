#ifndef _INT_LIST_H_
#define _INT_LIST_H_

/*
 * int_list.h / int_list.c
 *
 * Implements the following circular linked list structure:
 *
 *       list       node       node       node
 *     +-------+  +-------+  +-------+  +-------+
 *  +->| list  |->| list  |->| list  |->| list  |--+
 *  |  +-------+  + - - - +  + - - - +  + - - - +  |
 *  |             | value |  | value |  | value |  |
 *  |             +-------+  +-------+  +-------+  |
 *  +----------------------------------------------+
 *
 */

typedef struct int_list {
  struct int_list * next;
  struct int_list * prev;
  struct int_list * head;
} int_list_t;

typedef struct int_list_node {
  int_list_t list;
  int value;
} int_list_node_t;

/*
 * Initializes the given list to a valid state.
 */
void int_list_init(int_list_t * list);

/*
 * Deletes and removes all values present in the list.
 */
void int_list_clear(int_list_t * list);

/*
 * Deletes a single node and removes it from its list.
 */
void int_list_erase(int_list_node_t * node);

/*
 * Creates a new node with value `value` and inserts it at the back of `list`.
 *
 * Returns the new node.
 */
int_list_node_t * int_list_pushback(int_list_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it at the front of `list`.
 *
 * Returns the new node.
 */
int_list_node_t * int_list_pushfront(int_list_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it before `node`.
 *
 * Returns the new node.
 */
int_list_node_t * int_list_insertbefore(int_list_node_t * node, int value);

/*
 * Creates a new node with value `value` and inserts it after `node`.
 *
 * Returns the new node.
 */
int_list_node_t * int_list_insertafter(int_list_node_t * node, int value);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
int_list_node_t * int_list_first(const int_list_t * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
int_list_node_t * int_list_last(const int_list_t * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
int_list_node_t * int_list_next(const int_list_node_t * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
int_list_node_t * int_list_prev(const int_list_node_t * node);

#endif
