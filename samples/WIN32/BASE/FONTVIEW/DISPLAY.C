
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <windows.h>
#include <string.h>
#include <math.h>

#if defined (WIN32)
#define _MoveTo(hdc,x,y) MoveToEx(hdc,x,y,NULL)
#else
#define _MoveTo(hdc,x,y) MoveTo(hdc,x,y)
#endif

// These are the 'exported' functions from this file:
void DrawAscii (HDC hdc, RECT *pRect, WORD direction);
void DrawGlyph (HDC hdc, RECT *pRect, BYTE glyph, HFONT hfont);
BYTE FindChar (HDC hdc, RECT *pRect, int x, int y);

#pragma warning(4 : 4309)
// A data structure that DrawAscii will use:
char ascii[]=
    {
    // Low Ascii:

    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,

    // Standard Ascii:

    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
    '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '{', '|', '}', '~', '',

    // High Ascii:

    '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
    '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
    '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
    '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
    '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
    '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
    '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
    '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',

    '\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
    '\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
    '\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
    '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
    '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
    '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
    '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
    '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',


    // End of the list:

    000, 000};
#pragma warning(3 : 4309)

/* A function that the CharSet window will use to display the text */
void DrawAscii (HDC hdc, RECT *pRect, WORD direction)
{
    BYTE *pch;
    int h, w, incx, incy, width;
    unsigned int  wDisplay;
    POINT pt;
    TEXTMETRIC tm;
    BOOL bLineMode = FALSE;


    GetTextMetrics (hdc, &tm);
    h = tm.tmHeight;
    w = tm.tmMaxCharWidth;

    incx = 0;
    incy = h;
    pt.x = 0;
    pt.y = 0;

    //
    pt.y = tm.tmAscent + tm.tmExternalLeading;
    //

    pch = &(ascii[tm.tmFirstChar]);
    wDisplay = pRect->right -pRect->left;

    //
    SetTextAlign (hdc, TA_BASELINE | TA_CENTER);
    SetBkMode (hdc, TRANSPARENT);
    //

    while (pch[0] && (pch[0]<=tm.tmLastChar)) {
#if defined (WIN32)
        SIZE size;
        GetTextExtentPoint (hdc, pch, 1, &size);
        width = size.cx;
#else
        width = LOWORD (GetTextExtent (hdc, pch, 1));
#endif
        if ((unsigned)(pt.x + width) > wDisplay) {
            pt.x = 0;
            pt.y += incy;
        }
        TextOut (hdc, pt.x + (width/2), pt.y, pch, 1);
        pt.x += width;
        pch++;
    }
    return;

    direction;
}

// Return the character that was drawn at the provided location
// This allows us to 'click' on a character, and determined what
// it was we actually clicked on.
BYTE FindChar (HDC hdc, RECT *pRect, int x, int y)
{
    BYTE *pch;
    int h, w, incx, incy, width;
    unsigned int wDisplay;
    POINT pt;
    TEXTMETRIC tm;
    BOOL bLineMode = FALSE;


    GetTextMetrics (hdc, &tm);
    h = tm.tmHeight;
    w = tm.tmMaxCharWidth;

    incx = 0;
    incy = h;
    pt.x = 0;
    pt.y = 0;

    pch = &(ascii[tm.tmFirstChar]);
    wDisplay = pRect->right -pRect->left;

    while (pch[0] && (pch[0]<=tm.tmLastChar)) {
#if defined (WIN32)
        SIZE size;
        GetTextExtentPoint (hdc, pch, 1, &size);
        width = size.cx;
#else
        width = LOWORD (GetTextExtent (hdc, pch, 1));
#endif
        if ((unsigned)(pt.x + width) > wDisplay) {
            pt.x = 0;
            pt.y += incy;
        }
        if ((x>=pt.x) && (x <=(pt.x+width)) && (y>=pt.y) && (y<=pt.y+incy)) return pch[0];
        pt.x += width;
        pch++;
    }
    return 0;
}

// Essentially, a StretchBlt, but draws cicular pels.
BOOL CircleBlt (HDC hdcDest, int xDst, int yDst, int iWidthDst, int iHeightDst, HDC hdcSrc, int xSrc, int ySrc, int iWidthSrc, int iHeightSrc, DWORD dwROP)
{
    int x, y, x1, y1, x2, y2;
    DWORD rgb;
    float fXUnit, fYUnit;
    HBRUSH hbrush, hbrushOld;

    if (iWidthSrc != 0){
        fXUnit = (float)iWidthDst / (float)iWidthSrc;
    } else {
        fXUnit = (float)iWidthDst;
    }
    if (iHeightSrc != 0) {
        fYUnit = (float)iHeightDst / (float)iHeightSrc;
    } else {
        fYUnit = (float)iHeightDst;
    }

    for (y=ySrc; y<(ySrc+iHeightSrc); y++) {
        for (x=xSrc; x<(xSrc+iWidthSrc); x++) {

            rgb = GetPixel (hdcSrc, x, y);
            if (rgb != -1 && rgb != 0x00FFFFFF) {
                hbrush = CreateSolidBrush (rgb);
                hbrushOld = SelectObject (hdcDest, hbrush);
                x1 = xDst + (int)((x-xSrc)*fXUnit);
                y1 = yDst + (int)((y-ySrc)*fYUnit);
                x2 = xDst + (int)(((x-xSrc)+1)*fXUnit)+1;
                y2 = yDst + (int)(((y-ySrc)+1)*fYUnit)+1;
                Ellipse (hdcDest, x1, y1, x2, y2);
                DeleteObject(SelectObject(hdcDest, hbrushOld));
            }
        }
    }

    return TRUE;

    dwROP;
}

double Radian(double ang)
{
        return ang * (double)3.1415926535 / (double)180.0;
}

void CalcRot (double x, double y, double ang, double* x1, double* y1)
{
        ang = Radian(-ang);
        *x1 = (x * cos(ang)) - (y * sin(ang));
        *y1 = (x * sin(ang)) + (y * cos(ang));
}

void DrawGlyph (HDC hdc, RECT *pRect, BYTE glyph, HFONT hfont)
{
    TEXTMETRIC tm;
    LOGFONT lf;
    int wChar, h1, h2, w1, w2, x, y;
    double xTL, yTL, xTR, yTR, xBL, yBL, xBR, yBR, xML, yML, xMR, yMR;
    int cWidth, cExtentW, cExtentH;
    int unit, margin;
    RECT rect, rectChar;
    HDC hdcMem;
    HBITMAP hbitmap, hbitmapOld;
    HFONT hfontOld, hfontTmp;
    char szText[80];
    double fAng, fWa, fWb, fHa, fHb, fHeight, fWidth;
    BOOL bRotateable;
    double fxOff, fyOff;
#if defined(TT_AVAILABLE)
    ABC abc;
#endif

    GetTextMetrics (hdc, &tm);
    hfontTmp = SelectObject (hdc, GetStockObject(SYSTEM_FONT));
    GetObject (hfontTmp, sizeof(LOGFONT), &lf);

// Some fields of 'lf' need to be whacked out depending on the type of
// font involved. For instance, lfEscapement should only be non-zero for
// rotatable fonts.
        bRotateable = FALSE;
#if defined (TMPF_VECTOR)
        bRotateable = (tm.tmPitchAndFamily & TMPF_VECTOR);
#endif
#if defined (TMPF_TRUETYPE)
        bRotateable |= (tm.tmPitchAndFamily & TMPF_TRUETYPE);
#endif
        if (!bRotateable) {
                lf.lfEscapement = 0;
        }

    SelectObject (hdc, hfontTmp);
    GetCharWidth (hdc, glyph, glyph, &cWidth);

#if defined (WIN32)
    {
        SIZE size;
        GetTextExtentPoint (hdc, (LPSTR)&glyph, 1, &size);
        cExtentW = size.cx;
        cExtentH = size.cy;
    }
#else
    cExtentW = LOWORD (GetTextExtent (hdc, (LPSTR)&glyph, 1));
    cExtentH = HIWORD (GetTextExtent (hdc, (LPSTR)&glyph, 1));
#endif

    wChar = max(cWidth, cExtentW);

#if defined(TT_AVAILABLE)
    if (GetCharABCWidths (hdc, (UINT)glyph, (UINT)glyph, &abc)) {
        wChar = abc.abcA + abc.abcB + abc.abcC;
        wChar = max(abc.abcA,0) + max (abc.abcB,0) + max (abc.abcC,0);
    } else {
        abc.abcA = 0;
        abc.abcB = wChar;
        abc.abcC = 0;
    }
#endif

// ****
        // Lets figure out how large of a bitmap we need to draw this character into.
        // This will take lfEscapement into account.

        // Turn the 'escapement' value into a valid angle
    fAng = 90.0 - ((double)lf.lfEscapement/10.0);

    // We now need to calculate two values for height and width. These
    // represent the bases of the triangles that are formed as the
    // character cell rotates. Thus fWa+fWb is the 'width' of the space
    // required by the given character at the current rotation.
    // At 0, 90, 180, 270 and 360, one of the two values will be '0'.
    fWa = (double)cExtentW * sin(Radian(fAng));
    fHa = (double)cExtentW * sin(Radian((double)lf.lfEscapement/10.0));
    fHb = (double)cExtentH * sin(Radian(fAng));
    fWb = (double)cExtentH * sin(Radian((double)lf.lfEscapement/10.0));

    // This gives us a width/height of:
    fHeight = fabs(fHa) + fabs(fHb);
    fWidth = fabs(fWa) + fabs(fWb);
// ****

    h1 = pRect->bottom - pRect->top;
    h2 = tm.tmHeight + tm.tmExternalLeading;
    h2 = (int)fHeight;
    if (h2 == 0) {
        MessageBeep (0);
        h2 = 10;
    }

    w1 = pRect->right - pRect->left;
    w2 = (int)fWidth;

    unit = (h1 / h2);

    margin = (h1 - (unit * h2)) / 2;
    margin = 0;
    // Define a rectangle that will enclose our drawing area
    SetRect (&rect, 0, 0, wChar*unit, (tm.tmHeight*unit) + (tm.tmExternalLeading*unit));
    OffsetRect (&rect, margin, margin);
    // Define a rectangle that will define the bounding box of the character
    SetRect (&rectChar, 0, 0, wChar*unit, tm.tmHeight*unit);
    OffsetRect (&rectChar, margin, margin + (tm.tmExternalLeading*unit));


// ****
    // These are floating point values.
    SetRect (&rect, 0, 0, (int)(fWidth)*unit, ((int)(fHeight)*unit) + (tm.tmExternalLeading*unit));
    OffsetRect (&rect, margin, margin);
    // Define a rectangle that will define the bounding box of the character
    SetRect (&rectChar, 0, 0, (int)(fWidth)*unit, ((int)(fHeight)*unit));
    OffsetRect (&rectChar, margin, margin + (tm.tmExternalLeading*unit));

    // Calculate the points of the 'character cell'. This will take full
    // rotation into consideration.
        CalcRot (0, 0, (double)lf.lfEscapement/10.0, &xTL, &yTL);
        CalcRot ((double)cExtentW, 0, (double)lf.lfEscapement/10.0, &xTR, &yTR);
        CalcRot (0, (double)cExtentH, (double)lf.lfEscapement/10.0, &xBL, &yBL);
        CalcRot ((double)cExtentW, (double)cExtentH, (double)lf.lfEscapement/10.0, &xBR, &yBR);
        CalcRot (0, (double)tm.tmAscent, (double)lf.lfEscapement/10.0, &xML, &yML);
        CalcRot ((double)cExtentW, (double)tm.tmAscent, (double)lf.lfEscapement/10.0, &xMR, &yMR);

        fxOff = min(xTL, min(xTR, min(xBL, xBR)));
        fyOff = min(yTL, min(yTR, min(yBL, yBR)));

// Draw a rectangle around the drawing limits of the character.
    _MoveTo (hdc, (int)((xTL-fxOff)*(double)unit), (int)((yTL-fyOff)*(double)unit));
    LineTo (hdc, (int)((xTR-fxOff)*(double)unit), (int)((yTR-fyOff)*(double)unit));
    LineTo (hdc, (int)((xBR-fxOff)*(double)unit), (int)((yBR-fyOff)*(double)unit));
    LineTo (hdc, (int)((xBL-fxOff)*(double)unit), (int)((yBL-fyOff)*(double)unit));
    LineTo (hdc, (int)((xTL-fxOff)*(double)unit), (int)((yTL-fyOff)*(double)unit));

// Draw the baseline, which indicates the advance width of the character
    _MoveTo (hdc, (int)((xML-fxOff)*(double)unit), (int)((yML-fyOff)*(double)unit));
    LineTo (hdc, (int)((xMR-fxOff)*(double)unit), (int)((yMR-fyOff)*(double)unit));

    hdcMem = CreateCompatibleDC (hdc);
    if (hdcMem) {
        hbitmap = CreateCompatibleBitmap (hdc, (int)(fWidth), (int)(fHeight));
        if (hbitmap) {
            hbitmapOld = SelectObject (hdcMem, hbitmap);
            BitBlt (hdcMem, 0, 0, (int)(fWidth), (int)(fHeight), NULL, 0, 0, WHITENESS);
            hfontOld = SelectObject (hdcMem, hfont);

            SetTextAlign (hdcMem, TA_LEFT | TA_BASELINE);
            if (TextOut (hdcMem, (int)(xML-fxOff), (int)(yML-fyOff), (LPSTR)&glyph, 1)) {


                                /*
                // Just for debugging, lets mark some special pixels
                        SetPixel (hdcMem, (int)(xML-fxOff), (int)(yML-fyOff), 0x808080); // Base Lft
                        SetPixel (hdcMem, (int)(xTL-fxOff), (int)(yTL-fyOff), 0x808080); // Top Left
                        SetPixel (hdcMem, (int)(xTR-fxOff), (int)(yTR-fyOff), 0x808080); // Top Right
                        SetPixel (hdcMem, (int)(xBL-fxOff), (int)(yBL-fyOff), 0x808080); // Btm Left
                        SetPixel (hdcMem, (int)(xBR-fxOff), (int)(yBR-fyOff), 0x808080); // Btm Right
                        SetPixel (hdcMem, (int)(xML-fxOff), (int)(yML-fyOff), 0x808080); // Base Lft
                        SetPixel (hdcMem, (int)(xMR-fxOff), (int)(yMR-fyOff), 0x808080); // Base Rt
                */

                CircleBlt(hdc, margin, margin,
                    rectChar.right-rectChar.left, rectChar.bottom-rectChar.top,
                    hdcMem, 0, 0, (int)(fWidth), (int)(fHeight), SRCPAINT);

            } else {
                MessageBox (GetFocus(),
                        "Unable to perform TextOut", "DisplayGlyph", MB_OK);
            }
            SelectObject (hdcMem, hbitmapOld);
            SelectObject (hdcMem, hfontOld);
            DeleteObject (hbitmap);
        } else {
            MessageBox (GetFocus(), "Unable To create Bitmap", "DisplayGlyph", MB_OK);
        }
        DeleteDC (hdcMem);
    } else {
        MessageBox (GetFocus(), "Unable to create DC", "DisplayGlyph", MB_OK);
    }

    hfontTmp = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "");

    if (hfontTmp) {
        hfontOld = SelectObject (hdc, hfontTmp);
        x = rect.right + 10;
        GetTextMetrics (hdc, &tm);

        y = tm.tmHeight + tm.tmExternalLeading;

        y += tm.tmHeight + tm.tmExternalLeading;
        wsprintf (szText, "Character = '%c' %u (0x%X)", (char)glyph, (int)glyph, (int)glyph);
        TextOut (hdc, x, y, szText, lstrlen(szText));

        y += tm.tmHeight + tm.tmExternalLeading;
        wsprintf (szText, "CharWidth = %i", cWidth);
        TextOut (hdc, x, y, szText, lstrlen(szText));

                // ABC Width - Eventually, I want to 'draw' this into the character
                // cell, but for now, this at least makes the information easily
                // available:
#if defined (TT_AVAILABLE)
        y += tm.tmHeight + tm.tmExternalLeading;
        wsprintf (szText, "A|B|C = %i | %u | %i", abc.abcA, abc.abcB, abc.abcC);
        TextOut (hdc, x, y, szText, lstrlen(szText));
#endif

        y += tm.tmHeight + tm.tmExternalLeading;
        wsprintf (szText, "CharExtentW = %i", cExtentW);
        TextOut (hdc, x, y, szText, lstrlen(szText));

        y += tm.tmHeight + tm.tmExternalLeading;
        wsprintf (szText, "CharExtentH = %i", cExtentH);
        TextOut (hdc, x, y, szText, lstrlen(szText));

                // These are for some values I was using to debug the character
                // cell rotation calculation:

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "(%i,%i) : (%i,%i) [%i]", (int)xTL, (int)yTL, (int)xTR, (int)yTR,(int)(((double)lf.lfEscapement/10.0)*10));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "(%i,%i) : (%i,%i)", (int)xML, (int)yML, (int)xMR, (int)yMR);
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "(%i,%i) : (%i,%i)", (int)xBL, (int)yBL, (int)xBR, (int)yBR);
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fAng = %i", (int)(fAng));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fHa = %i", (int)(fHa));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fHb = %i", (int)(fHb));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fWa = %i", (int)(fWa));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fWb = %i", (int)(fWb));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fWd = %i", (int)(fWd));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fHd = %i", (int)(fHd));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fHeight = %i", (int)(fHeight));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        //y += tm.tmHeight + tm.tmExternalLeading;
        //wsprintf (szText, "fWidth = %i", (int)(fWidth));
        //TextOut (hdc, x, y, szText, lstrlen(szText));

        SelectObject (hdc, hfontOld);
        DeleteObject (hfontTmp);
    }
}
