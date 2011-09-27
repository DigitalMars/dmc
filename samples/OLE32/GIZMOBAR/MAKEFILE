#
# MAKEFILE
# GizmoBar Version 1.00, March 1993
#
#

!include <$(MSTOOLS)\samples\ole\include\olesampl.mak>

APP=gizmobar

all: $(APP).dll

OLE_FLAGS =

!ifndef NO_DEBUG
OLE_FLAGS = $(OLE_FLAGS) /D_DEBUG /DDEBUG /D_DEBUGTRACE=0
!endif

.SUFFIXES: .h .c .obj .exe .dll .cpp .res .rc

INCLS    = ..\include\$(APP).h gizmoint.h gizmo.h win1632.h

OBJS1    = $(APP).obj init.obj
OBJS2    = api.obj paint.obj gizmo.obj
OBJS     = $(OBJS1) $(OBJS2)


RCFILES1 = $(APP).rcv
RCFILES  = $(RCFILES1)


#
# Tool Directives
#
.c.obj:
    $(cc) $(cvars) $(cflags) $(cdebug) $(OLE_FLAGS) $*.c


.rc.res:
    rc -r -DWIN32 -DDEBUG $*.rc


clean:
    -del *.obj
    -del *.dll
    -del *.res
    -del *.lib
    -del *.map
    -del *.exp

$(APP).dll: $(OBJS) $(APP).lib $(APP).def $(APP).res
    $(link) $(linkdebug) $(dlllflags) $(APP).exp $(OBJS) $*.res -out:$@ -map:$*.map $(guilibsdll)  bttncur.lib
    if not exist ..\bin mkdir ..\bin
    copy $(APP).dll ..\bin

$(APP).lib: $(OBJS) $(APP).def
    $(implib) $(OBJS) -out:$(APP).lib -def:$(APP).def -machine:$(CPU)
    if not exist ..\lib mkdir ..\lib
    copy $(APP).lib ..\lib

##### Dependencies #####
$(APP).obj     : $(APP).c       $(INCLS)
paint.obj      : paint.c        $(INCLS)
init.obj       : init.c         $(INCLS)
api.obj        : api.c          $(INCLS)
gizmo.obj      : gizmo.c        $(INCLS)


$(APP).res  : $(APP).rc   $(INCLS) $(RCFILES)
