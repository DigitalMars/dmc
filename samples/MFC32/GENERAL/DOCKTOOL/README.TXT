========================================================================
	   MICROSOFT FOUNDATION CLASS LIBRARY : DOCKTOOL
========================================================================


This file contains a summary of what you will find in each of the files that
make up the DOCKTOOL demo application.

/////////////////////////////////////////////////////////////////////////////

The following files are unchanged from how AppWizard generated them:

DOCKTOOL.MAK
	This project file is compatible with the Visual C++ development
	environment.

	It is also compatible with the NMAKE program provided with Visual C++.

	To build a debug version of the program from the MS-DOS prompt, type


		nmake /f DOCKTOOL.MAK CFG="Win32 Debug"

	or to build a release version of the program, type

		nmake /f DOCKTOOL.MAK CFG="Win32 Release"

DOCKTOOL.H
	This is the main header file for the application.  It includes other
	project specific headers (including RESOURCE.H) and declares the
	CDockApp application class.

DOCKTOOL.CPP
	This is the main application source file that contains the application
	class CDockApp.

RES\DOCKTOOL.ICO
	This is an icon file, which is used as the application's icon.  This
	icon is included by the main resource file DOCKTOOL.RC.

RES\DOCKTOOL.RC2
	This file contains resources that are not edited by the Visual C++
	development environment.  You should place all resources not
	editable by the resource editor in this file.

DOCKTOOL.CLW
	This file contains information used by ClassWizard to edit existing
	classes or add new classes.  ClassWizard also uses this file to store
	information needed to create and edit message maps and dialog data
	maps and to create prototype member functions.

DOCKDOC.H, DOCKDOC.CPP - the document
	These files contain your CDockDoc class.  Edit these files to
	add your special document data and to implement file saving and loading
	(via CDockDoc::Serialize).

DOCKVW.H, DOCKVW.CPP - the view of the document
	These files contain your CDockView class.
	CDockView objects are used to view CDockDoc objects.

STDAFX.H, STDAFX.CPP
	These files are used to build a precompiled header (PCH) file
	named DOCKTOOL.PCH and a precompiled types file named STDAFX.OBJ.

RESOURCE.H
	This is the standard header file, which defines new resource IDs.
	Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

The following files are changed to support the functionality of the demo
application.

MAINFRM.H, MAINFRM.CPP
	These files contain the frame class CMainFrame, which is derived from
	CFrameWnd and controls all SDI frame features.  This file also contains:
	all the toolbar initialization and persistence code; special code for easy
	persistence of the window placement;the code that initializes
	the Tools Dialog and modifies the toolbars if the user presses OK; and
	special code that uses runtime type checking to easily add a right mouse
	button context menu to all toolbars and statusbars.

EDITBAR.H, EDITBAR.CPP
	These files contain the toolbar class CEditBar, which is derived from
	CToolBar and controls the special functionality of the Edit toolbar
	which has a CSearchBox(CComboBox) when it is horizontally oriented and
	a button when it is vertically oriented.  See the comments in these
	source for specifics on how this is accomplished.

SEARCHBX.H, SEARCHBX.CPP
	These files contain the combobox class CSearchBox, which is derived from
	CComboBox and controls the special functionality of the combobox in the
	Edit toolbar.  When the user press enter in the combobox, the text currently
	entered in the combobox is added to the top of the dropdown list.  This is
	done by overloading the PreTranslateMessage method to capture all WM_KEYDOWN
	messages and swallow any VK_RETURN keys and provide alternate functionality.
	It is initially tempting to just add a VK_RETURN accelerator and a command
	handler.  But if this was a real application, you would run into trouble
	quickly because Windows would steal all VK_RETURNs from the application meaning
	that multiline edit controls wouldn't work.  See the comments in these source
	for specifics on how this is accomplished.

PALETTE.H, PALETTE.CPP
	These files contain the toolbar class CPaletteBar, which is derived from
	CToolBar and controls the special functionality of the Palette toolbar
	which has multiple rows or columns.  Originally the idea was to take the
	CPalete class in the CTRLBARS sample and make it dockable.  It turned out
	all that was necessary was to remove a lot of the code in these files,
	and let CToolBar do the work for us.  See the comments in these source
	for specifics on how this is accomplished.

TOOLDLG.H, TOOLDLG.CPP
	These files contain the dialog class CToolDlg, which is derived from
	CDialog.  These files are maintianed completely through Class Wizards.

RES\CBROWSE.BMP
	This bitmap file is used to create tiled images for the Browse toolbar
	when it is in color mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\CDEBUG.BMP
	This bitmap file is used to create tiled images for the Debug toolbar
	when it is in color mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\CEDTHORZ.BMP
	This bitmap file is used to create tiled images for the Edit toolbar
	when it is horizontally oriented and in color mode.  Edit this toolbar
	bitmap along with the array in EDITBAR.CPP to add more toolbar buttons.

RES\CEDTVERT.BMP
	This bitmap file is used to create tiled images for the Edit toolbar
	when it is vertically oriented and in color mode.  Edit this toolbar
	bitmap along with the array in EDITBAR.CPP to add more toolbar buttons.

RES\CMAIN.BMP
	This bitmap file is used to create tiled images for the Main toolbar
	when it is in color mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\CPALETTE.BMP
	This bitmap file is used to create tiled images for the Palette toolbar
	when it is in color mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\CRESRC.BMP
	This bitmap file is used to create tiled images for the Resource toolbar
	when it is in color mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\MBROWSE.BMP
	This bitmap file is used to create tiled images for the Browse toolbar
	when it is in mono mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\MDEBUG.BMP
	This bitmap file is used to create tiled images for the Debug toolbar
	when it is in mono mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\MEDTHORZ.BMP
	This bitmap file is used to create tiled images for the Edit toolbar
	when it is horizontally oriented and in mono mode.  Edit this toolbar
	bitmap along with the array in EDITBAR.CPP to add more toolbar buttons.

RES\MEDTVERT.BMP
	This bitmap file is used to create tiled images for the Edit toolbar
	when it is vertically oriented and in mono mode.  Edit this toolbar
	bitmap along with the array in EDITBAR.CPP to add more toolbar buttons.

RES\MMAIN.BMP
	This bitmap file is used to create tiled images for the Main toolbar
	when it is in mono mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\MPALETTE.BMP
	This bitmap file is used to create tiled images for the Palette toolbar
	when it is in mono mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

RES\MRESRC.BMP
	This bitmap file is used to create tiled images for the Resource toolbar
	when it is in mono mode.  Edit this toolbar bitmap along with the
	array in MAINFRM.CPP to add more toolbar buttons.

DOCKTOOL.RC
	This is a listing of all of the Microsoft Windows resources that the
	program uses.  It includes the icons, bitmaps, and cursors that are stored
	in the RES subdirectory.  This file can be directly edited in the
	Visual C++ development environment.
