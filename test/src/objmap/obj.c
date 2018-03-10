
#include "obj.h"

static int _obj_num = 0;

int obj_num() {
  return _obj_num;
}

void obj_init(obj_t * obj) {
  obj->a = 4;
  obj->b = 5;
  obj->c = 6;

  _obj_num ++;
}

void obj_clear(obj_t * obj) {
  obj->a = 0;
  obj->b = 0;
  obj->c = 0;

  _obj_num --;
}

