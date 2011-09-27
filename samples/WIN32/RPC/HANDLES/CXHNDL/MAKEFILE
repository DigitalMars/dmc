#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   cxhndl Application                    **#
#**          Copyright(c) Microsoft Corp. 1992-1995         **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

!if "$(CPU)" == "i386"
cflags = $(cflags:G3=Gz)
!endif

all : cxhndlc cxhndls

# Make the cxhndl client
cxhndlc : cxhndlc.exe
cxhndlc.exe : cxhndlc.obj cxhndl_c.obj
    $(link) $(linkdebug) $(conflags) -out:cxhndlc.exe \
      cxhndlc.obj cxhndl_c.obj \
      rpcrt4.lib $(conlibs)

# client main program
cxhndlc.obj : cxhndlc.c cxhndl.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# client stub
cxhndl_c.obj : cxhndl_c.c cxhndl.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# Make the cxhndl server
cxhndls : cxhndls.exe
cxhndls.exe : cxhndls.obj cxhndlp.obj cxhndl_s.obj
    $(link) $(linkdebug) $(conflags) -out:cxhndls.exe \
      cxhndls.obj cxhndlp.obj cxhndl_s.obj \
      rpcrt4.lib $(conlibsmt)

# server main loop
cxhndls.obj : cxhndls.c cxhndl.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# remote procedures
cxhndlp.obj : cxhndlp.c cxhndl.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# server stub file
cxhndl_s.obj : cxhndl_s.c cxhndl.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c


# Stubs and header file from the IDL file
cxhndl.h cxhndl_c.c cxhndl_s.c : cxhndl.idl cxhndl.acf
    midl -oldnames -no_cpp cxhndl.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del cxhndl_c.c
    -del cxhndl_s.c
    -del cxhndl.h
