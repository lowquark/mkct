
Files:
  H_FILE
  C_FILE

Description:
  Implements the following circular linked list structure:

        list        node        node        node
      +-------+   +-------+   +-------+   +-------+
   +->| list  |<->| list  |<->| list  |<->| list  |<-+
   |  +-------+   + - - - +   + - - - +   + - - - +  |
   |              | value |   | value |   | value |  |
   |              +-------+   +-------+   +-------+  |
   +-------------------------------------------------+

  Values are passed by copy - no value initialization or allocation is
  performed.

Types:
  List object : LIST_TYPE
  Node object : NODE_TYPE
  Value type  : VALUE_TYPE

API:
  Initialize a list object   : LIST_METHOD_INIT         (LIST_TYPE * list)
  Erase all nodes            : LIST_METHOD_CLEAR        (LIST_TYPE * list)
  Erase a node from its list : LIST_METHOD_ERASE        (NODE_TYPE * node)
  Append to a list           : LIST_METHOD_PUSHBACK     (LIST_TYPE * list, VALUE_TYPE value) -> NODE_TYPE *
  Prepend to a list          : LIST_METHOD_PUSHFRONT    (LIST_TYPE * list, VALUE_TYPE value) -> NODE_TYPE *
  Insert before a node       : LIST_METHOD_INSERTBEFORE (NODE_TYPE * node, VALUE_TYPE value) -> NODE_TYPE *
  Insert after a node        : LIST_METHOD_INSERTAFTER  (NODE_TYPE * node, VALUE_TYPE value) -> NODE_TYPE *
  Retrieve the first node    : LIST_METHOD_FIRST        (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve the last node     : LIST_METHOD_LAST         (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve the next node     : LIST_METHOD_NEXT         (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve the previous node : LIST_METHOD_PREV         (const NODE_TYPE * node) -> NODE_TYPE *
  Retrieve a node's value    : LIST_METHOD_VALUE        (const NODE_TYPE * node) -> VALUE_TYPE

