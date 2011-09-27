//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (C) 1994-1995  Microsoft Corporation.  All Rights Reserved.
//
//==========================================================================;

#include "glos.h"
#include <GL/gl.h>
#include <GL/glaux.h>

void myinit(void);
void makeStringFont(char *);
void printString(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

#define JPN_TEXT_LIST 1


void getBitmap(BITMAP *bmp, LONG width, LONG height)
{
    FillMemory(bmp, sizeof(*bmp), 0);
    bmp->bmWidth = width;
    bmp->bmHeight = height;
    bmp->bmWidthBytes = ((width + 7) / 8 + 1) & (~1);
    bmp->bmPlanes = 1;
    bmp->bmBitsPixel = 1;
    bmp->bmBits = GlobalAlloc(GMEM_FIXED, bmp->bmWidthBytes*height);
}


char *getGLmonoBits(HDC hDC, HBITMAP hBmp, int* size)
{
    BITMAP bi;
    char   *bits;
    struct {
        BITMAPINFOHEADER bih;
        RGBQUAD col[2];
    }bic;
    BITMAPINFO *binf = (BITMAPINFO *)&bic;

    GetObject(hBmp, sizeof(bi), &bi);
    *size = bi.bmHeight*(((bi.bmWidth + 31) & (~31)) / 8);
    bits = (char *)GlobalAlloc(GPTR, *size);

    binf->bmiHeader.biSize = sizeof(binf->bmiHeader);
    binf->bmiHeader.biWidth = bi.bmWidth;
    binf->bmiHeader.biHeight = bi.bmHeight;
    binf->bmiHeader.biPlanes = 1;
    binf->bmiHeader.biBitCount = 1;
    binf->bmiHeader.biCompression = BI_RGB;
    binf->bmiHeader.biSizeImage = *size;
    binf->bmiHeader.biXPelsPerMeter = 1;
    binf->bmiHeader.biYPelsPerMeter = 1;
    binf->bmiHeader.biClrUsed = 0;
    binf->bmiHeader.biClrImportant = 0;

    GetDIBits(hDC, hBmp, 0, bi.bmHeight, bits, binf, DIB_RGB_COLORS);

    return bits;
}


char *createStringBitmapFont(HDC hDC, HFONT hFont, char *str, PSIZEL size)
{
    BITMAP  bmp;
    HBITMAP hbmp;
    char    *pbm;
    int     len = lstrlen(str);
    HFONT   hFontOld = SelectObject(hDC, hFont);

    GetTextExtentPoint32(hDC, str, len, size);
    getBitmap(&bmp, size->cx, size->cy);
    hbmp = CreateBitmapIndirect(&bmp);
    GlobalFree(bmp.bmBits);
    if(hbmp){
        HDC hMemDC = CreateCompatibleDC(hDC);
        if(hMemDC){
            HBITMAP hPrevBmp = SelectObject(hMemDC, hbmp);
            HFONT   hPrevFont;
            int     size0;
            BITMAP  bi;

            SetBkColor(hMemDC, RGB(0,0,0));
            SetTextColor(hMemDC, RGB(255,255,255));
            SetBkMode(hMemDC, OPAQUE);
            hPrevFont = SelectObject(hMemDC, hFont);
            TextOut(hMemDC, 0, 0, str, len);

            GetObject(hbmp, sizeof(bi), &bi);
            pbm = getGLmonoBits(hMemDC, hbmp, &size0);
            size->cx = ((bi.bmWidth+31)&(~31));
            size->cy = bi.bmHeight;

            SelectObject(hMemDC, hPrevFont);
            SelectObject(hMemDC, hPrevBmp);
            DeleteDC(hMemDC);
        }
        else{
            DeleteObject(hbmp);
        }
    }
    SelectObject(hDC, hFontOld);
    return pbm;
}


void makeStringFont(char *str)
{
    SIZE    size;
    LOGFONT lf;
    HFONT   hFont;
    char    *lpBitmap;
    HDC     hDC = GetDC(GetFocus());

    FillMemory(&lf, sizeof(lf), 0);
    lf.lfHeight = 12 * GetDeviceCaps(hDC, LOGPIXELSY) / 72;
    lf.lfCharSet = SHIFTJIS_CHARSET;
    lf.lfWeight = 400;
    lf.lfOutPrecision = OUT_STROKE_PRECIS;
    lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
    lstrcpy(lf.lfFaceName, "‚l‚r ƒSƒVƒbƒN");
    hFont = CreateFontIndirect(&lf);

    lpBitmap = createStringBitmapFont(hDC, hFont, str, &size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glNewList(JPN_TEXT_LIST, GL_COMPILE);
    glBitmap(size.cx, size.cy, 0.0, 2.0, size.cx+2, 0.0, lpBitmap);
    glEndList();

    GlobalFree(lpBitmap);
}


void myinit(void)
{
    glShadeModel (GL_FLAT);
    makeStringFont("ABC±²³‚ ‚¢‚¤Š¿Žš");
}


void printString(void)
{
    glPushAttrib (GL_LIST_BIT);
    glCallList(JPN_TEXT_LIST);
    glPopAttrib ();
}


void CALLBACK display(void)
{
    GLfloat white[3] = { 1.0, 1.0, 1.0 };

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3fv(white);
    glRasterPos2i(20, 200);
    printString();
    glFlush ();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0, w, 0.0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 400, 400);
    auxInitWindow ("Japanese Text");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
