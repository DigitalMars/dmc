DIRS =  dunion  \
        inout   \
        xmit    \
        repas

all: $(DIRS)

$(DIRS):
        cd $@
        echo *** rpc\data\$@ *** >>$(MSTOOLS)\samples\data.tmp
        nmake -a -i >>$(MSTOOLS)\samples\data.tmp
        cd ..

