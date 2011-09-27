//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  mixapp.c
//
//==========================================================================;

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdarg.h>

#include "mixapp.h"

#include "debug.h"


//
//  globals, no less
//
HINSTANCE   ghinst;
UINT        gfuAppOptions;

TCHAR       gszNull[]           = TEXT("");
TCHAR       gszCRLF[]           = TEXT("\r\n");

TCHAR       gszAppName[APP_MAX_APP_NAME_CHARS];




//
//
//
PTABBEDLISTBOX      gptlbLines;
HMIXER              ghmx;



BOOL                gfDisplayingControl     = FALSE;
DWORD               gdwControlID;
HWND                ghdlgControl;
MIXERLINE           gmxl;
MIXERCONTROL        gmxctrl;



//==========================================================================;
//
//  Application helper functions
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  void AppHourGlass
//
//  Description:
//      This function changes the cursor to that of the hour glass or
//      back to the previous cursor.
//
//      This function can be called recursively.
//
//  Arguments:
//      BOOL fHourGlass: TRUE if we need the hour glass.  FALSE if we need
//      the arrow back.
//
//  Return (void):
//      On return, the cursor will be what was requested.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

void FNGLOBAL AppHourGlass
(
    BOOL                    fHourGlass
)
{
    static HCURSOR  hcur;
    static UINT     uWaiting = 0;

    if (fHourGlass)
    {
        if (!uWaiting)
        {
            hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));
            ShowCursor(TRUE);
        }

        uWaiting++;
    }
    else
    {
        --uWaiting;

        if (!uWaiting)
        {
            ShowCursor(FALSE);
            SetCursor(hcur);
        }
    }
} // AppHourGlass()


//--------------------------------------------------------------------------;
//
//  int AppMsgBox
//
//  Description:
//      This function displays a message for the application in a standard
//      message box.
//
//      Note that this function takes any valid argument list that can
//      be passed to wsprintf. Because of this, the application must
//      remember to cast near string pointers to FAR when built for Win 16.
//      You will get a nice GP fault if you do not cast them correctly.
//
//  Arguments:
//      HWND hwnd: Handle to parent window for message box holding the
//      message.
//
//      UINT fuStyle: Style flags for MessageBox().
//
//      PTSTR pszFormat: Format string used for wvsprintf().
//
//  Return (int):
//      The return value is the result of MessageBox() function.
//
//  History:
//       2/13/93
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppMsgBox
(
    HWND                    hwnd,
    UINT                    fuStyle,
    PTSTR                  pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    //
    //  format and display the message..
    //
    va_start(va, pszFormat);
    wvsprintf(ach, pszFormat, va);
    va_end(va);

    n = MessageBox(hwnd, ach, gszAppName, fuStyle);

    return (n);
} // AppMsgBox()


//--------------------------------------------------------------------------;
//
//  int AppSetWindowText
//
//  Description:
//      This function formats a string and sets the specified window text
//      to the result.
//
//  Arguments:
//      HWND hwnd: Handle to window to receive the new text.
//
//      PTSTR pszFormat: Pointer to any valid format for wsprintf.
//
//  Return (int):
//      The return value is the number of bytes that the resulting window
//      text was.
//
//  History:
//       2/ 7/93
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppSetWindowText
(
    HWND                    hwnd,
    PTSTR                  pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    //
    //  format and display the string in the window...
    //
    va_start(va, pszFormat);
    n = wvsprintf(ach, pszFormat, va);
    va_end(va);

    SetWindowText(hwnd, ach);

    return (n);
} // AppSetWindowText()


//--------------------------------------------------------------------------;
//
//  void MixAppDebugLog
//
//  Description:
//      This function logs information to the debugger if the Debug Log
//      option is set. You can then run DBWin (or something similar)
//      to redirect the output whereever you want. Very useful for debugging
//      ACM drivers.
//
//  Arguments:
//      PTSTR pszFormat: Pointer to any valid format for wsprintf.
//
//  Return (void):
//      None.
//
//  History:
//       2/ 7/93
//
//--------------------------------------------------------------------------;

void FNCGLOBAL MixAppDebugLog
(
    PTSTR                  pszFormat,
    ...
)
{
    static  TCHAR   szDebugLogSeparator[] = TEXT("=============================================================================\r\n");

    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];


    //
    //  !!! UNICODE !!!
    //
    //
    if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
    {
        if (NULL == pszFormat)
        {
            OutputDebugString(szDebugLogSeparator);
            return;
        }

        //
        //  format and display the string in a message box...
        //
        va_start(va, pszFormat);
        wvsprintf(ach, pszFormat, va);
        va_end(va);

        OutputDebugString(ach);
    }
} // MixAppDebugLog()


//--------------------------------------------------------------------------;
//
//  int MEditPrintF
//
//  Description:
//      This function is used to print formatted text into a Multiline
//      Edit Control as if it were a standard console display. This is
//      a very easy way to display small amounts of text information
//      that can be scrolled and copied to the clip-board.
//
//  Arguments:
//      HWND hedit: Handle to a Multiline Edit control.
//
//      PTSTR pszFormat: Pointer to any valid format for wsprintf. If
//      this argument is NULL, then the Multiline Edit Control is cleared
//      of all text.
//
//
//  Return (int):
//      Returns the number of characters written into the edit control.
//
//  Notes:
//      The pszFormat string can contain combinations of escapes that
//      modify the default behaviour of this function. Escapes are single
//      character codes placed at the _beginning_ of the format string.
//
//      Current escapes defined are:
//
//      ~   :   Suppresses the default CR/LF added to the end of the
//              printed line. Since the most common use of this function
//              is to output a whole line of text with a CR/LF, that is
//              the default.
//
//      `   :   Suppresses logging to the debug terminal (regardless of
//              the global debug log options flag).
//
//  History:
//      05/16/93
//
//--------------------------------------------------------------------------;

int FNCGLOBAL MEditPrintF
(
    HWND                    hedit,
    PTSTR                   pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_RC_CHARS];
    int         n;
    BOOL        fCRLF;
    BOOL        fDebugLog;

    //
    //  default the escapes
    //
    fCRLF     = TRUE;
    fDebugLog = (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions));


    //
    //  if the pszFormat argument is NULL, then just clear all text in
    //  the edit control..
    //
    if (NULL == pszFormat)
    {
        SetWindowText(hedit, gszNull);

        if (fDebugLog)
            MixAppDebugLog(NULL);

        return (0);
    }

    //
    //  format and display the string in the window... first search for
    //  escapes to modify default behaviour.
    //
    for (;;)
    {
        switch (*pszFormat)
        {
            case '~':
                fCRLF = FALSE;
                pszFormat++;
                continue;

            case '`':
                fDebugLog = FALSE;
                pszFormat++;
                continue;
        }

        break;
    }

    va_start(va, pszFormat);
    n = wvsprintf(ach, pszFormat, va);
    va_end(va);

    Edit_SetSel(hedit, (WPARAM)-1, (LPARAM)-1);
    Edit_ReplaceSel(hedit, ach);

    if (fDebugLog)
        MixAppDebugLog(ach);

    if (fCRLF)
    {
        Edit_SetSel(hedit, (WPARAM)-1, (LPARAM)-1);
        Edit_ReplaceSel(hedit, gszCRLF);

        if (fDebugLog)
            MixAppDebugLog(gszCRLF);
    }

    return (n);
} // MEditPrintF()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  HMIXER MixAppNewDevice
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HMIXER hmxCur:
//
//      UINT uMxId:
//
//  Return (HMIXER):
//
//  History:
//      09/25/93
//
//--------------------------------------------------------------------------;

HMIXER FNGLOBAL MixAppNewDevice
(
    HWND                    hwnd,
    HMIXER                  hmxCur,
    UINT                    uMxId
)
{
    MMRESULT            mmr;
    HMIXER              hmx;
    MIXERCAPS           mxcaps;


    //
    //
    //
    mmr = mixerGetDevCaps(uMxId, &mxcaps, sizeof(mxcaps));
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(NULL, MB_OK | MB_ICONEXCLAMATION,
                "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!",
                uMxId, mmr);
        return (hmxCur);
    }


    //
    //
    //
    mmr = mixerOpen(&hmx, uMxId, (DWORD)(UINT)hwnd, 0L, CALLBACK_WINDOW);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(NULL, MB_OK | MB_ICONEXCLAMATION,
                "mixerOpen() failed on uMxId=%u, mmr=%u!",
                uMxId, mmr);
        return (hmxCur);
    }


    if (NULL != hmxCur)
    {
        mmr = mixerClose(hmxCur);
        if (MMSYSERR_NOERROR != mmr)
        {
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "mixerClose() failed on hmx=%.04Xh, mmr=%u!",
                      hmxCur, mmr);
        }

        hmxCur = NULL;
    }


    AppSetWindowText(hwnd, "Mixer Device: %s", (LPSTR)mxcaps.szPname);

    return (hmx);
} // MixAppNewDevice()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppDisplayDevCaps
//
//  Description:
//
//
//  Arguments:
//      HWND hedit:
//
//      UINT uMxId:
//
//      LPMIXERCAPS pmxcaps:
//
//  Return (BOOL):
//
//  History:
//      09/25/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppDisplayDevCaps
(
    HWND                    hedit,
    UINT                    uMxId,
    LPMIXERCAPS             pmxcaps
)
{
    static TCHAR    szDisplayTitle[]    = TEXT("[Mixer Device Capabilities]\r\n");

    SetWindowRedraw(hedit, FALSE);

    //
    //
    //
    MEditPrintF(hedit, NULL);
    MEditPrintF(hedit, szDisplayTitle);

    MEditPrintF(hedit, "%18s: %u", (LPTSTR)"Device Id", uMxId);

    MEditPrintF(hedit, "%18s: %u", (LPTSTR)"Manufacturer Id", pmxcaps->wMid);
    MEditPrintF(hedit, "%18s: %u", (LPTSTR)"Product Id", pmxcaps->wPid);
    MEditPrintF(hedit, "%18s: %u.%.02u", (LPTSTR)"Driver Version",
                (pmxcaps->vDriverVersion >> 8),
                (pmxcaps->vDriverVersion & 0x00FF));
    MEditPrintF(hedit, "%18s: '%s'", (LPTSTR)"Device Name", (LPSTR)pmxcaps->szPname);
    MEditPrintF(hedit, "%18s: %.08lXh", (LPTSTR)"Support Flags", pmxcaps->fdwSupport);
    MEditPrintF(hedit, "%18s: %lu", (LPTSTR)"Destinations", pmxcaps->cDestinations);

    SetWindowRedraw(hedit, TRUE);

    return (TRUE);
} // MixAppDisplayDevCaps()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcDevice
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      UINT uMsg:
//
//      WPARAM wParam:
//
//      LPARAM lParam:
//
//  Return (BOOL):
//
//  History:
//      09/25/93
//
//--------------------------------------------------------------------------;

BOOL CALLBACK MixAppDlgProcDevice
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
)
{
    UINT                cMixerDevs;
    UINT                u;
    UINT                uMxId;
    UINT                uId;
    UINT                uCmd;
    HWND                hcb;
    HWND                hedit;
    MIXERCAPS           mxcaps;

    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            hedit = GetDlgItem(hwnd, IDD_MADEVICE_EDIT_CAPABILITIES);
            SetWindowFont(hedit, GetStockFont(ANSI_FIXED_FONT), FALSE);

            SetWindowLong(hwnd, DWL_USER, lParam);

            uMxId = (UINT)lParam;

            //
            //
            //
            hcb = GetDlgItem(hwnd, IDD_MADEVICE_COMBO_DEVICE);

            cMixerDevs = mixerGetNumDevs();
            for (u = 0; u < cMixerDevs; u++)
            {
                mixerGetDevCaps(u, &mxcaps, sizeof(mxcaps));
                ComboBox_AddString(hcb, mxcaps.szPname);

                if (uMxId == u)
                {
                    hedit = GetDlgItem(hwnd, IDD_MADEVICE_EDIT_CAPABILITIES);
                    MixAppDisplayDevCaps(hedit, uMxId, &mxcaps);
                }
            }

            ComboBox_SetCurSel(hcb, uMxId);
            return (TRUE);


        case WM_COMMAND:
            uId = GET_WM_COMMAND_ID(wParam, lParam);

            switch (uId)
            {
                case IDOK:
                    hcb   = GetDlgItem(hwnd, IDD_MADEVICE_COMBO_DEVICE);
                    uMxId = ComboBox_GetCurSel(hcb);
                    if (CB_ERR != uMxId)
                    {
                        EndDialog(hwnd, uMxId);
                        break;
                    }

                    // -- fall through -- //

                case IDCANCEL:
                    uMxId = (UINT)GetWindowLong(hwnd, DWL_USER);
                    EndDialog(hwnd, uMxId);
                    break;


                case IDD_MADEVICE_COMBO_DEVICE:
                    uCmd = GET_WM_COMMAND_CMD(wParam, lParam);
                    hcb  = GET_WM_COMMAND_HWND(wParam, lParam);
                    switch (uCmd)
                    {
                        case CBN_SELCHANGE:
                            uMxId = ComboBox_GetCurSel(hcb);
                            if (CB_ERR == uMxId)
                                break;

                            mixerGetDevCaps(uMxId, &mxcaps, sizeof(mxcaps));

                            hedit = GetDlgItem(hwnd, IDD_MADEVICE_EDIT_CAPABILITIES);
                            MixAppDisplayDevCaps(hedit, uMxId, &mxcaps);
                            break;
                    }
            }
            return (TRUE);
    }

    return (FALSE);
} // MixAppDlgProcDevice()



//==========================================================================;
//
//  Main application window handling code...
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  LRESULT AppInitMenuPopup
//
//  Description:
//      This function handles the WM_INITMENUPOPUP message. This message
//      is sent to the window owning the menu that is going to become
//      active. This gives an application the ability to modify the menu
//      before it is displayed (disable/add items, etc).
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_INITMENUPOPUP
//      message.
//
//      HMENU hmenu: Handle to the menu that is to become active.
//
//      int nItem: Specifies the zero-based relative position of the menu
//      item that invoked the popup menu.
//
//      BOOL fSysMenu: Specifies whether the popup menu is a System menu
//      (TRUE) or it is not a System menu (FALSE).
//
//  Return (LRESULT):
//      Returns zero if the message is processed.
//
//  History:
//       1/ 2/93
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppInitMenuPopup
(
    HWND                    hwnd,
    HMENU                   hmenu,
    int                     nItem,
    BOOL                    fSysMenu
)
{
    int                 n;
    BOOL                f;

    DPF(1, "AppInitMenuPopup(hwnd=%Xh, hmenu=%Xh, nItem=%d, fSysMenu=%d)",
            hwnd, hmenu, nItem, fSysMenu);

    //
    //  if the system menu is what got hit, succeed immediately... this
    //  application has no stuff in the system menu.
    //
    if (fSysMenu)
        return (0L);

    //
    //  initialize the menu that is being 'popped up'
    //
    switch (nItem)
    {
        case APP_MENU_ITEM_FILE:
            f = (0 != mixerGetNumDevs());
            EnableMenuItem(hmenu, IDM_FILE_MIXER_DEVICE, (UINT)(f ? MF_ENABLED : MF_GRAYED));

            f = (0 != (gfuAppOptions & APP_OPTF_DEBUGLOG));
            CheckMenuItem(hmenu, IDM_FILE_DEBUG_LOG, (UINT)(f ? MF_CHECKED : MF_UNCHECKED));
            break;

        case APP_MENU_ITEM_VIEW:
            n = ListBox_GetCurSel(gptlbLines->hlb);
            f = (LB_ERR != n);

            EnableMenuItem(hmenu, IDM_VIEW_LINE_INFO,  (UINT)(f ? MF_ENABLED : MF_GRAYED));
            EnableMenuItem(hmenu, IDM_VIEW_LINE_CONTROLS,  (UINT)(f ? MF_ENABLED : MF_GRAYED));
            break;
    }

    //
    //  we processed the message--return 0...
    //
    return (0L);
} // AppInitMenuPopup()


//--------------------------------------------------------------------------;
//
//  LRESULT AppCommand
//
//  Description:
//      This function handles the WM_COMMAND message.
//
//  Arguments:
//      HWND hwnd: Handle to window receiving the WM_COMMAND message.
//
//      int nId: Control or menu item identifier.
//
//      HWND hwndCtl: Handle of control if the message is from a control.
//      This argument is NULL if the message was not generated by a control.
//
//      UINT uCode: Notification code. This argument is 1 if the message
//      was generated by an accelerator. If the message is from a menu,
//      this argument is 0.
//
//  Return (LRESULT):
//      Returns zero if the message is processed.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppCommand
(
    HWND                    hwnd,
    int                     nId,
    HWND                    hwndCtl,
    UINT                    uCode
)
{
    int                 n;
    UINT                uMxId;
    LRESULT             lr;

    switch (nId)
    {
        case IDM_FILE_MIXER_DEVICE:
            mixerGetID((HMIXEROBJ)ghmx, &uMxId, MIXER_OBJECTF_HMIXER);
            n = DialogBoxParam(ghinst, DLG_MIXAPP_DEVICE, hwnd, (DLGPROC)MixAppDlgProcDevice, uMxId);
            if ((UINT)n != uMxId)
            {
                uMxId = (UINT)n;
                ghmx  = MixAppNewDevice(hwnd, ghmx, uMxId);

                MixAppRefreshLineList(hwnd, gptlbLines);
            }
            break;

        case IDM_FILE_FONT:
            MixAppChooseFont(hwnd);
            break;

        case IDM_FILE_DEBUG_LOG:
            gfuAppOptions ^= APP_OPTF_DEBUGLOG;
            break;

        case IDM_FILE_ABOUT:
            DialogBox(ghinst, DLG_ABOUT, hwnd, (DLGPROC)AboutDlgProc);
            break;

        case IDM_FILE_EXIT:
            FORWARD_WM_CLOSE(hwnd, SendMessage);
            break;


        case IDM_VIEW_LINE_INFO:
            n  = ListBox_GetCurSel(gptlbLines->hlb);
            if (LB_ERR == n)
                break;

            lr = ListBox_GetItemData(gptlbLines->hlb, n);
            DialogBoxParam(ghinst, DLG_LINEINFO, hwnd, (DLGPROC)MixAppDlgProcLineInfo, lr);
            break;

        case IDM_VIEW_LINE_CONTROLS:
            n  = ListBox_GetCurSel(gptlbLines->hlb);
            if (LB_ERR == n)
                break;

            lr = ListBox_GetItemData(gptlbLines->hlb, n);
            DialogBoxParam(ghinst, DLG_LINECONTROLS, hwnd, (DLGPROC)MixAppDlgProcLineControls, lr);
            break;


        case IDM_UPDATE:
            MixAppRefreshLineList(hwnd, gptlbLines);
            break;


        case IDD_APP_LIST_LINES:
            switch (uCode)
            {
                case LBN_SELCHANGE:
                    break;

                case LBN_DBLCLK:
                    n  = ListBox_GetCurSel(hwndCtl);
                    if (LB_ERR == n)
                        break;

                    lr = ListBox_GetItemData(hwndCtl, n);
                    if (GetKeyState(VK_CONTROL) < 0)
                    {
                        DialogBoxParam(ghinst, DLG_LINEINFO, hwnd, (DLGPROC)MixAppDlgProcLineInfo, lr);
                    }
                    else
                    {
                        DialogBoxParam(ghinst, DLG_LINECONTROLS, hwnd, (DLGPROC)MixAppDlgProcLineControls, lr);
                    }
                    break;
            }
            break;
    }

    return (0L);
} // AppCommand()


//--------------------------------------------------------------------------;
//
//  LRESULT AppSize
//
//  Description:
//      This function handles the WM_SIZE message for the application's
//      window. This message is sent to the application window after the
//      size has changed (but before it is painted).
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_SIZE message.
//
//      UINT fuSizeType: Specifies the type of resizing requested. This
//      argument is one of the following: SIZE_MAXIMIZED, SIZE_MINIMIZED,
//      SIZE_RESTORED, SIZE_MAXHIDE, or SIZE_MAXSHOW.
//
//      int nWidth: Width of the new client area for the window.
//
//      int nHeight: Height of the new client area for the window.
//
//  Return (LRESULT):
//      Returns zero if the application processes the message.
//
//  History:
//       2/ 5/93
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppSize
(
    HWND                    hwnd,
    UINT                    fuSizeType,
    int                     nWidth,
    int                     nHeight
)
{
    RECT                rc;

    DPF(1, "AppSize(hwnd=%Xh, fuSizeType=%u, nWidth=%d, nHeight=%d)",
            hwnd, fuSizeType, nWidth, nHeight);

    //
    //  unless this application is the one being resized then don't waste
    //  time computing stuff that doesn't matter. this applies to being
    //  minimized also because this application does not have a custom
    //  minimized state.
    //
    if ((SIZE_RESTORED != fuSizeType) && (SIZE_MAXIMIZED != fuSizeType))
        return (0L);


    //
    //  size the devices listbox to be the total size of the client area--
    //  inflate the rect by one so borders are not visible. note that
    //  we need to leave room at the top for the title text which is one
    //  line of text in height...
    //
    GetClientRect(hwnd, &rc);
    InflateRect(&rc, 1, 1);


    TlbMove(gptlbLines, &rc, FALSE);


    //
    //  we processed the message..
    //
    return (0L);
} // AppSize()


//--------------------------------------------------------------------------;
//
//  LRESULT AppPaint
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//  Return (LRESULT):
//
//  History:
//      05/11/93
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppPaint
(
    HWND                    hwnd
)
{
    PAINTSTRUCT         ps;

    //
    //
    //
    BeginPaint(hwnd, &ps);

    TlbPaint(gptlbLines, hwnd, ps.hdc);

    EndPaint(hwnd, &ps);

    //
    //  we processed the message
    //
    return (0L);
} // AppPaint()


//--------------------------------------------------------------------------;
//
//  LRESULT AppWndProc
//
//  Description:
//      This is the main application window procedure.
//
//  Arguments:
//      HWND hwnd: Handle to window.
//
//      UINT uMsg: Message being sent to the window.
//
//      WPARAM wParam: Specific argument to message.
//
//      LPARAM lParam: Specific argument to message.
//
//  Return (LRESULT):
//      The return value depends on the message that is being processed.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

LRESULT CALLBACK AppWndProc
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    LRESULT             lr;

    switch (uMsg)
    {
        case WM_CREATE:
            lr = HANDLE_WM_CREATE(hwnd, wParam, lParam, AppCreate);
            return (lr);

        case WM_INITMENUPOPUP:
            HANDLE_WM_INITMENUPOPUP(hwnd, wParam, lParam, AppInitMenuPopup);
            return (0L);

        case WM_COMMAND:
            lr = HANDLE_WM_COMMAND(hwnd, wParam, lParam, AppCommand);
            return (lr);

        case WM_SIZE:
            //
            //  handle what we want for sizing, and then always call the
            //  default handler...
            //
            HANDLE_WM_SIZE(hwnd, wParam, lParam, AppSize);
            break;

        case WM_PAINT:
            HANDLE_WM_PAINT(hwnd, wParam, lParam, AppPaint);
            break;

        case WM_QUERYENDSESSION:
            lr = HANDLE_WM_QUERYENDSESSION(hwnd, wParam, lParam, AppQueryEndSession);
            return (lr);

        case WM_ENDSESSION:
            HANDLE_WM_ENDSESSION(hwnd, wParam, lParam, AppEndSession);
            return (0L);

        case WM_CLOSE:
            HANDLE_WM_CLOSE(hwnd, wParam, lParam, AppClose);
            return (0L);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0L);


        case MM_MIXM_LINE_CHANGE:
            lr = MixAppLineChange(hwnd, (HMIXER)wParam, lParam);
            return (lr);

        case MM_MIXM_CONTROL_CHANGE:
            lr = MixAppControlChange(hwnd, (HMIXER)wParam, lParam);
            return (lr);
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
} // AppWndProc()


//==========================================================================;
//
//  Main entry and message dispatching code
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  int WinMain
//
//  Description:
//      This function is called by the system as the initial entry point
//      for a Windows application.
//
//  Arguments:
//      HINSTANCE hinst: Identifies the current instance of the
//      application.
//
//      HINSTANCE hinstPrev: Identifies the previous instance of the
//      application (NULL if first instance). For Win 32, this argument
//      is _always_ NULL.
//
//      LPSTR pszCmdLine: Points to null-terminated unparsed command line.
//      This string is strictly ANSI regardless of whether the application
//      is built for Unicode. To get the Unicode equivalent call the
//      GetCommandLine() function (Win 32 only).
//
//      int nCmdShow: How the main window for the application is to be
//      shown by default.
//
//  Return (int):
//      Returns result from WM_QUIT message (in wParam of MSG structure) if
//      the application is able to enter its message loop. Returns 0 if
//      the application is not able to enter its message loop.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

int PASCAL WinMain
(
    HINSTANCE               hinst,
    HINSTANCE               hinstPrev,
    LPSTR                   pszCmdLine,
    int                     nCmdShow
)
{
    int                 nResult;
    HWND                hwnd;
    MSG                 msg;
    HACCEL              haccl;

    // * * * temp  bodge * * *
    {
      HKEY htemp;
      RegOpenKey(HKEY_LOCAL_MACHINE, "A Value That Does Not Exist", &htemp);
    }
    // * * * * * * * * * * * *

    //
    //  our documentation states that WinMain is supposed to return 0 if
    //  we do not enter our message loop--so assume the worst...
    //
    nResult = 0;

    //
    //  make our instance handle global for convenience..
    //
    ghinst = hinst;

    //
    //  init some stuff, create window, etc.. note the explicit cast of
    //  pszCmdLine--this is to mute a warning (and an ugly ifdef) when
    //  compiling for Unicode. see AppInit() for more details.
    //
    hwnd = AppInit(hinst, hinstPrev, (LPTSTR)pszCmdLine, nCmdShow);
    if (hwnd)
    {
        haccl = LoadAccelerators(hinst, ACCEL_APP);

        //
        //  dispatch messages
        //
        while (GetMessage(&msg, NULL, 0, 0))
        {
            //
            //  do all the special stuff required for this application
            //  when dispatching messages..
            //
            if (!TranslateAccelerator(hwnd, haccl, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        //
        //  return result of WM_QUIT message.
        //
        nResult = (int)msg.wParam;
    }

    //
    //  shut things down, clean up, etc.
    //
    nResult = AppExit(hinst, nResult);

    return (nResult);
} // WinMain()
