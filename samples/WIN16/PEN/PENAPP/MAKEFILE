##### Module Macro #####
NAME  = penapp
SRCS  = $(NAME).c
OBJS  =

##### C7 Macro #####
C7    = 1

##### Library Macro #####
LIBS  = libw slibcew penwin
MOD   = -AS

##### Include Macro #####
INCLS = main.h

##### Resource Macro #####
RCFILES  = $(NAME).rc

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
CC    = cl -nologo -c $(MOD) -G2 -Zp -W3 $(CLOPT) $(DDEF) $(DEF)
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
goal : $(NAME).exe

##### Dependents For Goal and Command Line #####
$(NAME).exe : $(SRCS:.c=.obj) $(NAME).def $(NAME).res
   $(LINK) @<<$(NAME).lnk
   $(SRCS:.c=.obj) $(OBJS),
   $(NAME).exe,
   $(NAME).map,
   $(LIBS),
   $(NAME).def
<<$(KEEPFLAG)
   $(RC) -T $(NAME).res
!if $(DEBUG)
!if !$(C7)
   cvpack -p $(NAME).exe
!endif
   mapsym $(NAME).map
!endif

##### Dependents #####
$(SRCS:.c=.obj) : $(INCLS)

$(NAME).res : $(INCLS) $(RCFILES)

##### Clean Directory #####
clean:
   -del *.obj
   -del *.res
   -del *.exe
   -del *.map
   -del *.sym
   -del *.lnk
   -del *.pdb
