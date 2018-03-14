#ifndef _OBJ_LIST_H_
#define _OBJ_LIST_H_

/*
 * obj_list.h / obj_list.c
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

typedef struct obj_list {
  struct obj_list * next;
  struct obj_list * prev;
  struct obj_list * head;
} obj_list_t;

typedef struct obj_list_node {
  obj_list_t list;
  int value;
} obj_list_node_t;

/*
 * Initializes the given list to a valid state.
 */
void obj_list_init(obj_list_t * list);

/*
 * Deletes and removes all values present in the list.
 */
void obj_list_clear(obj_list_t * list);

/*
 * Deletes a single node and removes it from its list.
 */
void obj_list_erase(obj_list_node_t * node);

/*
 * Creates a new node with value `value` and inserts it at the back of `list`.
 *
 * Returns the new node.
 */
obj_list_node_t * obj_list_pushback(obj_list_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it at the front of `list`.
 *
 * Returns the new node.
 */
obj_list_node_t * obj_list_pushfront(obj_list_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it before `node`.
 *
 * Returns the new node.
 */
obj_list_node_t * obj_list_insertbefore(obj_list_node_t * node, int value);

/*
 * Creates a new node with value `value` and inserts it after `node`.
 *
 * Returns the new node.
 */
obj_list_node_t * obj_list_insertafter(obj_list_node_t * node, int value);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
obj_list_node_t * obj_list_first(const obj_list_t * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
obj_list_node_t * obj_list_last(const obj_list_t * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
obj_list_node_t * obj_list_next(const obj_list_node_t * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
obj_list_node_t * obj_list_prev(const obj_list_node_t * node);

#endif
