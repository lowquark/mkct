
Files:
  H_FILE
  C_FILE

Description:
  Implements the following circular linked list structure:
 
         list         node         node         node
      +--------+   +--------+   +--------+   +--------+
   +->|  list  |<->|  list  |<->|  list  |<->|  list  |<-+
   |  +--------+   + - -- - +   + - -- - +   + - -- - +  |
   |               | object |   | object |   | object |  |
   |               +--------+   +--------+   +--------+  |
   +-----------------------------------------------------+

  Objects are allocated and initialized when nodes are pushed. Objects are
  cleared and freed when nodes are erased. Pointers to objects created will
  remain valid until their nodes are erased.

  Stubs for initializing and clearing objects can be found in the generated
  source. More detailed documentation can be found in the generated header.

Types:
  List object : OBJLIST_TYPE
  Node object : NODE_TYPE
  Object type : OBJECT_TYPE *

API:
  Initialize a list object   : OBJLIST_METHOD_INIT         (OBJLIST_TYPE * list)
  Erase all nodes            : OBJLIST_METHOD_CLEAR        (OBJLIST_TYPE * list)
  Erase a node from its list : OBJLIST_METHOD_ERASE        (NODE_TYPE * node)
  Append to a list           : OBJLIST_METHOD_PUSHBACK     (OBJLIST_TYPE * list) -> NODE_TYPE *
  Prepend to a list          : OBJLIST_METHOD_PUSHFRONT    (OBJLIST_TYPE * list) -> NODE_TYPE *
  Insert before a node       : OBJLIST_METHOD_INSERTBEFORE (NODE_TYPE * node) -> NODE_TYPE *
  Insert after a node        : OBJLIST_METHOD_INSERTAFTER  (NODE_TYPE * node) -> NODE_TYPE *
  Retrieve the first node    : OBJLIST_METHOD_FIRST        (const OBJLIST_TYPE * list) -> NODE_TYPE *
  Retrieve the last node     : OBJLIST_METHOD_LAST         (const OBJLIST_TYPE * list) -> NODE_TYPE *
  Retrieve the next node     : OBJLIST_METHOD_NEXT         (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve the previous node : OBJLIST_METHOD_PREV         (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve a node's object   : OBJLIST_METHOD_VALUE        (const NODE_TYPE * node) -> OBJECT_TYPE *

