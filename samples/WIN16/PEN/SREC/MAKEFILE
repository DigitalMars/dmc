##### Module Macro #####
NAME  = srec
SRCS  = $(NAME).c
OBJS  =

##### C7 Macro #####
C7    = 1

##### Library Macro #####
LIBS  = libw mdllcew penwin mlibcew
MOD   = -AMw

##### Include Macro #####
INCLS = ..\penapp\main.h

##### Resource Macro #####
RCFILES  = $(NAME).dll

##### DEBUG Defined #####
DEBUG = 1

##### Build Option Macros #####
!if $(DEBUG)
DDEF  = -DDEBUG
CLOPT = -Zi -Od
MOPT  = -Zi
LOPT  = /CO /LI /MAP
!else
DDEF  =
CLOPT = -Os
LOPT  =
!endif

##### General Macros #####
KEEPFLAG = KEEP
DEF   =

##### Tool Macros #####
ASM   = masm -Mx $(MOPT) $(DDEF) $(DEF)

## point to the penapp sample to locate share header files
CC    = cl -nologo -c -I ..\penapp $(MOD) -G2 -Zp -W3 $(CLOPT) $(DDEF) $(DEF)

LINK  = link /NOD /NOE $(LOPT)
RC    = rc $(DDEF) $(DEF)
HC    = hc

##### Inference Rules #####
.c.obj:
   $(CC) $<

.asm.obj:
   $(ASM) $<;

.rc.res:
   $(RC) -r $<

##### Main (default) Target #####
goal : $(NAME).dll

##### Dependents For Goal and Command Line #####
$(NAME).dll : $(SRCS:.c=.obj) $(NAME).def
   $(LINK) @<<$(NAME).lnk
   $(SRCS:.c=.obj) $(OBJS),
   $(NAME).dll,
   $(NAME).map,
   $(LIBS),
   $(NAME).def
<<$(KEEPFLAG)
   $(RC) -T $(NAME).dll
!if $(DEBUG)
!if !$(C7)
   cvpack -p $(NAME).dll
!endif
   mapsym $(NAME).map
!endif

##### Dependents #####
$(SRCS:.c=.obj) : $(INCLS)

$(NAME).res : $(INCLS) $(RCFILES)

##### Clean Directory #####
clean:
   -del *.obj
   -del *.dll
   -del *.map
   -del *.sym
   -del *.lnk
   -del *.pdb
