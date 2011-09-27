#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   xmit Application                      **#
#**          Copyright(c) Microsoft Corp. 1992-1995         **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

all : xmitc xmits

# Make the client
xmitc : xmitc.exe
xmitc.exe : xmitc.obj xmitu.obj xmit_c.obj
    $(link) $(linkdebug) $(conflags) -out:xmitc.exe -map:xmitc.map \
      xmitc.obj xmitu.obj xmit_c.obj \
      rpcrt4.lib $(conlibs)

# xmit client main program
xmitc.obj : xmitc.c xmit.h xmitu.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

xmitu.obj : xmitu.c xmit.h xmitu.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# xmit client stub
xmit_c.obj : xmit_c.c xmit.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c


# Make the server executable
xmits : xmits.exe
xmits.exe : xmits.obj xmitu.obj xmitp.obj xmit_s.obj
    $(link) $(linkdebug) $(conflags) -out:xmits.exe -map:xmits.map \
      xmits.obj xmitu.obj xmit_s.obj xmitp.obj \
      rpcrt4.lib $(conlibsmt)

# xmit server main
xmits.obj : xmits.c xmit.h xmitu.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# remote procedures
xmitp.obj : xmitp.c xmit.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# xmit server stub file
xmit_s.obj : xmit_s.c xmit.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# Stubs and header file from the IDL file
xmit.h xmit_c.c xmit_s.c : xmit.idl xmit.acf
    midl -oldnames -no_cpp xmit.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del xmit_c.c
    -del xmit_s.c
    -del xmit.h
