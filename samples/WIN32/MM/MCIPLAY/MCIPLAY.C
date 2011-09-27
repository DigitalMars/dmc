/*----------------------------------------------------------------------------*\
 *
 *  MCIPLAY:
 *
 *    Sample app showing the use of MCIWnd
 *
 *    MCIPLAY is a MDI aplication that demonstates the following:
 *
 *      - Using the MCIWnd window class to play MCI files.
 *
 *----------------------------------------------------------------------------*/
/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/

#define  STRICT
#define  INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <vfw.h>

#include "mciplay.h"

#define BUFSIZE 260
#define MCIPLAY_APP_CLASS "MCIPlay_App"
#define MCIPLAY_DOC_CLASS MCIWND_WINDOW_CLASS

//---------------------------------------------------------------------------

#define ISCHECKED(hwnd,id) (BOOL)(GetMenuState(GetMenu(hwnd), id, 0) & MF_CHECKED)

/*-------------------------------------------------------------------------*\
|                                                                          |
|   g l o b a l   v a r i a b l e s                                        |
|                                                                          |
\*------------------------------------------------------------------------*/

    HINSTANCE ghInstApp;        /* Instance handle */
    HACCEL ghAccelApp;
    HWND ghwndApp;          /* Handle to parent window */
    HWND ghwndMdi;          /* Handle to MCI client window */
    char gszBuffer[BUFSIZE];

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

LRESULT CALLBACK AppWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AppAbout(HWND, UINT, WPARAM, LPARAM);

static BOOL AppInit(HINSTANCE, HINSTANCE, LPSTR, int);
static HWND mdiCreateDoc(LPSTR, LPSTR, LPARAM);
static HWND mdiCreateClient(HWND, HMENU);
static LRESULT mdiSendMessage(HWND, HWND, UINT, WPARAM, LPARAM);
static BOOL FormatFilterString(UINT, LPSTR);
static void MCI_OnCommand(HWND, int, HWND, UINT);
static void mdiFilterSendMessage(HWND, UINT, WPARAM, LPARAM);
static void MCI_OnInitMenuPopup(HWND, HMENU, int, BOOL);
static void MCI_OnSize(HWND, UINT, int, int);

/*----------------------------------------------------------------------------*\
|   AppAbout( hDlg, msg, wParam, lParam )                                      |
|                                                                              |
|   Description:                                                               |
|       This function handles messages belonging to the "About" dialog box.    |
|       The only message that it looks for is WM_COMMAND, indicating the use   |
|       has pressed the "OK" button.  When this happens, it takes down         |
|       the dialog box.                                                        |
|                                                                              |
|   Arguments:                                                                 |
|       hDlg            window handle of about dialog window                   |
|       msg             message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if message has been processed, else FALSE                         |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK AppAbout(
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
    switch (msg) {
        case WM_COMMAND:
        EndDialog(hwnd,TRUE);
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*\
|   AppInit ( hInstance, hPrevInstance )                                       |
|                                                                              |
|   Description:                                                               |
|       This is called when the application is first loaded into               |
|       memory.  It performs all initialization that doesn't need to be done   |
|       once per instance.                                                     |
|                                                                              |
|   Arguments:                                                                 |
|   hPrevInstance   instance handle of previous instance                       |
|       hInstance       instance handle of current instance                    |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if successful, FALSE if not                                       |
|                                                                              |
\*----------------------------------------------------------------------------*/
static BOOL AppInit(
HINSTANCE hInst,
HINSTANCE hPrev,
LPSTR szCmd,
int sw)
{
    WNDCLASS cls;
    WORD wVer;
    char szAppName[BUFSIZE];

    /* Save instance handle for DialogBox */
    ghInstApp = hInst;

    LoadString( ghInstApp, IDS_APPNAME, szAppName, BUFSIZE );

    /* first let's make sure we are running on 1.1 */
    wVer = HIWORD(VideoForWindowsVersion());
    if (wVer < 0x010a){
        char szTitle[BUFSIZE];
        /* oops, we are too old, blow out of here */
        LoadString( ghInstApp, IDS_APPERR, szTitle, BUFSIZE );
        LoadString( ghInstApp, IDS_VFWTOOOLD, gszBuffer, BUFSIZE );
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL, gszBuffer, szTitle, MB_OK|MB_ICONSTOP);
        return FALSE;
    }

    ghAccelApp = LoadAccelerators(ghInstApp, "AppAccel");

    if (!hPrev) {
        cls.hCursor        = LoadCursor(NULL,IDC_ARROW);
        cls.hIcon          = LoadIcon(hInst,"AppIcon");
        cls.lpszMenuName   = "AppMenu";
        cls.lpszClassName  = MCIPLAY_APP_CLASS;
        cls.hbrBackground  = (HBRUSH)(COLOR_APPWORKSPACE+1);
        cls.hInstance      = hInst;
        cls.style          = 0;
        cls.lpfnWndProc    = (WNDPROC)AppWndProc;
        cls.cbClsExtra     = 0;
        cls.cbWndExtra     = 0;

        if (!RegisterClass(&cls))
            return FALSE;
    }

    // !!! Not using MCIWndCreate, so we must init ourselves
    if (!MCIWndRegisterClass())
        return FALSE;

    ghwndApp = CreateWindow(
        MCIPLAY_APP_CLASS,
        szAppName,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        (HWND)NULL,   /* no parent */
        (HMENU)NULL,      /* use class menu */
        (HANDLE)hInst,     /* handle to window instance */
        (LPSTR)NULL);     /* no params to pass on */

    /* Make window visible according to the way the app is activated */
    ShowWindow(ghwndApp,sw);

    return TRUE;
}

/*----------------------------------------------------------------------------*\
|   WinMain( hInstance, hPrevInstance, lpszCmdLine, cmdShow )                  |
|                                                                              |
|   Description:                                                               |
|       The main procedure for the App.  After initializing, it just goes      |
|       into a message-processing loop until it gets a WM_QUIT message         |
|       (meaning the app was closed).                                          |
|                                                                              |
|   Arguments:                                                                 |
|       hInstance       instance handle of this instance of the app            |
|       hPrevInstance   instance handle of previous instance, NULL if first    |
|       lpszCmdLine     ->null-terminated command line                         |
|       cmdShow         specifies how the window is initially displayed        |
|                                                                              |
|   Returns:                                                                   |
|       The exit code as specified in the WM_QUIT message.                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
int PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR szCmdLine,
int sw)
{
    MSG msg;

    if (!AppInit(hInstance,hPrevInstance,szCmdLine,sw))
        return FALSE;

    //Polling messages from event queue
    for (;;) {
        if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;

            if (ghAccelApp && ghwndApp && TranslateAccelerator(ghwndApp, ghAccelApp, &msg))
                continue;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
            } else {
            // idle time here, DONT BE A PIG!
            WaitMessage();
        }
    }
    return msg.wParam;
}

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

static HWND mdiCreateDoc(
LPSTR szClass,
LPSTR szTitle,
LPARAM l)
{
    MDICREATESTRUCT mdics;

    mdics.szClass   = szClass;
    mdics.szTitle   = szTitle;
    mdics.hOwner    = ghInstApp;
    mdics.x         = CW_USEDEFAULT;
    mdics.y         = 0;
    mdics.cx        = CW_USEDEFAULT;
    mdics.cy        = 0;
    mdics.style     = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MCIWNDF_SHOWALL;
    mdics.lParam    = l;

    return FORWARD_WM_MDICREATE(ghwndMdi, (LPMDICREATESTRUCT)&mdics, SendMessage);
}

/*----------------------------------------------------------------------------*\
|   mdiCreateClient()                                                          |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|   HWND if successful, NULL otherwise                                         |
|                                                                              |
\*----------------------------------------------------------------------------*/
static HWND mdiCreateClient(
HWND hwndP,
HMENU hmenuWindow)
{
    CLIENTCREATESTRUCT ccs;

    ccs.hWindowMenu = hmenuWindow;
    ccs.idFirstChild = 100;

    return CreateWindow(
            "MDICLIENT",
            NULL,
            WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
            0,
            0,
            0,
            0,
            hwndP,
            0,
            ghInstApp,
            (LPVOID)&ccs);
}

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

static LRESULT mdiSendMessage(
HWND hwndMdi,
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
    if (hwnd == (HWND)-1) {
        for (hwnd = GetWindow(hwndMdi, GW_CHILD); hwnd;
                    hwnd = GetWindow(hwnd, GW_HWNDNEXT))
            SendMessage(hwnd, msg, wParam, lParam);
        return 0L;
    } else {
        if (hwnd == NULL)
            hwnd = FORWARD_WM_MDIGETACTIVE(hwndMdi, SendMessage);
        if (hwnd)
            return SendMessage(hwnd, msg, wParam, lParam);
    }
}

//----------------------------------------------------------------------------
// MCI_OnCommand - Handles WM_COMMAND
//----------------------------------------------------------------------------
static void MCI_OnCommand(
HWND hwnd,
int id,
HWND hwndCtl,
UINT codeNotify)
{
    HWND hwndNew;
    HWND hwndMovie = FORWARD_WM_MDIGETACTIVE(ghwndMdi, SendMessage);

    switch(id) {
        case MENU_ABOUT:
            DialogBox(ghInstApp,MAKEINTRESOURCE(ABOUTBOX),hwnd,AppAbout);
            break;

        case MENU_EXIT:
            PostMessage(hwnd,WM_CLOSE,0,0L);
            break;

        case MENU_CLOSE:
            PostMessage(hwndMovie, WM_CLOSE, 0, 0L);
            break;

        case MENU_NEW:
            LoadString( ghInstApp, IDS_UNTITLLED, gszBuffer, BUFSIZE );
            if (hwndNew = mdiCreateDoc(MCIPLAY_DOC_CLASS, gszBuffer, 0))
                SetFocus(hwndNew);
            break;

        case MENU_OPEN:
        {
            OPENFILENAME ofn;
            char szFileName[BUFSIZE];
            char szFilter[BUFSIZE];

            FormatFilterString( IDS_FILTERSTRING, szFilter );

            /* prompt user for file to open */
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hwnd;
            ofn.hInstance = NULL;
            ofn.lpstrFilter = szFilter;
            ofn.lpstrCustomFilter = NULL;
            ofn.nMaxCustFilter = 0;
            ofn.nFilterIndex = 0;
            *szFileName = '\0';
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = BUFSIZE;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = "Open";
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
            ofn.nFileOffset = 0;
            ofn.nFileExtension = 0;
            ofn.lpstrDefExt = NULL;
            ofn.lCustData = 0;
            ofn.lpfnHook = NULL;
            ofn.lpTemplateName = NULL;

            if (GetOpenFileName(&ofn))
                if (hwndNew = mdiCreateDoc(MCIPLAY_DOC_CLASS, 0, (LPARAM)(LPSTR)szFileName))
                    SetFocus(hwndNew);
        }
        break;

        case WM_MDITILE:
        case WM_MDICASCADE:
        case WM_MDIICONARRANGE:
            SendMessage(ghwndMdi, id, 0, 0);
            break;

        /* Movie Menu */
        case IDM_PLAY:
            MCIWndPlay(hwndMovie);
            break;

        case IDM_RPLAY:
            MCIWndPlayReverse(hwndMovie);
            break;

        case IDM_STOP:
            MCIWndStop(hwndMovie);
            break;

        case IDM_HOME:
            MCIWndHome(hwndMovie);
            break;

        case IDM_END:
            MCIWndEnd(hwndMovie);
            break;

        case IDM_STEP:
            MCIWndStep(hwndMovie, 1);
            break;

        case IDM_RSTEP:
            MCIWndStep(hwndMovie, -1);
            break;

        /* Styles POPUP */

        case IDM_SAutosizeWindow:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOAUTOSIZEWINDOW,
                ISCHECKED(hwnd,id) ? MCIWNDF_NOAUTOSIZEWINDOW : 0);
            break;

        case IDM_SAutosizeMovie:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOAUTOSIZEMOVIE,
                ISCHECKED(hwnd,id) ? MCIWNDF_NOAUTOSIZEMOVIE : 0);
            break;

        case IDM_SPlaybar:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOPLAYBAR,
                ISCHECKED(hwnd,id) ? MCIWNDF_NOPLAYBAR : 0);
            break;

        case IDM_SRecord:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_RECORD,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_RECORD);
            break;

        case IDM_SMenu:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOMENU,
                ISCHECKED(hwnd,id) ? MCIWNDF_NOMENU : 0);
            break;

        case IDM_SErrorDlg:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOERRORDLG,
                ISCHECKED(hwnd,id) ? MCIWNDF_NOERRORDLG : 0);
            break;

        case IDM_SShowName:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_SHOWNAME,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_SHOWNAME);
            break;

        case IDM_SShowMode:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_SHOWMODE,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_SHOWMODE);
            break;

        case IDM_SShowPos:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_SHOWPOS,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_SHOWPOS);
            break;

        case IDM_SNotifyMedia:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOTIFYMEDIA,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_NOTIFYMEDIA);
            break;

        case IDM_SNotifyMode:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOTIFYMODE,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_NOTIFYMODE);
            break;

        case IDM_SNotifyPos:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOTIFYPOS,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_NOTIFYPOS);
            break;

        case IDM_SNotifySize:
            MCIWndChangeStyles(hwndMovie, MCIWNDF_NOTIFYSIZE,
                ISCHECKED(hwnd,id) ? 0 : MCIWNDF_NOTIFYSIZE);
            break;

        default:
            FORWARD_WM_COMMAND(NULL, id, hwndCtl, codeNotify, mdiFilterSendMessage);
            break;
    }
    return;
}

/*----------------------------------------------------------------------------*/
static void mdiFilterSendMessage(
HWND    hwnd,
UINT    msg,
WPARAM  wParam,
LPARAM  lParam)
{
    mdiSendMessage(ghwndMdi, hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------
// MCI_OnInitMenuPopup - Handles WM_INITMENUPOPUP
//----------------------------------------------------------------------------
static void MCI_OnInitMenuPopup(
HWND hwnd,
HMENU hMenu,
int item,
BOOL fSystemMenu)
{
    HMENU hStylesMenu = GetSubMenu(GetSubMenu(GetMenu(hwnd), 1), 10);
    HMENU hFileMenu = GetSubMenu(GetMenu(hwnd), 0);
    HMENU hMovieMenu = GetSubMenu(GetMenu(hwnd), 1);
    HMENU hWindowMenu = GetSubMenu(GetMenu(hwnd), 2);
    HWND  hwndMovie = FORWARD_WM_MDIGETACTIVE(ghwndMdi, SendMessage);
    UINT f;

    // be sure this isn't the system menu
    if (fSystemMenu)
        return;

    //
    // Check the styles properly when styles is chosen
    // !!! Make sure position constants don't change!
    // Dont forget that some of these styles use multiple bits
    // in wStyles - therefore check carefully.
    if ((hMenu == hStylesMenu) && hwndMovie) {

        UINT  wStyles = MCIWndGetStyles(hwndMovie);

        CheckMenuItem(hStylesMenu, IDM_SAutosizeWindow,
            ((wStyles & MCIWNDF_NOAUTOSIZEWINDOW) == MCIWNDF_NOAUTOSIZEWINDOW) ? MF_UNCHECKED : MF_CHECKED);
        CheckMenuItem(hStylesMenu, IDM_SAutosizeMovie,
            ((wStyles & MCIWNDF_NOAUTOSIZEMOVIE) == MCIWNDF_NOAUTOSIZEMOVIE) ? MF_UNCHECKED : MF_CHECKED);
        CheckMenuItem(hStylesMenu, IDM_SPlaybar,
            ((wStyles & MCIWNDF_NOPLAYBAR) == MCIWNDF_NOPLAYBAR) ? MF_UNCHECKED : MF_CHECKED);
        CheckMenuItem(hStylesMenu, IDM_SRecord,
            ((wStyles & MCIWNDF_RECORD) == MCIWNDF_RECORD) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SMenu,
            ((wStyles & MCIWNDF_NOMENU) == MCIWNDF_NOMENU) ? MF_UNCHECKED :MF_CHECKED);
        CheckMenuItem(hStylesMenu, IDM_SErrorDlg,
            ((wStyles & MCIWNDF_NOERRORDLG) == MCIWNDF_NOERRORDLG) ? MF_UNCHECKED :MF_CHECKED);
        CheckMenuItem(hStylesMenu, IDM_SShowName,
            ((wStyles & MCIWNDF_SHOWNAME) == MCIWNDF_SHOWNAME) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SShowMode,
            ((wStyles & MCIWNDF_SHOWMODE) == MCIWNDF_SHOWMODE) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SShowPos,
            ((wStyles & MCIWNDF_SHOWPOS) == MCIWNDF_SHOWPOS) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SNotifyMedia,
            ((wStyles & MCIWNDF_NOTIFYMEDIA) == MCIWNDF_NOTIFYMEDIA) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SNotifyMode,
            ((wStyles & MCIWNDF_NOTIFYMODE) == MCIWNDF_NOTIFYMODE) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SNotifyPos,
            ((wStyles & MCIWNDF_NOTIFYPOS) == MCIWNDF_NOTIFYPOS) ? MF_CHECKED :MF_UNCHECKED);
        CheckMenuItem(hStylesMenu, IDM_SNotifySize,
            ((wStyles & MCIWNDF_NOTIFYSIZE) == MCIWNDF_NOTIFYSIZE) ? MF_CHECKED :MF_UNCHECKED);
    }

    //
    // Enable/Disable the stuff under the MOVIE popup
    // !!! Make sure position constants don't change!
    //
    else if (hMenu == hMovieMenu) {

        EnableMenuItem( hMenu, 10,
                        MF_BYPOSITION | (hwndMovie ? MF_ENABLED : MF_GRAYED) );

        if ( !hwndMovie
           || MCIWndGetMode(hwndMovie, NULL, 0) == MCI_MODE_NOT_READY) {

            EnableMenuItem( hFileMenu, MENU_CLOSE,
                            hwndMovie ? MF_ENABLED : MF_GRAYED );

            EnableMenuItem(hMenu, IDM_STOP, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_PLAY, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_RPLAY, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_HOME, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_END, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_STEP, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_RSTEP, MF_GRAYED);

        } else {

            EnableMenuItem(hFileMenu, MENU_CLOSE, MF_ENABLED);
            f = MCIWndGetMode(hwndMovie, NULL, 0) != MCI_MODE_STOP;
            EnableMenuItem(hMenu, IDM_PLAY, !f ? MF_ENABLED : MF_GRAYED);
            EnableMenuItem(hMenu, IDM_RPLAY, !f ? MF_ENABLED : MF_GRAYED);
            EnableMenuItem(hMenu, IDM_STOP, f ? MF_ENABLED : MF_GRAYED);
            EnableMenuItem(hMenu, IDM_HOME, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_END,  MF_ENABLED);
            EnableMenuItem(hMenu, IDM_STEP, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_RSTEP,MF_ENABLED);
        }
    }

    //
    // Enable/Disable the stuff under the WINDOW menu
    // !!! Make sure position constants don't change!
    //
    else if (hMenu == hWindowMenu) {

            EnableMenuItem(hMenu, 0,
                        MF_BYPOSITION | (hwndMovie ? MF_ENABLED : MF_GRAYED));
            EnableMenuItem(hMenu, 1,
                        MF_BYPOSITION | (hwndMovie ? MF_ENABLED : MF_GRAYED));
            EnableMenuItem(hMenu, 2,
                        MF_BYPOSITION | (hwndMovie ? MF_ENABLED : MF_GRAYED));
    }

    //
    // Enable/Disable the stuff under the FILE menu
    //
    else if (hMenu == hFileMenu) {

        EnableMenuItem( hFileMenu, MENU_CLOSE,
                        hwndMovie ? MF_ENABLED : MF_GRAYED);
    }

    FORWARD_WM_INITMENUPOPUP(NULL, hMenu, item,
                             fSystemMenu, mdiFilterSendMessage);
    return;
}

//----------------------------------------------------------------------------
// MCI_OnSize - Handles WM_SIZE
//----------------------------------------------------------------------------
static void MCI_OnSize(
HWND hwnd,
UINT state,
int cx,
int cy)
{
    MoveWindow(ghwndMdi,0,0,cx,cy,TRUE);
    return;
}

/*----------------------------------------------------------------------------*\
|   AppWndProc( hwnd, msg, wParam, lParam )                                    |
|                                                                              |
|   Description:                                                               |
|       The window proc for the app's main (tiled) window.  This processes all |
|       of the parent window's messages.                                       |
|                                                                              |
|   Arguments:                                                                 |
|       hwnd            window handle for the parent window                    |
|       msg             message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
LRESULT CALLBACK AppWndProc(
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{

    switch (msg) {
       case WM_CREATE:
        ghwndMdi = mdiCreateClient(hwnd, GetSubMenu(GetMenu(hwnd), GetMenuItemCount(GetMenu(hwnd))-1));
        break;

        case WM_COMMAND:
            HANDLE_WM_COMMAND(hwnd, wParam, lParam, MCI_OnCommand);
            break;

    case WM_PALETTECHANGED:
        mdiSendMessage(ghwndMdi, (HWND)-1, msg, wParam, lParam);
        break;

    case WM_QUERYNEWPALETTE:
        return mdiSendMessage(ghwndMdi, NULL, msg, wParam, lParam);
            break;

        case WM_INITMENUPOPUP:
        HANDLE_WM_INITMENUPOPUP(hwnd, wParam, lParam, MCI_OnInitMenuPopup);
        break;

    case WM_SIZE:
        HANDLE_WM_SIZE(hwnd, wParam, lParam, MCI_OnSize);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }
    return DefFrameProc(hwnd,ghwndMdi,msg,wParam,lParam);
}

/*--------------------------------------------------------------+
| FormatFilterString                                            |
|                                                               |
+--------------------------------------------------------------*/
static BOOL FormatFilterString(
UINT uID,
LPSTR lpszString )
{
    int nLength, nCtr = 0;
    char chWildCard;

    *lpszString = 0;

    nLength = LoadString( ghInstApp, uID, lpszString, BUFSIZE );

    chWildCard = lpszString[nLength-1];

    while( lpszString[nCtr] ) {
        if( lpszString[nCtr] == chWildCard )
            lpszString[nCtr] = 0;
        nCtr++;
    }

    return TRUE;
}
