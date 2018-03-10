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

#include "ilist.h"

#include <stddef.h>
#include <stdlib.h>

void ilist_init(ilist_t * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void ilist_clear(ilist_t * l) {
  ilist_t * l_iter;
  ilist_t * l_iter_next;
  ilist_node_t * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (ilist_node_t *)l_iter;

    /* deinit(&node->value) */
    free(node);

    l_iter = l_iter_next;
  }

  ilist_init(l);
}

void ilist_erase(ilist_node_t * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  /* deinit(&node->value) */

  free(node);
}

ilist_node_t * ilist_pushback(ilist_t * l, int value) {
  ilist_node_t * newnode = calloc(sizeof(ilist_node_t), 1);

  if(newnode) {
    newnode->list.next       = l;
    newnode->list.prev       = l->prev;
    l->prev                  = &newnode->list;
    newnode->list.prev->next = &newnode->list;
    newnode->list.head       = l->head;
    newnode->value = value;
  }

  return newnode;
}

ilist_node_t * ilist_pushfront(ilist_t * l, int value) {
  ilist_node_t * newnode = calloc(sizeof(ilist_node_t), 1);

  if(newnode) {
    newnode->list.prev       = l;
    newnode->list.next       = l->next;
    l->next                  = &newnode->list;
    newnode->list.next->prev = &newnode->list;
    newnode->list.head       = l->head;
    newnode->value = value;
  }

  return newnode;
}

ilist_node_t * ilist_insertbefore(ilist_node_t * node, int value) {
  ilist_node_t * newnode = calloc(sizeof(ilist_node_t), 1);

  if(newnode) {
    newnode->list.next       = &node->list;
    newnode->list.prev       = node->list.prev;
       node->list.prev       = &newnode->list;
    newnode->list.prev->next = &newnode->list;
    newnode->list.head       = node->list.head;
    newnode->value           = value;
  }

  return newnode;
}

ilist_node_t * ilist_insertafter(ilist_node_t * node, int value) {
  ilist_node_t * newnode = calloc(sizeof(ilist_node_t), 1);

  if(newnode) {
    newnode->list.prev       = &node->list;
    newnode->list.next       = node->list.next;
       node->list.next       = &newnode->list;
    newnode->list.next->prev = &newnode->list;
    newnode->list.head       = node->list.head;
    newnode->value           = value;
  }

  return newnode;
}

ilist_node_t * ilist_first(const ilist_t * l) {
  if(l->next == l) { return 0; }
  return (ilist_node_t *)l->next;
}

ilist_node_t * ilist_last(const ilist_t * l) {
  if(l->prev == l) { return 0; }
  return (ilist_node_t *)l->prev;
}

ilist_node_t * ilist_next(const ilist_node_t * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (ilist_node_t *)l->list.next;
}

ilist_node_t * ilist_prev(const ilist_node_t * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (ilist_node_t *)l->list.prev;
}
