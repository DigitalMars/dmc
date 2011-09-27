========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : catalog2
========================================================================


AppWizard has created this catalog2 application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your catalog2 application.


catalog2.mak
    This project file is compatible with the Visual C++ development
	environment.

    It is also compatible with the NMAKE program provided with Visual C++.

    To build a debug version of the program from the MS-DOS prompt, type
nmake /f catalog2.mak CFG="Win32 Debug"
    or to build a release version of the program, type
nmake /f catalog2.mak CFG="Win32 Release"


catalog2.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CCatalog2App application class.

catalog2.cpp
    This is the main application source file that contains the application
    class CCatalog2App.

catalog2.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in the
    Visual C++ development environment.

res\catalog2.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file catalog2.rc.

res\catalog2.rc2
    This file contains resources that are not edited by the Visual C++
	development environment.  You should place all resources not
	editable by the resource editor in this file.

catalog2.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the
    CMainFrame class.  Edit this toolbar bitmap along with the
    array in MainFrm.cpp to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

cat2Doc.h, catalog2Doc.cpp - the document
    These files contain your CCatalog2Doc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CCatalog2Doc::Serialize).

cat2View.h, catalog2View.cpp - the view of the document
    These files contain your CCatalog2View class.
    CCatalog2View objects are used to view CCatalog2Doc objects.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named catalog2.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
