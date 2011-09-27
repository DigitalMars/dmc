/*
 * DLLFUNCS.C
 *
 * Contains entry and exit points for the DLL implementation
 * of the OLE 2.0 User Interface Support Library.
 *
 * This file is not needed if we are linking the static library
 * version of this library.
 *
 * Copyright (c)1992 Microsoft Corporation, All Right Reserved
 */

#define STRICT  1
#include "ole2ui.h"
#include "common.h"
#include "uiclass.h"

OLEDBGDATA


/*
 * LibMain
 *
 * Purpose:
 *  DLL-specific entry point called from LibEntry.  Initializes
 *  the DLL's heap and registers the GizmoBar GizmoBar.
 *
 * Parameters:
 *  hInst           HINSTANCE instance of the DLL.
 *  wDataSeg        WORD segment selector of the DLL's data segment.
 *  wHeapSize       WORD byte count of the heap.
 *  lpCmdLine       LPSTR to command line used to start the module.
 *
 * Return Value:
 *  HANDLE          Instance handle of the DLL.
 *
 */

#ifdef WIN32

extern  BOOL WINAPI _CRT_INIT(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved);
extern  _cexit(void);
int CALLBACK WEP(int);

extern  BOOL __stdcall LibMain
(
	HINSTANCE hInst,
	ULONG Reason,
	PCONTEXT Context
)
{
	UNREFERENCED_PARAMETER(Context);
	if (Reason == DLL_PROCESS_DETACH) {

	_cexit(); // BUGBUG32  replace with call to CRT_INIT when it's fixed

	WEP(0);
	return TRUE;
	}
	else if (Reason != DLL_PROCESS_ATTACH)
	return TRUE;

	if (!_CRT_INIT(hInst,Reason,NULL))
	 return FALSE;

	// Initialize OLE UI libraries.  If you're linking with the static LIB
	// version of this library, you need to make a call to OleUIInitialize
	// explicitly in your application (because this LibMain won't be executed)
	OleUIInitialize(hInst, (HINSTANCE)0);
	return TRUE;

}

#else

int FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSeg
			  , WORD cbHeapSize, LPSTR lpCmdLine)
	{
	OleDbgOut2("LibMain: OLE2UI.DLL loaded\r\n");

	// Initialize OLE UI libraries.  If you're linking with the static LIB version
	// of this library, you need to make the below call in your application (because
	// this LibMain won't be executed).

	// The symbol LIBNAME is defined on the compiler command line

	OleUIInitialize(hInst, (HINSTANCE)0);

	//All done...
	if (0!=cbHeapSize)
	UnlockData(0);

	return (int)hInst;
	}

#endif

/*
 * WEP
 *
 * Purpose:
 *  Required DLL Exit function.
 *
 * Parameters:
 *  bSystemExit     BOOL indicating if the system is being shut
 *                  down or the DLL has just been unloaded.
 *
 * Return Value:
 *  void
 *
 */
int CALLBACK EXPORT WEP(int bSystemExit)
{
	OleUIUnInitialize();
	return 0;
}


/*
 * DllCanUnloadNow
 *
 * Purpose:
 *   Determines when it is safe to go away.
 *
 * Return Value:
 *   HRESULT    NOERROR it is safe to go away, S_FALSE this code must stay
 *              loaded.
 *
 */
STDAPI DllCanUnloadNow(void)
{
	return OleUICanUnloadNow();
}



/*
 * OleUILockLibrary
 *
 *
 * Purpose:
 *   Increments & decrements a lock count to force this library to stay
 *   alive, all OLE 2.x DLL objects servers (ie. INPROC server DLL) which
 *   use the OLE2UI library in a DLL form must call this function passing TRUE
 *   in their InitInstance() routine and balance with a call, passing FALSE
 *   in their ExitInstance(). This is done so as to keep DllCanUnloadNow()
 *   from returning NOERROR until the OLE 2.x DLL server goes away.
 *   While the INPROC server DLL is loaded, it wants to force the OLE2UI
 *   DLL to remain loaded (ie. force it to return S_FALSE from the OLE2UI's
 *   DllCanUnloadNow function). It is possible that the INROC server DLL
 *   gets unloaded from a call to CoFreeUnusedLibraries, but it is still
 *   necessary for the OLE2UI library to remain in use (eg. because an
 *   enumerator returned from OleStdEnumFmtEtc_Create still exists). only
 *   after all explicit "OleUILockLibrary" locks and instances of objects
 *   created have been release will the DllCanUnloadNow function for the
 *   OLE2UI library DLL return NOERROR.
 *
 *   If the INPROC server DLL links to the OLE2UI library as a static
 *   library, then this OleUILockLibrary function should NOT be used.
 *   instead the INPROC server DLL should call OleUICanUnloadNow API from
 *   within its own DllCanUnloadNow function. The idea here is, that if there
 *   are any existing instance of objects created by the OLE2UI library
 *   functions (eg. EnumFORMATETC objects created by OleStdEnumFmtEtc_Create)
 *   then, the INPROC server DLL must NOT let itself be unloaded.
 *
 *   An EXE based object using the OLE2UI libray need NOT use either the
 *   OleUILockLibrary or OleUICanUnloadNow functions. All objects created
 *   by the OLE2UI library will have LRPC proxies and stubs created to
 *   manage remoting method calls. the worst that can happen when the EXE
 *   exits is that any outstanding proxies for unreleased objects will get
 *   RPC_E_SERVERDIED errors; they will not GPFault.
 *
 * Return Value:
 *   HRESULT    NOERROR it is safe to go away, S_FALSE this code must stay
 *              loaded.
 */

STDAPI OleUILockLibrary(BOOL fLock)
{
  HRESULT hRes = NOERROR;

  if (fLock == TRUE) {
	if (CoLoadLibrary(LIBNAME ".dll", TRUE) != NOERROR) {
	  g_dwObjectCount ++;

	}else{
	  hRes = ResultFromScode(CO_E_DLLNOTFOUND);
	}

  }else{
	g_dwObjectCount --;
  }

  return hRes;
}
