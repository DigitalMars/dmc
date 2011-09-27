!include <ntwin32.MAK>

proj = globchat
mylibs = wsock32.lib version.lib

all: $(proj).exe

# Update the resource if necessary
$(proj).res: $(proj).rc about.dlg globals.h
    rc -r -fo $(proj).res $(cvars) $(proj).rc


# Update the object files if necessary
$(proj).obj: $(proj).c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) $(proj).c

about.obj: about.c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) about.c

dispatch.obj: dispatch.c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) dispatch.c

init.obj: init.c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) init.c

misc.obj: misc.c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) misc.c

winmain.obj: winmain.c globals.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) winmain.c

# Since the link line has some severe differences depending on what
# platform we are running on, we need to special case this so that
# we execute the correct commands:

$(proj).exe: $(proj).obj about.obj dispatch.obj init.obj misc.obj winmain.obj $(proj).res
    $(link) $(linkdebug) $(guiflags) $(proj).obj about.obj dispatch.obj init.obj misc.obj winmain.obj $(guilibsmt) $(mylibs) $(proj).res -out:$(proj).exe


# Clean up everything
clean :
    -del *.res
    -del *.res
    -del *.exe
    -del *.obj
