/***********************************************************************
File:   TextWnd.c
Author: JohnMil
Date:   7-21-92


Abstract:
    This module contains functions which deal with the Text window. This
    includes a function to print to said window, and scroll.



Contents:

    RefreshText() -- Redraws text in the bottom of the Text message window
    lstrdup() -- same as strdup() but for LPSTR's
    PrintTextLine() -- Prints a string to the text message window, scrolls
    TextWndProc() -- Windows message processing loop for text message window


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "net.h"
#include <mmsystem.h>
#include <string.h>


#define MAX_TEXT_LINES 80

LPSTR cText[MAX_TEXT_LINES];
TEXTMETRIC tmTextInfo;
int iMaxNumLines,       // Number of lines that will fit in the TextWnd display.
    iCurLine,           // Line # in the text display we're on.
    iTextLines;         // # of lines of text which are valid in cText.


/*=====================================================================
Function: RefreshText()

Inputs: none

Outputs: none

Abstract:
    RefreshText() is responsible for drawing the bottom n lines of the
    display, however many can fit into it. This should only be called when
    a clipping region is in effect and has already been cleared.
======================================================================*/

void RefreshText(
    )
{
    HDC hDC;
    int i;

    hDC = GetDC(hWndText);
    iCurLine = 0;
    for(i=0;i<iMaxNumLines;i++) {
        if ((iTextLines -(iMaxNumLines-i)) >= 0) {
            TextOut(hDC,10,iCurLine * tmTextInfo.tmHeight + 2,
                    cText[iTextLines-(iMaxNumLines-i)],
                    lstrlen(cText[iTextLines-(iMaxNumLines-i)]));
            iCurLine++;
            }
        }

    ReleaseDC(hWndText,hDC);
}



/*=====================================================================
Function: lstrdup()

Inputs: String to duplicate

Outputs:Returns a copy of the string.

Abstract:
    Same as strdup, but works with LPSTR's
======================================================================*/

LPSTR lstrdup(
    LPSTR s
    )
{
    HGLOBAL hMem;
    LPSTR lpRet;

    hMem = GlobalAlloc(GHND,lstrlen(s)+1);
    lpRet = GlobalLock(hMem);
    lstrcpy(lpRet,s);

    return(lpRet);
}




/*=====================================================================
Function: PrintTextLine()

Inputs: Line of text to print

Outputs: none

Abstract:
    PrintTextLine will print a line of text to the text display window, and
    scroll as necessary.
======================================================================*/

void PrintTextLine(
    LPSTR cLine
    )
{
    HDC hDC;
    int i;
    RECT rScroll;
    HGLOBAL hMem;

    hDC = GetDC(hWndText);

    //
    // If we already have MAX_TEXT_LINES of text remembered,
    // we need to zap one to make room for the new line.
    //
    if (iTextLines == MAX_TEXT_LINES) {
        hMem = (HGLOBAL) GlobalHandle(SELECTOROF(cText[0]));
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        for(i=0;i<MAX_TEXT_LINES-1;i++) {
            cText[i] = cText[i+1];
            }
        iTextLines--;
        }

    //
    // Copy the line to be displayed into our buffer
    //
    cText[iTextLines++] = lstrdup(cLine);

    //
    // Scroll if necessary
    //
    if (iCurLine >= iMaxNumLines) {
        GetClientRect(hWndText,&rScroll);
        rScroll.top += 2;
        ScrollWindow(hWndText,0,-tmTextInfo.tmHeight,&rScroll,&rScroll);
        UpdateWindow(hWndText);
        iCurLine--;
        }

    TextOut(hDC,10,iCurLine * tmTextInfo.tmHeight + 2,
            cText[iTextLines-1],
            lstrlen(cText[iTextLines-1]));

    iCurLine++;

    ReleaseDC(hWndText,hDC);

    return;
}


/*=====================================================================
Function: TextWndProc()

Inputs: Standard windows entrypoint parms

Outputs: success

Abstract:
    This is the main procedure to take care of drawing, resizing etc.
    the text message window.
======================================================================*/


LONG FAR PASCAL TextWndProc(
    HWND hWnd,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PAINTSTRUCT ps;
    HDC hDC;
    int i;
    HGLOBAL hMem;

    switch (Message) {

        case WM_CREATE:
            for(i=0;i<MAX_TEXT_LINES;i++) {
                cText[i] = (LPSTR) NULL;
//                hMem = GlobalAlloc(GHND,sizeof(char));
//                cText[i] = GlobalLock(hMem);
//                cText[i][0] = '\0';
                }

            iTextLines = iCurLine = 0;
            hDC = GetDC(hWnd);
            if(!GetTextMetrics(hDC,&tmTextInfo)) {
                MessageBox((HWND)NULL,"Unable to get text metrics. Text won't work right.",
                           "TextWndProc",MB_ICONEXCLAMATION|MB_APPLMODAL);
                }
            ReleaseDC(hWnd,hDC);
            GetClientRect(hWnd,&rText);
            iMaxNumLines = (rText.bottom - rText.top-5)/tmTextInfo.tmHeight;
            break;

        case WM_KEYDOWN:
            SendMessage(hWndMaze,WM_KEYDOWN,wParam,lParam);
            break;

        case WM_MOVE:
            break;

        case WM_SIZE:
            GetClientRect(hWnd,&rText);
            iMaxNumLines = (rText.bottom - rText.top - 5)/tmTextInfo.tmHeight;
            RefreshText();
            break;

        case WM_PAINT:
            GetClientRect(hWnd,&rText);
            hDC = BeginPaint(hWnd, &ps);
            SetBkMode(hDC, OPAQUE);
            RefreshText();
            EndPaint(hWnd, &ps);
            break;

        case WM_CLOSE:
            for (i=0; i<iTextLines ; i++) {
                if (cText[i] != NULL) {
                    hMem = (HGLOBAL) GlobalHandle(SELECTOROF(cText[i]));
                    GlobalUnlock(hMem);
                    GlobalFree(hMem);
                }
            }
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
        }

    return(0);
}
