/*
 * INIT.C
 * GizmoBar Version 1.00, March 1993
 *
 * LibMain entry point and initialization code for the GizmoBar
 * DLL that is likely to be used once or very infrequently.
 *
 * Copyright (c)1992 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */

#ifdef WIN32
#define _INC_OLE
#define __RPC_H__
#endif


#define STRICT
#include <windows.h>
#include "gizmoint.h"



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
void FAR PASCAL WEP(int);

extern  BOOL __stdcall LibMain
(
	HINSTANCE hInstance,
	ULONG     Reason,
	PCONTEXT  Context
)
{
	OutputDebugString("gizmobar LibMain: gizmobar.dll loaded\r\n");

	UNREFERENCED_PARAMETER(Context);

	if (Reason == DLL_PROCESS_DETACH) {

		_cexit(); // BUGBUG32  replace with call to CRT_INIT when it's fixed

		WEP(0);
		return TRUE;
	}
	else if (Reason != DLL_PROCESS_ATTACH)
		return TRUE;

	if (!_CRT_INIT(hInstance,Reason,NULL))
		 return FALSE;

	 return FRegisterControl(hInstance);

}

#else

HANDLE FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSeg
	, WORD cbHeapSize, LPSTR lpCmdLine)
	{
	//Go register the control
	if (FRegisterControl(hInst))
		{
		if (0!=cbHeapSize)
			UnlockData(0);

		return hInst;
		}

	return (HANDLE)NULL;
	}


#endif


/*
 * WEP
 *
 * Purpose:
 *  Required DLL Exit function.  Does nothing.
 *
 * Parameters:
 *  bSystemExit     BOOL indicating if the system is being shut
 *                  down or the DLL has just been unloaded.
 *
 * Return Value:
 *  void
 *
 */

void FAR PASCAL WEP(int bSystemExit)
	{
	return;
	}






/*
 * FRegisterControl
 *
 * Purpose:
 *  Registers the GizmoBar control class, including CS_GLOBALCLASS
 *  to make the control available to all applications in the system.
 *
 * Parameters:
 *  hInst           HINSTANCE of the DLL that will own this class.
 *
 * Return Value:
 *  BOOL            TRUE if the class is registered, FALSE otherwise.
 */

BOOL FRegisterControl(HINSTANCE hInst)
	{
	static BOOL     fRegistered=FALSE;
	WNDCLASS        wc;

	if (!fRegistered)
		{
		wc.lpfnWndProc   =GizmoBarWndProc;
		wc.cbClsExtra    =0;
		wc.cbWndExtra    =CBWINDOWEXTRA;
		wc.hInstance     =hInst;
		wc.hIcon         =NULL;
		wc.hCursor       =LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground =(HBRUSH)(COLOR_BTNFACE+1);
		wc.lpszMenuName  =NULL;
		wc.lpszClassName =CLASS_GIZMOBAR;
		wc.style         =CS_DBLCLKS | CS_GLOBALCLASS | CS_VREDRAW | CS_HREDRAW;

		fRegistered=RegisterClass(&wc);
		}

	return fRegistered;
	}






/*
 * GizmoBarPAllocate
 *
 * Purpose:
 *  Allocates and initializes the control's primary data structure for
 *  each window that gets created.
 *
 * Parameters:
 *  pfSuccess       LPINT indicating success of the function.
 *  hWnd            HWND that is tied to this structure.
 *  hInst           HINSTANCE of the DLL.
 *  hWndAssociate   HWND to which we send messages.
 *  dwStyle         DWORD initial style.
 *  uState          UINT initial state.
 *
 *
 * Return Value:
 *  PGIZMOBAR       If NULL returned then GizmoBarPAllocate could not allocate
 *                  memory.  If a non-NULL pointer is returned with
 *                  *pfSuccess, then call GizmoBarPFree immediately.  If you
 *                  get a non-NULL pointer and *pfSuccess==TRUE then the
 *                  function succeeded.
 */

PGIZMOBAR GizmoBarPAllocate(LPINT pfSuccess, HWND hWnd, HINSTANCE hInst
	, HWND hWndAssociate, DWORD dwStyle, UINT uState)
	{
	PGIZMOBAR     pGB;

	if (NULL==pfSuccess)
		return NULL;

	*pfSuccess=FALSE;

	//Allocate the structure
	pGB=(PGIZMOBAR)(char *)LocalAlloc(LPTR, CBGIZMOBAR);

	if (NULL==pGB)
		return NULL;

	//Initialize LibMain parameter holders.
	pGB->hWnd         =hWnd;
	pGB->hInst        =hInst;
	pGB->hWndAssociate=hWndAssociate;
	pGB->dwStyle      =dwStyle;
	pGB->uState       =uState;
	pGB->fEnabled     =TRUE;

	pGB->crFace=GetSysColor(COLOR_BTNFACE);
	pGB->hBrFace=CreateSolidBrush(pGB->crFace);

	if (NULL==pGB->hBrFace)
		return pGB;

	pGB->hFont=GetStockObject(SYSTEM_FONT);

	*pfSuccess=TRUE;
	return pGB;
	}




/*
 * GizmoBarPFree
 *
 * Purpose:
 *  Reverses all initialization done by GizmoBarPAllocate, cleaning up
 *  any allocations including the application structure itself.
 *
 * Parameters:
 *  pGB           PGIZMOBAR to the control's structure
 *
 * Return Value:
 *  PGIZMOBAR       NULL if successful, pGB if not, meaning we couldn't
 *                  free some allocation.
 */

PGIZMOBAR GizmoBarPFree(PGIZMOBAR pGB)
	{
	if (NULL==pGB)
		return NULL;

	/*
	 * Free all the gizmos we own.  When we call GizmoPFree we always
	 * free the first one in the list which updates pGB->pGizmos for
	 * us, so we just have to keep going until pGizmos is NULL, meaning
	 * we're at the end of the list.
	 */
	while (NULL!=pGB->pGizmos)
		GizmoPFree(&pGB->pGizmos, pGB->pGizmos);

	if (NULL!=pGB->hBrFace)
		DeleteObject(pGB->hBrFace);

	/*
	 * Notice that since we never create a font, we aren't responsible
	 * for our hFont member.
	 */

	return (PGIZMOBAR)LocalFree((HLOCAL)(UINT)(LONG)pGB);
	}
