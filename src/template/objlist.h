#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

/*
 * H_FILE / C_FILE
 *
 * Implements the following circular linked list structure:
 *
 *        list        node        node        node
 *     +--------+  +--------+  +--------+  +--------+
 *  +->|  list  |->|  list  |->|  list  |->|  list  |--+
 *  |  +--------+  + - -- - +  + - -- - +  + - -- - +  |
 *  |              | object |  | object |  | object |  |
 *  |              +--------+  +--------+  +--------+  |
 *  +--------------------------------------------------+
 *
 */

typedef struct OBJLIST_STRUCT {
  struct OBJLIST_STRUCT * next;
  struct OBJLIST_STRUCT * prev;
  struct OBJLIST_STRUCT * head;
} OBJLIST_TYPE;

typedef struct NODE_STRUCT {
  OBJLIST_TYPE list;
  OBJECT_TYPE value;
} NODE_TYPE;

/*
 * Initializes the given list to a valid state.
 */
void OBJLIST_METHOD_INIT(OBJLIST_TYPE * list);

/*
 * Deletes and removes all values present in the list.
 */
void OBJLIST_METHOD_CLEAR(OBJLIST_TYPE * list);

/*
 * Deletes a single node and removes it from its list.
 */
void OBJLIST_METHOD_ERASE(NODE_TYPE * node);

/*
 * Creates a new object and places it at the back of `list`.
 *
 * Returns the new node.
 */
NODE_TYPE * OBJLIST_METHOD_PUSHBACK(OBJLIST_TYPE * list);

/*
 * Creates a new object and places it at the front of `list`.
 *
 * Returns the new node.
 */
NODE_TYPE * OBJLIST_METHOD_PUSHFRONT(OBJLIST_TYPE * list);

/*
 * Creates a new object and inserts it before `node`.
 *
 * Returns the new node.
 */
NODE_TYPE * OBJLIST_METHOD_INSERTBEFORE(NODE_TYPE * node);

/*
 * Creates a new object and inserts it after `node`.
 *
 * Returns the new node.
 */
NODE_TYPE * OBJLIST_METHOD_INSERTAFTER(NODE_TYPE * node);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
NODE_TYPE * OBJLIST_METHOD_FIRST(const OBJLIST_TYPE * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
NODE_TYPE * OBJLIST_METHOD_LAST(const OBJLIST_TYPE * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
NODE_TYPE * OBJLIST_METHOD_NEXT(const NODE_TYPE * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
NODE_TYPE * OBJLIST_METHOD_PREV(const NODE_TYPE * node);

/*
 * Returns the value of a given node.
 */
#define OBJLIST_METHOD_VALUE(_node_) ((OBJECT_TYPE *)&((const NODE_TYPE *)_node_)->value)

#endif
