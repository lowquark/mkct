
all: mkct.queue mkct.llist mkct.map mkct.objmap

mkct.queue: src/mkct.queue.sh src/template/queue.h src/template/queue.c
	# copy template
	cp src/mkct.queue.sh $@
	# substitute sources for silly #include statements
	sed -i -e '/#include "queue.h"/ r src/template/queue.h' -e '/#include "queue.h"/d' $@
	sed -i -e '/#include "queue.c"/ r src/template/queue.c' -e '/#include "queue.c"/d' $@

mkct.llist: src/mkct.llist.sh src/template/llist.h src/template/llist.c
	# copy template
	cp src/mkct.llist.sh $@
	# substitute sources for silly #include statements
	sed -i -e '/#include "llist.h"/ r src/template/llist.h' -e '/#include "llist.h"/d' $@
	sed -i -e '/#include "llist.c"/ r src/template/llist.c' -e '/#include "llist.c"/d' $@

mkct.map: src/mkct.map.sh src/template/map.h src/template/map.c
	# copy template
	cp src/mkct.map.sh $@
	# substitute sources for silly #include statements
	sed -i -e '/#include "map.h"/ r src/template/map.h' -e '/#include "map.h"/d' $@
	sed -i -e '/#include "map.c"/ r src/template/map.c' -e '/#include "map.c"/d' $@

mkct.objmap: src/mkct.objmap.sh src/template/objmap.h src/template/objmap.c
	# copy template
	cp src/mkct.objmap.sh $@
	# substitute sources for silly #include statements
	sed -i -e '/#include "objmap.h"/ r src/template/objmap.h' -e '/#include "objmap.h"/d' $@
	sed -i -e '/#include "objmap.c"/ r src/template/objmap.c' -e '/#include "objmap.c"/d' $@

