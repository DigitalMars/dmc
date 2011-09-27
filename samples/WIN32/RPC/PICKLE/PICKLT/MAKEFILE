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


all : pickltc

# Make the picklt client
pickltc : pickltc.exe
pickltc.exe : pickltc.obj picklt_c.obj
    $(link) $(linkdebug) $(conflags) -out:pickltc.exe \
      pickltc.obj picklt_c.obj \
      rpcrt4.lib $(conlibs)

# client main program
pickltc.obj : pickltc.c picklt.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# client stub
picklt_c.obj : picklt_c.c picklt.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# Stub and header file from the IDL file
picklt.h picklt_c.c: picklt.idl picklt.acf
    midl -oldnames -no_cpp -ms_ext picklt.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del picklt_c.c
    -del picklt.h

