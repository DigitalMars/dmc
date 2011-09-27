DIRS =	connect	\
	dgrecv	\
	dgsend	\
	listen	\
	ping	\
	testlib	

all: $(DIRS)

$(DIRS):
	cd $@
	echo *** $@ *** >>$(MSTOOLS)\samples\nwlink.tmp
	nmake -a -i -nologo >>$(MSTOOLS)\samples\nwlink.tmp
	cd ..
	
