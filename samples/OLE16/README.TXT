The source code samples in the \sc\samples\ole16 subdirectories
are OLE 2 SDK samples written in C/C++.  See \sc\mfc\samples
for OLE 2 applications written in C++ using the Microsoft Foundation
Classes.

For an overview of each OLE sample application, please refer to
readme.txt in its respective subdirectory of \sc\samples\ole16.

IMPORTANT:  Before running any of the following OLE sample applications,
you must do the following:

1. Register the application with Windows by running REGEDIT.EXE,
   which is distributed with Windows 3.1 and Windows NT.  Use the 
   Merge Registration File command in REGEDIT's File menu to register
   each of the following OLE sample .REG files.  The .REG files are
   located in each OLE sample's respective subdirectory of 
   \sc\samples\ole16.  

    cl2test.reg
    dataview.reg
    dispcalc.reg
    dspcalc2.reg
    hello.reg
    ilb.reg
    outline.reg
    prgidmon.reg
    pssamp.reg
    simpsvr.reg
    spoly.reg
    spoly2.reg
    sr2test.reg
    xserver.reg

2. Put the \sc\samples\ole16\bin directory on your PATH.  This is
   necessary because the above .REG files do not have fully qualified
   paths.
