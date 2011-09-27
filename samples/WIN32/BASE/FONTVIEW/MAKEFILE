PROJ = FONTVIEW
SRC = FONTVIEW.C TOOLS.C STATUS.C DIALOGS.C DISPLAY.C

# Nmake macros for building Windows 32-Bit apps
!include <ntwin32.mak>

all: $(PROJ).exe

# Inference Rules:

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(cf) $<

.rc.res:
    $(rc) $(rcvars) -r $<

$(PROJ).hlp: $(PROJ).rtf $(PROJ).hpj
    $(hc) -n $(PROJ).hpj

$(PROJ).exe: $(SRC:.C=.OBJ) $(PROJ).res $(PROJ).def $(PROJ).hlp
    $(link) $(linkdebug) $(guiflags) -out:$(PROJ).exe $(SRC:.C=.OBJ) $(PROJ).res $(guilibs)
