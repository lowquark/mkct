/* 
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org>
 */

#include "iimap.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


typedef struct iimap_entry {
  struct iimap_entry * next;
  int   key;
  int value;
} iimap_entry_t;


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

static iimap_entry_t ** bucket_of(iimap_t * map, int key) {
  assert(map->table);

  return map->table + hash_idx(key, map->table_size);
}

static int resize_table(iimap_t * map, unsigned long newsize) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  iimap_entry_t ** table = map->table;
  iimap_entry_t ** newtable = calloc(sizeof(*newtable), newsize);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    iimap_entry_t * entry = table[i];

    while(entry) {
      /* save next pointer */
      iimap_entry_t * next = entry->next;

      unsigned long idx = hash_idx(entry->key, newsize);

      /* lookup chain in the new table */
      iimap_entry_t ** slot = newtable + idx;

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

void iimap_init(iimap_t * m) {
  m->table       = NULL;
  m->table_size  = 0;
  m->entry_count = 0;
}

void iimap_clear(iimap_t * map) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  iimap_entry_t ** table = map->table;

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    iimap_entry_t * entry = table[i];

    while(entry) {
      /* cache next pointer */
      iimap_entry_t * next = entry->next;
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

int iimap_get(iimap_t * map, int key, int * value_out) {
  iimap_entry_t * list;

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

int iimap_set(iimap_t * map, int key, int value) {
  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(iimap_entry_t *), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return 0; }

    map->table_size = initial_size;
  } else if(map->entry_count > map->table_size*2) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return 0;
    }
  }

  iimap_entry_t ** slot = bucket_of(map, key);

  /* advance last slot */
  while(*slot) {
    iimap_entry_t * entry = *slot;

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
  iimap_entry_t * new_entry = malloc(sizeof(*new_entry));

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


int iimap_has(iimap_t * map, int key) {
  iimap_entry_t * list;

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

int iimap_erase(iimap_t * map, int key) {
  if(map->table == NULL) { return 0; }

  iimap_entry_t ** slot = bucket_of(map, key);

  while(*slot) {
    iimap_entry_t * entry = *slot;

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
