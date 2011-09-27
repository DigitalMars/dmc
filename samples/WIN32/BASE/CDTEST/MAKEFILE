# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: cdtest.exe

OBJS=cdtest.obj open.obj save.obj print.obj font.obj colors.obj find.obj replace.obj title.obj

OTHERCLOPTS=-DWIN32_LEAN_AND_MEAN -nologo
OTHERRCOPTS=-DWIN32_LEAN_AND_MEAN



# Update the resource if necessary

cdtest.res: res.rc cdtest.h
    $(rc) -r $(OTHERRCOPTS) -fo cdtest.res res.rc



# Update the object file if necessary

cdtest.obj: cdtest.c cdtest.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) cdtest.c

open.obj: open.c open.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) open.c

save.obj: save.c save.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) save.c

print.obj: print.c print.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) print.c

font.obj: font.c font.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) font.c

colors.obj: colors.c colors.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) colors.c

find.obj: find.c find.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) find.c

replace.obj: replace.c replace.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) replace.c

title.obj: title.c title.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) title.c



# Update the executable file if necessary, and if so, add the resource back in.

cdtest.exe:  $(OBJS) cdtest.res
    $(link) $(linkdebug) $(guiflags) -out:cdtest.exe $(OBJS) cdtest.res $(guilibs)
