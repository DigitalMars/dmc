DIRLIST =	book	\
		demos	\
		glaux	\
		scrsave \
		samples

all: $(DIRLIST) jtext

$(DIRLIST):
	cd $@
	@echo *** opengl\$@ *** >>$(MSTOOLS)\samples\opengl.tmp
	@nmake -a -i -nologo >>$(MSTOOLS)\samples\opengl.tmp
	cd ..
	
jtext:
!IF EXIST (jtext)
    cd jtext
    @echo *** opengl\$@ *** >>$(MSTOOLS)\samples\opengl.tmp
    @nmake -a -i -nologo >>$(MSTOOLS)\samples\opengl.tmp
    cd ..
!ENDIF
