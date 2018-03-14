#ifndef _INT_INT_MAP_H_
#define _INT_INT_MAP_H_


struct int_int_map_entry;

typedef struct int_int_map {
  struct int_int_map_entry ** table;
  unsigned long table_size;
  unsigned long entry_count;
} int_int_map_t;


void int_int_map_init  (int_int_map_t * map);

void int_int_map_clear (int_int_map_t * map);


int  int_int_map_get   (int_int_map_t * map, int key, int * value_out);

int  int_int_map_set   (int_int_map_t * map, int key, int value);


int  int_int_map_has   (int_int_map_t * map, int key);

int  int_int_map_erase (int_int_map_t * map, int key);

#endif
