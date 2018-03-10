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

#ifndef IMAP_OBJ_H
#define IMAP_OBJ_H

#include "obj.h"

struct imap_obj_entry;

/*
 * Hash map from `int` keys to `obj_t` values. Manages
 * initialization and allocation of values, and stores shallow copies of their
 * keys.
 */
typedef struct imap_obj {
  struct imap_obj_entry ** table;
  unsigned long table_size;
  unsigned long entry_count;
} imap_obj_t;

/* Initializes the given `imap_obj_t` to a valid, empty state.
 *
 * Warning: No memory will be freed. Use imap_obj_clear to erase all values in the map.
 */
void imap_obj_init(imap_obj_t * map);

/* Destroys all values in the map, and destroys all allocated memory owned by the map. */
void imap_obj_clear(imap_obj_t * map);

/* Looks up the value using a given key.
 *
 * Returns a pointer to the found value. Returns NULL if no value has key `key`.
 */
obj_t * imap_obj_find(imap_obj_t * map, int key);

/* Creates a new value for the given key.
 *
 * If an entry is already at `key`, the old entry is destroyed and the new one is put in its place.
 *
 * Returns a pointer to the new entry.
 */
obj_t * imap_obj_create(imap_obj_t * map, int key);

/* Finds and destroys the value with the given key.
 *
 * Returns 1 if the value was found (and destroyed) and 0 otherwise.
 */
int imap_obj_destroy(imap_obj_t * map, int key);

#endif
