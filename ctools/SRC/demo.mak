# modified Tue Feb 19 2002 by Cesar Rabak 
# Makefile for demo programs.  



all :   clean tsrclock.exe resdemo.exe listdemo.exe which.exe
	
tsrclock.exe : tsrclock.c
	sc -ms -I..\include tsrclock.c

resdemo.exe : resdemo.c
	sc -ms -I..\include resdemo.c

listdemo.exe : listdemo.c list.c mem.c
	sc -ms -I..\include listdemo.c list.c mem.c

which.exe : which.c filespec.c file.c mem.c
	sc -ms -I..\include -DMSDOS -DAfilesear -o+space which.c filespec.c file.c mem.c

clean :
 	del *.obj
	del tsrclock.exe
	del resdemo.exe
	del which.exe
	del listdemo.exe
	del menu.exe
	
