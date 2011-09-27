
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  display.c -- module to support the main MDI child windows.
*
* This module contains Window Procedure, openDisplay() and closeDisplay()
*  routines.  Call openDisplay() one time per process before creating the
*  first window, and call closeDisplay() one time at process termination.
*
* windows are created in main frame window procedure.
*
* design:  We have one window procedure here for potentially multiple
*  MDI child windows.  The nCharPerLine, SqrHeight, and SqrWidth are
*  stored on a per window basis.  Also, the window class is registered
*  with the style CS_OWNDC.  For this reason, it is possible to select
*  a logical font into the HDC, and it will be there each time you get
*  the DC new for the window.  In this way we have a log font for each
*  window without actually doing any work in this module.
*
*  Some of the data is global, and works for all of the windows.  The
*  startcount & endcount arrays are a good example.  These are computed
*  once when the font is created, but the values remain the same for each
*  window.  Thus we have the following assumption:
*
*   1.  No fonts will be installed or removed while program is running.
*       (CountUCSegments values remain valid for run time duration).
*
*
*  There are WM_USER+ messages to set values for this window, and to
*  notify it of global changes.  Looking at the create time, user message
*  stream is helpful.  On the WM_CREATE message, we send ourselves a
*  WMU_NEWFONT message to create a logical font and count the number of
*  character ranges (segments).  Before returning from this message, we
*  send ourselves a WMU_NEWRANGE message to set the title, and the
*  horizontal scroll bar correctly.
*
*
\**************************************************************************/
#define UNICODE

#include <windows.h>
#include "uniput.h"
#include "display.h"


/* global variables store the start and end codepoints for UC ranges. */
#define MAXSEGMENTS  100
USHORT *endCount= NULL;
USHORT *startCount= NULL;

int CountUCSegments (HDC);
/* error return value from CountUCSegments */
#define SEGMENTERROR -1


/* conversion between (x,y) pairs and rectangle index */
int transXYtoIndex (int, int, int, int, int);
int transIndextoRect (int, PRECT, int, int, int);


/* window extra bytes for use to store window specific data (see register class) */
#define GWLU_NCHAR        0
#define GWLU_SQRHEIGHT    4
#define GWLU_SQRWIDTH     8





/* Global logfont.  Used for CreateFontIndirect().
 *  also regerenced in the uniput.c file.
 */
LOGFONT logfont = {
      UCFONTHEIGHT ,
      UCFONTWIDTH ,
       0 ,
       0 ,
     400 ,
       0 ,
       0 ,
       0 ,
       UNICODE_CHARSET ,
       0 ,
       0 ,
       2 ,
       2 ,
      TEXT("Lucida Sans Unicode")};


/* Global, class specific data.
 *  ought to allocate once per class, and free once at app exit time.
 */
HDC hdcMemXOR;
HBITMAP hbmMemXOR;
PVOID pbitsXOR, pbitsAND;

int FontWidth, FontHeight;
int nSegments;

/**************************************************************************\
*
*  function:  openDisplay()
*
*  Global variables:
*   HDC hdcMemXOR;
*   HBITMAP hbmMemXOR;
*   PVOID pbitsXOR, pbitsAND;
*   int FontWidth, FontHeight;
*   int nSegments;
*
*  returns:  TRUE - if success,  FALSE - if fail.
*
*  essential side effect:
*   Registers window class, and initial global variables.
\**************************************************************************/
int openDisplay(VOID )
{
WNDCLASS  wc;
static int cxCursor, cyCursor;
HFONT hfont;

  /* Register the window class*/
  wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC)DisplayWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 12;
  wc.hInstance = hInst;
  wc.hIcon = LoadIcon(hInst, TEXT("uniputIcon"));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = DISPLAYCLASS;

  if (!RegisterClass(&wc)) return (FALSE);

  cxCursor = GetSystemMetrics (SM_CXCURSOR);
  cyCursor = GetSystemMetrics (SM_CYCURSOR);

  /* get pointer to 0-init memory */
  pbitsAND = (PVOID) LocalAlloc (LPTR, cxCursor*cyCursor/8);

  /* Create a memory DC to draw into for creating cursors. */
  hdcMemXOR = CreateCompatibleDC (NULL);
  hbmMemXOR = CreateBitmap (cxCursor,cyCursor,1,4,NULL);
  SelectObject (hdcMemXOR, hbmMemXOR);

  /* set text modes for the memory HDC */
  SetBkMode (hdcMemXOR, TRANSPARENT);
  SetTextAlign (hdcMemXOR, TA_CENTER | TA_TOP);

  /* set a unicode font into the memory DC */
  logfont.lfHeight = cyCursor  *2/3;  // HACK.. scale down Y
  logfont.lfWidth  = cxCursor  *2/3;
  hfont = CreateFontIndirect (&logfont);
  SelectObject (hdcMemXOR, hfont);

  /* get pointer to be used later as pointer to bitmap bits */
  pbitsXOR = (PVOID) LocalAlloc (LPTR, cxCursor*cyCursor/2);

  /* Assume that we got a unicode font.
   *  Count the number of segments, and fill in the proper global
   *  variables specifying the code point ranges covered.
   */
  nSegments = CountUCSegments (hdcMemXOR); /* slow computation */


  /* verify that we have SOME ranges to work with */
  if (nSegments == SEGMENTERROR) return FALSE;

  /* warn the user if we can't find the right font */
  if (nSegments < 30)
    MessageBox (NULL,
        TEXT("Working with limited codepoint coverage. \n Install uclucida.ttf"),
        TEXT("Warning: Lucida Sans Unicode font not found."), MB_OK);



  FontWidth  = UCFONTWIDTH;
  FontHeight = UCFONTHEIGHT;

  return TRUE;
}

/**************************************************************************\
*
*  function:  closeDisplay()
*
*  Global variables:
*   HDC hdcMemXOR;
*   HBITMAP hbmMemXOR;
*   PVOID pbitsXOR, pbitsAND;
*
*  essential side effect:
*   Frees global variables.
\**************************************************************************/
int closeDisplay(VOID)
{
HFONT hfont;

  hfont = SelectObject  (hdcMemXOR, GetStockObject(SYSTEM_FONT));
  DeleteObject (hfont);
  DeleteDC (hdcMemXOR);
  DeleteObject (hbmMemXOR);

  LocalFree (LocalHandle (pbitsXOR));
  LocalFree (LocalHandle (pbitsAND));

  if (startCount != NULL) LocalFree (LocalHandle (startCount));
  if (endCount != NULL) LocalFree (LocalHandle (endCount));

  return TRUE;
}


/**************************************************************************\
*
*  function:  DisplayWndProc()
*
*  input parameters:  normal window procedure parameters.
*
\**************************************************************************/
LRESULT CALLBACK DisplayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HFONT  hfont, hfontOld;

static HDC hdc;
static RECT rect;
static int iSeg;
static TCHAR buffer[100];

/* rDown and index used as the user points to a square. */
static RECT rDown;
static int index;

static WCHAR DragDropChar;
static HICON hIcon;



  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * Set up the LOGFONT in the HDC for this window.
    \**********************************************************************/
    case WM_CREATE:
      SendMessage (hwnd, WMU_SETNCHAR, 16, 0);
      SendMessage (hwnd, WMU_NEWFONT, FontWidth, FontHeight);
    break;


    /**********************************************************************\
    *  WM_DESTROY
    *
    * Get rid of the logical font we create for each display window.
    *  relying here on CS_ONWDC
    \**********************************************************************/
    case WM_DESTROY:
      hdc = GetDC (hwnd);
      hfont = SelectObject  (hdc, GetStockObject(SYSTEM_FONT));
      DeleteObject (hfont);
      ReleaseDC (hwnd, hdc);
    break;





    /**********************************************************************\
    *  WMU_SETNCHAR
    *
    * wParam - nCharPerLine
    *
    \**********************************************************************/
    case WMU_SETNCHAR:
      SetWindowLong (hwnd, GWLU_NCHAR, (int) wParam);
      InvalidateRect (hwnd, NULL, TRUE);
    break;

    /**********************************************************************\
    *  WMU_NEWFONT
    *
    * wParam - Width
    * lParam - Height
    *
    * Create font, select it into the HDC, and reset ranges.
    \**********************************************************************/
    case WMU_NEWFONT: {

      hdc = GetDC(hwnd);

      FontWidth  = (int) wParam;
      FontHeight = (int) lParam;
      logfont.lfHeight = FontHeight;
      logfont.lfWidth  = FontWidth;

      hfont = CreateFontIndirect (&logfont);
      hfontOld = SelectObject (hdc, hfont);
      DeleteObject (hfontOld);

      ReleaseDC (hwnd, hdc);


      SetScrollRange (hwnd, SB_HORZ, 0, (nSegments-1), TRUE);
      SetScrollPos (hwnd, SB_HORZ, 0, TRUE);

      SendMessage (hwnd, WMU_NEWRANGE, 0,0);

    } return TRUE;


    /**********************************************************************\
    *  WMU_NEWRANGE
    *
    *
    * Range changes, or font size changes, or title status changes.
    \**********************************************************************/
    case WMU_NEWRANGE: {
      int sqrHeight, sqrWidth;
      TEXTMETRIC tm;
      int i;

      hdc = GetDC(hwnd);
      GetTextMetrics (hdc, &tm);
      ReleaseDC (hwnd, hdc);

      /* index into the start, end arrays stored in scroll bar */
      iSeg = GetScrollPos (hwnd, SB_HORZ);

      /* Set the window title text to show display range. */
      wsprintf (buffer, TEXT("[%04x, %04x]"),
                                  (int)startCount[iSeg],
                                  (int)endCount[iSeg]);

      /* if we are supposed to look up the character range name,
       *  then step through the ranges stored in the RangeName
       *  table.  Find the correct one, and reset the buffer string.
       */
      if (gShowNames) {
        for (i = 0; i< NRANGE; i++) {
          if ((RangeName[i].start <= startCount[iSeg]) &&
              ( startCount[iSeg] <= RangeName[i].end))

            wsprintf (buffer, TEXT("%s [%04x, %04x]"),
                                  RangeName[i].String,
                                  (int)startCount[iSeg],
                                  (int)endCount[iSeg]);

        }
      }
      SetWindowText (hwnd, buffer);

      sqrHeight =  tm.tmHeight + tm.tmExternalLeading;
      sqrWidth = tm.tmMaxCharWidth;

      SetWindowLong (hwnd, GWLU_SQRHEIGHT,sqrHeight);
      SetWindowLong (hwnd, GWLU_SQRWIDTH, sqrWidth );

    } return TRUE;









    /**********************************************************************\
    *  WM_LBUTTONDOWN
    *
    * Static, shared variables set in WM_LBUTTONDOWN:
    *   rDown, hIcon, DragDropChar
    *
    \**********************************************************************/
    case WM_LBUTTONDOWN: {
      int x,y;
      int nCharPerLine;
      int sqrHeight, sqrWidth;
      nCharPerLine = GetWindowLong (hwnd, GWLU_NCHAR);
      sqrHeight    = GetWindowLong (hwnd, GWLU_SQRHEIGHT);
      sqrWidth     = GetWindowLong (hwnd, GWLU_SQRWIDTH );



      x = (int)LOWORD (lParam);
      y = (int)HIWORD (lParam);

      index = transXYtoIndex (x,y, sqrWidth, sqrHeight, nCharPerLine);

      /* verify that the index is within the shown segment range */
      iSeg= GetScrollPos (hwnd, SB_HORZ);
      if (index > (endCount[iSeg]- startCount[iSeg])) return 0;

      DragDropChar = index + startCount[iSeg];

      transIndextoRect (index, &rDown, sqrWidth, sqrHeight, nCharPerLine);

      hdc = GetDC (hwnd);
      InvertRect (hdc, &rDown);
      ReleaseDC (hwnd, hdc);


      /* Create a cursor which represents the character.
       * Global variables used:
       *   HDC hdcMemXOR;
       *   HBITMAP hbmMemXOR;
       *   PVOID pbitsXOR, pbitsAND;
       */
      {
      int cxCursor, cyCursor;
      RECT rect;

      cxCursor = GetSystemMetrics (SM_CXCURSOR);
      cyCursor = GetSystemMetrics (SM_CYCURSOR);

      rect.left = rect.top = 0;
      rect.right = cxCursor; rect.bottom = cyCursor;


      FrameRect (hdcMemXOR, &rect, GetStockObject (BLACK_BRUSH));
      InflateRect (&rect, -1, -1);
      FillRect (hdcMemXOR, &rect, GetStockObject (LTGRAY_BRUSH));
      FrameRect (hdcMemXOR, &rect, GetStockObject (GRAY_BRUSH));
      InflateRect (&rect, -1, -1);
      SelectObject (hdcMemXOR, GetStockObject (WHITE_PEN));
      MoveToEx (hdcMemXOR, rect.right, rect.top, NULL);
      LineTo (hdcMemXOR,rect.left, rect.top);
      LineTo (hdcMemXOR,rect.left, rect.bottom);

      ExtTextOut(hdcMemXOR, cxCursor/2,0, ETO_CLIPPED, &rect,
                   (LPCWSTR)&DragDropChar, 1, NULL);

      GetBitmapBits (hbmMemXOR, cxCursor*cyCursor/2, pbitsXOR);

      hIcon = CreateIcon( GetModuleHandle(NULL),
              cxCursor,cyCursor, 1,4,
              pbitsAND,
              pbitsXOR);

      SetCursor ((HCURSOR)hIcon);
      }



      GdiFlush ();
      SetCapture (hwnd);
    } break;

    /**********************************************************************\
    *  WM_LBUTTONUP
    *
    * Static, shared variables set in WM_LBUTTONDOWN:
    *   rDown, hIcon, DragDropChar
    *
    \**********************************************************************/
    case WM_LBUTTONUP: {
      POINT p;
      HWND hwndTarget, hwndTitle;
#define NUMCHARS  255
      TCHAR buffer[NUMCHARS];


      if (GetCapture() == hwnd) {

        p.x = (int)(short)LOWORD (lParam);
        p.y = (int)(short)HIWORD (lParam);

        ClientToScreen (hwnd, &p);
        hwndTarget = WindowFromPoint (p);

        if ((GetWindowThreadProcessId (hwnd, NULL)) !=
            (GetWindowThreadProcessId (hwndTarget, NULL))) {

          /* hack.  unipad has a client window which gets the chars,
           *  but we want to display the frame windows text (title bar),
           *  so try to get the parent of the target for Title purposes.
           */
          hwndTitle = (GetParent(hwndTarget)) ? GetParent(hwndTarget) : hwndTarget;
          GetWindowText (hwndTitle, buffer, NUMCHARS);

          SendMessage (hwndTarget, WM_CHAR, (WPARAM) DragDropChar, 0);
          SendMessage (hwndStatus, WMU_CHARACTER,
                         (WPARAM) DragDropChar, (LPARAM)hwndTarget);
          SendMessage (hwndStatus, WMU_SETTARGETNAME,
                         (WPARAM) IsWindowUnicode (hwndTarget), (LPARAM)buffer);
        } /* end different process */

        Beep (40,40);

        hdc = GetDC (hwnd);
        InvertRect (hdc, &rDown);
        ReleaseDC (hwnd, hdc);
        GdiFlush ();
        ReleaseCapture ();
        SetCursor ((HCURSOR)LoadCursor (NULL, IDC_ARROW));
        DestroyIcon (hIcon);
      }
    } break;



    /**********************************************************************\
    *  WM_HSCROLL
    *
    * Step through the character ranges.
    * In every case, inform the window the range has changed,
    *  and invalidate it to force a repaint.
    \**********************************************************************/
    case WM_HSCROLL:

      switch (LOWORD(wParam)){
        int OldPos, NewPos;

        case SB_PAGEDOWN:
        case SB_LINEDOWN:
          OldPos = GetScrollPos (hwnd, SB_HORZ);
          SetScrollPos (hwnd, SB_HORZ, (OldPos+1), TRUE);
          SendMessage (hwnd,WMU_NEWRANGE, 0,0);
          InvalidateRect (hwnd, NULL, TRUE);
        break;

        case SB_PAGEUP:
        case SB_LINEUP:
          OldPos = GetScrollPos (hwnd, SB_HORZ);
          SetScrollPos (hwnd, SB_HORZ, (OldPos-1), TRUE);
          SendMessage (hwnd,WMU_NEWRANGE, 0,0);
          InvalidateRect (hwnd, NULL, TRUE);
        break;

        case SB_THUMBPOSITION:
          OldPos = GetScrollPos (hwnd, SB_HORZ);
          NewPos = HIWORD (wParam);
          SetScrollPos (hwnd, SB_HORZ, NewPos, TRUE);
          SendMessage (hwnd,WMU_NEWRANGE, 0,0);
          InvalidateRect (hwnd, NULL, TRUE);
        break;

      }

    break;







    /**********************************************************************\
    *  WM_SIZE
    *
    \**********************************************************************/
    case WM_SIZE:
      /* make sure that scroll metrics are ok */
      SendMessage (hwnd, WMU_NEWRANGE, 0,0);

    break;  /* fall through to MDIChildProc */


    /**********************************************************************\
    *  WM_PAINT
    *
    \**********************************************************************/
    case WM_PAINT: {
      HDC hdc;
      PAINTSTRUCT ps;
      RECT rect;
      POINT  point;
      int i;
      USHORT start, end;
      WCHAR outChar;
      USHORT codepoint;
      int nCharPerLine;
      int sqrHeight, sqrWidth;
      nCharPerLine = GetWindowLong (hwnd, GWLU_NCHAR);
      sqrHeight    = GetWindowLong (hwnd, GWLU_SQRHEIGHT);
      sqrWidth     = GetWindowLong (hwnd, GWLU_SQRWIDTH );


      hdc = BeginPaint(hwnd, &ps);
      SetBkMode (hdc, TRANSPARENT);

      iSeg= GetScrollPos (hwnd, SB_HORZ);
      start = startCount[iSeg];
      end = endCount[iSeg];

      /* ensure that we are in a valid range... some fonts have problems */
      if (start != 0xffff)

        for (codepoint = start, i=0; codepoint<=end; codepoint++,i++) {

          /* paint box and frame it */
          transIndextoRect (i, &rect, sqrWidth, sqrHeight, nCharPerLine);
          InflateRect (&rect, -1, -1);
          FillRect (hdc, &rect, GetStockObject (LTGRAY_BRUSH));
          InflateRect (&rect, 1, 1);
          FrameRect (hdc, &rect, GetStockObject (GRAY_BRUSH));
          InflateRect (&rect, -1, -1);
          SelectObject (hdc, GetStockObject (WHITE_PEN));
          MoveToEx (hdc, rect.right, rect.top, NULL);
          LineTo (hdc,rect.left, rect.top);
          LineTo (hdc,rect.left, rect.bottom);



          /* set point that we will draw glyph at */
          point.x = (rect.right + rect.left)/2;
          point.y = rect.top;
          SetTextAlign (hdc, TA_CENTER | TA_TOP);
          SetTextColor (hdc, PALETTEINDEX (0));


          /* special case the non-spacing diacritic marks. U+0300 -> U+036F
           *  Write a space first, for them to 'modify.'
           */
          if ( (0x0300 <= codepoint) && (codepoint <= 0x036F) ) {
            outChar = (WCHAR) 0x0020;
            TextOutW (hdc, 0,0, &outChar, 1);
          }

          outChar = (WCHAR) codepoint;
          ExtTextOut(hdc, point.x, point.y, ETO_CLIPPED, &rect, (LPCWSTR)&outChar, 1, NULL);



          /* fill in unicode code point in hex */
          if (gShowhex) {
            int nchar;
            HANDLE hfonttemp;

            nchar = wsprintf (buffer, TEXT("%04x"), (int) codepoint);
            hfonttemp = SelectObject (hdc,GetStockObject (SYSTEM_FIXED_FONT));

            point.y = rect.bottom;
            SetTextAlign (hdc, TA_CENTER | TA_BOTTOM);
            SetTextColor (hdc, PALETTEINDEX (5));

            TextOut( hdc, point.x, point.y,
                          buffer, nchar);
            SelectObject (hdc,hfonttemp);
          }


        } /* end for */

      EndPaint (hwnd, &ps);
    } return FALSE; /* end WM_PAINT */




  } /* end switch */

  return (DefMDIChildProc(hwnd, message, wParam, lParam));
}




/**************************************************************************\
*
* Need a mapping between the (x,y) pair, and the index of the square on
*  the window.  Two routines provide this... one for each direction.
*
\**************************************************************************/

/**********************************************************************\
*  transXYtoIndex
*
*  Given x & y values, return the index.
*
\**********************************************************************/
int transXYtoIndex (int x, int y,
     int sqrWidth, int sqrHeight, int nCharPerLine)
{
    x /= sqrWidth;
    if (x < 0) x = 0;
    if (x >= nCharPerLine) x = nCharPerLine-1;

    y /= sqrHeight;
    if (y < 0) y = 0;

    return ( (y * nCharPerLine) + x);
}


/**********************************************************************\
*  transIndextoRect
*
*  Given an index, i.e. the number of one of the squares on the display
*   window, fill in the rectangle structure.
*
\**********************************************************************/
int transIndextoRect (int index, PRECT pr,
     int sqrWidth, int sqrHeight, int nCharPerLine)
{
int x,y;

    x = index % nCharPerLine;
    y = index / nCharPerLine;

    pr->left = x*sqrWidth;
    pr->top = y*sqrHeight;
    pr->right = pr->left +sqrWidth;
    pr->bottom = pr->top +sqrHeight;

    return TRUE;
}






/**************************************************************************\
*
* All of the code below is used for parsing 'font data.'  It will only
*  make sense if you have a copy of the True Type font spec.  In short,
*  we grab the 'cmap' table, look through it for a subtable, and then
*  get two parallel arrays from the subtable.  Complications arise because
*  the true type data is 'BIG ENDIAN' and NT is being run 'little endian.'
*  For this reason, once we locate the short or long, we call Swap* to
*  change the byte ordering.
*
\**************************************************************************/


VOID SwapShort (PUSHORT);
VOID SwapULong  (PULONG);



typedef struct tagTABLE{
    USHORT platformID;
    USHORT encodingID;
    ULONG  offset;
} TABLE, *PTABLE;

typedef struct tagSUBTABLE{
    USHORT format;
    USHORT length;
    USHORT version;
    USHORT segCountX2;
    USHORT searchRange;
    USHORT entrySelector;
    USHORT rangeShift;
} SUBTABLE, *PSUBTABLE;


/* 'cmap' is passed in by value in a DWORD */
#define CMAPHEX  0x70616d63
#define NBYTES   256
#define OFFSETERROR  0



/**************************************************************************\
*
*  function:  CountUCSegments()
*
*  input parameters:
*   hdc - with the logical font set into it.
*   prect - pointer to client rectangle.
*
*  Global variables:
*   startCount
*   endCount
*
*  returns:
*   number of UC segments or
*   SEGMENTERROR if there is some kind of error.
*
*  essential side effect:
*   Fills in global startCount, endCount arrays.
\**************************************************************************/
int CountUCSegments (HDC hdc)
{
DWORD       cbData;
USHORT      aShort[2];
DWORD       nBytes;
USHORT      i, nTables;
PTABLE      pTable;
PSUBTABLE   pSubTable;
ULONG       offset,offsetFormat4;
USHORT      segCount;
BYTE        buffer[NBYTES];




    /* find number of "subtables", second long in cmap */
    nBytes=GetFontData (hdc, CMAPHEX, 0, aShort, 4);
    if (nBytes == GDI_ERROR) {
      MessageBox (NULL, MBGETFONTDATAERR,MBERROR , MBERRORFLAGS);
      return SEGMENTERROR;
    }
    if (nBytes == 0) {
      MessageBox (NULL, TEXT("No 'cmap' table."),MBERROR , MBERRORFLAGS);
      return SEGMENTERROR;
    }
    nTables = aShort[1];
    SwapShort (&nTables);


    cbData = nTables * sizeof(TABLE);
    if (cbData >NBYTES) {
      MessageBox (NULL, TEXT("cbData >NBYTES"),MBERROR , MBERRORFLAGS);
      return SEGMENTERROR;
    }

    /* get array of subtables information.  Check each one for 3,1*/
    nBytes=GetFontData (hdc, CMAPHEX, 4, buffer, cbData);
    pTable = (PTABLE)buffer;
    offsetFormat4 = OFFSETERROR;
    for (i = 0; i< nTables; i++) {

        SwapShort (&(pTable->encodingID));
        SwapShort (&(pTable->platformID));

        if ((pTable->platformID == 3)&&(pTable->encodingID == 1)) {
          offsetFormat4 = pTable->offset;
          SwapULong (&offsetFormat4);
          break;
        }
        pTable++;
    }

    /* verify that we got the correct offset to the FORMAT 4 subtable */
    if (offsetFormat4 == OFFSETERROR){
      MessageBox (NULL, TEXT("Can not find 3,1 subtable"),MBERROR , MBERRORFLAGS);
      return SEGMENTERROR;
    }

    /* Get the beginning of the subtable, especially the segment count */
    nBytes=GetFontData (hdc, CMAPHEX, offsetFormat4, buffer, sizeof(SUBTABLE));
    pSubTable = (PSUBTABLE) buffer;
    SwapShort (&(pSubTable->format));
    SwapShort (&(pSubTable->segCountX2));

    if (pSubTable->format != 4){
      MessageBox (NULL, TEXT("format !=4"), MBERROR, MBERRORFLAGS);
      return SEGMENTERROR;
    }

    segCount = pSubTable->segCountX2 / 2;

    /* Now that we know how many segments that the font contains,
     *  free up the old memory, and realloc. the two global arrays.
     */
    if (startCount != NULL) LocalFree (LocalHandle (startCount));
    if (endCount != NULL) LocalFree (LocalHandle (endCount));
    startCount = LocalAlloc (LPTR, segCount * sizeof(USHORT));
    endCount = LocalAlloc (LPTR, segCount * sizeof(USHORT));

    if ((startCount == NULL) || (endCount == NULL)) {
      MessageBox (NULL, TEXT("LocalAlloc failed"), MBERROR, MBERRORFLAGS);
      return SEGMENTERROR;
    }

    /* read in the array of endCount values */
    offset = offsetFormat4
           + (7 * sizeof (USHORT));  /* skip constant # bytes in subtable */
    cbData = segCount * sizeof (USHORT);
    nBytes=GetFontData (hdc, CMAPHEX, offset, endCount, cbData );
    for (i = 0; i<segCount; i++)
        SwapShort (& (endCount[i]));

    /* read in the array of startCount values */
    offset = offsetFormat4
           + (7 * sizeof (USHORT))   /* skip constant # bytes in subtable */
           + (segCount * sizeof (USHORT)) /* skip endCount array */
           + sizeof (USHORT);             /* skip reservedPad */
    cbData = segCount * sizeof (USHORT);
    nBytes=GetFontData (hdc, CMAPHEX, offset, startCount, cbData );
    for (i = 0; i<segCount; i++)
        SwapShort (& (startCount[i]));


    return segCount;
}










VOID SwapShort (PUSHORT p)
{
SHORT temp;

    temp =(SHORT)( HIBYTE (*p) + (LOBYTE(*p) << 8));
    *p = temp;
}



VOID SwapULong (PULONG p)
{
ULONG temp;

    temp = (LONG) ((BYTE) *p);
    temp <<= 8;
    *p >>=8;

    temp += (LONG) ((BYTE) *p);
    temp <<= 8;
    *p >>=8;

    temp += (LONG) ((BYTE) *p);
    temp <<= 8;
    *p >>=8;

    temp += (LONG) ((BYTE) *p);
    *p = temp;
}
