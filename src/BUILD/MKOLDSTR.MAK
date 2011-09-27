# MKOLDSTR.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile includes for OLDSTR streams library
# $Revision: 1.1.1.1 $

all:  ..\..\lib\oldstrl.lib ..\..\lib\oldstrs.lib ..\..\lib\oldstrm.lib ..\..\lib\oldstrc.lib \
      ..\..\lib\oldstrx.lib ..\..\lib\oldstrv.lib
      

clean:
    -del ..\..\lib\oldstrl.lib 
    -del ..\..\lib\oldstrs.lib 
    -del ..\..\lib\oldstrm.lib 
    -del ..\..\lib\oldstrc.lib 
    -del ..\..\lib\oldstrx.lib 
    -del ..\..\lib\oldstrv.lib
    
..\..\lib\oldstrl.lib:
    smake /nologo -foldstr.mak OS=MSDOS MODEL=l
..\..\lib\oldstrs.lib:
    smake /nologo -foldstr.mak OS=MSDOS MODEL=s
..\..\lib\oldstrm.lib:
    smake /nologo -foldstr.mak OS=MSDOS MODEL=m
..\..\lib\oldstrc.lib:
    smake /nologo -foldstr.mak OS=MSDOS MODEL=c
..\..\lib\oldstrx.lib:
    smake /nologo -foldstr.mak OS=DOS386 MODEL=x
..\..\lib\oldstrv.lib    
    smake /nologo -foldstr.mak OS=MSDOS  MODEL=v

