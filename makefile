
all: bin/mkct.stack \
	   bin/mkct.queue \
		 bin/mkct.list  \
		 bin/mkct.map   \
     bin/mkct.objstack \
	   bin/mkct.objqueue \
		 bin/mkct.objlist  \
		 bin/mkct.objmap

bin/mkct.%: src/mkct.%.sh
	./template_sub.pl $< > $@
	chmod +x $@

