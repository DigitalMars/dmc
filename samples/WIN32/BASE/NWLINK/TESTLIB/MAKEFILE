!include <ntwin32.MAK>

.c.obj:
   $(cc) $(cdebug) $(cflags) $(cvars) $<

proj = testlib
mylibs = wsock32.lib
myobjs = errmsg.obj dperror.obj dstrerr.obj geterror.obj \
		 cmdline.obj netprint.obj wperror.obj memory.obj
myinc = wsipx.h

all: $(proj).lib


#  Compile the C files

errmsg.obj:   $(*B).c 
dperror.obj:   $(*B).c 
wperror.obj:   $(*B).c 
dstrerr.obj:   $(*B).c 
geterror.obj:   $(*B).c
cmdline.obj:   $(*B).c 
netprint.obj:   $(*B).c
memory.obj:   $(*B).c 

#  Create the library

$(proj).lib: $(myobjs) $(SOCKLIB)
        $(implib) -OUT:$(proj).lib -SUBSYSTEM:windows $(myobjs)

# Clean up everything
clean :
    -del *.lib
    -del *.obj
