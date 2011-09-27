// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: output.c
//
//  PURPOSE: Show basic text and graphics output
//   
//
//  FUNCTIONS:
//    WndProc    - Processes messages for the main window.
//    InitOutput - Create graphics objects used by DrawShapes
//    FreeOutput - Destroy graphics objects used by DrawShapes
//    DrawOutput - Excercise some basic text and shape drawing api
//    UseTextOut - Use the TextOut function to display text
//    UseDrawText- Use the DrawText function to display text
//    DrawShapes - Use several GDI functions to draw some shapes
//
//   COMMENTS:
//      

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"


int  UseTextOut(HDC,POINT);
int  UseDrawText(HDC,POINT);
void DrawShapes(HDC,POINT);

static HPEN hpenDash;           // "---" pen handle
static HPEN hpenDot;            // "..." pen handle
static HBRUSH hbrushRed;        // red brush handle
static HBRUSH hbrushGreen;      // green brush handle
static HBRUSH hbrushBlue;       // blue brush handle


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant of message number)
//    lparam   - additional information (dependant of message number)
//
//   MESSAGES:
//   
//      WM_COMMAND    - exit command
//      WM_DESTROY    - destroy window
//   
//  RETURN VALUE:
//
//    Depends on the message number.
//
//  COMMENTS:
//
//

LRESULT CALLBACK WndProc(HWND hwnd, 
                         UINT uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    switch (uMessage)
    {

        //
        // **TODO** Add cases here for application messages
        //

        case WM_PAINT:

            // **OUTPUT** - Do the actual painting
            DrawOutput(hwnd);
            break;
    
        case WM_COMMAND: // message: command from application menu
    
            // Message packing of wparam and lparam have changed for Win32,
            // so use the GET_WM_COMMAND macro to unpack the commnad
    
            switch (GET_WM_COMMAND_ID(wparam,lparam))
            {
    
                //
                // **TODO** Add cases here for application
                //          specific command messages
                //
    
                case IDM_EXIT:
                    DestroyWindow (hwnd);
                    break;
    
               
                default:
                    return(DefWindowProc(hwnd, uMessage, wparam, lparam));
            }
            break;
    
        case WM_DESTROY:  // message: window being destroyed

            PostQuitMessage(0);
            break;
    
        default:          // Passes it on if unproccessed
            return(DefWindowProc(hwnd, uMessage, wparam, lparam));
    }
    return (0);
}
    
    
//
//  FUNCTION: InitOutput(void)
//
//  PURPOSE: Create brushes and pens
//
//  PARAMETERS:
//      NONE.
//
//  RETURN VALUE:
//      TRUE  - Success
//      FALSE - Failed, most likely due to memeory allocation problems
//
//  COMMENTS:
//      Initialize all of the static gdi objects that are to be
//      used by paint.
//

BOOL InitOutput(void)
{
    // Create the brush objects

    hbrushRed   = CreateSolidBrush(RGB(255,   0,   0));
    hbrushGreen = CreateSolidBrush(RGB(  0, 255,   0));
    hbrushBlue  = CreateSolidBrush(RGB(  0,   0, 255));

    // Create the "---" pen

    hpenDash = CreatePen(PS_DASH,                // style
                         1,                      // width
                         RGB(0, 0, 0));          // color

    // Create the "..." pen

    hpenDot = CreatePen(PS_DOT,                 // style
                        1,                      // width
                        RGB(0, 0, 0));          // color

    // Return success only if all of the above creates succeeded

    return !(hpenDash    == NULL &&
             hpenDot     == NULL &&
             hbrushRed   == NULL &&
             hbrushGreen == NULL &&
             hbrushBlue  == NULL);
}

//
//  FUNCTION: FreeOutput(void)
//
//  PURPOSE: Frees the brushes and pens
//
//  PARAMETERS:
//      NONE.
//
//  RETURN VALUE:
//      NONE.
//
//  COMMENTS:
//      Deletes the brushes and pens created by InitOutput().
//

void FreeOutput(void)
{
    DeleteObject(hbrushRed);
    DeleteObject(hbrushGreen);
    DeleteObject(hbrushBlue);
    DeleteObject(hpenDash);
    DeleteObject(hpenDot);
}

//
//  FUNCTION: DrawOutput(HWND)
//
//  PURPOSE: Uses GDI api to draw text and graphics to the window.
//
//  PARAMETERS:
//      hwnd - Handle to the window on which to draw.
//
//  RETURN VALUE:
//      NONE.
//
//  COMMENTS:
//      Within the BeginPaint/EndPaint pair, which is used to get
//      and release a handle to a display context, functions are
//      called to draw text in two different ways and to draw
//      several different kinds of graphical objects.
//


void DrawOutput(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    POINT pnt;
    int xQI;        // Quarter inch in x pixels
    int yQI;        // Quarter inch in y pixels

    // Set up a display context to begin painting
    hdc = BeginPaint(hwnd, &ps);

    // Initialize the x & y quarter inch values
    xQI = GetDeviceCaps(hdc, LOGPIXELSX) / 4;   // 1/4 inch
    yQI = GetDeviceCaps(hdc, LOGPIXELSY) / 4;   // 1/4 inch

    // Initialize drawing position to 1/4 inch from the top
    // and from the left of the top, left corner of the
    // client area of the main windows.

    pnt.x = xQI;
    pnt.y = yQI;

    //  Draw some text using the TextOut function
    pnt.y += UseTextOut(hdc, pnt) + yQI;

    // Draw some text using the DrawTextFunction
    pnt.y += UseDrawText(hdc, pnt);

    // Draw some graphics object
    DrawShapes(hdc, pnt);

    // Tell Windows you are done painting
    EndPaint(hwnd,  &ps);
}

//
//  FUNCTION: UseTextOut(HDC, POINT)
//
//  PURPOSE: Uses the TextOut function to draw text to the display context
//
//  PARAMETERS:
//      hdc  - Handle to the display context on which to draw.
//      pnt  - The coordinates for the origin of the TextOut.
//
//  RETURN VALUE:
//      Return - The height of the text drawn
//
//  COMMENTS:
//   
//

int UseTextOut(HDC hdc, POINT pnt)
{
    TEXTMETRIC tm;
    char rgchText[80];  // Text buffer
    int  dy;            // Line height

    // Get the size characteristics of the current font.
    // This information will be used for determining the
    // vertical spacing of text on the screen.

    GetTextMetrics (hdc, &tm);

    // Calculate the pixel distance between the top of each pair of
    //  lines of text.  This is equal to the standard height of the
    //  font characters (tmHeight) plus the standard amount of spacing
    //  (tmExternalLeading) between adjacent lines.

    dy = tm.tmExternalLeading + tm.tmHeight;

    // Send characters to the screen.  After displaying each
    // line of text, advance the vertical position for the
    // next line of text.

    lstrcpy(rgchText, "These characters are being painted using ");
    TextOut(hdc, pnt.x, pnt.y, rgchText, lstrlen (rgchText));
    pnt.y += dy;

    lstrcpy(rgchText, "the TextOut() function, which is fast and ");
    TextOut(hdc, pnt.x, pnt.y, rgchText, lstrlen (rgchText));
    pnt.y += dy;

    lstrcpy(rgchText, "allows programmer control of placement and ");
    TextOut(hdc, pnt.x, pnt.y, rgchText, lstrlen (rgchText));
    pnt.y += dy;

    lstrcpy(rgchText, "formatting details.  However, TextOut() ");
    TextOut(hdc, pnt.x, pnt.y, rgchText, lstrlen (rgchText));
    pnt.y += dy;

    lstrcpy(rgchText, "does not provide any automatic formatting.");
    TextOut(hdc, pnt.x, pnt.y, rgchText, lstrlen (rgchText));
    pnt.y += dy;

    // Return the total height of the text drawn.
    return dy * 5;
}

//
//  FUNCTION: UseDrawText(HDC, POINT)
//
//  PURPOSE: Uses the DrawText function to draw text to the display context
//
//  PARAMETERS:
//      hdc - Handle to the display context on which to draw.
//      pnt - The coordinates for the origin of the DrawText
//
//  RETURN VALUE:
//      Return - The height of the text drawn
//
//  COMMENTS:
//      Put text in a 5-inch by 1-inch rectangle and display it.
//

int UseDrawText(HDC hdc, POINT pnt)
{
    RECT rc;
    char rgchText[300];
    int  dx;                // Width of the text
    int  dy;                // Height of the text

    // First define the size of the rectangle around the text

    dx = (5 * GetDeviceCaps(hdc, LOGPIXELSX));  // 5"
    dy = (1 * GetDeviceCaps(hdc, LOGPIXELSY));  // 1"

    SetRect(&rc, pnt.x, pnt.y, pnt.x + dx, pnt.y + dy);

    // Draw the text within the bounds of the above rectangle
    lstrcpy(rgchText,
            "This text is being displayed with a single call to DrawText().  "
            "DrawText() isn't as fast as TextOut(), and it is somewhat more "
            "constrained, but it provides numerous optional formatting features, "
            "such as the centering and line breaking used in this example.");

    DrawText(hdc,
             rgchText,
             lstrlen(rgchText),
             &rc,
             DT_CENTER | DT_EXTERNALLEADING | 
             DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK);

    // Return the height of the text
    return dy;
}

//
//  FUNCTION: DrawShapes(HDC, POINT)
//
//  PURPOSE: Draw several shapes using a variety of GDI api and objects
//
//  PARAMETERS:
//
//      hdc - Handle to the display context on which to draw.
//      pnt - The coordinates for the origin of the DrawText
//
//  RETURN VALUE:
//
//      Return - NONE.
//
//  COMMENTS:
//
//
//

void DrawShapes(HDC hdc, POINT pnt)
{
    HPEN   hpenOld;         // old pen handle
    HBRUSH hbrushOld;       // old brush handle

    // The (x,y) pixel coordinates of the objects about to be
    // drawn are below, and to the right of, the current
    // coordinate (pnt.x,pnt.y).

    // Draw a red rectangle.

    hbrushOld = SelectObject(hdc, hbrushRed);
    Rectangle(hdc, pnt.x, pnt.y, pnt.x + 50, pnt.y + 30);

    // Draw a green ellipse

    SelectObject(hdc, hbrushGreen);
    Ellipse(hdc, pnt.x + 150, pnt.y, pnt.x + 150 + 50, pnt.y + 30);

    // Draw a blue pie shape

    SelectObject(hdc, hbrushBlue);
    Pie(hdc,
        pnt.x + 300, pnt.y, pnt.x + 300 + 50, pnt.y + 50,   // Bounding rect
        pnt.x + 300 + 50, pnt.y,                            // 1st Radial
        pnt.x + 300 + 50, pnt.y + 50);                      // 2nd Radial
    pnt.y += 50;

    // Restore the old brush
    SelectObject(hdc, hbrushOld);

    // Select a "---" pen, save the old value
    pnt.y += GetDeviceCaps(hdc, LOGPIXELSY) / 4;  // 1/4 inch
    hpenOld = SelectObject(hdc, hpenDash);

    // Move to a specified point
    MoveToEx(hdc, pnt.x, pnt.y, NULL );

    // Draw a line
    LineTo(hdc, pnt.x + 350, pnt.y);

    // Select a "..." pen
    SelectObject(hdc, hpenDot);

    // Draw an arc connecting the line
    Arc(hdc,
        pnt.x, pnt.y - 20, pnt.x + 350, pnt.y + 20, // Bounding rectangle
        pnt.x, pnt.y,                               // 1st radial endpoint
        pnt.x + 350, pnt.y);                        // 2st radial endpoint

    // Restore the old pen
    SelectObject(hdc, hpenOld);
}
