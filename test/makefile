

TEMPLATES_DIR = ../templates/

MKCT_LLIST  = ../mkct.llist
MKCT_QUEUE  = ../mkct.queue
MKCT_MAP    = ../mkct.map
MKCT_OBJMAP = ../mkct.objmap


OBJECTS += src/llist/llist_check.o
OBJECTS += src/llist/ilist.o

OBJECTS += src/queue/queue_check.o
OBJECTS += src/queue/iqueue.o

OBJECTS += src/map/map_check.o
OBJECTS += src/map/iimap.o

OBJECTS += src/objmap/objmap_check.o
OBJECTS += src/objmap/imap_obj.o
OBJECTS += src/objmap/obj.o

OBJECTS += src/check_all.o


test_all: $(OBJECTS)
	gcc -o $@ $^ -lcheck

.PHONY: ilist_src
ilist_src:
	$(MKCT_LLIST) --name=ilist --value-type=int
	mv ilist.h src/llist/ilist.h
	mv ilist.c src/llist/ilist.c

.PHONY: iqueue_src
iqueue_src:
	$(MKCT_QUEUE) --name=iqueue --value-type=int
	mv iqueue.h src/queue/iqueue.h
	mv iqueue.c src/queue/iqueue.c

.PHONY: iimap_src
iimap_src:
	$(MKCT_MAP) --name=iimap --key-type=int --value-type=int
	mv iimap.h src/map/iimap.h
	mv iimap.c src/map/iimap.c

.PHONY: imap_obj_src
imap_obj_src:
	$(MKCT_OBJMAP) --name=imap_obj --key-type=int --value-type=obj_t
	mv imap_obj.h src/objmap/imap_obj.h
	mv imap_obj.c src/objmap/imap_obj.c
	patch -d src/objmap/ < src/objmap/imap_obj.c.patch
	patch -d src/objmap/ < src/objmap/imap_obj.h.patch

src/llist/ilist.o: ilist_src
src/llist/llist_check.o: ilist_src

src/queue/iqueue.o: iqueue_src
src/queue/queue_check.o: iqueue_src

src/map/iimap.o: iimap_src
src/map/map_check.o: iimap_src

src/objmap/imap_obj.o: imap_obj_src
src/objmap/objmap_check.o: imap_obj_src

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm 'test_all'
	find -name '*.o' -delete
	find -name '*.rej' -delete
	find -name '*.orig' -delete

