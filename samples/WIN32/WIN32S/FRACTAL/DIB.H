
#ifndef _DIB_H
#define _DIB_H
#if 0
// __cplusplus
extern "C" {
#endif
/* Macros to display/remove hourglass cursor for lengthy operations */
#define StartWait() hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
#define EndWait()   SetCursor(hcurSave)

/* Header signatutes for various resources */
#define BFT_ICON			 0x4349	/* 'IC' */
#define BFT_BITMAP 0x4d42			/* 'BM' */
#define BFT_CURSOR 0x5450			/* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* Macro to align given value to the closest DWORD (unsigned long ) */
#define ALIGNULONG(i)	((i+3)/4*4)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)	((i+31)/32*4)

#define PALVERSION				  0x300
#define MAXPALETTE		256		 /* max. # supported palette entries */

/*****************************************************/
/* make new sizeof structs to cover NT/32s alignment */
/*****************************************************/

#define SIZEOF_BITMAPFILEHEADER_PACKED  (   \
    sizeof(WORD) +      /* bfType      */   \
    sizeof(DWORD) +     /* bfSize      */   \
    sizeof(WORD) +      /* bfReserved1 */   \
    sizeof(WORD) +      /* bfReserved2 */   \
    sizeof(DWORD))      /* bfOffBits   */

/***********************************************************/
/* Declarations of functions used in dib.c module		   */
/* Modified from Martin Heller's _Advanced Windows         */
/* Programming_, originally from SDK example               */
/***********************************************************/

HANDLE	    OpenDIB (LPSTR szFile);
BOOL		WriteDIB (LPSTR szFile,HANDLE hdib);
WORD		PaletteSize (VOID FAR * pv);
WORD		DibNumColors (VOID FAR * pv);
HPALETTE 	CreateDibPalette (HANDLE hdib);
HPALETTE 	CreateBIPalette (LPBITMAPINFOHEADER lpbi);
HANDLE		DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal);
HANDLE		BitmapFromDib (HANDLE hdib, HPALETTE hpal);
BOOL		DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);
BOOL		StretchDibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, int dx1, int dy1, LONG rop);
BOOL		DibInfo (HANDLE hdib,LPBITMAPINFOHEADER lpbi);
HANDLE		ReadDibBitmapInfo (int fh);
BOOL		DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);

// Useful functions (tl)
BOOL        GetAllBitmapInfo(const char * pszDib, HANDLE &hDib, 
                             HPALETTE &hpalDib, HBITMAP &hBm);

HPALETTE    DuplicatePalette(HPALETTE hpal);

HANDLE SimplePaletteConform(HANDLE hDib, HPALETTE hpalConformTo);

// Add system colors to start of palette, fill the rest with what's in
// hpal, use bitmap to identify important colors if there's not enough
// space. bitmap can be null (tl)

HPALETTE CreateOptimized256Palette(HPALETTE hpal, HBITMAP hBmForColorQuant);

// Create a unified palette with the properies of an optimized palette, plus
// any available slots filled with colors from the submissive (tl)

HPALETTE CreateUnified256Palette(HPALETTE hpalDominant, 
                                 HPALETTE hpalSubmissive,
                                 HBITMAP  hBmForDominantColorQuant,
                                 HBITMAP  hBmForSubmissiveColorQuant);
#if 0
//__cplusplus
};
#endif
#endif
