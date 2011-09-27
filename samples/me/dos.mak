#_ dos.mak
# Build 16 bit DOS version of microemacs
# Needs Digital Mars C compiler to build, available free from:
# www.digitalmars.com

D=\dm
LIB=$D\lib
INCLUDE=

MODEL=C
TARGET=medos
CFLAGS= -o -m$(MODEL) $(INCLUDE) $(DEBUG)
LFLAGS=

.c.obj :
	sc -c $(CFLAGS) $*

OBJa= ansi.obj basic.obj buffer.obj display.obj file.obj fileio.obj line.obj
OBJb= random.obj region.obj search.obj spawn.obj tcap.obj termio.obj vt52.obj
OBJc= window.obj word.obj main.obj more.obj disprev.obj ibmpc.obj
OBJd= mouse.obj menu.obj memenu.obj capture.obj patchexe.obj

ALLOBJS=$(OBJa) $(OBJb) $(OBJc) $(OBJd)

all:	$(TARGET).exe

#################################################

$(TARGET).exe : $(ALLOBJS) $(TARGET).lnk
	link @$(TARGET).lnk

keypress.exe : keypress.c
	sc $(CFLAGS) keypress


########################################

$(TARGET).lnk : dos.mak
	echo $(OBJa)+ > $(TARGET).lnk
	echo $(OBJb)+ >> $(TARGET).lnk
	echo $(OBJc)+ >> $(TARGET).lnk
	echo $(OBJd) >> $(TARGET).lnk
	echo $(TARGET) >>$(TARGET).lnk
	echo $(TARGET) >>$(TARGET).lnk
	echo $(LIB)\sd$(MODEL)/map/noe; >>$(TARGET).lnk

###### Source file dependencies ######

menu.obj : menu.h
capture.obj : capture.h
spawn.obj : capture.h

######################################

