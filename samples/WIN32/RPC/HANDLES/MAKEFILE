DIRS =  auto    \
        cxhndl  \
        usrdef

all: $(DIRS)

$(DIRS):
        cd $@
        echo *** rpc\handles\$@ *** >>$(MSTOOLS)\samples\handles.tmp
        nmake -a -i >>$(MSTOOLS)\samples\handles.tmp
        cd ..

