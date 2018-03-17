
Files:
  H_FILE
  C_FILE

Description:
  Implements the following circular linked list structure:
 
         list        node        node        node
      +--------+  +--------+  +--------+  +--------+
   +->|  list  |->|  list  |->|  list  |->|  list  |--+
   |  +--------+  + - -- - +  + - -- - +  + - -- - +  |
   |              | object |  | object |  | object |  |
   |              +--------+  +--------+  +--------+  |
   +--------------------------------------------------+

   Objects are allocated and initialized when nodes are pushed. When nodes are
   erased, they are cleared before being freed. To access a node's object, use
   `OBJLIST_METHOD_VALUE`. See `C_FILE` for init/clear hooks.

Types:
  List object : OBJLIST_TYPE
  Node object : NODE_TYPE
  Object type : OBJECT_TYPE *

API:
  Initialize a list object             : OBJLIST_METHOD_INIT         (OBJLIST_TYPE * list)
  Clear a list                         : OBJLIST_METHOD_CLEAR        (OBJLIST_TYPE * list)
  Erase a node from a list             : OBJLIST_METHOD_ERASE        (NODE_TYPE * node)
  Add an object to the back of a list  : OBJLIST_METHOD_PUSHBACK     (OBJLIST_TYPE * list)
  Add an object to the front of a list : OBJLIST_METHOD_PUSHFRONT    (OBJLIST_TYPE * list)
  Add an object before a node          : OBJLIST_METHOD_INSERTBEFORE (NODE_TYPE * node)
  Add an object after a node           : OBJLIST_METHOD_INSERTAFTER  (NODE_TYPE * node)
  Retrieve the first node in a list    : OBJLIST_METHOD_FIRST        (const OBJLIST_TYPE * list)
  Retrieve the last node in a list     : OBJLIST_METHOD_LAST         (const OBJLIST_TYPE * list)
  Retrieve the next node a the list    : OBJLIST_METHOD_NEXT         (const NODE_TYPE * node)
  Retrieve the previous node in a list : OBJLIST_METHOD_PREV         (const NODE_TYPE * node)
  Retrieve the object of a node        : OBJLIST_METHOD_VALUE        (const NODE_TYPE * node)

