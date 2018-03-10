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

#include "imap_obj.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct imap_obj_entry {
  struct imap_obj_entry * next;
  int   key;
  obj_t value;
} imap_obj_entry_t;

/*** type specific functionaility ***/

/* TODO: Implement hash for int. */
static unsigned long hash_key(int key) {
  return (*(unsigned long*)&key);
}

/* Called to compare keys. Must return 1 if keys match, and 0 if they don't.
 *
 * Alternatively:
 * static int compare_key(int key0, int key1) {
 *   return memcmp(&key0, &key1, sizeof(int)) == 0;
 * }
 */

#define compare_key(key0, key1) ((key0) == (key1))

/* TODO: Cleanup the key, if applicable. This function is called
 * when entries are erased, but not when they are overwritten. */
static void deinit_key(int * key) {
}

/* TODO: Initialize a value, if applicable. This function is called
 * when entries created or overwritten. */
static void init_value(obj_t * value) {
  obj_init(value);
}

/* TODO: Cleanup a value, if applicable. This function is called
 * when entries are erased or overwritten. */
static void deinit_value(obj_t * value) {
  obj_clear(value);
}

/*** general functionaility ***/

static const unsigned long initial_size = 32;

static unsigned long hash_idx(int key, unsigned long table_size) {
  assert(table_size > 0);

  return hash_key(key) % table_size;
}

static imap_obj_entry_t ** bucket_of(imap_obj_t * map, int key) {
  assert(map->table);

  return map->table + hash_idx(key, map->table_size);
}

static int resize_table(imap_obj_t * map, unsigned long newsize) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  imap_obj_entry_t ** table = map->table;
  imap_obj_entry_t ** newtable = calloc(sizeof(*newtable), newsize);

  if(!newtable) {
    return 0;
  }

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    imap_obj_entry_t * entry = table[i];

    while(entry) {
      /* save next pointer */
      imap_obj_entry_t * next = entry->next;

      unsigned long idx = hash_idx(entry->key, newsize);

      /* lookup chain in the new table */
      imap_obj_entry_t ** slot = newtable + idx;

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

void imap_obj_init(imap_obj_t * m) {
  m->table       = NULL;
  m->table_size  = 0;
  m->entry_count = 0;
}

void imap_obj_clear(imap_obj_t * map) {
  unsigned long i;
  unsigned long table_size = map->table_size;
  imap_obj_entry_t ** table = map->table;

  for(i = 0 ; i < table_size ; i ++) {
    /* free chain */
    imap_obj_entry_t * entry = table[i];

    while(entry) {
      /* cache next pointer */
      imap_obj_entry_t * next = entry->next;
      /* destroy this one */
      deinit_key(&entry->key);
      deinit_value(&entry->value);
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

obj_t * imap_obj_find(imap_obj_t * map, int key) {
  imap_obj_entry_t * list;

  if(map->table == NULL) { return NULL; }

  list = *bucket_of(map, key);

  while(list) {
    if(compare_key(list->key, key)) {
      return &list->value;
    }
    list = list->next;
  }

  return NULL;
}

obj_t * imap_obj_create(imap_obj_t * map, int key) {
  assert(map);

  if(map->table == NULL) { 
    /* allocate since not allocated already */
    map->table = calloc(sizeof(imap_obj_entry_t *), initial_size);

    /* couldn't alloc, escape before anything breaks */
    if(!map->table) { return NULL; }

    map->table_size = initial_size;
  } else if(map->entry_count > map->table_size*2) {
    if(!resize_table(map, map->table_size * 2)) {
      /* couldn't resize, escape before anything breaks */
      return NULL;
    }
  }

  imap_obj_entry_t ** slot = bucket_of(map, key);

  /* advance last slot */
  while(*slot) {
    imap_obj_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* already exists, only deinit value, not key */
      deinit_value(&entry->value);
      init_value(&entry->value);
      /* good as new */
      return &entry->value;
    }

    slot = &(*slot)->next;
  }

  /* reached end of chain, create a new entry */
  imap_obj_entry_t * new_entry = malloc(sizeof(*new_entry));

  /* couldn't alloc, escape before anything breaks */
  if(!new_entry) { return NULL; }

  new_entry->next = NULL;
  new_entry->key = key;
  *slot = new_entry;

  /* initialize object */
  init_value(&new_entry->value);

  map->entry_count ++;

  return &new_entry->value;
}


int imap_obj_destroy(imap_obj_t * map, int key) {
  if(map->table == NULL) { return 0; }

  imap_obj_entry_t ** slot = bucket_of(map, key);

  while(*slot) {
    imap_obj_entry_t * entry = *slot;

    if(compare_key(entry->key, key)) {
      /* matches, skip over */
      *slot = entry->next;

      /* free */
      deinit_key(&entry->key);
      deinit_value(&entry->value);
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
