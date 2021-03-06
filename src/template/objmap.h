#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

struct ENTRY_STRUCT;

/*
 * Hash map from `KEY_TYPE` keys to `OBJECT_TYPE` objects. Manages
 * initialization and allocation of the objects, and stores shallow copies of
 * their keys.
 */
typedef struct OBJMAP_STRUCT {
  struct ENTRY_STRUCT ** table;
  unsigned long table_size;
  unsigned long entry_count;
} OBJMAP_TYPE;

/* Initializes the given `OBJMAP_TYPE` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJMAP_METHOD_CLEAR to destroy all
 * objects in the map.
 */
void OBJMAP_METHOD_INIT(OBJMAP_TYPE * map);

/*
 * Destroys all objects in the map, and frees all allocated memory it owns.
 */
void OBJMAP_METHOD_CLEAR(OBJMAP_TYPE * map);

/* Looks up the object using a given key.
 *
 * Returns a pointer to the found object. Returns NULL if no object has key
 * `key`.
 */
OBJECT_TYPE * OBJMAP_METHOD_FIND(OBJMAP_TYPE * map, KEY_TYPE key);

/* Creates a new object for the given key.
 *
 * If an entry is already at `key`, the old entry is destroyed and the new one
 * is put in its place.
 *
 * Returns a pointer to the new entry.
 */
OBJECT_TYPE * OBJMAP_METHOD_CREATE(OBJMAP_TYPE * map, KEY_TYPE key);

/* Finds and destroys the object with the given key.
 *
 * Returns 1 if the object was found (and destroyed) and 0 otherwise.
 */
int OBJMAP_METHOD_DESTROY(OBJMAP_TYPE * map, KEY_TYPE key);

/*
 * Returns the number of elements in the map
 */
#define OBJMAP_METHOD_SIZE(_map_) (((const MAP_TYPE *)_map_)->entry_count)

#endif
