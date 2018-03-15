
# mkct - C Template Generators

A collection of bash scripts that generate a container for one of your types --
like a C++ template but without the C++.


## `mkct.queue`

Generates a queue (FIFO) for a given value type.

## `mkct.objqueue`

Generates a queue (FIFO) of managed objects for a given object type. Manages
allocation and initialization of objects.

## `mkct.stack`

Generates a stack (FILO) for a given value type.

## `mkct.objstack`

Generates a stack (FILO) of managed objects for a given object type. Manages
allocation and initialization of objects.

## `mkct.list`

Generates a circular linked list for a given value type.

## `mkct.objlist`

Generates a circular linked list for a given object type. Manages allocation
and initialization of objects.

## `mkct.map`

Generates a hash map for given key / value types.

## `mkct.objmap`

Generates a hash map for given key / object types. Manages allocation and
initialization of objects, but not keys.


## Example:

    The following commands will create a simple queue of type `double`:

    $ mkct.queue --header --name=iqueue --value-type=double > dqueue.h
    $ mkct.queue --source --name=iqueue --value-type=double > dqueue.c

`dqueue.h` and `dqueue.c` will be created. See `dqueue.h` for usage.


## Why?

Because I'm tired of boilerplate like this:

    void * some_value;
    int my_key;
    if(generic_map_get((void *)&my_key, sizeof(my_key), &some_value)) {
      int my_actual_value = *(int *)some_value;
    }

And reading generic macro implementations like this:

    typedef unsigned long hash_ul_key_t;

    #define HASH_KEY_T    hash_ul_key_t
    #define HASH_ENTRY_T  hash_ul_entry_t
    #define HASH_T        hash_ul_t

    #define HASH_CLEAR    hash_ul_clear
    #define HASH_GET      hash_ul_get
    #define HASH_SET      hash_ul_set

    #include "hash_generic.h"

    #undef HASH_KEY_T
    #undef HASH_ENTRY_T
    #undef HASH_T

    #undef HASH_CLEAR
    #undef HASH_GET
    #undef HASH_SET

I'd really just rather do this, once:

    $ mkct.map --name=iimap --key-type=int --value-type=int

And write code like this:

    #include "iimap.h"
    
    ...
    
    int my_value;
    int my_key = 0xBEEF;
    if(iimap_get(my_key, &my_value)) {
      // booyah
      printf("the value of %d is %d\n", my_key, my_value);
    }

Of course, if I wasn't such a masochist I'd just use C++.

