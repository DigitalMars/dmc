DIRS =	auxdemo	 \
	backtrce \
	gengl	 \
        pgonoffs \
	stonehng \
        vasphere

all: $(DIRS)

$(DIRS):
	cd $@
	@echo *** opengl\demos\$@ *** >>$(MSTOOLS)\samples\demos.tmp
	@nmake -a -i -nologo >>$(MSTOOLS)\samples\demos.tmp
	cd ..
	
