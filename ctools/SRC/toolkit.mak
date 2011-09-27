# toolkit.mak   Thu Mar  1 1990   Modified by: Walter Bright */
# modified Sat May 22 1993 by Andrew Bushnell
# modified Tue Feb 19 2002 by Cesar Rabak 
# Makefile for all the auxillary tools in Ctools


CFLAGS= -DMSDOS -o+space -I..\include 

.c.obj:
	sc -ms -c $(CFLAGS) $*

FILE=filedire.obj filecopy.obj fileexis.obj filesett.obj \
	filetime.obj filesear.obj

all: clean archive.com update.com makedep.com chmod.com dump.com dumpobj.com \
     timer.com wc.com freq.exe um.com scmore.com


clean:
	del *.obj
	del *.com
	del freq.exe
        
archive.com : archive.obj mem.obj filespec.obj filedire.obj filecopy.obj \
	filesett.obj filetime.obj
    sc -mt archive.obj mem.obj filespec.obj filedire.obj filecopy.obj \
	        filesett.obj filetime.obj

update.com : update.obj mem.obj filespec.obj filedire.obj filecopy.obj \
	filesett.obj filetime.obj
	sc -mt update.obj mem.obj filespec.obj filedire.obj filecopy.obj \
	        filesett.obj filetime.obj

makedep.com : makedep.obj mem.obj list.obj filespec.obj filesear.obj
	sc -mt makedep.obj mem.obj list.obj filespec.obj filesear.obj \

chmod.com : chmod.c
       sc -mt -I..\include chmod.c

dump.com : dump.c
       sc -mt -I..\include dump.c

dumpobj.com : dumpobj.c
       sc -mt -I..\include dumpobj.c

timer.com : timer.c
       sc -mt -I..\include timer.c

wc.com   : wc.c
       sc -mt -I..\include wc.c

freq.exe : freq.c
      sc -ml -I..\include freq.c

um.com  : um.c
      sc -mt -I..\include um.c

scmore.com : scmore.c
      sc -mt -I..\include scmore.c
		       
menu.exe : ..\include\menu.h menu.c
	sc -mt -I..\include -DTEST menu

$(FILE) : ..\include\file.h file.c
	sc -ms -c $(CFLAGS) -DA$* file -o$*.obj
