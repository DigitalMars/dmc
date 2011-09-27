/*
 * GBDEMO.H
 * GizmoBar Test Version 1.00, March 1993
 *
 * Definitions, structures, types, and function prototypes.
 *
 * Copyright (c)1992 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */



#ifndef _GBDEMO_H_
#define _GBDEMO_H_


#include "..\..\include\bttncur.h"
#include "..\gizmobar.h"
#include "..\..\include\win1632.h"



/*
 * Resource identifiers
 */

//Menus & menu commands
#define IDR_MENU                            10
#define IDR_BITMAPS72                       11
#define IDR_BITMAPS96                       12
#define IDR_BITMAPS120                      13

#define IDM_GIZMOSCREATE                    100
#define IDM_GIZMOSDISABLEINDIVIDUALLY       101
#define IDM_GIZMOSENABLEINDIVIDUALLY        102
#define IDM_GIZMOSDISABLEALL                103
#define IDM_GIZMOSENABLEALL                 104

#define IDM_GIZMOSSHOWHIDELISTBOX           105
#define IDM_GIZMOSSENDMESSAGES              106
#define IDM_GIZMOSCHANGEFONT                107

#define IDM_GIZMOSNOTIFYON                  108
#define IDM_GIZMOSNOTIFYOFF                 109
#define IDM_GIZMOSDATASET                   110
#define IDM_GIZMOSDATAGET                   111
#define IDM_GIZMOSSETGETTEXT                112
#define IDM_GIZMOSSETGETINT                 113


//IDs of the GizmoBar and the Client area window
#define ID_GIZMOBAR                         2000
#define ID_CLIENTAREA                       2001


//IDs of the Gizmos we'll create in the GizmoBar we test.
#define GIZMOID_MIN                         1000
#define GIZMOID_TEXT                        1000
#define GIZMOID_LISTBOX                     1001
#define GIZMOID_EDIT                        1002
#define GIZMOID_COMBOBOX                    1003
#define GIZMOID_SEPARATOR1                  1004
#define GIZMOID_BUTTON1                     1005
#define GIZMOID_BUTTON2                     1006
#define GIZMOID_BUTTON3                     1007
#define GIZMOID_BUTTON4                     1008
#define GIZMOID_SEPARATOR2                  1009
#define GIZMOID_BUTTON5                     1010
#define GIZMOID_BUTTON6                     1011
#define GIZMOID_BUTTON7                     1012
#define GIZMOID_SEPARATOR3                  1013
#define GIZMOID_BUTTON8                     1014
#define GIZMOID_BUTTON9                     1015
#define GIZMOID_BUTTON10                    1016
#define GIZMOID_BUTTON11                    1017
#define GIZMOID_MAX                         1017




/*
 * Application-defined types.
 */

//Application-wide Variables
typedef struct
	{
	HINSTANCE   hInst;              //WinMain parameters
	HINSTANCE   hInstPrev;
	LPSTR       pszCmdLine;
	int         nCmdShow;

	HWND        hWnd;               //Main window handle
	HWND        hWndGB;             //GizmoBar handle
	HWND        hWndC;              //Client area handle

	HBITMAP     hBmp;               //Images
	HFONT       hFont;              //Font currently used in Gizmobar.

	UINT        iGizmo;             //Current gizmo index to add.
	WORD        wID;

	TOOLDISPLAYDATA tdd;            //From UIToolConfigureForDisplay
	} APPVARS;

typedef APPVARS *PAPPVARS;
#define CBAPPVARS sizeof(APPVARS)




/*
 * Function prototypes.
 */

//INIT.C
PAPPVARS     PASCAL AppPAllocate(PINT, HINSTANCE, HINSTANCE, LPSTR, int);
PAPPVARS     PASCAL AppPFree(PAPPVARS);


//GBDEMO.C
LRESULT FAR PASCAL GBDemoWndProc(HWND, UINT, WPARAM, LONG);
LRESULT FAR PASCAL GBClientWndProc(HWND, UINT, WPARAM, LONG);


#endif  //_GBDEMO_H_
