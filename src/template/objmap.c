
#include "H_FILE"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct ENTRY_STRUCT {
  struct ENTRY_STRUCT * next;
  KEY_TYPE   key;
  OBJECT_TYPE object;
} ENTRY_TYPE;

/*** type specific functionaility ***/

/* TODO: Implement hash for KEY_TYPE. */
static unsigned long hash_key(KEY_TYPE key) {
  return (*(unsigned long*)&key);
}

/* Called to compare keys. Must return 1 if keys match, and 0 if they don't.
 *
 * Alternatively:
 * static int compare_key(KEY_TYPE key0, KEY_TYPE key1) {
 *   return memcmp(&key0, &key1, sizeof(KEY_TYPE)) == 0;
 * }
 */

#define compare_key(key0, key1) ((key0) == (key1))

/* TODO: Cleanup the key, if applicable. This function is called
 * when entries are erased, but not when they are overwritten. */
static void deinit_key(KEY_TYPE * key) {
}

/* TODO: Initialize a object, if applicable. This function is called
 * when entries created or overwritten. */
static void init_object(OBJECT_TYPE * object) {
  memset(object, 0, sizeof(OBJECT_TYPE));
}

/* TODO: Cleanup a object, if applicable. This function is called
 * when entries are erased or overwritten. */
static void deinit_object(OBJECT_TYPE * object) {
  memset(object, 0, sizeof(OBJECT_TYPE));
}

/*** general functionaility ***/

static const unsigned long initial_size = 32;

static unsigned long hash_idx(KEY_TYPE key, unsigned long table_size) {
  assert(table_size > 0);

  return hash_key(key) % table_size;
}

static ENTRY_TYPE ** bucket_of(OBJMAP_TYPE * map, KEY_TYPE key) {
  assert(map->table);

  return map->table + hash_idx(key, map->table_size);
}

static int resize_table(OBJMAP_TYPE * map, unsigned long newsize) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  ENTRY_TYPE ** table = map->table;
  ENTRY_TYPE ** newtable = calloc(sizeof(*newtable), newsize);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    ENTRY_TYPE * entry = table[i];

    while(entry) {
      /* save next pointer */
      ENTRY_TYPE * next = entry->next;

      unsigned long idx = hash_idx(entry->key, newsize);

      /* lookup chain in the new table */
      ENTRY_TYPE ** slot = newtable + idx;

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

void OBJMAP_METHOD_INIT(OBJMAP_TYPE * m) {
  m->table       = NULL;
  m->table_size  = 0;
  m->entry_count = 0;
}

void OBJMAP_METHOD_CLEAR(OBJMAP_TYPE * map) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  ENTRY_TYPE ** table = map->table;

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    ENTRY_TYPE * entry = table[i];

    while(entry) {
      /* cache next pointer */
      ENTRY_TYPE * next = entry->next;
      /* destroy this one */
      deinit_key(&entry->key);
      deinit_object(&entry->object);
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

OBJECT_TYPE * OBJMAP_METHOD_FIND(OBJMAP_TYPE * map, KEY_TYPE key) {
  ENTRY_TYPE * list;

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

OBJECT_TYPE * OBJMAP_METHOD_CREATE(OBJMAP_TYPE * map, KEY_TYPE key) {
  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(ENTRY_TYPE *), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return NULL; }

    map->table_size = initial_size;
  } else if(map->entry_count > map->table_size*2) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return NULL;
    }
  }

  ENTRY_TYPE ** slot = bucket_of(map, key);

  /* advance last slot */
  while(*slot) {
    ENTRY_TYPE * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* already exists, only deinit object, not key */
      deinit_object(&entry->object);
      init_object(&entry->object);
      /* good as new */
      return &entry->object;
    }

    slot = &(*slot)->next;
  }

  /* reached end of chain, create a new entry */
  ENTRY_TYPE * new_entry = malloc(sizeof(*new_entry));

  /* couldn't alloc, escape before anything breaks */
  if(!new_entry) { return NULL; }

  new_entry->next = NULL;
  new_entry->key = key;
  *slot = new_entry;

  /* initialize object */
  init_object(&new_entry->object);

  map->entry_count ++;

  return &new_entry->object;
}


int OBJMAP_METHOD_DESTROY(OBJMAP_TYPE * map, KEY_TYPE key) {
  if(map->table == NULL) { return 0; }

  ENTRY_TYPE ** slot = bucket_of(map, key);

  while(*slot) {
    ENTRY_TYPE * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* matches, skip over */
      *slot = entry->next;

      /* free */
      deinit_key(&entry->key);
      deinit_object(&entry->object);
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

