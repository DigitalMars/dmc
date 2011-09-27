//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
//  A PARTICULAR PURPOSE.
//
//  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  mciapp.c
//
//  Description:
//      This is a sample application that demonstrates how to use the
//      Media Control Interface (MCI) in Windows. This application is
//      also useful as an MCI device tester.
//
//  History:
//       2/ 6/93    created.
//
//==========================================================================;

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commdlg.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "appport.h"
#include "app.h"
#include "mciapp.h"

#include "debug.h"


//
//
//
BOOL            gfExecuting;
BOOL            gfAbortExec;


HWND            ghwndDevices;

TCHAR           gszNone[]           = TEXT("(none)");


//--------------------------------------------------------------------------;
//
//  BOOL MciAppFileSaveModified
//
//  Description:
//      This function tests if the current script has been modified, and
//      if it has it gives the option of saving the file.
//
//      NOTE! This function does *NOT* clear the modified bit for the
//      script window. The calling function must do this if necessary.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PTSTR pszFilePath: Pointer to null terminated file path of current
//      script. This buffer will receive the new file path if one is chosen.
//
//      PTSTR pszFileTitle: Pointer to null terminated file title of
//      current script. This buffer will receive the new file title if one
//      is chosen.
//
//  Return (BOOL):
//      Returns TRUE if the calling function should continue--the file was
//      either saved or the user does not wish to save it. Returns FALSE
//      if the calling function should cancel its operation--the user
//      wants to keep the data, but it has not been saved.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppFileSaveModified
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
    BOOL    f;
    int     n;
    HWND    hwndScript;

    //
    //  check if the contents of the script window have been modified--if
    //  they have then ask the user if they want to save the current
    //  contents...
    //
    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    f = Edit_GetModify(hwndScript);
    if (f)
    {
        //
        //  display an appropriate message box asking for the user's opinion
        //
        n = AppMsgBoxId(hwnd, MB_YESNOCANCEL | MB_ICONQUESTION,
                        IDS_MCI_SCRIPT_CHANGED, (LPSTR)pszFilePath);
        switch (n)
        {
            case IDYES:
                f = AppFileSave(hwnd, pszFilePath, pszFileTitle, FALSE);
                if (f)
                    break;

                // -- fall through --

            case IDCANCEL:
                //
                //  don't continue!
                //
                return (FALSE);

            case IDNO:
                break;
        }
    }

    //
    //  ok to continue...
    //
    return (TRUE);
} // MciAppFileSaveModified()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppFileNew
//
//  Description:
//      This function simply clears the script window. If a modified script
//      will be destroyed, then the user is asked if the script should be
//      saved first.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PTSTR pszFilePath: Pointer to null terminated file path of current
//      script. This buffer will receive the newly initialized file path.
//
//      PTSTR pszFileTitle: Pointer to null terminated file title of
//      current script. This buffer will receive the newly initialized file
//      title.
//
//  Return (BOOL):
//      The return value is TRUE if the script window was cleared. It is
//      FALSE if the user canceled the operation.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppFileNew
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
    BOOL    f;
    HWND    hwndScript;

    //
    //  test for a modified script first...
    //
    f = MciAppFileSaveModified(hwnd, pszFilePath, pszFileTitle);
    if (!f)
        return (FALSE);

    //
    //  blow away all the text and set the modified bit to 'NOT'
    //
    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    SetWindowText(hwndScript, gszNull);
    Edit_SetModify(hwndScript, FALSE);

    //
    //  success
    //
    return (TRUE);
} // MciAppFileNew()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppFileOpen
//
//  Description:
//      This function opens the specified text file and copies the contents
//      of the file into the script edit control.
//
//      NOTE! This function does NOT check for a modified script! It is
//      assumed that the calling function took care of everything before
//      calling this function.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PTSTR pszFilePath: Pointer to null terminated file path to open as
//      an MCI script. This buffer will be returned with a fully qualified
//      path of the file that was opened (if successful).
//
//      PTSTR pszFileTitle: Pointer to buffer to receive the file title of
//      the newly opened file. This buffer does not need to be initialized.
//
//  Return (BOOL):
//      The return value is TRUE if the function is successful. It is FALSE
//      if an error occurred. If an error does occur, then the contents
//      of the script window, pszFilePath and pszFileTitle will remain
//      unchanged.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppFileOpen
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
#ifdef UNICODE
    HANDLE      hf;
#else
    #define SEEK_SET        0       // flags for _lseek
    #define SEEK_CUR        1
    #define SEEK_END        2

    HFILE       hf;
    OFSTRUCT    of;
#endif
    HWND        hwndScript;
    UINT        uFileLen;
    LPTSTR      psz;
    BOOL        fReturn;

    //
    //  open the file for reading..
    //
#ifdef UNICODE
    hf = CreateFile(pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
                    OPEN_EXISTING, 0, 0);
    if (INVALID_HANDLE_VALUE == hf)
        return (FALSE);
#else
    of.cBytes = sizeof(of);
    hf = OpenFile(pszFilePath, &of, OF_READ);
    if (HFILE_ERROR == hf)
        return (FALSE);
#endif

    //
    //  assume the worst
    //
    fReturn = FALSE;

    //
    //  determine the length in _bytes_ of the file--note that win 16 is
    //  limited to 32k so big files get truncated... improperly.
    //
    //  !!! need to handle files that are too big better !!!
    //
#ifdef WIN32
    uFileLen = (UINT)GetFileSize((HANDLE)hf, NULL);
#else
    uFileLen = (UINT)_llseek(hf, 0L, SEEK_END);
    _llseek(hf, 0L, SEEK_SET);
#endif


    //
    //  now read the contents of the file into a buffer--display an hour
    //  glass in case the file is large and/or we are reading from a slow
    //  device... note that the memory allocation may take some time if
    //  the pager is in meltdown mode.
    //
    AppHourGlass(TRUE);
    for (;;)
    {
        //
        //  allocate enough memory to hold the complete image of the file
        //  plus one character for good measure (the forced null termination
        //  could fault if don't add one char to size).
        //
        psz = GlobalAllocPtr(GMEM_MOVEABLE, uFileLen + sizeof(TCHAR));
        if (NULL == psz)
            break;

        //
        //  read the file and copy the contents into the script window
        //
#ifdef UNICODE
        fReturn = ReadFile(hf, psz, uFileLen, (LPDWORD)&uFileLen, NULL);
        if (!fReturn)
            break;
#else
        uFileLen = _lread(hf, psz, uFileLen);
        if ((UINT)-1 == uFileLen)
            break;
#endif

        //
        //  make sure the text gets null terminated, then shove it into
        //  the script window.
        //
        psz[uFileLen / sizeof(TCHAR)] = '\0';

        hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
#ifdef UNICODE
        //
        //  determine whether the text is Unicode or ANSI: Unicode files
        //  all start with a Byte Order Mark (BOM) that will be 0xFEFF in
        //  the proper Endian for the file.
        //
        //  !!! this application only deals with ANSI and Little Endian
        //      Unicode files--should handle more gracefully if not...
        //
        if ((uFileLen >= sizeof(TCHAR)) && (psz[0] == 0xFEFF))
        {
            //
            //  set the window text as Unicode--note that we do not send
            //  the BOM, the edit control doesn't want it...
            //
            gfuAppOptions |= APP_OPTF_UNICODE;
            SetWindowTextW(hwndScript, (LPCWSTR)&psz[1]);
        }
        else
        {
            gfuAppOptions &= ~APP_OPTF_UNICODE;
            SetWindowTextA(hwndScript, (LPCSTR)psz);
        }
#else
        SetWindowText(hwndScript, (LPCTSTR)psz);
#endif

        //
        //  now return the fully qualified path and title for the file
        //!!!
#ifndef UNICODE
        lstrcpy(pszFilePath, of.szPathName);
#endif
        if (NULL != pszFileTitle)
        {
            AppGetFileTitle(pszFilePath, pszFileTitle);
        }

        fReturn = TRUE;
        break;
    }
    AppHourGlass(FALSE);


    //
    //  free memory (if allocated) and close the file. return the result
    //  of our attempt...
    //
    if (psz)
        GlobalFreePtr(psz);

#ifdef UNICODE
    CloseHandle(hf);
#else
    _lclose(hf);
#endif


    //
    //  !!! before returning, we really should try to display a error
    //      message... memory error, etc..
    //
    return (fReturn);
} // MciAppFileOpen()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppFileSave
//
//  Description:
//      This function saves the current script to the specified file.
//
//      NOTE! This function does NOT bring up a save file chooser dialog
//      if the file path is invalid. The calling function is responsible
//      for making sure the file path is valid before calling this function.
//
//      This function also does NOT modify the 'modified' bit of the script
//      window. This is up to the calling function.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PCTSTR pszFilePath: Pointer to null terminated file path to save
//      the script to.
//
//  Return (BOOL):
//      The return value is TRUE if the function is successful. It is FALSE
//      if an error occurred. If an error does occur, then the contents
//      of the script window was not saved.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppFileSave
(
    HWND            hwnd,
    PCTSTR          pszFilePath
)
{
#ifdef UNICODE
    HANDLE      hf;
#else
    HFILE       hf;
    OFSTRUCT    of;
#endif
    HWND        hwndScript;
    UINT        uFileLen;
    DWORD       cbBytes;
    LPTSTR      psz;
    BOOL        fReturn;

    //
    //  create the new file--if it already exists, this will open and init
    //  it to zero length.
    //
#ifdef UNICODE
    hf = CreateFile(pszFilePath, GENERIC_WRITE, FILE_SHARE_READ,
                    NULL, CREATE_ALWAYS, 0, 0);
    if (INVALID_HANDLE_VALUE == hf)
        return (FALSE);
#else
    of.cBytes = sizeof(of);
    hf = OpenFile(pszFilePath, &of, OF_CREATE);
    if (HFILE_ERROR == hf)
        return (FALSE);
#endif

    //
    //  assume the worst
    //
    fReturn = FALSE;

    //
    //  get the length in bytes of the script--we add 1 to the result
    //  because GetWindowTextLength returns the length in bytes NOT including
    //  the null terminator.
    //
    //  !!! UNICODE !!!
    //
    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    uFileLen = (UINT)GetWindowTextLength(hwndScript) + 1;

    //!!! nFileLen = nFileLen*sizeof(TCHAR);  We write ASCII

    //
    //  allocate a buffer to hold the script text, get the text, and write
    //  it out. display an hour glass in case the file is large and/or we
    //  are writing to a slow device...
    //
    AppHourGlass(TRUE);
    for (;;)
    {
        //
        //  allocate enough memory to hold the complete image of the script
        //
        psz = GlobalAllocPtr(GMEM_MOVEABLE, uFileLen + sizeof(TCHAR));
        if (NULL == psz)
            break;

        //
        //  read the contents of the script window and write it to the
        //  new file
        //
#ifdef UNICODE
        // !!!! Save ASCII file
        GetWindowTextA(hwndScript, (LPSTR)psz, uFileLen);
        WriteFile(hf, psz, uFileLen, &cbBytes, NULL);
#else
        GetWindowText(hwndScript, (LPTSTR)psz, uFileLen);
        cbBytes = (DWORD)_lwrite(hf, (LPSTR)psz, uFileLen);
#endif

        //
        //  succeed
        //
        fReturn = TRUE;
        break;
    }
    AppHourGlass(FALSE);


    //
    //  free memory (if allocated) and close the file. return the result
    //  of our attempt...
    //
    if (psz)
        GlobalFreePtr(psz);

#ifdef UNICODE
    CloseHandle(hf);
#else
    _lclose(hf);
#endif


    //
    //  !!! before returning, we really should try to display an error
    //      message... memory error, etc..
    //
    return (fReturn);
} // MciAppFileSave()


//==========================================================================;
//
//  MCI Device List stuff
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  UINT MciAppGetNumDevices
//
//  Description:
//      This function sends a command to MCI querying it as to how many
//      devices are currently open in the system. This number is the
//      return value.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (UINT):
//      Returns the number of currently open MCI devices in the system.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

UINT FNGLOBAL MciAppGetNumDevices
(
    HWND            hwnd
)
{
    MCI_SYSINFO_PARMS   msip;
    MCIERROR            mciError;
    DWORD               dwDevices;

    //
    //  set things up so that MCI puts the number of open devices directly
    //  into dwDevices
    //
    msip.lpstrReturn = (LPVOID)&dwDevices;
    msip.dwRetSize   = sizeof(dwDevices);

    //
    //  ask MCI how many open devices are in the system--if this errors
    //  then return 0...
    //
    mciError = mciSendCommand(MCI_ALL_DEVICE_ID,
                              MCI_SYSINFO,
                              MCI_SYSINFO_OPEN | MCI_SYSINFO_QUANTITY,
                              (DWORD)(LPVOID)&msip);

    if (MMSYSERR_NOERROR == mciError)
        return ((UINT)dwDevices);

    return (0);
} // MciAppGetNumDevices()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppDeviceListUpdate
//
//  Description:
//      This function updates the MCI Device List window if it is displayed.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      BOOL fForceUpdate: Forces the list to be updated even if number
//      of devices has not changed.
//
//  Return (BOOL):
//      Returns TRUE if the device list needed to be updated. FALSE if
//      the list was fine...
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MciAppDeviceListUpdate
(
    HWND            hwnd,
    HWND            hwndDevices,
    BOOL            fForceUpdate
)
{
    static TCHAR    szFormatDevice[]    = TEXT("%2d. '%s'");
    static UINT     uLastNumDevices;

    TCHAR               ach[APP_MAX_STRING_RC_CHARS];
    TCHAR               szDevName[APP_MAX_STRING_RC_CHARS];
    MCI_SYSINFO_PARMS   msip;
    MCIERROR            mciError;
    HWND                hwndList;
    UINT                uNumDevs;
    UINT                u;

    //
    //  if the devices window is not displayed, then fail..
    //
    if (NULL == hwndDevices)
        return (FALSE);

    uNumDevs = MciAppGetNumDevices(hwnd);

    //
    //  if not being forced to update list, then make a quick check to
    //  see if we should update the list...
    //
    if (!fForceUpdate && (uNumDevs == uLastNumDevices))
        return (FALSE);

    //
    //  really update the device list...
    //
    uLastNumDevices = uNumDevs;

    //
    //  initialize the devices listbox...
    //
    hwndList = GetDlgItem(hwndDevices, IDD_MCIDEVS_LIST_OPEN);
    ListBox_ResetContent(hwndList);

    SetWindowRedraw(hwndList, FALSE);

    //
    //  get the name of each open device in the system and add it to the
    //  device list box...
    //
    for (u = 1; u <= uNumDevs; ++u)
    {
        msip.dwNumber    = u;
        msip.lpstrReturn = (LPVOID)&szDevName;
        msip.dwRetSize   = SIZEOF(szDevName);

        //
        //  get the name--if an error is encountered, then skip to the
        //  next device...
        //
        mciError = mciSendCommand(MCI_ALL_DEVICE_ID,
                                  MCI_SYSINFO,
                                  MCI_SYSINFO_OPEN | MCI_SYSINFO_NAME,
                                  (DWORD)(LPVOID)&msip);

        if (MMSYSERR_NOERROR != mciError)
            continue;

        //
        //  add the device name to the listbox..
        //
        wsprintf(ach, szFormatDevice, u, (LPSTR)szDevName);
        ListBox_AddString(hwndList, ach);
    }

    if (0 != uNumDevs)
        ListBox_SetCurSel(hwndList, 0);

    SetWindowRedraw(hwndList, TRUE);
    InvalidateRect(hwndList, NULL, TRUE);

    //
    //  set the info button to the correct state: enabled if there are
    //  devices open--disabled if no devices are open.
    //
    EnableWindow(GetDlgItem(hwndDevices, IDD_MCIDEVS_BTN_INFO), 0 != uNumDevs);

    return (TRUE);
} // MciAppDeviceListUpdate()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppCloseAllDevices
//
//  Description:
//      This function sends the MCI command "close all" and then updates
//      the displayed info for the application.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (BOOL):
//      Always returns TRUE.
//
//  History:
//       2/ 8/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppCloseAllDevices
(
    HWND            hwnd
)
{
    static TCHAR    szCloseAll[]    = TEXT("close all");

    //
    //  close all open devices and update the device list if it is being
    //  displayed...
    //
    mciSendString(szCloseAll, NULL, 0, NULL);
    MciAppDeviceListUpdate(hwnd, ghwndDevices, FALSE);

    return (TRUE);
} // MciAppCloseAllDevices()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppDeviceDlgProc
//
//  Description:
//      Callback function for the dialog box which displays a list of the
//      currently opened MCI devices.
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
//  Return (BOOL):
//      The return value is specific to the message that was received. For
//      the most part, it is FALSE if this dialog procedure does not handle
//      a message.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNEXPORT MciAppDeviceDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
)
{
    RECT    rcApp;
    RECT    rc;
    HFONT   hfont;
    HWND    hwndParent;
    UINT    u;
    int     n;

    switch (uMsg)
    {
        case WM_INITDIALOG:
            //
            //  move the window so it sits in the upper right corner of the
            //  main window by default...
            //
            GetWindowRect(GetParent(hwnd), &rcApp);
            GetWindowRect(hwnd, &rc);

            n = (int)(rc.right - rc.left);
            rc.left = rcApp.right - n - 30;
            MoveWindow(hwnd, (int)rc.left, (int)rc.top,
                       n, (int)(rc.bottom - rc.top), FALSE);

            //
            //
            //
            hfont = GetStockFont(ANSI_FIXED_FONT);
            SetWindowFont(GetDlgItem(hwnd, IDD_MCIDEVS_LIST_OPEN), hfont, FALSE);

            //
            //  update the information displayed in the listbox
            //
            MciAppDeviceListUpdate(GetParent(hwnd), hwnd, TRUE);

            return (TRUE);

        case WM_COMMAND:
            u = GET_WM_COMMAND_ID(wParam, lParam);
            switch (u)
            {
                case IDCANCEL:
                case IDOK:
                    //
                    //  return button id of the one that was pressed...
                    //
                    hwndParent = GetParent(hwnd);
                    EndDialog(hwnd, u);

                    ghwndDevices = NULL;
                    gfuAppOptions &= ~APP_OPTF_DEVICELIST;
                    SetFocus(GetDlgItem(hwndParent, IDD_APP_EDIT_SCRIPT));
                    break;

                case IDD_MCIDEVS_BTN_INFO:
                    n = ListBox_GetCurSel(GetDlgItem(hwnd, IDD_MCIDEVS_LIST_OPEN));
                    if (LB_ERR == n)
                        break;

                    break;
            }
            break;
    }

    return (FALSE);
} // MciAppDeviceDlgProc()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppDeviceList
//
//  Description:
//      This function either displays or destroys the MCI device list
//      window.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      BOOL fActivate: TRUE if the device list window should keep
//      the activation. FALSE if the current active window should remain
//      active.
//
//  Return (BOOL):
//      The return value is TRUE if the device list is displayed. It is
//      FALSE if the device list has been canceled/closed.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppDeviceList
(
    HWND            hwnd,
    BOOL            fActivate
)
{
    HWND        hwndFocus;

    //
    //  should we display or destroy it?
    //
    if (0 != (gfuAppOptions & APP_OPTF_DEVICELIST))
    {
        if (NULL != ghwndDevices)
            return (TRUE);

        hwndFocus = GetFocus();

        ghwndDevices = CreateDialog(ghinst, DLG_MCIDEVS, hwnd, (DLGPROC)MciAppDeviceDlgProc);
        if (NULL  == ghwndDevices)
        {
            gfuAppOptions &= ~APP_OPTF_DEVICELIST;
            return (FALSE);
        }

        //
        //  note that the window will 'flash' because we used CreateDialog
        //  when creating the device list window. we keep the flash to
        //  a minimum by creating the device window hidden and then showing
        //  it... if you want to get rid of the flash completely, then
        //  use CreateWindow and write a lot more code.
        //
        if (fActivate)
        {
            ShowWindow(ghwndDevices, SW_SHOW);
        }
        else
        {
            SetActiveWindow(hwnd);
            SetFocus(hwndFocus);
            ShowWindow(ghwndDevices, SW_SHOWNA);
        }

        return (TRUE);
    }
    else
    {
        if (NULL != ghwndDevices)
        {
            EndDialog(ghwndDevices, IDOK);
            ghwndDevices = NULL;
        }

        return (FALSE);
    }
} // MciAppDeviceList()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MciAppUpdateOptions
//
//  Description:
//      This function updates the options status window to reflect the
//      current status of the options.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (BOOL):
//      Always returns TRUE.
//
//  History:
//       2/ 8/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppUpdateOptions
(
    HWND            hwnd
)
{
#ifdef UNICODE
    static TCHAR    szFormatOptions[]   = TEXT("%c%c%c%c");
#else
    static TCHAR    szFormatOptions[]   = TEXT("%c%c%c");
#endif

    TCHAR       ach[APP_MAX_STRING_RC_CHARS];

    //
    //  format an appropriate string for the options
    //
    wsprintf(ach, szFormatOptions,
#ifdef UNICODE
             (0 != (gfuAppOptions & APP_OPTF_UNICODE))   ? 'U' : '-',
#endif
             (0 != (gfuAppOptions & APP_OPTF_EDITONLY))  ? 'E' : '-',
             (0 != (gfuAppOptions & APP_OPTF_YIELDEXEC)) ? 'Y' : '-',
             (0 != (gfuAppOptions & APP_OPTF_DEBUGLOG))  ? 'L' : '-');

    SetWindowText(GetDlgItem(hwnd, IDD_APP_TEXT_OPTIONS), ach);

    return (TRUE);
} // MciAppUpdateOptions()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppResetStatus
//
//  Description:
//      This function resets all of the status windows (status, notify,
//      and output). This is used when a new script is created/opened.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (BOOL):
//      Always returns TRUE.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppResetStatus
(
    HWND            hwnd
)
{
    //
    //  nuke all status text...
    //
    SetWindowText(GetDlgItem(hwnd, IDD_APP_TEXT_OUTPUT), gszNone);
    SetWindowText(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS), gszNull);
    SetWindowText(GetDlgItem(hwnd, IDD_APP_TEXT_NOTIFY), gszNull);

    MciAppUpdateOptions(hwnd);

    return (TRUE);
} // MciAppResetStatus()


//--------------------------------------------------------------------------;
//
//  void MciAppDebugLog
//
//  Description:
//      This function logs information to the debugger if the Debug Log
//      option is set. You can then run DBWin (or something similar)
//      to redirect the output whereever you want. Very useful for debugging
//      MCI drivers.
//
//  Arguments:
//      PCTSTR pszFormat: Pointer to any valid format for wsprintf.
//
//  Return (void):
//      None.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

void FNCGLOBAL MciAppDebugLog
(
    PCTSTR          pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];


    //
    //  !!! UNICODE !!!
    //
    //
    if (0 != (gfuAppOptions & APP_OPTF_DEBUGLOG))
    {
        //
        //  format and display the string in a message box...
        //
        va_start(va, pszFormat);
        wvsprintf(ach, pszFormat, (LPSTR)va);
        va_end(va);

        OutputDebugString(gszAppName);
        OutputDebugString(TEXT(": "));
        OutputDebugString(ach);
        OutputDebugString(TEXT("\r\n"));
    }
} // MciAppDebugLog()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppHandleNotify
//
//  Description:
//      This function handles displaying the notification message from
//      commands sent with the 'notify' option.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      UINT fuNotify: Notification flags (wParam) from MM_MCINOTIFY message.
//
//      UINT uId: Device id sending notification.
//
//  Return (BOOL):
//      Always returns TRUE.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppHandleNotify
(
    HWND            hwnd,
    UINT            fuNotify,
    UINT            uId
)
{
    static TCHAR    szFormatNotify[]        = TEXT("Notify(%u,%u): %s");
    static TCHAR    szDBFormatNotify[]      = TEXT("    MCI Notify: Id=%u, Flag(%u)='%s'");

    HWND        hwndNotify;
    TCHAR       ach[APP_MAX_STRING_RC_CHARS];
    UINT        uIds;

    //
    //
    //
    //
    switch (fuNotify)
    {
        case MCI_NOTIFY_SUCCESSFUL:
            uIds = IDS_MCI_NOTIFY_SUCCESSFUL;
            break;

        case MCI_NOTIFY_SUPERSEDED:
            uIds = IDS_MCI_NOTIFY_SUPERSEDED;
            break;

        case MCI_NOTIFY_ABORTED:
            uIds = IDS_MCI_NOTIFY_ABORTED;
            break;

        case MCI_NOTIFY_FAILURE:
            uIds = IDS_MCI_NOTIFY_FAILURE;
            break;

        default:
            uIds = IDS_MCI_NOTIFY_UNKNOWN;
            break;
    }

    LoadString(ghinst, uIds, ach, SIZEOF(ach));
    hwndNotify = GetDlgItem(hwnd, IDD_APP_TEXT_NOTIFY);
    AppSetWindowText(hwndNotify, szFormatNotify, uId, fuNotify, (LPSTR)ach);

    //
    //  !!! UNICODE !!!
    //
    MciAppDebugLog(szDBFormatNotify, uId, fuNotify, (LPSTR)ach);

    return (TRUE);
} // MciAppHandleNotify()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppErrorDlgProc
//
//  Description:
//      Callback function for the dialog box which occurs during the
//      execution of an error in a loop of script commands. It displays
//      Abort, Continue and Ignore buttons.
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
//  Return (BOOL):
//      The return value is specific to the message that was received. For
//      the most part, it is FALSE if this dialog procedure does not handle
//      a message.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNEXPORT MciAppErrorDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
)
{
    UINT    u;

    switch (uMsg)
    {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:
            u = GET_WM_COMMAND_ID(wParam, lParam);
            switch (u)
            {
                case IDABORT:
                case IDOK:
                case IDIGNORE:
                    //
                    //  return button id of the one that was pressed...
                    //
                    EndDialog(hwnd, u);
                    break;
            }
            break;
    }

    return (FALSE);
} // MciAppErrorDlgProc()


//--------------------------------------------------------------------------;
//
//  MCIERROR MciAppSendString
//
//  Description:
//      Sends the specified string command to an MCI device via the MCI
//      string interface. Any return strings from MCI devices are displayed
//      in the 'Output' text box. Error values are displayed in the status
//      bar.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PCTSTR pszCommand: Pointer to the string command to be executed.
//
//  Return (MCIERROR):
//      The return value is the result of the mciSendString() function.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

MCIERROR FNGLOBAL MciAppSendString
(
    HWND            hwnd,
    PCTSTR          pszCommand
)
{
    static TCHAR    szFormatDBSendString[]  = TEXT("mciSendString('%s')");
    static TCHAR    szFormatOutput[]        = TEXT("'%s'");
    static TCHAR    szFormatDBOutput[]      = TEXT("    MCI Output: '%s'");
    static TCHAR    szFormatError[]         = TEXT("MCI Error: [%lu], %s");
    static TCHAR    szFormatDBError[]       = TEXT("    MCI Error : [%lu], '%s'");

    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    HWND        hwndOutput;
    HWND        hwndStatus;
    MCIERROR    mciError;

    //
    //  reset the notify window text..
    //
    SetWindowText(GetDlgItem(hwnd, IDD_APP_TEXT_NOTIFY), gszNull);


    //
    //  send the string command to MCI--capture the return value and have
    //  the notification code go to the main window (MM_MCINOTIFY)...
    //
    //  !!! UNICODE !!!
    //
    MciAppDebugLog(szFormatDBSendString, (LPSTR)pszCommand);
    mciError = mciSendString(pszCommand, ach, SIZEOF(ach), hwnd);

    //
    //  if the command errored, then beep the _speaker_ (-1). don't want
    //  to use wave devices because many mci scripts use them..
    //
    if (MMSYSERR_NOERROR != mciError)
        MessageBeep((UINT)-1);

    MciAppDebugLog(szFormatDBOutput, (LPSTR)ach);

    //
    //  put the text message returned by MCI into the 'Output' box
    //
    hwndOutput = GetDlgItem(hwnd, IDD_APP_TEXT_OUTPUT);
    if ('\0' == ach[0])
        SetWindowText(hwndOutput, gszNone);
    else
        AppSetWindowText(hwndOutput, szFormatOutput, (LPSTR)ach);


    //
    //  decode the error number returned by MCI and display the string in
    //  the status bar...
    //
    //  !!! UNICODE !!!
    //
    mciGetErrorString(mciError, ach, SIZEOF(ach));
    MciAppDebugLog(szFormatDBError, mciError, (LPSTR)ach);

    hwndStatus = GetDlgItem(hwnd, IDD_APP_TEXT_STATUS);
    AppSetWindowText(hwndStatus, szFormatError, mciError, (LPSTR)ach);


    //
    //  update our list of currently open devices and stuff...
    //
    MciAppDeviceListUpdate(hwnd, ghwndDevices, FALSE);

    return (mciError);
} // MciAppSendString()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppGetLine
//
//  Description:
//      Retrieves the contents of line (uLine) from the script window.
//      The string is placed in the pszLineBuffer. All leading and trailing
//      spaces (and comments) are removed.
//
//  Arguments:
//      HWND hwndScript: Handle to script window (multiline edit control)
//      to extract nLine from.
//
//      int nLine: Line index (zero based) to retrieve.
//
//      PTSTR pszBuffer: Pointer to string buffer to receive nLine's
//      contents.
//
//      UINT cchBuffer: Size of pszBuffer is _characters_.
//
//  Return (BOOL):
//      The return value is TRUE if nLine was extracted from the script
//      window. The return value is FALSE if nLine is out of range for
//      the current contents of the script window.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MciAppGetLine
(
    HWND            hwndScript,
    int             nLine,
    PTSTR           pszBuffer,
    UINT            cchBuffer
)
{
    #define IS_SPACE(c)     (' '  == (c) || '\t' == (c))
    #define IS_EOL(c)       ('\n' == (c) || '\r' == (c))
    #define IS_WHITE(c)     (IS_SPACE(c) || IS_EOL(c))

    int     nNumLines;

    //
    //  find out how many lines are in the script window--if it is out of
    //  range then fail....
    //
    nNumLines = Edit_GetLineCount(hwndScript);
    if ((nLine < 0) || (nLine >= nNumLines))
    {
        pszBuffer[0] = '\0';
        return (FALSE);
    }

    //
    //  read the line requested into the string buffer..
    //
    cchBuffer = Edit_GetLine(hwndScript, nLine, pszBuffer, cchBuffer);


    //
    //  strip trailing spaces
    //
    while ((cchBuffer > 0) && IS_WHITE(pszBuffer[cchBuffer - 1]))
    {
        cchBuffer--;
    }

    pszBuffer[cchBuffer] = '\0';

    return (TRUE);
} // MciAppGetLine()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppInternalCommand
//
//  Description:
//
//
//  Arguments:
//
//
//  Return (BOOL):
//
//
//  History:
//       2/20/93    created. 
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MciAppInternalCommand
(
    HWND            hwnd,
    PTSTR           pszCommandLine,
    BOOL            fYield
)
{
    static TCHAR    szWhiteToken[]      = " \t\n";
    static TCHAR    szTokenSleep[]      = "sleep";
    static TCHAR    szTokenPause[]      = "pause";

    static TCHAR    szFormatError[]     = TEXT("INTERNAL Error: Command (%s), %s.");
    static TCHAR    szFormatDBError[]   = TEXT("    ICmd Error: Command (%s), %s.");

    static TCHAR    szSleepComplete[]   = TEXT("Sleep complete.");
    static TCHAR    szSleepDBComplete[] = TEXT("      Sleeping:  sleep complete.");

    static TCHAR    szErrorBadCmd[]     = TEXT("unrecognized command");
    static TCHAR    szErrorBadParam[]   = TEXT("invalid parameter");
    static TCHAR    szErrorParamRqd[]   = TEXT("parameter(s) required");

    static TCHAR    szFormatSleeping[]  = TEXT("Sleeping for %lu %s...");
    static TCHAR    szSeconds[]         = TEXT("seconds");
    static TCHAR    szMilliseconds[]    = TEXT("milliseconds");
    static TCHAR    szMinutes[]         = TEXT("minutes");
    static TCHAR    szHours[]           = TEXT("hours");

    PTSTR       pchToken;
    PTSTR       pchCommand;
    HWND        hwndStatus;
    long        l;
    DWORD       dwSleep;
    PTSTR       szSleepUnit;

    //
    //  parse the internal command line...
    //
    pchToken = strtok(pszCommandLine, szWhiteToken);
    if (NULL == pchToken)
        return (TRUE);

    hwndStatus = GetDlgItem(hwnd, IDD_APP_TEXT_STATUS);
    pchCommand = pchToken;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  sleep x [ms | s | m | h]        (default is milliseconds)
//  pause x [ms | s | m | h]        (default is milliseconds)
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

    if ((0 == lstrcmpi(pchCommand, szTokenSleep)) ||
        (0 == lstrcmpi(pchCommand, szTokenPause)))
    {
        pchToken = strtok(NULL, szWhiteToken);
        if (NULL == pchToken)
        {
            MessageBeep((UINT)-1);
            AppSetWindowText(hwndStatus, szFormatError,
                             (LPSTR)pchCommand, (LPSTR)szErrorParamRqd);
            MciAppDebugLog(szFormatDBError, (LPSTR)pchCommand,
                           (LPSTR)szErrorParamRqd);
            return (TRUE);
        }

        //
        //  assume the first token after sleep/pause command is a
        //  number--do not allow zero or negative numbers!
        //
        l = atol(pchToken);
        if (0 >= l)
        {
            MessageBeep((UINT)-1);
            AppSetWindowText(hwndStatus, szFormatError,
                             (LPSTR)pchCommand, (LPSTR)szErrorBadParam);
            MciAppDebugLog(szFormatDBError, (LPSTR)pchCommand,
                           (LPSTR)szErrorBadParam);
            return (TRUE);
        }

        //
        //  now see what unit
        //
        dwSleep     = (DWORD)l;
        szSleepUnit = szMilliseconds;
        pchToken    = strtok(NULL, szWhiteToken);
        if (NULL != pchToken)
        {
            switch (pchToken[0])
            {
                case 's':
                case 'S':
                    dwSleep *= 1000;
                    szSleepUnit = szSeconds;
                    break;

                case 'm':
                case 'M':
                    //
                    //  if unit marker is not 'ms' then assume minutes
                    //
                    if ('s' != pchToken[1])
                    {
                        dwSleep *= 1000L * 60;
                        szSleepUnit = szMinutes;
                    }
                    break;

                case 'h':
                case 'H':
                    dwSleep *= 1000L * 60 * 60;
                    szSleepUnit = szHours;
                    break;
            }
        }

        //
        //
        //
        AppSetWindowText(hwndStatus, szFormatSleeping, l, (LPSTR)szSleepUnit);
        MciAppDebugLog(szFormatSleeping, l, (LPSTR)szSleepUnit);

        if (!fYield)
            UpdateWindow(hwndStatus);

        //
        //  !!! broken !!!
        //
        //
#ifdef WIN32
        Sleep(dwSleep);
#else
{
        DWORD   dwStart;

        if (!fYield)
            AppHourGlass(TRUE);

        dwStart = timeGetTime();
        while ((timeGetTime() - dwStart) < dwSleep)
        {
            if (fYield)
            {
                AppYield(hwnd, FALSE);
                if (gfAbortExec)
                    break;
            }
            else
            {
                if (GetAsyncKeyState(VK_ESCAPE) < 0)
                    break;
            }
        }

        if (!fYield)
            AppHourGlass(FALSE);
}
#endif
        
        AppSetWindowText(hwndStatus, szSleepComplete);
        MciAppDebugLog(szSleepDBComplete);

        if (!fYield)
            UpdateWindow(hwndStatus);

        return (TRUE);
    }


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  unrecognized command...
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

    MessageBeep((UINT)-1);
    AppSetWindowText(hwndStatus, szFormatError,
                     (LPSTR)pchCommand, (LPSTR)szErrorBadCmd);
    MciAppDebugLog(szFormatDBError, (LPSTR)pchCommand, (LPSTR)szErrorBadCmd);

    return (TRUE);
} // MciAppInternalCommand()


//--------------------------------------------------------------------------;
//
//  MCIERROR MciAppSingleStep
//
//  Description:
//      Executes one line from the script window.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (MCIERROR):
//      The return value is the MCIERROR return value from mciSendString.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

MCIERROR FNGLOBAL MciAppSingleStep
(
    HWND            hwnd
)
{
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    HWND        hwndScript;
    int         nLine;
    int         nLineStart;
    int         nSelStart;
    int         nSelEnd;
    MCIERROR    mciError;
    BOOL        fDidOne;

    //
    //  initialize a bunch of stuff...
    //
    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    mciError   = MMSYSERR_NOERROR;
    fDidOne    = FALSE;
    nLineStart = Edit_LineFromChar(hwndScript, -1);
    nLine      = nLineStart;

    //
    //  step through the script starting with the current line until we
    //  execute one non-comment/blank line...
    //
    for ( ; MciAppGetLine(hwndScript, nLine, ach, SIZEOF(ach)); nLine++)
    {
        //
        //  select the current line that is going to be executed....
        //
        nSelStart = Edit_LineIndex(hwndScript, nLine);
        nSelEnd   = Edit_LineIndex(hwndScript, nLine + 1);
        if (nSelEnd < nSelStart)
            nSelEnd = 0x7FFF;
        else if (nSelEnd > nSelStart)
            nSelEnd -= 1;

        Edit_SetSel(hwndScript, nSelStart, nSelEnd);

        //
        //  anything worth doing something with?
        //
        if (('\0' == ach[0]) || (';' == ach[0]))
            continue;

        //
        //  if first character is '!' then process rest of line as an
        //  internale MCI App command...
        //
        if ('!' == ach[0])
        {
            MciAppInternalCommand(hwnd, &ach[1], FALSE);
            continue;
        }

        //
        //  if we have actually executed a line, then break out... we do
        //  this so the above code will have selected the next line for
        //  single stepping.
        //
        if (fDidOne)
            break;


        //
        //  send the command on the current line to MCI via the string
        //  interface.
        //
        mciError = MciAppSendString(hwnd, ach);
        fDidOne  = TRUE;
    }

    SetFocus(hwndScript);
    return (mciError);
} // MciAppSingleStep()


//--------------------------------------------------------------------------;
//
//  MCIERROR MciAppExecute
//
//  Description:
//      Executes the MCI command which is currently selected in the script
//      window. If fSingleStep is TRUE, then only this one line will be
//      executed. Otherwise, every line from the currently selected line to
//      the last line in the script window will be executed sequentially.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      BOOL fYield: TRUE if execution should yield between MCI commands.
//
//  Return (MCIERROR):
//      The return value is the last MCIERROR return value from executing.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

MCIERROR FNGLOBAL MciAppExecute
(
    HWND            hwnd,
    BOOL            fYield
)
{
    static TCHAR    szFormatRuncount[]     = TEXT("%u");

    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    HWND        hwndScript;
    HWND        hwndRuncount;
    UINT        uRunCount;
    UINT        u;
    UINT        uLinesExecuted;
    int         n;
    int         nLine;
    int         nLineStart;
    int         nSelStart;
    int         nSelEnd;
    MCIERROR    mciError;
    BOOL        fIgnoreErrors;


    hwndScript   = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    hwndRuncount = GetDlgItem(hwnd, IDD_APP_EDIT_RUNCOUNT);

    uRunCount  = GetDlgItemInt(hwnd, IDD_APP_EDIT_RUNCOUNT, NULL, TRUE);

    uLinesExecuted = 0;
    mciError       = MMSYSERR_NOERROR;
    fIgnoreErrors  = FALSE;

    gfExecuting    = TRUE;
    gfAbortExec    = FALSE;


    //
    //  if we are yielding, then disable a bunch of controls...
    //
    //  if we are not yielding during then bring up an hour glass to show
    //  we be busy... don't bother with disabling controls--they can't
    //  be used
    //
    if (fYield)
    {
        static TCHAR    szButtonStop[]  = TEXT("&Stop");

        SetWindowText(GetDlgItem(hwnd, IDD_APP_BTN_STEP), szButtonStop);

        EnableWindow(GetDlgItem(hwnd, IDD_APP_BTN_GO),        FALSE);
        EnableWindow(GetDlgItem(hwnd, IDD_APP_BTN_RUN),       FALSE);
        EnableWindow(GetDlgItem(hwnd, IDD_APP_EDIT_RUNCOUNT), FALSE);
    }
    else
    {
        AppHourGlass(TRUE);
    }


    //
    //  go through every line in the script window from the currently
    //  selected line to the last line...
    //
    nLineStart = Edit_LineFromChar(hwndScript, -1);
    for (u = uRunCount; u; u--)
    {
        nLine = nLineStart;
        for ( ; MciAppGetLine(hwndScript, nLine, ach, SIZEOF(ach)); nLine++)
        {
            //
            //  select the current line that is going to be executed....
            //
            nSelStart = Edit_LineIndex(hwndScript, nLine);
            nSelEnd   = Edit_LineIndex(hwndScript, nLine + 1);
            if (nSelEnd < nSelStart)
                nSelEnd = 0x7FFF;
            else if (nSelEnd > nSelStart)
                nSelEnd -= 1;

            Edit_SetSel(hwndScript, nSelStart, nSelEnd);


            //
            //  check for abort...
            //
            if (gfAbortExec)
            {
                n = AppMsgBoxId(hwnd, MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL,
                                IDS_EXEC_ABORT);
                if (IDYES == n)
                    goto MciApp_Execute_Exit;

                gfAbortExec = FALSE;
            }


            //
            //  anything worth doing something with?
            //
            if (('\0' == ach[0]) || (';' == ach[0]))
                continue;

            //
            //  if first character is '!' then process rest of line as an
            //  internale MCI App command...
            //
            if ('!' == ach[0])
            {
                MciAppInternalCommand(hwnd, &ach[1], fYield);
                continue;
            }

            //
            //  send the command on the current line to MCI via the string
            //  interface.
            //
            mciError = MciAppSendString(hwnd, ach);
            if ((MMSYSERR_NOERROR != mciError) && !fIgnoreErrors)
            {
                DialogBox(ghinst, DLG_MCIERR, hwnd, (DLGPROC)MciAppErrorDlgProc);
                switch (n)
                {
                    case IDABORT:
                        goto MciApp_Execute_Exit;

                    case IDIGNORE:
                        fIgnoreErrors = TRUE;
                        break;
                }
            }

            uLinesExecuted++;

            //
            //  yield like a good little app..
            //
            if (fYield)
                AppYield(hwnd, FALSE);
            else
            {
                UpdateWindow(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS));
                UpdateWindow(GetDlgItem(hwnd, IDD_APP_TEXT_OUTPUT));
            }
        }

        //
        //  SetDlgItemInt() won't update the field unless we yield
        //  so set window text instead... in case not yielding.
        //
        SetDlgItemInt(hwnd, IDD_APP_EDIT_RUNCOUNT, u, TRUE);
        if (fYield)
            AppYield(hwnd, FALSE);
        else
            UpdateWindow(GetDlgItem(hwnd, IDD_APP_EDIT_RUNCOUNT));
    }


MciApp_Execute_Exit:

    gfExecuting = FALSE;
    if (fYield)
    {
        static TCHAR    szButtonStep[]  = TEXT("&Step");

        SetWindowText(GetDlgItem(hwnd, IDD_APP_BTN_STEP), szButtonStep);

        EnableWindow(GetDlgItem(hwnd, IDD_APP_BTN_GO),        TRUE);
        EnableWindow(GetDlgItem(hwnd, IDD_APP_BTN_RUN),       TRUE);
        EnableWindow(GetDlgItem(hwnd, IDD_APP_EDIT_RUNCOUNT), TRUE);
    }
    else
    {
        AppHourGlass(FALSE);
    }

    SetDlgItemInt(hwnd, IDD_APP_EDIT_RUNCOUNT, uRunCount, TRUE);
    SetFocus(hwndScript);

    return (mciError);
} // MciAppExecute()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppEnterLine
//
//  Description:
//      This function handles entering a new line into the script window.
//      This may involve executing the new line if Options.Edit Only is
//      not selected.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      BOOL fEditOnly: TRUE if in data entry mode (won't single step the
//      current line before inserting CR/LF). FALSE if the current line
//      should be executed and a CR/LF inserted at the _END_ of the
//      current line.
//
//  Return (BOOL):
//      The return value is always TRUE.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppEnterLine
(
    HWND            hwnd,
    BOOL            fEditOnly
)
{
    static TCHAR    szEOL[]     = TEXT("\r\n");

    HWND    hwndScript;
    int     n;

    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);

    //
    //  if Options.Edit Only is checked or the user pressed Alt+Enter,
    //  then act like a simple edit control...
    //
    if (fEditOnly)
    {
        Edit_ReplaceSel(hwndScript, szEOL);
        return (TRUE);
    }

    //
    //  get current line (containing caret)--if there is a
    //  selection, the line number of the line containing the
    //  _beginning_ of the selection is retrieved.
    //
    SetFocus(hwndScript);
    n = Edit_LineFromChar(hwndScript, -1);
    MciAppSingleStep(hwnd);
    n = Edit_LineIndex(hwndScript, n);
    Edit_SetSel(hwndScript, n, n);

    //
    //  now force CR/LF at the _end_ of the current line
    //
    FORWARD_WM_KEYDOWN(hwndScript, VK_END, 1, 0, SendMessage);
    FORWARD_WM_KEYUP(hwndScript, VK_END, 1, 0, SendMessage);
    Edit_ReplaceSel(hwndScript, szEOL);

    return (TRUE);
} // MciAppEnterLine()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MciAppDispatchMessage
//
//  Description:
//
//
//  Arguments:
//
//
//  Return (BOOL):
//
//
//  History:
//       2/ 8/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppDispatchMessage
(
    HWND            hwnd,
    PMSG            pmsg
)
{
    static HWND     hwndScript;
    static HACCEL   haccl;

    UINT        u;

    //
    //  if this is the first time through, cache some stuff...
    //
    if (NULL == hwndScript)
        hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);

    if (NULL == haccl)
        haccl = LoadAccelerators(ghinst, ACCEL_APP);


    //
    //  peek at the message being sent. if it is one of the special
    //  things for the script window, then deal with it...
    //
    //  !!! this is somewhat bogus !!!
    //
    if ((WM_KEYDOWN == pmsg->message) || (WM_SYSKEYDOWN == pmsg->message))
    {
        if (GetActiveWindow() != hwnd)
            goto MciApp_Dispatch_Continue;

        switch (pmsg->wParam)
        {
            case VK_RETURN:
                if (GetFocus() != hwndScript)
                    break;

                //
                //  force a line to be 'executed' and stuff... if Alt+Enter
                //  was used, then reverse the logic of 'Edit Only' by
                //  setting uCode to 0 (from menu)
                //
                u = (GetKeyState(VK_MENU) < 0) ? 0 : 1;
                FORWARD_WM_COMMAND(hwnd, IDOK, hwndScript, u, PostMessage);
                return (TRUE);

            case VK_TAB:
                if (WM_SYSKEYDOWN == pmsg->message)
                    break;

                if (GetKeyState(VK_CONTROL) < 0)
                {
                    if (GetFocus() == hwndScript)
                    {
                        SetFocus(GetDlgItem(hwnd, IDD_APP_BTN_STEP));
                        return (TRUE);
                    }

                    SetFocus(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT));
                    return (TRUE);
                }
                else if (GetFocus() == hwndScript)
                {
                    static TCHAR    szTab[]     = TEXT("\t");
                    Edit_ReplaceSel(hwndScript, szTab);
                    return (TRUE);
                }
                break;
        }
    }


MciApp_Dispatch_Continue:

    //
    //  update the device list if it needs to be updated--also
    //  do the floating dialog box message thing...
    //
    if (NULL != ghwndDevices)
    {
        MciAppDeviceListUpdate(hwnd, ghwndDevices, FALSE);

        if (IsDialogMessage(ghwndDevices, pmsg))
            return (TRUE);
    }


    //
    //  take care of accelerators and dialog box style things...
    //
    if (TranslateAccelerator(hwnd, haccl, pmsg) ||
        IsDialogMessage(hwnd, pmsg))
    {
        return (TRUE);
    }

    TranslateMessage(pmsg);
    DispatchMessage(pmsg);

    return (TRUE);
} // MciAppDispatchMessage()


//==========================================================================;
//
//  Startup and shutdown code...
//
//
//==========================================================================;

TCHAR   gszKeyOptions[]     = TEXT("Options");
TCHAR   gszFormatOptions[]  = TEXT("%u");

TCHAR   gszKeyWindow[]      = TEXT("Window");
TCHAR   gszKeyFont[]        = TEXT("Font");


//--------------------------------------------------------------------------;
//
//  BOOL MciAppChooseFont
//
//  Description:
//      This function lets the user choose a new font for the script window.
//      After a new font is chosen, the font structure is stored to the
//      .ini file so it can be restored on the next run of this application.
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//  Return (BOOL):
//      The return value is TRUE if a new font was chosen. It is FALSE if
//      the user canceled the operation.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppChooseFont
(
    HWND            hwnd
)
{
    LOGFONT     lf;
    HWND        hwndScript;
    HFONT       hfont;
    HFONT       hfontNew;

    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);

    //
    //  get the current script font and pass it to the choose font dialog
    //
    hfont = GetWindowFont(hwndScript);

    hfontNew = AppChooseFont(hwnd, hfont, &lf);
    if (NULL == hfontNew)
        return (FALSE);

    //
    //  select the new font into the script window and delete the old one
    //
    SetWindowFont(hwndScript, hfontNew, TRUE);
    DeleteFont(hfont);


    //
    //  save the complete description of the chosen font so there can be
    //  no strangness in the font mapping next run. this is overkill, but
    //  it works...
    //
    AppProfileWriteBytes(gszKeyFont, (LPBYTE)&lf, sizeof(lf));

    return (TRUE);
} // MciAppChooseFont()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppSettingsRestore
//
//  Description:
//      This function restores state information for the application. This
//      function is called just after the main window is created (it has
//      not been ShowWindow()'d). This function will generate the call
//      to ShowWindow before returning.
//
//  Arguments:
//      HWND hwnd: Handle to main window that has just been created but
//      not shown.
//
//      int nCmdShow: The state that the application window should show as.
//
//  Return (BOOL):
//      The return value is always TRUE.
//
//  History:
//       2/15/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MciAppSettingsRestore
(
    HWND            hwnd,
    int             nCmdShow
)
{
    static TCHAR    szSecExtensions[]   = TEXT("Extensions");
    static TCHAR    szKeyMCI[]          = TEXT("mci");
    static TCHAR    szValAssocMCI[]     = TEXT("mciapp.exe ^.mci");
    static TCHAR    szKeyMCS[]          = TEXT("mcs");
    static TCHAR    szValAssocMCS[]     = TEXT("mciapp.exe ^.mcs");

    TCHAR           ach[APP_MAX_STRING_RC_CHARS];
    WINDOWPLACEMENT wp;
    HFONT           hfont;
    LOGFONT         lf;
    UINT            u;
    RECT            rc;
    RECT            rcWindow;
    POINT           pt;
    int             n;
    BOOL            f;


    //
    //  restore the previous Options state...
    //
    gfuAppOptions = GetProfileInt(gszAppSection, gszKeyOptions, gfuAppOptions);


    //
    //  we want to make sure that the association for .MCI and .MCS files is
    //  to run MCIAPP.EXE. this way any app that ShellExecute's a .MCI or
    //  .MCS file will run this app...
    //
    //  in WIN.INI:
    //
    //      [Extensions]
    //      mci = mciapp.exe ^.mci
    //      mcs = mciapp.exe ^.mcs
    //
    u = (UINT)GetProfileString(szSecExtensions, szKeyMCI, gszNull, ach, SIZEOF(ach));
    if ((0 == u) || lstrcmpi(ach, szValAssocMCI))
    {
        WriteProfileString(szSecExtensions, szKeyMCI, szValAssocMCI);
    }

    u = (UINT)GetProfileString(szSecExtensions, szKeyMCS, gszNull, ach, SIZEOF(ach));
    if ((0 == u) || lstrcmpi(ach, szValAssocMCS))
    {
        WriteProfileString(szSecExtensions, szKeyMCS, szValAssocMCS);
    }

    //
    //  restore the user's preferred font.
    //
    f = AppProfileReadBytes(gszKeyFont, (LPBYTE)&lf, sizeof(lf));
    if (f)
    {
        hfont = CreateFontIndirect(&lf);
        if (NULL != hfont)
        {
            SetWindowFont(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT), hfont, FALSE);
        }
    }


    //
    //  grab the stored window position and size from the .ini file...
    //  there must be four arguments stored or the entry is considered
    //  invalid.
    //
    f = AppProfileReadBytes(gszKeyWindow, (LPBYTE)&rcWindow, sizeof(rcWindow));
    if (f)
    {
        //
        //  to make sure the user can always get at the window, check to
        //  see if the midpoint of the caption is visible--if it is not,
        //  then default to the default position used when creating the
        //  window.
        //
        n = (rcWindow.right - rcWindow.left) / 2;
        pt.x = (n + rcWindow.left);

        n = GetSystemMetrics(SM_CYCAPTION) / 2 + GetSystemMetrics(SM_CXFRAME);
        pt.y = (n + rcWindow.top);

        GetWindowRect(GetDesktopWindow(), &rc);
        if (PtInRect(&rc, pt))
        {
            //
            //  fill out the window placement structure--default the
            //  maximized and minimized states to default placement by
            //  getting its current placement.
            //
            wp.length = sizeof(wp);
            GetWindowPlacement(hwnd, &wp);

            wp.showCmd          = nCmdShow;
            wp.rcNormalPosition = rcWindow;

            SetWindowPlacement(hwnd, &wp);
            return (TRUE);
        }
    }

    //
    //  show defaulted and succeed
    //
    ShowWindow(hwnd, nCmdShow);
    return (TRUE);
} // MciAppSettingsRestore()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppSettingsSave
//
//  Description:
//      This function saves the current state information for the application.
//      It is called just before the main window is closed (destroyed); or
//      as Windows is exiting (query end session).
//
//      Note that this function should not destroy any resources--it can
//      be called at any time to save a snapshot of the application state.
//
//  Arguments:
//      HWND hwnd: Handle to main window that will be destroyed shortly.
//
//  Return (BOOL):
//      The return value is always TRUE.
//
//  History:
//       2/15/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppSettingsSave
(
    HWND            hwnd
)
{
    TCHAR           ach[APP_MAX_STRING_RC_CHARS];
    WINDOWPLACEMENT wp;
    PRECT           prc;
    BOOL            f;

    //
    //  save the current option settings--note that we ALWAYS turn off the
    //  debug logging option so the app doesn't try to OutputDebugString
    //  unexpectedly during the next session...
    //
    gfuAppOptions &= ~APP_OPTF_DEBUGLOG;
    if (GetProfileInt(gszAppSection, gszKeyOptions, 0) != gfuAppOptions)
    {
        wsprintf(ach, gszFormatOptions, gfuAppOptions);
        WriteProfileString(gszAppSection, gszKeyOptions, ach);
    }

    //
    //  save the current window placement--only store the size and location
    //  of the restored window. maximized and minimized states should
    //  remain defaulted on the next invocation of this application.
    //
    wp.length = sizeof(wp);
    f = GetWindowPlacement(hwnd, &wp);
    if (f)
    {
        prc = &wp.rcNormalPosition;

        DPF(0, "WindowPlacement: show=%d, minX=%d, minY=%d, maxX=%d, maxY=%d",
             wp.showCmd, wp.ptMinPosition.x, wp.ptMinPosition.y,
             wp.ptMaxPosition.x, wp.ptMaxPosition.y);

        DPF(0, "                 normX=%d, normY=%d, normW=%d, normH=%d",
             prc->left, prc->top, prc->right, prc->bottom);

        //
        //  save the _bounding rectangle_ of the restored window state...
        //
        AppProfileWriteBytes(gszKeyWindow, (LPBYTE)prc, sizeof(*prc));
    }


    //
    //  succeed
    //
    return (TRUE);
} // MciAppSettingsSave()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppShutdown
//
//  Description:
//      This function is called to gracefully shut down the application.
//      If the application should not be closed, a FALSE value is returned.
//      This function is called for WM_CLOSE and WM_QUERYENDSESSION
//      messages...
//
//  Arguments:
//      HWND hwnd: Handle to main window.
//
//      PTSTR pszFilePath: Pointer to current file path of script. If the
//      file needs to be saved, then this buffer will receive the new
//      file path if one is chosen.
//
//      PTSTR pszFilePath: Pointer to current file title of script. If the
//      file needs to be saved, then this buffer will receive the new
//      file title if one is chosen.
//
//  Return (BOOL):
//      Returns TRUE if the application can proceed with close. Returns
//      FALSE if the application should NOT be closed.
//
//  History:
//       2/ 9/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppShutdown
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
    int         n;
    UINT        u;
    BOOL        f;

    //
    //  if we are currently executing a script (Run or Go), things would
    //  get very messy if we tried to close the application. so disallow
    //  it!
    //
    //  !!! this is rude--bring up dialog with the problem statement !!!
    //
    if (gfExecuting)
    {
        MessageBeep((UINT)-1);
        return (FALSE);
    }

    //
    //  check if the script has been modified without saving. if the user
    //  cancels the operation, then we will NOT close the application.
    //
    f = MciAppFileSaveModified(hwnd, pszFilePath, pszFileTitle);
    if (!f)
        return (FALSE);

    //
    //  if devices are still open, ask user if we should close them down
    //  before exiting. the only reason we don't do this by default is
    //  for application cleanup testing in mmsystem... i suppose it is
    //  a nifty reminder for scripts that don't close devices also.
    //
    u = MciAppGetNumDevices(hwnd);
    if (0 != u)
    {
        n = AppMsgBoxId(hwnd, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_TASKMODAL,
                        IDS_WARN_OPEN_DEVICES, u);
        switch (n)
        {
            case IDYES:
                MciAppCloseAllDevices(hwnd);
                break;

            case IDNO:
                break;

            case IDCANCEL:
                return (FALSE);
        }
    }


    //
    //  save any settings that should be saved on app termination...
    //
    MciAppSettingsSave(hwnd);

    //
    //  allow closing of application...
    //
    return (TRUE);
} // MciAppShutdown()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppInit
//
//  Description:
//
//
//  Arguments:
//
//
//  Return (BOOL):
//
//
//  History:
//       2/15/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppInit
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    LPTSTR          pszCmdLine,
    int             nCmdShow
)
{
    BOOL        f;

    //
    //
    //
    MciAppSettingsRestore(hwnd, nCmdShow);

    //
    //  strip the command line..
    //
    if (NULL != pszCmdLine)
    {
        while (('\0' != *pszCmdLine) && (' ' == *pszCmdLine))
            pszCmdLine++;
    }

    //
    //  if there is a command line, assume it is a filename for a script
    //  and try to open it. otherwise, just initialize the script window.
    //
    if ((NULL != pszCmdLine) && ('\0' != *pszCmdLine))
    {
        //
        //  attempt to open the specified file..
        //
        lstrcpy(pszFilePath, pszCmdLine);
        f = MciAppFileOpen(hwnd, pszFilePath, pszFileTitle);
        if (f)
        {
            AppTitle(hwnd, pszFileTitle);
            SetFocus(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT));
        }
        else
        {
            //
            //  opening the command line file was untriumphant..
            //
            AppMsgBoxId(hwnd, MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL,
                        IDS_ERROR_OPEN_FAILED, (LPSTR)pszFilePath);


            pszFilePath[0]  = '\0';
            pszFileTitle[0] = '\0';
            AppFileNew(hwnd, pszFilePath, pszFileTitle);
        }
    }
    else
    {
        AppFileNew(hwnd, pszFilePath, pszFileTitle);
    }


    //
    //
    //
    //
    MciAppDeviceList(hwnd, FALSE);

    return (TRUE);
} // MciAppInit()


//--------------------------------------------------------------------------;
//
//  BOOL MciAppExit
//
//  Description:
//
//
//  Arguments:
//
//
//  Return (BOOL):
//
//
//  History:
//       2/15/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MciAppExit
(
    void
)
{
    //
    //  clean up any resources and stuff we have allocated...
    //

    return (TRUE);
} // MciAppExit()
