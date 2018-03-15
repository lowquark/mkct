#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

struct ENTRY_STRUCT;

/*
 * Hash map from `KEY_TYPE` keys to `VALUE_TYPE` values. Keys/Values are
 * simply copied, not referenced.
 */
typedef struct MAP_STRUCT {
  struct ENTRY_STRUCT ** table;
  unsigned long table_size;
  unsigned long entry_count;
} MAP_TYPE;


/* Initializes the given `MAP_TYPE` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use MAP_METHOD_CLEAR to erase all values in the map.
 */
void MAP_METHOD_INIT  (MAP_TYPE * map);

/*
 * Erases all values in the map, and frees all allocated memory it owns.
 */
void MAP_METHOD_CLEAR (MAP_TYPE * map);


/*
 * If a value exists with the given key, stores its value in `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int  MAP_METHOD_GET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE * value_out);

/*
 * Assigns the value with the given key to the given value.
 */
int  MAP_METHOD_SET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE value);

/*
 * Returns 1 if a value exists in the map with the given key, and 0 otherwise.
 */
int  MAP_METHOD_HAS   (MAP_TYPE * map, KEY_TYPE key);

/* Finds and erases the value with the given key.
 *
 * Returns 1 if the value was found (and erased) and 0 otherwise.
 */
int  MAP_METHOD_ERASE (MAP_TYPE * map, KEY_TYPE key);

/*
 * Returns the number of elements in the map
 */
#define MAP_METHOD_SIZE(_map_) (((const MAP_TYPE *)_map_)->entry_count)

#endif
