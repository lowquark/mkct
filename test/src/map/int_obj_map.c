#include "int_obj_map.h"

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


/*  ========  object functionality  ========  */


/* This function is called after an object's memory has been allocated. */
static void object_init(obj_t * obj) {
  obj_init(obj);
}

/* This function is called before an object's memory is freed. */
static void object_clear(obj_t * obj) {
  obj_clear(obj);
}


/*  ========  general functionality  ========  */


static const unsigned long initial_size = 32;

typedef struct int_obj_map_entry {
  struct int_obj_map_entry * next;
  int   key;
  obj_t object;
} int_obj_map_entry_t;

static unsigned long hash_idx(int key, unsigned long table_size) {
  assert(table_size > 0);

  return hash_key(key) % table_size;
}

static int_obj_map_entry_t ** bucket_of(int_obj_map_t * map, int key) {
  assert(map->table);

  return map->table + hash_idx(key, map->table_size);
}

static int resize_table(int_obj_map_t * map, unsigned long newsize) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  int_obj_map_entry_t ** table = map->table;
  int_obj_map_entry_t ** newtable = calloc(sizeof(*newtable), newsize);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    int_obj_map_entry_t * entry = table[i];

    while(entry) {
      /* save next pointer */
      int_obj_map_entry_t * next = entry->next;

      unsigned long idx = hash_idx(entry->key, newsize);

      /* lookup chain in the new table */
      int_obj_map_entry_t ** slot = newtable + idx;

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

void int_obj_map_init(int_obj_map_t * m) {
  m->table       = NULL;
  m->table_size  = 0;
  m->entry_count = 0;
}

void int_obj_map_clear(int_obj_map_t * map) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  int_obj_map_entry_t ** table = map->table;

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    int_obj_map_entry_t * entry = table[i];

    while(entry) {
      /* cache next pointer */
      int_obj_map_entry_t * next = entry->next;
      /* destroy this one */
      object_clear(&entry->object);
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

obj_t * int_obj_map_find(int_obj_map_t * map, int key) {
  int_obj_map_entry_t * list;

  if(map->table == NULL) { return NULL; }

  list = *bucket_of(map, key);

  while(list) {
    if(compare_key(list->key, key)) {
      return &list->object;
    }
    list = list->next;
  }

  return NULL;
}

obj_t * int_obj_map_create(int_obj_map_t * map, int key) {
  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(int_obj_map_entry_t *), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return NULL; }

    map->table_size = initial_size;
  } else if(map->entry_count > map->table_size*2) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return NULL;
    }
  }

  int_obj_map_entry_t ** slot = bucket_of(map, key);

  /* advance last slot */
  while(*slot) {
    int_obj_map_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* already exists, only deinit object, not key */
      object_clear(&entry->object);
      object_init(&entry->object);
      /* good as new */
      return &entry->object;
    }

    slot = &(*slot)->next;
  }

  /* reached end of chain, create a new entry */
  int_obj_map_entry_t * new_entry = malloc(sizeof(*new_entry));

  /* couldn't alloc, escape before anything breaks */
  if(!new_entry) { return NULL; }

  new_entry->next = NULL;
  new_entry->key = key;
  *slot = new_entry;

  /* initialize object */
  object_init(&new_entry->object);

  map->entry_count ++;

  return &new_entry->object;
}


int int_obj_map_destroy(int_obj_map_t * map, int key) {
  if(map->table == NULL) { return 0; }

  int_obj_map_entry_t ** slot = bucket_of(map, key);

  while(*slot) {
    int_obj_map_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* matches, skip over */
      *slot = entry->next;

      /* free */
      object_clear(&entry->object);
      free(entry);

      /* one less entry total */
      map->entry_count --;

      return 1;
    }

    slot = &entry->next;
  }

  /* nothing was destroyed */
  return 0;
}
