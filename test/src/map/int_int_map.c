#include "int_int_map.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*  ========  key functionality  ========  */


/* TODO: Implement hash for int. */
static unsigned long hash_key(int key) {
  return (*(unsigned long*)&key);
}

/* Called to compare keys. Must return 1 if keys match, and 0 if they don't. */
#define compare_key(key0, key1) ((key0) == (key1))
/* Alternatively: */
/*
static int compare_key(int key0, int key1) {
  return memcmp(&key0, &key1, sizeof(int)) == 0;
}
*/


/*  ========  general functionality  ========  */


typedef enum entry_flag {
  ENTRY_FLAG_NULL = 0,
  ENTRY_FLAG_SET,
  ENTRY_FLAG_UNSET,
} entry_flag_t;

typedef struct int_int_map_entry {
  entry_flag_t flag;
  int     key;
  int   value;
} int_int_map_entry_t;


static const unsigned long initial_size = 32;

/* search for an entry in the table */
static int_int_map_entry_t * find(int_int_map_t * map, int key) {
  unsigned long idx;
  unsigned long first_idx;

  idx = hash_key(key) % map->table_size;
  first_idx = idx;

  /* iterate over set and unset entries in this linearly-probed chain */
  while(map->table[idx].flag != ENTRY_FLAG_NULL) {
    /* compare key if set */
    if(map->table[idx].flag == ENTRY_FLAG_SET && compare_key(map->table[idx].key, key)) {
      /* this is the one */
      return map->table + idx;
    }

    idx ++;
    /* wrap */
    if(idx >= map->table_size) { idx -= map->table_size; }
    /* searched whole table, give up */
    if(idx == first_idx) { return NULL; }
  }

  /* reached end of chain, give up */
  return NULL;
}

/* search for an unset entry, or a set entry with a matching key in the table */
static int_int_map_entry_t * find_insert(int_int_map_t * map, int key) {
  unsigned long idx;
  unsigned long first_idx;

  idx = hash_key(key) % map->table_size;
  first_idx = idx;

  /* skip set entries whose keys do not match */
  while(map->table[idx].flag == ENTRY_FLAG_SET && !compare_key(map->table[idx].key, key)) {
    idx ++;
    /* wrap */
    if(idx >= map->table_size) { idx -= map->table_size; }
    /* searched whole table, give up */
    if(idx == first_idx) { return NULL; }
  }

  /* this marks the first null; unset; or set entry whose key matches */
  return map->table + idx;
}

static int resize_table(int_int_map_t * map, unsigned long newsize) {
  unsigned long idx;
  unsigned long new_fill_count = 0;

  unsigned long i;
  unsigned long table_size = map->table_size;
  int_int_map_entry_t * table = map->table;
  int_int_map_entry_t * newtable = calloc(sizeof(*newtable), newsize);
  int_int_map_entry_t * entry;

  assert(newsize >= table_size);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    entry = table + i;
    /* look for set entries */
    if(entry->flag == ENTRY_FLAG_SET) {
      /* copy to new table at hashed location */

      /* new hash location */
      idx = hash_key(entry->key) % newsize;

      /* skip set entries, also key matches are not possible */
      while(newtable[idx].flag == ENTRY_FLAG_SET) {
        idx ++;
        /* wrap */
        if(idx >= newsize) { idx -= newsize; }
        /* infinite loop not possible, given newsize >= table_size */
      }

      /* newtable[idx] is the first null */
      newtable[idx].flag  = ENTRY_FLAG_SET;
      newtable[idx].key   = entry->key;
      newtable[idx].value = entry->value;

      /* update new fill count */
      new_fill_count ++;
    }
  }

  /* free old table and replace */
  free(table);
  map->table = newtable;
  map->table_size = newsize;
  map->fill_count = new_fill_count;

  return 1;
}

void int_int_map_init(int_int_map_t * map) {
  assert(map);

  map->table      = NULL;
  map->table_size = 0;
  map->fill_count = 0;
}

void int_int_map_clear(int_int_map_t * map) {
  assert(map);

  /* free buffer */
  free(map->table);

  /* cleared! */
  map->table = NULL;
  map->table_size = 0;
}

int int_int_map_get(int_int_map_t * map, int key, int * value_out) {
  int_int_map_entry_t * entry;

  assert(map);

  if(map->table == NULL) { return 0; }

  entry = find(map, key);

  if(entry) {
    *value_out = entry->value;
  }

  return entry != NULL;
}

int int_int_map_set(int_int_map_t * map, int key, int value) {
  int_int_map_entry_t * entry;

  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(int_int_map_entry_t), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return 0; }

    map->table_size = initial_size;
  } else if(map->fill_count * 2 > map->table_size) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return 0;
    }
  }

  entry = find_insert(map, key);

  if(entry) {
    if(entry->flag == ENTRY_FLAG_NULL) {
      /* previously null, increment fill count */
      map->fill_count ++;
    }

    entry->flag  = ENTRY_FLAG_SET;
    entry->key   = key;
    entry->value = value;
  }

  return entry != NULL;
}


int int_int_map_has(int_int_map_t * map, int key) {
  assert(map);

  if(map->table == NULL) { return 0; }

  return find(map, key) != NULL;
}

int int_int_map_erase(int_int_map_t * map, int key) {
  int_int_map_entry_t * entry;

  assert(map);

  if(map->table == NULL) { return 0; }

  entry = find(map, key);

  if(entry) {
    entry->flag = ENTRY_FLAG_UNSET;
  }

  return entry != NULL;
}
