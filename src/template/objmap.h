#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

struct ENTRY_STRUCT;

/*
 * Hash map from `KEY_TYPEDEF` keys to `VALUE_TYPEDEF` values. Manages
 * initialization and allocation of values, and stores shallow copies of their
 * keys.
 */
typedef struct OBJMAP_STRUCT {
  struct ENTRY_STRUCT ** table;
  unsigned long table_size;
  unsigned long entry_count;
} OBJMAP_TYPEDEF;

/* Initializes the given `OBJMAP_TYPEDEF` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use OBJMAP_METHOD_CLEAR to erase all values in the map.
 */
void OBJMAP_METHOD_INIT(OBJMAP_TYPEDEF * map);

/* Destroys all values in the map, and destroys all allocated memory owned by the map. */
void OBJMAP_METHOD_CLEAR(OBJMAP_TYPEDEF * map);

/* Looks up the value using a given key.
 *
 * Returns a pointer to the found value. Returns NULL if no value has key `key`.
 */
VALUE_TYPEDEF * OBJMAP_METHOD_FIND(OBJMAP_TYPEDEF * map, KEY_TYPEDEF key);

/* Creates a new value for the given key.
 *
 * If an entry is already at `key`, the old entry is destroyed and the new one is put in its place.
 *
 * Returns a pointer to the new entry.
 */
VALUE_TYPEDEF * OBJMAP_METHOD_CREATE(OBJMAP_TYPEDEF * map, KEY_TYPEDEF key);

/* Finds and destroys the value with the given key.
 *
 * Returns 1 if the value was found (and destroyed) and 0 otherwise.
 */
int OBJMAP_METHOD_DESTROY(OBJMAP_TYPEDEF * map, KEY_TYPEDEF key);

#endif
