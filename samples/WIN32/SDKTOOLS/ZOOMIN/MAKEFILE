# some NT Win32 nmake macros
# for now this resides in samples\inc

!include <ntwin32.mak>

cvars=-DNT -DWIN -DWIN32
obj=obj
mycflag = -Fo.\$(obj)\\

all: zoomin.exe

# Update the object file if necessary

$(obj)\zoomin.obj: zoomin.c zoomin.h
    IF NOT EXIST $(obj) md obj
    $(cc) $(cdebug) $(cflags) $(mycflag) $(cvars) zoomin.c

# Update the resources if necessary

$(obj)\zoomin.res: zoomin.rc zoomin.h
    rc -r -fo .\$(obj)\zoomin.res zoomin.rc

# Update the executable file if necessary, and if so, add the resource back in.

zoomin.exe: $(obj)\zoomin.obj \
	     $(obj)\zoomin.res
    $(link) $(linkdebug) $(guiflags) -out:zoomin.exe	 \
    $(obj)\zoomin.obj \
    $(obj)\zoomin.res $(guilibs) shell32.lib
