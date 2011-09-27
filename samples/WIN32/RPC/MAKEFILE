DIRLIST =       callback        \
                        cluuid          \
                        data            \
                        dict            \
                        doctor          \
                        dynept          \
                        handles         \
                        hello           \
                        interop     \
                        mandel          \
                        ns                      \
                        object          \
                        pickle          \
                        rpcssm          \
                        rpcsvc          \
                        whello          \
                        yield


all: $(DIRLIST)




$(DIRLIST):
        cd $@
        echo *** rpc\$@ *** >>$(MSTOOLS)\samples\rpc.tmp
        nmake $(BUILD_OPT) -nologo -a -i >>$(MSTOOLS)\samples\rpc.tmp
        cd ..
