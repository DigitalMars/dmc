// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   tabctrl.c
//
//  PURPOSE:  Implements the window procedure for the main application
//            window.  
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    ChildWndProc - Processes messages for the child window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    MsgNotify    - Handle the WM_NOTIFY message
//    CmdDemo      - Handles the Demo.TabControl menuitem
//    CmdExit      - Handles the Demo.Exit menuitem
//    ChildMsgPaint- Handle the WM_PAINT case for the child window
//
//  COMMENTS:
//

#include <windows.h>          // required for all Windows applications
#include <commctrl.h>         // required for applications using Shell controls.
#include "globals.h"          // prototypes specific to this application
#include "resource.h"
#include "demo.h"


#define  NUMTABS 4

extern HWND hwndDisplayChild;

LRESULT MsgNotify(HWND, UINT, WPARAM, LPARAM);

// Child window message table definition.
MSD rgmsdChild[] =
{
    {WM_PAINT, ChildMsgPaint}
};

MSDI msdiChild =
{
    sizeof(rgmsdChild) / sizeof(MSD),
    rgmsdChild,
    edwpWindow
};

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_NOTIFY,  MsgNotify},
    {WM_DESTROY, MsgDestroy}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};

// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_DEMOTABCTRL, CmdDemo},
    {IDM_EXIT,        CmdExit},
    {IDM_ABOUT,       CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{

    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);

}

LRESULT CALLBACK ChildWndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{

    return DispMessage(&msdiChild, hwnd, uMessage, wparam, lparam);

}
//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: MsgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Handles the Notification message sent from the TAB CONTROL
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgNotify(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPNMHDR	lpnmhdr = (LPNMHDR)lparam; 
	USHORT  ctrlRetVal, shftRetVal;
	int    	tabIndex;

    if (lpnmhdr->code == TCN_SELCHANGE)
        InvalidateRect (hwndDisplayChild, NULL, TRUE);

	else if(lpnmhdr->code == TCN_KEYDOWN)
	{
		TC_KEYDOWN FAR * ptcKeydown = (TC_KEYDOWN FAR *)lparam;

		if(ptcKeydown->wVKey == VK_TAB)
		{
		 // MessageBeep(MB_OK);
			ctrlRetVal = GetKeyState(VK_CONTROL);
			shftRetVal = GetKeyState(VK_SHIFT);
			if((ctrlRetVal & 0x8000) && (shftRetVal & 0x8000))
			{
			   
			  // The ctrl Key and the shift key are down, 
			  // Move the focus to the previous tab in the tab control. 
			  // If the focus is on the first tab, move focus to the
			  // last tab. 

				tabIndex = (int) SendMessage(hwndTabControl, 
											 TCM_GETCURSEL,
											 (WPARAM)0,
											 (LPARAM)0);				
				if(tabIndex == -1)
				{
					MessageBox(NULL, "INVALID SELECTION - 1", "ERROR", MB_OK);
					return 0;
				}

				else if(tabIndex == 0)
					SendMessage(hwndTabControl, 
								TCM_SETCURSEL, 
								(WPARAM)(int)NUMTABS - 1, 
								(LPARAM)0);

				else
					SendMessage(hwndTabControl, 
								TCM_SETCURSEL, 
								(WPARAM)(int)tabIndex - 1, 
								(LPARAM)0);

				InvalidateRect (hwndDisplayChild, NULL, TRUE);
			}

			else if(ctrlRetVal & 0x8000)
			{ 
			 			
			 // The Ctrl key is down..when tab was pressed
			 // Move the focus to the next tab in the tab control. 
			 // If the focus is on the last tab, move focus to the
			 // first tab. 

			 tabIndex = (int) SendMessage(hwndTabControl, 
										 TCM_GETCURSEL,
			 							 (WPARAM)0,
										 (LPARAM)0);

			 if(tabIndex == -1)
				{
					MessageBox(NULL, "INVALID SELECTION - 1", "ERROR", MB_OK);
					return 0;
				}

				else if(tabIndex == NUMTABS - 1)
					SendMessage(hwndTabControl, 
								TCM_SETCURSEL, 
								(WPARAM)(int)0,
								(LPARAM)0);

 				else
					SendMessage(hwndTabControl, 
								TCM_SETCURSEL, 
								(WPARAM)(int)tabIndex + 1, 
								(LPARAM)0);


				InvalidateRect (hwndDisplayChild, NULL, TRUE);

			}



		} // End of if(ptcKeydown->wVKey if statement
 
	}

    // Processes tooltip notification
    else if (lpnmhdr->code == TTN_NEEDTEXT)
    {
	    
        LPTOOLTIPTEXT   lpToolTipText;
        char szBuffer [80];

        lpToolTipText = (LPTOOLTIPTEXT)lparam;
        LoadString(hInst,
                   IDS_STARTDESC+lpToolTipText->hdr.idFrom,
                   szBuffer,
                   sizeof(szBuffer));
        lpToolTipText->lpszText = szBuffer;



    }
    return 0;
}


//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: CmdDemo(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Demo" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_DEMOTABCTRL     (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_DEMOTABCTRL message, call CreateDialog which
//    will create the box according to the information in your resource
//    script file and turn control over to the Demo() function.
//
//

LRESULT CmdDemo(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HMENU hMenu;
   
    hMenu = GetMenu(hwnd);
    if(DemoInit(hwnd))
        EnableMenuItem(hMenu, 
                       IDM_DEMOTABCTRL, 
                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    else
        DestroyWindow(hwnd);



    return 0;
}


//
//  FUNCTION: ChildMsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes WM_PAINT sent to display child window
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT ChildMsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT rect;
    HDC  hDC, hDC2;
    BITMAP  bmp;
    int   iCurSel;
    HBITMAP  hBitmap, hOldBitmap;
    PAINTSTRUCT ps;

 
    hDC = BeginPaint (hwnd, &ps);

    // Get child window Rect
    GetClientRect (hwndDisplayChild, &rect);

    // Which tab item is currently selected
    iCurSel = TabCtrl_GetCurSel (hwndTabControl);

    // Load corresponding bitmap 
    switch (iCurSel)
    {
        case 0: hBitmap = LoadBitmap (hInst, "SHAPES");
                break;
        case 1: hBitmap = LoadBitmap (hInst, "CIRCLES");
                break;
        case 2: hBitmap = LoadBitmap (hInst, "LEAVES");
                break;
        case 3: 
        default:
            hBitmap = LoadBitmap (hInst, "STREAMERS");
            break;
    }

    hDC2= CreateCompatibleDC (hDC);
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    hOldBitmap = SelectObject(hDC2, hBitmap);
    StretchBlt(hDC,  0, 0, rect.right, rect.bottom,
               hDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

    DeleteObject (SelectObject (hDC2, hOldBitmap));
    DeleteDC (hDC2);

    EndPaint (hwnd, &ps);
    return 0;
}


