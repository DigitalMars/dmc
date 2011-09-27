Ole2View Interface Viewer DLLs

INTRODUCTION

Ole2View is extensible in that it can call functions in
DLLs to display interface information.  These DLLs are
called Ole2View Interface Viewers, and typically have
file extensions of .O2V.

Ole2View comes with one Interface Viewer DLL (DEFO2V.O2V) which
contains interface viewers for IDataObject and IDispatch. The
source code is included to provide an example of how to implement
an Interface Viewer for Ole2View, and to provide sample code
for both the IDataObject and IDispatch interfaces.

THE "DisplayInterface" FUNCTIONS

An Ole2View Interface Viewer is a single function exported
from any DLL prototyped as:

	HRESULT WINAPI _export
	DisplayInterface
	(
		HWND        hwndParent, // Ole2View's main frame wnd
		LPUNKNOWN   lpunk,      // Pointer to valid interface
		LPIID       lpiid,      // pointer to interface id
		LPSTR       lpszName    // Interface name
	} ;

Your DLL should export the DisplayInterface functions by
name.

For example, DEFO2V.O2V includes DisplayInterface functions
for both IDataObject and IDispatch (DisplayIDataObject and
DisplayIDispatch respectively).   They are prototyped within
the DEFO2V source code as:

	HRESULT WINAPI _export
	DisplayIDataObject( HWND hwndParent, LPDATAOBJECT lpDO,
						LPIID lpiid, LPSTR lpszName ) ;

	HRESULT WINAPI _export
	DisplayIDispatch( HWND hwndParent, LPDISPATCH lpDisp,
					  LPIID lpiid, LPSTR lpszName ) ;

and are included in the EXPORTS section of the DEFO2V.DEF file.

You can do whatevery you wish within a DisplayInterface
function, as long as you follow standard Windows DLL
programming rules.   The information passed to the DisplayInterface
functions should be enough to allow you to do just about
anything.

THE [Interface Viewers] SECTION OF OLE2VIEW.INI FILE

Ole2View looks in the [Interface Viewers] section of the
OLE2VIEW.INI file to determine where to find installed
interface viewers.

The format of an entry in the [Interface Viewers] section
is:

<interface id>=<pathname of dll>, <displayinterface function name>

For example:

[Interface Viewers]
{0000010E-0000-0000-C000-000000000046}=defo2v.dll,DisplayIDataObject
{00020400-0000-0000-C000-000000000046}=defo2v.dll,DisplayIDispatch

If you wanted an "IOleContainer" interface viewer you would create
a DLL named, say, "contain.o2v" and export the "DisplayIOleContainer"
function from it.   Then the follwing line in the OLE2VIEW.INI
file would enable Ole2View to find it.

{0000011B-0000-0000-C000-000000000046}=contain.o2v,DisplayIOleContainer

Ole2View knows internally about the interface viewers in DEFO2V.O2V.
It is not necessary to have entries for them in OLE2VIEW.INI.  However,
you can override the default entries if you wish.
