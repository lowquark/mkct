#include "int_list.h"

#include <stddef.h>
#include <stdlib.h>

void int_list_init(int_list_t * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void int_list_clear(int_list_t * l) {
  int_list_t * l_iter;
  int_list_t * l_iter_next;
  int_list_node_t * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (int_list_node_t *)l_iter;

    /* deinit(&node->value) */
    free(node);

    l_iter = l_iter_next;
  }

  int_list_init(l);
}

void int_list_erase(int_list_node_t * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  /* deinit(&node->value) */

  free(node);
}

int_list_node_t * int_list_pushback(int_list_t * l, int value) {
  int_list_node_t * newnode = calloc(sizeof(int_list_node_t), 1);

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

int_list_node_t * int_list_pushfront(int_list_t * l, int value) {
  int_list_node_t * newnode = calloc(sizeof(int_list_node_t), 1);

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

int_list_node_t * int_list_insertbefore(int_list_node_t * node, int value) {
  int_list_node_t * newnode = calloc(sizeof(int_list_node_t), 1);

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

int_list_node_t * int_list_insertafter(int_list_node_t * node, int value) {
  int_list_node_t * newnode = calloc(sizeof(int_list_node_t), 1);

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

int_list_node_t * int_list_first(const int_list_t * l) {
  if(l->next == l) { return 0; }
  return (int_list_node_t *)l->next;
}

int_list_node_t * int_list_last(const int_list_t * l) {
  if(l->prev == l) { return 0; }
  return (int_list_node_t *)l->prev;
}

int_list_node_t * int_list_next(const int_list_node_t * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (int_list_node_t *)l->list.next;
}

int_list_node_t * int_list_prev(const int_list_node_t * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (int_list_node_t *)l->list.prev;
}
