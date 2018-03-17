
Files:
  Header : H_FILE
  Source : C_FILE

Description:
  Implements a single-chained hash map from `KEY_TYPE` to `OBJECT_TYPE`.

  Objects are allocated and initialized when entries are created. Objects are
  cleared and freed when entries are destroyed. Pointers to objects will remain
  valid until their entries are destroyed. Existing objects are destroyed
  before being replaced by new ones.

  Stubs for initializing objects, clearing objects, and hashing keys can be
  found in the generated source. More detailed documentation can be found in
  the generated header.

Types:
  Map object                 : OBJMAP_TYPE
  Map entry type (unexposed) : ENTRY_TYPE
  Key type                   : KEY_TYPE
  Object type                : OBJECT_TYPE

API:
  Initialize a map object : OBJMAP_METHOD_INIT    (OBJMAP_TYPE * map)
  Destroy all entries     : OBJMAP_METHOD_CLEAR   (OBJMAP_TYPE * map)
  Find an entry           : OBJMAP_METHOD_FIND    (OBJMAP_TYPE * map, KEY_TYPE key) -> OBJECT_TYPE *
  Create an entry         : OBJMAP_METHOD_CREATE  (OBJMAP_TYPE * map, KEY_TYPE key) -> OBJECT_TYPE *
  Destroy an entry        : OBJMAP_METHOD_DESTROY (OBJMAP_TYPE * map, KEY_TYPE key) -> int (success/failure)

