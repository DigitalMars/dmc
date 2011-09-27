/*
 * BOOK1632.H
 *
 * Macros and other definitions that assist in porting between Win16
 * and Win32 applications.  Defines WIN32 to enable 32-bit versions.
 *
 * Copyright (c)1993-1995 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  INTERNET>kraigb@microsoft.com
 */


#ifndef _BOOK1632_H_
#define _BOOK1632_H_

#ifdef WIN32

#ifndef COMMANDPARAMS
#define COMMANDPARAMS(wID, wCode, hWndMsg)                          \
    WORD        wID     = LOWORD(wParam);                           \
    WORD        wCode   = HIWORD(wParam);                           \
    HWND        hWndMsg = (HWND)(UINT)lParam;
#endif  //COMMANDPARAMS

#ifndef SendCommand
#define SendCommand(hWnd, wID, wCode, hControl)                     \
            SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(wID, wCode)    \
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
            SendMessage(hWnd, WM_MENUSELECT                         \
                , MAKEWPARAM((WORD)wID, (WORD)wFlags), (LPARAM)hMenu)

#endif  //SendMenuSelect

#ifndef SendScrollPosition
#define SendScrollPosition(hWnd, iMsg, iPos)                        \
            SendMessage(hWnd, iMsg, MAKEWPARAM(SB_THUMBPOSITION, iPos), 0)
#endif //SendScrollPosition

#ifndef ScrollThumbPosition
#define ScrollThumbPosition(w, l) HIWORD(w)
#endif //ScrollThumbPosition

#ifndef GETWINDOWINSTANCE
#define GETWINDOWINSTANCE(h) (HINSTANCE)GetWindowLong(h, GWL_HINSTANCE)
#endif  //GETWINDOWINSTANCE

#ifndef GETWINDOWID
#define GETWINDOWID(h) (UINT)GetWindowLong(h, GWW_ID)
#endif  //GETWINDOWID

#ifndef POINTFROMLPARAM
#define POINTFROMLPARAM(p, l) {p.x=(LONG)(SHORT)LOWORD(l); \
                               p.y=(LONG)(SHORT)HIWORD(l);}
#endif  //POINTEFROMLPARAM

#ifndef MDIREFRESHMENU
#define MDIREFRESHMENU(h) SendMessage(h, WM_MDIREFRESHMENU, 0, 0L)
#endif  //MDIREFRESHMENU


//Activated child window in WM_MDIACTIVATE
#ifndef NEWMDIACTIVE
#define NEWMDIACTIVE lParam
#endif  //NEWMDIACTIVE

#ifndef UNICODETOANSI
#ifdef UNICODE
#define UNICODETOANSI(s, d, cch) WideCharToMultiByte(CP_ACP \
            , 0, s, -1, d, cch, NULL, NULL)
#else  //ANSI
#define UNICODETOANSI(s, d, cch) lstrcpy(d, (LPTSTR)s)
#endif
#endif //UNICODETOANSI


//****END WIN32



#else



//****START !WIN32

#ifndef POINTS
typedef POINT POINTS;
#endif  //POINTS

#ifndef LPPOINTS
typedef POINTS FAR *LPPOINTS;
#endif  //LPPOINTS

#ifndef MAKEPOINTS
#define MAKEPOINTS MAKEPOINT
#endif  //MAKEPOINTS


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
                        , MAKELONG((WORD)wFlags, (WORD)hMenu))
#endif  //SendMenuSelect

#ifndef SendScrollPosition
#define SendScrollPosition(hWnd, iMsg, iPos)                        \
            SendMessage(hWnd, iMsg, SB_THUMBPOSITION, MAKELONG(iPos, 0))
#endif //Send ScrollPosition

#ifndef ScrollThumbPosition
#define ScrollThumbPosition(w, l) LOWORD(l)
#endif //ScrollThumbPosition

#ifndef GETWINDOWINSTANCE
#define GETWINDOWINSTANCE(h) (HINSTANCE)GetWindowWord(h, GWW_HINSTANCE)
#endif  //GETWINDOWINSTANCE

#ifndef GETWINDOWID
#define GETWINDOWID(h) (UINT)GetWindowWord(h, GWW_ID)
#endif  //GETWINDOWID

#ifndef POINTFROMLPARAM
#define POINTFROMLPARAM(p, l) {p.x=LOWORD(l); p.y=HIWORD(l);}
#endif  //POINTEFROMLPARAM

#ifndef MDIREFRESHMENU
#define MDIREFRESHMENU(h) SendMessage(h, WM_MDISETMENU, TRUE, 0L)
#endif  //MDIREFRESHMENU

//Activated child window in WM_MDIACTIVATE
#ifndef NEWMDIACTIVE
#define NEWMDIACTIVE wParam
#endif  //NEWMDIACTIVE


//Things not present in Win3.1 SDK but present in Win32 SDK.

#ifndef APIENTRY
#define APIENTRY __export FAR PASCAL
#endif  //APIENTRY

#ifndef USHORT
typedef unsigned short USHORT;
#endif  //USHORT

//These are so we can write ANSI/UNICODE portable code.
#ifndef TCHAR
typedef char TCHAR;
#endif  //TCHAR

#ifndef LPTSTR
typedef TCHAR *LPTSTR;
#endif  //LPTSTR

#ifndef LPCTSTR
typedef const TCHAR *LPCTSTR;
#endif  //LPCTSTR

#ifndef _tcsncpy
#define _tcsncpy strncpy
#endif  //_tcsncpy

#ifndef lstrcpyA
#define lstrcpyA lstrcpy
#endif  //lstrcpyA

#ifndef lstrcmpiA
#define lstrcmpiA lstrcmpi
#endif  //lstrcmpiA

#ifndef TEXT
#define TEXT(a) a
#endif  //TEXT

#ifndef DeleteFile
#define DeleteFile(f)                       \
            {                               \
            OFSTRUCT    of;                 \
            OpenFile(f, &of, OF_DELETE);    \
            }
#endif  //DeleteFile

#ifndef UNICODETOANSI
#define UNICODETOANSI(s, d, cch) lstrcpy(d, (LPTSTR)s)
#endif  //UNICODETOANSI

#ifndef SetForegroundWindow
#define SetForegroundWindow(w) SetActiveWindow(w)
#endif  //SetForegroundWindow

#endif  //!WIN32


//These definitions we need regardless of Win16 or Win32

typedef struct
    {
    short   left;
    short   top;
    short   right;
    short   bottom;
    } RECTS, FAR *LPRECTS;

#define RECTSTORECT(rs, r) {(r).left=(rs).left;(r).top=(rs).top; \
    (r).right=(rs).right;(r).bottom=(rs).bottom;};

#define RECTTORECTS(r, rs) {(rs).left=(short)(r).left;   \
    (rs).top=(short)(r).top;(rs).right=(short)(r).right; \
    (rs).bottom=(short)(r).bottom;};

#endif  //_BOOK1632_H_
