
		STLport
		-------


This is STLport 4.5.3, modified to work with the Digital Mars C++ compiler.
New files are:

	readme.txt	This file.
	diff.txt	File differences between the official release 4.5.3
			and the Digital Mars version.
	hello.cpp	Simple smoke test.

To compile a program using STLport's <iostreams> with the static library:

	dmc hello -I\dm\stlport\stlport

To use STLport without iostreams, simply add \dm\stlport\stlport to the
INCLUDE search path before \dm\include. Or, modify the INCLUDE entry
in \dm\bin\sc.ini to be:

	INCLUDE="%@P%\..\stlport\stlport";"%@P%\..\include";"%@P%\..\mfc\include";%INCLUDE%

If not using iostreams, be sure to compile with:

	-D_STLP_NO_NEW_IOSTREAMS

or modify \dm\stlport\stlport\config\stl_user_config.h and uncomment the
#define for it.

=========== For CD Users ==================================================

With the CD version of the compiler comes all the STLport libraries and
DLL's already built.

To compile a program using STLport's <iostreams> with the DLL library:

	dmc hello -I\dm\stlport\stlport \dm\lib\stlp45dms.lib -D_DLL

The following instructions for rebuilding them
require the CD version of the compiler:
To build the STLport libraries and dll's:

	cd \dm\stlport\src
	smake -f dm.mak

which will build them into \dm\stlport\lib. Note that this is smake.exe,
not make.exe. To install them:

	xcopy \dm\stlport\lib\*.lib \dm\lib
	xcopy \dm\stlport\lib\*.dll \dm\bin

Libraries:

    For static STL, static RTL:
	stlp45dm_static.lib		optimized (*)
	stlp45dm_debug_static.lib	with symbolic debug symbols
	stlp45dm_stldebug_static.lib	with symbolic debug symbols and _STLP_DEBUG on

    For static STL, static RTL:
	stlp45dm_staticx.lib
	stlp45dm_debug_staticx.lib
	stlp45dm_stldebug_staticx.lib

    For DLL STL, DLL RTL:
	stlp45dm.lib			For use with stlp45dm.dll
	stlp45dm_debug.lib		For use with stlp45dm_debug.dll
	stlp45dm_stldebug.lib		For use with stlp45dm_stldebug.dll

    For DLL STL, static RTL:
	stlp45dms.lib			For use with stlp45dms.dll
	stlp45dms_debug.lib		For use with stlp45dms_debug.dll
	stlp45dms_stldebug.lib		For use with stlp45dms_stldebug.dll

(*) This comes with the free downloadable stlport.zip, so there is no
need to build the STLport library.

DLLs:
	stlp45dm.dll
	stlp45dm_debug.dll
	stlp45dm_stldebug.dll

	stlp45dms.dll
	stlp45dms_debug.dll
	stlp45dms_stldebug.dll

Special thanks to Christof Meerwald for his work porting STLport to
Digital Mars.

