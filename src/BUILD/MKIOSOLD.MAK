# MKIOSOLD.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile includes for IOSOLD streams library
# $Revision: 1.1.1.1 $


all:  ..\..\lib\iosolddl.lib ..\..\lib\iosoldds.lib ..\..\lib\iosolddm.lib ..\..\lib\iosolddc.lib \
      ..\..\lib\iosoldwl.lib ..\..\lib\iosoldws.lib ..\..\lib\iosoldwm.lib ..\..\lib\iosoldwc.lib \
      ..\..\lib\iosolddx.lib ..\..\lib\iosoldnn.lib

clean:
    -del ..\..\lib\iosolddl.lib 
    -del ..\..\lib\iosoldds.lib 
    -del ..\..\lib\iosolddm.lib 
    -del ..\..\lib\iosolddc.lib 
    -del ..\..\lib\iosoldwl.lib 
    -del ..\..\lib\iosoldws.lib 
    -del ..\..\lib\iosoldwm.lib 
    -del ..\..\lib\iosoldwc.lib 
    -del ..\..\lib\iosolddx.lib 
    -del ..\..\lib\iosoldnn.lib

..\..\lib\iosolddl.lib:
    smake /nologo -fiosold.mak OS=MSDOS  MODEL=L INTSIZE=16
..\..\lib\iosoldds.lib:                                    
    smake /nologo -fiosold.mak OS=MSDOS  MODEL=S INTSIZE=16
..\..\lib\iosolddm.lib:
    smake /nologo -fiosold.mak OS=MSDOS  MODEL=M INTSIZE=16
..\..\lib\iosolddc.lib:
    smake /nologo -fiosold.mak OS=MSDOS  MODEL=C INTSIZE=16
..\..\lib\iosoldwl.lib: 
    smake /nologo -fiosold.mak OS=_WINDOWS  MODEL=L INTSIZE=16
..\..\lib\iosoldws.lib: 
    smake /nologo -fiosold.mak OS=_WINDOWS  MODEL=S INTSIZE=16
..\..\lib\iosoldwm.lib: 
    smake /nologo -fiosold.mak OS=_WINDOWS  MODEL=M INTSIZE=16
..\..\lib\iosoldwc.lib:
    smake /nologo -fiosold.mak OS=_WINDOWS  MODEL=C INTSIZE=16
..\..\lib\iosolddx.lib:
    smake /nologo -fiosold.mak OS=DOS386  MODEL=X INTSIZE=32
..\..\lib\iosoldnn.lib:
    smake /nologo -fiosold.mak OS=__NT__  MODEL=N INTSIZE=16


