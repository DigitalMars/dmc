
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
 *  CLOCK.C - Windows DDEML Clock
 *
 *  DDE Transactions:
 *  ----------------
 *  Service: Clock
 *  Topic  : Time
 *  Item   : Now
 *
 *  Use Request or Advise to get the time or use Poke to change the time.
 *  Time Format Hour:Minute:Seconds where
 *   Hour    = 0-23
 *   Minute  = 0-59
 *   Seconds = 0-59
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "windows.h"
#include <ddeml.h>
#include "clock.h"


DWORD   idInst = 0;

CLOCKDISPSTRUCT ClockDisp;

HANDLE  hInst;
HWND    hWindow;

HBRUSH  hbrBackground;
HBRUSH  hbrColorWindow;
HBRUSH  hbrColorBlack;
HBRUSH  hbrForeground;
HFONT   hFont;
HPEN    hpenForeground;
HPEN    hpenBackground;

INT     nLeadZero   = 0;
INT     TimerID     = 1;    /* number used for timer-id */
INT     clockRadius;
INT     HorzRes;
INT     VertRes;

LONG    aspectD;
LONG    aspectN;

CHAR    szBuffer[BUFLEN];    /* buffer for stringtable stuff */
CHAR    szFontFile[20];
CHAR    szIniFile[20];
CHAR    szSection[30];

POINT   clockCenter;

TIME    oTime;

RECT    clockRect;
RECT    rCoordRect;

HDDEDATA CALLBACK DdeCallback(WORD usType, WORD usFmt, HCONV hConv, HSZ hsz1,
        HSZ hsz2, HDDEDATA hData, DWORD lData1, DWORD lData2);
HSZ hszTime, hszNow, hszClock;    /* Hszs for DDEML use */

/*
 *  Function Prototypes
 */

LONG APIENTRY ClockWndProc(register HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void NEAR PASCAL ClockCreate(HWND hWnd);
BOOL NEAR PASCAL ClockInit(HANDLE hInstance);
void NEAR PASCAL ClockPaint(HWND hWnd, register HDC hDC, INT hint);
void NEAR PASCAL ClockSize(register HWND hWnd,INT newWidth,INT newHeight, WORD SizeWord);
void NEAR PASCAL ClockTimer(HWND hWnd, UINT msg);
void NEAR PASCAL CompClockDim(void);
void NEAR PASCAL CreateTools(void);
void NEAR PASCAL DeleteTools(void);
void NEAR PASCAL DrawBorder(HWND hWnd, register HDC hDC);
void NEAR PASCAL DrawFace(HDC hDC);
void NEAR PASCAL DrawHand(register HDC hDC, INT pos, HPEN hPen, INT scale, INT patMode);
void NEAR PASCAL DrawFatHand(register HDC hDC, INT pos, HPEN hPen, BOOL hHand);
void NEAR PASCAL FormatInit(register HANDLE hInstance, BOOL fModeChange);
void NEAR PASCAL SizeFont(HWND hWnd, INT newHeight, INT newWidth, INT wlen);
void NEAR PASCAL SetMenuBar( HWND hWnd );


/*
 *  CreateTools()
 */

void NEAR PASCAL CreateTools(void)

{
  hbrForeground  = CreateSolidBrush(GetSysColor(COLOR_WINDOWTEXT));
  hbrColorWindow = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
  hbrColorBlack  = CreateSolidBrush(0L);
  hbrBackground  = hbrColorWindow;
  hpenForeground = CreatePen(0, 1, GetSysColor(COLOR_WINDOWTEXT));
  hpenBackground = CreatePen(0, 1, GetSysColor(COLOR_WINDOW));
}


/*
 *  DeleteTools()
 */

void NEAR PASCAL DeleteTools(void)

{
  DeleteObject(hbrForeground);
  DeleteObject(hbrColorWindow);
  DeleteObject(hbrColorBlack);
  DeleteObject(hpenForeground);
  DeleteObject(hpenBackground);
}


/*
 * SizeFont() - size font according to window size
 */

void NEAR PASCAL SizeFont(HWND hWnd, INT newHeight, INT newWidth, INT wlen)
{
register HDC    hDC;
TEXTMETRIC    tm;
LOGFONT    FontStruct;

    hDC = GetDC(hWnd);
    GetTextMetrics(hDC, &tm);

    if (ClockDisp.wFormat == IDM_DIGITAL) {
        if (hFont != NULL)
            DeleteObject(hFont);

        FontStruct.lfUnderline = FALSE;
        FontStruct.lfStrikeOut = FALSE;
        FontStruct.lfItalic    = FALSE;
        FontStruct.lfEscapement = FALSE;
        FontStruct.lfOrientation = FALSE;
        FontStruct.lfOutPrecision = OUT_DEFAULT_PRECIS;
        FontStruct.lfClipPrecision = CLIP_DEFAULT_PRECIS;

        /* Note that the numbers used in this formula depend on the
         * size of the numbers in the fonts which are 12 vert, 7 horz.
        */
        if (!ClockDisp.bIconic)  {
            FontStruct.lfHeight = (SHORT)min(newHeight/2, 45);

            /* The formula at the end is somewhat empirical. */
            FontStruct.lfWidth = (SHORT)min(25, newWidth/(wlen*3/2));

            /* This if is here because of a problem where:  if a clock
             * existed with a maximum size font, any new clock with a
             * smaller client area would get the same font that would not
             * fit in the Window.  So here, if the width does not match
             * maximum font dimensions, don't use the maximum height.
             */
            if (FontStruct.lfWidth != 25 && FontStruct.lfHeight == 45)
                FontStruct.lfHeight = 40;
        }
        else {
            FontStruct.lfHeight = (SHORT)(newHeight/3);
            FontStruct.lfWidth  = (SHORT)(newWidth/5);
        }

        FontStruct.lfCharSet      = ANSI_CHARSET;
        FontStruct.lfQuality      = DRAFT_QUALITY;
        FontStruct.lfWeight      = FW_NORMAL;
        FontStruct.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;

        lstrcpy(FontStruct.lfFaceName, "DIGITAL");

        hFont = CreateFontIndirect(&FontStruct);

        SelectObject(hDC, hFont);
        GetTextMetrics(hDC, &tm);
        SelectObject(hDC, GetStockObject(SYSTEM_FONT));
    }

    ReleaseDC(hWnd, hDC);

    /* Compute placement for digital text. */
    ClockDisp.line1.x = (newWidth) / 2;
    ClockDisp.line1.y = (newHeight - (tm.tmHeight)) / 2;
    ClockDisp.yline2  = ClockDisp.line1.y + tm.tmHeight;
}


/*
 *  CompClockDim() - Recompute the clock's dimensions.
 */

void NEAR PASCAL CompClockDim(void)

{
INT        i;
register INT    tWidth;
register INT    tHeight;

    tWidth = clockRect.right - clockRect.left;
    tHeight = clockRect.bottom - clockRect.top;

    if (tWidth > (INT)((tHeight * aspectD) / aspectN)) {
        i = (INT)((tHeight * aspectD) / aspectN);
        clockRect.left += (tWidth - i) >> 1;
        clockRect.right = clockRect.left + i;
    }
    else {
        i = (INT)((tWidth * aspectN) / aspectD);
        clockRect.top += (tHeight - i) >> 1;
        clockRect.bottom = clockRect.top + i;
    }
}


/*
 *  ClockSize()
 */

void NEAR PASCAL ClockSize(register HWND hWnd,
               INT       newWidth,
               INT       newHeight,
               WORD      SizeWord)
{
    SetRect((LPRECT)&(clockRect), 0, 0, newWidth, newHeight);
    CompClockDim();

    if (SizeWord == SIZEICONIC) {
        /* Update once every 1/2 minute in the iconic state */
        KillTimer(hWnd, TimerID);
        SetTimer(hWnd, TimerID, (WORD)ICON_TLEN, 0L);
        ClockDisp.bIconic = TRUE;
    }
    else if (ClockDisp.bIconic) {
        /* Update every 1/2 second in the opened state. */
        KillTimer(hWnd, TimerID);
        SetTimer(hWnd, TimerID, OPEN_TLEN, 0L);
        ClockDisp.bIconic = FALSE;
    }

    /* Compute where the digital readout should go. */
    SizeFont(hWnd, newHeight, newWidth, ClockDisp.wDigTimeLen);
}


/*
 *  DrawBorder() - Draws a Red Border around either clock.
 */

void NEAR PASCAL DrawBorder(HWND hWnd, register HDC hDC)
{
RECT Rect;
HPEN hPen;

    GetClientRect(hWnd, (LPRECT) &Rect);

    hPen = CreatePen(PS_SOLID, (ClockDisp.bIconic) ? 1 : 2,
               (ClockDisp.bColor)  ? RGB(255,0,0) : RGB(255,255,255));

    SelectObject(hDC, hPen);
    Rectangle(hDC, Rect.left+1, Rect.top+1, Rect.right, Rect.bottom);
    SelectObject(hDC, GetStockObject(BLACK_PEN));

    DeleteObject(hPen);

    /* Draw an external black border on an icon without killing the client area. */
    if (ClockDisp.bIconic) {
        MoveToEx(hDC, Rect.left, Rect.top, NULL);
        LineTo(hDC, Rect.left,  Rect.bottom);
        LineTo(hDC, Rect.right, Rect.bottom);
        LineTo(hDC, Rect.right, Rect.top);
        LineTo(hDC, Rect.left,  Rect.top);
    }
}


/*
 *  DrawFace()
 */

void NEAR PASCAL DrawFace(HDC hDC)
{
INT        i;
RECT       tRect;
INT        blobHeight, blobWidth;

    blobWidth = (INT)((MAXBLOBWIDTH * (LONG)(clockRect.right - clockRect.left)) / HorzRes);
    blobHeight = (INT)((blobWidth * aspectN) / aspectD);

    if (blobHeight < 2)
        blobHeight = 1;

    if (blobWidth < 2)
        blobWidth = 2;

    InflateRect((LPRECT)&clockRect, -(blobHeight >> 1), -(blobWidth >> 1));

    clockRadius = (clockRect.right - clockRect.left-6) >> 1;
    clockCenter.y = clockRect.top + ((clockRect.bottom - clockRect.top) >> 1);
    clockCenter.x = clockRect.left + clockRadius+3;

    for (i=0; i < 60; i++) {
        tRect.top  = (INT)(((LONG)(CirTab[i].cos) * clockRadius) / CLKSCALE + clockCenter.y);
        tRect.left = (INT)(((LONG)(CirTab[i].sin) * clockRadius) / CLKSCALE + clockCenter.x);

        if (i % 5) {
            /* Draw a dot. */
            if (blobWidth > 2 && blobHeight >= 2) {
                tRect.right = tRect.left + 1;
                tRect.bottom = tRect.top + 1;
                FillRect(hDC, (LPRECT)&tRect, hbrForeground);
            }
        }
        else {
            tRect.right = tRect.left + blobWidth;
            tRect.bottom = tRect.top + blobHeight;
            OffsetRect((LPRECT)&tRect, -(blobWidth >> 1) , -(blobHeight >> 1));
            FillRect(hDC, (LPRECT)&tRect, hbrForeground);
        }
    }
    InflateRect((LPRECT)&clockRect, (blobHeight >> 1), (blobWidth >> 1));
}


/*
 *  DrawHand() - Draw the second hand using XOR mode.
 */

void NEAR PASCAL DrawHand(register HDC hDC,
              INT          pos,
              HPEN         hPen,
              INT          scale,
              INT          patMode)
{
INT       radius;

    MoveToEx(hDC, clockCenter.x, clockCenter.y, NULL);
    radius = (INT)(((LONG)clockRadius * scale) / 100);
    SetROP2(hDC, patMode);
    SelectObject(hDC, hPen);

    LineTo(hDC, clockCenter.x + (INT)(((LONG)(CirTab[pos].sin) * (radius)) / CLKSCALE),
        clockCenter.y + (INT)(((LONG)(CirTab[pos].cos) * (radius)) / CLKSCALE));
}


/*
 *  DrawFatHand() - Draws either hour or minute hand.
 */

void NEAR PASCAL DrawFatHand(register HDC hDC, INT pos, HPEN hPen, BOOL hHand)
{
register INT    m;
INT        n;
INT        scale;
POINT      tip;
POINT      stip;

    SetROP2(hDC, R2_COPYPEN);

    SelectObject(hDC, hPen);

    scale = hHand ? 7 : 5;

    n = (pos+15)%60;
    m = (INT)((((LONG)clockRadius*scale) / 100));
    stip.y = (INT)((LONG)(CirTab[n].cos) * m / CLKSCALE);
    stip.x = (INT)((LONG)(CirTab[n].sin) * m / CLKSCALE);

    scale = hHand ? 65 : 80;
    tip.y = (INT)((LONG)(CirTab[pos].cos) * (((LONG)clockRadius * scale) / 100) / CLKSCALE);
    tip.x = (INT)((LONG)(CirTab[pos].sin) * (((LONG)clockRadius * scale) / 100) / CLKSCALE);

    MoveToEx(hDC, clockCenter.x+stip.x, clockCenter.y+stip.y, NULL);
    LineTo(hDC, clockCenter.x+tip.x,  clockCenter.y+tip.y);
    MoveToEx(hDC, clockCenter.x-stip.x, clockCenter.y-stip.y, NULL);
    LineTo(hDC, clockCenter.x+tip.x,  clockCenter.y+tip.y);

    scale = hHand ? 15 : 20;

    n = (pos + 30) % 60;
    m = (INT)(((LONG)clockRadius * scale) / 100);
    tip.y = (INT)((LONG)(CirTab[n].cos) * m / CLKSCALE);
    tip.x = (INT)((LONG)(CirTab[n].sin) * m / CLKSCALE);
    MoveToEx(hDC, clockCenter.x+stip.x, clockCenter.y+stip.y, NULL);
    LineTo(hDC, clockCenter.x+tip.x,  clockCenter.y+tip.y);
    MoveToEx(hDC, clockCenter.x-stip.x, clockCenter.y-stip.y, NULL);
    LineTo(hDC, clockCenter.x+tip.x,  clockCenter.y+tip.y);
}


/*
 *  ClockPaint()
 */

void NEAR PASCAL ClockPaint(HWND hWnd, register HDC hDC, INT hint)
{
INT     hour;
CHAR    *pszTime;
RECT    Rect;
TIME    nTime;
DWORD   rgbCol;
HBRUSH  hBr;

    GetClientRect(hWnd, (LPRECT) &Rect);

    GetTime(&nTime);

    if (ClockDisp.wFormat == IDM_DIGITAL) {  /* Digital Display */
        if (hint == REPAINT || ClockDisp.bIconic) {
            SelectObject(hDC, GetStockObject(BLACK_BRUSH));
            DrawBorder(hWnd, hDC);

            /* Set old values as undefined, so entire clock updated. */
            oTime.hour24 = 25;
            oTime.minute = 60;
            oTime.ampm   = 2;
        }

        if (oTime.hour24 != nTime.hour24) {
            if (ClockDisp.wTimeFormat)
                hour = nTime.hour24;
            else
                hour = nTime.hour12;

            ClockDisp.szDigTime[0] = (CHAR)('0' + hour / 10);
            ClockDisp.szDigTime[1] = (CHAR)('0' + hour % 10);
        }

        if (oTime.minute != nTime.minute) {
            ClockDisp.szDigTime[3]  = (CHAR)('0' + nTime.minute / 10);
            ClockDisp.szDigTime[4]  = (CHAR)('0' + nTime.minute % 10);
        }

        /* Kill Leading zero if needed. */
        if (nLeadZero == 0 && ClockDisp.szDigTime[0] == '0')
            pszTime = ClockDisp.szDigTime + 1;
        else
            pszTime = ClockDisp.szDigTime;

        SetTextColor(hDC, (ClockDisp.bColor) ? RGB(0,255,0) : RGB(255,255,255));
        SetBkColor(hDC, 0L);
        SetTextAlign(hDC, TA_CENTER);

        ClockDisp.wDigTimeLen = (WORD)((ClockDisp.bIconic ? 5 : 8) + ClockDisp.szDigTime - pszTime);

        /* Is the font ready yet? */
        if (hFont == 0 || ClockDisp.bNewFont)  {
            /* Create a suitable font */
            SizeFont(hWnd, Rect.bottom - Rect.top, Rect.right - Rect.left, ClockDisp.wDigTimeLen);
            ClockDisp.bNewFont = FALSE;
        }

        SelectObject(hDC, hFont);
        ClockDisp.szDigTime[6] = (CHAR)('0' + nTime.second / 10);
        ClockDisp.szDigTime[7] = (CHAR)('0' + nTime.second % 10);

        Rect.left += 4;
        Rect.right -= 4;
        Rect.top = ClockDisp.line1.y;
        Rect.bottom = ClockDisp.yline2;
        ExtTextOut(hDC, ClockDisp.line1.x, ClockDisp.line1.y, ETO_OPAQUE | ETO_CLIPPED,
          (LPRECT)&Rect, pszTime, (UINT)ClockDisp.wDigTimeLen, (LPINT)NULL);
        SelectObject(hDC, GetStockObject(SYSTEM_FONT));
    }
    else {
        /* Analog display */
        SetBkMode(hDC, TRANSPARENT);
        if (hint == REPAINT) {
            SetBkMode(hDC, OPAQUE);
            /* When switching from Digital to analog, the brush selected
             *  continued to be black; So, the current background is to be
             * selected;
             * Fix for Bug #6385 -- SANKAR -- 11-26-89 */
            SelectObject(hDC, hbrBackground);

            /* Make a temp brush to color the background.  This is to
             * force use of a solid color so the hand motion is painted
             * correctly.
             */
            rgbCol = GetNearestColor(hDC, GetSysColor(COLOR_WINDOW));
            hBr = CreateSolidBrush(rgbCol);

            FillRect(hDC, &Rect, hBr);

            DeleteObject(hBr);

            SetBkMode(hDC, TRANSPARENT);

            DrawBorder(hWnd, hDC);
            DrawFace(hDC);
            DrawFatHand(hDC, oTime.hour * 5 + (oTime.minute / 12), hpenForeground, HHAND);
            DrawFatHand(hDC, oTime.minute, hpenForeground, MHAND);

            if (!ClockDisp.bIconic)       /* Draw the second hand. */
                DrawHand(hDC, oTime.second, hpenBackground, SECONDSCALE, R2_NOT);

            /* NOTE: Don't update oTime in this case! */

            return;
        }
        else if (hint == HANDPAINT) {
            if ((!ClockDisp.bIconic) && nTime.second != oTime.second) /* Erase the old second hand. */
                DrawHand(hDC, oTime.second, hpenBackground, SECONDSCALE, R2_NOT);

            if (nTime.minute != oTime.minute || nTime.hour != oTime.hour) {
                if (ClockDisp.bIconic) {
                    DrawHand(hDC, oTime.minute, hpenBackground, MINUTESCALE, R2_COPYPEN);
                    DrawHand(hDC, oTime.hour * 5 + (oTime.minute / 12), hpenBackground, HOURSCALE, R2_COPYPEN);
                    DrawHand(hDC, nTime.minute, hpenForeground, MINUTESCALE, R2_COPYPEN);
                    DrawHand(hDC, nTime.hour * 5 + (nTime.minute / 12), hpenForeground, HOURSCALE, R2_COPYPEN);
                }
                else {
                    DrawFatHand(hDC, oTime.minute, hpenBackground, MHAND);
                    DrawFatHand(hDC, oTime.hour*5+(oTime.minute/12), hpenBackground, HHAND);
                   DrawFatHand(hDC, nTime.minute, hpenForeground, MHAND);
                    DrawFatHand(hDC, (nTime.hour) * 5 + (nTime.minute / 12), hpenForeground, HHAND );
                }
            }

            if (!ClockDisp.bIconic && nTime.second != oTime.second) /* Draw new second hand */
                DrawHand(hDC, nTime.second, hpenBackground, SECONDSCALE, R2_NOT);
        }
    }
    oTime = nTime;
}


/*
 *  ClockTimer()
 *
 *  msg - timer ID
 *
 * Called by windows to tell CLOCK there has been a time change.
 *
 */

void NEAR PASCAL ClockTimer(HWND hWnd, UINT msg)
{
HDC    hDC;
TIME    nTime;

    GetTime(&nTime);

    /* It's possible to change any part of the system at any time
     * through the Control Panel.  So we check everything.
     */
    if (((nTime.second == oTime.second) || ClockDisp.bIconic) &&
        (nTime.minute == oTime.minute)          &&
        (nTime.hour24 == oTime.hour24))
        return;

    hDC = GetDC(hWnd);
    ClockPaint(hWnd, hDC, HANDPAINT);
    ReleaseDC(hWnd, hDC);
    DdePostAdvise(idInst, hszTime, hszNow);
      UNREFERENCED_PARAMETER(msg);
}


/*
 *  ClockCreate()
 */

void NEAR PASCAL ClockCreate(HWND hWnd)
{
INT        i;
register HDC    hDC;
INT        HorzSize;
INT        VertSize;

    hDC = GetDC(hWnd);
    VertRes = GetDeviceCaps(hDC, VERTRES);
    HorzRes = GetDeviceCaps(hDC, HORZRES);
    VertSize= GetDeviceCaps(hDC, VERTSIZE);
    HorzSize= GetDeviceCaps(hDC, HORZSIZE);
    ReleaseDC(hWnd, hDC);

    aspectN = ((LONG)VertRes * 100) / (LONG)VertSize;
    aspectD = ((LONG)HorzRes * 100) / (LONG)HorzSize;

    CreateTools();

    /* Scale sines for aspect ratio if this is the first instance */
    for (i=0; i < 60; i++) {
        CirTab[i].sin = (SHORT)((CirTab[i].sin * aspectN) / aspectD);
    }
}



/*
 *  FormatInit() -  International initialization.
 */

void NEAR PASCAL FormatInit(register HANDLE hInstance, BOOL fModeChange)
{
WORD i, ii;
CHAR szWinHeader[21], szKeyName[21], szRetVal[21];

    for (i=0; i < 11; i++)
        ClockDisp.szDigTime[i] = ' ';

    LoadString(hInstance, IDS_INTL, (LPSTR)szWinHeader, 20);

    LoadString(hInstance, IDS_ITIME, (LPSTR)szKeyName, 20);
    ClockDisp.wTimeFormat = (WORD)GetProfileInt((LPSTR)szWinHeader, (LPSTR)szKeyName, 0);

    LoadString(hInstance, IDS_S1159, (LPSTR)szKeyName, 20);
    LoadString(hInstance, IDS_1159, (LPSTR)szRetVal, 20);
    i = (WORD)GetProfileString((LPSTR)szWinHeader, (LPSTR)szKeyName, (LPSTR)szRetVal, (LPSTR)&ClockDisp.szAMPM[0][0], 7);

    LoadString(hInstance, IDS_S2359, (LPSTR)szKeyName, 20);
    LoadString(hInstance, IDS_2359, (LPSTR)szRetVal, 20);
    ii = (WORD)GetProfileString((LPSTR)szWinHeader, (LPSTR)szKeyName, (LPSTR)szRetVal, (LPSTR)&ClockDisp.szAMPM[1][0], 7);

    nLeadZero = GetProfileInt((LPSTR)szWinHeader, "iTLzero", 0);

    LoadString(hInstance, IDS_STIME, (LPSTR)szKeyName, 20);
    LoadString(hInstance, IDS_TIMESEP, (LPSTR)szRetVal, 20);

    GetProfileString((LPSTR)szWinHeader, (LPSTR)szKeyName, (LPSTR)szRetVal, (LPSTR)szRetVal, 20);
    ClockDisp.cTimeSep = szRetVal[0];

    ClockDisp.szDigTime[2] = ClockDisp.cTimeSep;
    ClockDisp.szDigTime[5] = ClockDisp.cTimeSep;

    LoadString(hInstance, IDS_USNAME, (LPSTR)szWinHeader, 20);
    LoadString(hInstance, IDS_CLKFORMAT, (LPSTR)szKeyName, 20);

    /* We will read the new mode (DIGITAL/ANALOG) only during init time. */
    if (fModeChange)
    {
        if (GetPrivateProfileInt((LPSTR)szWinHeader, (LPSTR)szKeyName, 1, (LPSTR)szIniFile))
            ClockDisp.wFormat = IDM_ANALOG;
        else
            ClockDisp.wFormat = IDM_DIGITAL;
    }

    if (ClockDisp.wFormat == IDM_ANALOG)
        hbrBackground = hbrColorWindow;
    else
        hbrBackground = hbrColorBlack;

    ClockDisp.wDigTimeLen = 2+1+2+1+2+1;

    if (!ClockDisp.wTimeFormat)
        ClockDisp.wDigTimeLen += ((i > ii) ? (i) : (ii));
}


/*
 *  ClockInit()
 */

BOOL NEAR PASCAL ClockInit(HANDLE hInstance)
{
HDC       hDC;
WNDCLASS  ClockClass;

    FormatInit(hInstance, TRUE);

    ClockClass.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    ClockClass.lpfnWndProc   = ClockWndProc;
    ClockClass.cbClsExtra    = 0;
    ClockClass.cbWndExtra    = 0;
    ClockClass.hInstance     = hInstance;
    ClockClass.hIcon         = NULL;
    ClockClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    ClockClass.hbrBackground = (HBRUSH) NULL;
    ClockClass.lpszMenuName  = (LPSTR)"Clock";
    ClockClass.lpszClassName = (LPSTR)"Clock";

    if (!RegisterClass((LPWNDCLASS)&ClockClass))
        return(FALSE);

    LoadString(hInstance, IDS_FONTFILE, (LPSTR)szFontFile, 20);
    AddFontResource(szFontFile);

    /* Check the number of colors that the display is capable of. */
    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    ClockDisp.bColor = (GetDeviceCaps(hDC, NUMCOLORS) > 2);
    ClockDisp.bColor = FALSE;
    DeleteDC(hDC);

    ClockDisp.bTmpHide = FALSE;
    ClockDisp.bNewFont = FALSE;
    ClockDisp.bColor   = TRUE;
    ClockDisp.bNewFont = TRUE;

    return(TRUE);
}

/*
 *  SetMenuBar() - places or removes the menu bar, etc.
 *
 *  Based on the flag ClockDisp.bNoTitle (ie: do we want a menu/title
 *  bar or not?), adds or removes the window title and menu bar:
 *    Gets current style, toggles the bits, and re-sets the style.
 *    Must then resize the window frame and show it.
 */

void NEAR PASCAL SetMenuBar( HWND hWnd )
{
    static DWORD wID;
    DWORD   dwStyle;

    dwStyle = GetWindowLong( hWnd, GWL_STYLE );
    if( ClockDisp.bNoTitle ) {
        /* remove caption & menu bar, etc. */
        dwStyle &= ~(WS_DLGFRAME | WS_SYSMENU |
                   WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
        wID = SetWindowLong( hWnd, GWL_ID, 0 );
    }
    else {
        /* put menu bar & caption back in */
        dwStyle = WS_TILEDWINDOW | dwStyle;
        SetWindowLong( hWnd, GWL_ID, wID );
    }
    SetWindowLong( hWnd, GWL_STYLE, dwStyle );
    SetWindowPos( hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE |
        SWP_NOZORDER | SWP_FRAMECHANGED );
    ShowWindow( hWnd, SW_SHOW );
}

/*
 *  AboutDlgProc()
 */

BOOL APIENTRY AboutDlgProc ( hwnd, msg, wParam, lParam )
HWND          hwnd;
UINT msg;
WPARAM wParam;
LPARAM lParam;
{
    switch (msg) {
        case WM_INITDIALOG:
            /* nothing to initialize */
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    break;

                default:
                    return FALSE;
            }
            break;

        default:
            return(FALSE);
    }

    return TRUE;
}



/*
 *  ClockWndProc()
 */

LONG APIENTRY ClockWndProc(register HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HMENU       hMenu;
static CHAR szAppName[12];  /* application name buffer */
PAINTSTRUCT ps;

    switch (message) {
        case WM_COMMAND:
        switch (wParam)  {

            case IDM_ANALOG:
            case IDM_DIGITAL:
                if ((WORD)wParam != ClockDisp.wFormat) {
                    /* Switch flag to other choice */
                    hMenu = GetMenu(hWnd);
                    CheckMenuItem(hMenu, ClockDisp.wFormat, MF_BYCOMMAND | MF_UNCHECKED);
                    CheckMenuItem(hMenu, ClockDisp.wFormat = (WORD) wParam, MF_BYCOMMAND | MF_CHECKED);
                    InvalidateRect(hWnd, (LPRECT) NULL, TRUE);
                }
            break;

            case IDM_NOTITLE:
                goto toggle_title;

            case IDM_ABOUT: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)AboutDlgProc);
                break;
            }

            default:
                goto defproc;
        }
        break;

        case WM_SIZE:
            ClockDisp.bNewFont = TRUE;
            ClockSize(hWnd, LOWORD(lParam), HIWORD(lParam), (WORD)wParam);
            UpdateWindow(hWnd);
            break;

        case WM_QUERYDRAGICON:
            return (LONG)LoadIcon(hInst, "cckk");

        case WM_CLOSE:
        case WM_ENDSESSION:
          DestroyWindow( hWnd );
          break;

        case WM_DESTROY: {
            CHAR           szInt[10];
            HCURSOR        hTempCursor;

            KillTimer(hWnd, TimerID);
            DeleteTools();
            if (hFont)
                DeleteObject(hFont);
            RemoveFontResource(szFontFile);

            SetCapture(hWnd);
            hTempCursor=SetCursor(LoadCursor(NULL, IDC_WAIT));

            if (!(IsIconic(hWnd) || IsZoomed(hWnd)))
                GetWindowRect(hWnd, &rCoordRect);

            /* Write new configuration to DDEMLCLK.INI */
            LoadString(hInst, IDS_CLKFORMAT, (LPSTR)szBuffer, BUFLEN-1);
            szInt[0] = (CHAR)('0' + (ClockDisp.wFormat == IDM_ANALOG));
            szInt[1] = 0;
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)szBuffer,
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)IsIconic(hWnd));
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Minimized",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)IsZoomed(hWnd));
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Maximized",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)rCoordRect.left);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Left",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)rCoordRect.top);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Top",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)rCoordRect.right);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Right",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)rCoordRect.bottom);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"Bottom",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)ClockDisp.bTopMost);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"TopMost",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            wsprintf((LPSTR)szInt, (LPSTR)"%i", (INT)ClockDisp.bNoTitle);
            WritePrivateProfileString((LPSTR)szSection,
                                      (LPSTR)"NoTitle",
                                      (LPSTR)szInt,
                                      (LPSTR)szIniFile);
            PostQuitMessage(0);
            break;
        }

        case WM_WININICHANGE:
            /* FALSE indicates that we don't want to change the display format */
            FormatInit(hInst, FALSE);
            InvalidateRect(hWnd, (LPRECT)NULL, TRUE);
            break;

        case WM_PAINT:
            /* Added to force total repaint to solve
             * problem of garbage under second hand when hidden
             * by menu or popup.
             */
            InvalidateRect(hWnd, (LPRECT)NULL, TRUE);
            BeginPaint(hWnd, (LPPAINTSTRUCT)&ps);

            if (ClockDisp.wFormat == IDM_DIGITAL) {
                hbrBackground = hbrColorBlack;
                FillRect(ps.hdc, (LPRECT)&clockRect, hbrBackground);
            }
            else
                hbrBackground=hbrColorWindow;

            ClockPaint(hWnd, ps.hdc, REPAINT);
            EndPaint(hWnd, (LPPAINTSTRUCT)&ps);
            break;

        case WM_TIMECHANGE:
            /* Redraw. */
            InvalidateRect(hWnd, (LPRECT)NULL, TRUE);

        case WM_TIMER:
            ClockTimer(hWnd, (WORD)wParam);
            break;

        case WM_SYSCOMMAND:
            switch (wParam)  {

                case SC_MINIMIZE:
                    if (!IsZoomed(hWnd))
                        GetWindowRect(hWnd, (LPRECT)&rCoordRect);
                    ClockDisp.bMinimized = TRUE;
                    ClockDisp.bMaximized = FALSE;
                    break;
                case SC_MAXIMIZE:
                    if (!IsIconic(hWnd))
                        GetWindowRect(hWnd, (LPRECT)&rCoordRect);
                    ClockDisp.bMinimized = FALSE;
                    ClockDisp.bMaximized = TRUE;
                    break;

                case IDM_TOPMOST: {
                    /* toggles topmost option
                     */
                    hMenu = GetSystemMenu(hWnd, FALSE);
                    if( ClockDisp.bTopMost )  {
                        CheckMenuItem( hMenu, IDM_TOPMOST, MF_BYCOMMAND | MF_UNCHECKED );
                        SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
                        ClockDisp.bTopMost = FALSE;
                    }
                    else {
                        CheckMenuItem( hMenu, IDM_TOPMOST, MF_BYCOMMAND | MF_CHECKED );
                        SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
                        ClockDisp.bTopMost = TRUE;
                    }
                    break;
                }

            }
            return(DefWindowProc(hWnd, message, wParam, lParam));
            break;

        case WM_MOUSEACTIVATE:
            /* right button temporarily hides the window if topmost is
             * enabled (window re-appears when right button is released).
             * When this happens, we don't want to activate the clock window
             * just before hiding it (it would look really bad), so we
             * intercept the activate message.
             */
            if( GetAsyncKeyState( VK_RBUTTON ) & 0x8000 )
                return( MA_NOACTIVATE );
            else
                goto defproc;
            break;

        case WM_RBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
            /* right button temporarily hides the window, if the window
             * is topmost, and if no menu is currently "active"
             */
            if( !ClockDisp.bTmpHide && ClockDisp.bTopMost ) {
                ShowWindow( hWnd, SW_HIDE );
                SetCapture( hWnd );
                ClockDisp.bTmpHide = TRUE;
            }
            break;

        case WM_RBUTTONUP:
        case WM_NCRBUTTONUP:
            /* if window is currently hidden, right button up brings it
             * back. Must make sure we show it in its previous state - ie:
             * minimized, maximized or normal.
             */
            if( ClockDisp.bTmpHide ) {
                ReleaseCapture();
                if( IsIconic(hWnd) )
                    ShowWindow( hWnd, SW_SHOWMINNOACTIVE );
                else if( IsZoomed( hWnd ) )
                    ShowWindow( hWnd, SW_SHOWMAXIMIZED );
                else
                    ShowWindow( hWnd, SW_SHOWNOACTIVATE );
                ClockDisp.bTmpHide = FALSE;
            }
            break;

        case WM_KEYDOWN:
            /* ESC key toggles the menu/title bar (just like a double click
             * on the client area of the window.
             */
            if( (wParam == VK_ESCAPE) && !(HIWORD( lParam ) & 0x4000) )
                goto toggle_title;
            break;

        case WM_NCLBUTTONDBLCLK:
            if( !ClockDisp.bNoTitle )
                /* if we have title bars etc. let the normal stuff take place */
                goto defproc;

            /* else: no title bars, then this is actually a request to bring
             * the title bars back...
             */

            /* fall through */

        case WM_LBUTTONDBLCLK:
toggle_title:
            ClockDisp.bNoTitle = (ClockDisp.bNoTitle ? FALSE : TRUE );
            SetMenuBar( hWnd );
           break;

        case WM_NCHITTEST:
            /* if we have no title/menu bar, clicking and dragging the client
             * area moves the window. To do this, return HTCAPTION.
             * Note dragging not allowed if window maximized, or if caption
             * bar is present.
             */
            wParam = DefWindowProc(hWnd, message, wParam, lParam);
            if( ClockDisp.bNoTitle && (wParam == HTCLIENT) && !IsZoomed(hWnd) )
                return HTCAPTION;
            else
              return wParam;

        case WM_SYSCOLORCHANGE:
            DeleteTools();
            CreateTools();
            break;

        case WM_ERASEBKGND: {
            RECT rect;

            GetClientRect(hWnd, (LPRECT)&rect);
            SelectObject((HDC)wParam, hbrBackground);
            FillRect((HDC)wParam, (LPRECT)&rect, hbrBackground);
            break;
        }

        default:
defproc:
        return(DefWindowProc(hWnd, message, wParam, lParam));
    }
    return(0L);
}


/*
 *  WinMain()
 */

INT PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrev,
LPSTR lpszCmdLine,
INT cmdShow)
{
register HWND hWnd;
MSG           msg;
TIME          nTime;
PSTR          szTooMany;
HMENU         hMenu;
CHAR          szTopmost[80];

    LoadString(hInstance, IDS_USNAME, (LPSTR)szBuffer, BUFLEN);
    LoadString(hInstance, IDS_INIFILE, (LPSTR)szIniFile, 20);
    LoadString(hInstance, IDS_USNAME, (LPSTR)szSection, 30);

    if (!ClockInit(hInstance))
        return(FALSE);

    ClockCreate((HWND)NULL);

    LoadString(hInstance, IDS_APPNAME, (LPSTR)szBuffer, BUFLEN);

    rCoordRect.top=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Top",
                                         (DWORD)-1, (LPSTR)szIniFile);
    rCoordRect.left=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Left",
                                         (DWORD)-1, (LPSTR)szIniFile);
    rCoordRect.right=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Right",
                                         (DWORD)-1, (LPSTR)szIniFile);
    rCoordRect.bottom=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Bottom",
                                         (DWORD)-1, (LPSTR)szIniFile);

    hWnd = CreateWindow((LPSTR)"Clock",    /* The class name.           */
            (LPSTR)szBuffer,               /* The window instance name. */
            WS_TILEDWINDOW,
            (rCoordRect.left < 0) ? CW_USEDEFAULT : rCoordRect.left,
            (rCoordRect.top  < 0) ? CW_USEDEFAULT : rCoordRect.top,
            (rCoordRect.left < 0) ? (INT)( (HorzRes/3) + GetSystemMetrics(SM_CXFRAME)*2 )
                  : rCoordRect.right - rCoordRect.left,
            (rCoordRect.left < 0) ? (INT)( (((HorzRes/3)*aspectN)/aspectD)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFRAME)*2 )
                  : rCoordRect.bottom - rCoordRect.top,
            NULL,
            NULL,
            hInstance,
            (LPSTR)NULL);

    hWindow=hWnd;

    // Loop if control panel time being changed.
    GetTime((TIME *)&nTime);
    do {
        GetTime((TIME *)&oTime);
    } while (nTime.second == oTime.second && nTime.minute == oTime.minute &&
           nTime.hour24 == oTime.hour24);

    if (!SetTimer(hWnd, TimerID, OPEN_TLEN, 0L) )  {
        /* Windows only supports 16 public timers */
        szTooMany = (PSTR)LocalAlloc(LPTR, 160);
        LoadString(hInstance, IDS_TOOMANY, (LPSTR)szTooMany, 160);
        MessageBox((HWND)NULL, (LPSTR)szTooMany, (LPSTR)szBuffer, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
        DeleteTools();
        return(FALSE);
    }

    /* Add the topmost system menu item */
    hMenu = GetSystemMenu( hWnd, FALSE );
    AppendMenu( hMenu, MF_SEPARATOR, 0, NULL );
    LoadString(hInstance, IDS_TOPMOST, szTopmost, 79);

    /* Check the default setting to the clock as topmost or not */
    ClockDisp.bTopMost=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"TopMost",
                                         0, (LPSTR)szIniFile);
    if( ClockDisp.bTopMost ) {
        AppendMenu( hMenu, MF_ENABLED | MF_CHECKED | MF_STRING, IDM_TOPMOST,
                    szTopmost );
        SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
        AppendMenu( hMenu, MF_ENABLED | MF_UNCHECKED | MF_STRING, IDM_TOPMOST,
                    szTopmost );

    /* Check the default menu item either analog or digital */
    CheckMenuItem(GetMenu(hWnd), ClockDisp.wFormat, MF_BYCOMMAND | MF_CHECKED);

    /* Check the default setting to show title bar or not */
    ClockDisp.bNoTitle=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"NoTitle",
                                         0, (LPSTR)szIniFile);
    if( ClockDisp.bNoTitle ) {
        SetMenuBar( hWnd );
    }

    hInst = hInstance;

    /* Check the default minimized state, minimized or not */
    ClockDisp.bMinimized=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Minimized",
                                         0, (LPSTR)szIniFile);
    if (!ClockDisp.bMinimized) {
        ClockDisp.bMaximized=GetPrivateProfileInt((LPSTR)szSection,
                                         (LPSTR)"Maximized",
                                         0, (LPSTR)szIniFile);
        if (ClockDisp.bMaximized)
            ShowWindow(hWnd, SW_MAXIMIZE);
        else {
            ShowWindow(hWnd, cmdShow);
            GetWindowRect(hWnd, (LPRECT)&rCoordRect);
        }
    }
    else
        ShowWindow(hWnd, SW_MINIMIZE);

    DdeInitialize(&idInst, (PFNCALLBACK)MakeProcInstance((FARPROC)DdeCallback, hInstance),
            CBF_FAIL_EXECUTES | CBF_SKIP_ALLNOTIFICATIONS, 0L);
    hszTime = DdeCreateStringHandle(idInst, "Time", 0);
    hszNow = DdeCreateStringHandle(idInst, "Now", 0);
    hszClock = DdeCreateStringHandle(idInst, "Clock", 0);
    DdeNameService(idInst, hszClock, 0L, DNS_REGISTER);



    while (GetMessage((LPMSG)&msg, NULL, 0, 0) ) {
        TranslateMessage((LPMSG)&msg);
        DispatchMessage((LPMSG)&msg);
    }

    DdeUninitialize(idInst);

    return(msg.wParam);
}


/*
 *  GetTime()
 */

VOID GetTime(
TIME *ptime)
{
    time_t t;
    struct tm *ptm;

    time(&t);
    ptm = localtime(&t);
    ptime->second = ptm->tm_sec;
    ptime->minute = ptm->tm_min;
    ptime->hour12 =
    ptime->hour = ptm->tm_hour > 12 ? ptm->tm_hour - 12 : ptm->tm_hour;
    ptime->hour24 = ptm->tm_hour;
    ptime->ampm = ptm->tm_hour > 12 ? 1 : 0;
}


/*
 *  DdeCallback()
 */

HDDEDATA CALLBACK DdeCallback(
WORD usType,
WORD usFmt,
HCONV hConv,
HSZ hsz1,
HSZ hsz2,
HDDEDATA hData,
DWORD lData1,
DWORD lData2)
{

static HANDLE           hToken;
static TOKEN_PRIVILEGES tp;
static LUID             luid;

    if (usType == XTYP_CONNECT) {
        return((HDDEDATA)TRUE);
    }

    if (usType == XTYP_WILDCONNECT) {
        HDDEDATA hData;
        HSZPAIR FAR *phszp;
        DWORD cb;

        if ((!hsz1 || hsz1 == hszTime) && (!hsz2 || hsz2 == hszClock)) {
            if ((hData = DdeCreateDataHandle(idInst, NULL,
                    2 * sizeof(HSZPAIR), 0L, 0, 0, 0))) {
                phszp = (HSZPAIR FAR *)DdeAccessData(hData, &cb);
                phszp[0].hszSvc = hszClock;
                phszp[0].hszTopic = hszTime;
                phszp[1].hszSvc = phszp[1].hszTopic = 0;
                DdeUnaccessData(hData);
                return(hData);
            }
        }
        return(0);
    }

    if (usFmt == CF_TEXT) {
        CHAR sz[40];

        if (usType == XTYP_ADVSTART || usType == XTYP_ADVSTOP) {
            return((HDDEDATA)TRUE);
        }

        if (hsz1 == hszTime && hsz2 == hszNow) {
            if (usType == XTYP_REQUEST || usType == XTYP_ADVREQ) {

                itoa(oTime.hour, sz, 10);
                strcat(sz, ":");
                itoa(oTime.minute, &sz[strlen(sz)], 10);
                strcat(sz, ":");
                itoa(oTime.second, &sz[strlen(sz)], 10);
                return(DdeCreateDataHandle(idInst, (LPBYTE)sz, strlen(sz) + 1, 0L,
                        hszNow, CF_TEXT, 0));
            }
            if (usType == XTYP_POKE) {
                SYSTEMTIME SysTime;

                DdeGetData(hData, (LPBYTE)sz, 40L, 0L);
                GetLocalTime(&SysTime);
                sscanf(sz, "%2d:%2d:%2d", &SysTime.wHour, &SysTime.wMinute, &SysTime.wSecond);

                /* enable system-time privilege, set time, disable privilege */
                OpenProcessToken( GetCurrentProcess(),
                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) ;
                LookupPrivilegeValue( NULL, "SeSystemTimePrivilege", &luid );
                tp.PrivilegeCount           = 1;
                tp.Privileges[0].Luid       = luid;
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                AdjustTokenPrivileges( hToken, FALSE, &tp,
                  sizeof(TOKEN_PRIVILEGES), NULL, NULL );
                SetLocalTime(&SysTime);
                AdjustTokenPrivileges( hToken, TRUE, &tp,
                  sizeof(TOKEN_PRIVILEGES), NULL, NULL );

                DdePostAdvise(idInst, hszTime, hszNow);
                return((HDDEDATA)DDE_FACK);
            }
        }
    }
    return(0);

    UNREFERENCED_PARAMETER(lData1);
    UNREFERENCED_PARAMETER(lData2);
    UNREFERENCED_PARAMETER(hConv);
}
