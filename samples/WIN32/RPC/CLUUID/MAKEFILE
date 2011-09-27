#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   cluuid Application                    **#
#**         Copyright(c) Microsoft Corp. 1992-1995          **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>


all : cluuidc cluuids

# Make the client side application cluuidc
cluuidc : cluuidc.exe
cluuidc.exe : cluuidc.obj cluuid_c.obj
    $(link) $(linkdebug) $(conflags) -out:cluuidc.exe \
      cluuidc.obj cluuid_c.obj \
      rpcrt4.lib $(conlibs)

# cluuidc main program
cluuidc.obj : cluuidc.c cluuid.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# cluuidc stub
cluuid_c.obj : cluuid_c.c cluuid.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# Make the server side application
cluuids : cluuids.exe
cluuids.exe : cluuids.obj cluuidp.obj cluuid_s.obj
    $(link) $(linkdebug) $(conflags) -out:cluuids.exe \
      cluuids.obj cluuid_s.obj cluuidp.obj \
      rpcrt4.lib $(conlibsmt)

# cluuid server main program
cluuids.obj : cluuids.c cluuid.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# remote procedures
cluuidp.obj : cluuidp.c cluuid.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# cluuids stub file
cluuid_s.obj : cluuid_s.c cluuid.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# Stubs and header file from the IDL file
cluuid.h cluuid_c.c cluuid_s.c : cluuid.idl cluuid.acf
    midl -oldnames -use_epv -no_cpp cluuid.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del cluuid_c.c
    -del cluuid_s.c
    -del cluuid.h
