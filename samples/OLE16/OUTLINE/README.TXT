TOPIC: OLE 2.0 Outline Sample Code
FILE: readme.txt
DATE: 11/11/93


OUTLINE SERIES
--------------

This sample application set demonstrates taking a 'base' application
(in this case OUTLINE.EXE) and extending it into an OLE 2 server and
container (SVROUTL and CNTROUTL respectively) and into OLE 2 in-place
(aka visual editing) capable server and container (ISVROTL and ICNTROTL
respectively). These applications attempt to implement the complete
OLE 2 functionality and recommended user model. For example, these
applications implement all of the OLE 2 User Interface dialogs. As
such this is not a trivial sample application set. It is recommended
to first start using the SIMPSVR and SIMCNTR sample applications in
order to get an introduction to OLE 2. Later when you are ready for
more complex issues to use the OUTLINE sample series.

All OUTLINE applications support loading/saving files; they all use
docfiles for storage.


DIRECTORY STRUCTURE
-------------------

The OUTLINE sample uses the following directory structure:

								   SAMP
									|
							 +------+-------+
							 |              |
						  OUTLINE         OLE2UI
							 |
	   +--------------+------+-----+------------+------------+
	   |              |            |            |            |
	OUTLINE        SVROUTL      CNTROUTL     ISVROTL      ICNTROTL

	OUTLINE     -- source directory for OUTLINE sample app series
	OLE2UI      -- source directory for common OLE 2.0 UI library

	OUTLINE\OUTLINE     -- dir used to build OUTLINE.EXE
	OUTLINE\SVROUTL     -- dir used to build SVROUTL.EXE
	OUTLINE\CNTROUTL    -- dir used to build CNTROUTL.EXE
	OUTLINE\ISVROTL     -- dir used to build ISVROTL.EXE
	OUTLINE\ICNTROTL    -- dir used to build ICNTROTL.EXE

All five variations of the OUTLINE applications are built from common
sources.  All of the common source code (*.C and *.H files) is in the
..\OUTLINE directory. Each variant of the application builds in a
separate directory.

Files that begin with OUTL comprise the base version of the
application. They are also used by OLE versions of the applicaton. Files
that begin with CNTR are specific to the container version and files that
begin with SVR are specific to the server version.  Files that begin
with OLE are common to both the container and server versions. This
series of sample applications highlights the changes necessary to
implement OLE into an existing application.  The container version has
the same functionality as the base (OUTLINE) and also allows you to
embed objects.

Some of the major OLE features are organized into special
files in order to group related code across the application variants:
	DRAGDROP.C  -- Drag/Drop implementation
	CLIPBRD.C   -- OLE 2 style copy/paste implementation (data transfer)
	LINKING.C   -- Linking and Moniker support implementation
	CNTRINPL.C  -- In-place container implementation
	SVRINPL.C   -- In-place server implementation
	CLASSFAC.C  -- IClassFactory implementation

Conditional compilation is used to control what code is used with which
version. The following defines are used to control the compilation:
	OLE_VERSION         -- code common to all OLE versions
	OLE_SERVER          -- code used by both SVROUTL and ISVROTL
	OLE_CNTR            -- code used by both CNTROUTL and ICNTROTL
	INPLACE_CNTR        -- code used by ICNTROTL only
	INPLACE_SVR         -- code used by ISVROTL only

The following defines are used to identify code specific for a particular
feature (NOTE: it is not really intended that these symbols should be
turned off):
	USE_DRAGDROP        -- drag/drop code
	USE_FRAMETOOLS      -- formula bar and tool bar
	USE_HEADING         -- row/column headings code
	USE_STATUSBAR       -- status bar code
	USE_CTL3D           -- use CTL3D.DLL to have 3D effect for dialogs
	USE_MSGFILTER       -- IMessageFilter code
	_DEBUG              -- debug code

The master code in the ..\OUTLINE directory includes many #if statements
with the symbols described above. Some people might find it confusing to
read the code with all of this conditional compilation. In order to address
this issue a special tool was written (STRIPPER.EXE) to partially expand
out this conditional compilation to show the simplified resulting code. A
MKSRC.BAT batch file has been included in each of the build directories
that uses the STRIPPER tool to expand the source without most of the
conditional compilation. These stripped *.C files are put in the respective
build directories. The *.DFN files control what symbols are expanded,
stripped, or left unchanged.


HOW TO BUILD
------------

It is first necessary to build the OUTLUI.DLL library. This library is
a private version of the OLE2UI library to be used exclusively by the
OUTLINE samples applications. NO OTHER APPLICATION MAY SHIP A DLL CALLED
"OUTLUI.DLL". Every application that builds the OLE2UI library as a DLL
must build it with a unique name for that application. See the
OLE2UI\README.TXT for instructions on how to build the OLE2UI library.

The five versions of the OUTLINE application are built in five
subdirectories (OUTLINE, SVROUTL, CNTROUTL, ISVROTL, ICNTROTL).
The same makefile (MAKEFILE) is used to build all five applications:

	cd \ole2\samp\outline
	nmake                   -- builds OUTLINE base application
	nmake app=svroutl       -- builds SVROUTL server application
	nmake app=cntroutl      -- builds CNTROUTL container application
	nmake app=isvrotl       -- builds ISVROTL in-place server application
	nmake app=icntrotl      -- builds ICNTROTL in-place container app
	nmake clean             -- deletes generated files for OUTLINE
	nmake install           -- copies OUTLINE.EXE to SAMP\BIN directory
	nmake app=cntroutl clean-- deletes generated files for CNTROUTL
	nmake app=cntroutl install-- copies CNTROUTL.EXE to the SAMP\BIN dir



LIMITATIONS AND HINTS
---------------------

The following limitations affect the current version of the sample code:

*       The CNTROUTL sample container application can only support OLE
objects that are <=255 pixels high. OINEC currently uses a standard
OwnerDraw listbox; this is a limitation of the OwnerDraw listbox. If
larger objects are embedded or linked then they are forced to scale
(maintaining the aspect ratio).

*       Both SVROUTL/ISVROTL support linking to the entire document as
well as linking to ranges (partial selections). Both CNTROUTL/ICNTROTL
support linking to embeddings.

*       SVROUTL/ISVROTL/CNTROUTL/ICNTROTL operate as both a drag source
and a drop target. The top border and the bottom border of the selection
are used as the drag handles to start a drag. An arrow cursor is displayed
when the mouse hovers over a drag handle region. A plus cursor is displayed
when then next click start a new selection of lines.

*       ICNTROTL can operate both as an inside-out style in-place container
as well as an outside-in style container. This mode can be changed at runtime
with a command on the "DbgCntr" menu.

*       ISVROTL is a inside-out style in-place server. It will behave as
an outside-in style in-place server when used by an outside-in style
container.

*       Different levels of debug output messages can be enabled in
the OUTLINE sample applications. This is controlled at runtime with the
"Debug Level..." command in the "Dbg*" menu. The following values
of debug level produce the following debugging information:
	0 -- no debug messages (default)
	1 -- print message when a Document or Application is destroyed
	2 -- trace every OLE API and method call.
	3 -- give more OUTLINE app specific context to the OLE calls
	4 -- track all memory reference counts (AddRef/Release)
