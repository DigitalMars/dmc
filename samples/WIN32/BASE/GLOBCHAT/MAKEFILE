DIRS =	client	\
	server

all: $(DIRS)

$(DIRS):
	cd $@
	@echo *** win32\globchat\$@ *** >>$(MSTOOLS)\samples\globchat.tmp
	@nmake -a -i -nologo >>$(MSTOOLS)\samples\globchat.tmp
	cd ..
	
