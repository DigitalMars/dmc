DIRLIST  =  idl     \
            engines \
            app

all: $(DIRLIST)

$(DIRLIST):
        cd $@
        @echo *** ole\mfract\$@ *** >>$(MSTOOLS)\samples\mfract.tmp
        @nmake -a -i -nologo >>$(MSTOOLS)\samples\mfract.tmp
        cd ..
