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

#ifndef ILIST_H
#define ILIST_H

/*
 * ilist.h / ilist.c
 *
 * Implements the following circular linked list structure:
 *
 *       list       node       node       node
 *     +-------+  +-------+  +-------+  +-------+
 *  +->| list  |->| list  |->| list  |->| list  |--+
 *  |  +-------+  + - - - +  + - - - +  + - - - +  |
 *  |             | value |  | value |  | value |  |
 *  |             +-------+  +-------+  +-------+  |
 *  +----------------------------------------------+
 *
 */

typedef struct ilist {
  struct ilist * next;
  struct ilist * prev;
  struct ilist * head;
} ilist_t;

typedef struct ilist_node {
  ilist_t list;
  int value;
} ilist_node_t;

/*
 * Initializes the given list to a valid state.
 */
void ilist_init(ilist_t * list);

/*
 * Deletes and removes all values present in the list.
 */
void ilist_clear(ilist_t * list);

/*
 * Deletes a single node and removes it from its list.
 */
void ilist_erase(ilist_node_t * node);

/*
 * Creates a new node with value `value` and inserts it at the back of `list`.
 *
 * Returns the new node.
 */
ilist_node_t * ilist_pushback(ilist_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it at the front of `list`.
 *
 * Returns the new node.
 */
ilist_node_t * ilist_pushfront(ilist_t * list, int value);

/*
 * Creates a new node with value `value` and inserts it before `node`.
 *
 * Returns the new node.
 */
ilist_node_t * ilist_insertbefore(ilist_node_t * node, int value);

/*
 * Creates a new node with value `value` and inserts it after `node`.
 *
 * Returns the new node.
 */
ilist_node_t * ilist_insertafter(ilist_node_t * node, int value);

/*
 * Returns the first node in the list. If the list is empty, returns NULL.
 */
ilist_node_t * ilist_first(const ilist_t * list);

/*
 * Returns the last node in the list. If the list is empty, returns NULL.
 */
ilist_node_t * ilist_last(const ilist_t * list);

/*
 * Returns the next node in the list after `node`. If `node` is the last node
 * in the list, returns NULL.
 */
ilist_node_t * ilist_next(const ilist_node_t * node);

/*
 * Returns the previous node in the list after `node`. If `node` is the first
 * node in the list, returns NULL.
 */
ilist_node_t * ilist_prev(const ilist_node_t * node);

#endif
