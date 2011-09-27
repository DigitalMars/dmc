DIRS =  simpcntr \
        simpsvr


all: $(DIRS)

$(DIRS):
	cd $@
	echo *** frmwork\ole2\$@ *** >>$(MSTOOLS)\samples\fwk_ole.tmp
	nmake -a -i -nologo >>$(MSTOOLS)\samples\fwk_ole.tmp
	cd ..
	
