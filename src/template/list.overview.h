
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

  Values are passed by copy, no initialization/deinitialization is performed.

Types:
  List object : LIST_TYPE
  Node object : NODE_TYPE
  Value type  : VALUE_TYPE

API:
  Initialize a list object             : LIST_METHOD_INIT         (LIST_TYPE * list)
  Clear a list                         : LIST_METHOD_CLEAR        (LIST_TYPE * list)
  Erase a node from a list             : LIST_METHOD_ERASE        (NODE_TYPE * node)
  Add a value to the back of a list    : LIST_METHOD_PUSHBACK     (LIST_TYPE * list, VALUE_TYPE value)
  Add a value to the front of a list   : LIST_METHOD_PUSHFRONT    (LIST_TYPE * list, VALUE_TYPE value)
  Add a value before a node            : LIST_METHOD_INSERTBEFORE (NODE_TYPE * node, VALUE_TYPE value)
  Add a value after a node             : LIST_METHOD_INSERTAFTER  (NODE_TYPE * node, VALUE_TYPE value)
  Retrieve the first node in a list    : LIST_METHOD_FIRST        (const NODE_TYPE * node)
  Retrieve the last node in a list     : LIST_METHOD_LAST         (const NODE_TYPE * node)
  Retrieve the next node a the list    : LIST_METHOD_NEXT         (const NODE_TYPE * node)
  Retrieve the previous node in a list : LIST_METHOD_PREV         (const NODE_TYPE * node)
  Retrieve the value of a node         : LIST_METHOD_VALUE        (const NODE_TYPE * node)

