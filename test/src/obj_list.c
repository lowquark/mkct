#include "obj_list.h"

#include <stddef.h>
#include <stdlib.h>

void obj_list_init(obj_list_t * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void obj_list_clear(obj_list_t * l) {
  obj_list_t * l_iter;
  obj_list_t * l_iter_next;
  obj_list_node_t * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (obj_list_node_t *)l_iter;

    /* deinit(&node->value) */
    free(node);

    l_iter = l_iter_next;
  }

  obj_list_init(l);
}

void obj_list_erase(obj_list_node_t * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  /* deinit(&node->value) */

  free(node);
}

obj_list_node_t * obj_list_pushback(obj_list_t * l, int value) {
  obj_list_node_t * newnode = calloc(sizeof(obj_list_node_t), 1);

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

obj_list_node_t * obj_list_pushfront(obj_list_t * l, int value) {
  obj_list_node_t * newnode = calloc(sizeof(obj_list_node_t), 1);

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

obj_list_node_t * obj_list_insertbefore(obj_list_node_t * node, int value) {
  obj_list_node_t * newnode = calloc(sizeof(obj_list_node_t), 1);

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

obj_list_node_t * obj_list_insertafter(obj_list_node_t * node, int value) {
  obj_list_node_t * newnode = calloc(sizeof(obj_list_node_t), 1);

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

obj_list_node_t * obj_list_first(const obj_list_t * l) {
  if(l->next == l) { return 0; }
  return (obj_list_node_t *)l->next;
}

obj_list_node_t * obj_list_last(const obj_list_t * l) {
  if(l->prev == l) { return 0; }
  return (obj_list_node_t *)l->prev;
}

obj_list_node_t * obj_list_next(const obj_list_node_t * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (obj_list_node_t *)l->list.next;
}

obj_list_node_t * obj_list_prev(const obj_list_node_t * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (obj_list_node_t *)l->list.prev;
}
