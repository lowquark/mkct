#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD


struct ENTRY_STRUCT;

typedef struct MAP_STRUCT {
  struct ENTRY_STRUCT ** table;
  unsigned long table_size;
  unsigned long entry_count;
} MAP_TYPEDEF;


void MAP_METHOD_INIT  (MAP_TYPEDEF * map);

void MAP_METHOD_CLEAR (MAP_TYPEDEF * map);


int  MAP_METHOD_GET   (MAP_TYPEDEF * map, KEY_TYPEDEF key, VALUE_TYPEDEF * value_out);

int  MAP_METHOD_SET   (MAP_TYPEDEF * map, KEY_TYPEDEF key, VALUE_TYPEDEF value);


int  MAP_METHOD_HAS   (MAP_TYPEDEF * map, KEY_TYPEDEF key);

int  MAP_METHOD_ERASE (MAP_TYPEDEF * map, KEY_TYPEDEF key);

#endif
