#define WIN31
#include <windows.h>
#include <scrnsave.h>
#include "cparrow.h"
#include "comstrin.h"
#include "uniconv.h"


/* Global variables... */
HANDLE  hMainInstance;
HWND    hMainWindow;

TCHAR   szName[TITLEBARNAMELEN];
TCHAR   szAppName[APPNAMEBUFFERLEN];       // Section name in CONTROL.INI
TCHAR   szIniFile[MAXFILELEN];
TCHAR   szScreenSaver[22];
TCHAR   szHelpFile[MAXFILELEN];
TCHAR   szNoHelpMemory[BUFFLEN];
UINT    MyHelpMessage;


/* Local Function definitions... */
BOOL AppInit           (HANDLE hInst, HANDLE hPrev, WORD sw, LPTSTR szCmdLine);
int  DoConfigureDialog (HANDLE hInst, BOOL fParent);

HCURSOR hcurOld;

HHOOK hhkNextMsgFilterHookFunc = NULL;

LRESULT CALLBACK HelpMessageFilterHookFunction (int nCode, WPARAM wParam, LPMSG lpMsg);

#define THRESHOLD   3


//***************************************************************************

BOOL AppInit (HANDLE hInst, HANDLE hPrev, WORD sw, LPTSTR szCmdLine)
{
    WNDCLASS cls;
    int    dx, dy;

    if (hPrev != NULL)
        return FALSE;

    /*
     *  Register a class for the main application window
     */
    cls.hCursor        = NULL;
    cls.hIcon          = LoadIcon (hInst, MAKEINTATOM (ID_APP));
    cls.lpszMenuName   = NULL;
    cls.lpszClassName  = TEXT("WindowsScreenSaverClass");
    cls.hbrBackground  = GetStockObject (BLACK_BRUSH);
    cls.hInstance      = hInst;
    cls.style          = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    cls.lpfnWndProc    = ScreenSaverProc;
    cls.cbWndExtra     = 0;
    cls.cbClsExtra     = 0;

    if (!RegisterClass (&cls))
        return FALSE;

    dx = GetSystemMetrics (SM_CXSCREEN);
    dy = GetSystemMetrics (SM_CYSCREEN);
    hMainWindow = CreateWindowEx (WS_EX_TOPMOST,
                            TEXT("WindowsScreenSaverClass"), // Class name
                            TEXT("\0"),                      // Caption
                            WS_POPUP | WS_VISIBLE |          // Style bits
                            WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                            0, 0,                            // Position
                            dx, dy,                          // Size
                            (HWND)NULL,                      // Parent window (no parent)
                            (HMENU)NULL,                     // use class menu
                            (HANDLE)hInst,                   // handle to window instance
                            (LPVOID)NULL                     // no params to pass on
                               );
    if (hMainWindow)
        return TRUE;

    return FALSE;
}


//***************************************************************************

int _CRTAPI1 main (USHORT argc, CHAR **argv)
{
    HANDLE   hInst;
    HANDLE   hPrev     = NULL;
    LPTSTR   szCmdLine = GetCommandLine();
    WORD     sw        = SW_SHOWNORMAL;
    MSG      msg;
    LPTSTR   lpT;

    hInst = GetModuleHandle (NULL);

    /* Save instance handle for DialogBoxs */
    hMainInstance = hInst;

    // This has to be loaded first so we know who we are and to find if we
    // already around.

    LoadString (hInst, idsAppName, szAppName, CharSizeOf(szAppName));

    //=================================================================
    // on NT, szCmdLine's first string includes its own name, remove this
    // to make it exactly like the windows command line.

    if (*szCmdLine)
    {
        lpT = _tcschr (szCmdLine, TEXT(' '));   // skip self name
        if (lpT)
        {
            szCmdLine = lpT;
            while (*szCmdLine == TEXT(' '))
                szCmdLine++;            // skip spaces to end or first cmd
        }
        else
        {
            szCmdLine += lstrlen(szCmdLine);   // point to NULL
        }
    }
    //=====================================================================

    MyHelpMessage = RegisterWindowMessage (szAppName);

    if (MyHelpMessage)
        hhkNextMsgFilterHookFunc = SetWindowsHook (WH_MSGFILTER,
                                      (HOOKPROC)HelpMessageFilterHookFunction);
    /* Parse through the command line.  If the parameter is -s, /s, or s, then
       bring up a configure dialog box.  Otherwise, bring up the normal
       window...*/
    if (!((!_tcsnicmp (szCmdLine, TEXT("/s"), 2) || !_tcsicmp (szCmdLine, TEXT("-s"))) ||
        !_tcsicmp (szCmdLine, TEXT("s"))))
    {
        if (!((!_tcsicmp (szCmdLine, TEXT("/c")) || !_tcsicmp (szCmdLine, TEXT("-c"))) ||
            !_tcsicmp (szCmdLine, TEXT("c"))))
            return DoConfigureDialog (hInst, FALSE);
        else
            return DoConfigureDialog (hInst, TRUE);
    }

    /* Call initialization procedure */
    if (!AppInit (hInst, hPrev, sw, szCmdLine))
        return FALSE;

    /* Message Loop for the program... */
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    if (MyHelpMessage)
        UnhookWindowsHook (WH_MSGFILTER, (HOOKPROC)HelpMessageFilterHookFunction);

    return msg.wParam;
}


//***************************************************************************


LONG DefScreenSaverProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static BOOL     fHere = FALSE;
    static POINT    ptLast;
    static BOOL     bDialogUp = FALSE;
    POINT           ptCursor, ptCheck;

    switch (msg)
    {
    case WM_SYSCOMMAND:
        if ((wParam==SC_SCREENSAVE) || (wParam==SC_CLOSE))
            return FALSE;
        break;

    case WM_DESTROY:
        PostQuitMessage (0);
        break;

    case WM_SETCURSOR:
        SetCursor (NULL);
        break;

    case WM_NCACTIVATE:
        if (wParam==FALSE && !bDialogUp)
            return FALSE;
        break;

    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        if (wParam != FALSE)        // only fall through if we are
            break;                  // losing the focus...

    case WM_MOUSEMOVE:
        if (!fHere)
        {
            GetCursorPos (&ptLast);
            fHere = TRUE;
        }
        else
        {
            GetCursorPos (&ptCheck);
            if (ptCursor.x = ptCheck.x - ptLast.x)
            {
                if (ptCursor.x < 0)
                    ptCursor.x *= -1;
            }
            if (ptCursor.y = ptCheck.y - ptLast.y)
            {
                if (ptCursor.y < 0)
                    ptCursor.y *= -1;
            }
            if ((ptCursor.x + ptCursor.y) > THRESHOLD)
                goto SHOWDIALOG;
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        GetCursorPos (&ptCursor);
        ptCursor.x ++;
        ptCursor.y ++;
        SetCursorPos (ptCursor.x, ptCursor.y);
        GetCursorPos (&ptCheck);
        if (ptCheck.x != ptCursor.x && ptCheck.y != ptCursor.y)
            ptCursor.x -= 2;
        ptCursor.y -= 2;
        SetCursorPos (ptCursor.x, ptCursor.y);

        // fall thru

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
SHOWDIALOG:
        PostMessage (hWnd, WM_CLOSE, 0, 0l);
        break;
    }
    return DefWindowProc (hWnd, msg, wParam, lParam);
}


//***************************************************************************

int    DoConfigureDialog (HANDLE hInst, BOOL fParent )
{
    HWND    hWndParent;

    if (fParent)
        hWndParent = GetActiveWindow ();
    else
        hWndParent = NULL;

    if (RegisterDialogClasses (hInst))
    {
        hMainWindow = NULL;
        DialogBox (hInst, MAKEINTRESOURCE (DLG_SCRNSAVECONFIGURE),
                          hWndParent, (WNDPROC) ScreenSaverConfigureDialog);
    }

    return TRUE;
}

//***************************************************************************

LRESULT CALLBACK HelpMessageFilterHookFunction (int nCode, WPARAM wParam, LPMSG lpMsg)
{

    if (nCode < 0)
        goto DefHook;

    if (nCode == MSGF_DIALOGBOX)
        if (lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_F1)
        {
            HWND hTemp;
            HWND hParent = lpMsg->hwnd;

            while (hParent != NULL)
            {
                hTemp = hParent;
                if (!(GetWindowLong(hTemp, GWL_STYLE) & WS_CHILD))
                    break;
                hParent = (HWND) GetWindowLong (hParent, GWL_HWNDPARENT);
            }

            PostMessage (hTemp, MyHelpMessage, 0, 0L);
            return TRUE;
        }
        else
DefHook:
        return (int)DefHookProc(nCode, wParam, (LONG)lpMsg, &hhkNextMsgFilterHookFunc);

    return 0;
}

