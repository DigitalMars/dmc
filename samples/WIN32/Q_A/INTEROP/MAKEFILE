#
# dll32.mak
#
# Note that the Universal Thunking code is not built for RISC platforms.
# The makefile is special-cased with !IF "$(CPU)" == "i386"
#

# Nmake macros for building Windows 32-Bit applications

!include <ntwin32.mak>

proj1 = dll32
proj2 = utdll32
proj3 = stub32
proj4 = disp32

all: $(proj1).dll \
!IF "$(CPU)" == "i386"
     $(proj2).dll \
     $(proj3).exe \
!ENDIF
     $(proj4).exe

# Update the object files if necessary

$(proj1).obj: $(proj1).c
    $(cc) $(cdebug) $(cflags) $(cvars) $(proj1).c

!IF "$(CPU)" == "i386"
$(proj2).obj: $(proj2).c
    $(cc) $(cdebug) $(cflags) $(cvars) $(proj2).c

$(proj3).obj: $(proj3).c
    $(cc) $(cdebug) $(cflags) $(cvars) $(proj3).c
!ENDIF

$(proj4).obj: $(proj4).c
    $(cc) $(cdebug) $(cflags) $(cvars) $(proj4).c

# Update the import library

$(proj1).lib $(proj1).exp : $(proj1).def
   $(implib) -machine:$(CPU)     \
             -def:$(proj1).def        \
             -out:$(proj1).lib \
             $(proj1).obj

!IF "$(CPU)" == "i386"
$(proj2).lib $(proj2).exp : $(proj2).def
   $(implib) -machine:$(CPU)     \
             -def:$(proj2).def        \
             -out:$(proj2).lib \
             $(proj2).obj
!ENDIF

# Update the dynamic link library

$(proj1).dll: $(proj1).obj $(proj1).exp
    $(link) $(linkdebug) $(dlllflags)    \
            -base:0x1C000000  \
            -out:$(proj1).dll   \
            $(proj1).exp $(proj1).obj $(guilibs)

!IF "$(CPU)" == "i386"
$(proj2).dll: $(proj2).obj $(proj2).exp $(proj1).lib
    $(link) $(linkdebug) $(dlllflags)    \
            -base:0x1C000000  \
            -dll              \
            -out:$(proj2).dll   \
	    $(proj2).exp $(proj2).obj $(guilibs) $(proj1).lib

$(proj3).exe: $(proj3).obj $(proj2).lib
    $(link) $(ldebug) $(guilflags) \
	    $(proj3).obj $(guilibs) \
	    $(proj2).lib -out:$(proj3).exe
!ENDIF

$(proj4).exe: $(proj4).obj $(proj1).lib
    $(link) $(ldebug) $(guilflags) \
	    $(proj4).obj $(guilibs) $(proj1).lib -out:$(proj4).exe
