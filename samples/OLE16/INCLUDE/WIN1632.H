/*
 * WIN1632.H
 *
 * Macros and other definitions that assist in porting between Win16
 * and Win32 applications.  Define WIN32 to enable 32-bit versions.
 *
 * Copyright (c)1993 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  INTERNET>kraigb@microsoft.com
 */


#ifndef _WIN1632_H_
#define _WIN1632_H_

//Macros to handle control message packing between Win16 and Win32
#ifdef WIN32

#define EXPORT
#define MAKEPOINT MAKEPOINTS

#ifndef COMMANDPARAMS
#define COMMANDPARAMS(wID, wCode, hWndMsg)                          \
	WORD        wID     = LOWORD(wParam);                           \
	WORD        wCode   = HIWORD(wParam);                           \
	HWND        hWndMsg = (HWND)(UINT)lParam;
#endif  //COMMANDPARAMS

#ifndef SendCommand
#define SendCommand(hWnd, wID, wCode, hControl)                     \
			SendMessage(hWnd, WM_COMMAND, MAKELONG(wID, wCode)      \
						, (LPARAM)hControl)
#endif  //SendCommand

#ifndef MENUSELECTPARAMS
#define MENUSELECTPARAMS(wID, wFlags, hMenu)                        \
	WORD        wID     = LOWORD(wParam);                           \
	WORD        wFlags  = HIWORD(wParam);                           \
	HMENU       hMenu   = (HMENU)lParam;
#endif  //MENUSELECTPARAMS


#ifndef SendMenuSelect
#define SendMenuSelect(hWnd, wID, wFlags, hMenu)                    \
			SendMessage(hWnd, WM_MENUSELECT, (WPARAM)MAKELONG(wID, wFlags)  \
						, (LPARAM)hMenu)
#endif  //SendMenuSelect

#else   //Start !WIN32

#define EXPORT __export

#ifndef COMMANDPARAMS
#define COMMANDPARAMS(wID, wCode, hWndMsg)                          \
	WORD        wID     = LOWORD(wParam);                           \
	WORD        wCode   = HIWORD(lParam);                           \
	HWND        hWndMsg = (HWND)(UINT)lParam;
#endif  //COMMANDPARAMS

#ifndef SendCommand
#define SendCommand(hWnd, wID, wCode, hControl)                     \
			SendMessage(hWnd, WM_COMMAND, wID                       \
						, MAKELONG(hControl, wCode))
#endif  //SendCommand

#ifndef MENUSELECTPARAMS
#define MENUSELECTPARAMS(wID, wFlags, hMenu)                        \
	WORD        wID     = LOWORD(wParam);                           \
	WORD        wFlags  = LOWORD(lParam);                           \
	HMENU       hMenu   = (HMENU)HIWORD(lParam);
#endif  //MENUSELECTPARAMS

#ifndef SendMenuSelect
#define SendMenuSelect(hWnd, wID, wFlags, hMenu)                    \
			SendMessage(hWnd, WM_MENUSELECT, wID                    \
						, MAKELONG(wFlags, hMenu))
#endif  //SendMenuSelect


#endif  //!WIN32

#endif  //_WIN1632_H_
