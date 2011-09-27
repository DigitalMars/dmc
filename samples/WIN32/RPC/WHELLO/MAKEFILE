#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                    whello Application                   **#
#**            Copyright(c) Microsoft Corp. 1991-1995       **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

.c.obj:
   $(cc) $(cdebug) $(cflags) $(cvars) $<

all: whellos whelloc

# Update the resource if necessary
whello.rbj: whello.rc whello.h rpc.ico whello.dlg
    rc -r whello.rc
    cvtres -$(CPU) whello.res -o whello.rbj

# Update the executable file if necessary, and if so, add the resource back in.
whelloc : whelloc.exe
whelloc.exe : whelloc.obj whello.def whello_c.obj whello.rbj
    $(link) $(linkdebug) $(guiflags) -out:whelloc.exe -map:whelloc.map \
      whelloc.obj whello_c.obj whello.rbj \
      rpcrt4.lib $(guilibs)

whelloc.obj : whelloc.c whelloc.h whello.h

whello_c.obj : whello_c.c whello.h

# Make the whellos
whellos : whellos.exe
whellos.exe : whellos.obj whellop.obj whello_s.obj
    $(link) $(linkdebug) $(conflags) -out:whellos.exe -map:whellos.map \
      whellos.obj whellop.obj whello_s.obj \
      rpcrt4.lib $(conlibs)

whellos.obj : whellos.c whello.h

whellop.obj : whellop.c whello.h

whello_s.obj : whello_s.c whello.h

# Make the stubs source
whello.h whello_c.c whello_s.c : whello.idl whello.acf
    midl -oldnames -cpp_cmd $(cc) -cpp_opt "-E" whello.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del whello.res
    -del whello.rbj
    -del whello_c.c
    -del whello_s.c
    -del whello.h
