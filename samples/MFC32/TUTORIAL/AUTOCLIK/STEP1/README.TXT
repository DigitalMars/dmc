========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : AutoClik
========================================================================

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


AppWizard has created this AutoClik application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your AutoClik application.


AutoClik.mak
    This project file is compatible with the Visual C++ development
	environment.

    It is also compatible with the NMAKE program provided with Visual C++.

    To build a debug version of the program from the MS-DOS prompt, type
nmake /f AutoClik.mak CFG="Win32 Debug"
    or to build a release version of the program, type
nmake /f AutoClik.mak CFG="Win32 Release"


AutoClik.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CAutoClickApp application class.

AutoClik.cpp
    This is the main application source file that contains the application
    class CAutoClickApp.

AutoClik.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in the
    Visual C++ development environment.

res\AutoClik.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file AutoClik.rc.

res\AutoClik.rc2
    This file contains resources that are not edited by the Visual C++
	development environment.  You should place all resources not
	editable by the resource editor in this file.

AutoClik.reg
    This is an example .REG file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .REG
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.

AutoClik.odl
    This file contains the Object Description Language source code for the
    type library of your application.

AutoClik.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CMDIFrameWnd and controls all MDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the
    CMainFrame class.  Edit this toolbar bitmap along with the
    array in MainFrm.cpp to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

ClikDoc.h, ClikDoc.cpp - the document
    These files contain your CAutoClickDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CAutoClickDoc::Serialize).

ClikView.h, ClikView.cpp - the view of the document
    These files contain your CAutoClickView class.
    CAutoClickView objects are used to view CAutoClickDoc objects.

res\ClikDoc.ico
    This is an icon file, which is used as the icon for MDI child windows
    for the CAutoClickDoc class.  This icon is included by the main
    resource file AutoClik.rc.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named AutoClik.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
