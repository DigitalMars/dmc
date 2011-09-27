/*
 * BCDEMO.H
 *
 * Definitions, structures, types, and function prototypes.
 *
 * Copyright (c)1992-1993 Microsoft Corporation, All Right Reserved,
 * as applied to redistribution of this source code in source form
 * License is granted to use of compiled code in shipped binaries.
 */


/*
 * Resource identifiers
 */

//Menus & menu commands
#define IDR_MENU                            1
#define IDM_NORMALCURSOR                    100


//Bitmaps
#define IDB_BUTTONS72                       2
#define IDB_BUTTONS96                       3
#define IDB_BUTTONS120                      4


/*
 * Application-defined types.
 */

//Application-wide Variables
typedef struct
	{
	HINSTANCE       hInst;              //WinMain parameters
	HINSTANCE       hInstPrev;
	LPSTR           pszCmdLine;
	int             nCmdShow;

	HWND            hWnd;               //Main window handle

	TOOLDISPLAYDATA tdd;
	UINT            idrBmp;
	} APPVARS;

typedef APPVARS *PAPPVARS;
#define CBAPPVARS sizeof(APPVARS)




/*
 * Function prototypes.
 */

//INIT.C
PAPPVARS     PASCAL AppPAllocate(PINT, HINSTANCE, HINSTANCE, LPSTR, int);
PAPPVARS     PASCAL AppPFree(PAPPVARS);


//BCTEST.C
LONG     FAR PASCAL BCDemoWndProc(HWND, UINT, WPARAM, LONG);
