#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                    yield Application                    **#
#**            Copyright(c) Microsoft Corp. 1991-1995       **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>


all: yields

# Make the yield server
yields : yields.exe
yields.exe : yields.obj yieldp.obj yield_s.obj
    $(link) $(linkdebug) $(conflags) -out:yields.exe -map:yields.map \
      yields.obj yieldp.obj yield_s.obj \
      rpcrt4.lib $(conlibsmt)

yields.obj : yields.c yield.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

yieldp.obj : yieldp.c yield.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

yield_s.obj : yield_s.c yield.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# Make the stubs source
yield.h yield_s.c : yield.idl yield.acf
    midl -oldnames -cpp_cmd $(cc) -cpp_opt "-E" yield.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del yield_s.c
    -del yield.h
