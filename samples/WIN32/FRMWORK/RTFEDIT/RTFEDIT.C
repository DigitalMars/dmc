// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   generic.c
//
//  PURPOSE:   Implement the windows procedure for the main application
//    windows.  Also implement the generic message and command dispatchers.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory 
//                   window on the main window.
//
//  COMMENTS:
//

#define CHICAGO                 // for richedit.h  REVIEW: is this necessary?

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <richedit.h>           // For Richedit control
#include <commdlg.h>            // For Richedit control
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

HWND        hRTFWnd;
HANDLE      hRTFLib;

HWND RTFEDIT_Init(HWND hWndParent); // Function to create RTF Edit control
void RTFEDIT_Shutdown(void);        // Function to housekeep for RTF Edit

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_CREATE,  MsgCreate},
    {WM_DESTROY, MsgDestroy},
    {WM_SIZE,    MsgSize}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_EXIT,      CmdExit},
    {IDM_ABOUT,     CmdAbout},
    {IDM_BOLD,      RTFEDIT_CmdBold},
    {IDM_ITALIC,    RTFEDIT_CmdItalic},
    {IDM_UNDERLINE, RTFEDIT_CmdUnderline},
    {IDM_FONTS,     RTFEDIT_CmdFonts}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls the RTFEDIT_Init function
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - WM_CREATE
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Call the RTFEDIT_Init function, which will load the RTF
    // DLL and create the RTF Edit control window.
    hRTFWnd = RTFEDIT_Init(hwnd);
    SetFocus (hRTFWnd);
    return 0;
}

//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RTFEDIT_Shutdown(); // Destroy the RTF Edit control and free the library
    PostQuitMessage(0);
    return 0;
}

//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Resizes the RTF Edit control
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - WM_SIZE
//    wparam    - How the window was sized
//    lparam    - MAKELONG (dx,dy)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Elementary resizing of children to fit in the parent code.
    if (hRTFWnd) 
    {
        MoveWindow(hRTFWnd, 0, 0, LOWORD(lparam), HIWORD(lparam), TRUE );
    }
    return 0;
}

//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}

//
//  FUNCTION: RTFEDIT_Init(HWND)
//
//  PURPOSE: Register the RTF Edit Class and create the RTF Edit Control Window
//
//  PARAMETERS:
//    hwnd      - Window handle to use for RTF's Parent
//
//  RETURN VALUE:
//
//    On Success: HWND of RTF Edit Control
//    On Failure: NULL
//
//  COMMENTS:
//    This function calls LoadLibrary to load the RTF Edit control
//    DLL (RICHED32.DLL). Make sure to call FreeLibrary (this
//    sample program does it in RTFEDIT_ShutDown).
//
//    A global variable is changed in this function:
//
//      hRTFLib - The handle to the RTF Edit Control DLL
//    

HWND RTFEDIT_Init(HWND hWndParent)
{
    HWND       hRTF;

    if (!hRTFLib)
      hRTFLib = LoadLibrary("RICHED32.DLL");

    if (!hRTFLib)
    {
        char szErr[255];
        wsprintf(szErr, "LoadLibrary Failed. Error Code %ld", GetLastError());
        MessageBox(hWndParent, szErr, szAppName, MB_OK );
        return (hRTFLib = NULL);
    }

    hRTF = CreateWindow("RICHEDIT",          // Class Name of RTF Edit
                        "",                  // Text of edit control
                        WS_CHILD       | 
                            WS_VISIBLE | 
                            WS_BORDER  | 
                            ES_MULTILINE,    // Window style
                        0, 0,                // Initially create 0 size,
                        0, 0,                // Main Wnd's WM_SIZE handler will resize
                        hWndParent,          // Use main parent
                        (HMENU)0,            // ID of zero (we don't care)
                        hInst,               // This app instance owns this window
                        NULL                 // Don't need data in WM_CREATE
    );

    return hRTF;
}

//
//  FUNCTION: RTFEDIT_Shutdown()
//
//  PURPOSE: Destroy the RTF Edit Control and free the RTF DLL.
//
//  PARAMETERS:
//    None
//
//  RETURN VALUE:
//
//    none
//
//  COMMENTS:
//    This function calls FreeLibrary to free the RTF Edit control
//    DLL (RICHED32.DLL). If your application plans on calling this
//    function more than once (in case you have multiple RTF Edit
//    controls), be sure to only call FreeLibrary on the last time.
//    A load count would work fine.
//
//    Two global variables are changed in this function:
//
//      hRTFWnd - The handle to the RTF Edit control in this sample
//      hRTFLib - The handle to the RTF Edit Control DLL
//    

void RTFEDIT_Shutdown(void)
{
    if (hRTFWnd) 
    {
        DestroyWindow(hRTFWnd);
        hRTFWnd = NULL;
    }

    if (hRTFLib) 
    {
        FreeLibrary(hRTFLib);
        hRTFLib = NULL;
    }
}

//
//  FUNCTION: RTFEDIT_CmdBold(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Apply/Remove Bold from text selection
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_BOLD (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    This function uses the EM_GETCHARFORMAT and EM_SETCHARFORMAT
//    messages to work with the RTF Edit Control
//    

LRESULT RTFEDIT_CmdBold(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    // Version control this structure
    cf.cbSize = sizeof(cf);

    // Set the mask to ask the RTF Edit control for the current
    // bold status of the selection (which will be placed
    // in the dwEffects field).
    cf.dwMask      = CFM_BOLD;

    // Get the bold status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Set the mask to tell the RTF Edit control to pay attention to
    // the bold bit of the dwEffects field.
    cf.dwMask      = CFM_BOLD;
    // Flip da bits
    cf.dwEffects  ^= CFE_BOLD;

    // Set the new bold status to the selected text
    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    return 0;
}

//
//  FUNCTION: RTFEDIT_CmdIatlic(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Apply/Remove Italic from text selection
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ITALIC (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    This function uses the EM_GETCHARFORMAT and EM_SETCHARFORMAT
//    messages to work with the RTF Edit Control
//

LRESULT RTFEDIT_CmdItalic(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    // Version control this structure
    cf.cbSize = sizeof(cf);

    // Set the mask to ask the RTF Edit control for the current
    // italic status of the selection (which will be placed
    // in the dwEffects field).
    cf.dwMask      = CFM_ITALIC;

    // Get the italic status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Set the mask to tell the RTF Edit control to pay attention to
    // the italic bit of the dwEffects field.
    cf.dwMask      = CFM_ITALIC;
    // 'dem bits need a twiddlin'!
    cf.dwEffects  ^= CFE_ITALIC;

    // Set the new underline status to the selected text
    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    return 0;
}

//
//  FUNCTION: RTFEDIT_CmdUnderline (HWND, WORD, WORD, HWND)
//
//  PURPOSE: Apply/Remove Underline from text selection
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_UNDERLINE (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    This function uses the EM_GETCHARFORMAT and EM_SETCHARFORMAT
//    messages to work with the RTF Edit Control/
//    

LRESULT RTFEDIT_CmdUnderline(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    // Version control this structure
    cf.cbSize = sizeof(cf);

    // Set the mask to ask the RTF Edit control for the current
    // underline status of the selection (which will be placed
    // in the dwEffects field).
    cf.dwMask      = CFM_UNDERLINE;


    // Get the underline status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Set the mask to tell the RTF Edit control to pay attention to
    // the underline bit of the dwEffects field.
    cf.dwMask      = CFM_UNDERLINE;
    // Twiddle de bits, captain!
    cf.dwEffects  ^= CFE_UNDERLINE;

    // Set the new underline status to the selected text
    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    return 0;
}

//
//  FUNCTION: RTFEDIT_CmdFonts (HWND, WORD, WORD, HWND)
//
//  PURPOSE: Allow use to change font of selected text
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_FONTS (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    This function uses the EM_GETCHARFORMAT and EM_SETCHARFORMAT
//    messages to work with the RTF Edit Control
//    

LRESULT RTFEDIT_CmdFonts(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHARFORMAT cf;                // This is defined in RICHEDIT.H
    CHOOSEFONT ChooseFontStruct;  // Common Dialog fun
    LOGFONT    lf;                // Log font information
    HDC        hDC;

    // Version control this structure
    cf.cbSize = sizeof(cf);

    // Get the font formatting status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected text, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Fill in the font info for the font common fialog

    memset(&ChooseFontStruct, 0, sizeof(CHOOSEFONT));
    memset(&lf, 0, sizeof(LOGFONT));

    hDC = GetDC(hwnd); // Need a screen DC, use the parent's DC

    // The RTF Edit control measures in twips. Each point is
    // 20 twips.

    lf.lfHeight = cf.yHeight/-20;

    // Set up the rest of the logfont structure according to the 
    // information retrieved from the EM_GETCHARFORMAT message

    if (cf.dwEffects & CFE_BOLD)
        lf.lfWeight = FW_BOLD;
    else
        lf.lfWeight = FW_NORMAL;
    lf.lfItalic = (BOOL)(cf.dwEffects & CFE_ITALIC);
    lf.lfUnderline = (BOOL)(cf.dwEffects & CFE_UNDERLINE);
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = cf.bPitchAndFamily;
    lstrcpy(lf.lfFaceName, cf.szFaceName);

    // Fire up the common dialog.

    ChooseFontStruct.lStructSize = sizeof(ChooseFontStruct);
    ChooseFontStruct.hwndOwner = hwnd;
    ChooseFontStruct.hDC = hDC;
    ChooseFontStruct.lpLogFont = &lf;
    ChooseFontStruct.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
    ChooseFontStruct.rgbColors = RGB(0,0,0);
    ChooseFontStruct.nFontType = SCREEN_FONTTYPE;

    if (ChooseFont(&ChooseFontStruct))
    {
        // Set the mask to tell the RTF Edit control to pay attention to
        // the font formatting bits.
        cf.dwMask = CFM_BOLD | CFM_FACE | CFM_ITALIC | 
                    CFM_OFFSET | CFM_SIZE | CFM_UNDERLINE;

        //  Undo the equation from above.

        cf.yHeight = lf.lfHeight * -20;

        // Fill in the rest of the characater formatting
         
        cf.dwEffects = 0;
        if (FW_BOLD == lf.lfWeight)
            cf.dwEffects |= CFE_BOLD;
        if (lf.lfItalic)
            cf.dwEffects |= CFE_ITALIC;

        if (lf.lfUnderline)
            cf.dwEffects |= CFE_UNDERLINE;

        cf.bPitchAndFamily = lf.lfPitchAndFamily;

        lstrcpy(cf.szFaceName, lf.lfFaceName);

        // Set the new formatting to the selected text
        SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    // Don't forget this!
    ReleaseDC(hwnd, hDC);

    return 0L;
}


