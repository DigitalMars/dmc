Windows Sample Framework
========================

This document contains the following sections:

   Overview
   Graph
   Brief Sample Descriptions
   Building the Samples
   Architectural Approach
   File Specific Information

Overview
--------

The sample framework has been designed to illustrate many different
Windows programming concepts.  This includes samples that are very
basic, as well as samples that demonstrate the new Windows 95 and
Windows NT controls and API's.  It is designed to help both the
novice and advanced Windows programmers.

The Graph
---------

As you can see from the graph below (shown only in README.WRI),
most samples in the framework "derive" from Generic and Gen32
samples.  There are four basic samples (SIMPLE, INPUT, OUTPUT, 
and MENU) that do not derive from Generic.


Brief Sample Descriptions
-------------------------

ANALOG   - Common controls: Status bar, Progress Bar,
           Tracker(slider), Up/Down(spin).
BARMDI   - Status bar and Toolbar in MDI application.
           Also shows Tooltips.
BARSDI   - Status bar and Toolbar in SDI application.
           Also shows ToolTips.
BLANDMDI - Basic MDI application
CLIPTEXT - Clipboard sample
COMMONDG - Common dialog sample
DIALOG   - Basic modal and modeless dialog boxes.
DLLSKEL  - Basic Win32 DLL sample
EDITSDI  - Notepad-like sample in SDI application.
FILESYS  - Shows basic implementation of some of the new
           File System API's.
GDIDIB   - Demonstrates CreateDIBSection
GDIINPUT - GDI sample showing lines, points, beziers,
           brushes, pens, etc.  User input with mouse here.
GDIOUT   - Simple GDI drawing/output functions.  
	   No user input here.
GDIPAL   - Shows the use of the Windows palette manager on
           palette devices.  User can choose pen and brush
           colors from the system palette.
GDIPRINT - Shows how to print DIBs and metafiles.
           NOTE:  At this time, this sample does not print
           metafiles.
GEN32    - 32-bit-only version of Generic.
GENERIC  - Basic windows application with About box.
HOOKS    - Comprehensive sample showing a "Spy"-like utility.
IATHREAD - Interactive thread sample.  Shows multithreaded MDI
           application, with user-set thread priority.
INITREE  - Shows use of TreeView control by enumerating the .INI
           files in the Windows 95 directory.  Use of Imagelists
           is also demonstrated.
INPUT    - Show the use of simple windows input via mouse, 
	   keyboard, and timer.
LISTCTRL - Demonstrates the Listview control.
           The listview control also uses an Imagelist.
MENU     - Show several ways of using and manipulating menus, 
	   including handling menu commands from menu bars and 
	   pop menus, inserting menus on the fly, modifying menus, 
	   and implementing owner draw.  
	   This sample uses several API's specific to Windows 95 
	   and therefore will run only on Windows 95.           
MINIHELP - Show how to call the Windows Help Engine from
           an application.
MULTIPAD - MDI version of EDITSDI. Notepad-like sample.
OLE2	 - SIMPCNTR - Simple OLE2 container.
	   SIMPSVR  - Simple OLE2 Server
OUTPUT   - Shows the use of two fundamental text drawing API's 
	   and several of the line and shape drawing API's.
PROPSHET - Simple implementation of a property sheet control.
RTFEDIT  - Basic demo of the new RTF edit control.
SIMPLE   - Shows the use of two fundamental text drawing API's 
	   and several of the line and shape drawing API's.
SUBCLS   - Subclassing a listbox and edit control.
TABCTRL  - Shows how to implement the new tab control.
THREADS  - Basic thread sample.
WRITEPAD - Multipad-like sample that uses the RTF edit control.


Building the Samples
--------------------

ALL of the samples will build for 32 bits.  

Each sample contains a MAKEFILE file for building the application.
Simply define the environment variables needed for SMAKE. Then, run the
SMAKE command to build the sample.  


Architectural Approach
----------------------

The basic programming architecture is implemented using a message-
based dispatching mechanism similar to that used by the Microsoft
Foundation Classes.  Note that the mechanism used here is very 
basic and is intended to be simple yet functional.  It is NOT 
intended to replace the method used by MFC.

Each message that needs to be processed is entered in a table 
(all window and dialog procedures have their own table), and an 
associated "handler function" is specified.  This allows the 
processing for each message to be isolated in a single function.  
When a given message is bound for a window procedure, the 
dispatching architecture will call the appropriate handler 
function.  All processing for the message is done in the handler 
function.

The framework is designed to be modular, so that you can take
"pieces" of one sample and "plug" them into another.

File Specific Information
-------------------------

All samples from Generic (or Gen32) down contain the following 
base files.  Note that GENERIC.RC, GENERIC.ICO and GENERIC.DEF 
(.DEF file will  not exist for 32 bit only samples) will be 
<samplename>.* rather than GENERIC.*

ABOUT    C    -- Complete code to bring up About box
ABOUT    DLG  -- About box dialog template
DISPATCH C    -- Dispatching routines for the messages
GENERIC  DEF  -- Module definition file for 16 bit builds (some
                 samples are 32 bit only and will not contain a 
                 DEF file)
GENERIC  ICO  -- Icon file
GENERIC  RC   -- Resource file.  Includes menus, icons, dialog 
                 template
GLOBALS  H    -- Header file containing global variables and 
                 function prototypes
INIT     C    -- Initialization code for main application
MAKEFILE      -- Makefile for building the sample.
MISC     C    -- Contains generic support functions
README   TXT  -- Describes the sample
WIN16EXT H    -- Header file for 16 bit builds.  Will not be there 
                 for 32-bit-only samples.
WINMAIN  C    -- Standard WinMain function
