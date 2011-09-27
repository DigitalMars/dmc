#_ win32.mak
# Build win32 version of microemacs
# Needs Digital Mars C compiler to build, available free from:
# www.digitalmars.com

D=\dm
LIB=$D\lib
SNN=$D\lib\snn
INCLUDE=-I$D\include

TARGET=men

CFLAGS= -o $(INCLUDE) $(DEBUG)
LFLAGS=/map

.c.obj :
	sc -c $(CFLAGS) $*

OBJa= ansi.obj basic.obj buffer.obj display.obj file.obj fileio.obj line.obj
OBJb= random.obj region.obj search.obj spawn.obj tcap.obj termio.obj vt52.obj
OBJc= window.obj word.obj main.obj more.obj disprev.obj
OBJd= mouse.obj menu.obj memenu.obj capture.obj patchexe.obj win32.obj

ALLOBJS=$(OBJa) $(OBJb) $(OBJc) $(OBJd)

all: $(TARGET).exe

#################################################

$(TARGET).exe : $(ALLOBJS) $(TARGET).lnk $(TARGET).def
	$D\bin\link @$(TARGET).lnk

keypress.exe : keypress.c
	sc $(CFLAGS) keypress

########################################

$(TARGET).lnk : win32.mak
	echo $(OBJa)+				>  $*.lnk
	echo $(OBJb)+				>> $*.lnk
	echo $(OBJc)+				>> $*.lnk
	echo $(OBJd)$(LFLAGS)			>> $*.lnk
	echo $*					>> $*.lnk
	echo $*					>> $*.lnk
	echo $(SNN)+				>> $*.lnk
	echo $(LIB)\kernel32+			>> $*.lnk
	echo $(LIB)\user32			>> $*.lnk
	echo $*.def;				>> $*.lnk

########################################

$(TARGET).def : win32.mak
	echo NAME $(TARGET)			>  $*.def
	echo SUBSYSTEM CONSOLE			>> $*.def
	echo EXETYPE NT				>> $*.def
	echo CODE SHARED EXECUTE		>> $*.def
	echo STUB '$(LIB)\ntstub.exe'		>> $*.def

###### Source file dependencies ######

menu.obj : menu.h
capture.obj : capture.h
spawn.obj : capture.h

###################################

