============
  LAUNCHER
============

  DESCRIPTION
---------------

launcher.exe is a small application which allows OLE 1.0 compatibility
for OLE 2.X InProc (DLL) object servers. there are two minor problems
with this compatibility and they are: the exe registers the IClassFactory
as SINGLE_USE so for each object you will get about 39k or so overhead
and the second is that IViewObject is NOT marshaled so you will need
to support giving out presentation through IDataObject::GetData().


  HOW TO
----------

launcher can be rebuilt using MFC 2.0 and C8 (VC++) using the NMAKE command
>>> NMAKE DEBUG=0 (retail build)
>>> NMAKE DEBUG=1 (debug build)


to make launcher work for your application you need to do the following
things:

  1. Modify you reg.dat entry for your InProc Server to contain the following
	 two additions, StdFileEditing entry and your Verb entries.

example:

HKEY_CLASSES_ROOT\{YourProgID}\protocol\StdFileEditing\server = {your exe}.exe
HKEY_CLASSES_ROOT\{YourProgID}\protocol\StdFileEditing\verb\0 = &Edit

  2. Modify the entry in launcher.h for the following

>>> #define SZCLASSID   "{A11CC4E0-C0E1-1068-B87C-00DD0107D0BD}"

  to...

>>> #define SZCLASSID   "Your class id"

  3. Modify the makfile for the entry, ***NOTE*** this can not be the
	 same name as your DLL !!!

>>> EXENAME=DVLAUNCH

  to...

>>> EXENAME=YourExeName
