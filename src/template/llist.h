#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * H_FILE / C_FILE
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

typedef struct LIST_STRUCT {
  struct LIST_STRUCT * next;
  struct LIST_STRUCT * prev;
  struct LIST_STRUCT * head;
} LIST_TYPEDEF;

typedef struct NODE_STRUCT {
  LIST_TYPEDEF list;
  VALUE_TYPEDEF value;
} NODE_TYPEDEF;

/*
 * Initializes the given list to a valid state.
 */
void LIST_METHOD_INIT(LIST_TYPEDEF * list);

/*
 * Deletes and removes all values present in the list.
 */
void LIST_METHOD_CLEAR(LIST_TYPEDEF * list);

/*
 * Deletes a single node and removes it from its list.
 */
void LIST_METHOD_ERASE(NODE_TYPEDEF * node);

/*
 * Creates a new node with value `value` and inserts it at the back of `list`.
 *
 * Returns the new node.
 */
NODE_TYPEDEF * LIST_METHOD_PUSHBACK(LIST_TYPEDEF * list, VALUE_TYPEDEF value);

/*
 * Creates a new node with value `value` and inserts it at the front of `list`.
 *
 * Returns the new node.
 */
NODE_TYPEDEF * LIST_METHOD_PUSHFRONT(LIST_TYPEDEF * list, VALUE_TYPEDEF value);

/*
 * Creates a new node with value `value` and inserts it before `node`.
 *
 * Returns the new node.
 */
NODE_TYPEDEF * LIST_METHOD_INSERTBEFORE(NODE_TYPEDEF * node, VALUE_TYPEDEF value);

/*
 * Creates a new node with value `value` and inserts it after `node`.
 *
 * Returns the new node.
 */
NODE_TYPEDEF * LIST_METHOD_INSERTAFTER(NODE_TYPEDEF * node, VALUE_TYPEDEF value);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
NODE_TYPEDEF * LIST_METHOD_FIRST(const LIST_TYPEDEF * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
NODE_TYPEDEF * LIST_METHOD_LAST(const LIST_TYPEDEF * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
NODE_TYPEDEF * LIST_METHOD_NEXT(const NODE_TYPEDEF * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
NODE_TYPEDEF * LIST_METHOD_PREV(const NODE_TYPEDEF * node);

#endif
