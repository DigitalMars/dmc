D = .

all:   create.dir $D\sccrt70d.lib

create.dir:
	@-if not exist $D\*.* md $D

$D\sccrt70d.lib : ..\..\redist32\sccrt70d.lib
    -del $D\sccrt70d.lib
    copy ..\..\redist32\sccrt70d.lib $D


clean :
    -del $D\sccrt70d.lib

