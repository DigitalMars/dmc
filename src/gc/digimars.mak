# Makefile to build Hans Boehm garbage collector using the Digital Mars
# compiler from www.digitalmars.com
# Written by Walter Bright
# Needs compiler version 8.1b7 or later.
# To build gc.dll, gc.lib and gctest.exe, type:
#	make -f digimars.mak

DEFINES=-DNDEBUG -DSILENT -DGC_BUILD -D_WINDOWS -DGC_DLL -DALL_INTERIOR_POINTERS -D__STDC__ -DWIN32_THREADS
CFLAGS=-Iinclude $(DEFINES) -wx -o
LFLAGS=/ma/implib
CC=sc

.c.obj:
	$(CC) -c $(CFLAGS) $*

.cpp.obj:
	$(CC) -c $(CFLAGS) -Aa  $*

OBJS=	\
	allchblk.obj\
	alloc.obj\
	blacklst.obj\
	checksums.obj\
	dbg_mlc.obj\
	dyn_load.obj\
	finalize.obj\
	gc_cpp.obj\
	headers.obj\
	mach_dep.obj\
	malloc.obj\
	mallocx.obj\
	mark.obj\
	mark_rts.obj\
	misc.obj\
	new_hblk.obj\
	obj_map.obj\
	os_dep.obj\
	ptr_chck.obj\
	reclaim.obj\
	stubborn.obj\
	typd_mlc.obj\
	win32_threads.obj

targets: gc.dll gc.lib gctest.exe

gc.dll: $(OBJS) gc.def digimars.mak
	sc -ogc.dll $(OBJS) -L$(LFLAGS) gc.def 	kernel32.lib user32.lib

gc.def: digimars.mak
	echo LIBRARY GC >gc.def
	echo DESCRIPTION "Hans Boehm Garbage Collector" >>gc.def
	echo EXETYPE NT	>>gc.def

clean:
	del gc.def
	del $(OBJS)
	del gctest\Release\test.obj


gctest.exe : gc.lib gctest\Release\test.obj
	sc -g -ogctest.exe gctest\Release\test.obj gc.lib

gctest\Release\test.obj : tests\test.c
	-md gctest
	-md gctest\Release
	$(CC) -c -g -Ae -DNDEBUG -DWIN32 -D_WINDOWS \
	-DALL_INTERIOR_POINTERS -DWIN32_THREADS -DGC_DLL \
	-Iinclude tests\test.c -ogctest\Release\test.obj

allchblk.obj: allchblk.c
alloc.obj: alloc.c
blacklst.obj: blacklst.c
checksums.obj: checksums.c
dbg_mlc.obj: dbg_mlc.c
dyn_load.obj: dyn_load.c
finalize.obj: finalize.c
gc_cpp.obj: gc_cpp.cpp
headers.obj: headers.c
mach_dep.obj: mach_dep.c
malloc.obj: malloc.c
mallocx.obj: mallocx.c
mark.obj: mark.c
mark_rts.obj: mark_rts.c
misc.obj: misc.c
new_hblk.obj: new_hblk.c
obj_map.obj: obj_map.c
os_dep.obj: os_dep.c
ptr_chck.obj: ptr_chck.c
reclaim.obj: reclaim.c
stubborn.obj: stubborn.c
typd_mlc.obj: typd_mlc.c
win32_threads.obj: win32_threads.c
