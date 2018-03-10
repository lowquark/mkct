
# mkct - C Template Generators

A collection of bash scripts that generate a container for one of your types --
like a C++ template but without the C++.

## `mkct.llist`

Generates a circular linked list for some value type.

    $ mkct.llist --name=ilist --value-type=int

`ilist.h` and `ilist.c` will be created. See `ilist.h` for usage.

## `mkct.queue`

Generates a queue for some value type.

    $ mkct.queue --name=iqueue --value-type=int

`iqueue.h` and `iqueue.c` will be created. See `iqueue.h` for usage.

## `mkct.map`

Generates a key-value mapping for some key type and some value type.

    $ mkct.map --name=iimap --key-type=int --value-type=int

`iimap.h` and `iimap.c` will be created. See `iimap.h` for usage. It's up to
you to implement the hash function in `iimap.c`.

## `mkct.objmap`

Like `mkct.map`, but "owns" the objects that it contains. Calls an init and
deinit on its values, and returns a NULL pointer for non-existent objects.

    $ mkct.map --name=iobjmap --key-type=int --value-type=my_object_t

`iobjmap.h` and `iobjmap.c` will be created. See `iobjmap.h` for usage. It's up to
you to implement the init, deinit, and hash function in `iobjmap.c`.

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

