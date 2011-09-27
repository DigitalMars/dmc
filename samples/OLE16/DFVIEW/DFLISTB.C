/*************************************************************************
**
**    OLE 2 DFVIEW Sample Code
**
**    dflistb.c
**
**    This file contains routines which interface DFVIEW to the
**    owner-draw listbox code in foldlist.c.
**
**    (c) Copyright Microsoft Corp. 1990 - 1992 All Rights Reserved
**
*************************************************************************/

#include "precomp.h"
#include <assert.h>
#include <string.h>
#include "dfview.h"
#include "foldlist.h"
#include "resource.h"

/*************************** Magic #'s *********************************/
#define DSPDxax   0x00E20746L       // A nice ROP code

/********************** Locally Used "Globals" *************************/
extern HWND ghFoldList;
extern HANDLE ghInst;
int xoff, yoff;
HBITMAP           ghbmpFolders   = NULL;    // Folder's bitmap
HBITMAP           ghbmpBullets   = NULL;    // 3-D Bullet bitmap
HBITMAP           ghbmpWindows   = NULL;    // Window guy bitmap
HBITMAP           ghbmpBlanks    = NULL;    // Window guy bitmap
HBITMAP           ghbmpDocuments = NULL;    // Window guy bitmap


/********************** Locally used Functions *************************/
BOOL DDBChangeColor (HBITMAP hBitmap, COLORREF crFrom, COLORREF crTo);
static BOOL LoadBitmaps (void);
BOOL LoadAndStretch (HBITMAP hbmpDest, LPCSTR lpszResource, UINT cxBitmap,
					 UINT cyBitmap, HDC hdcDest, HDC hdcSrc,
					 COLORREF crHigh, COLORREF crNorm);


/************************************************************************
**  Function: DFListBoxCreate
**
**  Purpose : Creates a FOLDLIST owner-draw listbox which completely
**            covers the client area of hWndParent.  The x and y
**            parameters are used to offset the listbox within the client
**            area.
**
**  Returns : 0 on success.
**            -1 on failure.
**
************************************************************************/

long DFListBoxCreate(HWND hWndParent, int x, int y)
{
   RECT               rcWnd;

   GetClientRect (hWndParent, &rcWnd);
   xoff = x;
   yoff = y;

   ghFoldList = CreateWindow (FLClassName, "",
							  WS_CHILD | WS_VISIBLE,
							  x, y, rcWnd.right, rcWnd.bottom,
							  hWndParent, 5,
							  ghInst,
							  NULL);
   if (!ghFoldList)
	  {
	  assert (FALSE);
	  return -1;        //!!!! Message box??? !!!!
	  }


   /*
   **  Load bitmaps we use for our folders/items.  Note:  LoadBitmaps
   **  requires ghFoldList/ghInst to be set.
   */
   if (!LoadBitmaps ())
	  {
	  assert (FALSE);
	  return -1;
	  }

   return 0;
}


/************************************************************************
**  Function: DFListBoxCleanup
**
**  Purpose : Cleans up after ourselves.
**
**  Parms   : None.
**
**  Returns : None.
**
************************************************************************/

void DFListBoxCleanup (void)
{
   DeleteObject (ghbmpFolders);
   DeleteObject (ghbmpBullets);
   DeleteObject (ghbmpWindows);
   DeleteObject (ghbmpBlanks);
   DeleteObject (ghbmpDocuments);

}


/************************************************************************
**  Function: LoadBitmaps
**
**  Purpose : Loads the bitmaps our app uses.  !!NOTE!! Uses global
**            variables, for simplicity's sake.  Requires ghFoldList
**            and ghInst to have been set prior to being called.
**
**  Parms   : None -- Uses GLOBALS:  ghFoldList and ghInst
**
**  Returns : TRUE  == Success.
**            FALSE == Failure.
**
************************************************************************/
static BOOL LoadBitmaps (void)
{
   HDC      hdcDest  = NULL;
   HDC      hdcSrc   = NULL;
   HBITMAP  hbmpOld1 = NULL;
   HBRUSH   hbr, hbrOld;
   HPEN     hpenOld;
   int      cx, cy, x, y, i;
   UINT     cxBitmap, cyBitmap;
   COLORREF crNorm, crHigh;
   LONG     lbmSize;

   /*
   **  Get some metrics/info from the FoldList.
   */
   lbmSize  = SendMessage (ghFoldList, WM_FLGETSIZEFOLDERS,  0, 0L);
   cxBitmap = LOWORD (lbmSize);
   cyBitmap = HIWORD (lbmSize);
   crHigh   = (COLORREF) SendMessage (ghFoldList, WM_FLGETCOLORHIGH, 0, 0L);
   crNorm   = (COLORREF) SendMessage (ghFoldList, WM_FLGETCOLORNORM, 0, 0L);

   /*
   **  We'll need some DC's and some new bitmaps to play with.
   */
   hdcDest = CreateCompatibleDC (NULL);
   hdcSrc  = CreateCompatibleDC (NULL);
   if (!hdcDest || !hdcSrc)
	  goto ERRLOADBITMAP;

   ghbmpFolders = CreateBitmap (4*cxBitmap, cyBitmap,
								GetDeviceCaps (hdcDest, PLANES),
								GetDeviceCaps (hdcDest, BITSPIXEL), NULL);
   ghbmpBullets = CreateBitmap (4*cxBitmap, cyBitmap,
								GetDeviceCaps (hdcDest, PLANES),
								GetDeviceCaps (hdcDest, BITSPIXEL), NULL);
   ghbmpWindows = CreateBitmap (4*cxBitmap, cyBitmap,
								GetDeviceCaps (hdcDest, PLANES),
								GetDeviceCaps (hdcDest, BITSPIXEL), NULL);
   ghbmpBlanks = CreateBitmap (4*cxBitmap, cyBitmap,
								GetDeviceCaps (hdcDest, PLANES),
								GetDeviceCaps (hdcDest, BITSPIXEL), NULL);
   ghbmpDocuments = CreateBitmap (4*cxBitmap, cyBitmap,
								GetDeviceCaps (hdcDest, PLANES),
								GetDeviceCaps (hdcDest, BITSPIXEL), NULL);

   if (!ghbmpFolders || !ghbmpBullets || !ghbmpWindows || !ghbmpBlanks || !ghbmpDocuments)
	  goto ERRLOADBITMAP;


   /*
   **  Load our folders&windows bitmaps, and munge them to create
   **  our 4-folder bitmap "arrays".  Anything RED in the loaded
   **  bitmap will be considered "clear."
   */
   if (!LoadAndStretch (ghbmpFolders, MAKEINTRESOURCE(BMP_FOLDERS), cxBitmap,
						cyBitmap, hdcDest, hdcSrc, crHigh, crNorm))
	  goto ERRLOADBITMAP;

   if (!LoadAndStretch (ghbmpWindows, MAKEINTRESOURCE(BMP_WINDOWS), cxBitmap,
						cyBitmap, hdcDest, hdcSrc, crHigh, crNorm))
	  goto ERRLOADBITMAP;

   if (!LoadAndStretch (ghbmpBlanks, MAKEINTRESOURCE(BMP_BLANKS), cxBitmap,
						cyBitmap, hdcDest, hdcSrc, crHigh, crNorm))
	  goto ERRLOADBITMAP;

   if (!LoadAndStretch (ghbmpDocuments, MAKEINTRESOURCE(BMP_DOCUMENTS), cxBitmap,
						cyBitmap, hdcDest, hdcSrc, crHigh, crNorm))
	  goto ERRLOADBITMAP;


   /*
   **   Create our "bullet" bitmap.  !!!NOTE!!!  This looks highly
   **   eneficient and sloppy.  Let's see if we can clean it up.
   */

   hbmpOld1 = SelectObject (hdcDest, ghbmpBullets);
   hbr      = CreateSolidBrush (crHigh);
   hbrOld   = SelectObject (hdcDest, hbr);

   PatBlt (hdcDest, 0, 0, cxBitmap * 2, cyBitmap, PATCOPY);
   SelectObject (hdcDest, hbrOld);
   DeleteObject (hbr);

   hbr = CreateSolidBrush (crNorm);
   SelectObject (hdcDest, hbr);
   PatBlt (hdcDest, cxBitmap*2, 0, cxBitmap*2, cyBitmap, PATCOPY);
   SelectObject (hdcDest, hbrOld);
   DeleteObject (hbr);

   cx      = cxBitmap / 2;
   cy      = cyBitmap / 2;
   x       = (cxBitmap - cx) / 2;
   y       = (cyBitmap - cy) / 2;
   hpenOld = SelectObject (hdcDest, GetStockObject (NULL_PEN));
   hbr     = CreateSolidBrush (GetSysColor (COLOR_BTNSHADOW));
   SelectObject (hdcDest, hbr);
   for (i=0;  i < 4;  i++)
	  Ellipse (hdcDest, x + i*cxBitmap + GetSystemMetrics (SM_CXBORDER),
			   y + GetSystemMetrics (SM_CYBORDER),
			   x + i*cxBitmap + GetSystemMetrics (SM_CXBORDER) + cx,
			   y + GetSystemMetrics (SM_CYBORDER) + cy);
   SelectObject (hdcDest, hbrOld);
   DeleteObject (hbr);

   hbr     = CreateSolidBrush (GetSysColor (COLOR_BTNHIGHLIGHT));
   SelectObject (hdcDest, hbr);
   for (i=0;  i < 4;  i++)
	  Ellipse (hdcDest, x + i*cxBitmap - GetSystemMetrics (SM_CXBORDER),
			   y - GetSystemMetrics (SM_CYBORDER),
			   x + i*cxBitmap - GetSystemMetrics (SM_CXBORDER) + cx,
			   y - GetSystemMetrics (SM_CYBORDER) + cy);
   SelectObject (hdcDest, hbrOld);
   DeleteObject (hbr);

   hbr     = CreateSolidBrush (GetSysColor (COLOR_BTNFACE));
   SelectObject (hdcDest, hbr);
   for (i=0;  i < 4;  i++)
	  Ellipse (hdcDest, x + i*cxBitmap, y, x + i*cxBitmap + cx, y + cy);
   SelectObject (hdcDest, hbrOld);
   DeleteObject (hbr);

   SelectObject (hdcDest, hpenOld);

   SelectObject (hdcDest, hbmpOld1);
   DeleteDC     (hdcDest);
   DeleteDC     (hdcSrc);
   return TRUE;

ERRLOADBITMAP:
   /*
   ** Bitmap loading error.
   */
   DeleteDC     (hdcDest);
   DeleteDC     (hdcSrc);
   DeleteObject (ghbmpFolders);
   DeleteObject (ghbmpBullets);
   DeleteObject (ghbmpWindows);

   ghbmpFolders = NULL;
   ghbmpBullets = NULL;
   ghbmpWindows = NULL;

   return FALSE;
}



/************************************************************************
**  Function: LoadAndStretch
**
**  Purpose : Loads a bitmap from our resources, and creates a bitmap
**            "array."  The red portions of the bitmap are considered
**            clear.  Each resource consists of two bitmaps side by
**            side:  an "open folder" and a "closed folder".  These
**            bitmaps are turned into 4 bitmaps:  two selected, and
**            two unselected bitmaps.
**
**  Parms   : hbmpDest     -- Where we're putting our resultant
**                             bitmap array.
**            lpszResource -- Name of our two bitmap array in our
**                             RC file.
**            cxBitmap     -- Width to stretch to.
**            cyBitmap     -- Height to stretch to.
**            hdcDest      -- Two scratch memory DCs to do our work in.
**            hdcSrc           We pass them in for speed/laziness
**                             reasons.
**            crHigh       -- Highlight color for our two "highlight"
**                             bitmaps.
**            crNorm       -- Normal color for our two "normal" bitmaps.
**
**  Returns : TRUE  == Success.
**            FALSE == Failure.
**
************************************************************************/

BOOL LoadAndStretch (HBITMAP hbmpDest, LPCSTR lpszResource, UINT cxBitmap,
					 UINT cyBitmap, HDC hdcDest, HDC hdcSrc,
					 COLORREF crHigh, COLORREF crNorm)
{
   HBITMAP hbmpOld1, hbmpOld2, hbmp;
   BITMAP  bm;
   int     i;

   for (i=0;  i < 2;  i++)
	  {
	  hbmp = LoadBitmap (ghInst, lpszResource);
	  if (!hbmp)
		 return FALSE;

	  DDBChangeColor (hbmp, RGB (255,0,0), ((i == 0) ? crHigh : crNorm));
	  GetObject      (hbmp, sizeof (bm), &bm);

	  hbmpOld1 = SelectObject (hdcDest, hbmpDest);
	  hbmpOld2 = SelectObject (hdcSrc,  hbmp);

	  SetStretchBltMode (hdcDest, COLORONCOLOR);
	  StretchBlt (hdcDest, i * cxBitmap * 2, 0, cxBitmap * 2, cyBitmap,
				  hdcSrc,  0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	  SelectObject (hdcDest, hbmpOld1);
	  SelectObject (hdcSrc,  hbmpOld2);
	  DeleteObject (hbmp);
	  }

   return TRUE;
}



/************************************************************************
**  Function: DDBChangeColor
**
**   Purpose: Change a particular color in a bitmap to another color.
**            Strategy is to create a monochrome mask, where each pixel
**            in the source bitmap that matches the color we're converting
**            from is set to white (1), and all other colors to black.
**            We then Blt this mask into the original bitmap, with a ROP
**            code that does:
**
**              (~Mask&Source) | (~Mask&Pattern)
**
**            where Pattern is the color we're changing to.
**
**            In other words, wherever the Mask is 1, we want to put the pattern;
**            wherever the Mask is 0, we want to leave the source alone.  By
**            using a Truth Table, you'll find that this ROP code is equivalent
**            to DSPDxax or ROP code 0x00E20746.
**
**            For info on figuring out ROP codes given a set of boolean ops,
**            check out the Windows 3.0 SDK, Reference Volume 2, chapter 11.
**
**  Parms   : hWnd    == Handle to _this_ window.
**            message == Message to process.
**            wParam  == WORD parameter -- depends on message
**            lParam  == LONG parameter -- depends on message
**
**  Returns : TRUE  = Success.
**            FALSE = Failure.
**
************************************************************************/

BOOL DDBChangeColor (HBITMAP hBitmap, COLORREF crFrom, COLORREF crTo)
{
   register int cx, cy;
   BITMAP       bm;
   HDC          hdcBmp, hdcMask;
   HBITMAP      hbmMask, hbmOld1, hbmOld2;
   HBRUSH       hBrush, hbrOld;

   if (!hBitmap)
	  return FALSE;

   GetObject (hBitmap, sizeof (bm), &bm);
   cx = bm.bmWidth;
   cy = bm.bmHeight;

   hbmMask = CreateBitmap (cx, cy, 1, 1, NULL);
   hdcMask = CreateCompatibleDC  (NULL);
   hdcBmp  = CreateCompatibleDC  (NULL);
   hBrush  = CreateSolidBrush (crTo);

   if (!hdcMask || !hdcBmp || !hBrush || !hbmMask)
	  {
	  DeleteObject (hbmMask);
	  DeleteObject (hBrush);
	  DeleteDC     (hdcMask);
	  DeleteDC     (hdcBmp);
	  return FALSE;
	  }

   hbmOld1 = SelectObject (hdcBmp,  hBitmap);
   hbmOld2 = SelectObject (hdcMask, hbmMask);
   hbrOld  = SelectObject (hdcBmp, hBrush);

   SetBkColor (hdcBmp, crFrom);
   BitBlt     (hdcMask, 0, 0, cx, cy, hdcBmp,  0, 0, SRCCOPY);
   SetBkColor (hdcBmp, RGB(255,255,255));
   BitBlt     (hdcBmp,  0, 0, cx, cy, hdcMask, 0, 0, DSPDxax);

   SelectObject (hdcBmp,  hbrOld);
   SelectObject (hdcBmp,  hbmOld1);
   SelectObject (hdcMask, hbmOld2);
   DeleteDC     (hdcBmp);
   DeleteDC     (hdcMask);
   DeleteObject (hBrush);
   DeleteObject (hbmMask);

   return TRUE;
}


/************************************************************************
**  Function: WndPeepSize
**
**  Purpose : Handles WM_SIZE messages for MainWndProc().  Size our
**            folder-list to fit within the client area of our main
**            window.
**
**  Parms   : cxWindow -- Width to resize to.
**            cyWindow -- Height to resize to.
**
**  Returns : None
**
************************************************************************/

void DFListBoxSize (UINT cxWindow, UINT cyWindow)
{
   SetWindowPos (ghFoldList, NULL, xoff, yoff, cxWindow, cyWindow,
				 SWP_NOMOVE | SWP_NOZORDER);
}
