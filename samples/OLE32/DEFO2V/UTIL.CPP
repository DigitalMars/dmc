// utixl.cpp
//
// Implementation file for utility functions.
//
// Copyright (c) 1993-1995 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 7, 1993
//
// InterNet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 7, 1993  cek     First implementation.
//
#include "precomp.h"
#include "defo2v.h"
#include "util.h"

#if defined(WIN32) && !defined(_UNICODE)
    #pragma message("Building ANSI version of " __FILE__)
#endif

/////
// Utility functions
//

static TCHAR szBold[]        = _T("Bold") ;
static TCHAR szItalic[]      = _T("Italic") ;
static TCHAR szBoldItalic[]  = _T("Bold Italic") ;
static TCHAR szRegular[]     = _T("Regular") ;

extern "C"
int EXPORT WINAPI
    fnEnumReallyEx( LPENUMLOGFONT lpLogFont,
                LPNEWTEXTMETRIC lpTextMetrics,
                int nFontType,
                LPENUMLOGFONT lpELF ) ;

 /****************************************************************
 *  HFONT WINAPI
 *    ReallyCreateFontEx( HDC hDC, LPTSTR lpszFace,
 *                        LPTSTR lpszStyle, int nPointSize, UINT uiFlags )
 *
 *  Description:
 *
 *    Creates a font based on facename, pointsize, and style.
 *
 *    Uses 3.1 API's to correctly select TT fonts.
 *
 *    HDC   hDC         the target DC (optional)
 *
 *    LPTSTR lpszFace    pointer the facename (required)
 *
 *    LPTSTR lpszStyle   pointer to the style (like "Demibold Italic")
 *                      (optional).
 *
 *    int nPointSize  size in points (required)
 *
 *    UINT  uiFlags      Flags, same as for ReallyCreateFont() except
 *                      RCF_BOLD and RCF_ITALIC are ignored if lpszStyle
 *                      is not NULL.
 *
 *  Comments:
 *
 ****************************************************************/

HFONT WINAPI
   ReallyCreateFont( HDC hDC, LPTSTR lpszFace, LPTSTR lpszStyle, UINT nPointSize, UINT uiFlags )
{
   ENUMLOGFONT  elf ;
   FONTENUMPROC lpfn ;
   HDC          hdcCur ;

   lpfn = (FONTENUMPROC)fnEnumReallyEx ;

   // if the DC wasn't specified then use the display...
   //
   if (!hDC)
   {
      if (!(hdcCur = GetDC( NULL )))
         return FALSE ;
   }
   else
      hdcCur = hDC ;

   if (hdcCur)
   {
      /*
       * EnumFontFamilies takes the family name as the second param.
       * For example the family name might be "Lucida Sans" and the
       * style might be "Demibold Roman".
       *
       * The last parameter is app. defined.  In our case we pass a
       * structure that has a LOGFONT and a TCHAR array as it's members.
       * We put the style in the char array, and when this function
       * returns the LOGFONT has the logfont for the font we want to create.
       */
      if (lpszStyle)
         lstrcpy( (LPTSTR) elf.elfStyle, lpszStyle ) ;
      else
        switch( uiFlags & ~(RCF_NODEFAULT | RCF_STRIKEOUT | RCF_UNDERLINE) )
        {
            case RCF_BOLD:
                lstrcpy( (LPTSTR) elf.elfStyle, szBold ) ;
            break ;

            case RCF_ITALIC:
                lstrcpy( (LPTSTR) elf.elfStyle, szItalic ) ;
            break ;

            case RCF_BOLD | RCF_ITALIC:
                lstrcpy( (LPTSTR) elf.elfStyle, szBold ) ;
                lstrcat( (LPTSTR) elf.elfStyle, _T(" ") ) ;
                lstrcat( (LPTSTR) elf.elfStyle, szItalic ) ;
            break ;

            default:
                lstrcpy( (LPTSTR) elf.elfStyle, szRegular ) ;
                lpszStyle = (LPTSTR) elf.elfStyle ;
            break ;
        }

      if (lpszFace)
#ifndef WIN32
         EnumFontFamilies( hdcCur, lpszFace, lpfn, (LONG)(LPVOID) &elf ) ;
#else
         EnumFontFamilies( hdcCur, lpszFace, lpfn, (LPARAM)(LPTSTR) &elf ) ;
#endif

      if (!lpszFace || lstrcmpi( elf.elfLogFont.lfFaceName, lpszFace))
      {
         if ((uiFlags & RCF_NODEFAULT) == RCF_NODEFAULT)
         {
            if (hdcCur != hDC)
               ReleaseDC( NULL, hdcCur ) ;

            return NULL ;
         }
         else
            GetObject( GetStockObject( ANSI_VAR_FONT ),
                       sizeof( LOGFONT ), (LPTSTR)&elf.elfLogFont ) ;
      }

      // See pages 4-48, 4-49, of Reference Vol. 1 for explaination
      // of why we set lfWidth to 0
      //
      elf.elfLogFont.lfWidth = 0 ;

      // The equation for calculating point size based on font
      // height (a point is 1/72 of an inch) is:
      //
      //
      //    pt = (height * 72) / number of pixels in the Y direction
      //
      // Using GetTextMetrics() you should calculate height as:
      //
      //    height = tm.tmHeight - tm.tmInternalLeading
      //
      // This is because Windows' interpretation of a font height
      // is different from everyone else's.
      //
      //
      elf.elfLogFont.lfHeight = -MulDiv( nPointSize,
                             GetDeviceCaps( hdcCur, LOGPIXELSY),
                             72 )  ;

      elf.elfLogFont.lfStrikeOut = (BYTE)((uiFlags & RCF_STRIKEOUT) == RCF_STRIKEOUT) ;
      elf.elfLogFont.lfUnderline = (BYTE)((uiFlags & RCF_UNDERLINE) == RCF_UNDERLINE) ;

      if (!lpszStyle)
      {
         elf.elfLogFont.lfWeight    = (uiFlags & RCF_BOLD) ? FW_BOLD : FW_NORMAL ;
         elf.elfLogFont.lfItalic    = (BYTE)((uiFlags & RCF_ITALIC) == RCF_ITALIC) ;
      }

   }

   if (hdcCur != hDC)
      ReleaseDC( NULL, hdcCur ) ;

   return CreateFontIndirect( &elf.elfLogFont ) ;

} /* ReallyCreateFontEx()  */

/****************************************************************
 *  int CALLBACK
 *    fnEnumReallyEx( LPLOGFONT lpLogFont,
 *                  LPTEXTMETRIC lpTextMetrics,
 *                  int nFontType, LPLOGFONT lpLF )
 *
 *  Description:
 *
 *    Call back for EnumFonts and ReallySelectFontEx().
 *
 *    DO NOT FORGET TO EXPORT!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  Comments:
 *
 ****************************************************************/
extern "C"
int EXPORT WINAPI
fnEnumReallyEx( LPENUMLOGFONT lpELFin,
                LPNEWTEXTMETRIC lpTextMetrics,
                int nFontType,
                LPENUMLOGFONT lpELF )
{
   /* If we are not true type, then we will only be called once,
    * in in this case we need to compare the weight and italic
    * members of the TM struct to some strings.
    */
   if (!(nFontType & TRUETYPE_FONTTYPE))
   {
      lpELF->elfLogFont = lpELFin->elfLogFont ;

       // HACK TO FIX WindowsJ bug
       //
       LOGFONT lf ;
       GetObject( GetStockObject( SYSTEM_FONT ),
                           sizeof( LOGFONT ), (LPTSTR)&lf ) ;
       lpELF->elfLogFont.lfCharSet = lf.lfCharSet;

      /* Parse szStyle.  It may be something like "Bold Italic" or
       * "Demibold Italic".  Let's be somewhat smart about this.
       *
       * If "Demibold Italic" is passed in then he wants bold and
       * italic.
       *
       * We want to search szStyle for the word "bold".  Got a better
       * search strategy, go ahead and put it here (but tell me about it!).
       *
       * WARNING!!!
       *   The style string is language dependent.
       */
      if (lpELF->elfStyle)
      {
         /*
          * Assume "Regular"
          */
         lpELF->elfLogFont.lfWeight = FW_NORMAL ;
         lpELF->elfLogFont.lfItalic = FALSE ;

         if (!lstrcmpi( (LPTSTR) lpELF->elfStyle, szBold ))
         {
            lpELF->elfLogFont.lfWeight = FW_BOLD ;
            return 0 ;
         }

         if (!lstrcmpi( (LPTSTR) lpELF->elfStyle, szItalic))
         {
            lpELF->elfLogFont.lfItalic = TRUE ;
            return 0 ;
         }

         if (!lstrcmpi( (LPTSTR) lpELF->elfStyle, szBoldItalic ))
         {
            lpELF->elfLogFont.lfWeight = FW_BOLD ;
            lpELF->elfLogFont.lfItalic = TRUE ;
            return 0 ;
         }
      }

      return 0 ;  /* stop the enumeration */

   }

   /* We now know we have a TT font.  For each style in the
    * family passed in, we will get here.
    *
    * If we get an exact match copy it into lpELF and return.
    * Otherwise get as close as possible.
    */
   if (0==lstrcmpi( (LPTSTR) lpELF->elfStyle, (LPTSTR)lpELFin->elfStyle ))
   {
      *lpELF = *lpELFin;
      return 0;
   }

   lpELF->elfLogFont = lpELFin->elfLogFont ;

   return 1 ;

}/* fnEnumReallyEx() */


/*************************************************************************
 *  void WINAPI
 *  DlgCenter( HWND hwndCenter, HWND hwndWithin, BOOL fClient )
 *
 *  Description:
 *
 *    Centers a window within another window.
 *
 *  Type/Parameter
 *          Description
 *
 *    HWND  hwndCenter
 *          Window to center.  This does not have to be a child of
 *          hwndWithin.
 *
 *    HWND  hwndWithin
 *          Window to center the above window within.  Can be NULL.
 *
 *    BOOL  fClient
 *          If TRUE the window is centered within the available client
 *          area.  Otherwise it's centered within the entire window area.
 *
 *  Comments:
 *
 *************************************************************************/
void WINAPI
DlgCenter( HWND hwndCenter, HWND hwndWithin, BOOL fClient )
{
   RECT rcWithin ;
   RECT rcCenter ;
   int   x, y ;
   int   dxCenter, dyCenter ;
   int   dxScreen, dyScreen ;

   dxScreen = GetSystemMetrics( SM_CXSCREEN ) ;
   dyScreen = GetSystemMetrics( SM_CYSCREEN ) ;

   if (!IsWindow(hwndCenter))
      return ;

   if (hwndWithin && !IsWindow(hwndWithin))
      return ;

   if (hwndWithin == NULL)
   {
      rcWithin.left = rcWithin.top = 0 ;
      rcWithin.right = dxScreen ;
      rcWithin.bottom = dyScreen ;
   }
   else
   {
      if (fClient)
      {
         /*
          * First get screen coords of rectangle we're going to be
          * centered within.
          */
         GetClientRect( hwndWithin, (LPRECT)&rcWithin ) ;
         ClientToScreen( hwndWithin, (LPPOINT)&rcWithin.left ) ;
         ClientToScreen( hwndWithin, (LPPOINT)&rcWithin.right ) ;
      }
      else
         GetWindowRect( hwndWithin, (LPRECT)&rcWithin ) ;
   }

   GetWindowRect( hwndCenter, (LPRECT)&rcCenter ) ;
   dxCenter = rcCenter.right - rcCenter.left ;
   dyCenter = rcCenter.bottom - rcCenter.top ;

   /*
    * Now we have both the within and center rects in screen coords.
    *
    * SetWindowPos behaves differently for Non child windows
    * than for child windows.  For popups it's coordinates
    * are relative to the upper left corner of the screen.  For
    * children it's coords are relative to the upper left corner
    * of the client area of the parent.
    */
   x = ((rcWithin.right - rcWithin.left) - dxCenter) / 2 ;
   y = ((rcWithin.bottom - rcWithin.top) - dyCenter) / 2 ;

   if (hwndWithin == GetParent( hwndCenter ) &&
       !(GetWindowLong( hwndCenter, GWL_STYLE ) & WS_CHILD ))
   {
      x += rcWithin.left ;
      y += rcWithin.top ;

      if (x + dxCenter > dxScreen )
         x = dxScreen - dxCenter ;

      if (y + dyCenter > dyScreen )
         y = dyScreen - dyCenter ;

      SetWindowPos( hwndCenter, NULL,
                    max(x,0),
                    max(y,0),
                    0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER ) ;

      return ;
   }

   SetWindowPos( hwndCenter, NULL,
                 max(x,0),
                 max(y,0),
                 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER ) ;

   return ;

}/* DlgCenter() */

// column text out

/****************************************************************
 *  int WINAPI
 *    GetTextMetricsCorrectly( HDC hDC, LPTEXTMETRIC lpTextMetrics )
 *
 *  Description:
 *
 *    This function gets the textmetrics of the font currently
 *    selected into the hDC.  It returns the average char width as
 *    the return value.
 *
 *    This function computes the average character width correctly
 *    by using GetTextExtent() on the string "abc...xzyABC...XYZ"
 *    which works out much better for proportional fonts.
 *
 *    Note that this function returns the same TEXTMETRIC
 *    values that GetTextMetrics() does, it simply has a different
 *    return value.
 *
 *  Comments:
 *
 ****************************************************************/
int WINAPI
   GetTextMetricsCorrectly( HDC hDC, LPTEXTMETRIC lpTM )
{
   int    nAveWidth ;
   TCHAR   rgchAlphabet[52] ;
   int  i ;
   SIZE size ;

   // Get the TM of the current font.  Note that GetTextMetrics
   // gets the incorrect AveCharWidth value for proportional fonts.
   // This is the whole poshort in this exercise.
   //
   if (lpTM)
    GetTextMetrics(hDC, lpTM);

   // If it's not a variable pitch font GetTextMetrics was correct
   // so just return.
   //
   if (lpTM && !(lpTM->tmPitchAndFamily & FIXED_PITCH))
      return lpTM->tmAveCharWidth ;
   else
   {
      for ( i = 0 ; i <= 25 ; i++)
         rgchAlphabet[i] = (TCHAR)(i+(int)'a') ;

      for ( i = 0 ; i<=25 ; i++)
         rgchAlphabet[i+25] = (TCHAR)(i+(int)'A') ;

      GetTextExtentPoint( hDC, (LPTSTR)rgchAlphabet, 52, &size ) ;
      nAveWidth = size.cx / 26 ;
      nAveWidth = (nAveWidth + 1) / 2 ;
   }

   // Return the calculated average char width
   //
   return nAveWidth ;

} /* GetTextMetricsCorrectly()  */

/****************************************************************
 *
 *  Description:
 *
 *    The ColumntTextOut function writes a character string at
 *    the specified location, using tabs to identify column breaks.  Each
 *    column is aligned according to the array of COLUMNSTRUCTs.
 *
 *    A COLUMNSTRUCT looks like this:
 *
 *    {
 *       int   nLeft ;       // starting x position of the column
 *       int   nRight ;      // ending x position of the column
 *       UINT  uiFlags ;      // format flags
 *    }
 *
 *    If a column has another column to the left of it, it's nLeft member
 *    is ignored.
 *
 *    uiFlags can be any of the following:
 *
 *          #define DT_LEFT      0x0000
 *          #define DT_CENTER    0x0001
 *          #define DT_RIGHT     0x0002
 *
 *  Comments:
 *
 ****************************************************************/
void WINAPI ColumnTextOut( HDC hdc, int nX, int nY, LPTSTR lpszIN,
                            int cColumns, LPCOLUMNSTRUCT rgColumns )
{

#define ETOFLAGS ETO_CLIPPED

    COLUMNSTRUCT   CS ;              // local copy for speed
    RECT           rc ;              // current cell rect
    int            cIteration = 0 ;  // what column
    LPTSTR          lpNext ;          // next string (current is lpsz)
    int            cch ;             // count of chars in current string
    SIZE           size ;            // extent of current string
    #ifdef WIN32
    LONG          dx ;              // column width
    #else
    int             dx ;
    #endif

    rc.left = nX ;
    rc.top = nY ;
    rc.right = 0 ;

    if (rgColumns == NULL || cColumns <= 1)
    {
        int Tab = 15 ;
        TabbedTextOut( hdc, nX, nY, lpszIN, lstrlen(lpszIN), 1, &Tab, nX ) ;
        return ;
    }

    // For each sub string (bracketed by a tab)
    //
    LPTSTR lpsz = lpszIN ;
    while (*lpsz)
    {
        if (cIteration >= cColumns)
            return ;

        for (cch = 0, lpNext = lpsz ;
             *lpNext != '\t' && *lpNext ;
             lpNext++, cch++)
            ;

        CS = rgColumns[cIteration] ;

        // If it's the leftmost column use
        //
        if (cIteration == 0)
        {
            rc.left = nX + CS.nLeft ;
            rc.right = nX + CS.nRight ;
        }
        else
            rc.right = nX + CS.nRight ;

        GetTextExtentPoint( hdc, lpsz, cch, &size ) ;
        rc.bottom = rc.top + size.cy ;

        // If the width of the column is 0 do nothing
        //
        if ((dx = (rc.right - rc.left)) > 0)
        {
            switch(CS.uiFlags)
            {
                case DT_CENTER:
                    ExtTextOut( hdc, rc.left + ((dx - size.cx) / (int)2),
                    rc.top, ETOFLAGS, &rc, lpsz, cch, NULL ) ;
                break ;

                case DT_RIGHT:
                    // If it's right justified then make the left border 0
                    //
                    //rc.left = nX + rgColumns[0].nLeft ;
                    ExtTextOut( hdc, rc.left + (dx - size.cx),
                    rc.top, ETOFLAGS, &rc, lpsz, cch, NULL ) ;
                break ;

                case DT_LEFT:
                default:
                    ExtTextOut( hdc, rc.left, rc.top, ETOFLAGS, &rc, lpsz, cch, NULL ) ;
                break ;
            }
        }
        rc.left = rc.right ;
        cIteration++ ;
        lpsz = lpNext + 1 ;
    }

} // ColumnTextOut()

// Stolen from OLEMISC.CPP in the MFC 3.0 source.  Function names
// changed from Afx* to _*.
//
#define _countof(array)       (sizeof(array)/sizeof(array[0]))
LPCTSTR  _GetScodeString(SCODE sc)
{
      struct SCODE_ENTRY
      {
              SCODE sc;
              LPCTSTR lpszName;
      };
      #define MAKE_SCODE_ENTRY(sc)    { sc, _T(#sc) }
      static const SCODE_ENTRY scNameTable[] =
      {
              MAKE_SCODE_ENTRY(S_OK),
              MAKE_SCODE_ENTRY(S_FALSE),

              MAKE_SCODE_ENTRY(CACHE_S_FORMATETC_NOTSUPPORTED),
              MAKE_SCODE_ENTRY(CACHE_S_SAMECACHE),
              MAKE_SCODE_ENTRY(CACHE_S_SOMECACHES_NOTUPDATED),
              MAKE_SCODE_ENTRY(CONVERT10_S_NO_PRESENTATION),
              MAKE_SCODE_ENTRY(DATA_S_SAMEFORMATETC),
              MAKE_SCODE_ENTRY(DRAGDROP_S_CANCEL),
              MAKE_SCODE_ENTRY(DRAGDROP_S_DROP),
              MAKE_SCODE_ENTRY(DRAGDROP_S_USEDEFAULTCURSORS),
              MAKE_SCODE_ENTRY(INPLACE_S_TRUNCATED),
              MAKE_SCODE_ENTRY(MK_S_HIM),
              MAKE_SCODE_ENTRY(MK_S_ME),
              MAKE_SCODE_ENTRY(MK_S_MONIKERALREADYREGISTERED),
              MAKE_SCODE_ENTRY(MK_S_REDUCED_TO_SELF),
              MAKE_SCODE_ENTRY(MK_S_US),
              MAKE_SCODE_ENTRY(OLE_S_MAC_CLIPFORMAT),
              MAKE_SCODE_ENTRY(OLE_S_STATIC),
              MAKE_SCODE_ENTRY(OLE_S_USEREG),
              MAKE_SCODE_ENTRY(OLEOBJ_S_CANNOT_DOVERB_NOW),
              MAKE_SCODE_ENTRY(OLEOBJ_S_INVALIDHWND),
              MAKE_SCODE_ENTRY(OLEOBJ_S_INVALIDVERB),
              MAKE_SCODE_ENTRY(OLEOBJ_S_LAST),
              MAKE_SCODE_ENTRY(STG_S_CONVERTED),
              MAKE_SCODE_ENTRY(VIEW_S_ALREADY_FROZEN),

              MAKE_SCODE_ENTRY(E_UNEXPECTED),
              MAKE_SCODE_ENTRY(E_NOTIMPL),
              MAKE_SCODE_ENTRY(E_OUTOFMEMORY),
              MAKE_SCODE_ENTRY(E_INVALIDARG),
              MAKE_SCODE_ENTRY(E_NOINTERFACE),
              MAKE_SCODE_ENTRY(E_POINTER),
              MAKE_SCODE_ENTRY(E_HANDLE),
              MAKE_SCODE_ENTRY(E_ABORT),
              MAKE_SCODE_ENTRY(E_FAIL),
              MAKE_SCODE_ENTRY(E_ACCESSDENIED),

              MAKE_SCODE_ENTRY(CACHE_E_NOCACHE_UPDATED),
              MAKE_SCODE_ENTRY(CLASS_E_CLASSNOTAVAILABLE),
              MAKE_SCODE_ENTRY(CLASS_E_NOAGGREGATION),
              MAKE_SCODE_ENTRY(CLIPBRD_E_BAD_DATA),
              MAKE_SCODE_ENTRY(CLIPBRD_E_CANT_CLOSE),
              MAKE_SCODE_ENTRY(CLIPBRD_E_CANT_EMPTY),
              MAKE_SCODE_ENTRY(CLIPBRD_E_CANT_OPEN),
              MAKE_SCODE_ENTRY(CLIPBRD_E_CANT_SET),
              MAKE_SCODE_ENTRY(CO_E_ALREADYINITIALIZED),
              MAKE_SCODE_ENTRY(CO_E_APPDIDNTREG),
              MAKE_SCODE_ENTRY(CO_E_APPNOTFOUND),
              MAKE_SCODE_ENTRY(CO_E_APPSINGLEUSE),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(CO_E_BAD_PATH),
              #endif
              MAKE_SCODE_ENTRY(CO_E_CANTDETERMINECLASS),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(CO_E_CLASS_CREATE_FAILED),
              #endif
              MAKE_SCODE_ENTRY(CO_E_CLASSSTRING),
              MAKE_SCODE_ENTRY(CO_E_DLLNOTFOUND),
              MAKE_SCODE_ENTRY(CO_E_ERRORINAPP),
              MAKE_SCODE_ENTRY(CO_E_ERRORINDLL),
              MAKE_SCODE_ENTRY(CO_E_IIDSTRING),
              MAKE_SCODE_ENTRY(CO_E_NOTINITIALIZED),
              MAKE_SCODE_ENTRY(CO_E_OBJISREG),
              MAKE_SCODE_ENTRY(CO_E_OBJNOTCONNECTED),
              MAKE_SCODE_ENTRY(CO_E_OBJNOTREG),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(CO_E_OBJSRV_RPC_FAILURE),
              MAKE_SCODE_ENTRY(CO_E_SCM_ERROR),
              MAKE_SCODE_ENTRY(CO_E_SCM_RPC_FAILURE),
              MAKE_SCODE_ENTRY(CO_E_SERVER_EXEC_FAILURE),
              MAKE_SCODE_ENTRY(CO_E_SERVER_STOPPING),
              #endif
              MAKE_SCODE_ENTRY(CO_E_WRONGOSFORAPP),
              MAKE_SCODE_ENTRY(CONVERT10_E_OLESTREAM_BITMAP_TO_DIB),
              MAKE_SCODE_ENTRY(CONVERT10_E_OLESTREAM_FMT),
              MAKE_SCODE_ENTRY(CONVERT10_E_OLESTREAM_GET),
              MAKE_SCODE_ENTRY(CONVERT10_E_OLESTREAM_PUT),
              MAKE_SCODE_ENTRY(CONVERT10_E_STG_DIB_TO_BITMAP),
              MAKE_SCODE_ENTRY(CONVERT10_E_STG_FMT),
              MAKE_SCODE_ENTRY(CONVERT10_E_STG_NO_STD_STREAM),
              MAKE_SCODE_ENTRY(DISP_E_ARRAYISLOCKED),
              MAKE_SCODE_ENTRY(DISP_E_BADCALLEE),
              MAKE_SCODE_ENTRY(DISP_E_BADINDEX),
              MAKE_SCODE_ENTRY(DISP_E_BADPARAMCOUNT),
              MAKE_SCODE_ENTRY(DISP_E_BADVARTYPE),
              MAKE_SCODE_ENTRY(DISP_E_EXCEPTION),
              MAKE_SCODE_ENTRY(DISP_E_MEMBERNOTFOUND),
              MAKE_SCODE_ENTRY(DISP_E_NONAMEDARGS),
              MAKE_SCODE_ENTRY(DISP_E_NOTACOLLECTION),
              MAKE_SCODE_ENTRY(DISP_E_OVERFLOW),
              MAKE_SCODE_ENTRY(DISP_E_PARAMNOTFOUND),
              MAKE_SCODE_ENTRY(DISP_E_PARAMNOTOPTIONAL),
              MAKE_SCODE_ENTRY(DISP_E_TYPEMISMATCH),
              MAKE_SCODE_ENTRY(DISP_E_UNKNOWNINTERFACE),
              MAKE_SCODE_ENTRY(DISP_E_UNKNOWNLCID),
              MAKE_SCODE_ENTRY(DISP_E_UNKNOWNNAME),
              MAKE_SCODE_ENTRY(DRAGDROP_E_ALREADYREGISTERED),
              MAKE_SCODE_ENTRY(DRAGDROP_E_INVALIDHWND),
              MAKE_SCODE_ENTRY(DRAGDROP_E_NOTREGISTERED),
              MAKE_SCODE_ENTRY(DV_E_CLIPFORMAT),
              MAKE_SCODE_ENTRY(DV_E_DVASPECT),
              MAKE_SCODE_ENTRY(DV_E_DVTARGETDEVICE),
              MAKE_SCODE_ENTRY(DV_E_DVTARGETDEVICE_SIZE),
              MAKE_SCODE_ENTRY(DV_E_FORMATETC),
              MAKE_SCODE_ENTRY(DV_E_LINDEX),
              MAKE_SCODE_ENTRY(DV_E_NOIVIEWOBJECT),
              MAKE_SCODE_ENTRY(DV_E_STATDATA),
              MAKE_SCODE_ENTRY(DV_E_STGMEDIUM),
              MAKE_SCODE_ENTRY(DV_E_TYMED),
              MAKE_SCODE_ENTRY(INPLACE_E_NOTOOLSPACE),
              MAKE_SCODE_ENTRY(INPLACE_E_NOTUNDOABLE),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(MEM_E_INVALID_LINK),
              MAKE_SCODE_ENTRY(MEM_E_INVALID_ROOT),
              MAKE_SCODE_ENTRY(MEM_E_INVALID_SIZE),
              #endif
              MAKE_SCODE_ENTRY(MK_E_CANTOPENFILE),
              MAKE_SCODE_ENTRY(MK_E_CONNECTMANUALLY),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(MK_E_ENUMERATION_FAILED),
              #endif
              MAKE_SCODE_ENTRY(MK_E_EXCEEDEDDEADLINE),
              MAKE_SCODE_ENTRY(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED),
              MAKE_SCODE_ENTRY(MK_E_INVALIDEXTENSION),
              MAKE_SCODE_ENTRY(MK_E_MUSTBOTHERUSER),
              MAKE_SCODE_ENTRY(MK_E_NEEDGENERIC),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(MK_E_NO_NORMALIZED),
              #endif
              MAKE_SCODE_ENTRY(MK_E_NOINVERSE),
              MAKE_SCODE_ENTRY(MK_E_NOOBJECT),
              MAKE_SCODE_ENTRY(MK_E_NOPREFIX),
              MAKE_SCODE_ENTRY(MK_E_NOSTORAGE),
              MAKE_SCODE_ENTRY(MK_E_NOTBINDABLE),
              MAKE_SCODE_ENTRY(MK_E_NOTBOUND),
              MAKE_SCODE_ENTRY(MK_E_SYNTAX),
              MAKE_SCODE_ENTRY(MK_E_UNAVAILABLE),
              MAKE_SCODE_ENTRY(OLE_E_ADVF),
              MAKE_SCODE_ENTRY(OLE_E_ADVISENOTSUPPORTED),
              MAKE_SCODE_ENTRY(OLE_E_BLANK),
              MAKE_SCODE_ENTRY(OLE_E_CANT_BINDTOSOURCE),
              MAKE_SCODE_ENTRY(OLE_E_CANT_GETMONIKER),
              MAKE_SCODE_ENTRY(OLE_E_CANTCONVERT),
              MAKE_SCODE_ENTRY(OLE_E_CLASSDIFF),
              MAKE_SCODE_ENTRY(OLE_E_ENUM_NOMORE),
              MAKE_SCODE_ENTRY(OLE_E_INVALIDHWND),
              MAKE_SCODE_ENTRY(OLE_E_INVALIDRECT),
              MAKE_SCODE_ENTRY(OLE_E_NOCACHE),
              MAKE_SCODE_ENTRY(OLE_E_NOCONNECTION),
              MAKE_SCODE_ENTRY(OLE_E_NOSTORAGE),
              MAKE_SCODE_ENTRY(OLE_E_NOT_INPLACEACTIVE),
              MAKE_SCODE_ENTRY(OLE_E_NOTRUNNING),
              MAKE_SCODE_ENTRY(OLE_E_OLEVERB),
              MAKE_SCODE_ENTRY(OLE_E_PROMPTSAVECANCELLED),
              MAKE_SCODE_ENTRY(OLE_E_STATIC),
              MAKE_SCODE_ENTRY(OLE_E_WRONGCOMPOBJ),
              MAKE_SCODE_ENTRY(OLEOBJ_E_INVALIDVERB),
              MAKE_SCODE_ENTRY(OLEOBJ_E_NOVERBS),
              MAKE_SCODE_ENTRY(REGDB_E_CLASSNOTREG),
              MAKE_SCODE_ENTRY(REGDB_E_IIDNOTREG),
              MAKE_SCODE_ENTRY(REGDB_E_INVALIDVALUE),
              MAKE_SCODE_ENTRY(REGDB_E_KEYMISSING),
              MAKE_SCODE_ENTRY(REGDB_E_READREGDB),
              MAKE_SCODE_ENTRY(REGDB_E_WRITEREGDB),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_ATTEMPTED_MULTITHREAD),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_CALL_CANCELED),
              MAKE_SCODE_ENTRY(RPC_E_CALL_REJECTED),
              MAKE_SCODE_ENTRY(RPC_E_CANTCALLOUT_AGAIN),
              MAKE_SCODE_ENTRY(RPC_E_CANTCALLOUT_INASYNCCALL),
              MAKE_SCODE_ENTRY(RPC_E_CANTCALLOUT_INEXTERNALCALL),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_CANTCALLOUT_ININPUTSYNCCALL),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_CANTPOST_INSENDCALL),
              MAKE_SCODE_ENTRY(RPC_E_CANTTRANSMIT_CALL),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_CHANGED_MODE),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_CLIENT_CANTMARSHAL_DATA),
              MAKE_SCODE_ENTRY(RPC_E_CLIENT_CANTUNMARSHAL_DATA),
              MAKE_SCODE_ENTRY(RPC_E_CLIENT_DIED),
              MAKE_SCODE_ENTRY(RPC_E_CONNECTION_TERMINATED),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_DISCONNECTED),
              MAKE_SCODE_ENTRY(RPC_E_FAULT),
              MAKE_SCODE_ENTRY(RPC_E_INVALID_CALLDATA),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_INVALID_DATA),
              MAKE_SCODE_ENTRY(RPC_E_INVALID_DATAPACKET),
              MAKE_SCODE_ENTRY(RPC_E_INVALID_PARAMETER),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_INVALIDMETHOD),
              MAKE_SCODE_ENTRY(RPC_E_NOT_REGISTERED),
              MAKE_SCODE_ENTRY(RPC_E_OUT_OF_RESOURCES),
              MAKE_SCODE_ENTRY(RPC_E_RETRY),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_SERVER_CANTMARSHAL_DATA),
              MAKE_SCODE_ENTRY(RPC_E_SERVER_CANTUNMARSHAL_DATA),
              MAKE_SCODE_ENTRY(RPC_E_SERVER_DIED),
              #ifdef _WIN32
              MAKE_SCODE_ENTRY(RPC_E_SERVER_DIED_DNE),
              MAKE_SCODE_ENTRY(RPC_E_SERVERCALL_REJECTED),
              MAKE_SCODE_ENTRY(RPC_E_SERVERCALL_RETRYLATER),
              MAKE_SCODE_ENTRY(RPC_E_SERVERFAULT),
              MAKE_SCODE_ENTRY(RPC_E_SYS_CALL_FAILED),
              MAKE_SCODE_ENTRY(RPC_E_THREAD_NOT_INIT),
              MAKE_SCODE_ENTRY(RPC_E_WRONG_THREAD),
              #endif
              MAKE_SCODE_ENTRY(RPC_E_UNEXPECTED),
              MAKE_SCODE_ENTRY(STG_E_ABNORMALAPIEXIT),
              MAKE_SCODE_ENTRY(STG_E_ACCESSDENIED),
              MAKE_SCODE_ENTRY(STG_E_CANTSAVE),
              MAKE_SCODE_ENTRY(STG_E_DISKISWRITEPROTECTED),
              MAKE_SCODE_ENTRY(STG_E_EXTANTMARSHALLINGS),
              MAKE_SCODE_ENTRY(STG_E_FILEALREADYEXISTS),
              MAKE_SCODE_ENTRY(STG_E_FILENOTFOUND),
              MAKE_SCODE_ENTRY(STG_E_INSUFFICIENTMEMORY),
              MAKE_SCODE_ENTRY(STG_E_INUSE),
              MAKE_SCODE_ENTRY(STG_E_INVALIDFLAG),
              MAKE_SCODE_ENTRY(STG_E_INVALIDFUNCTION),
              MAKE_SCODE_ENTRY(STG_E_INVALIDHANDLE),
              MAKE_SCODE_ENTRY(STG_E_INVALIDHEADER),
              MAKE_SCODE_ENTRY(STG_E_INVALIDNAME),
              MAKE_SCODE_ENTRY(STG_E_INVALIDPARAMETER),
              MAKE_SCODE_ENTRY(STG_E_INVALIDPOINTER),
              MAKE_SCODE_ENTRY(STG_E_LOCKVIOLATION),
              MAKE_SCODE_ENTRY(STG_E_MEDIUMFULL),
              MAKE_SCODE_ENTRY(STG_E_NOMOREFILES),
              MAKE_SCODE_ENTRY(STG_E_NOTCURRENT),
              MAKE_SCODE_ENTRY(STG_E_NOTFILEBASEDSTORAGE),
              MAKE_SCODE_ENTRY(STG_E_OLDDLL),
              MAKE_SCODE_ENTRY(STG_E_OLDFORMAT),
              MAKE_SCODE_ENTRY(STG_E_PATHNOTFOUND),
              MAKE_SCODE_ENTRY(STG_E_READFAULT),
              MAKE_SCODE_ENTRY(STG_E_REVERTED),
              MAKE_SCODE_ENTRY(STG_E_SEEKERROR),
              MAKE_SCODE_ENTRY(STG_E_SHAREREQUIRED),
              MAKE_SCODE_ENTRY(STG_E_SHAREVIOLATION),
              MAKE_SCODE_ENTRY(STG_E_TOOMANYOPENFILES),
              MAKE_SCODE_ENTRY(STG_E_UNIMPLEMENTEDFUNCTION),
              MAKE_SCODE_ENTRY(STG_E_UNKNOWN),
              MAKE_SCODE_ENTRY(STG_E_WRITEFAULT),
              MAKE_SCODE_ENTRY(TYPE_E_AMBIGUOUSNAME),
              MAKE_SCODE_ENTRY(TYPE_E_BADMODULEKIND),
              MAKE_SCODE_ENTRY(TYPE_E_BUFFERTOOSMALL),
              MAKE_SCODE_ENTRY(TYPE_E_CANTCREATETMPFILE),
              MAKE_SCODE_ENTRY(TYPE_E_CANTLOADLIBRARY),
              MAKE_SCODE_ENTRY(TYPE_E_CIRCULARTYPE),
              MAKE_SCODE_ENTRY(TYPE_E_DLLFUNCTIONNOTFOUND),
              MAKE_SCODE_ENTRY(TYPE_E_DUPLICATEID),
              MAKE_SCODE_ENTRY(TYPE_E_ELEMENTNOTFOUND),
              MAKE_SCODE_ENTRY(TYPE_E_INCONSISTENTPROPFUNCS),
              MAKE_SCODE_ENTRY(TYPE_E_INVALIDSTATE),
              MAKE_SCODE_ENTRY(TYPE_E_INVDATAREAD),
              MAKE_SCODE_ENTRY(TYPE_E_IOERROR),
              MAKE_SCODE_ENTRY(TYPE_E_LIBNOTREGISTERED),
              MAKE_SCODE_ENTRY(TYPE_E_NAMECONFLICT),
              MAKE_SCODE_ENTRY(TYPE_E_OUTOFBOUNDS),
              MAKE_SCODE_ENTRY(TYPE_E_QUALIFIEDNAMEDISALLOWED),
              MAKE_SCODE_ENTRY(TYPE_E_REGISTRYACCESS),
              MAKE_SCODE_ENTRY(TYPE_E_SIZETOOBIG),
              MAKE_SCODE_ENTRY(TYPE_E_TYPEMISMATCH),
              MAKE_SCODE_ENTRY(TYPE_E_UNDEFINEDTYPE),
              MAKE_SCODE_ENTRY(TYPE_E_UNKNOWNLCID),
              MAKE_SCODE_ENTRY(TYPE_E_UNSUPFORMAT),
              MAKE_SCODE_ENTRY(TYPE_E_WRONGTYPEKIND),
              MAKE_SCODE_ENTRY(VIEW_E_DRAW),
      };
      #undef MAKE_SCODE_ENTRY

      // look for it in the table
      for (int i = 0; i < _countof(scNameTable); i++)
      {
              if (sc == scNameTable[i].sc)
                      return scNameTable[i].lpszName;
      }
      return NULL;    // not found
}

LPCTSTR  _GetScodeRangeString(SCODE sc)
{
      struct RANGE_ENTRY
      {
              SCODE scFirst;
              SCODE scLast;
              LPCTSTR lpszName;
      };
      #define MAKE_RANGE_ENTRY(scRange) \
              { scRange##_FIRST, scRange##_LAST, \
                      _T(#scRange) _T("_FIRST...") _T(#scRange) _T("_LAST") }

      static const RANGE_ENTRY scRangeTable[] =
      {
              MAKE_RANGE_ENTRY(CACHE_E),
              MAKE_RANGE_ENTRY(CACHE_S),
              MAKE_RANGE_ENTRY(CLASSFACTORY_E),
              MAKE_RANGE_ENTRY(CLASSFACTORY_S),
              MAKE_RANGE_ENTRY(CLIENTSITE_E),
              MAKE_RANGE_ENTRY(CLIENTSITE_S),
              MAKE_RANGE_ENTRY(CLIPBRD_E),
              MAKE_RANGE_ENTRY(CLIPBRD_S),
              MAKE_RANGE_ENTRY(CONVERT10_E),
              MAKE_RANGE_ENTRY(CONVERT10_S),
              MAKE_RANGE_ENTRY(CO_E),
              MAKE_RANGE_ENTRY(CO_S),
              MAKE_RANGE_ENTRY(DATA_E),
              MAKE_RANGE_ENTRY(DATA_S),
              MAKE_RANGE_ENTRY(DRAGDROP_E),
              MAKE_RANGE_ENTRY(DRAGDROP_S),
              MAKE_RANGE_ENTRY(ENUM_E),
              MAKE_RANGE_ENTRY(ENUM_S),
              MAKE_RANGE_ENTRY(INPLACE_E),
              MAKE_RANGE_ENTRY(INPLACE_S),
              MAKE_RANGE_ENTRY(MARSHAL_E),
              MAKE_RANGE_ENTRY(MARSHAL_S),
              MAKE_RANGE_ENTRY(MK_E),
              MAKE_RANGE_ENTRY(MK_S),
              MAKE_RANGE_ENTRY(OLEOBJ_E),
              MAKE_RANGE_ENTRY(OLEOBJ_S),
              MAKE_RANGE_ENTRY(OLE_E),
              MAKE_RANGE_ENTRY(OLE_S),
              MAKE_RANGE_ENTRY(REGDB_E),
              MAKE_RANGE_ENTRY(REGDB_S),
              MAKE_RANGE_ENTRY(VIEW_E),
              MAKE_RANGE_ENTRY(VIEW_S),
      };
      #undef MAKE_RANGE_ENTRY

      // look for it in the table
      for (int i = 0; i < _countof(scRangeTable); i++)
      {
              if (sc >= scRangeTable[i].scFirst && sc <= scRangeTable[i].scLast)
                      return scRangeTable[i].lpszName;
      }
      return NULL;    // not found
}

LPCTSTR  _GetSeverityString(SCODE sc)
{
      static const TCHAR* rgszSEVERITY[] =
      {
              _T("SEVERITY_SUCCESS"),
              _T("SEVERITY_ERROR"),
      };
      return rgszSEVERITY[SCODE_SEVERITY(sc)];
}

LPCTSTR  _GetFacilityString(SCODE sc)
{
      static const TCHAR* rgszFACILITY[] =
      {
              _T("FACILITY_NULL"),
              _T("FACILITY_RPC"),
              _T("FACILITY_DISPATCH"),
              _T("FACILITY_STORAGE"),
              _T("FACILITY_ITF"),
              _T("FACILITY_0x05"),
              _T("FACILITY_0x06"),
              _T("FACILITY_WIN32"),
              _T("FACILITY_WINDOWS"),
      };
      if (SCODE_FACILITY(sc) >= _countof(rgszFACILITY))
              return _T("<Unknown Facility>");

      return rgszFACILITY[SCODE_FACILITY(sc)];
}

LPCTSTR  _GetFullScodeString(SCODE sc)
{
      static TCHAR szBuf[128];
      LPCTSTR lpsz;
      if ((lpsz = _GetScodeString(sc)) != NULL)
      {
              // found exact match
              wsprintf(szBuf, _T("%s ($%08lX)"), lpsz, sc);
      }
      else if ((lpsz = _GetScodeRangeString(sc)) != NULL)
      {
              // found suitable range
              wsprintf(szBuf, _T("range: %s ($%08lX)"), lpsz, sc);
      }
      else
      {
              // not found at all -- split it up into its parts
              wsprintf(szBuf, _T("severity: %s, facility: %s ($%08lX)"),
                      _GetSeverityString(sc), _GetFacilityString(sc), sc);
      }
      return szBuf;
}


LPTSTR HRtoString( HRESULT hr )
{
    SCODE   sc ;
    sc = GetScode( hr ) ;

#ifdef FORMATMSG
    LPVOID lpMessageBuffer ;
    if (FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM,
          NULL,
          sc,
          MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
          (LPTSTR) &lpMessageBuffer,
          0,
          NULL ))
    {
        wsprintf( sz, _T( "%s (0x%lx)" ), (LPTSTR)lpMessageBuffer, sc);
        LocalFree(lpMessageBuffer) ;
    }
    else
    {
        wsprintf( sz, _T( "Unknown Scode (0x%lx)" ), sc);
    }


#endif // !FORMATMSG

    return (LPTSTR)_GetFullScodeString(sc) ;
}

static TCHAR * g_rgszVT[] =
{
    _T("Void"),             //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
    _T("Null"),             //VT_NULL            = 1,   /* [V]        SQL style Null              */
    _T("Integer"),          //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
    _T("Long"),             //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
    _T("Single"),           //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
    _T("Double"),           //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
    _T("Currency"),         //VT_CY              = 6,   /* [V][T][P]  currency                    */
    _T("Date"),             //VT_DATE            = 7,   /* [V][T][P]  date                        */
    _T("String"),           //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
    _T("LPDISPATCH"),           //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
    _T("SCODE"),            //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
    _T("Boolean"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
    _T("Variant"),          //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
    _T("LPUNKNOWN"),        //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
    _T("Unicode"),          //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
    _T(""),                 //                   = 15,
    _T("BYTE"),             //VT_I1              = 16,  /*    [T]     signed char                 */
    _T("char"),             //VT_UI1             = 17,  /*    [T]     unsigned char               */
    _T("USHORT"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
    _T("ULONG"),            //VT_UI4             = 19,  /*    [T]     unsigned short              */
    _T("int64"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
    _T("uint64"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
    _T("int"),              //VT_INT             = 22,  /*    [T]     signed machine int          */
    _T("UINT"),             //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
    _T("VOID"),             //VT_VOID            = 24,  /*    [T]     C style void                */
    _T("HRESULT"),          //VT_HRESULT         = 25,  /*    [T]                                 */
    _T("PTR"),              //VT_PTR             = 26,  /*    [T]     pointer type                */
    _T("SAFEARRAY"),        //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
    _T("CARRAY"),           //VT_CARRAY          = 28,  /*    [T]     C style array               */
    _T("USERDEFINED"),      //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
    _T("LPSTR"),            //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
    _T("LPWSTR"),           //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
    _T(""),                 //                   = 32,
    _T(""),                 //                   = 33,
    _T(""),                 //                   = 34,
    _T(""),                 //                   = 35,
    _T(""),                 //                   = 36,
    _T(""),                 //                   = 37,
    _T(""),                 //                   = 38,
    _T(""),                 //                   = 39,
    _T(""),                 //                   = 40,
    _T(""),                 //                   = 41,
    _T(""),                 //                   = 42,
    _T(""),                 //                   = 43,
    _T(""),                 //                   = 44,
    _T(""),                 //                   = 45,
    _T(""),                 //                   = 46,
    _T(""),                 //                   = 47,
    _T(""),                 //                   = 48,
    _T(""),                 //                   = 49,
    _T(""),                 //                   = 50,
    _T(""),                 //                   = 51,
    _T(""),                 //                   = 52,
    _T(""),                 //                   = 53,
    _T(""),                 //                   = 54,
    _T(""),                 //                   = 55,
    _T(""),                 //                   = 56,
    _T(""),                 //                   = 57,
    _T(""),                 //                   = 58,
    _T(""),                 //                   = 59,
    _T(""),                 //                   = 60,
    _T(""),                 //                   = 61,
    _T(""),                 //                   = 62,
    _T(""),                 //                   = 63,
    _T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
    _T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
    _T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
    _T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
    _T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
    _T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
    _T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
    _T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
    _T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};

LPTSTR VTtoString( VARTYPE vt )
{
    static TCHAR szBuf[64];

    if (vt <= VT_CLSID)
        return (LPTSTR)g_rgszVT[vt] ;

    if (vt & VT_VECTOR)
    {
        vt &= ~VT_VECTOR ;
        if (vt <= VT_CLSID)
            wsprintf( szBuf, _T("VECTOR of %s"), (LPTSTR)g_rgszVT[vt] ) ;
        else
            wsprintf( szBuf, _T("<Unknown %08lX>"), vt & VT_VECTOR ) ;
        return (LPTSTR)szBuf ;
    }

    if (vt & VT_ARRAY)
    {
        vt &= ~VT_ARRAY ;
        if (vt <= VT_CLSID)
            wsprintf( szBuf, _T("Array of %s"), (LPTSTR)g_rgszVT[vt] ) ;
        else
            wsprintf( szBuf, _T("<Unknown %08lX>"), vt & VT_ARRAY ) ;
        return (LPTSTR)szBuf ;
    }

    if (vt & VT_BYREF)
    {
        vt &= ~VT_BYREF ;
        if (vt <= VT_CLSID)
            wsprintf( szBuf, _T("%s BYREF "), (LPTSTR)g_rgszVT[vt] ) ;
        else
            wsprintf( szBuf, _T("<Unknown %08lX>"), vt & VT_BYREF ) ;
        return (LPTSTR)szBuf ;
    }

    if (vt & VT_RESERVED)
    {
        vt &= ~VT_RESERVED ;
        if (vt <= VT_CLSID)
            wsprintf( szBuf, _T("RESERVED (%s)"), (LPTSTR)g_rgszVT[vt] ) ;
        else
            wsprintf( szBuf, _T("<Unknown %08lX>"), vt & VT_RESERVED ) ;
        return (LPTSTR)szBuf ;
    }

    wsprintf( szBuf, _T("<Unknown %08lX>"), vt ) ;
    return (LPTSTR)szBuf ;
}

LPTSTR VTtoString2( VARTYPE vt )
{
    static TCHAR szBuf[64];

    vt &= ~0xF000 ;

    if (vt <= VT_CLSID)
        return (LPTSTR)g_rgszVT[vt] ;

    wsprintf( szBuf, _T("<Unknown %08lX>"), vt ) ;
    return (LPTSTR)szBuf ;
}

LPTSTR TYPEDESCtoString( TYPEDESC FAR* lptdesc )
{
    static TCHAR sz[64] ;

    lstrcpy( sz, VTtoString( lptdesc->vt ) ) ;
    if ((lptdesc->vt & 0x0FFF) == VT_PTR)
    {
        // lptdesc points to a TYPEDESC that specifies the thing pointed to
        lstrcat( sz, _T(" to ") ) ;
        lstrcat( sz, VTtoString( lptdesc->lptdesc->vt ) ) ;
        if ((lptdesc->lptdesc->vt & 0x0FFF) == VT_CARRAY)
        {
            lstrcat( sz,  _T(" of ") ) ;
            lstrcat( sz,  VTtoString( lptdesc->lptdesc->lpadesc->tdescElem.vt ) ) ;
            TCHAR szNum[16] ;
            wsprintf( szNum, _T("[%u]"), lptdesc->lptdesc->lpadesc->cDims ) ;
            lstrcat( sz, szNum ) ;
        }
    }

    if ((lptdesc->vt & 0x0FFF) == VT_CARRAY)
    {
        lstrcat( sz,  _T(" of ") ) ;
        lstrcat( sz,  VTtoString( lptdesc->lpadesc->tdescElem.vt ) ) ;
        TCHAR szNum[16] ;
        wsprintf( szNum, _T("[%u]"), lptdesc->lpadesc->cDims ) ;
        lstrcat( sz, szNum ) ;
    }
    return sz ;
}



int _lwriteT( int handle, TCHAR *taBuf, unsigned int count )
{

#ifdef _UNICODE
    char   achBuf[256];
    wcstombs( achBuf, taBuf, 256 );
    return (_lwrite( handle,  achBuf, count ));
#else
    return (_lwrite( handle, taBuf, count ));
#endif

}

HFILE WINAPI OpenFileT( LPTSTR tszFileName, LPOFSTRUCT lpReOpenBuf, UINT uStyle )
{
#ifdef _UNICODE
    char   achName[MAX_PATH];
    wcstombs( achName, tszFileName, MAX_PATH );
    return OpenFile( achName, lpReOpenBuf, uStyle );
#else
    return OpenFile( tszFileName, lpReOpenBuf, uStyle );
#endif

}
