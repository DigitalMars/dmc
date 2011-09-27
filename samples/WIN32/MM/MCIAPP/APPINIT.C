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
//  appinit.c
//
//  Description:
//      This file contains initialization and termination code for the
//      application (as well as some rarely used stuff).
//
//  History:
//      11/ 8/92    created.
//
//==========================================================================;

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commdlg.h>
#ifndef WIN32
#include <shellapi.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "appport.h"
#include "app.h"
#include "mciapp.h"

#include "debug.h"



//==========================================================================;
//
//  Application helper functions and rarely called stuff...
//
//
//==========================================================================;

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
//                  will be one of the following:
//
//                  #ifdef WIN32
//                      the HIWORD() is RESERVED except for the high bit:
//                          high bit is 0 = Windows NT
//                          high bit is 1 = Win32s/Windows 3.1
//                  #else
//                      0xMMRR = Major and Minor version of [MS-]DOS
//                      GetWinFlags() & 0x8000 = Windows on OS/2 (WLO)
//                      GetWinFlags() & 0x4000 = Windows on Windows NT (WOW)
//                  #endif
//
//  History:
//       2/13/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppGetWindowsVersion
(
    PTSTR           pszEnvironment,
    PTSTR           pszPlatform
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
    //
    //
    //
#ifdef WIN32
{
    static TCHAR    szFormatVersion[]   = TEXT("%s Version %u.%.2u");
    static TCHAR    szEnvWinNT[]        = TEXT("Windows NT");
    static TCHAR    szEnvWin32s[]       = TEXT("Win32s");

    wsprintf(pszEnvironment, szFormatVersion,
             (LPSTR)((0x8000 & uVerEnv) ? szEnvWin32s : szEnvWinNT),
             bVerWinMajor, bVerWinMinor);
}
#else
{
#ifndef WF_WINNT
    #define WF_WINNT        0x4000
    #define WF_WLO          0x8000
#endif
#ifndef WF_CPUMASK
    #define WF_CPUMASK      0xFC000000
    #define WF_CPU_X86      0
    #define WF_CPU_R4000    1
    #define WF_CPU_ALPHA    2
    #define WF_CPU_CLIPPER  3
#endif

    static TCHAR    szFormatSubSys[]= TEXT("Windows Version %u.%.2u (%s%s)\n%s Subsystem, DOS Version %u.%.2u");
    static TCHAR    szFormatDOS[]   = TEXT("Windows Version %u.%.2u (%s%s)\nDOS Version %u.%.2u");
    static TCHAR    szSubSysWLO[]   = TEXT("WLO");
    static TCHAR    szSubSysWOW[]   = TEXT("WOW");
    static TCHAR    szModeEnhanced[]= TEXT("Enhanced");
    static TCHAR    szModeStandard[]= TEXT("Standard");
    static TCHAR    szEnvPaging[]   = TEXT(", Paging");

    DWORD   dwWinFlags;
    PTSTR   pszMode;

    BYTE    bVerEnvMajor    = HIBYTE(LOWORD(uVerEnv));
    BYTE    bVerEnvMinor    = LOBYTE(LOWORD(uVerEnv));

    dwWinFlags = GetWinFlags();

    pszMode = (dwWinFlags & WF_ENHANCED) ? szModeEnhanced : szModeStandard;
    if (dwWinFlags & (WF_WLO | WF_WINNT))
    {
        wsprintf(pszEnvironment, szFormatSubSys, bVerWinMajor, bVerWinMinor,
                 (LPSTR)pszMode,
                 (LPSTR)((dwWinFlags & WF_PAGING) ? szEnvPaging : gszNull),
                 (LPSTR)((dwWinFlags & WF_WINNT) ? szSubSysWOW : szSubSysWLO),
                 bVerEnvMajor, bVerEnvMinor);
    }
    else
    {
        wsprintf(pszEnvironment, szFormatDOS, bVerWinMajor, bVerWinMinor,
                 (LPSTR)pszMode,
                 (LPSTR)((dwWinFlags & WF_PAGING) ? szEnvPaging : gszNull),
                 bVerEnvMajor, bVerEnvMinor);
    }
}
#endif
    }

    //
    //  if caller wants the platform string version...
    //
    if (NULL != pszPlatform)
    {
#ifdef WIN32
{
    static TCHAR    szFormatPlatform[]  = TEXT("%s%u, %u Processor(s)");
    static TCHAR    szProcessorIntel[]  = TEXT("Intel ");
    static TCHAR    szProcessorMIPS[]   = TEXT("MIPS R");
    static TCHAR    szProcessorAlpha[]  = TEXT("DEC Alpha ");
    static TCHAR    szProcessorUnknown[]  = TEXT("Unknown");

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
        case PROCESSOR_INTEL_PENTIUM:
            pszProcessor = szProcessorIntel;
            break;

     // case PROCESSOR_MIPS_R2000:
     // case PROCESSOR_MIPS_R3000:
        case PROCESSOR_MIPS_R4000:
            pszProcessor = szProcessorMIPS;
            break;

        case PROCESSOR_ALPHA_21064:
            pszProcessor = szProcessorAlpha;
            break;

        default:
            pszProcessor = szProcessorUnknown;
            break;
    }

    //
    //
    //
    if (pszProcessor != szProcessorUnknown)
        wsprintf(pszPlatform, szFormatPlatform, (LPSTR)pszProcessor,
                 sysinfo.dwProcessorType, sysinfo.dwNumberOfProcessors);
    else
        lstrcpy(pszPlatform, pszProcessor);
}
#else
{
    static TCHAR    szPlat286[]         = TEXT("80286");
    static TCHAR    szPlat386[]         = TEXT("80386");
    static TCHAR    szPlat486[]         = TEXT("i486");
    static TCHAR    szPlatR4000[]       = TEXT("MIPS R4000, Emulation: ");
    static TCHAR    szPlatAlpha21064[]  = TEXT("Alpha 21064, Emulation: ");
    static TCHAR    szPlatClipper[]     = TEXT("Clipper, Emulation: ");
    static TCHAR    szPlat80x87[]       = TEXT(", 80x87");

    DWORD   dwWinFlags;

    dwWinFlags = GetWinFlags();
    pszPlatform[0] = '\0';

    if (dwWinFlags & (WF_WLO | WF_WINNT))
    {
        switch ((dwWinFlags & WF_CPUMASK) >> 26)
        {
            case WF_CPU_X86:
                break;

            case WF_CPU_R4000:
                lstrcpy(pszPlatform, szPlatR4000);
                break;

            case WF_CPU_ALPHA:
                lstrcpy(pszPlatform, szPlatAlpha21064);
                break;

            case WF_CPU_CLIPPER:
                lstrcpy(pszPlatform, szPlatClipper);
                break;
        }
    }

    if (dwWinFlags & WF_CPU286)
        lstrcat(pszPlatform, szPlat286);
    else if (dwWinFlags & WF_CPU386)
        lstrcat(pszPlatform, szPlat386);
    else if (dwWinFlags & WF_CPU486)
        lstrcat(pszPlatform, szPlat486);

    if (dwWinFlags & WF_80x87)
        lstrcat(pszPlatform, szPlat80x87);
}
#endif
    }

    //
    //  return the result
    //
    return (lr);
} // AppGetWindowsVersion()


//--------------------------------------------------------------------------;
//
//  LRESULT AppWinIniChange
//
//  Description:
//      This function is responsible for handling the WM_WININICHANGE
//      message. This message is sent when modifications have been made
//      to WIN.INI (from SystemParametersInfo() or other sources).
//
//      An application should re-enumerate system metrics (GetSystemMetrics)
//      and system color (GetSystemColors) information that it has cached.
//      Note that checking the section that was modified should be done if
//      some enumerations are time consuming.
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_INITMENUPOPUP
//      message.
//
//      LPCTSTR pszSection: Pointer to section name that has been modified
//      in WIN.INI. Note that this argument might be NULL (sent by apps
//      that were written incorrectly!). If it is NULL, then this application
//      should re-enumerate ALL metrics, colors, etc.
//
//  Return (LRESULT):
//      The return value is zero if the message is processed.
//
//  History:
//       2/15/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppWinIniChange
(
    HWND            hwnd,
    LPCTSTR         pszSection
)
{
    DPF(0, "AppWinIniChanged(hwnd=%Xh, pszSection='%s')",
            hwnd, (NULL == pszSection) ? TEXT("(null)") : pszSection);

    //
    //  we processed the message...
    //
    return (0L);
} // AppWinIniChange()


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
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

HFONT FNGLOBAL AppChooseFont
(
    HWND            hwnd,
    HFONT           hfont,
    PLOGFONT        plf
)
{
    LOGFONT     lf;
    CHOOSEFONT  cf;
    BOOL        f;
    HFONT       hfontNew;

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
//      PCTSTR pszKey: Pointer to key name for the stored data.
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
//       3/10/93   created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppProfileWriteBytes
(
    PCTSTR          pszKey,
    LPBYTE          pbStruct,
    UINT            cbStruct
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
            psz = GlobalAllocPtr(GHND, cchTemp * sizeof(TCHAR));
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
            bChecksum += (b = *pbStruct);

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
    fReturn = WriteProfileString(gszAppSection, pszKey, psz);

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
//      PCTSTR pszKey: Pointer to key that contains the data.
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
//       3/10/93   created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppProfileReadBytes
(
    PCTSTR          pszKey,
    LPBYTE          pbStruct,
    UINT            cbStruct
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
        psz = GlobalAllocPtr(GHND, cchTemp * sizeof(TCHAR));
        if (NULL == psz)
            return (FALSE);

        fAllocated = TRUE;
    }

    //
    //  read the hex string... if it is not the correct length, then assume
    //  error and return.
    //
    fReturn = FALSE;
    u = (UINT)GetProfileString(gszAppSection, pszKey, gszNull, psz, cchTemp);
    if ((cbStruct * 2) == u)
    {
        bChecksum = 0;
        for (pch = psz; 0 != cbStruct; cbStruct--, pbStruct++)
        {
            ch = *pch++;
            b  = CHAR2NIBBLE(ch) << (BYTE)4;
            ch = *pch++;
            b |= CHAR2NIBBLE(ch);

            //
            //  if this is not the final byte (the checksum byte), then
            //  store it and accumulate checksum..
            //
            if (cbStruct != 1)
                bChecksum += (*pbStruct = b);
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


//==========================================================================;
//
//  Initialization and exit code...
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL AppRegisterPenApp
//
//  Description:
//      This function is used to register and de-register an application
//      as being 'Pen Enhanced.' If the Windows installation is Pen enabled
//      then this function allows the RC Manager to replace all 'Edit'
//      controls with 'HEdit' controls.
//
//      This function must be called to register the application BEFORE
//      creating any edit controls.
//
//  Arguments:
//      BOOL fRegister: If this argument is TRUE, the app is registered
//      with the RC Manager as being Pen aware. If this argument is FALSE
//      the app is de-registered.
//
//  Return (BOOL):
//      The return value is TRUE if Windows for Pen Computing is installed
//      on the system. The return value is FALSE if the Windows installation
//      is not Pen enabled.
//
//  History:
//       3/ 1/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL AppRegisterPenApp
(
    BOOL    fRegister
)
{
    #define RPA_DEFAULT     0x0001

    typedef void (FAR PASCAL *PENWINREGISTERPROC)(UINT, BOOL);

    static char                 szRegPenApp[]   = "RegisterPenApp";
    static PENWINREGISTERPROC   pfnRegPenApp    = NULL;

    HINSTANCE   hinstPenWin;

    //
    //  check if Windows for Pen Computing is installed--and if it is,
    //  dyna-link to the RegisterPenApp() function.
    //
    //  if Pens are not supported, then return FALSE.
    //
    if (NULL == pfnRegPenApp)
    {
        hinstPenWin = (HINSTANCE)GetSystemMetrics(SM_PENWINDOWS);
        if (NULL == hinstPenWin)
            return (FALSE);

        (FARPROC)pfnRegPenApp = GetProcAddress(hinstPenWin, szRegPenApp);
        if (NULL == pfnRegPenApp)
            return (FALSE);
    }

    //
    //  either enable or disable the RC Manager's Pen meathooks..
    //
    (*pfnRegPenApp)(RPA_DEFAULT, fRegister);

    return (TRUE);
} // AppRegisterPenApp()


//--------------------------------------------------------------------------;
//
//  LRESULT AppCreate
//
//  Description:
//      This function is called to handle the WM_CREATE message for the
//      applications window. The application should finish the creation
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
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppCreate
(
    HWND            hwnd,
    LPCREATESTRUCT  pcs
)
{
    static TCHAR    szButtonStep[]  = TEXT("&Step");
    static TCHAR    szButtonGo[]    = TEXT("&Go");
    static TCHAR    szButtonRun[]   = TEXT("&Run");

    static TCHAR    szTextCount[]   = TEXT("&Count: ");
    static TCHAR    szTextOutput[]  = TEXT("Output: ");
    static TCHAR    szTextOne[]     = TEXT("1");        // !!!

    static TCHAR    szBunnySmag[]   = TEXT("zYzBunnySmag");
    static TCHAR    szButton[]      = TEXT("button");
    static TCHAR    szEdit[]        = TEXT("edit");
    static TCHAR    szStatic[]      = TEXT("static");

    HINSTANCE   hinst;
    HFONT       hfont;
    HFONT       hfontFixed;
    TEXTMETRIC  tm;
    HDC         hdc;
    HWND        hwndButton;
    HWND        hwndScript;
    HWND        hwndStatus;
    SIZE        sSize;
    int         nHeightButton;
    int         nWidthButton;
    int         nHeightText;

    DPF(0, "AppCreate(hwnd=%Xh, cs.x=%d, cs.y=%d, cs.cx=%d, cs.cy=%d)",
            hwnd, pcs->x, pcs->y, pcs->cx, pcs->cy);

    hinst = pcs->hInstance;

    //
    //  create the button bar and status bar--use a non-bold var font.
    //  to use the non-bold font and make the controls a 'proper' size,
    //  we need to get the metrics of the font.. note that the extent
    //  we use for the width of the buttons on the button bar are
    //  computed using the longest text we expect to display
    //
    hfont = GetStockFont(ANSI_VAR_FONT);
    hfontFixed = GetStockFont(ANSI_FIXED_FONT);
    hdc = GetDC(hwnd);
    {
        hfont = SelectFont(hdc, hfont);
        GetTextMetrics(hdc, &tm);
        GetTextExtentPoint(hdc, szButton, SIZEOF(szButton), (LPSIZE)&sSize);
        hfont = SelectFont(hdc, hfont);
    }
    ReleaseDC(hwnd, hdc);

    //
    //  compute some almost random numbers for width and height...
    //
    nHeightText   = (int)(tm.tmHeight + tm.tmExternalLeading);
    nHeightButton = nHeightText * 3 / 2 + 2;
    nWidthButton  = sSize.cx * 3 / 2;


    //
    //  create the script window--use a non-bold fixed font...
    //
    hwndScript = CreateWindow(szEdit, gszNull,
                              ES_LEFT | ES_MULTILINE | WS_TABSTOP |
                              ES_AUTOVSCROLL | ES_AUTOHSCROLL |
                              ES_NOHIDESEL | WS_BORDER | WS_VSCROLL |
                              WS_HSCROLL | WS_VISIBLE | WS_CHILD,
                              0, 0, 0, 0, hwnd, (HMENU)IDD_APP_EDIT_SCRIPT,
                              hinst, NULL);
    SetWindowFont(hwndScript, hfontFixed, FALSE);

    //
    //  when creating a multiline edit control, the default text limit is
    //  32k _even in Win 32_. note that in Win 32 you can set text length
    //  to >32k without sending EM_LIMITTEXT, but you cannot EDIT the text!
    //
    //  so, send a limit of 'max int' which is the largest size this
    //  application can deal with (selections, etc are integers):
    //
    //      Win 16  : 0x7FFF _bytes_
    //      Win 32  : 0x7FFFFFFF _characters_
    //
    Edit_LimitText(hwndScript, (int)((UINT)-1 << 1 >> 1));


    //
    //  the status bar
    //
    hwndStatus = CreateWindow(szEdit, gszNull,
                              ES_LEFT | ES_AUTOHSCROLL | ES_READONLY |
                              WS_VISIBLE | WS_CHILD,
                              0, 0, 0, nHeightText,
                              hwnd, (HMENU)IDD_APP_TEXT_STATUS, hinst, NULL);
    SetWindowFont(hwndStatus, hfont, FALSE);

    hwndStatus = CreateWindow(szStatic, gszNull,
                              WS_BORDER | WS_VISIBLE | WS_CHILD,
                              0, 0, 0, nHeightText,
                              hwnd, (HMENU)IDD_APP_BOX_SEPARATOR1, hinst, NULL);

    hwndStatus = CreateWindow(szEdit, gszNull,
                              ES_LEFT | ES_AUTOHSCROLL | ES_READONLY |
                              WS_VISIBLE | WS_CHILD,
                              0, 0, 0, nHeightText,
                              hwnd, (HMENU)IDD_APP_TEXT_NOTIFY, hinst, NULL);
    SetWindowFont(hwndStatus, hfont, FALSE);


    hwndStatus = CreateWindow(szStatic, gszNull,
                              WS_BORDER | WS_VISIBLE | WS_CHILD,
                              0, 0, 0, nHeightText,
                              hwnd, (HMENU)IDD_APP_BOX_SEPARATOR2, hinst, NULL);

    hwndStatus = CreateWindow(szStatic, gszNull,
                              SS_CENTER | WS_VISIBLE | WS_CHILD,
                              0, 0, 0, nHeightText,
                              hwnd, (HMENU)IDD_APP_TEXT_OPTIONS, hinst, NULL);
    SetWindowFont(hwndStatus, hfontFixed, FALSE);




    //
    //  the button bar
    //
    hwndButton = CreateWindow(szButton, szButtonStep,
                              BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                              0, 0, nWidthButton, nHeightButton,
                              hwnd, (HMENU)IDD_APP_BTN_STEP, hinst, NULL);
    SetWindowFont(hwndButton, hfont, FALSE);

    hwndButton = CreateWindow(szButton, szButtonGo,
                              BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                              nWidthButton, 0, nWidthButton, nHeightButton,
                              hwnd, (HMENU)IDD_APP_BTN_GO, hinst, NULL);
    SetWindowFont(hwndButton, hfont, FALSE);

    hwndButton = CreateWindow(szButton, szButtonRun,
                              BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                              nWidthButton * 2, 0, nWidthButton, nHeightButton,
                              hwnd, (HMENU)IDD_APP_BTN_RUN, hinst, NULL);
    SetWindowFont(hwndButton, hfont, FALSE);


    //
    //
    //
    hwndStatus = CreateWindow(szStatic, szTextCount,
                              SS_RIGHT | WS_VISIBLE | WS_CHILD,
                              nWidthButton * 3, (nHeightButton - nHeightText) / 2,
                              nWidthButton, nHeightText,
                              hwnd, (HMENU)-1, hinst, NULL);
    SetWindowFont(hwndStatus, hfont, FALSE);

    hwndScript = CreateWindow(szEdit, szTextOne,
                              ES_LEFT | ES_AUTOHSCROLL | WS_BORDER |
                              WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                              nWidthButton * 4, 0, nWidthButton, nHeightButton,
                              hwnd, (HMENU)IDD_APP_EDIT_RUNCOUNT, hinst, NULL);
    SetWindowFont(hwndScript, hfont, FALSE);


    //
    //
    //
    hwndStatus = CreateWindow(szStatic, szTextOutput,
                              SS_RIGHT | WS_VISIBLE | WS_CHILD,
                              nWidthButton * 5, (nHeightButton - nHeightText) / 2,
                              nWidthButton, nHeightText,
                              hwnd, (HMENU)-1, hinst, NULL);
    SetWindowFont(hwndStatus, hfont, FALSE);

    hwndStatus = CreateWindow(szEdit, gszNull,
                              ES_LEFT | ES_AUTOHSCROLL | ES_READONLY |
                              WS_VISIBLE | WS_CHILD,
                              nWidthButton * 6, (nHeightButton - nHeightText) / 2,
                              nWidthButton, nHeightText,
                              hwnd, (HMENU)IDD_APP_TEXT_OUTPUT, hinst, NULL);
    SetWindowFont(hwndStatus, hfont, FALSE);



    //
    //  we want the focus to default to the script window so the user
    //  can immediately start typing...
    //
    SetFocus(hwndScript);


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
//       2/ 9/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppQueryEndSession
(
    HWND            hwnd
)
{
    BOOL    f;

    DPF(0, "AppQueryEndSession(hwnd=%Xh)", hwnd);

    //
    //  attempt to shut down--if this fails (user canceled it, etc) then
    //  do NOT allow the Windows to exit.
    //
    f = MciAppShutdown(hwnd, gszAppFilePath, gszAppFileTitle);
    if (!f)
        return (0L);


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
//       2/ 9/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppEndSession
(
    HWND            hwnd,
    BOOL            fEndSession
)
{
    DPF(0, "AppEndSession(hwnd=%Xh, fEndSession=%d)", hwnd, fEndSession);

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
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL AppClose
(
    HWND            hwnd
)
{
    HWND        hwndScript;
    HFONT       hfont;
    BOOL        f;

    //
    //  if the Shift key is held down during the close message, then just
    //  save the current state but don't destroy the window... this is
    //  useful if the user does not want to exit the app and rerun it
    //  to make sure the state is saved--just before the user does something
    //  that may crash Windows or something..
    //
    if (GetKeyState(VK_SHIFT) < 0)
    {
        //
        //  save any settings that should be saved on app termination...
        //
        MciAppSettingsSave(hwnd);
        return (0L);
    }

    //
    //  attempt to shut down--if this fails (user canceled it, etc) then
    //  do NOT allow the window to be destroyed.
    //
    f = MciAppShutdown(hwnd, gszAppFilePath, gszAppFileTitle);
    if (!f)
        return (0L);

    //
    //  destroy the font we are using... before deleting the font, select
    //  the system font back into the script window so the font won't
    //  be 'in use' anymore.
    //
    hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);

    hfont = GetWindowFont(hwndScript);
    SetWindowFont(hwndScript, NULL, FALSE);
    DeleteFont(hfont);

    //
    //  make the window close and terminate the application
    //
    DestroyWindow(hwnd);

    return (0L);
} // AppClose()


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
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

HWND FNGLOBAL AppInit
(
    HINSTANCE       hinst,
    HINSTANCE       hinstPrev,
    LPTSTR          pszCmdLine,
    int             nCmdShow
)
{
    HWND        hwnd;
    WNDCLASS    wc;

    DPF(0, "AppInit(hinst=%Xh, hinstPrev=%Xh, pszCmdLine='%s', nCmdShow=%d)",
            hinst, hinstPrev, pszCmdLine, nCmdShow);

    LoadString(hinst, IDS_APP_NAME, gszAppName, SIZEOF(gszAppName));
    LoadString(hinst, IDS_FILE_UNTITLED, gszFileUntitled, SIZEOF(gszFileUntitled));


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
    //  if Windows for Pen Computing is installed, then allow the RC
    //  Manager to replace all edit controls created from this point on
    //  with hedit controls
    //
    AppRegisterPenApp(TRUE);


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
    hwnd = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_NOPARENTNOTIFY,
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
    //  GetCommandLine() returns a pointer to our command line. but this
    //  command line includes the complete command line used to launch
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
    MciAppInit(hwnd, gszAppFilePath, gszAppFileTitle, pszCmdLine, nCmdShow);


    //
    //  finally, get the window displayed and return success
    //
    //  the ShowWindow call is made during MciAppInit
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
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

int FNGLOBAL AppExit
(
    HINSTANCE       hinst,
    int             nResult
)
{
    DPF(0, "AppExit(hinst=%Xh, nResult=%d)", hinst, nResult);

    MciAppExit();

    //
    //  if Windows for Pen Computing is installed, then de-register the
    //  application so the RC Manager knows we will no longer need its
    //  services...
    //
    AppRegisterPenApp(FALSE);

    return (nResult);
} // AppExit()


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
//       1/ 2/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNEXPORT AboutDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
)
{
    HWND    hwndT;
    PTSTR   pach;
    UINT    u;

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
