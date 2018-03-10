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
} LIST_TYPE;

typedef struct NODE_STRUCT {
  LIST_TYPE list;
  VALUE_TYPE value;
} NODE_TYPE;

/*
 * Initializes the given list to a valid state.
 */
void LIST_METHOD_INIT(LIST_TYPE * list);

/*
 * Deletes and removes all values present in the list.
 */
void LIST_METHOD_CLEAR(LIST_TYPE * list);

/*
 * Deletes a single node and removes it from its list.
 */
void LIST_METHOD_ERASE(NODE_TYPE * node);

/*
 * Creates a new node with value `value` and inserts it at the back of `list`.
 *
 * Returns the new node.
 */
NODE_TYPE * LIST_METHOD_PUSHBACK(LIST_TYPE * list, VALUE_TYPE value);

/*
 * Creates a new node with value `value` and inserts it at the front of `list`.
 *
 * Returns the new node.
 */
NODE_TYPE * LIST_METHOD_PUSHFRONT(LIST_TYPE * list, VALUE_TYPE value);

/*
 * Creates a new node with value `value` and inserts it before `node`.
 *
 * Returns the new node.
 */
NODE_TYPE * LIST_METHOD_INSERTBEFORE(NODE_TYPE * node, VALUE_TYPE value);

/*
 * Creates a new node with value `value` and inserts it after `node`.
 *
 * Returns the new node.
 */
NODE_TYPE * LIST_METHOD_INSERTAFTER(NODE_TYPE * node, VALUE_TYPE value);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
NODE_TYPE * LIST_METHOD_FIRST(const LIST_TYPE * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
NODE_TYPE * LIST_METHOD_LAST(const LIST_TYPE * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
NODE_TYPE * LIST_METHOD_NEXT(const NODE_TYPE * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
NODE_TYPE * LIST_METHOD_PREV(const NODE_TYPE * node);

#endif
