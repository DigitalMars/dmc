#+---------------------------------------------------------------------------
#
#  Microsoft Windows
#  Copyright (C) Microsoft Corporation, 1994-1995.
#
#  File:       makefile
#
#  Contents:   builds the MFract fractal engines
#
#  History:     5-03-94   stevebl   Created
#
#----------------------------------------------------------------------------

DIRLIST = mandel julia quad plasma rgen

all: $(DIRLIST)

$(DIRLIST):
        cd $@
        @nmake -a -i -nologo
        cd ..


