/*
 * INIT.C
 *
 * Initialization code for Button Images and Cursors demonstration
 *
 * Copyright (c)1992-1993 Microsoft Corporation, All Right Reserved,
 * as applied to redistribution of this source code in source form
 * License is granted to use of compiled code in shipped binaries.
 */

#define STRICT
#include <windows.h>
#include "..\bttncur.h"
#include "bcdemo.h"




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

PAPPVARS PASCAL AppPAllocate(PINT pfSuccess, HINSTANCE hInst
	, HINSTANCE hInstPrev, LPSTR pszCmdLine, int nCmdShow)
	{
	PAPPVARS    pAV;
	WNDCLASS    wc;

	if (NULL==pfSuccess)
		return NULL;

	*pfSuccess=FALSE;

	//Allocate the structure
	pAV=(PAPPVARS)LocalAlloc(LPTR, CBAPPVARS);

	if (NULL==pAV)
		return NULL;

	//Initialize WinMain parameter holders.
	pAV->hInst=hInst;
	pAV->hInstPrev=hInstPrev;
	pAV->pszCmdLine=pszCmdLine;
	pAV->nCmdShow=nCmdShow;

	//Register our window classes.
	if (!hInstPrev)
		{
		wc.style          = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc    = BCDemoWndProc;
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = sizeof(PAPPVARS);
		wc.hInstance      = hInst;
		wc.hIcon          = LoadIcon(hInst, "Icon");
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU);
		wc.lpszClassName  = "BCDEMO";

		if (!RegisterClass(&wc))
			return pAV;
		}

	//Create the main window.
	pAV->hWnd=CreateWindow("BCDEMO", "Buttons & Cursors", WS_OVERLAPPEDWINDOW
		,35, 35, 500, 280, NULL, NULL, hInst, pAV);

	if (NULL==pAV->hWnd)
		return pAV;

	UIToolConfigureForDisplay(&pAV->tdd);

	switch (pAV->tdd.uDPI)
		{
		case 72:
			pAV->idrBmp=IDB_BUTTONS72;
			break;

		case 96:
			pAV->idrBmp=IDB_BUTTONS96;
			break;

		case 120:
			pAV->idrBmp=IDB_BUTTONS120;
			break;
		}

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

	if (IsWindow(pAV->hWnd))
		DestroyWindow(pAV->hWnd);

	return (PAPPVARS)LocalFree((HLOCAL)pAV);
	}
