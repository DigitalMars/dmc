========================================================================
	   MICROSOFT FOUNDATION CLASS LIBRARY : CONTAIN
========================================================================

AppWizard has generated this CONTAIN application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your CONTAIN application.


CONTAIN.MAK
	This project file is compatible with the Visual C++ Workbench.

	It is also compatible with the NMAKE program provided with the
	Professional Edition of Visual C++.

	To build a debug version of the program from the MS-DOS prompt, type
	nmake DEBUG=1 /f CONTAIN.MAK
	or to build a release version of the program, type
	nmake DEBUG=0 /f CONTAIN.MAK

CONTAIN.H
	This is the main include file for the application.  It includes other
	project specific includes (including RESOURCE.H) and declares the
	CContainApp application class.

CONTAIN.CPP
	This is the main application source file that contains the application
	class CContainApp.

CONTAIN.RC
	This is a listing of all of the Microsoft Windows resources that the
	program uses.  It includes the icons, bitmaps, and cursors that are stored
	in the RES subdirectory.  This file can be directly edited with App Studio.

RES\CONTAIN.ICO
	This is an icon file, which is used as the application's icon.  This
	icon is included by the main resource file CONTAIN.RC.

RES\CONTAIN.RC2
	This file contains resources that are not edited by App Studio.  Initially
	this contains a VERSIONINFO resource that you can customize for your
	application.  You should place other non-App Studio editable resources
	in this file.

CONTAIN.DEF
	This file contains information about the application that must be
	provided to run with Microsoft Windows.  It defines parameters
	such as the name and description of the application, and the size
	of the initial local heap.  The numbers in this file are typical
	for applications developed with the Microsoft Foundation Class Library.
	The default stack size can be adjusted by editing the project file.

CONTAIN.CLW
	This file contains information used by ClassWizard to edit existing
	classes or add new classes.  ClassWizard also uses this file to store
	information needed to generate and edit message maps and dialog data
	maps and to generate prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MAINFRM.H, MAINFRM.CPP
	These files contain the frame class CMainFrame, which is derived from
	CMDIFrameWnd and controls all MDI frame features.

RES\TOOLBAR.BMP
	This bitmap file is used to create tiled images for the toolbar.
	The initial toolbar and status bar are constructed in the
	CMainFrame class.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

CONTRDOC.H, CONTRDOC.CPP - the document
	These files contain your CContainDoc class.  Edit these files to
	add your special document data and to implement file saving and loading
	(via CContainDoc::Serialize).

CONTRVW.H, CONTRVW.CPP - the view of the document
	These files contain your CContainView class.
	CContainView objects are used to view CContainDoc objects.


RES\CONTRDOC.ICO
	This is an icon file, which is used as the icon for MDI child windows
	for the CContainDoc class.  This icon is included by the main
	resource file CONTAIN.RC.

CONTAIN.REG
	This is an example .REG file that shows you the kind of registration
	settings the framework will set for you.  You can use this as a .REG
	file to go along with your application or just delete it and rely
	on the default RegisterShellFileTypes registration.

/////////////////////////////////////////////////////////////////////////////

AppWizard has also created classes specific to OLE 2.0 Linking and Embedding

CNTRITEM.H, CNTRITEM.CPP - this class is used to
	manipulate OLE objects.  They are usually displayed by your
	CContainView class and serialized as part of your CContainDoc class.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

STDAFX.H, STDAFX.CPP
	These files are used to build a precompiled header (PCH) file
	named STDAFX.PCH and a precompiled types (PCT) file named STDAFX.OBJ.

RESOURCE.H
	This is the standard header file, which defines new resource IDs.
	App Studio reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
