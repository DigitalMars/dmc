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
//  mainit.c
//
//  Description:
//
//
//  History:
//       9/21/93
//
//==========================================================================;

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commdlg.h>

#include "mixapp.h"

#include "debug.h"



//
//
//
TCHAR       gszAppProfile[]     = TEXT("mixapp.ini");

TCHAR       gszSecConfig[]      = TEXT("Configuration");

TCHAR       gszKeyOptions[]     = TEXT("Options");
TCHAR       gszFormatOptions[]  = TEXT("%u");

TCHAR       gszKeyWindow[]      = TEXT("Window");
TCHAR       gszKeyFont[]        = TEXT("Font");


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL AppProfileWriteBytes
//
//  Description:
//      This function writes a raw structure of bytes to the application's
//      ini section that can later be retrieved using AppProfileReadBytes.
//      This gives an application the ability to write any structure to
//      the ini file and restore it later--very useful.
//
//      NOTE! Starting with Windows for Workgroups 3.1 there are two new
//      profile functions that provide the same functionality of this
//      function. Specifically, these functions are GetPrivateProfileStruct
//      and WritePrivateProfileStruct. These new functions are provided
//      by the Common Controls DLL. The prototypes are as follows:
//
//      BOOL GetPrivateProfileStruct
//      (
//          LPSTR       szSection,
//          LPSTR       szKey,
//          LPBYTE      lpStruct,
//          UINT        uSizeStruct,
//          LPSTR       szFile
//      );
//
//      BOOL WritePrivateProfileStruct
//      (
//          LPSTR       szSection,
//          LPSTR       szKey,
//          LPBYTE      lpStruct,
//          UINT        uSizeStruct,
//          LPSTR       szFile
//      );
//
//      If you are building an application that is for Window for Workgroups
//      or newer versions of Windows, you will probably want to use the
//      above functions.
//
//  Arguments:
//      PTSTR pszSection: Pointer to section for the stored data.
//
//      PTSTR pszKey: Pointer to key name for the stored data.
//
//      LPBYTE pbStruct: Pointer to the data to be saved.
//
//      UINT cbStruct: Count in bytes of the data to store.
//
//  Return (BOOL):
//      The return value is TRUE if the function is successful. It is FALSE
//      if it fails.
//
//  History:
//       3/10/93
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppProfileWriteBytes
(
    PTSTR                   pszSection,
    PTSTR                   pszKey,
    LPBYTE                  pbStruct,
    UINT                    cbStruct
)
{
    static TCHAR achNibbleToChar[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };
    #define     NIBBLE2CHAR(x)      (achNibbleToChar[x])

    TCHAR       ach[APP_MAX_STRING_RC_CHARS];
    LPTSTR      psz;
    LPTSTR      pch;
    UINT        cchTemp;
    BOOL        fAllocated;
    BOOL        fReturn;
    BYTE        b;
    BYTE        bChecksum;

    //
    //  if pbStruct is NULL, then erase the key from the ini file, otherwise
    //  format the raw bytes into a hex string and write that out...
    //
    fAllocated = FALSE;
    psz        = NULL;
    if (NULL != pbStruct)
    {
        //
        //  check if the quick buffer can be used for formatting the output
        //  text--if it cannot, then alloc space for it. note that space
        //  must be available for an ending checksum byte (2 bytes for high
        //  and low nibble) as well as a null terminator.
        //
        psz     = (LPTSTR)ach;
        cchTemp = cbStruct * 2 + 3;
        if (cchTemp > SIZEOF(ach))
        {
            psz = (LPTSTR)GlobalAllocPtr(GHND, cchTemp * sizeof(TCHAR));
            if (NULL == psz)
                return (FALSE);

            fAllocated = TRUE;
        }

        //
        //  step through all bytes in the structure and convert it to
        //  a string of hex numbers...
        //
        bChecksum = 0;
        for (pch = psz; 0 != cbStruct; cbStruct--, pbStruct++)
        {
            //
            //  grab the next byte and add into checksum...
            //
            bChecksum = (BYTE) (bChecksum + (b = *pbStruct));

            *pch++ = NIBBLE2CHAR((b >> (BYTE)4) & (BYTE)0x0F);
            *pch++ = NIBBLE2CHAR(b & (BYTE)0x0F);
        }

        //
        //  add the checksum byte to the end and null terminate the hex
        //  dumped string...
        //
        *pch++ = NIBBLE2CHAR((bChecksum >> (BYTE)4) & (BYTE)0x0F);
        *pch++ = NIBBLE2CHAR(bChecksum & (BYTE)0x0F);
        *pch   = '\0';
    }

    //
    //  write the string of hex bytes out to the ini file...
    //
    fReturn = WritePrivateProfileString(pszSection, pszKey, psz, gszAppProfile);

    //
    //  free the temporary buffer if one was allocated (lots of bytes!)
    //
    if (fAllocated)
        GlobalFreePtr(psz);

    return (fReturn);
} // AppProfileWriteBytes


//--------------------------------------------------------------------------;
//
//  BOOL AppProfileReadBytes
//
//  Description:
//      This function reads a previously stored structure of bytes from
//      the application's ini file. This data must have been written with
//      the AppProfileWriteBytes function--it is checksumed to keep bad
//      data from blowing up the application.
//
//      NOTE! Starting with Windows for Workgroups 3.1 there are two new
//      profile functions that provide the same functionality of this
//      function. Specifically, these functions are GetPrivateProfileStruct
//      and WritePrivateProfileStruct. These new functions are provided
//      by the Common Controls DLL. The prototypes are as follows:
//
//      BOOL GetPrivateProfileStruct
//      (
//          LPSTR       szSection,
//          LPSTR       szKey,
//          LPBYTE      lpStruct,
//          UINT        uSizeStruct,
//          LPSTR       szFile
//      );
//
//      BOOL WritePrivateProfileStruct
//      (
//          LPSTR       szSection,
//          LPSTR       szKey,
//          LPBYTE      lpStruct,
//          UINT        uSizeStruct,
//          LPSTR       szFile
//      );
//
//      If you are building an application that is for Window for Workgroups
//      or newer versions of Windows, you will probably want to use the
//      above functions.
//
//  Arguments:
//      PTSTR pszSection: Pointer to section that contains the data.
//
//      PTSTR pszKey: Pointer to key that contains the data.
//
//      LPBYTE pbStruct: Pointer to buffer to receive the data.
//
//      UINT cbStruct: Number of bytes expected.
//
//  Return (BOOL):
//      The return value is TRUE if the function is successful. It is FALSE
//      if the function fails (bad checksum, missing key, etc).
//
//  History:
//       3/10/93
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppProfileReadBytes
(
    PTSTR                   pszSection,
    PTSTR                   pszKey,
    LPBYTE                  pbStruct,
    UINT                    cbStruct
)
{
    //
    //  note that the following works for both upper and lower case, and
    //  will return valid values for garbage chars
    //
    #define CHAR2NIBBLE(ch) (BYTE)( ((ch) >= '0' && (ch) <= '9') ?  \
                                (BYTE)((ch) - '0') :                \
                                ((BYTE)(10 + (ch) - 'A') & (BYTE)0x0F) )

    TCHAR       ach[APP_MAX_STRING_RC_CHARS];
    LPTSTR      psz;
    LPTSTR      pch;
    UINT        cchTemp;
    UINT        u;
    BOOL        fAllocated;
    BOOL        fReturn;
    BYTE        b;
    BYTE        bChecksum;
    TCHAR       ch;

    //
    //  add one the the number of bytes needed to accomodate the checksum
    //  byte placed at the end by AppProfileWriteBytes...
    //
    cbStruct++;

    //
    //  check if the quick buffer can be used for retrieving the input
    //  text--if it cannot, then alloc space for it. note that there must
    //  be space available for the null terminator (the +1 below).
    //
    fAllocated = FALSE;
    psz        = (LPTSTR)ach;
    cchTemp    = cbStruct * 2 + 1;
    if (cchTemp > SIZEOF(ach))
    {
        psz = (LPTSTR)GlobalAllocPtr(GHND, cchTemp * sizeof(TCHAR));
        if (NULL == psz)
            return (FALSE);

        fAllocated = TRUE;
    }

    //
    //  read the hex string... if it is not the correct length, then assume
    //  error and return.
    //
    fReturn = FALSE;
    u = (UINT)GetPrivateProfileString(pszSection, pszKey, gszNull,
                                      psz, cchTemp, gszAppProfile);
    if ((cbStruct * 2) == u)
    {
        bChecksum = 0;
        for (pch = psz; 0 != cbStruct; cbStruct--, pbStruct++)
        {
            ch = *pch++;
            b  = (BYTE) (CHAR2NIBBLE(ch) << (BYTE)4);
            ch = *pch++;
            b |= CHAR2NIBBLE(ch);

            //
            //  if this is not the final byte (the checksum byte), then
            //  store it and accumulate checksum..
            //
            if (cbStruct != 1)
                bChecksum = (BYTE) (bChecksum + (*pbStruct = b));
        }

        //
        //  check the last byte read against the checksum that we calculated
        //  if they are not equal then return error...
        //
        fReturn = (bChecksum == b);
    }


    //
    //  free the temporary buffer if one was allocated (lots of bytes!)
    //
    if (fAllocated)
        GlobalFreePtr(psz);

    return (fReturn);
} // AppProfileReadBytes


//--------------------------------------------------------------------------;
//
//  DWORD AppGetWindowsVersion
//
//  Description:
//      This function returns the version of Windows that the application
//      is running on plus some platform information.
//
//  Arguments:
//      PTSTR pach: Options pointer to buffer to receive text string of
//      the Windows version and platform.
//
//  Return (LRESULT):
//      The return value will be the version and platform information of
//      the current operating system in the following format:
//
//      0xPPPPMMRR where:
//
//      MM      :   major version of Windows
//      RR      :   minor version (revision) of Windows
//      PPPP    :   the platform the application is running on which
//                  the HIWORD() is RESERVED except for the high bit:
//                      high bit is 0 = Windows NT
//                      high bit is 1 = Win32s/Windows 3.1
//
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppGetWindowsVersion
(
    PTSTR                   pszEnvironment,
    PTSTR                   pszPlatform
)
{

    BYTE    bVerWinMajor;
    BYTE    bVerWinMinor;
    UINT    uVerEnv;
    DWORD   dw;
    LRESULT lr;

    dw = GetVersion();

    //
    //  massage the version information into something intelligent
    //
    //
    bVerWinMajor = LOBYTE(LOWORD(dw));
    bVerWinMinor = HIBYTE(LOWORD(dw));
    uVerEnv      = HIWORD(dw);
    lr = MAKELPARAM(((UINT)bVerWinMajor << 8) | bVerWinMinor, uVerEnv);

    //
    //  if caller wants the environment string version...
    //
    if (NULL != pszEnvironment)
    {
        static TCHAR    szFormatVersion[]   = TEXT("Win32 Version %u.%.2u");

        wsprintf(pszEnvironment, szFormatVersion,
                 bVerWinMajor, bVerWinMinor);
    }

    //
    //  if caller wants the platform string version...
    //
    if (NULL != pszPlatform)
    {
        static TCHAR    szFormatPlatform[]  = TEXT("%s%u, %u Processor(s)");
        static TCHAR    szProcessorIntel[]  = TEXT("Intel ");
        static TCHAR    szProcessorMIPS[]   = TEXT("MIPS R");
        static TCHAR    szProcessorAlpha[]  = TEXT("DEC Alpha ");
        static TCHAR    szProcessorDunno[]  = TEXT("Dunno zYz");

        SYSTEM_INFO sysinfo;
        PTSTR       pszProcessor;

        //
        //  this is absolutely silly. one would think that the dwOemId member
        //  would provide something useful like the processor class... but
        //  no, it doesn't--it is always 0.
        //
        GetSystemInfo(&sysinfo);
        switch (sysinfo.dwProcessorType)
        {
            case PROCESSOR_INTEL_386:
            case PROCESSOR_INTEL_486:
                pszProcessor = szProcessorIntel;
                break;

            case PROCESSOR_MIPS_R4000:
                pszProcessor = szProcessorMIPS;
                break;

            case PROCESSOR_ALPHA_21064:
                pszProcessor = szProcessorAlpha;
                break;

            default:
                pszProcessor = szProcessorDunno;
                break;
        }

        //
        //
        //
        wsprintf(pszPlatform, szFormatPlatform, (LPSTR)pszProcessor,
                 sysinfo.dwProcessorType, sysinfo.dwNumberOfProcessors);
    }

    //
    //  return the result
    //
    return (lr);
} // AppGetWindowsVersion()


//--------------------------------------------------------------------------;
//
//  HFONT AppChooseFont
//
//  Description:
//      This function is a wrapper for the ChooseFont() common dialog.
//      The purpose of this function is to let the user choose a font that
//      looks good to them--regardless of how stupid it really looks.
//
//  Arguments:
//      HWND hwnd: Handle to parent window for chooser dialog.
//
//      HFONT hfont: Handle to current font (default for chooser dialog).
//
//      PLOGFONT plf: Pointer to optional LOGFONT structure to receive a
//      copy of the LOGFONT information for the newly chosen font.
//
//  Return (HFONT):
//      The return value is the newly chosen font. If no new font was chosen
//      then the return value is NULL.
//
//--------------------------------------------------------------------------;

HFONT FNGLOBAL AppChooseFont
(
    HWND                    hwnd,
    HFONT                   hfont,
    PLOGFONT                plf
)
{
    LOGFONT             lf;
    CHOOSEFONT          cf;
    BOOL                f;
    HFONT               hfontNew;

    //
    //  get the font info for the current font...
    //
    GetObject(hfont, sizeof(LOGFONT), (LPVOID)&lf);

    //
    //  fill in the choosefont structure
    //
    cf.lStructSize  = sizeof(CHOOSEFONT);
    cf.hwndOwner    = hwnd;
    cf.hDC          = NULL;
    cf.Flags        = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
    cf.lCustData    = 0;
    cf.lpfnHook     = NULL;
    cf.hInstance    = NULL;
    cf.nFontType    = SCREEN_FONTTYPE;
    cf.lpLogFont    = (LPLOGFONT)&lf;

    //
    //  splash a dialog into the user's face..
    //
    hfontNew = NULL;
    f = ChooseFont(&cf);
    if (f)
    {
        //
        //  create the new font..
        //
        hfontNew = CreateFontIndirect(&lf);
        if (NULL == hfontNew)
            return (NULL);

        //
        //  copy the logfont structure if caller wants it
        //
        if (NULL != plf)
            *plf = lf;
    }

    //
    //  return the new font (if one was chosen)
    //
    return (hfontNew);
} // AppChooseFont()


//==========================================================================;
//
//  Misc rarely used application dialogs and stuff...
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL AboutDlgProc
//
//  Description:
//      This dialog procedure is used for the ubiquitous about box.
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
//       1/ 2/93
//
//--------------------------------------------------------------------------;

BOOL CALLBACK AboutDlgProc
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    HWND                hwndT;
    PTSTR               pach;
    UINT                u;

    switch (uMsg)
    {
        case WM_INITDIALOG:
            //
            //  display some OS version information
            //
            //
            pach = (PTSTR)LocalAlloc(LPTR, APP_MAX_STRING_RC_BYTES);
            if (NULL == pach)
                return (TRUE);

            AppGetWindowsVersion(pach, NULL);
            hwndT = GetDlgItem(hwnd, IDD_ABOUT_VERSION_OS);
            SetWindowText(hwndT, pach);

            AppGetWindowsVersion(NULL, pach);
            hwndT = GetDlgItem(hwnd, IDD_ABOUT_VERSION_PLATFORM);
            SetWindowText(hwndT, pach);
            LocalFree((HLOCAL)pach);
            return (TRUE);

        case WM_COMMAND:
            u = GET_WM_COMMAND_ID(wParam, lParam);
            if ((IDOK == u) || (IDCANCEL == u))
            {
                EndDialog(hwnd, (IDOK == u));
            }
            break;
    }

    return (FALSE);
} // AboutDlgProc()


//==========================================================================;
//
//  Initialization and exit code...
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppChooseFont
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
//       2/ 7/93
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MixAppChooseFont
(
    HWND                    hwnd
)
{
    LOGFONT             lf;
    HWND                hlb;
    HFONT               hfont;
    HFONT               hfontNew;


    //
    //  get the current font and pass it to the choose font dialog
    //
    hlb   = GetDlgItem(hwnd, IDD_APP_LIST_LINES);
    hfont = GetWindowFont(hlb);

    hfontNew = AppChooseFont(hwnd, hfont, &lf);
    if (NULL == hfontNew)
        return (FALSE);

    //
    //  select the new font into the script window and delete the old one
    //
    TlbSetFont(gptlbLines, hfontNew, TRUE);
    DeleteFont(hfont);


    //
    //  save the complete description of the chosen font so there can be
    //  no strangness in the font mapping next run. this is overkill, but
    //  it works...
    //
    AppProfileWriteBytes(gszSecConfig, gszKeyFont, (LPBYTE)&lf, sizeof(lf));

    return (TRUE);
} // MixAppChooseFont()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppSettingsRestore
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
//      05/11/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppSettingsRestore
(
    HWND                    hwnd,
    int                     nCmdShow
)
{
    WINDOWPLACEMENT     wp;
    HFONT               hfont;
    LOGFONT             lf;
    RECT                rc;
    RECT                rcWindow;
    POINT               pt;
    int                 n;
    BOOL                f;


    //
    //  restore the user's preferred font.
    //
    hfont = NULL;
    f = AppProfileReadBytes(gszSecConfig, gszKeyFont, (LPBYTE)&lf, sizeof(lf));
    if (f)
    {
        hfont = CreateFontIndirect(&lf);
    }

    if (NULL == hfont)
    {
        hfont = GetStockFont(ANSI_VAR_FONT);
    }

    TlbSetFont(gptlbLines, hfont, TRUE);


    //
    //  grab the stored window position and size from the .ini file...
    //  there must be four arguments stored or the entry is considered
    //  invalid.
    //
    f = AppProfileReadBytes(gszSecConfig, gszKeyWindow,
                            (LPBYTE)&rcWindow, sizeof(rcWindow));
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
} // MixAppSettingsRestore()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppSettingsSave
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
//      05/11/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppSettingsSave
(
    HWND                    hwnd
)
{
    WINDOWPLACEMENT     wp;
    PRECT               prc;
    BOOL                f;

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

        DPF(1, "WindowPlacement: show=%d, minX=%d, minY=%d, maxX=%d, maxY=%d",
             wp.showCmd, wp.ptMinPosition.x, wp.ptMinPosition.y,
             wp.ptMaxPosition.x, wp.ptMaxPosition.y);

        DPF(1, "                 normX=%d, normY=%d, normW=%d, normH=%d",
             prc->left, prc->top, prc->right, prc->bottom);

        //
        //  save the _bounding rectangle_ of the restored window state...
        //
        AppProfileWriteBytes(gszSecConfig, gszKeyWindow, (LPBYTE)prc, sizeof(*prc));
    }


    //
    //  succeed
    //
    return (TRUE);
} // MixAppSettingsSave()


//--------------------------------------------------------------------------;
//
//  LRESULT AppCreate
//
//  Description:
//      This function is called to handle the WM_CREATE message for the
//      application's window. The application should finish the creation
//      of the window (create controls, allocate resources, etc). The
//      window has not been displayed (CreateWindow[Ex] has not returned).
//
//  Arguments:
//      HWND hwnd: Handle to the window that is in the process of being
//      created.
//
//      LPCREATESTRUCT pcs: Pointer to a CREATESTRUCT that contains info
//      about the window being created.
//
//  Return (LRESULT):
//      The return value should be nonzero if the application wishes to
//      let the window finish being created. A return of zero tells
//      CreateWindow[Ex] to fail the creation of the window.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppCreate
(
    HWND                    hwnd,
    LPCREATESTRUCT          pcs
)
{
    extern TCHAR       gszLineFormatTitle[];

    DPF(1, "AppCreate(hwnd=%Xh, cs.x=%d, cs.y=%d, cs.cx=%d, cs.cy=%d)",
            hwnd, pcs->x, pcs->y, pcs->cx, pcs->cy);

    //
    //  create the driver selection listbox
    //
    gptlbLines = TlbCreate(hwnd, IDD_APP_LIST_LINES, NULL);
    if (NULL == gptlbLines)
        return (0L);

    TlbSetTitleAndTabs(gptlbLines, gszLineFormatTitle, FALSE);


    //
    //  default to device id zero... a real app would allow configuration
    //  and set to previous selection. but this is not a real app.
    //
    ghmx = MixAppNewDevice(hwnd, NULL, 0);
    if (NULL == ghmx)
    {
        //
        //  must be having a very bad day..
        //
        return (0L);
    }


    //
    //
    //
    MixAppRefreshLineList(hwnd, gptlbLines);


    //
    //  return nonzero to succeed the creation of the window
    //
    return (1L);
} // AppCreate()


//--------------------------------------------------------------------------;
//
//  LRESULT AppQueryEndSession
//
//  Description:
//      This function handles the WM_QUERYENDSESSION. This message is sent
//      by USER when ExitWindows has been called to end the Windows session.
//      This function can stop Windows from exiting if it is not convenient
//      for Windows to end.
//
//      Giving the user the option to save modified data before continueing
//      with the shutdown of Windows is a good idea.
//
//      Telling Windows to continue with the exit procedure does not
//      necessarily mean Windows will exit. All applications are queried
//      for shutdown approval. When the actual decision is made on whether
//      Windows will exit, WM_ENDSESSION will be sent with the result.
//
//  Arguments:
//      HWND hwnd: Handle to window that received the message.
//
//  Return (LRESULT):
//      Returns zero to STOP Windows from exiting. Returns non-zero to
//      allows windows to shut down.
//
//  History:
//       2/ 9/93
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppQueryEndSession
(
    HWND                    hwnd
)
{
    DPF(1, "AppQueryEndSession(hwnd=%Xh)", hwnd);

    //
    //  tell Windows to proceed with the shutdown process!
    //
    return (1L);
} // AppQueryEndSession()


//--------------------------------------------------------------------------;
//
//  LRESULT AppEndSession
//
//  Description:
//      This function is called to handle the WM_ENDSESSION message. This
//      message is generated after the application answers the
//      WM_QUERYENDSESSION message. The purpose of the WM_ENDSESSION
//      message is to tell the application if Windows will be exiting
//      (TRUE  == fEndSession) or the end session was canceled by an
//      application (FALSE == fEndSession).
//
//  Arguments:
//      HWND hwnd: Handle to window that received the message.
//
//      BOOL fEndSession: TRUE if Windows is exiting. FALSE if the end
//      session was canceled.
//
//  Return (LRESULT):
//      Returns zero if the message is processed. Note that an application
//      cannot halt the termination of Windows from this message--the
//      WM_QUERYENDSESSION is the only message that allows that behaviour.
//      If fEndSession is TRUE, Windows *WILL* exit--whether you like it
//      or not.
//
//  History:
//       2/ 9/93
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppEndSession
(
    HWND                    hwnd,
    BOOL                    fEndSession
)
{
    DPF(1, "AppEndSession(hwnd=%Xh, fEndSession=%d)", hwnd, fEndSession);

    //
    //  we processed the message, return zero..
    //
    return (0L);
} // AppEndSession()


//--------------------------------------------------------------------------;
//
//  LRESULT AppClose
//
//  Description:
//      This function handles the WM_CLOSE message for the application.
//      If the application should close, DestroyWindow() must be called
//      by this function. Otherwise the application will not close.
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_CLOSE message.
//
//  Return (LRESULT):
//      There return value is zero. The DestroyWindow function will have
//      been called if the application should actually close.
//
//  History:
//       2/ 6/93
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppClose
(
    HWND                    hwnd
)
{
    HWND                hlb;
    HFONT               hfont;
    MMRESULT            mmr;


    DPF(1, "AppClose(hwnd=%Xh)", hwnd);

    //
    //  save any settings that should be saved on app termination...
    //
    MixAppSettingsSave(hwnd);


    //
    //  if the Shift key is held down during the close message, then just
    //  save the current state but don't destroy the window... this is
    //  useful if the user does not want to exit the app and rerun it
    //  to make sure the state is saved--just before the user does something
    //  that may crash Windows or something..
    //
    if (GetKeyState(VK_SHIFT) < 0)
    {
        return (0L);
    }


    //
    //  destroy the font we are using... before deleting the font, select
    //  the system font back into the script window so the font won't
    //  be 'in use' anymore.
    //
    hlb = GetDlgItem(hwnd, IDD_APP_LIST_LINES);

    hfont = GetWindowFont(hlb);
    SetWindowFont(hlb, NULL, FALSE);
    DeleteFont(hfont);



    //
    //
    //
    if (NULL != ghmx)
    {
        mmr = mixerClose(ghmx);
        if (MMSYSERR_NOERROR != mmr)
        {
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "mixerClose() failed on hmx=%.04Xh, mmr=%u!",
                      ghmx, mmr);
        }

        ghmx = NULL;
    }


    gptlbLines->hlb = NULL;
    TlbDestroy(gptlbLines);
    gptlbLines = NULL;


    //
    //  make the window close and terminate the application
    //
    DestroyWindow(hwnd);

    return (0L);
} // AppClose()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL AppInit
//
//  Description:
//      This function is called to initialize a new instance of the
//      application. We want to parse our command line, create our window,
//      allocate resources, etc.
//
//      The arguments passed to this function are exactly the same as
//      those passed to WinMain.
//
//  Arguments:
//      HINSTANCE hinst: Identifies the current instance of the
//      application.
//
//      HINSTANCE hinstPrev: Identifies the previous instance of the
//      application (NULL if first instance). For Win 32, this argument
//      is _always_ NULL.
//
//      LPTSTR pszCmdLine: Points to null-terminated unparsed command line.
//      If the application is compiled for Unicode, then this argument is
//      ignored.
//
//      int nCmdShow: How the main window for the application is to be
//      shown by default.
//
//  Return (HWND):
//      Returns the newly created handle to the applications main window.
//      This handle is NULL if something went wrong and tells the application
//      to exit immediately.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

HWND FNGLOBAL AppInit
(
    HINSTANCE               hinst,
    HINSTANCE               hinstPrev,
    LPTSTR                  pszCmdLine,
    int                     nCmdShow
)
{
    LRESULT CALLBACK AppWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND                hwnd;
    WNDCLASS            wc;
    UINT                cMixerDevs;

    DPF(1, "AppInit(hinst=%Xh, hinstPrev=%Xh, pszCmdLine='%s', nCmdShow=%d)",
            hinst, hinstPrev, pszCmdLine, nCmdShow);

    LoadString(hinst, IDS_APP_NAME, gszAppName, SIZEOF(gszAppName));


    //
    //  WARNING DANGER WARNING DANGER WARNING DANGER WARNING DANGER WARNING
    //
    //      BEFORE calling any other mixer API's, we must call the
    //      mixerGetNumDevs() function to let the mixer thunk library
    //      dynamically link to all the mixer API's!
    //
    //  WARNING DANGER WARNING DANGER WARNING DANGER WARNING DANGER WARNING
    //
    cMixerDevs = mixerGetNumDevs();
    if (0 == cMixerDevs)
    {
        AppMsgBox(NULL, MB_OK | MB_ICONEXCLAMATION,
                  "There are no mixer devices installed. This application is useless.");
        return (NULL);
    }


    //
    //  determine whether a new window class needs to be registered for
    //  this application. for Win 16, this only needs to be done for the
    //  first instance of the application created. for Win 32, this must
    //  be done for EVERY instance of the application.
    //
    if (NULL == hinstPrev)
    {
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = (WNDPROC)AppWndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hinst;
        wc.hIcon         = LoadIcon(hinst, ICON_APP);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName  = MENU_APP;
        wc.lpszClassName = gszAppName;

        if (!RegisterClass(&wc))
            return (NULL);
    }


    //
    //  create the application's main window
    //
    //  style bits available:
    //      WS_EX_ACCEPTFILES   :  will receive WM_DROPFILES messages
    //      WS_EX_DLGMODALFRAME :  creates window with double border
    //      WS_EX_NOPARENTNOTIFY:  won't receive WM_PARENTNOTIFY messages
    //      WS_EX_TOPMOST       :  puts window in topmost space
    //      WS_EX_TRANSPARENT   :  a very bizarre style indeed (Win 16 only)
    //
    hwnd = CreateWindowEx(WS_EX_NOPARENTNOTIFY,
                          gszAppName,
                          gszAppName,
                          WS_OVERLAPPEDWINDOW,
                          APP_WINDOW_XOFFSET,
                          APP_WINDOW_YOFFSET,
                          APP_WINDOW_WIDTH,
                          APP_WINDOW_HEIGHT,
                          NULL,
                          NULL,
                          hinst,
                          NULL);

    if (NULL == hwnd)
        return (NULL);

#ifdef UNICODE
    //
    //  the application--which is different than the pszCmdLine argument
    //  passed through WinMain()...
    //
    //  so, skip over the command name to get to the argument string
    //
    pszCmdLine = GetCommandLine();
    if (NULL != pszCmdLine)
    {
        while (('\0' != *pszCmdLine) && (' ' != *pszCmdLine++))
            ;
    }
#endif


    //
    //
    //
    //
    MixAppSettingsRestore(hwnd, nCmdShow);


    //
    //  finally, get the window displayed and return success
    //
    //  the ShowWindow call is made during MixAppInit
    //
//  ShowWindow(hwnd, nCmdShow);
//  UpdateWindow(hwnd);

    return (hwnd);
} // AppInit()


//--------------------------------------------------------------------------;
//
//  int AppExit
//
//  Description:
//      This function is called just before the application exits from
//      WinMain. Its purpose is to clean up any resources that were allocated
//      for running the application: brushes, heaps, etc..
//
//  Arguments:
//      HINSTANCE hinst: Identifies the current instance of the
//      application that is exiting.
//
//      int nResult: The result of the WM_QUIT message (in wParam of the
//      MSG structure. This argument will usually be 0 (even if the message
//      loop was never entered).
//
//  Return (int):
//      The return value is usually nResult--be we give this function the
//      opportunity to modify its value.
//
//  History:
//      11/ 8/92
//
//--------------------------------------------------------------------------;

int FNGLOBAL AppExit
(
    HINSTANCE               hinst,
    int                     nResult
)
{
    DPF(1, "AppExit(hinst=%Xh, nResult=%d)", hinst, nResult);

    //
    //
    //
    //

    return (nResult);
} // AppExit()
