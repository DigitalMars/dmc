ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= book
APPTYPE		= WIN32 CONSOLE
PROJTYPE	= EXE

CC		= SC
CPP		= SPP
JAVAC		= sj
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= 

DEFFILE		= book.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -C -WA -S -3 -a8 -c -g 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= -D_CONSOLE
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WA -3 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= -D_CONSOLE
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= 

RCFILES		= 

RESFILES	= 

HELPFILES	= 

PROJS		= accnot.prj accpersp.prj accum.prj  \
		aim.prj alpha.prj alpha3d.prj anti.prj  \
		antindx.prj antipndx.prj antipoly.prj bezcurve.prj  \
		bezmesh.prj bezsurf.prj checker.prj checker2.prj  \
		chess.prj clip.prj colormat.prj cone.prj  \
		cube.prj curve.prj depthcue.prj disk.prj  \
		dof.prj dofnot.prj double.prj drawf.prj  \
		feedback.prj fog.prj fogindex.prj font.prj  \
		light.prj linelist.prj lines.prj list.prj  \
		list2.prj maplight.prj material.prj mipmap.prj  \
		model.prj movelght.prj nurbs.prj pickdpth.prj  \
		pickline.prj picksqr.prj plane.prj planet.prj  \
		planetup.prj polys.prj robot.prj scclrlt.prj  \
		scene.prj scenebmb.prj sceneflt.prj select.prj  \
		smooth.prj sphere.prj stencil.prj stroke.prj  \
		surface.prj tea.prj teaamb.prj teapots.prj  \
		texgen.prj trim.prj accanti.prj antipnt.prj  \
		texsurf.prj simple.prj 

BATS		= 

PRECOMPILE	=  accnot?Pprj  accpersp?Pprj  accum?Pprj  \
		 aim?Pprj  alpha?Pprj  alpha3d?Pprj  anti?Pprj  \
		 antindx?Pprj  antipndx?Pprj  antipoly?Pprj  bezcurve?Pprj  \
		 bezmesh?Pprj  bezsurf?Pprj  checker?Pprj  checker2?Pprj  \
		 chess?Pprj  clip?Pprj  colormat?Pprj  cone?Pprj  \
		 cube?Pprj  curve?Pprj  depthcue?Pprj  disk?Pprj  \
		 dof?Pprj  dofnot?Pprj  double?Pprj  drawf?Pprj  \
		 feedback?Pprj  fog?Pprj  fogindex?Pprj  font?Pprj  \
		 light?Pprj  linelist?Pprj  lines?Pprj  list?Pprj  \
		 list2?Pprj  maplight?Pprj  material?Pprj  mipmap?Pprj  \
		 model?Pprj  movelght?Pprj  nurbs?Pprj  pickdpth?Pprj  \
		 pickline?Pprj  picksqr?Pprj  plane?Pprj  planet?Pprj  \
		 planetup?Pprj  polys?Pprj  robot?Pprj  scclrlt?Pprj  \
		 scene?Pprj  scenebmb?Pprj  sceneflt?Pprj  select?Pprj  \
		 smooth?Pprj  sphere?Pprj  stencil?Pprj  stroke?Pprj  \
		 surface?Pprj  tea?Pprj  teaamb?Pprj  teapots?Pprj  \
		 texgen?Pprj  trim?Pprj  accanti?Pprj  antipnt?Pprj  \
		 texsurf?Pprj  simple?Pprj 

POSTCOMPILE	= 

POSTLINK	= 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 

.C.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.CXX.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cxx

.CC.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cc

.CP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hxx

.C.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)   $*.c   -o$*.lst

.CPP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o$*.lst

.CXX.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cxx -o$*.lst

.CP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cp  -o$*.lst

.CC.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cc  -o$*.lst

.ASM.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

.OBJ.COD:
	$(DISASM) $*.OBJ -c

!IF $(DEBUG)
.OBJ.EXE:
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK

$$SCW$$.EXE
NUL
advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
book.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<

$$SCW$$.EXE
NUL
advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
book.def;
<<
!ENDIF

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

.RC.RES: 
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $*.rc -o$*.res

.DLG.RES:
	echo ^#include "windows.h" >$$$*.rc
	echo ^IF EXIST "$*.h" >>$$$*.rc
	echo ^#include "$*.h" >>$$$*.rc
	echo ^#include "$*.dlg" >>$$$*.rc
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $$$*.rc
	-del $*.res
	-ren $$$*.res $*.res



all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

noteout:
	REM Output to $(OUTPUTDIR)

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(OUTPUTDIR)\SCPH.SYM
		-del book.dpd
		$(MAKE) -faccnot.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -faccpersp.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -faccum.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -faim.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -falpha.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -falpha3d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fanti.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fantindx.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fantipndx.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fantipoly.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fbezcurve.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fbezmesh.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fbezsurf.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fchecker.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fchecker2.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fchess.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fclip.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fcolormat.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fcone.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fcube.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fcurve.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdepthcue.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdisk.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdof.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdofnot.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdouble.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fdrawf.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ffeedback.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ffog.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ffogindex.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ffont.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -flight.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -flinelist.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -flines.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -flist.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -flist2.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fmaplight.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fmaterial.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fmipmap.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fmodel.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fmovelght.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fnurbs.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fpickdpth.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fpickline.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fpicksqr.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fplane.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fplanet.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fplanetup.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fpolys.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -frobot.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fscclrlt.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fscene.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fscenebmb.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsceneflt.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fselect.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsmooth.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsphere.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fstencil.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fstroke.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsurface.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftea.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fteaamb.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fteapots.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftexgen.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftrim.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -faccanti.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fantipnt.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftexsurf.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsimple.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:


accnot?Pprj:
		$(MAKE) -faccnot.mak "SUB_DEBUG=$(DEBUG)"

accpersp?Pprj:
		$(MAKE) -faccpersp.mak "SUB_DEBUG=$(DEBUG)"

accum?Pprj:
		$(MAKE) -faccum.mak "SUB_DEBUG=$(DEBUG)"

aim?Pprj:
		$(MAKE) -faim.mak "SUB_DEBUG=$(DEBUG)"

alpha?Pprj:
		$(MAKE) -falpha.mak "SUB_DEBUG=$(DEBUG)"

alpha3d?Pprj:
		$(MAKE) -falpha3d.mak "SUB_DEBUG=$(DEBUG)"

anti?Pprj:
		$(MAKE) -fanti.mak "SUB_DEBUG=$(DEBUG)"

antindx?Pprj:
		$(MAKE) -fantindx.mak "SUB_DEBUG=$(DEBUG)"

antipndx?Pprj:
		$(MAKE) -fantipndx.mak "SUB_DEBUG=$(DEBUG)"

antipoly?Pprj:
		$(MAKE) -fantipoly.mak "SUB_DEBUG=$(DEBUG)"

bezcurve?Pprj:
		$(MAKE) -fbezcurve.mak "SUB_DEBUG=$(DEBUG)"

bezmesh?Pprj:
		$(MAKE) -fbezmesh.mak "SUB_DEBUG=$(DEBUG)"

bezsurf?Pprj:
		$(MAKE) -fbezsurf.mak "SUB_DEBUG=$(DEBUG)"

checker?Pprj:
		$(MAKE) -fchecker.mak "SUB_DEBUG=$(DEBUG)"

checker2?Pprj:
		$(MAKE) -fchecker2.mak "SUB_DEBUG=$(DEBUG)"

chess?Pprj:
		$(MAKE) -fchess.mak "SUB_DEBUG=$(DEBUG)"

clip?Pprj:
		$(MAKE) -fclip.mak "SUB_DEBUG=$(DEBUG)"

colormat?Pprj:
		$(MAKE) -fcolormat.mak "SUB_DEBUG=$(DEBUG)"

cone?Pprj:
		$(MAKE) -fcone.mak "SUB_DEBUG=$(DEBUG)"

cube?Pprj:
		$(MAKE) -fcube.mak "SUB_DEBUG=$(DEBUG)"

curve?Pprj:
		$(MAKE) -fcurve.mak "SUB_DEBUG=$(DEBUG)"

depthcue?Pprj:
		$(MAKE) -fdepthcue.mak "SUB_DEBUG=$(DEBUG)"

disk?Pprj:
		$(MAKE) -fdisk.mak "SUB_DEBUG=$(DEBUG)"

dof?Pprj:
		$(MAKE) -fdof.mak "SUB_DEBUG=$(DEBUG)"

dofnot?Pprj:
		$(MAKE) -fdofnot.mak "SUB_DEBUG=$(DEBUG)"

double?Pprj:
		$(MAKE) -fdouble.mak "SUB_DEBUG=$(DEBUG)"

drawf?Pprj:
		$(MAKE) -fdrawf.mak "SUB_DEBUG=$(DEBUG)"

feedback?Pprj:
		$(MAKE) -ffeedback.mak "SUB_DEBUG=$(DEBUG)"

fog?Pprj:
		$(MAKE) -ffog.mak "SUB_DEBUG=$(DEBUG)"

fogindex?Pprj:
		$(MAKE) -ffogindex.mak "SUB_DEBUG=$(DEBUG)"

font?Pprj:
		$(MAKE) -ffont.mak "SUB_DEBUG=$(DEBUG)"

light?Pprj:
		$(MAKE) -flight.mak "SUB_DEBUG=$(DEBUG)"

linelist?Pprj:
		$(MAKE) -flinelist.mak "SUB_DEBUG=$(DEBUG)"

lines?Pprj:
		$(MAKE) -flines.mak "SUB_DEBUG=$(DEBUG)"

list?Pprj:
		$(MAKE) -flist.mak "SUB_DEBUG=$(DEBUG)"

list2?Pprj:
		$(MAKE) -flist2.mak "SUB_DEBUG=$(DEBUG)"

maplight?Pprj:
		$(MAKE) -fmaplight.mak "SUB_DEBUG=$(DEBUG)"

material?Pprj:
		$(MAKE) -fmaterial.mak "SUB_DEBUG=$(DEBUG)"

mipmap?Pprj:
		$(MAKE) -fmipmap.mak "SUB_DEBUG=$(DEBUG)"

model?Pprj:
		$(MAKE) -fmodel.mak "SUB_DEBUG=$(DEBUG)"

movelght?Pprj:
		$(MAKE) -fmovelght.mak "SUB_DEBUG=$(DEBUG)"

nurbs?Pprj:
		$(MAKE) -fnurbs.mak "SUB_DEBUG=$(DEBUG)"

pickdpth?Pprj:
		$(MAKE) -fpickdpth.mak "SUB_DEBUG=$(DEBUG)"

pickline?Pprj:
		$(MAKE) -fpickline.mak "SUB_DEBUG=$(DEBUG)"

picksqr?Pprj:
		$(MAKE) -fpicksqr.mak "SUB_DEBUG=$(DEBUG)"

plane?Pprj:
		$(MAKE) -fplane.mak "SUB_DEBUG=$(DEBUG)"

planet?Pprj:
		$(MAKE) -fplanet.mak "SUB_DEBUG=$(DEBUG)"

planetup?Pprj:
		$(MAKE) -fplanetup.mak "SUB_DEBUG=$(DEBUG)"

polys?Pprj:
		$(MAKE) -fpolys.mak "SUB_DEBUG=$(DEBUG)"

robot?Pprj:
		$(MAKE) -frobot.mak "SUB_DEBUG=$(DEBUG)"

scclrlt?Pprj:
		$(MAKE) -fscclrlt.mak "SUB_DEBUG=$(DEBUG)"

scene?Pprj:
		$(MAKE) -fscene.mak "SUB_DEBUG=$(DEBUG)"

scenebmb?Pprj:
		$(MAKE) -fscenebmb.mak "SUB_DEBUG=$(DEBUG)"

sceneflt?Pprj:
		$(MAKE) -fsceneflt.mak "SUB_DEBUG=$(DEBUG)"

select?Pprj:
		$(MAKE) -fselect.mak "SUB_DEBUG=$(DEBUG)"

smooth?Pprj:
		$(MAKE) -fsmooth.mak "SUB_DEBUG=$(DEBUG)"

sphere?Pprj:
		$(MAKE) -fsphere.mak "SUB_DEBUG=$(DEBUG)"

stencil?Pprj:
		$(MAKE) -fstencil.mak "SUB_DEBUG=$(DEBUG)"

stroke?Pprj:
		$(MAKE) -fstroke.mak "SUB_DEBUG=$(DEBUG)"

surface?Pprj:
		$(MAKE) -fsurface.mak "SUB_DEBUG=$(DEBUG)"

tea?Pprj:
		$(MAKE) -ftea.mak "SUB_DEBUG=$(DEBUG)"

teaamb?Pprj:
		$(MAKE) -fteaamb.mak "SUB_DEBUG=$(DEBUG)"

teapots?Pprj:
		$(MAKE) -fteapots.mak "SUB_DEBUG=$(DEBUG)"

texgen?Pprj:
		$(MAKE) -ftexgen.mak "SUB_DEBUG=$(DEBUG)"

trim?Pprj:
		$(MAKE) -ftrim.mak "SUB_DEBUG=$(DEBUG)"

accanti?Pprj:
		$(MAKE) -faccanti.mak "SUB_DEBUG=$(DEBUG)"

antipnt?Pprj:
		$(MAKE) -fantipnt.mak "SUB_DEBUG=$(DEBUG)"

texsurf?Pprj:
		$(MAKE) -ftexsurf.mak "SUB_DEBUG=$(DEBUG)"

simple?Pprj:
		$(MAKE) -fsimple.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (book.dpd)
!INCLUDE book.dpd
!ENDIF


