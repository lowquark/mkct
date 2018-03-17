
Files:
  H_FILE
  C_FILE

Description:
  Implements a lineraly-probed hash map from `KEY_TYPE` to `VALUE_TYPE`.

  Values are passed by copy - no value initialization or allocation is
  performed. Existing values are overwritten by new ones.

  A stub for hashing keys can be found in the generated source. More detailed
  documentation can be found in the generated header.

Types:
  Map object                 : MAP_TYPE
  Map entry type (unexposed) : ENTRY_TYPE
  Key type                   : KEY_TYPE
  Value type                 : VALUE_TYPE

API:
  Initialize a map object : MAP_METHOD_INIT  (MAP_TYPE * map)
  Erase all entries       : MAP_METHOD_CLEAR (MAP_TYPE * map)
  Retrieve an entry       : MAP_METHOD_GET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE * value_out) -> int (success/failure)
  Set an entry            : MAP_METHOD_SET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE value) -> int (success/failure)
  Check for an entry      : MAP_METHOD_HAS   (MAP_TYPE * map, KEY_TYPE key) -> int (success/failure)
  Erase an entry          : MAP_METHOD_ERASE (MAP_TYPE * map, KEY_TYPE key) -> int (success/failure)

