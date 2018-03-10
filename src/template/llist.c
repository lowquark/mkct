
#include "H_FILE"

#include <stddef.h>
#include <stdlib.h>

void LIST_METHOD_INIT(LIST_TYPEDEF * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void LIST_METHOD_CLEAR(LIST_TYPEDEF * l) {
  LIST_TYPEDEF * l_iter;
  LIST_TYPEDEF * l_iter_next;
  NODE_TYPEDEF * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (NODE_TYPEDEF *)l_iter;

    /* deinit(&node->value) */
    free(node);

    l_iter = l_iter_next;
  }

  LIST_METHOD_INIT(l);
}

void LIST_METHOD_ERASE(NODE_TYPEDEF * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  /* deinit(&node->value) */

  free(node);
}

NODE_TYPEDEF * LIST_METHOD_PUSHBACK(LIST_TYPEDEF * l, VALUE_TYPEDEF value) {
  NODE_TYPEDEF * newnode = calloc(sizeof(NODE_TYPEDEF), 1);

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

NODE_TYPEDEF * LIST_METHOD_PUSHFRONT(LIST_TYPEDEF * l, VALUE_TYPEDEF value) {
  NODE_TYPEDEF * newnode = calloc(sizeof(NODE_TYPEDEF), 1);

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

NODE_TYPEDEF * LIST_METHOD_INSERTBEFORE(NODE_TYPEDEF * node, VALUE_TYPEDEF value) {
  NODE_TYPEDEF * newnode = calloc(sizeof(NODE_TYPEDEF), 1);

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

NODE_TYPEDEF * LIST_METHOD_INSERTAFTER(NODE_TYPEDEF * node, VALUE_TYPEDEF value) {
  NODE_TYPEDEF * newnode = calloc(sizeof(NODE_TYPEDEF), 1);

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

NODE_TYPEDEF * LIST_METHOD_FIRST(const LIST_TYPEDEF * l) {
  if(l->next == l) { return 0; }
  return (NODE_TYPEDEF *)l->next;
}

NODE_TYPEDEF * LIST_METHOD_LAST(const LIST_TYPEDEF * l) {
  if(l->prev == l) { return 0; }
  return (NODE_TYPEDEF *)l->prev;
}

NODE_TYPEDEF * LIST_METHOD_NEXT(const NODE_TYPEDEF * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (NODE_TYPEDEF *)l->list.next;
}

NODE_TYPEDEF * LIST_METHOD_PREV(const NODE_TYPEDEF * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (NODE_TYPEDEF *)l->list.prev;
}

