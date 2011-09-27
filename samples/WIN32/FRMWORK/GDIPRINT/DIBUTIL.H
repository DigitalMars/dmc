// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for DIB-related functions in dibutil.c
//    These routines are taken from the WINCAP32 sample

#define PALVERSION            0x300
#define HDIB                  HANDLE           // handle to a DIB
#define IS_WIN30_DIB(lpbi)    ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.
#define WIDTHBYTES(bits)      ((((bits) + 31) & ~31) >> 3)
 
// Function prototypes
HPALETTE CreateDIBPalette(HDIB);
void DIBError(int);
DWORD DIBHeight(LPSTR);
WORD DIBNumColors(LPSTR);
LPSTR FindDIBBits(LPSTR);
WORD PaletteSize(LPSTR);
WORD SaveDIB(HDIB, LPVOID, LPSTR);
BOOL PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE); 
int  FillColorTable(HWND, LPRGBQUAD, HPALETTE, int);

 
