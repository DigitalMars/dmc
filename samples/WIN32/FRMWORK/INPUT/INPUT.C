// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: input.c
//
//  PURPOSE: Show windows input: mouse, keyboard, control(scroll), and timer
//
//  FUNCTIONS:
//    WndProc - Processes messages for the main window.
//    MsgCommand - Handle the WM_COMMAND messages for the main window.
//    MsgCreate - Set the timer for five-second intervals.
//    MsgDestroy - Kills the timer and posts the quit message.
//    MsgMouseMove - Display mouse move message and its parameters.
//    MsgLButtonDown -
//      Display left mouse button down message and its parameters.
//    MsgLButtonUp - Display left mouse button up message and its parameters.
//    MsgLButtonDoubleClick -
//      Display left mouse button double click message and its parameters.
//    MsgRButtonDown -
//      Display right mouse button down message and its parameters.
//    MsgRButtonUp - Display right mouse button up message and its parameters.
//    MsgRButtonDoubleClick -
//      Display right mouse button double click message and its parameters.
//    MsgKeyDown - Display key down message and its parameters.
//    MsgKeyUp - Display key up message and its parameters.
//    MsgChar - Display character recieved message and its parameters.
//    MsgTimer - Display timer message and a current time.
//    MsgScroll - Display scrollbar events and position.
//    MsgPaint - Draw the strings for current messages.
//    InitInput - Set up the rectangles for dispay of each type of message.
//
//   COMMENTS:
//    Message dispatch table -
//      For every message to be handled by the main window procedure
//      place the message number and handler function pointer in
//      rgmsd (the message dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Globals.h Contains the definitions of the structures and dispatch.c
//      contains the functions that use these structures.
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

// Global variables
static char MouseText[48];         //  mouse state
static char ButtonText[48];        //  mouse-button state
static char KeyboardText[48];      //  keyboard state
static char CharacterText[48];     //  latest character
static char ScrollText[48];        //  scroll status
static char TimerText[48];         //  timer state
static RECT rectMouse;
static RECT rectButton;
static RECT rectKeyboard;
static RECT rectCharacter;
static RECT rectScroll;
static RECT rectTimer;
static UINT idTimer;                //  timer ID
static int  nTimerCount = 0;        //  current timer count

#define TIMERID ((UINT) 't')


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND,        MsgCommand},
    {WM_CREATE,         MsgCreate},
    {WM_DESTROY,        MsgDestroy},
    {WM_MOUSEMOVE,      MsgMouseMove},
    {WM_LBUTTONDOWN,    MsgLButtonDown},
    {WM_LBUTTONUP,      MsgLButtonUp},
    {WM_LBUTTONDBLCLK,  MsgLButtonDoubleClick},
    {WM_RBUTTONDOWN,    MsgRButtonDown},
    {WM_RBUTTONUP,      MsgRButtonUp},
    {WM_RBUTTONDBLCLK,  MsgRButtonDoubleClick},
    {WM_KEYDOWN,        MsgKeyDown},
    {WM_KEYUP,          MsgKeyUp},
    {WM_CHAR,           MsgChar},
    {WM_TIMER,          MsgTimer},
    {WM_HSCROLL,        MsgScroll},
    {WM_VSCROLL,        MsgScroll},
    {WM_PAINT,          MsgPaint}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
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

LRESULT CALLBACK WndProc
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages
//
//  PARAMETERS:
//    hwnd - The window handle
//    uMessage - WM_COMMAND (unused)
//    GET_WM_COMMAND_ID(wparam,lparam) - The command number
//
//  RETURN VALUE:
//    Depends on the command.
//
//  COMMENTS:
//
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LRESULT lRet = 0;

    // Message packing of wparam and lparam have changed for Win32,
    // so use the GET_WM_COMMAND macro to unpack the commnad

    switch (GET_WM_COMMAND_ID(wparam,lparam))
    {
        //
        // **TODO** Add cases here for application specific command messages.
        //

        case IDM_EXIT:
            DestroyWindow (hwnd);
            break;

        default:
            lRet = DefWindowProc(hwnd, uMessage, wparam, lparam);
    }

    return lRet;
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Set the timer for five-second intervals
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_CREATE      (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Set the timer for five-second intervals
    idTimer =  SetTimer(hwnd, TIMERID, 5000, NULL);

    return 0;
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Kills the timer and posts the quit message.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_DESTROY (Unused)
//    wparam    - Extra data (Unused)
//    lparam    - Extra data (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    KillTimer(hwnd, idTimer);       // Stops the timer
    PostQuitMessage(0);

    return 0;
}


//
//  FUNCTION: MsgMouseMove(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display mouse move message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_MOUSEMOVE (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        MouseText,
        "WM_MOUSEMOVE: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectMouse, TRUE);

    return 0;
}


//
//  FUNCTION: MsgLButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONDOWN (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgLButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_LBUTTONDOWN: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgLButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONUP (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgLButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_LBUTTONUP: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}

//
//  FUNCTION: MsgLButtonDoubleClick(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button double click message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONDOBLECLICK (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgLButtonDoubleClick
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_LBUTTONDBLCLK: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgRButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONDOWN (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgRButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_RBUTTONDOWN: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgRButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONUP (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgRButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_RBUTTONUP: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgRButtonDoubleClick(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button double click message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONDOUBLECLICK (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgRButtonDoubleClick
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        ButtonText,
        "WM_RBUTTONDBLCLK: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgKeyDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display key down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_KEYDOWN (Unused)
//    wparam    - Virtual Key Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgKeyDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        KeyboardText,
        "WM_KEYDOWN: %x, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectKeyboard, TRUE);

    return 0;
}


//
//  FUNCTION: MsgKeyUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display key up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_KEYUP (Unused)
//    wparam    - Virtual Key Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgKeyUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        KeyboardText,
        "WM_KEYUP: %x, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectKeyboard, TRUE);

    return 0;
}


//
//  FUNCTION: MsgChar(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display character recieved message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_CHAR (Unused)
//    wparam    - Character Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgChar(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        CharacterText,
        "WM_CHAR: %c, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
    InvalidateRect(hwnd, &rectCharacter, TRUE);

    return 0;
}


//
//  FUNCTION: MsgTimer(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display timer message and a current time.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_TIMER (Unused)
//    wparam    - The timer identifier
//    lparam    - NULL           (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgTimer(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if ( wparam == TIMERID ) {
        wsprintf(
            TimerText,
            "WM_TIMER: %d seconds",
            nTimerCount += 5
        );
        InvalidateRect(hwnd, &rectTimer, TRUE);
    }

    return 0;
}


//
//  FUNCTION: MsgScroll(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display scrollbar events and current position.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_VSCROLL or WM_HSCROLL
//    GET_WM_HSCROLL_CODE(wparam,lparam) - Type of scroll
//    GET_WM_HSCROLL_POS(wparam, lparam) - Position of scroll
//          Only valid for SB_THUMBPOSTION and SB_THUMBTRACK
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgScroll(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    char *szDirection;
    char *szType;
    int  fnBar = (uMessage == WM_HSCROLL) ? SB_HORZ : SB_VERT;
    int  nPos  = GetScrollPos(hwnd, fnBar);

    szDirection = (uMessage == WM_HSCROLL) ? "WM_HSCROLL" : "WM_VSCROLL";

    switch(GET_WM_HSCROLL_CODE(wparam,lparam))
    {
        case SB_LINEUP:
            szType = "SB_LINEUP";
            nPos = max(0, nPos-1);
            break;
        case SB_LINEDOWN:
            szType = "SB_LINEDOWN";
            nPos = min(100, nPos+1);
            break;
        case SB_PAGEUP:
            szType = "SB_PAGEUP";
            nPos = max(0, nPos-10);
            break;
        case SB_PAGEDOWN:
            szType = "SB_PAGEDOWN";
            nPos = min(100, nPos+10);
            break;
        case SB_THUMBPOSITION:
            szType = "SB_THUMBPOSITION";
            nPos = GET_WM_HSCROLL_POS(wparam, lparam);
            break;
        case SB_THUMBTRACK:
            szType = "SB_THUMBTRACK";
            nPos = GET_WM_HSCROLL_POS(wparam, lparam);
            break;
        case SB_ENDSCROLL:
            szType = "SB_ENDSCROLL";
            break;
        default:
            szType = "unknown";
            break;
    }

    wsprintf(
         ScrollText, "%s: %s, %x",
        (LPSTR)szDirection, (LPSTR)szType, nPos
    );
    InvalidateRect(hwnd, &rectScroll, TRUE);

    if (GET_WM_HSCROLL_CODE(wparam,lparam) != SB_THUMBTRACK)
        SetScrollPos(hwnd, fnBar, nPos, TRUE);

    return 0;
}


//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Draw the strings for current messages.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_PAINT (Unused)
//    wparam    - Extra data (Unused)
//    lparam    - Extra data (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (IntersectRect(&rect, &rectMouse, &ps.rcPaint))
        TextOut(
            hdc,
            rectMouse.left, rectMouse.top,
            MouseText, strlen(MouseText)
        );
    if (IntersectRect(&rect, &rectButton, &ps.rcPaint))
        TextOut(
            hdc,
            rectButton.left, rectButton.top,
            ButtonText, strlen(ButtonText)
        );
    if (IntersectRect(&rect, &rectKeyboard, &ps.rcPaint))
        TextOut(
            hdc,
            rectKeyboard.left, rectKeyboard.top,
            KeyboardText, strlen(KeyboardText)
        );
    if (IntersectRect(&rect, &rectCharacter, &ps.rcPaint))
        TextOut(
            hdc,
            rectCharacter.left, rectCharacter.top,
            CharacterText, strlen(CharacterText)
        );
    if (IntersectRect(&rect, &rectTimer, &ps.rcPaint))
        TextOut(
            hdc, rectTimer.left, rectTimer.top,
            TimerText, strlen(TimerText)
        );
    if (IntersectRect(&rect, &rectScroll, &ps.rcPaint))
        TextOut(
            hdc,
            rectScroll.left, rectScroll.top,
            ScrollText, strlen(ScrollText)
        );

    EndPaint(hwnd, &ps);

    return 0;
}

//
//  FUNCTION: InitInput(HWND)
//
//  PURPOSE: Set up the rectangles for dispay of each type of message
//
//  PARAMETERS:
//    hwnd   - Window handle
//
//  RETURN VALUE:
//    Always returns TRUE - Sucess
//
//  COMMENTS:
//
//

BOOL InitInput(HWND hwnd)
{
    HDC hdc;
    TEXTMETRIC tm;
    RECT rect;
    int  dyLine;

    // Get a handle to the display context for the window in order
    //  to compute line height, screen width, and pixel count for 1/4 inch

    hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    dyLine = tm.tmExternalLeading + tm.tmHeight;

    rect.left   = GetDeviceCaps(hdc, LOGPIXELSX) / 4;   // 1/4 inch
    rect.right  = GetDeviceCaps(hdc, HORZRES);
    rect.top    = GetDeviceCaps(hdc, LOGPIXELSY) / 4;   // 1/4 inch
    ReleaseDC(hwnd, hdc);

    // Compute rectangle for mouse move events
    rect.bottom = rect.top + dyLine;
    rectMouse   = rect;

    // Compute rectangle for mouse button events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectButton = rect;

    // Compute rectangle for keyboard events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectKeyboard = rect;

    // Compute rectangle for character events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectCharacter = rect;

    // Compute rectangle for scroll events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectScroll = rect;

    // Compute rectangle for timer events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectTimer = rect;

    return TRUE;
}
