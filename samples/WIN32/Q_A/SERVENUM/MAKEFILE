
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.

#
# Master Makefile for the Win32 ServerEnumDialog Sample
#

#
# To execute a build, refer to the following table:
#
#     Build Type                Command line
#     -----------------         ----------------------------
#     32-bit, debug             nmake
#     32-bit, non-debug 	nmake "nodebug=1"
#

!ifdef nodebug
# Non-debugging options
!MESSAGE 32-bit Non-debugging build
All:
    cd dll
    nmake "nodebug=1"
    copy servenum.dll ..\demo
    copy servenum.lib ..\demo
    cd ..\demo
    nmake "nodebug=1"
    cd ..
!else
# Debugging options
!MESSAGE 32-bit Debugging build
All:
    cd dll
    nmake
    copy servenum.dll ..\demo
    copy servenum.lib ..\demo
    cd ..\demo
    nmake
    cd ..
!endif


#--------------------------------------------------------------------------
# Rules for cleaning out those old files

clean:
    cd dll
    del *.bak
    del *.pdb
    del *.obj
    del *.res
    del *.exp
    del *.map
    del *.sbr
    del *.bsc

    cd ..\demo
    del *.bak
    del *.pdb
    del *.obj
    del *.res
    del *.exp
    del *.map
    del *.sbr
    del *.bsc

    cd ..

cleaner: clean
    cd dll
    del *.exe
    del *.lib
    del *.dll

    cd ..\demo
    del *.exe
    del *.lib
    del *.dll

    cd ..

# End of file -------------------------------------------------------------
