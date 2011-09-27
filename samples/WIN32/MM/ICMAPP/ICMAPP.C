/*----------------------------------------------------------------------------*\
 *
 *      ICMAPP:
 *
 *      Sample app showing the use of DrawDib and the
 *      VfW Image compression/decompression APIs
 *
 *      ICMAPP is a MDI aplication that demonstates the following:
 *
 *      - loading/saving windows bitmaps (ie DIBs) see DIB.C and DIB.H
 *
 *      - using DRAWDIB to draw compressed/uncompressed images
 *
 *      - calling ICImageCompress and ICImageDecompress and ICCompressorChoose
 *
 *----------------------------------------------------------------------------*/

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

#define  STRICT
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>

#define NOAVIFILE
#include <vfw.h>

#include "icmapp.h"
#include "dib.h"
#include "pro.h"

/*----------------------------------------------------------------------------*/

static HCURSOR  ghcSave;
#define StartWait()     ghcSave = SetCursor(LoadCursor(NULL, IDC_WAIT))
#define EndWait()       SetCursor(ghcSave)

/*----------------------------------------------------------------------------*/

#define BOUND(x,min,max)        ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define SWAP(x,y)       ((x)^=(y)^=(x)^=(y))
#define ALIGNB(x)       (((x) + 7) & ~0x07)

/*----------------------------------------------------------------------------*/

#ifndef MKFOURCC
#define MKFOURCC(ch0, ch1, ch2, ch3) \
        ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
        ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))
#endif

/*----------------------------------------------------------------------------*/

#define mdiActiveDoc(hwnd) \
        FORWARD_WM_MDIGETACTIVE(hwnd, SendMessage)

#define DocClass        "ChildChild"            /*  the class of the child */

/*----------------------------------------------------------------------------*/

typedef struct {
        HWND    hwnd;
        HDRAWDIB        hdd;
        HANDLE  hdib;
        LPBITMAPINFOHEADER      lpbi;
        BITMAPINFOHEADER        bi;
        RECT    rcDraw;
        RECT    rcSource;
        int     iZoom;
        BOOL    fCanDecompress;
        BOOL    fCanCompress;
        char    achFileName[BUFSIZE];
} DIBINFO, *PDIBINFO;

//----------------------------------------------------------------------------

int PASCAL       WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

LRESULT CALLBACK AppWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK mdiDocWndProc(HWND, UINT, WPARAM, LPARAM);
LONG CALLBACK    PreviewStatusProc(LPARAM, UINT, LONG);
BOOL CALLBACK    AppAbout(HWND,UINT, WPARAM, LPARAM);

static BOOL      AppInit(HINSTANCE, HINSTANCE, LPSTR, int);
static int       ErrMsg(LPCSTR, ...);
static void      GetRealClientRect(HWND, LPRECT);
static void      InitSize(PDIBINFO);
static void      SizeWindowToImage(PDIBINFO);
static LPCSTR    FileName(LPCSTR);
static BOOL      InitDib(PDIBINFO, HANDLE);
static BOOL      FormatFilterString(UINT, LPSTR);
static HGLOBAL   CopyHandle(HGLOBAL);
static LRESULT   mdiSendMessage(HWND, HWND, UINT, WPARAM, LPARAM);
static HWND      mdiCreateClient(HWND, HMENU);
static HWND      mdiCreateChild(HWND, LPSTR, DWORD, int, int, int, int, LPARAM);
static HWND      mdiCreateDoc(LPSTR, LPARAM);
static void      mdiFilterSendMessage(HWND, UINT, WPARAM, LPARAM);
static void      app_OnCommand(HWND, int, HWND, UINT);
static void      app_OnSize(HWND, UINT, int, int);
static void      app_OnInitMenu(HWND, HMENU);
static void      mdi_OnInitMenu(HWND, HMENU);
static BOOL      mdi_OnCreate(HWND, LPCREATESTRUCT);
static void      mdi_OnCommand(HWND, int, HWND, UINT);
static void      mdi_OnKey(HWND, UINT, BOOL, int, UINT);
static void      mdi_OnVScroll(HWND, HWND, UINT, int);
static void      mdi_OnHScroll(HWND, HWND, UINT, int);
static BOOL      mdi_OnEraseBkgnd(HWND, HDC);


//----------------------------------------------------------------------------

static HINSTANCE        ghInstApp;              /* Instance handle */
static HACCEL           ghAccelApp;             /* Handle to accel table */
static HWND             ghwndApp;               /* Handle to parent window */
static HWND             ghwndMdi;               /* Handle to MDI client window */
static BOOL             gfStretchToWindow;
static char             szStringBuffer[BUFSIZE];

/*----------------------------------------------------------------------------*\
|   ErrMsg - Opens a Message box with a error message in it.  The user can     |
|            select the OK button to continue or the CANCEL button to kill     |
|            the parent application.                                           |
\*----------------------------------------------------------------------------*/
static int ErrMsg(
        LPCSTR  sz,
        ...)
{
        char    szOutput[300];
    va_list va;

    va_start(va, sz);
        wvsprintf(szOutput, sz, va);   /* Format the string */
    va_end(va);
        return MessageBox(NULL, szOutput, NULL, MB_OK | MB_ICONEXCLAMATION);
}

/*----------------------------------------------------------------------------*/
static void GetRealClientRect(
        HWND    hwnd,
        LPRECT  lprc)
{
        DWORD   dwStyle;

        dwStyle = GetWindowStyle(hwnd);
        GetClientRect(hwnd,lprc);
        if (dwStyle & WS_HSCROLL)
                lprc->bottom += GetSystemMetrics(SM_CYHSCROLL);
        if (dwStyle & WS_VSCROLL)
                lprc->right  += GetSystemMetrics(SM_CXVSCROLL);
}

/*----------------------------------------------------------------------------*/
static void InitSize(
        PDIBINFO        pdi)
{
        RECT    rc;
        int     i;
        int     iRangeV;
        int     iRangeH;
        static int      iSem;

        if (pdi->hdib == NULL || iSem)
                return;
        iSem++; // Only works on luck, really needs a CRITICAL_SECTION;

        // stretch to window?
        if (gfStretchToWindow || IsIconic(pdi->hwnd)) {
                SetScrollRange(pdi->hwnd, SB_VERT, 0, 0, TRUE);
                SetScrollRange(pdi->hwnd, SB_HORZ, 0, 0, TRUE);
                GetClientRect(pdi->hwnd, &pdi->rcDraw);
                pdi->iZoom = 0;
        } else if (pdi->iZoom) {
                SetRect(&pdi->rcDraw,0, 0, (int)pdi->bi.biWidth * pdi->iZoom / ZOOM, (int)pdi->bi.biHeight * pdi->iZoom / ZOOM);
                GetRealClientRect(pdi->hwnd,&rc);
                for (i = 0; i < 2; i++) {
                        iRangeV = pdi->rcDraw.bottom - rc.bottom;
                        iRangeH = pdi->rcDraw.right - rc.right;

                        if (iRangeH < 0)
                                iRangeH = 0;
                        if (iRangeV < 0)
                                iRangeV = 0;

                        if (GetScrollPos(pdi->hwnd,SB_VERT) > iRangeV ||
                            GetScrollPos(pdi->hwnd,SB_HORZ) > iRangeH)
                                InvalidateRect(pdi->hwnd,NULL,TRUE);

                        SetScrollRange(pdi->hwnd,SB_VERT,0,iRangeV,TRUE);
                        SetScrollRange(pdi->hwnd,SB_HORZ,0,iRangeH,TRUE);

                        GetClientRect(pdi->hwnd,&rc);
                }
        }
        iSem--;
}

/*----------------------------------------------------------------------------*/
static void SizeWindowToImage(
        PDIBINFO        pdi)
{
        RECT    rc;
        RECT    rcMdi;
        RECT    rcChild;

        /* don't size window if it's currently maximized */
        if (!IsZoomed(pdi->hwnd) && !IsIconic(pdi->hwnd) && pdi->iZoom > 0) {
                SetRect(&rc, 0, 0, (int)pdi->bi.biWidth * pdi->iZoom / ZOOM, (int)pdi->bi.biHeight * pdi->iZoom / ZOOM);
                AdjustWindowRect(&rc, GetWindowStyle(pdi->hwnd), FALSE);

                /* Determine size of MDI Client area and image window */
                GetClientRect(ghwndMdi, &rcMdi);
                rc.right -= rc.left;
                rc.bottom -= rc.top;

                /* Make sure window is positioned so that entire window is seen */
                GetWindowRect(pdi->hwnd, &rcChild);
                ScreenToClient(ghwndMdi, (LPPOINT)&rcChild);
                if (rc.right > rcMdi.right) {
                        rc.right = rcMdi.right + 2;
                        rcChild.left = -1;
                }
                if (rc.bottom > rcMdi.bottom) {
                        rc.bottom = rcMdi.bottom+2;
                        rcChild.top = -1;
                }
                SetWindowPos(pdi->hwnd, NULL, rcChild.left, rcChild.top, rc.right, rc.bottom, SWP_NOACTIVATE|SWP_NOZORDER);
        }
        InitSize(pdi);
}

/*----------------------------------------------------------------------------*/
static LPCSTR FileName(
        LPCSTR  szFilePath)
{
        LPCSTR  szCurrent;

#define SLASH(c)     ((c) == '/' || (c) == '\\')

        for (szCurrent = szFilePath; *szCurrent; szCurrent++)
                ;
        for (; szCurrent >= szFilePath && !SLASH(*szCurrent) && *szCurrent != ':'; szCurrent--)
                ;
        return ++szCurrent;
}

/*----------------------------------------------------------------------------*/
static BOOL InitDib(
        PDIBINFO        pdi,
        HANDLE          hdib)
{
        char    ach[128];
        DWORD   dwFOURCC;

        if (hdib == NULL)
                hdib = OpenDIB(pdi->achFileName);

        if (hdib == NULL)
                return FALSE;

        pdi->hdd = DrawDibOpen();
        pdi->hdib = hdib;
        pdi->iZoom = ZOOM;
        pdi->lpbi = (LPVOID)GlobalLock(hdib);
        pdi->fCanCompress = pdi->lpbi->biCompression == 0;
        pdi->fCanDecompress = pdi->lpbi->biCompression != 0;

        // Fix up the default DIB fields
        if (pdi->lpbi->biClrUsed == 0 && pdi->lpbi->biBitCount <= 8)
                pdi->lpbi->biClrUsed = (1 << (int)pdi->lpbi->biBitCount);

        if (!DrawDibBegin(pdi->hdd,NULL,-1,-1,pdi->lpbi,-1,-1,0)) {
                LoadString(ghInstApp, IDS_NODRAW, szStringBuffer, BUFSIZE);
                ErrMsg(szStringBuffer);
        }

        pdi->bi = *pdi->lpbi;
        SetRect(&pdi->rcSource,  0, 0, (int)pdi->bi.biWidth, (int)pdi->bi.biHeight);

        switch (dwFOURCC = pdi->bi.biCompression) {
        case BI_RGB:
                dwFOURCC = mmioFOURCC('N', 'o', 'n', 'e');
                break;
        case BI_RLE4:
                dwFOURCC = mmioFOURCC('R', 'l', 'e', '4');
                break;
        case BI_RLE8:
                dwFOURCC = mmioFOURCC('R', 'l', 'e', '8');
                break;
        }

        wsprintf(ach, "%s (%dx%dx%d '%4.4s' %dk)", FileName(pdi->achFileName), (int)pdi->bi.biWidth, (int)pdi->bi.biHeight, (int)pdi->bi.biBitCount, (LPCSTR)&dwFOURCC, (int)(pdi->bi.biSizeImage / 1024));

        SetWindowText(pdi->hwnd, ach);
        SizeWindowToImage(pdi);
        return TRUE;
}

/*----------------------------------------------------------------------------*/
static BOOL FormatFilterString(
        UINT    uID,
        LPSTR   szString)
{
        int     nLength;
        int     nCtr = 0;
        char    chWildCard;

        *szString = 0;
        nLength = LoadString(ghInstApp, uID, szString, BUFSIZE);
        chWildCard = szString[nLength-1];
        while(szString[nCtr]) {
                if (szString[nCtr] == chWildCard)
                        szString[nCtr] = 0;
                nCtr++;
        }
        return TRUE;
}

/*----------------------------------------------------------------------------*/
static HGLOBAL CopyHandle(
        HGLOBAL h)
{
        HGLOBAL hCopy = GlobalAlloc(GHND, GlobalSize(h));

        if (hCopy)
                hmemcpy(GlobalLock(hCopy), GlobalLock(h), GlobalSize(h));
        return hCopy;
}

/*----------------------------------------------------------------------------*/
LONG CALLBACK PreviewStatusProc(LPARAM lParam, UINT message, LONG l)
{
        switch (message) {
                MSG msg;

        case ICSTATUS_START:
                ProOpen((HWND)(UINT) lParam);
                LoadString(ghInstApp, IDS_COMPRESSING, szStringBuffer, BUFSIZE);
                ProSetText(ID_STATUS1, szStringBuffer);
                break;
        case ICSTATUS_STATUS:
                ProSetBarPos((int) l);
                while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                        if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                                return 1;
                        if (msg.message == WM_SYSCOMMAND && (msg.wParam & 0xFFF0) == SC_CLOSE)
                                return 1;
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                }
                break;
        case ICSTATUS_END:
                ProClose();
                break;
        case ICSTATUS_YIELD:
                break;
        }
        return 0;
}

/*----------------------------------------------------------------------------*/
static LRESULT mdiSendMessage(
        HWND    hwndMdi,
        HWND    hwnd,
        UINT    msg,
        WPARAM  wParam,
        LPARAM  lParam)
{
        if (hwnd == HWND_BROADCAST) {
                for (hwnd = GetWindow(hwndMdi, GW_CHILD); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT))
                        SendMessage(hwnd, msg, wParam, lParam);
                return 0L;
        } else {
                if (hwnd == NULL)
                        hwnd = mdiActiveDoc(hwndMdi);
                if (hwnd)
                        return SendMessage(hwnd, msg, wParam, lParam);
        }
}

/*----------------------------------------------------------------------------*\
|   mdiCreateClient()                                                           |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       HWND if successful, NULL otherwise                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
static HWND mdiCreateClient(
        HWND    hwndP,
        HMENU   hmenuWindow)
{
        CLIENTCREATESTRUCT      ccs;

        ccs.hWindowMenu = hmenuWindow;
        ccs.idFirstChild = 10000;

        return CreateWindowEx(0, "MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hwndP, 0, GetWindowInstance(hwndP), (LPVOID)&ccs);
}

/*----------------------------------------------------------------------------*\
|   mdiCreateChild()                                                           |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       HWND if successful, NULL otherwise                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
static HWND mdiCreateChild(
        HWND    hwndMdi,
        LPSTR   szTitle,
        DWORD   dwStyle,
        int     x,
        int     y,
        int     dx,
        int     dy,
        LPARAM  l)
{
        MDICREATESTRUCT mdics;

        mdics.szClass = DocClass;
        mdics.szTitle = szTitle;
        mdics.hOwner = GetWindowInstance(hwndMdi);
        mdics.x = x;
        mdics.y = y;
        mdics.cx = dx;
        mdics.cy = dy;
        mdics.style = dwStyle;
        mdics.lParam = l;
        return FORWARD_WM_MDICREATE(hwndMdi, (LPMDICREATESTRUCT)&mdics, SendMessage);
}

/*----------------------------------------------------------------------------*\
|   mdiCreateDoc()                                                           |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       HWND if successful, NULL otherwise                                     |
|                                                                              |
|   Comments: We check the high word of the return value from WM_MDIGETACTIVE  |
|   to determine whether we need to create the new window maximized or not.    |
|                                                                              |
\*----------------------------------------------------------------------------*/
static HWND mdiCreateDoc(
        LPSTR   szTitle,
        LPARAM  l)
{
        BOOL    fMax = (BOOL)(GetWindowStyle(ghwndMdi) & WS_MAXIMIZE);

        return mdiCreateChild(ghwndMdi, szTitle, (fMax ? WS_MAXIMIZE : 0L), CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, l);
}

/*----------------------------------------------------------------------------*\
|   AppAbout(hDlg, msg, wParam, lParam)                                        |
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
        HWND    hwnd,
        UINT    msg,
        WPARAM  wParam,
        LPARAM  lParam)
{
        switch (msg) {
        case WM_COMMAND:
                EndDialog(hwnd, TRUE);
                return TRUE;
        case WM_INITDIALOG:
                return TRUE;
        }
        return FALSE;
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

/*----------------------------------------------------------------------------*/
static void app_OnCommand(
        HWND    hwnd,
        int     id,
        HWND    hwndCtl,
        UINT    codeNotify)
{
        HANDLE  h;
        HWND    hwndActive;

        switch(id) {
        case MENU_ABOUT:
                DialogBox(ghInstApp, "ABOUTBOX", hwnd, AppAbout);
                break;
        case MENU_EXIT:
                FORWARD_WM_CLOSE(hwnd, PostMessage);
                break;
        case MENU_CLOSE:
                FORWARD_WM_CLOSE(NULL, mdiFilterSendMessage);
                break;
        case MENU_CLOSEALL:
                while (hwndActive = mdiActiveDoc(ghwndMdi))
                        FORWARD_WM_CLOSE(hwndActive, SendMessage);
                break;
        case MENU_PASTE:
                if (!OpenClipboard(hwnd))
                        break;
                if (h = GetClipboardData(CF_DIB)) {
                        LoadString(ghInstApp, IDS_CLIPBOARD, szStringBuffer, BUFSIZE);
                        mdiCreateDoc(szStringBuffer, (LPARAM)(UINT)CopyHandle(h));
                } else if (h = GetClipboardData(CF_BITMAP)) {
                        LoadString(ghInstApp, IDS_CLIPBOARD, szStringBuffer, BUFSIZE);
                        mdiCreateDoc(szStringBuffer, (LPARAM)(UINT)DibFromBitmap(h, BI_RGB, 0, GetClipboardData(CF_PALETTE), 0));
                }
                CloseClipboard();
                break;
        case MENU_NEW:
                LoadString(ghInstApp, IDS_NEW, szStringBuffer, BUFSIZE);
                mdiCreateDoc(szStringBuffer, 0);
                break;
        case MENU_OPEN:
        {
                OPENFILENAME    ofn;
                char    szBuffer[BUFSIZE];
                char    szFileName[BUFSIZE];

                lstrcpy(szFileName, "");
                FormatFilterString(IDS_OPENFILTER, szBuffer);

                /* prompt user for file to open */
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hwnd;
                ofn.hInstance = NULL;
                ofn.lpstrFilter = szBuffer;
                ofn.lpstrCustomFilter = NULL;
                ofn.nMaxCustFilter = 0;
                ofn.nFilterIndex = 0;
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName);
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.lpstrTitle = NULL;
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
                ofn.nFileOffset = 0;
                ofn.nFileExtension = 0;
                ofn.lpstrDefExt = NULL;
                ofn.lCustData = 0;
                ofn.lpfnHook = NULL;
                ofn.lpTemplateName = NULL;

                if (GetOpenFileName(&ofn)) {
                        mdiCreateDoc(szFileName, 0);
                }
                break;
        }
        case WM_MDITILE:
        case WM_MDICASCADE:
        case WM_MDIICONARRANGE:
                SendMessage(ghwndMdi, id, 0, 0);
                break;
        default:
                FORWARD_WM_COMMAND(NULL, id, hwndCtl, codeNotify, mdiFilterSendMessage);
                break;
        }
}

/*----------------------------------------------------------------------------*/
static void app_OnSize(
        HWND    hwnd,
        UINT    state,
        int     cx,
        int     cy)
{
        MoveWindow(ghwndMdi, 0, 0, cx, cy, TRUE);
}

/*----------------------------------------------------------------------------*/
static void app_OnInitMenu(
        HWND    hwnd,
        HMENU   hMenu)
{
        UINT    fuEnable;
        HWND    hwndActive;

        fuEnable = (IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_BITMAP)) ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem(hMenu, MENU_PASTE, fuEnable);
        hwndActive = mdiActiveDoc(ghwndMdi);
        if(hwndActive) {
                fuEnable = MF_ENABLED;
                EnableMenuItem(hMenu, MENU_CLOSE, fuEnable);
                EnableMenuItem(hMenu, MENU_CLOSEALL, fuEnable);
                FORWARD_WM_INITMENU(hwndActive, hMenu, SendMessage);
        } else {
                fuEnable = MF_GRAYED;
                EnableMenuItem(hMenu, MENU_CLOSE, fuEnable);
                EnableMenuItem(hMenu, MENU_CLOSEALL, fuEnable);
                EnableMenuItem(hMenu, MENU_SAVE, fuEnable);
                EnableMenuItem(hMenu, MENU_COPY, fuEnable);
                EnableMenuItem(hMenu, MENU_COMPRESS, fuEnable);
                EnableMenuItem(hMenu, MENU_DECOMPRESS, fuEnable);
        }
}

/*----------------------------------------------------------------------------*\
|   AppWndProc(hwnd, msg, wParam, lParam)                                    |
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
        HWND    hwnd,
        UINT    msg,
        WPARAM  wParam,
        LPARAM  lParam)
{
        PAINTSTRUCT     ps;
        HMENU   hmenu;
        HDC     hdc;

        switch (msg) {
        case WM_COMMAND:
                HANDLE_WM_COMMAND(hwnd, wParam, lParam, app_OnCommand);
                break;
        case WM_PALETTECHANGED:
                return mdiSendMessage(ghwndMdi, HWND_BROADCAST, msg, wParam, lParam);
        case WM_QUERYNEWPALETTE:
                return mdiSendMessage(ghwndMdi, NULL, msg, wParam, lParam);
        case WM_INITMENU:
                HANDLE_WM_INITMENU(hwnd, wParam, lParam, app_OnInitMenu);
                break;
        case WM_CREATE:
                hmenu = GetMenu(hwnd);
                ghwndMdi = mdiCreateClient(hwnd, GetSubMenu(hmenu, GetMenuItemCount(hmenu)-1));
                break;
        case WM_SIZE:
                HANDLE_WM_SIZE(hwnd, wParam, lParam, app_OnSize);
                break;
        case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        case WM_PAINT:
                hdc = BeginPaint(hwnd, &ps);
                EndPaint(hwnd, &ps);
                return 0;
        }
        return DefFrameProc(hwnd, ghwndMdi, msg, wParam, lParam);
}

/*----------------------------------------------------------------------------*/
static void mdi_OnInitMenu(
        HWND    hwnd,
        HMENU   hMenu)
{
        PDIBINFO        pdi;
        UINT    fuEnable;

        pdi = (PDIBINFO)(UINT)GetWindowLong(hwnd, 0);
        fuEnable = pdi->hdib ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem(hMenu, MENU_COPY,  fuEnable);
        EnableMenuItem(hMenu, MENU_SAVE,  fuEnable);

        EnableMenuItem(hMenu, MENU_DECOMPRESS, pdi->fCanDecompress ? MF_ENABLED : MF_GRAYED);
        EnableMenuItem(hMenu, MENU_COMPRESS,   pdi->fCanCompress ? MF_ENABLED : MF_GRAYED);

        CheckMenuItem(hMenu, MENU_ZOOMW,   gfStretchToWindow    ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, MENU_ZOOM1,   pdi->iZoom == ZOOM   ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, MENU_ZOOM2,   pdi->iZoom == ZOOM*2 ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, MENU_ZOOM12,  pdi->iZoom == ZOOM/2 ? MF_CHECKED : MF_UNCHECKED);
}

/*----------------------------------------------------------------------------*/
static BOOL mdi_OnCreate(
        HWND    hwnd,
        LPCREATESTRUCT lpCreateStruct)
{
        PDIBINFO        pdi;

        pdi = (PDIBINFO)LocalAlloc(LPTR, sizeof(DIBINFO));
        SetWindowLong(hwnd, 0, (LONG)(UINT)pdi);

        if (pdi == NULL)
                return FALSE;

        pdi->hwnd = hwnd;
        GetWindowText(hwnd, pdi->achFileName, sizeof(pdi->achFileName));

        if (!InitDib(pdi, (HANDLE)((LPMDICREATESTRUCT)lpCreateStruct->lpCreateParams)->lParam)) {
                LoadString(ghInstApp, IDS_NOOPEN, szStringBuffer, BUFSIZE);
                ErrMsg(szStringBuffer,(LPSTR)pdi->achFileName);
                return FALSE;
        }
        return TRUE;
}

/*----------------------------------------------------------------------------*/
static void mdi_OnCommand(
        HWND    hwnd,
        int     id,
        HWND    hwndCtl,
        UINT    codeNotify)
{
        PDIBINFO        pdi;
        COMPVARS        compvars;
        HANDLE  hdib;
        BOOL    fChoose;
#ifdef TIMEIT
        DWORD   dwStartTime;
        DWORD   dwTimeToCompress;
        char    buf[200];
#endif // TIMEIT

        pdi = (PDIBINFO)(UINT)GetWindowLong(hwnd, 0);
        switch (id) {
        case MENU_COMPRESS:
                compvars.cbSize = sizeof(COMPVARS);     // validate it
                compvars.dwFlags = 0;
                fChoose = ICCompressorChoose(
                        hwnd,           // parent window for dialog
                        0,              // no special boxes
                        pdi->lpbi,      // only if can compress this
                        0,              // !!! DibPtr(pdi->lpbi) -> PAVI
                        &compvars,      // data structure
                        NULL);          // title
                if (fChoose) {

                        StartWait();
                        UpdateWindow(ghwndMdi);
                        ICSetStatusProc(compvars.hic, 0, (LPARAM)(UINT)ghwndApp, PreviewStatusProc);

#ifdef TIMEIT
                        dwStartTime = timeGetTime();
#endif // TIMEIT
                        hdib = ICImageCompress(
                                compvars.hic,           // compressor to use
                                0,                      // flags
                                (LPBITMAPINFO)pdi->lpbi,// format to compress from
                                DibPtr(pdi->lpbi),      // bits to compress
                                NULL,                   // output format (default)
                                compvars.lQ,            // quality to use.
                                NULL);                  // size of output (whatever)
                        EndWait();
#ifdef TIMEIT
                        dwTimeToCompress = timeGetTime() - dwStartTime;
                        wsprintf(buf,
                                 "Time to compress was %d milliseconds",
                                 dwTimeToCompress);
                        MessageBox(NULL, buf, "icmapp", MB_OK);

#endif // TIMEIT
                        ICSetStatusProc(compvars.hic, 0, (LPARAM) (UINT) ghwndMdi, NULL);
                        ICCompressorFree(&compvars);
                        if (hdib)
                                mdiCreateDoc(pdi->achFileName, (LPARAM)(UINT)hdib);
                        else {
                                LoadString(ghInstApp, IDS_NOCOMPRESS, szStringBuffer, BUFSIZE);
                                ErrMsg(szStringBuffer);
                        }
                }
                break;
        case MENU_DECOMPRESS:
                StartWait();
#ifdef TIMEIT
                dwStartTime = timeGetTime();
#endif // TIMEIT
                hdib = ICImageDecompress(
                                NULL,                   // decompressor to use (any)
                                0,                      // flags
                                (LPBITMAPINFO)pdi->lpbi,// format to decompress from
                                DibPtr(pdi->lpbi),      // bits to decompress
                                NULL);                  // format to compress to. (any)
                EndWait();
#ifdef TIMEIT
                dwTimeToCompress = timeGetTime() - dwStartTime;
                wsprintf(buf,
                         "Time to decompress was %d milliseconds",
                         dwTimeToCompress);
                MessageBox(NULL, buf, "icmapp", MB_OK);
#endif // TIMEIT
                if (hdib)
                        mdiCreateDoc(pdi->achFileName, (LPARAM)(UINT)hdib);
                break;
        case MENU_ZOOMW:
                // toggle stretch to window
                gfStretchToWindow = !gfStretchToWindow;
                pdi->iZoom = 0;
                SizeWindowToImage(pdi);
                InvalidateRect(hwnd, NULL, TRUE);
                break;
        case MENU_ZOOM1:
        case MENU_ZOOM2:
        case MENU_ZOOM12:
                pdi->iZoom = id - MENU_ZOOM;
                SizeWindowToImage(pdi);
                InvalidateRect(hwnd, NULL, TRUE);
                break;
        case MENU_COPY:
                if (!OpenClipboard(hwnd))
                        break;
                EmptyClipboard();
                SetClipboardData(CF_DIB, (HANDLE)CopyHandle(pdi->hdib));
                CloseClipboard();
                break;
        case MENU_SAVE:
        {
                OPENFILENAME    ofn;
                char szBuffer[BUFSIZE];
                char szFileName[BUFSIZE];

                /* set filename to NULL if you are off the */
                /* clipboard, else set it to the filename  */
                LoadString(ghInstApp, IDS_CLIPBOARD, szStringBuffer, BUFSIZE);
                if (lstrcmpi(pdi->achFileName, szStringBuffer) == 0)
                        szFileName[0] = '\0';
                else
                        lstrcpy(szFileName, pdi->achFileName);

                FormatFilterString(IDS_SAVEFILTER, szBuffer);

                /* prompt user for file to save */
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hwnd;
                ofn.hInstance = NULL;
                ofn.lpstrFilter = szBuffer;
                ofn.lpstrCustomFilter = NULL;
                ofn.nMaxCustFilter = 0;
                ofn.nFilterIndex = 0;
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName);
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.lpstrTitle = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
                ofn.nFileOffset = 0;
                ofn.nFileExtension = 0;
                ofn.lpstrDefExt = NULL;
                ofn.lCustData = 0;
                ofn.lpfnHook = NULL;
                ofn.lpTemplateName = NULL;

                if (GetSaveFileName(&ofn))
                        WriteDIB(szFileName, pdi->hdib);
                break;
        }
        }
}

/*----------------------------------------------------------------------------*/
static void mdi_OnKey(
        HWND    hwnd,
        UINT    vk,
        BOOL    fDown,
        int     cRepeat,
        UINT    flags)
{
        if (fDown) {
                switch (vk) {
                case VK_UP:
                        FORWARD_WM_VSCROLL(hwnd, NULL, SB_LINEUP, 0, PostMessage);
                        break;
                case VK_DOWN:
                        FORWARD_WM_VSCROLL(hwnd, NULL, SB_LINEDOWN, 0, PostMessage);
                        break;
                case VK_PRIOR:
                        FORWARD_WM_VSCROLL(hwnd, NULL, SB_PAGEUP, 0, PostMessage);
                        break;
                case VK_NEXT:
                        FORWARD_WM_VSCROLL(hwnd, NULL, SB_PAGEDOWN, 0, PostMessage);
                        break;
                case VK_HOME:
                        FORWARD_WM_HSCROLL(hwnd, NULL, SB_PAGEUP, 0, PostMessage);
                        break;
                case VK_END:
                        FORWARD_WM_HSCROLL(hwnd, NULL, SB_PAGEDOWN, 0, PostMessage);
                        break;
                case VK_LEFT:
                        FORWARD_WM_HSCROLL(hwnd, NULL, SB_LINEUP, 0, PostMessage);
                        break;
                case VK_RIGHT:
                        FORWARD_WM_HSCROLL(hwnd, NULL, SB_LINEDOWN, 0, PostMessage);
                        break;
                }
        } else {
                switch (vk) {
                case VK_UP:
                case VK_DOWN:
                case VK_PRIOR:
                case VK_NEXT:
                        FORWARD_WM_VSCROLL(hwnd, NULL, SB_ENDSCROLL, 0, PostMessage);
                        break;
                case VK_HOME:
                case VK_END:
                case VK_LEFT:
                case VK_RIGHT:
                        FORWARD_WM_HSCROLL(hwnd, NULL, SB_ENDSCROLL, 0, PostMessage);
                        break;
                }
        }
}

/*----------------------------------------------------------------------------*/
static void mdi_OnVScroll(
        HWND    hwnd,
        HWND    hwndCtl,
        UINT    code,
        int     pos)
{
        int     iMax;
        int     iMin;
        int     iPos;
        int     dn;
        RECT    rc;

        GetScrollRange(hwnd, SB_VERT, &iMin, &iMax);
        iPos = GetScrollPos(hwnd, SB_VERT);
        GetClientRect(hwnd, &rc);
        switch (code) {
        case SB_LINEDOWN:
                dn =  rc.bottom / 16 + 1;
                break;
        case SB_LINEUP:
                dn = -rc.bottom / 16 + 1;
                break;
        case SB_PAGEDOWN:
                dn =  rc.bottom / 2  + 1;
                break;
        case SB_PAGEUP:
                dn = -rc.bottom / 2  + 1;
                break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
                dn = pos - iPos;
                break;
        default:
                dn = 0;
                break;
        }

        if (dn = BOUND(iPos + dn, iMin, iMax) - iPos) {
                ScrollWindow(hwnd, 0, -dn, NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, iPos + dn, TRUE);
                UpdateWindow(hwnd);
        }
}

/*----------------------------------------------------------------------------*/
static void mdi_OnHScroll(
        HWND    hwnd,
        HWND    hwndCtl,
        UINT    code,
        int     pos)
{
        int     iMax;
        int     iMin;
        int     iPos;
        int     dn;
        RECT    rc;

        GetScrollRange(hwnd, SB_HORZ, &iMin, &iMax);
        iPos = GetScrollPos(hwnd, SB_HORZ);
        GetClientRect(hwnd, &rc);
        switch (code) {
        case SB_LINEDOWN:
                dn =  rc.right / 16 + 1;
                break;
        case SB_LINEUP:
                dn = -rc.right / 16 + 1;
                break;
        case SB_PAGEDOWN:
                dn =  rc.right / 2  + 1;
                break;
        case SB_PAGEUP:
                dn = -rc.right / 2  + 1;
                break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
                dn = pos - iPos;
                break;
        default:
                dn = 0;
                break;
        }
        dn = ALIGNB(dn);
        if (dn = BOUND(iPos + dn, iMin, iMax) - iPos) {
                ScrollWindow(hwnd, -dn, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, iPos + dn, TRUE);
                UpdateWindow(hwnd);
        }
}

/*----------------------------------------------------------------------------*/
static BOOL mdi_OnEraseBkgnd(
        HWND    hwnd,
        HDC     hdc)
{
        PDIBINFO        pdi;

        SaveDC(hdc);

        pdi = (PDIBINFO)(UINT)GetWindowLong(hwnd, 0);
        ExcludeClipRect(hdc, pdi->rcDraw.left, pdi->rcDraw.top, pdi->rcDraw.right, pdi->rcDraw.bottom);
        FORWARD_WM_ERASEBKGND(hwnd, hdc, DefWindowProc);

        RestoreDC(hdc, -1);
        return 0L;
}

/*----------------------------------------------------------------------------*\
|   mdiDocProc(hwnd, msg, wParam, lParam)                                    |
|                                                                              |
|   Description:                                                               |
|       The window proc for a MDI child window                                 |
|                                                                              |
|   Arguments:                                                                 |
|       hwnd            window handle for the parent window                    |
|       msg       message number                                               |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
LRESULT CALLBACK mdiDocWndProc(
        HWND    hwnd,
        UINT    msg,
        WPARAM  wParam,
        LPARAM  lParam)
{
        PDIBINFO        pdi;
        PAINTSTRUCT     ps;
        HDC     hdc;
        UINT    uRealize;

        pdi = (PDIBINFO)(UINT)GetWindowLong(hwnd, 0);
        switch (msg) {
        case WM_CREATE:
                return HANDLE_WM_CREATE(hwnd, wParam, lParam, mdi_OnCreate);
        case WM_SIZE:
                InitSize(pdi);
                break;
        case WM_CLOSE:
                break;
        case WM_DESTROY:
                if (pdi->hdib)
                        GlobalFree(pdi->hdib);
                if (pdi->hdd)
                        DrawDibClose(pdi->hdd);
                LocalFree((HLOCAL)pdi);
                SetWindowLong(hwnd, 0, 0);
                break;
        case WM_INITMENU:
                HANDLE_WM_INITMENU(hwnd, wParam, lParam, mdi_OnInitMenu);
                break;
        case WM_COMMAND:
                HANDLE_WM_COMMAND(hwnd, wParam, lParam, mdi_OnCommand);
                break;
        case WM_KEYDOWN:
                HANDLE_WM_KEYDOWN(hwnd, wParam, lParam, mdi_OnKey);
                break;
        case WM_KEYUP:
                HANDLE_WM_KEYUP(hwnd, wParam, lParam, mdi_OnKey);
                break;
        case WM_VSCROLL:
                HANDLE_WM_VSCROLL(hwnd, wParam, lParam, mdi_OnVScroll);
                break;
        case WM_HSCROLL:
                HANDLE_WM_HSCROLL(hwnd, wParam, lParam, mdi_OnHScroll);
                break;
        case WM_CHILDACTIVATE:
        case WM_ACTIVATE:
                FORWARD_WM_QUERYNEWPALETTE(hwnd, SendMessage);
                break;
        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
                if (pdi == NULL || pdi->hdd == NULL)
                        break;

                hdc = GetDC(hwnd);
                if (uRealize = DrawDibRealize(pdi->hdd, hdc, (msg == WM_PALETTECHANGED)))
                        InvalidateRect(hwnd, NULL, TRUE);
                ReleaseDC(hwnd, hdc);
                return uRealize;
        case WM_ERASEBKGND:
                return HANDLE_WM_ERASEBKGND(hwnd, wParam, lParam, mdi_OnEraseBkgnd);
        case WM_PAINT:
                hdc = BeginPaint(hwnd, &ps);
                if (pdi->hdib) {
                        BOOL    fDrawDib;

                        SetWindowOrgEx(hdc, GetScrollPos(hwnd, SB_HORZ), GetScrollPos(hwnd, SB_VERT), NULL);
                        fDrawDib = DrawDibDraw(pdi->hdd,hdc,
                                        pdi->rcDraw.left,
                                        pdi->rcDraw.top,
                                        pdi->rcDraw.right -pdi->rcDraw.left,
                                        pdi->rcDraw.bottom-pdi->rcDraw.top,
                                        pdi->lpbi, NULL,
                                        pdi->rcSource.left,
                                        pdi->rcSource.top,
                                        pdi->rcSource.right -pdi->rcSource.left,
                                        pdi->rcSource.bottom-pdi->rcSource.top,
                                        (hwnd == mdiActiveDoc(ghwndMdi)) ? 0 : DDF_BACKGROUNDPAL);

                        if (!fDrawDib)
                                FillRect(hdc, &pdi->rcDraw, GetStockBrush(DKGRAY_BRUSH));
                }
                EndPaint(hwnd, &ps);
                return 0;
        }
        return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

/*----------------------------------------------------------------------------*\
|   AppInit (hInstance, hPrevInstance)                                         |
|                                                                              |
|   Description:                                                               |
|       This is called when the application is first loaded into               |
|       memory.  It performs all initialization that doesn't need to be done   |
|       once per instance.                                                     |
|                                                                              |
|   Arguments:                                                                 |
|       hPrevInstance   instance handle of previous instance                   |
|       hInstance       instance handle of current instance                    |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if successful, FALSE if not                                       |
|                                                                              |
\*----------------------------------------------------------------------------*/
static BOOL AppInit(
        HINSTANCE       hInst,
        HINSTANCE       hPrev,
        LPSTR   szCmd,
        int     sw)
{
        WNDCLASS        cls;
        WORD    wVer;
        char    szAppName[BUFSIZE];

        /* first let's make sure we are running on 1.1 */
        wVer = HIWORD(VideoForWindowsVersion());
        if (wVer < 0x010a) {
                char    szTitle[BUFSIZE];
                char    szMessage[BUFSIZE];

                LoadString(hInst, IDS_APPERR, szTitle, BUFSIZE);
                LoadString(hInst, IDS_OLDVFW, szMessage, BUFSIZE);

                /* oops, we are too old, blow out of here */
                MessageBeep(MB_ICONHAND);
                MessageBox(NULL, szMessage, szTitle, MB_OK | MB_ICONSTOP);
                return FALSE;
        }

        /* Save instance handle for DialogBox */
        ghInstApp = hInst;

        ghAccelApp = LoadAccelerators(ghInstApp, "AppAccel");

        LoadString(hInst, IDS_APPNAME, szAppName, BUFSIZE);

        if (!hPrev) {
                cls.hCursor = LoadCursor(NULL,IDC_ARROW);
                cls.hIcon = LoadIcon(hInst,"AppIcon");
                cls.lpszMenuName = "AppMenu";
                cls.lpszClassName = szAppName;
                cls.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE+1;
                cls.hInstance = hInst;
                cls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
                cls.lpfnWndProc = AppWndProc;
                cls.cbClsExtra = 0;
                cls.cbWndExtra = 0;
                if (!RegisterClass(&cls))
                        return FALSE;

                cls.hCursor = LoadCursor(NULL,IDC_ARROW);
                cls.hIcon = LoadIcon(hInst,"AppIcon");
                cls.lpszMenuName = NULL;
                cls.lpszClassName = DocClass;
                cls.hbrBackground = (HBRUSH)COLOR_WINDOW + 1;
                cls.hInstance = hInst;
                cls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
                cls.lpfnWndProc = mdiDocWndProc;
                cls.cbClsExtra = 0;
                cls.cbWndExtra = sizeof(LONG);
                if (!RegisterClass(&cls))
                        return FALSE;
        }

        ProInit(hPrev, hInst);

        ghwndApp = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

        /* Make window visible according to the way the app is activated */
        ShowWindow(ghwndApp, sw);

//      Needs to be parsed
//
//      if (szCmd && szCmd[0])
//              mdiCreateDoc(szCmd, 0);

        return TRUE;
}

/*----------------------------------------------------------------------------*\
|   WinMain(hInstance, hPrevInstance, lpszCmdLine, cmdShow)                  |
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
        HINSTANCE       hInstance,
        HINSTANCE       hPrevInstance,
        LPSTR   szCmdLine,
        int     sw)
{
        MSG     msg;

        if (!AppInit(hInstance, hPrevInstance, szCmdLine, sw))
                return FALSE;

        // Polling messages from event queue
        for (;;) {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                        if (msg.message == WM_QUIT)
                                break;
                        if (ghAccelApp && TranslateAccelerator(ghwndApp, ghAccelApp, &msg))
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
