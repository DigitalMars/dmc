DIRS =	client	\
	clock	\
	ddeinst	\
	ddemo	\
	ddeprog	\
	server

all: $(DIRS)

$(DIRS):
	cd $@
	@echo *** win32\ddeml\$@ *** >>$(MSTOOLS)\samples\ddeml.tmp
	@nmake -a -i -nologo >>$(MSTOOLS)\samples\ddeml.tmp
	cd ..
	
