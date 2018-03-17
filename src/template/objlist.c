
#include "H_FILE"

#include <stddef.h>
#include <stdlib.h>


/*  ========  object functionaility  ========  */


/* This function is called after an object's memory has been allocated. */
static void object_init(OBJECT_TYPE * obj) {
  memset(obj, 0, sizeof(OBJECT_TYPE));
}

/* This function is called before an object's memory is freed. */
static void object_clear(OBJECT_TYPE * obj) {
}


/*  ========  general functionaility  ========  */


void OBJLIST_METHOD_INIT(OBJLIST_TYPE * l) {
  l->next = l;
  l->prev = l;
  l->head = l;
}

void OBJLIST_METHOD_CLEAR(OBJLIST_TYPE * l) {
  OBJLIST_TYPE * l_iter;
  OBJLIST_TYPE * l_iter_next;
  NODE_TYPE * node;

  l_iter = l->next;

  while(l_iter != l) {
    l_iter_next = l_iter->next;

    /* no offsetof *should* be necessary */
    node = (NODE_TYPE *)l_iter;

    object_clear(&node->value);
    free(node);

    l_iter = l_iter_next;
  }

  OBJLIST_METHOD_INIT(l);
}

void OBJLIST_METHOD_ERASE(NODE_TYPE * node) {
  node->list.prev->next = node->list.next;
  node->list.next->prev = node->list.prev;

  node->list.next = &node->list;
  node->list.prev = &node->list;
  node->list.head = &node->list;

  object_clear(&node->value);
  free(node);
}

NODE_TYPE * OBJLIST_METHOD_PUSHBACK(OBJLIST_TYPE * l) {
  NODE_TYPE * newnode = malloc(sizeof(NODE_TYPE));

  if(newnode) {
    object_init(&newnode->value);

    newnode->list.next       = l;
    newnode->list.prev       = l->prev;
    l->prev                  = &newnode->list;
    newnode->list.prev->next = &newnode->list;
    newnode->list.head       = l->head;
  }

  return newnode;
}

NODE_TYPE * OBJLIST_METHOD_PUSHFRONT(OBJLIST_TYPE * l) {
  NODE_TYPE * newnode = malloc(sizeof(NODE_TYPE));

  if(newnode) {
    object_init(&newnode->value);

    newnode->list.prev       = l;
    newnode->list.next       = l->next;
    l->next                  = &newnode->list;
    newnode->list.next->prev = &newnode->list;
    newnode->list.head       = l->head;
  }

  return newnode;
}

NODE_TYPE * OBJLIST_METHOD_INSERTBEFORE(NODE_TYPE * node) {
  NODE_TYPE * newnode = malloc(sizeof(NODE_TYPE));

  if(newnode) {
    object_init(&newnode->value);

    newnode->list.next       = &node->list;
    newnode->list.prev       = node->list.prev;
       node->list.prev       = &newnode->list;
    newnode->list.prev->next = &newnode->list;
    newnode->list.head       = node->list.head;
  }

  return newnode;
}

NODE_TYPE * OBJLIST_METHOD_INSERTAFTER(NODE_TYPE * node) {
  NODE_TYPE * newnode = malloc(sizeof(NODE_TYPE));

  if(newnode) {
    object_init(&newnode->value);

    newnode->list.prev       = &node->list;
    newnode->list.next       = node->list.next;
       node->list.next       = &newnode->list;
    newnode->list.next->prev = &newnode->list;
    newnode->list.head       = node->list.head;
  }

  return newnode;
}

NODE_TYPE * OBJLIST_METHOD_FIRST(const OBJLIST_TYPE * l) {
  if(l->next == l) { return 0; }
  return (NODE_TYPE *)l->next;
}

NODE_TYPE * OBJLIST_METHOD_LAST(const OBJLIST_TYPE * l) {
  if(l->prev == l) { return 0; }
  return (NODE_TYPE *)l->prev;
}

NODE_TYPE * OBJLIST_METHOD_NEXT(const NODE_TYPE * l) {
  if(l->list.next == l->list.head) { return 0; }
  return (NODE_TYPE *)l->list.next;
}

NODE_TYPE * OBJLIST_METHOD_PREV(const NODE_TYPE * l) {
  if(l->list.prev == l->list.head) { return 0; }
  return (NODE_TYPE *)l->list.prev;
}

