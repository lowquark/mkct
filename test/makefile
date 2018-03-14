
BINDIR = ../bin/

MKCT_STACK = $(BINDIR)mkct.stack
MKCT_QUEUE = $(BINDIR)mkct.queue
MKCT_LIST  = $(BINDIR)mkct.list
MKCT_MAP   = $(BINDIR)mkct.map

MKCT_OBJSTACK = $(BINDIR)mkct.objstack
MKCT_OBJQUEUE = $(BINDIR)mkct.objqueue
MKCT_OBJLIST  = $(BINDIR)mkct.objlist
MKCT_OBJMAP   = $(BINDIR)mkct.objmap

OBJECTS += src/stack_check.o
OBJECTS += src/queue_check.o
OBJECTS += src/list_check.o
OBJECTS += src/map_check.o

OBJECTS += src/int_stack.o
OBJECTS += src/int_queue.o
OBJECTS += src/int_list.o
OBJECTS += src/int_int_map.o

OBJECTS += src/obj_stack.o
OBJECTS += src/obj_queue.o
OBJECTS += src/obj_list.o
OBJECTS += src/int_obj_map.o

OBJECTS += src/obj.o
OBJECTS += src/check_all.o

test_all: generate_sources $(OBJECTS)
	gcc -o $@ $(OBJECTS) -lcheck

../mkct.%:
	make -C .. $(notdir $@)

.PHONY: generate_sources
generate_sources:
	$(MKCT_STACK) --value-type=int --name=int_stack --header > src/int_stack.h
	$(MKCT_STACK) --value-type=int --name=int_stack --source > src/int_stack.c
	$(MKCT_QUEUE) --value-type=int --name=int_queue --header > src/int_queue.h
	$(MKCT_QUEUE) --value-type=int --name=int_queue --source > src/int_queue.c
	$(MKCT_LIST) --value-type=int --name=int_list --header > src/int_list.h
	$(MKCT_LIST) --value-type=int --name=int_list --source > src/int_list.c
	$(MKCT_MAP) --key-type=int --value-type=int --name=int_int_map --header > src/int_int_map.h
	$(MKCT_MAP) --key-type=int --value-type=int --name=int_int_map --source > src/int_int_map.c
	$(MKCT_OBJSTACK) --value-type=int --name=obj_stack --header > src/obj_stack.h
	$(MKCT_OBJSTACK) --value-type=int --name=obj_stack --source > src/obj_stack.c
	$(MKCT_OBJQUEUE) --value-type=int --name=obj_queue --header > src/obj_queue.h
	$(MKCT_OBJQUEUE) --value-type=int --name=obj_queue --source > src/obj_queue.c
	$(MKCT_OBJLIST) --value-type=int --name=obj_list --header > src/obj_list.h
	$(MKCT_OBJLIST) --value-type=int --name=obj_list --source > src/obj_list.c
	$(MKCT_OBJMAP) --key-type=int --value-type=int --name=int_obj_map --header > src/int_obj_map.h
	$(MKCT_OBJMAP) --key-type=int --value-type=int --name=int_obj_map --source > src/int_obj_map.c

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm 'test_all'
	find -name '*.o' -delete
	find -name '*.rej' -delete
	find -name '*.orig' -delete

