#ifndef OBJ_H
#define OBJ_H

#define OBJ_INITIAL_A 4
#define OBJ_INITIAL_B 5
#define OBJ_INITIAL_C 6

typedef struct obj {
  int a, b, c;
} obj_t;

int obj_num();

void obj_init(obj_t * obj);

void obj_clear(obj_t * obj);

#endif
