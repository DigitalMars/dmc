#_ makefile
# Needs Digital Mars C compiler to build, available free from:
# www.digitalmars.com


target:
	echo make men, mex, medos

all:
	make clean
	make -f win32.mak
	make clean
	make -f dos32.mak
	make clean
	make -f dos.mak
	make clean

men:
	make -f win32.mak

mex:
	make -f dos32.mak

medos:
	make -f dos.mak

clean:
	del *.obj
	del *.lnk
	del *.sym
	del *.def
#	del *.bak
#	del test.*
#	del *.exe
	del *.map
	del *.tmp
	del *.lst
	del *.exp
	del *.dbg

########################################

# and the dependencies:

SRC1= ansi.c basic.c bsdunix.c buffer.c display.c file.c fileio.c line.c main.c
SRC2= random.c region.c search.c spawn.c tcap.c termio.c vt52.c
SRC3= window.c word.c more.c disprev.c ibmpc.c mouse.c
SRC4= memenu.c menu.c capture.c patchexe.c win32.c keypress.c
SRC5= ed.h capture.h menu.h

SRC= $(SRC1) $(SRC2) $(SRC3) $(SRC4)
MAK= linux.mak win32.mak dos.mak dos32.mak makefile
DOC= me.html
EXE= melinux men.exe mex.exe medos.exe

######################################

zip :
	zip32 -u me $(SRC1)
	zip32 -u me $(SRC2)
	zip32 -u me $(SRC3)
	zip32 -u me $(SRC4)
	zip32 -u me $(SRC5)
	zip32 -u me $(MAK)
	zip32 -u me $(DOC)
	zip32 -u me $(EXE)


