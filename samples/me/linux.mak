#_ linux.mak

#FLGS=-g -DDEBUG
FLGS=-O
#LDFLG=
LDFLG=-s
CC=gcc

CFLAGS = -w $(FLGS) -DLINUX -DTERMCAP

SRC= ansi.c basic.c buffer.c display.c file.c fileio.c line.c main.c \
	random.c region.c search.c spawn.c tcap.c termio.c vt52.c \
	window.c word.c more.c mouse.c

OBJ= ansi.o basic.o buffer.o display.o file.o fileio.o line.o main.o \
	random.o region.o search.o spawn.o tcap.o termio.o vt52.o \
	window.o word.o more.o mouse.o

SOURCE= ed.h $(SRC) linux.mak me.html disprev.c

melinux : $(OBJ) linux.mak
	cc $(CFLAGS) -c disprev.c
	cc $(LDFLG) $(FLGS) -o melinux $(OBJ) disprev.o -ltermcap

tags: $(SRC)
	ctags $(SRC)

me.zip : $(SOURCE)
	zip me $(SOURCE)

keypress : keypress.c
	$(CC) $(CFLAGS) keypress.c -o keypress





