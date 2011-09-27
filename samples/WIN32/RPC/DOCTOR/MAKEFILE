#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   doctor Application                    **#
#**         Copyright(c) Microsoft Corp. 1992-1995          **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

all : doctorc doctors

# Make the doctor client executable
doctorc : doctorc.exe
doctorc.exe : doctorc.obj doctor_c.obj
    $(link) $(linkdebug) $(conflags) -out:doctorc.exe \
      doctorc.obj doctor_c.obj \
      rpcrt4.lib $(conlibs)

# client main program
doctorc.obj : doctorc.c doctor.h
   $(cc) $(cdebug) $(cflags) $(cvars) $(DEBUG) $*.c

# client stub
doctor_c.obj : doctor_c.c doctor.h
   $(cc) $(cdebug) $(cflags) $(cvars) $(DEBUG) $*.c


# Make the doctor server executable
doctors : doctors.exe
doctors.exe : doctors.obj doctorp.obj doctor_s.obj
    $(link) $(linkdebug) $(conflags) -out:doctors.exe \
      doctors.obj doctor_s.obj doctorp.obj \
      rpcrt4.lib $(conlibsmt)

# server main loop
doctors.obj : doctors.c doctor.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $(DEBUG) $*.c

# remote procedures
doctorp.obj  : doctorp.c doctor.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $(DEBUG) $*.c

# server stub file
doctor_s.obj : doctor_s.c doctor.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $(DEBUG) $*.c

# Stubs and header file from the IDL file
doctor.h doctor_c.c doctor_s.c : doctor.idl doctor.acf
    midl -oldnames -no_cpp -ms_ext doctor.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del doctor_c.c
    -del doctor_s.c
    -del doctor.h
