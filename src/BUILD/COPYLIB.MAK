D = .

all:   create.dir $D\sccrt70.lib

create.dir:
	@-if not exist $D\*.* md $D

$D\sccrt70.lib : ..\..\redist32\sccrt70.lib
    -del $D\sccrt70.lib
    copy ..\..\redist32\sccrt70.lib $D


clean :
    -del $D\sccrt70.lib

