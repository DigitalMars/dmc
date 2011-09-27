# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.

TARGETOS=WINNT

!include <ntwin32.mak>

all: simple.exe client.exe

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c


## Simple dependencies

simple.obj: simple.c service.h

service.obj: service.c service.h

simple_objs = simple.obj service.obj

simple_libs = $(conlibsmt) advapi32.lib shell32.lib

simple.exe: $(simple_objs)
    $(link) $(linkdebug) $(conflags) -out:simple.exe $(simple_objs) $(simple_libs)



## Client dependencies

client.exe: client.obj
    $(link) $(linkdebug) $(conflags) -out:client.exe client.obj $(conlibsmt)
