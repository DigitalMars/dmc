/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/**************************************************************************
 * MPLAY.C - Movie Player App using MCIWnd window class
 *
 *************************************************************************/

#define  STRICT
#include <windows.h>
#include <mmsystem.h>
#include <vfw.h>

#include "mplay.h"

//----------------------------------------------------------------------------

#define BUFSIZE 260

//----------------------------------------------------------------------------

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
    HWND          hwndApp;
    MSG           msg;
    WORD          wVer;
    static char   szBuffer[BUFSIZE];

    /* first let's make sure we are running on 1.1 */
    wVer = HIWORD(VideoForWindowsVersion());
    if (wVer < 0x010a) {
    	char szTitle[BUFSIZE];
    	char szMessage[BUFSIZE];

        LoadString( hInst, IDS_APPERR, szTitle, BUFSIZE );
    	LoadString( hInst, IDS_OLDVFW, szMessage, BUFSIZE );
    	/* oops, we are too old, blow out of here */
    	MessageBeep(MB_ICONHAND);
    	MessageBox(NULL, szMessage, szTitle, MB_OK|MB_ICONSTOP);
    	return FALSE;
    }

    /* create the window */
    hwndApp = MCIWndCreate(NULL,
                           hInst,
	                   MCIWNDF_SHOWNAME            |
	                   MCIWNDF_SHOWMODE            |
	                   WS_OVERLAPPEDWINDOW         |
	                   WS_VISIBLE,
	                   NULL);

    if (hwndApp == NULL)
    	return -1;

    LoadString( hInst, IDS_DIALOGNAME, szBuffer, BUFSIZE );
    SetWindowText(hwndApp, szBuffer);

    //Polling messages from event queue
    while (GetMessage(&msg,NULL,0,0)) {

        TranslateMessage(&msg);
    	DispatchMessage(&msg);

    	if (!IsWindow(hwndApp))
    	    PostQuitMessage(0);
    }

    return msg.wParam;
}
