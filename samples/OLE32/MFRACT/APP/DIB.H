/****************************************************************************
 *                                                                          *
 *  FILE    : DIB.H                                                         *
 *                                                                          *
 *  DESCRIPTION : Header/include file for DIB.C                             *
 *                                                                          *
 ****************************************************************************/

// This is code that I lifted years ago from the Windows SDK and have since
// ported over to NT.
//      -- stevebl

#ifndef __DIB_H__
#define __DIB_H__
#ifdef __cplusplus
extern "C" {
#endif

/* Macros to display/remove hourglass cursor for lengthy operations */
extern HCURSOR hcurSave;
#define StartWait() hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT))
#define EndWait()   SetCursor(hcurSave)

/* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0

#define MAXREAD  32768             /* Number of bytes to be read during */
                       /* each read operation.          */

/* Header signatutes for various resources */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((i+31)/32*4)

#define PALVERSION  0x300
#define MAXPALETTE  256   /* max. # supported palette entries */


/***************** FUNCTION DECLARATIONS *******************/

DWORD PASCAL lread(int fh, VOID FAR *pv, DWORD ul);
DWORD PASCAL lwrite(int fh, VOID FAR *pv, DWORD ul);

/***********************************************************/
/* Declarations of functions used in dib.c module      */
/***********************************************************/

HANDLE      OpenDIB (LPWSTR szFile);
BOOL        WriteDIB (LPWSTR szFile,HANDLE hdib);
WORD        PaletteSize (VOID FAR * pv);
WORD        DibNumColors (VOID FAR * pv);
HPALETTE    CreateDibPalette (HANDLE hdib);
HPALETTE    CreateBIPalette (LPBITMAPINFOHEADER lpbi);
HANDLE      DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal);
HBITMAP     BitmapFromDib (HANDLE hdib, HPALETTE hpal);
BOOL        DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);
BOOL        StretchDibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, int dx1, int dy1, LONG rop);
BOOL        DibInfo (HANDLE hdib,LPBITMAPINFOHEADER lpbi);
HANDLE      ReadDibBitmapInfo (int fh);
BOOL        DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);
#ifdef __cplusplus
}
#endif
#endif // __DIB_H__
