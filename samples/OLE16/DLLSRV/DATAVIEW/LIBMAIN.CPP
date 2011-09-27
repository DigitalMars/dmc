/*
	libmain.cpp

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#include "dataview.h"
#include <ole2ui.h>

// This line is needed for the debug utilities in OLE2UI
extern "C" {
	OLEDBGDATA_MAIN("DATAVIEW")
}

HINSTANCE vDV_DLLhinst;

WORD cfRealtimeItem;
#ifdef __TURBOC__
extern "C"
int PASCAL FAR LibMain
(
	HINSTANCE   hinst,
	WORD        segDS,
	WORD        cbHeapSize,
	LPSTR       lpCmdLine
)
#else
extern "C"
BOOL PASCAL FAR LibMain
(
	HINSTANCE   hinst,
	HANDLE      segDS,
	UINT        cbHeapSize,
	LPSTR       lpCmdLine
)
#endif
{
	vDV_DLLhinst = hinst;

	UNREFERENCED(hinst);
	UNREFERENCED(cbHeapSize);
	UNREFERENCED(segDS);

  // Leave our DS unlocked when we're not running
  UnlockData( 0 );
  cfRealtimeItem = RegisterClipboardFormat("RealtimeItem");

#if STATIC_OLE2UI == 1
	/* Initialization required for OLE 2 UI library.  This call is
	**    needed ONLY if we are using the static link version of the UI
	**    library. If we are using the DLL version, we should NOT call
	**    this function in our application.
	*/
	if (!OleUIInitialize(hinst, NULL))
		{
		OleDbgOut("Could not initialize OLEUI library\n");
		return FALSE;
		}
#else
	/* OLE2NOTE: if we use the OLE2UI library in DLL form then we must
	**    call OleUILockLibrary(TRUE) during our LibMain processing and
	**    call OleUILockLibrary(FALSE) in our WEP. this is done INSTEAD of
	**    calling OleUICanUnloadNow from our DllCanUnloadNow function.
	**    the idea here is that the OLE2UI DLL would be held locked for
	**    the duration that our DLL is in use. after our DLL is
	**    unloaded then the OLE2UI DLL can be unloaded if it is not
	**    being used (eg. all object instances created by the OLE2UI
	**    DLL have been released). if, for an example, an
	**    OleStdEnumFmtEtc object had not been released, then the
	**    OLE2UI DLL would remain loaded even after our DLL was
	**    unloaded. this is achieved because the OleUILockLibrary calls
	**    the OLE API "CoLoadLibrary" which tells OLE to manage the
	**    library.
	**
	**    if the OLE2UI library had been used as a static library,
	**    then we need to include a check whether the OLE2UI code is
	**    able to be unloaded inside our own DllCanUnloadNow function.
	**    this means if the OLE2UI code, for example, has passed out a
	**    pointer to a OleStdEnumFmtEtc object that has not yet been
	**    released, then the OLE2UI code may not be unloaded. however,
	**    because OLE2UI code is statically linked with our DLL, this
	**    really means OUR DLL can not be unloaded. Thus it is
	**    necessary to call OleUICanUnloadNow from within our own
	**    DllCanUnloadNow function.
	*/
	OleUILockLibrary(TRUE);
#endif

  return TRUE;
}

extern "C" int FAR PASCAL _WEP(int)
{

	/* Your WEP functionality goes here */

#if STATIC_OLE2UI == 1
	// De-initialization for UI libraries.  Just like OleUIInitialize, this
	// funciton is needed ONLY if we are using the static link version of the
	// OLE UI library.
	OleUIUninitialize();
#else
	/* OLE2NOTE: unlock the OLE2UI DLL to allow it to be unloaded if it
	**    is not needed any more. after our DLL is
	**    unloaded then the OLE2UI DLL can be unloaded if it is not
	**    being used (eg. there are not object instances created by the
	**    DLL existing). if, for an example, an OleStdEnumFmtEtc object
	**    had not been released, then the OLE2UI DLL would remain
	**    loaded even after our DLL was unloaded. this is achieved
	**    because the OleUILockLibrary calls the OLE API
	**    "CoLoadLibrary" which tells OLE to manage the library.
	*/
	OleUILockLibrary(FALSE);
#endif

	return 1;
}
