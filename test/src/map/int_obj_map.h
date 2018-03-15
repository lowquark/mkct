#ifndef _INT_OBJ_MAP_H_
#define _INT_OBJ_MAP_H_

#include "obj.h"

struct int_obj_map_entry;

/*
 * Hash map from `int` keys to `obj_t` objects. Manages
 * initialization and allocation of the objects, and stores shallow copies of
 * their keys.
 */
typedef struct int_obj_map {
  struct int_obj_map_entry ** table;
  unsigned long table_size;
  unsigned long entry_count;
} int_obj_map_t;

/* Initializes the given `int_obj_map_t` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use int_obj_map_clear to destroy all
 * objects in the map.
 */
void int_obj_map_init(int_obj_map_t * map);

/*
 * Destroys all objects in the map, and frees all allocated memory it owns.
 */
void int_obj_map_clear(int_obj_map_t * map);

/* Looks up the object using a given key.
 *
 * Returns a pointer to the found object. Returns NULL if no object has key
 * `key`.
 */
obj_t * int_obj_map_find(int_obj_map_t * map, int key);

/* Creates a new object for the given key.
 *
 * If an entry is already at `key`, the old entry is destroyed and the new one
 * is put in its place.
 *
 * Returns a pointer to the new entry.
 */
obj_t * int_obj_map_create(int_obj_map_t * map, int key);

/* Finds and destroys the object with the given key.
 *
 * Returns 1 if the object was found (and destroyed) and 0 otherwise.
 */
int int_obj_map_destroy(int_obj_map_t * map, int key);

/*
 * Returns the number of elements in the map
 */
#define int_obj_map_size(_map_) (((const MAP_TYPE *)_map_)->entry_count)

#endif
