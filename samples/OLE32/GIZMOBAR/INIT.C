/*
 * INIT.C
 * GizmoBar Version 1.01
 *
 * LibMain entry point and initialization code for the GizmoBar
 * DLL that is likely to be used once or very infrequently.
 *
 * Copyright (c)1993-1995 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */


#include <windows.h>
#include "gizmoint.h"


/*
 * LibMain(32)
 *
 * Purpose:
 *  Entry point conditionally compiled for Windows NT and Windows
 *  3.1.  Provides the proper structure for each environment.
 */

#ifdef WIN32

BOOL WINAPI DllMain(HINSTANCE hInstance, ULONG ulReason
    , PCONTEXT pContext)
    {
    UNREFERENCED_PARAMETER(pContext);

    if (DLL_PROCESS_ATTACH==ulReason)
        return FRegisterControl(hInstance);

    return TRUE;
    }

#else

int WINAPI LibMain(HANDLE hInstance, WORD wDataSeg
    , WORD cbHeapSize, LPTSTR lpCmdLine)
    {
     //Perform global initialization.
    if (FRegisterControl(hInstance))
        {
        if (0!=cbHeapSize)
            UnlockData(0);
        }

    return (int)hInstance;
    }

#endif



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
        wc.style         =CS_DBLCLKS | CS_GLOBALCLASS
                          | CS_VREDRAW | CS_HREDRAW;

        fRegistered=RegisterClass(&wc);
        }

    return fRegistered;
    }






/*
 * GizmoBarPAllocate
 *
 * Purpose:
 *  Allocates and initializes the control's primary data structure
 *  for each window that gets created.
 *
 * Parameters:
 *  pfSuccess       int * indicating success of the function.
 *  hWnd            HWND that is tied to this structure.
 *  hInst           HINSTANCE of the DLL.
 *  hWndAssociate   HWND to which we send messages.
 *  dwStyle         DWORD initial style.
 *  uState          UINT initial state.
 *  uID             UINT identifier for this window.
 *
 * Return Value:
 *  PGIZMOBAR       If NULL returned then GizmoBarPAllocate could not
 *                  allocate memory.  If a non-NULL pointer is
 *                  returned with *pfSuccess, then call GizmoBarPFree
 *                  immediately.  If you get a non-NULL pointer and
 *                  *pfSuccess==TRUE then the function succeeded.
 */

PGIZMOBAR GizmoBarPAllocate(int *pfSuccess, HWND hWnd
    , HINSTANCE hInst, HWND hWndAssociate, DWORD dwStyle
    , UINT uState , UINT uID)
    {
    PGIZMOBAR     pGB;

    if (NULL==pfSuccess)
        return NULL;

    *pfSuccess=FALSE;

    //Allocate the structure
    pGB=(PGIZMOBAR)(void *)LocalAlloc(LPTR, CBGIZMOBAR);

    if (NULL==pGB)
        return NULL;

    //Initialize LibMain parameter holders.
    pGB->hWnd         =hWnd;
    pGB->hInst        =hInst;
    pGB->hWndAssociate=hWndAssociate;
    pGB->dwStyle      =dwStyle;
    pGB->uState       =uState;
    pGB->uID          =uID;
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
 *  Reverses all initialization done by GizmoBarPAllocate, cleaning
 *  up any allocations including the application structure itself.
 *
 * Parameters:
 *  pGB             PGIZMOBAR to the control's structure
 *
 * Return Value:
 *  PGIZMOBAR       NULL if successful, pGB if not, meaning we
 *                  couldn't free some allocation.
 */

PGIZMOBAR GizmoBarPFree(PGIZMOBAR pGB)
    {
    if (NULL==pGB)
        return NULL;

    /*
     * Free all the gizmos we own.  When we call GizmoPFree we always
     * free the first one in the list which updates pGB->pGizmos for
     * us, so we just have to keep going until pGizmos is NULL,
     * meaning we're at the end of the list.
     */
    while (NULL!=pGB->pGizmos)
        GizmoPFree(&pGB->pGizmos, pGB->pGizmos);

    if (NULL!=pGB->hBrFace)
        DeleteObject(pGB->hBrFace);

    /*
     * Notice that since we never create a font, we aren't
     * responsible for our hFont member.
     */

    return (PGIZMOBAR)(void *)LocalFree((HLOCAL)(UINT)(LONG)pGB);
    }
