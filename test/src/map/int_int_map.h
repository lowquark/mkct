#ifndef _INT_INT_MAP_H_
#define _INT_INT_MAP_H_

struct int_int_map_entry;

/*
 * Hash map from `int` to `int` via linear-probing.
 */
typedef struct int_int_map {
  struct int_int_map_entry * table;
  unsigned long table_size;
  unsigned long fill_count;
} int_int_map_t;


/* Initializes the given `int_int_map_t` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use int_int_map_clear to erase all values in the map.
 */
void int_int_map_init  (int_int_map_t * map);

/*
 * Erases all values in the map, and frees all allocated memory it owns.
 */
void int_int_map_clear (int_int_map_t * map);


/*
 * If a value exists with the given key, stores its value in `*value_out` and returns 1.
 * Otherwise, leaves `*value_out` unmodified and returns 0.
 */
int  int_int_map_get   (int_int_map_t * map, int key, int * value_out);

/*
 * Assigns the value with the given key to the given value.
 */
int  int_int_map_set   (int_int_map_t * map, int key, int value);

/*
 * Returns 1 if a value exists in the map with the given key, and 0 otherwise.
 */
int  int_int_map_has   (int_int_map_t * map, int key);

/* Finds and erases the value with the given key.
 *
 * Returns 1 if the value was found (and erased) and 0 otherwise.
 */
int  int_int_map_erase (int_int_map_t * map, int key);

#endif
