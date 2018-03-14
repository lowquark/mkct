
#include "H_FILE"

#include <stddef.h>
#include <stdlib.h>

void LIST_METHOD_INIT(LIST_TYPE * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void LIST_METHOD_CLEAR(LIST_TYPE * l) {
  LIST_TYPE * l_iter;
  LIST_TYPE * l_iter_next;
  NODE_TYPE * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (NODE_TYPE *)l_iter;

    free(node);

    l_iter = l_iter_next;
  }

  LIST_METHOD_INIT(l);
}

void LIST_METHOD_ERASE(NODE_TYPE * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  free(node);
}

NODE_TYPE * LIST_METHOD_PUSHBACK(LIST_TYPE * l, VALUE_TYPE value) {
  NODE_TYPE * newnode = calloc(sizeof(NODE_TYPE), 1);

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

NODE_TYPE * LIST_METHOD_PUSHFRONT(LIST_TYPE * l, VALUE_TYPE value) {
  NODE_TYPE * newnode = calloc(sizeof(NODE_TYPE), 1);

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

NODE_TYPE * LIST_METHOD_INSERTBEFORE(NODE_TYPE * node, VALUE_TYPE value) {
  NODE_TYPE * newnode = calloc(sizeof(NODE_TYPE), 1);

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

NODE_TYPE * LIST_METHOD_INSERTAFTER(NODE_TYPE * node, VALUE_TYPE value) {
  NODE_TYPE * newnode = calloc(sizeof(NODE_TYPE), 1);

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

NODE_TYPE * LIST_METHOD_FIRST(const LIST_TYPE * l) {
  if(l->next == l) { return 0; }
  return (NODE_TYPE *)l->next;
}

NODE_TYPE * LIST_METHOD_LAST(const LIST_TYPE * l) {
  if(l->prev == l) { return 0; }
  return (NODE_TYPE *)l->prev;
}

NODE_TYPE * LIST_METHOD_NEXT(const NODE_TYPE * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (NODE_TYPE *)l->list.next;
}

NODE_TYPE * LIST_METHOD_PREV(const NODE_TYPE * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (NODE_TYPE *)l->list.prev;
}

