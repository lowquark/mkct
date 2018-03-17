
BINDIR = ../bin/

MKCT_STACK = $(BINDIR)mkct.stack
MKCT_QUEUE = $(BINDIR)mkct.queue
MKCT_LIST  = $(BINDIR)mkct.list
MKCT_MAP   = $(BINDIR)mkct.map

MKCT_OBJSTACK = $(BINDIR)mkct.objstack
MKCT_OBJQUEUE = $(BINDIR)mkct.objqueue
MKCT_OBJLIST  = $(BINDIR)mkct.objlist
MKCT_OBJMAP   = $(BINDIR)mkct.objmap

OBJECTS += src/stack/int_stack.o
OBJECTS += src/stack/obj_stack.o
OBJECTS += src/stack/stack_check.o
OBJECTS += src/stack/objstack_check.o

OBJECTS += src/queue/int_queue.o
OBJECTS += src/queue/obj_queue.o
OBJECTS += src/queue/queue_check.o
OBJECTS += src/queue/objqueue_check.o

OBJECTS += src/map/int_int_map.o
OBJECTS += src/map/int_obj_map.o
OBJECTS += src/map/map_check.o
OBJECTS += src/map/objmap_check.o

OBJECTS += src/list/int_list.o
OBJECTS += src/list/obj_list.o
OBJECTS += src/list/list_check.o

OBJECTS += src/obj.o
OBJECTS += src/check_all.o

GENERATED_SOURCES += src/stack/int_stack.h \
                     src/stack/int_stack.c \
                     src/stack/obj_stack.h \
                     src/stack/obj_stack.c \
                     src/queue/int_queue.h \
                     src/queue/int_queue.c \
                     src/queue/obj_queue.h \
                     src/queue/obj_queue.c \
                     src/list/int_list.h \
                     src/list/int_list.c \
                     src/list/obj_list.h \
                     src/list/obj_list.c \
                     src/map/int_int_map.h \
                     src/map/int_int_map.c \
                     src/map/int_obj_map.h \
                     src/map/int_obj_map.c

test_all: $(GENERATED_SOURCES) $(OBJECTS)
	gcc -o $@ $(OBJECTS) -lcheck

../mkct.%:
	make -C .. $(notdir $@)

#### stack ####
src/stack/int_stack.h:
	$(MKCT_STACK) --value-type=int --name=int_stack --header > src/stack/int_stack.h
src/stack/int_stack.c:
	$(MKCT_STACK) --value-type=int --name=int_stack --source > src/stack/int_stack.c
src/stack/obj_stack.h:
	$(MKCT_OBJSTACK) --object-type=obj_t --name=obj_stack --header > src/stack/obj_stack.h
	patch -d src/stack/ < $@.patch
src/stack/obj_stack.c:
	$(MKCT_OBJSTACK) --object-type=obj_t --name=obj_stack --source > src/stack/obj_stack.c
	patch -d src/stack/ < $@.patch

#### queue ####
src/queue/int_queue.h:
	$(MKCT_QUEUE) --value-type=int --name=int_queue --header > src/queue/int_queue.h
src/queue/int_queue.c:
	$(MKCT_QUEUE) --value-type=int --name=int_queue --source > src/queue/int_queue.c
src/queue/obj_queue.h:
	$(MKCT_OBJQUEUE) --object-type=obj_t --name=obj_queue --header > src/queue/obj_queue.h
	patch -d src/queue/ < $@.patch
src/queue/obj_queue.c:
	$(MKCT_OBJQUEUE) --object-type=obj_t --name=obj_queue --source > src/queue/obj_queue.c
	patch -d src/queue/ < $@.patch

#### list ####
src/list/int_list.h:
	$(MKCT_LIST) --value-type=int --name=int_list --header > src/list/int_list.h
src/list/int_list.c:
	$(MKCT_LIST) --value-type=int --name=int_list --source > src/list/int_list.c
src/list/obj_list.h:
	$(MKCT_OBJLIST) --value-type=int --name=obj_list --header > src/list/obj_list.h
src/list/obj_list.c:
	$(MKCT_OBJLIST) --value-type=int --name=obj_list --source > src/list/obj_list.c

#### map ####
src/map/int_int_map.h:
	$(MKCT_MAP) --key-type=int --value-type=int --name=int_int_map --header > $@
src/map/int_int_map.c:
	$(MKCT_MAP) --key-type=int --value-type=int --name=int_int_map --source > $@
src/map/int_obj_map.h: src/map/int_obj_map.h.patch
	$(MKCT_OBJMAP) --key-type=int --object-type=obj_t --name=int_obj_map --header > $@
	patch -d src/map/ < $@.patch
src/map/int_obj_map.c: src/map/int_obj_map.c.patch
	$(MKCT_OBJMAP) --key-type=int --object-type=obj_t --name=int_obj_map --source > $@
	patch -d src/map/ < $@.patch

%.o: %.c
	gcc -g -Wall -Wpedantic -c -o $@ $< -Isrc/

.PHONY: clean
clean:
	rm 'test_all' $(GENERATED_SOURCES)
	find -name '*.o' -delete
	find -name '*.rej' -delete
	find -name '*.orig' -delete

