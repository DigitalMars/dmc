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
/****************************************************************************
 *
 *   captest.c: Source Code for the CapTest Sample Program
 *
 *   Microsoft Video for Windows Capture Class Sample Program
 *
 ***************************************************************************/


#define ENABLE_ERROR_CALLBACK           1
#define ENABLE_STATUS_CALLBACK          1
#define ENABLE_VIDEOFRAME_CALLBACKS     0

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <vfw.h>
#include <mmreg.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dos.h>

#include "captest.h"

//
// Global Variables
//
TCHAR           gachAppName[]  = TEXT("CapTestApp") ;
TCHAR           gachIconName[] = TEXT("CapTestIcon") ;
TCHAR           gachMenuName[] = TEXT("CapTestMenu") ;
TCHAR           gachMCIDeviceName[21] = TEXT("VideoDisc") ;  // default MCI device
TCHAR           gachString[128] ;
TCHAR           gachBuffer[200] ;

HINSTANCE      ghInstApp ;
HWND           ghWndMain ;
HWND           ghWndCap ;
HANDLE         ghAccel ;
WORD           gwDeviceIndex ;
WORD           gwPalFrames = DEF_PALNUMFRAMES ;
WORD           gwPalColors = DEF_PALNUMCOLORS ;
WORD           gwCapFileSize ;
DWORD          gdwFrameNum ;
DWORD          gdwVideoNum ;

CAPSTATUS      gCapStatus ;
CAPDRIVERCAPS  gCapDriverCaps ;
CAPTUREPARMS   gCapParms ;

LPWAVEFORMATEX glpwfex ;

// MakeProcInstance is only required for 16-bit apps
#ifndef WIN32
 FARPROC        fpErrorCallback;
 FARPROC        fpStatusCallback;
 FARPROC        fpFrameCallback;
 FARPROC        fpVideoCallback;
#endif

// Function prototypes
//
LONG FAR PASCAL MainWndProc(HWND, UINT, UINT, LONG) ;
LRESULT FNWCALLBACK ErrorCallbackProc(HWND, int, LPTSTR) ;
LRESULT FNWCALLBACK StatusCallbackProc(HWND, int, LPTSTR) ;
LRESULT FNWCALLBACK FrameCallbackProc(HWND, LPVIDEOHDR) ;
LRESULT FNWCALLBACK VideoCallbackProc(HWND, LPVIDEOHDR) ;

//
// WinMain: Application Entry Point Function
//
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
///////////////////////////////////////////////////////////////////////
//  hInstance:      handle for this instance
//  hPrevInstance:  handle for possible previous instances
//  lpszCmdLine:    long pointer to exec command line
//  nCmdShow:       Show code for main window display
///////////////////////////////////////////////////////////////////////

    MSG          msg ;
    WNDCLASS     wc ;

    ghInstApp = hInstance ;
    if (! hPrevInstance) {
        // If it's the first instance, register the window class
        wc.lpszClassName = gachAppName ;
        wc.hInstance     = hInstance ;
        wc.lpfnWndProc   = MainWndProc ;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
        wc.hIcon         = LoadIcon(hInstance, gachIconName) ;
        wc.lpszMenuName  = gachMenuName ;
        wc.hbrBackground = GetStockObject(WHITE_BRUSH) ;
        wc.style         = CS_HREDRAW | CS_VREDRAW ;
        wc.cbClsExtra    = 0 ;
        wc.cbWndExtra    = 0 ;

        if (! RegisterClass(&wc)) {
            LoadString(ghInstApp, IDS_ERR_REGISTER_CLASS, gachString, sizeof(gachString)/sizeof(TCHAR)) ;
            MessageBox(NULL, gachString, NULL,
#ifdef BIDI
                MB_RTL_READING |
#endif

            MB_ICONEXCLAMATION) ;
            return 0 ;
        }
    }

    // Create Application's Main window
    ghWndMain =
#ifdef BIDI

//
// unfortunately you can't just #ifdef the CreateWindow line and leave
// the parameters common: on Win32, CreateWindow is a macro and does not
// expand correctly if you ifdef only the 'CreateWindow(' line.
//
        CreateWindowEx(WS_EX_BIDI_SCROLL |  WS_EX_BIDI_MENU |WS_EX_BIDI_NOICON,
                             gachAppName,
                             TEXT("Capture Test App"),
                             WS_CAPTION      |
                             WS_SYSMENU      |
                             WS_MINIMIZEBOX  |
                             WS_MAXIMIZEBOX  |
                             WS_THICKFRAME   |
                             WS_CLIPCHILDREN |
                             WS_OVERLAPPED,
                             CW_USEDEFAULT, 0,
                             320, 240,
                             NULL,
                             NULL,
                             ghInstApp,
                             NULL) ;
#else
        CreateWindow (
                             gachAppName,
                             TEXT("Capture Test App"),
                             WS_CAPTION      |
                             WS_SYSMENU      |
                             WS_MINIMIZEBOX  |
                             WS_MAXIMIZEBOX  |
                             WS_THICKFRAME   |
                             WS_CLIPCHILDREN |
                             WS_OVERLAPPED,
                             CW_USEDEFAULT, 0,
                             320, 240,
                             NULL,
                             NULL,
                             ghInstApp,
                             NULL) ;
#endif

    if (ghWndMain == NULL) {
        LoadString(ghInstApp, IDS_ERR_CREATE_WINDOW, gachString, sizeof(gachString)/sizeof(TCHAR)) ;
        MessageBox(NULL, gachString, NULL,
#ifdef BIDI
                MB_RTL_READING |
#endif

        MB_ICONEXCLAMATION | MB_OK) ;
        return IDS_ERR_CREATE_WINDOW ;
    }

    ShowWindow(ghWndMain, nCmdShow) ;
    UpdateWindow(ghWndMain) ;
    ghAccel = LoadAccelerators(ghInstApp, gachAppName) ;

    // All set; get and process messages
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (! TranslateAccelerator(ghWndMain, ghAccel, &msg)) {
            TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }
    }

    return msg.wParam ;
}  // End of WinMain


//
// ErrorCallbackProc: Error Callback Function
//
LRESULT FNWCALLBACK ErrorCallbackProc(HWND hWnd, int nErrID, LPTSTR lpErrorText)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:          Application main window handle
//  nErrID:        Error code for the encountered error
//  lpErrorText:   Error text string for the encountered error
////////////////////////////////////////////////////////////////////////

    if (!ghWndMain)
        return FALSE;

    if (nErrID == 0)            // Starting a new major function
        return TRUE;            // Clear out old errors...

    // Show the error ID and text
    wsprintf(gachBuffer, TEXT("Error# %d"), nErrID) ;

    MessageBox(hWnd, lpErrorText, gachBuffer,
#ifdef BIDI
                MB_RTL_READING |
#endif
                MB_OK | MB_ICONEXCLAMATION) ;

    return (LRESULT) TRUE ;
}


//
// StatusCallbackProc: Status Callback Function
//
LRESULT FNWCALLBACK StatusCallbackProc(HWND hWnd, int nID, LPTSTR lpStatusText)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:           Application main window handle
//  nID:            Status code for the current status
//  lpStatusText:   Status text string for the current status
////////////////////////////////////////////////////////////////////////

    if (!ghWndMain)
        return FALSE;

    if (nID == 0) {              // Zero means clear old status messages
        SetWindowText(ghWndMain, (LPTSTR) gachAppName) ;
        return (LRESULT) TRUE ;
    }

    // Show the status ID and status text...
    wsprintf(gachBuffer, TEXT("Status# %d: %s"), nID, lpStatusText) ;

        SetWindowText(ghWndMain, (LPTSTR)gachBuffer) ;

    return (LRESULT) TRUE ;
}


//
// FrameCallbackProc: Frame Callback Function
// Called whenever a new frame is captured but not streaming
//
LRESULT FNWCALLBACK FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:       Application main window handle
//  lpVHdr:     long pointer to VideoHdr struct containing captured
//              frame information
////////////////////////////////////////////////////////////////////////

    if (!ghWndMain)
        return FALSE;

    wsprintf(gachBuffer, TEXT("Preview frame# %ld "), gdwFrameNum++) ;

    SetWindowText(ghWndMain, (LPTSTR)gachBuffer) ;
    return (LRESULT) TRUE ;
}


//
// VideoCallbackProc: Video Stream Callback Function
// Called whenever a new frame is captured while streaming
//
LRESULT FNWCALLBACK VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:       Application main window handle
//  lpVHdr:     long pointer to VideoHdr struct containing captured
//              frame information
////////////////////////////////////////////////////////////////////////

    gdwVideoNum++;      // Testing:  just count the callbacks

    return (LRESULT) TRUE ;
}


//
// CenterCaptureWindow: Placess Capture Window at the Center of Main Window
//
static void CenterCaptureWindow(HWND hWndM, HWND hWndC)
{
////////////////////////////////////////////////////////////////////////
//  hWndM:      Application main window handle
//  hWndC:      Capture window handle
////////////////////////////////////////////////////////////////////////

    RECT       MainRect ;
    RECT       CapRect ;
    WORD       wCapXPos ;
    WORD       wCapYPos ;

    // Get the sizes of main and capture windows and
    // calculate the location for centering
    GetClientRect(hWndM, &MainRect) ;
    GetClientRect(hWndC, &CapRect) ;
    wCapXPos = max(0, (Width(MainRect) - Width(CapRect)) / 2) ;
    wCapYPos = max(0, (Height(MainRect) - Height(CapRect)) / 2) ;

    // Position the capture window at the required location
    MoveWindow(hWndC, wCapXPos, wCapYPos, Width(CapRect),
               Height(CapRect), TRUE) ;
}


//
// StartNewVideoChannel: Gets Selected Driver's Caps -- Updates menu,
//                       Checks Image Size -- Resizes display window,
//                       Enables Preview (at 15 FPS rate)
//
static void StartNewVideoChannel(HWND hWndM, HWND hWndC, WORD wIndex)
{
////////////////////////////////////////////////////////////////////////
//  hWndM:      Application main window handle
//  hWndC:      Capture window handle
//  wIndex:     Selected capture driver index
////////////////////////////////////////////////////////////////////////

    HMENU       hMenu = GetMenu(hWndM) ;

    // Get capture driver settings and update menu
    capDriverGetCaps(hWndC, &gCapDriverCaps, sizeof(CAPDRIVERCAPS)) ;
    EnableMenuItem(hMenu, IDM_O_OVERLAY, MF_BYCOMMAND |
                gCapDriverCaps.fHasOverlay ? MF_ENABLED : MF_GRAYED) ;
    EnableMenuItem(hMenu, IDM_O_VIDEOFORMAT, MF_BYCOMMAND |
                gCapDriverCaps.fHasDlgVideoFormat ? MF_ENABLED : MF_GRAYED) ;
    EnableMenuItem(hMenu, IDM_O_VIDEOSOURCE, MF_BYCOMMAND |
                gCapDriverCaps.fHasDlgVideoSource ? MF_ENABLED : MF_GRAYED) ;
    EnableMenuItem(hMenu, IDM_O_VIDEODISPLAY, MF_BYCOMMAND |
                gCapDriverCaps.fHasDlgVideoDisplay ? MF_ENABLED : MF_GRAYED) ;

    // Get video format and adjust capture window
    capGetStatus(hWndC, &gCapStatus, sizeof(CAPSTATUS)) ;
    SetWindowPos(hWndC, NULL, 0, 0, gCapStatus.uiImageWidth,
                 gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;

    // Start preview by default
    capPreviewRate(hWndC, MS_FOR_15FPS) ;
    capPreview(hWndC, TRUE) ;

    // Put check mark beside appropriate menu options
    CheckMenuItem(hMenu, wIndex + IDM_O_DRIVERS, MF_BYCOMMAND | MF_CHECKED) ;
}


//
// MenuProc: Processes All Menu-based Operations
//
long FAR PASCAL MenuProc(HWND hWnd, UINT wParam, LONG lParam)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:      Application main window handle
//  hMenu:     Application menu handle
//  wParam:    Menu option
//  lParam:    Additional info for any menu option
////////////////////////////////////////////////////////////////////////

    OPENFILENAME ofn ;
    DWORD        dwError ;
    WORD         wIndex ;
    BOOL         fResult ;
    DWORD        dwSize ;
    TCHAR        achBuffer[_MAX_PATH] ;
    TCHAR        achFileName[_MAX_PATH] ;
    TCHAR        *szFileFilter = TEXT("Microsoft AVI\0")
                                 TEXT("*.avi\0")
                                 TEXT("All Files\0")
                                 TEXT("*.*\0") ;

    HMENU hMenu = GetMenu(hWnd) ;

    switch (wParam) {
        case IDM_F_SETCAPTUREFILE:
        {
            LPTSTR p;

            // Get current capture file name and
            // then try to get the new capture file name
            dwError = capFileGetCaptureFile(ghWndCap, achFileName,
                                        sizeof(achFileName)/sizeof(TCHAR));
            if (dwError) {

                // Get just the path info
                // Terminate the full path at the last backslash
                lstrcpy (achBuffer, achFileName);
                for (p = achBuffer + lstrlen(achBuffer); p > achBuffer; p--) {
                    if (*p == '\\') {
                        *(p+1) = '\0';
                        break;
                    }
                }

                _fmemset(&ofn, 0, sizeof(OPENFILENAME)) ;
                ofn.lStructSize = sizeof(OPENFILENAME) ;
                ofn.hwndOwner = hWnd ;
                ofn.lpstrFilter = szFileFilter ;
                ofn.nFilterIndex = 0 ;
                ofn.lpstrFile = achFileName ;
                ofn.nMaxFile = sizeof(achFileName)/sizeof(TCHAR) ;
                ofn.lpstrFileTitle = NULL;
                ofn.lpstrTitle = TEXT("Set Capture File") ;
                ofn.nMaxFileTitle = 0 ;
                ofn.lpstrInitialDir = achBuffer;
                ofn.Flags =
#ifdef BIDI
                OFN_BIDIDIALOG |
#endif
                OFN_HIDEREADONLY |
                OFN_NOREADONLYRETURN |
                OFN_PATHMUSTEXIST ;

                if (GetOpenFileName(&ofn))
                    // If the user has hit OK then set capture file name
                    capFileSetCaptureFile(ghWndCap, achFileName) ;
            }
        }
        break;

        case IDM_F_SAVEVIDEOAS:
            // Get the current capture file name and
            // then get the substitute file name to save video in
            dwError = capFileGetCaptureFile(ghWndCap, achFileName, sizeof(achFileName)/sizeof(TCHAR));
            if (dwError) {

                _fmemset(&ofn, 0, sizeof(OPENFILENAME)) ;
                ofn.lStructSize = sizeof(OPENFILENAME) ;
                ofn.hwndOwner = hWnd ;
                ofn.lpstrFilter = szFileFilter ;
                ofn.nFilterIndex = 0 ;
                ofn.lpstrFile = achFileName ;
                ofn.nMaxFile = sizeof(achFileName)/sizeof(TCHAR) ;
                ofn.lpstrFileTitle = NULL ;
                ofn.lpstrTitle = TEXT("Save Video As...") ;
                ofn.nMaxFileTitle = 0 ;
                ofn.lpstrInitialDir = NULL ;
                ofn.Flags =
#ifdef BIDI
                OFN_BIDIDIALOG |
#endif
                OFN_PATHMUSTEXIST ;

                if (GetSaveFileName(&ofn))
                    // If the user has hit OK then set save file name
                    capFileSaveAs(ghWndCap, achFileName) ;
            }
            break;

        case IDM_F_ALLOCATESPACE:
            if (DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_AllocCapFileSpace),
                          hWnd, AllocCapFileProc))
                // If user has hit OK then alloc requested capture file space
                if (! capFileAlloc(ghWndCap, (long) gwCapFileSize * ONEMEG))
                    MessageBox(NULL, TEXT("Can't pre-allocate capture file space"),
                               TEXT("Error"),
#ifdef BIDI
                                MB_RTL_READING |
#endif
                                MB_OK | MB_ICONEXCLAMATION) ;
            break ;

        case IDM_F_EXIT:
            DestroyWindow(hWnd) ;
            break;

        case IDM_E_COPY:
            capEditCopy(ghWndCap) ;
            break;

        case IDM_E_PASTEPALETTE:
            capPalettePaste(ghWndCap) ;
            break;

        case IDM_O_PREVIEW:
            // Toggle Preview
    	    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
            capPreview(ghWndCap, !gCapStatus.fLiveWindow) ;
            break;

        case IDM_O_OVERLAY:
            // Toggle Overlay
    	    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
            capOverlay(ghWndCap, !gCapStatus.fOverlayWindow) ;
            break ;

        case IDM_O_AUDIOFORMAT:
#ifdef  USE_ACM
            {
                ACMFORMATCHOOSE cfmt;

                // Ask the ACM what the largest wave format is.....
                acmMetrics(NULL,
                            ACM_METRIC_MAX_SIZE_FORMAT,
                            &dwSize);

                // Get the current audio format
                dwSize = max (dwSize, capGetAudioFormatSize (ghWndCap));
                glpwfex = (LPWAVEFORMATEX) GlobalAllocPtr(GHND, dwSize) ;
                capGetAudioFormat(ghWndCap, glpwfex, (WORD)dwSize) ;

                _fmemset (&cfmt, 0, sizeof (ACMFORMATCHOOSE));
                cfmt.cbStruct = sizeof (ACMFORMATCHOOSE);
                cfmt.fdwStyle =  ACMFORMATCHOOSE_STYLEF_INITTOWFXSTRUCT;
                cfmt.fdwEnum =   ACM_FORMATENUMF_HARDWARE |
                                 ACM_FORMATENUMF_INPUT;
                cfmt.hwndOwner = hWnd;
                cfmt.pwfx =     glpwfex;
                cfmt.cbwfx =    dwSize;
                if (!acmFormatChoose(&cfmt))
                    capSetAudioFormat(ghWndCap, glpwfex, (WORD)dwSize) ;

                GlobalFreePtr(glpwfex) ;
            }
#else
            // If not using ACM, remove the reference in the link line
            // of makefile.

            // Get current audio format and then find required format
            dwSize = capGetAudioFormatSize (ghWndCap);
            glpwfex = (LPWAVEFORMATEX) GlobalAllocPtr(GHND, dwSize) ;
            capGetAudioFormat(ghWndCap, glpwfex, (WORD)dwSize) ;

            if (DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_AudioFormat), hWnd, AudioFormatProc))
                capSetAudioFormat(ghWndCap, glpwfex, (WORD)dwSize);  // If the user has hit OK, set the new audio format

            GlobalFreePtr(glpwfex) ;
#endif
            break ;

        case IDM_O_VIDEOFORMAT:
            if (gCapDriverCaps.fHasDlgVideoFormat) {
                // Only if the driver has a "Video Format" dialog box
                if (capDlgVideoFormat(ghWndCap)) {  // If successful,
                    // Get the new image dimension and center capture window
                    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
                    SetWindowPos(ghWndCap, NULL, 0, 0, gCapStatus.uiImageWidth,
                        gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ;
                    CenterCaptureWindow(hWnd, ghWndCap) ;
                }
            }
            break;

        case IDM_O_VIDEOSOURCE:
            if (gCapDriverCaps.fHasDlgVideoSource) {
                // Only if the driver has a "Video Source" dialog box
                capDlgVideoSource(ghWndCap) ;
            }
            break ;

        case IDM_O_VIDEODISPLAY:
            if (gCapDriverCaps.fHasDlgVideoDisplay) {
                // Only if the driver has a "Video Display" dialog box
                capDlgVideoDisplay(ghWndCap) ;
            }
            break ;

        case IDM_O_PALETTE:
            if (DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_MakePalette), hWnd, MakePaletteProc))
                // If the user has hit OK, capture palette with the
                // specified number of colors and frames
                capPaletteAuto(ghWndCap, gwPalFrames, gwPalColors) ;
            break;

        case IDM_C_CAPTUREVIDEO:
            gdwVideoNum = 0 ;  // Start counting video frames
            // Capture video sequence
            fResult = capCaptureSequence(ghWndCap) ;
            break;

        case IDM_C_CAPTUREFRAME:
            gdwFrameNum = 0 ;  // Start counting single frames
            // Turn off overlay / preview (gets turned off by frame capture)
	    capPreview(ghWndCap, FALSE);
	    capOverlay(ghWndCap, FALSE);

            // Grab a frame
            fResult = capGrabFrameNoStop(ghWndCap) ;
            break;

        case IDM_C_CAPTURESETTINGS:
            // Get the current setup for video capture
            capCaptureGetSetup(ghWndCap, &gCapParms, sizeof(CAPTUREPARMS)) ;

            // Invoke a Dlg box to setup all the params
            if (DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_CapSetUp), hWnd, CapSetUpProc))
                // If the user has hit OK, set the new setup info
                capCaptureSetSetup(ghWndCap, &gCapParms, sizeof(CAPTUREPARMS)) ;
            break;

        case IDM_O_CHOOSECOMPRESSOR:
            capDlgVideoCompression(ghWndCap);
            break;

        case IDM_H_ABOUT:
            DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_HelpAboutBox), hWnd, AboutProc) ;
            break ;

        default:
            // There is a chance, a driver change has been requested
            if ( IsDriverIndex(wParam) ) {
                // If it's a valid driver index...
                if (wParam - IDM_O_DRIVERS != gwDeviceIndex) {
                    // and a different one too then we need to do the rest

                    // Turn off preview/overlay, uncheck current driver option
                    capPreview(ghWndCap, FALSE) ;
                    capOverlay(ghWndCap, FALSE) ;
                    CheckMenuItem(GetMenu(hWnd), gwDeviceIndex + IDM_O_DRIVERS,
                                  MF_BYCOMMAND | MF_UNCHECKED) ;

                    // Connect to requested driver
                    if ( capDriverConnect(ghWndCap, (wIndex = (WORD) (wParam - IDM_O_DRIVERS))) ) {
                        // Connect worked fine -- update menu, start new driver...
                        CheckMenuItem(GetMenu(hWnd), wParam, MF_BYCOMMAND | MF_CHECKED) ;
                        gwDeviceIndex = (WORD) (wParam - IDM_O_DRIVERS) ;
                        StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
                        CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                    else {
                        // if connect failed, re-connect back to previous driver
                        if (! capDriverConnect(ghWndCap, gwDeviceIndex)) {
                            MessageBox(hWnd, TEXT("Now can't connect back to previous driver !!"),
                                       TEXT("Error"),
#ifdef BIDI
                            MB_RTL_READING |
#endif

                            MB_OK | MB_ICONSTOP) ;
                            return -1L ;
                        }
                        else
                            // Re-start previous driver as it was before
                            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;
                            CenterCaptureWindow(hWnd, ghWndCap) ;
                    }
                }   // end of if ( != gwDeviceIndex)
            }   // end of if (IsDriverIndex())
            else {
                wsprintf(achBuffer, TEXT("How could you specify this (%u) Driver Index ?"),
                         wParam - IDM_O_DRIVERS) ;
                MessageBox(hWnd, achBuffer, TEXT("Oops!!"),
#ifdef BIDI
                MB_RTL_READING |
#endif
                MB_OK | MB_ICONEXCLAMATION) ;
            }

            break ;
    }

    return 0L ;
}


//
// MainWndProc: Application Main Window Procedure
//
LONG FAR PASCAL MainWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam)
{
////////////////////////////////////////////////////////////////////////
//  hWnd:      Application main window handle
//  Message:   Next message to be processed
//  wParam:    WORD param for the message
//  lParam:    LONG param for the message
////////////////////////////////////////////////////////////////////////

    switch (Message) {
        case WM_COMMAND:
            MenuProc(hWnd, wParam, lParam) ;
            break ;

        case WM_CREATE:
        {
            TCHAR    achDeviceName[80] ;
            TCHAR    achDeviceVersion[100] ;
            TCHAR    achBuffer[100] ;
            WORD    wDriverCount = 0 ;
            WORD    wIndex ;
            DWORD   dwError ;
            HMENU   hMenu ;

            // First create the capture window
            ghWndCap = capCreateCaptureWindow((LPTSTR)TEXT("Capture Window"),
                                              WS_CHILD | WS_VISIBLE,
                                              0, 0, 160, 120,
                                              (HWND) hWnd, (int) 0) ;

            hMenu = GetSubMenu(GetMenu(hWnd), 2) ;  // 2 for "Option"

#if ENABLE_ERROR_CALLBACK
  #ifdef WIN32
            // Register the status and error callbacks before driver connect
            capSetCallbackOnError(ghWndCap, ErrorCallbackProc) ;
  #else
            fpErrorCallback = MakeProcInstance((FARPROC)ErrorCallbackProc, ghInstApp) ;
            capSetCallbackOnError(ghWndCap, fpErrorCallback) ;
  #endif
#endif

#if ENABLE_STATUS_CALLBACK
  #ifdef WIN32
            capSetCallbackOnStatus(ghWndCap, StatusCallbackProc) ;
  #else
            fpStatusCallback = MakeProcInstance((FARPROC)StatusCallbackProc, ghInstApp) ;
            capSetCallbackOnStatus(ghWndCap, fpStatusCallback) ;
  #endif
#endif

#if ENABLE_VIDEOFRAME_CALLBACKS
  #ifdef WIN32
            capSetCallbackOnVideoStream(ghWndCap, VideoCallbackProc) ;
            capSetCallbackOnFrame(ghWndCap, FrameCallbackProc) ;
  #else
            fpVideoCallback = MakeProcInstance((FARPROC)VideoCallbackProc, ghInstApp) ;
            capSetCallbackOnVideoStream(ghWndCap, fpVideoCallback) ;

            fpFrameCallback = MakeProcInstance((FARPROC)FrameCallbackProc, ghInstApp) ;
            capSetCallbackOnFrame(ghWndCap, fpFrameCallback) ;
  #endif
#endif
            // Try to connect one of the MSVIDEO drivers
            for (wIndex = 0 ; wIndex < MAXVIDDRIVERS ; wIndex++) {
                if (capGetDriverDescription(wIndex,
                           (LPTSTR)achDeviceName, sizeof(achDeviceName)/ sizeof(TCHAR),
                           (LPTSTR)achDeviceVersion, sizeof(achDeviceVersion)/sizeof(TCHAR))) {

                    // There is such a driver in the "system.ini" file.
                    // Append driver name to "Options" list in menu
                    wsprintf(achBuffer, TEXT("&%d %s"), wIndex, (LPTSTR)achDeviceName) ;
                    AppendMenu(hMenu, MF_ENABLED, IDM_O_DRIVERS+wIndex, achBuffer) ;

                    if (wDriverCount++ == 0) {
                        // Only if no other driver is already connected
                        dwError = capDriverConnect(ghWndCap, wIndex);
                        if (dwError) {
                            CheckMenuItem(GetMenu(hWnd), IDM_O_DRIVERS+wIndex, MF_BYCOMMAND | MF_CHECKED) ;
                            gwDeviceIndex = wIndex ;
                        }
                    }
                } // end of if (capGetDriverDesc..())
            }

            // Now refresh menu, position capture window, start driver etc
            DrawMenuBar(hWnd) ;
            CenterCaptureWindow(hWnd, ghWndCap) ;
            StartNewVideoChannel(hWnd, ghWndCap, gwDeviceIndex) ;

            break ;
        }

        case WM_MOVE:
        case WM_SIZE:
            CenterCaptureWindow(hWnd, ghWndCap) ;
            break ;

        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
            // Pass the buck to Capture window proc
            PostMessage(ghWndCap, Message, wParam, lParam) ;
            break ;

        case WM_INITMENU:
        {
            BOOL          fResult ;

            // Initially check if "Options.PastePalette" should be enabled
            fResult = IsClipboardFormatAvailable(CF_PALETTE) ?
                      MF_ENABLED : MF_GRAYED ;
            EnableMenuItem((HMENU) wParam, IDM_E_PASTEPALETTE, fResult) ;

	    // Check/Uncheck Preview and Overlay
    	    capGetStatus(ghWndCap, &gCapStatus, sizeof(CAPSTATUS)) ;
    	    CheckMenuItem((HMENU)wParam, IDM_O_PREVIEW, gCapStatus.fLiveWindow
						? MF_CHECKED : MF_UNCHECKED);
    	    CheckMenuItem((HMENU)wParam, IDM_O_OVERLAY,gCapStatus.fOverlayWindow
						? MF_CHECKED : MF_UNCHECKED);
        }

        case WM_PAINT:
        {
            HDC           hDC ;
            PAINTSTRUCT   ps ;

            hDC = BeginPaint(hWnd, &ps) ;

            // Included in case the background is not a pure color
            SetBkMode(hDC, TRANSPARENT) ;

            EndPaint(hWnd, &ps) ;
            break ;
        }

        case WM_CLOSE:
            // Disable and free all the callbacks
#if ENABLE_ERROR_CALLBACK
            capSetCallbackOnError(ghWndCap, NULL) ;
  #ifndef WIN32
            FreeProcInstance(fpErrorCallback) ;
  #endif
#endif

#if ENABLE_STATUS_CALLBACK
            capSetCallbackOnStatus(ghWndCap, NULL) ;
  #ifndef WIN32
            FreeProcInstance(fpStatusCallback) ;
  #endif
#endif

#if ENABLE_VIDEOFRAME_CALLBACKS
            capSetCallbackOnFrame(ghWndCap, NULL) ;
            capSetCallbackOnVideoStream(ghWndCap, NULL) ;
  #ifndef WIN32
            FreeProcInstance(fpFrameCallback) ;
            FreeProcInstance(fpVideoCallback) ;
  #endif
#endif
            // Destroy child windows, modeless dialogs, then this window...

            DestroyWindow(ghWndCap) ;
            DestroyWindow(hWnd) ;
            break ;

        case WM_DESTROY:
            PostQuitMessage(0) ;
            break ;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam) ;
    }

    return 0L;
}   // End of MainWndProc

