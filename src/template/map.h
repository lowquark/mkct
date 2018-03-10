#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD


struct ENTRY_STRUCT;

typedef struct MAP_STRUCT {
  struct ENTRY_STRUCT ** table;
  unsigned long table_size;
  unsigned long entry_count;
} MAP_TYPE;


void MAP_METHOD_INIT  (MAP_TYPE * map);

void MAP_METHOD_CLEAR (MAP_TYPE * map);


int  MAP_METHOD_GET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE * value_out);

int  MAP_METHOD_SET   (MAP_TYPE * map, KEY_TYPE key, VALUE_TYPE value);


int  MAP_METHOD_HAS   (MAP_TYPE * map, KEY_TYPE key);

int  MAP_METHOD_ERASE (MAP_TYPE * map, KEY_TYPE key);

#endif
