DIRS =  picklp  \
        picklt

all: $(DIRS)

$(DIRS):
        cd $@
        echo *** rpc\pickle\$@ *** >>$(MSTOOLS)\samples\pickle.tmp
        nmake -a -i >>$(MSTOOLS)\samples\pickle.tmp
        cd ..

