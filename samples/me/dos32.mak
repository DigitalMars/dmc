#_ dos32.mak
# Build DOS32 version of microemacs
# Needs Digital Mars C compiler from:
# www.digitalmars.com
# Needs 32 bit DOS extender from:
# www.dosextender.com

D=\dm
LIB=$D\lib
INCLUDE=

MODEL=X
TARGET=mex
CFLAGS= -o -m$(MODEL) $(INCLUDE) $(DEBUG)
LFLAGS=/map

.c.obj :
	sc -c $(CFLAGS) $*

OBJa= ansi.obj basic.obj buffer.obj display.obj file.obj fileio.obj line.obj
OBJb= random.obj region.obj search.obj spawn.obj tcap.obj termio.obj vt52.obj
OBJc= window.obj word.obj main.obj more.obj disprev.obj ibmpc.obj
OBJd= mouse.obj menu.obj memenu.obj capture.obj patchexe.obj

ALLOBJS=$(OBJa) $(OBJb) $(OBJc) $(OBJd)

all: $(TARGET).exe

#################################################

$(TARGET).exe : $(ALLOBJS) $(TARGET).lnk
	link cx.obj+@$*.lnk

keypress.exe : keypress.c
	sc $(CFLAGS) keypress x32v.lib

########################################

$(TARGET).lnk : dos32.mak
	echo $(OBJa)+				>  $*.lnk
	echo $(OBJb)+				>> $*.lnk
	echo $(OBJc)+				>> $*.lnk
	echo $(OBJd)				>> $*.lnk
	echo $*					>> $*.lnk
	echo $*					>> $*.lnk
	echo x32v$(LFLAGS);			>> $*.lnk

###### Source file dependencies ######

menu.obj : menu.h
capture.obj : capture.h
spawn.obj : capture.h

###################################

