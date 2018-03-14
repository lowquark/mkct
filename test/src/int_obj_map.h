#ifndef _INT_OBJ_MAP_H_
#define _INT_OBJ_MAP_H_


struct int_obj_map_entry;

typedef struct int_obj_map {
  struct int_obj_map_entry ** table;
  unsigned long table_size;
  unsigned long entry_count;
} int_obj_map_t;


void int_obj_map_init  (int_obj_map_t * map);

void int_obj_map_clear (int_obj_map_t * map);


int  int_obj_map_get   (int_obj_map_t * map, int key, int * value_out);

int  int_obj_map_set   (int_obj_map_t * map, int key, int value);


int  int_obj_map_has   (int_obj_map_t * map, int key);

int  int_obj_map_erase (int_obj_map_t * map, int key);

#endif
