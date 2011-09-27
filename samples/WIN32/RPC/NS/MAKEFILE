DIRS =   nhello

all: $(DIRS)

$(DIRS):
        cd $@
        echo *** rpc\ns\$@ *** >>$(MSTOOLS)\samples\ns.tmp
        nmake -a -i >>$(MSTOOLS)\samples\ns.tmp
        cd ..

