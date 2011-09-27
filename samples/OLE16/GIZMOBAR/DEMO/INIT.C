/*
 * INIT.C
 * GizmoBar Test Version 1.00, March 1993
 *
 * Initialization code for the GizmoBar demonstration
 *
 * Copyright (c)1992 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */



#define STRICT
#include <windows.h>
#include "gbdemo.h"




/*
 * AppPAllocate
 *
 * Purpose:
 *  Initializes application data structures, registers window classes,
 *  and creates the main window.
 *
 * Parameters:
 *  pfSuccess       PINT in which a flag indicates the outcome of the function.
 *
 *  Other parameters are from WinMain
 *
 * Return Value:
 *  PAPPVARS        If NULL returned then AppPAllocate could not allocate
 *                  memory.  If a non-NULL pointer is returned with
 *                  *pfSuccess, then call AppPFree immediately.  If you
 *                  get a non-NULL pointer and *pfSuccess==TRUE then the
 *                  function succeeded.
 */

PAPPVARS PASCAL AppPAllocate(PINT pfSuccess, HINSTANCE hInst, HINSTANCE hInstPrev
	, LPSTR pszCmdLine, int nCmdShow)
	{
	PAPPVARS    pAV;
	WNDCLASS    wc;
	RECT        rc;
	UINT        uID;

	if (NULL==pfSuccess)
		return NULL;

	*pfSuccess=FALSE;

	//Allocate the structure
	pAV=(PAPPVARS)LocalAlloc(LPTR, CBAPPVARS);

	if (NULL==pAV)
		return NULL;

	//Initialize WinMain parameter holders.
	pAV->hInst     =hInst;
	pAV->hInstPrev =hInstPrev;
	pAV->pszCmdLine=pszCmdLine;
	pAV->nCmdShow  =nCmdShow;

	pAV->wID=200;

	//Register our window classes.
	if (!hInstPrev)
		{
		wc.style          = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc    = GBDemoWndProc;
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = sizeof(PAPPVARS);
		wc.hInstance      = hInst;
		wc.hIcon          = LoadIcon(hInst, "Icon");
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH)(COLOR_APPWORKSPACE + 1);
		wc.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU);
		wc.lpszClassName  = "GBDEMO";

		if (!RegisterClass(&wc))
			return pAV;

		wc.lpfnWndProc    = GBClientWndProc;
		wc.cbWndExtra     = 0;
		wc.hIcon          = NULL;
		wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName   = NULL;
		wc.lpszClassName  = "GBCLIENT";

		if (!RegisterClass(&wc))
			return pAV;
		}

	//Create the main window.
	pAV->hWnd=CreateWindow("GBDEMO", "GizmoBar Demonstration"
		, WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW,35, 35, 640, 180
		, NULL, NULL, hInst, pAV);


	if (NULL==pAV->hWnd)
		return pAV;

	//Get BTTNCUR's display information
	UIToolConfigureForDisplay(&pAV->tdd);

	//Get our image bitmaps for the display type we're on
	if (72==pAV->tdd.uDPI)  uID=IDR_BITMAPS72;
	if (96==pAV->tdd.uDPI)  uID=IDR_BITMAPS96;
	if (120==pAV->tdd.uDPI) uID=IDR_BITMAPS120;

	pAV->hBmp=LoadBitmap(hInst, MAKEINTRESOURCE(uID));

	if (NULL==pAV->hBmp)
		return FALSE;

	//Create the GizmoBar and the client area window
	GetClientRect(pAV->hWnd, &rc);
	pAV->hWndGB=CreateWindow(CLASS_GIZMOBAR, "Wooley", WS_CHILD | WS_VISIBLE
		, 0, 0, rc.right-rc.left, pAV->tdd.cyBar, pAV->hWnd
		, (HMENU)ID_GIZMOBAR, hInst, 0L);

	if (NULL==pAV->hWndGB)
		return pAV;


	pAV->hWndC=CreateWindow("GBCLIENT", "llamma", WS_CHILD | WS_VISIBLE
		, 0, pAV->tdd.cyBar, rc.right-rc.left, rc.bottom-rc.top-pAV->tdd.cyBar
		, pAV->hWnd, (HMENU)ID_CLIENTAREA, hInst, 0L);

	if (NULL==pAV->hWndC)
		return pAV;


	//Set the GizmoBar's associate to be this client window
	GBHwndAssociateSet(pAV->hWndGB, pAV->hWndC);

	ShowWindow(pAV->hWnd, nCmdShow);
	UpdateWindow(pAV->hWnd);

	*pfSuccess=TRUE;
	return pAV;
	}






/*
 * AppPFree
 *
 * Purpose:
 *  Reverses all initialization done by AppPAllocate, cleaning up
 *  any allocations including the application structure itself.
 *
 * Parameters:
 *  pAV             PAPPVARS to the application structure
 *
 * Return Value:
 *  PAPPVARS        NULL if successful, pAV if not, meaning we couldn't
 *                  free some allocation.
 */

PAPPVARS PASCAL AppPFree(PAPPVARS pAV)
	{
	if (NULL==pAV)
		return NULL;

	if (NULL!=pAV->hFont && IsGDIObject(pAV->hFont))
		DeleteObject(pAV->hFont);

	if (IsWindow(pAV->hWndC))
		DestroyWindow(pAV->hWndC);

	if (IsWindow(pAV->hWndGB))
		DestroyWindow(pAV->hWndGB);

	if (IsWindow(pAV->hWnd))
		DestroyWindow(pAV->hWnd);

	if (NULL!=pAV->hBmp)
		DeleteObject(pAV->hBmp);


	return (PAPPVARS)LocalFree((HLOCAL)pAV);
	}
