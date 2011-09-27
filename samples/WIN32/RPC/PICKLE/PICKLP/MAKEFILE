#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   pickle Application                    **#
#**            Copyright(c) Microsoft Corp. 1992-1995       **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

!if "$(CPU)" == "i386"
cflags = $(cflags:G3=Gz)
!endif


all : picklpc

# Make the picklp client
picklpc : picklpc.exe
picklpc.exe : picklpc.obj picklp_c.obj
    $(link) $(linkdebug) $(conflags) -out:picklpc.exe \
      picklpc.obj picklp_c.obj \
      rpcrt4.lib $(conlibs)

# client main program
picklpc.obj : picklpc.c picklp.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# client stub
picklp_c.obj : picklp_c.c picklp.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c


# Stub and header file from the IDL file
picklp.h picklp_c.c: picklp.idl picklp.acf
    midl -oldnames -no_cpp picklp.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del picklp_c.c
    -del picklp.h

