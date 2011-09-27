DIRLIST =       common  \
                3dfo    \
		flwbox	\
		maze  	\
		pipes  \
                text3d

all: $(DIRLIST)

$(DIRLIST):
	cd $@
	@echo *** opengl\scrsave\$@ *** >>$(MSTOOLS)\samples\scrsave.tmp
	@nmake -a -i -nologo >>$(MSTOOLS)\samples\scrsave.tmp
	cd ..
	
