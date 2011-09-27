========================================================================
        OLE Custom Control DLL : DB
========================================================================

ControlWizard has created this project for your DB OLE Custom
Control DLL, which contains 1 Control.

This skeleton project not only demonstrates the basics of writing an OLE
Custom Control, but is also a starting point for writing the specific
features of your control.

This file contains a summary of what you will find in each of the files
that make up your DB OLE Custom Control DLL.


DB.MAK
    The project makefile for building your 16-bit OLE Custom Control.
    This project file is compatible with the Visual C++ 1.5 Workbench.
    It is also compatible with NMAKE.

DB32.MAK
    The project makefile for building your 32-bit OLE Custom Control.
    This project file is compatible with the Visual C++ 2.0 Workbench.
    It is also compatible with NMAKE.

MAKEFILE
    A makefile that makes it easy to run NMAKE from the command prompt.
    Use the following parameters with NMAKE:
        DEBUG=0     Builds retail version
        DEBUG=1     Builds debug version (default)
        WIN32=0     Builds 16-bit version (default)
        WIN32=1     Builds 32-bit version
        UNICODE=0   Builds ANSI/DBCS version (default for WIN32=0)
        UNICODE=1   Build Unicode version (default for WIN32=1)

DB.H
    This is the main include file for the OLE Custom Control DLL.  It
    includes other project-specific includes such as RESOURCE.H.

DB.CPP
    This is the main source file that contains the OLE DLL initialization,
    termination and other bookkeeping.

DB.RC
    This is a listing of the Microsoft Windows resources that the project
    uses.  This file can be directly edited with the Visual C++ resource
    editor.

DB.RC2
    This file contains resources that are not edited by the resource editor.
    Initially this contains a VERSIONINFO resource that you can customize
    for your OLE Custom Control DLL, and a TYPELIB resource for your DLL's
    type library.  You should place other manually maintained resources in
    this file.

DB.DEF
    This file contains information about the OLE Custom Control DLL that
    must be provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the DLL, and the size of the initial
    local heap.  The numbers in this file are typical for OLE Custom Control
    DLLs.

DB32.DEF
	This is a version of DB.DEF for when building a 32-bit version of
	the OLE Custom Control DLL.

DB.CLW
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to generate and edit message maps and dialog data
    maps and to generate prototype member functions.

DB.ODL
    This file contains the Object Description Language source code for the
    type library of your OLE Control.

DB.ICO
    This file contains an icon that will appear in the About box.  This icon
    is included by the main resource file DB.RC.

/////////////////////////////////////////////////////////////////////////////

DBCTL.H
    This file contains the declaration of the CDbCtrl C++ class.

DBCTL.CPP
    This file contains the implementation of the CDbCtrl C++ class.

DBPPG.H
    This file contains the declaration of the CDbPropPage C++ class.

DBPPG.CPP
    This file contains the implementation of the CDbPropPage C++ class.

DBCTL.BMP
    This file contains a bitmap that a container will use to represent the
    CDbCtrl control when it appears on a tool palette.  This bitmap
    is included by the main resource file DB.RC.

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

ControlWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
