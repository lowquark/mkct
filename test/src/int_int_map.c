#include "int_int_map.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


typedef struct int_int_map_entry {
  struct int_int_map_entry * next;
  int   key;
  int value;
} int_int_map_entry_t;


/*** type specific functionaility ***/


/* TODO: Implement hash for int. */
static unsigned long hash_key(int key) {
  return (*(unsigned long*)&key);
}

/* Called to compare keys. Must return 1 if keys match, and 0 if they don't.
 *
 * Alternatively:
 * #define compare_key(key0, key1) ((key0) == (key1))
 */
static int compare_key(int key0, int key1) {
  return memcmp(&key0, &key1, sizeof(int)) == 0;
}

/* TODO: Cleanup the key and value, if applicable. This function is called
 * when entries are erased, but not when they are overwritten. */
static void deinit_key(int key) {
}

/* TODO: Cleanup the key and value, if applicable. This function is called
 * when entries are erased or overwritten. */
static void deinit_value(int value) {
}


/*** general functionaility ***/


static const unsigned long initial_size = 32;

static unsigned long hash_idx(int key, unsigned long table_size) {
  assert(table_size > 0);

  return hash_key(key) % table_size;
}

static int_int_map_entry_t ** bucket_of(int_int_map_t * map, int key) {
  assert(map->table);

  return map->table + hash_idx(key, map->table_size);
}

static int resize_table(int_int_map_t * map, unsigned long newsize) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  int_int_map_entry_t ** table = map->table;
  int_int_map_entry_t ** newtable = calloc(sizeof(*newtable), newsize);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    int_int_map_entry_t * entry = table[i];

    while(entry) {
      /* save next pointer */
      int_int_map_entry_t * next = entry->next;

      unsigned long idx = hash_idx(entry->key, newsize);

      /* lookup chain in the new table */
      int_int_map_entry_t ** slot = newtable + idx;

      /* advance slot in the new chain */
      while(*slot) {
        assert(!compare_key((*slot)->key, entry->key));
        slot = &(*slot)->next;
      }

      /* place at end of destination chain */
      *slot = entry;
      entry->next = NULL;

      /* repeate again with the next entry in the old chain */
      entry = next;
    }
  }

  free(table);
  map->table = newtable;
  map->table_size = newsize;

  return 1;
}

void int_int_map_init(int_int_map_t * m) {
  m->table       = NULL;
  m->table_size  = 0;
  m->entry_count = 0;
}

void int_int_map_clear(int_int_map_t * map) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  int_int_map_entry_t ** table = map->table;

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    int_int_map_entry_t * entry = table[i];

    while(entry) {
      /* cache next pointer */
      int_int_map_entry_t * next = entry->next;
      /* destroy this one */
      deinit_key(entry->key);
      deinit_value(entry->value);
      free(entry);
      /* try again with the next */
      entry = next;
    }

    table[i] = NULL;
  }

  /* free buffer */
  free(map->table);

  /* cleared! */
  map->table = NULL;
  map->table_size = 0;
}

int int_int_map_get(int_int_map_t * map, int key, int * value_out) {
  int_int_map_entry_t * list;

  if(map->table == NULL) { return 0; }

  list = *bucket_of(map, key);

  while(list) {
    if(compare_key(list->key, key)) {
      *value_out = list->value;
      return 1;
    }
    list = list->next;
  }

  return 0;
}

int int_int_map_set(int_int_map_t * map, int key, int value) {
  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(int_int_map_entry_t *), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return 0; }

    map->table_size = initial_size;
  } else if(map->entry_count > map->table_size*2) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return 0;
    }
  }

  int_int_map_entry_t ** slot = bucket_of(map, key);

  /* advance last slot */
  while(*slot) {
    int_int_map_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* already exists, overwrite */

      /* only deinit value, not key */
      deinit_value(entry->value);
      /* set new value */
      entry->value = value;

      /* successfully set */
      return 1;
    }

    slot = &(*slot)->next;
  }

  /* reached end of chain, create a new entry */
  int_int_map_entry_t * new_entry = malloc(sizeof(*new_entry));

  /* couldn't alloc, escape before anything breaks */
  if(!new_entry) { return 0; }

  new_entry->next = NULL;
  new_entry->key = key;
  new_entry->value = value;
  *slot = new_entry;

  map->entry_count ++;

  /* successfully set */
  return 1;
}


int int_int_map_has(int_int_map_t * map, int key) {
  int_int_map_entry_t * list;

  if(map->table == NULL) { return 0; }

  list = *bucket_of(map, key);

  while(list) {
    if(compare_key(list->key, key)) {
      return 1;
    }
    list = list->next;
  }

  return 0;
}

int int_int_map_erase(int_int_map_t * map, int key) {
  if(map->table == NULL) { return 0; }

  int_int_map_entry_t ** slot = bucket_of(map, key);

  while(*slot) {
    int_int_map_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* matches, skip over */
      *slot = entry->next;

      /* free */
      deinit_key(entry->key);
      deinit_value(entry->value);
      free(entry);

      /* one less entry total */
      map->entry_count --;

      return 1;
    }

    slot = &entry->next;
  }

  return 0;
}
