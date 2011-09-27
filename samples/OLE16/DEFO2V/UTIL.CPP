// util.cpp
//
// Implementation file for utility functions.
//
// Copyright (c) 1993 Microsoft Corporation, All Rights Reserved.
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

/////
// Utility functions
//

static char szBold[]        = "Bold" ;
static char szItalic[]      = "Italic" ;
static char szBoldItalic[]  = "Bold Italic" ;
static char szRegular[]     = "Regular" ;

extern "C"
int _export CALLBACK
	fnEnumReallyEx( LPENUMLOGFONT lpLogFont,
				LPNEWTEXTMETRIC lpTextMetrics,
				int nFontType,
				LPENUMLOGFONT lpELF ) ;

 /****************************************************************
 *  HFONT WINAPI
 *    ReallyCreateFontEx( HDC hDC, LPSTR lpszFace,
 *                        LPSTR lpszStyle, int nPointSize, UINT uiFlags )
 *
 *  Description:
 *
 *    Creates a font based on facename, pointsize, and style.
 *
 *    Uses 3.1 API's to correctly select TT fonts.
 *
 *    HDC   hDC         the target DC (optional)
 *
 *    LPSTR lpszFace    pointer the facename (required)
 *
 *    LPSTR lpszStyle   pointer to the style (like "Demibold Italic")
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
   ReallyCreateFont( HDC hDC, LPSTR lpszFace, LPSTR lpszStyle, UINT nPointSize, UINT uiFlags )
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
	   * structure that has a LOGFONT and a char array as it's members.
	   * We put the style in the char array, and when this function
	   * returns the LOGFONT has the logfont for the font we want to create.
	   */
	  if (lpszStyle)
		 lstrcpy( elf.elfStyle, lpszStyle ) ;
	  else
		switch( uiFlags & ~(RCF_NODEFAULT | RCF_STRIKEOUT | RCF_UNDERLINE) )
		{
			case RCF_BOLD:
				lstrcpy( elf.elfStyle, szBold ) ;
			break ;

			case RCF_ITALIC:
				lstrcpy( elf.elfStyle, szItalic ) ;
			break ;

			case RCF_BOLD | RCF_ITALIC:
				lstrcpy( elf.elfStyle, szBold ) ;
				lstrcat( elf.elfStyle, " " ) ;
				lstrcat( elf.elfStyle, szItalic ) ;
			break ;

			default:
				lstrcpy( elf.elfStyle, szRegular ) ;
				lpszStyle = elf.elfStyle ;
			break ;
		}

	  if (lpszFace)
		 EnumFontFamilies( hdcCur, lpszFace, lpfn, (LPSTR)(LPVOID)&elf ) ;

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
					   sizeof( LOGFONT ), (LPSTR)&elf.elfLogFont ) ;
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
int _export CALLBACK
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

		 if (!lstrcmpi( lpELF->elfStyle, szBold ))
		 {
			lpELF->elfLogFont.lfWeight = FW_BOLD ;
			return 0 ;
		 }

		 if (!lstrcmpi( lpELF->elfStyle, szItalic))
		 {
			lpELF->elfLogFont.lfItalic = TRUE ;
			return 0 ;
		 }

		 if (!lstrcmpi( lpELF->elfStyle, szBoldItalic ))
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
   if (0==lstrcmpi( lpELF->elfStyle, lpELFin->elfStyle ))
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
   char   rgchAlphabet[52] ;
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
		 rgchAlphabet[i] = (char)(i+(int)'a') ;

	  for ( i = 0 ; i<=25 ; i++)
		 rgchAlphabet[i+25] = (char)(i+(int)'A') ;

	  GetTextExtentPoint( hDC, (LPSTR)rgchAlphabet, 52, &size ) ;
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
void WINAPI ColumnTextOut( HDC hdc, int nX, int nY, LPSTR lpszIN,
							int cColumns, LPCOLUMNSTRUCT rgColumns )
{

#define ETOFLAGS ETO_CLIPPED

	COLUMNSTRUCT   CS ;              // local copy for speed
	RECT           rc ;              // current cell rect
	int            cIteration = 0 ;  // what column
	LPSTR          lpNext ;          // next string (current is lpsz)
	int            cch ;             // count of chars in current string
	SIZE           size ;            // extent of current string
	short          dx ;              // column width

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
	LPSTR lpsz = lpszIN ;
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


LPSTR HRtoString( HRESULT hr )
{
	static  char sz[256] ;
	LPSTR  szErrName ;
	SCODE   sc ;
#define CASE_SCODE(sc)  case sc: szErrName = (LPSTR)#sc ; break ;

	sc = GetScode( hr ) ;

	switch (sc) {
		/* SCODE's defined in SCODE.H */
		CASE_SCODE(S_OK)
 // same value as S_OK      CASE_SCODE(S_TRUE)
		CASE_SCODE(S_FALSE)
		CASE_SCODE(E_UNEXPECTED)
		CASE_SCODE(E_NOTIMPL)
		CASE_SCODE(E_OUTOFMEMORY)
		CASE_SCODE(E_INVALIDARG)
		CASE_SCODE(E_NOINTERFACE)
		CASE_SCODE(E_POINTER)
		CASE_SCODE(E_HANDLE)
		CASE_SCODE(E_ABORT)
		CASE_SCODE(E_FAIL)
		CASE_SCODE(E_ACCESSDENIED)

		/* SCODE's defined in DVOBJ.H */
		CASE_SCODE(DATA_E_FORMATETC)
// same as DATA_E_FORMATETC     CASE_SCODE(DV_E_FORMATETC)
		CASE_SCODE(DATA_S_SAMEFORMATETC)
		CASE_SCODE(VIEW_E_DRAW)
//  same as VIEW_E_DRAW         CASE_SCODE(E_DRAW)
		CASE_SCODE(VIEW_S_ALREADY_FROZEN)
		CASE_SCODE(CACHE_E_NOCACHE_UPDATED)
		CASE_SCODE(CACHE_S_FORMATETC_NOTSUPPORTED)
		CASE_SCODE(CACHE_S_SAMECACHE)
		CASE_SCODE(CACHE_S_SOMECACHES_NOTUPDATED)

		/* SCODE's defined in OLE2.H */
		CASE_SCODE(OLE_E_OLEVERB)
		CASE_SCODE(OLE_E_ADVF)
		CASE_SCODE(OLE_E_ENUM_NOMORE)
		CASE_SCODE(OLE_E_ADVISENOTSUPPORTED)
		CASE_SCODE(OLE_E_NOCONNECTION)
		CASE_SCODE(OLE_E_NOTRUNNING)
		CASE_SCODE(OLE_E_NOCACHE)
		CASE_SCODE(OLE_E_BLANK)
		CASE_SCODE(OLE_E_CLASSDIFF)
		CASE_SCODE(OLE_E_CANT_GETMONIKER)
		CASE_SCODE(OLE_E_CANT_BINDTOSOURCE)
		CASE_SCODE(OLE_E_STATIC)
		CASE_SCODE(OLE_E_PROMPTSAVECANCELLED)
		CASE_SCODE(OLE_E_INVALIDRECT)
		CASE_SCODE(OLE_E_WRONGCOMPOBJ)
		CASE_SCODE(OLE_E_INVALIDHWND)
		CASE_SCODE(DV_E_DVTARGETDEVICE)
		CASE_SCODE(DV_E_STGMEDIUM)
		CASE_SCODE(DV_E_STATDATA)
		CASE_SCODE(DV_E_LINDEX)
		CASE_SCODE(DV_E_TYMED)
		CASE_SCODE(DV_E_CLIPFORMAT)
		CASE_SCODE(DV_E_DVASPECT)
		CASE_SCODE(DV_E_DVTARGETDEVICE_SIZE)
		CASE_SCODE(DV_E_NOIVIEWOBJECT)
		CASE_SCODE(OLE_S_USEREG)
		CASE_SCODE(OLE_S_STATIC)
		CASE_SCODE(OLE_S_MAC_CLIPFORMAT)
		CASE_SCODE(CONVERT10_E_OLESTREAM_GET)
		CASE_SCODE(CONVERT10_E_OLESTREAM_PUT)
		CASE_SCODE(CONVERT10_E_OLESTREAM_FMT)
		CASE_SCODE(CONVERT10_E_OLESTREAM_BITMAP_TO_DIB)
		CASE_SCODE(CONVERT10_E_STG_FMT)
		CASE_SCODE(CONVERT10_E_STG_NO_STD_STREAM)
		CASE_SCODE(CONVERT10_E_STG_DIB_TO_BITMAP)
		CASE_SCODE(CONVERT10_S_NO_PRESENTATION)
		CASE_SCODE(CLIPBRD_E_CANT_OPEN)
		CASE_SCODE(CLIPBRD_E_CANT_EMPTY)
		CASE_SCODE(CLIPBRD_E_CANT_SET)
		CASE_SCODE(CLIPBRD_E_BAD_DATA)
		CASE_SCODE(CLIPBRD_E_CANT_CLOSE)
		CASE_SCODE(DRAGDROP_E_NOTREGISTERED)
		CASE_SCODE(DRAGDROP_E_ALREADYREGISTERED)
		CASE_SCODE(DRAGDROP_E_INVALIDHWND)
		CASE_SCODE(DRAGDROP_S_DROP)
		CASE_SCODE(DRAGDROP_S_CANCEL)
		CASE_SCODE(DRAGDROP_S_USEDEFAULTCURSORS)
		CASE_SCODE(OLEOBJ_E_NOVERBS)
		CASE_SCODE(OLEOBJ_S_INVALIDVERB)
		CASE_SCODE(OLEOBJ_S_CANNOT_DOVERB_NOW)
		CASE_SCODE(OLEOBJ_S_INVALIDHWND)
		CASE_SCODE(INPLACE_E_NOTUNDOABLE)
		CASE_SCODE(INPLACE_E_NOTOOLSPACE)
		CASE_SCODE(INPLACE_S_TRUNCATED)

		/* SCODE's defined in STORAGE.H */
		CASE_SCODE(STG_E_INVALIDFUNCTION)
		CASE_SCODE(STG_E_FILENOTFOUND)
		CASE_SCODE(STG_E_PATHNOTFOUND)
		CASE_SCODE(STG_E_TOOMANYOPENFILES)
		CASE_SCODE(STG_E_ACCESSDENIED)
		CASE_SCODE(STG_E_INVALIDHANDLE)
		CASE_SCODE(STG_E_INSUFFICIENTMEMORY)
		CASE_SCODE(STG_E_INVALIDPOINTER)
		CASE_SCODE(STG_E_NOMOREFILES)
		CASE_SCODE(STG_E_DISKISWRITEPROTECTED)
		CASE_SCODE(STG_E_SEEKERROR)
		CASE_SCODE(STG_E_WRITEFAULT)
		CASE_SCODE(STG_E_READFAULT)
		CASE_SCODE(STG_E_LOCKVIOLATION)
		CASE_SCODE(STG_E_FILEALREADYEXISTS)
		CASE_SCODE(STG_E_INVALIDPARAMETER)
		CASE_SCODE(STG_E_MEDIUMFULL)
		CASE_SCODE(STG_E_ABNORMALAPIEXIT)
		CASE_SCODE(STG_E_INVALIDHEADER)
		CASE_SCODE(STG_E_INVALIDNAME)
		CASE_SCODE(STG_E_UNKNOWN)
		CASE_SCODE(STG_E_UNIMPLEMENTEDFUNCTION)
		CASE_SCODE(STG_E_INVALIDFLAG)
		CASE_SCODE(STG_E_INUSE)
		CASE_SCODE(STG_E_NOTCURRENT)
		CASE_SCODE(STG_E_REVERTED)
		CASE_SCODE(STG_E_CANTSAVE)
		CASE_SCODE(STG_E_OLDFORMAT)
		CASE_SCODE(STG_E_OLDDLL)
		CASE_SCODE(STG_E_SHAREREQUIRED)
		CASE_SCODE(STG_S_CONVERTED)
//        CASE_SCODE(STG_S_BUFFEROVERFLOW)
//        CASE_SCODE(STG_S_TRYOVERWRITE)

		/* SCODE's defined in COMPOBJ.H */
		CASE_SCODE(CO_E_NOTINITIALIZED)
		CASE_SCODE(CO_E_ALREADYINITIALIZED)
		CASE_SCODE(CO_E_CANTDETERMINECLASS)
		CASE_SCODE(CO_E_CLASSSTRING)
		CASE_SCODE(CO_E_IIDSTRING)
		CASE_SCODE(CO_E_APPNOTFOUND)
		CASE_SCODE(CO_E_APPSINGLEUSE)
		CASE_SCODE(CO_E_ERRORINAPP)
		CASE_SCODE(CO_E_DLLNOTFOUND)
		CASE_SCODE(CO_E_ERRORINDLL)
		CASE_SCODE(CO_E_WRONGOSFORAPP)
		CASE_SCODE(CO_E_OBJNOTREG)
		CASE_SCODE(CO_E_OBJISREG)
		CASE_SCODE(CO_E_OBJNOTCONNECTED)
		CASE_SCODE(CO_E_APPDIDNTREG)
		CASE_SCODE(CLASS_E_NOAGGREGATION)
		CASE_SCODE(REGDB_E_READREGDB)
		CASE_SCODE(REGDB_E_WRITEREGDB)
		CASE_SCODE(REGDB_E_KEYMISSING)
		CASE_SCODE(REGDB_E_INVALIDVALUE)
		CASE_SCODE(REGDB_E_CLASSNOTREG)
		CASE_SCODE(REGDB_E_IIDNOTREG)
		CASE_SCODE(RPC_E_FIRST)
		CASE_SCODE(RPC_E_CALL_REJECTED)
		CASE_SCODE(RPC_E_CALL_CANCELED)
		CASE_SCODE(RPC_E_CANTPOST_INSENDCALL)
		CASE_SCODE(RPC_E_CANTCALLOUT_INASYNCCALL)
		CASE_SCODE(RPC_E_CANTCALLOUT_INEXTERNALCALL)
		CASE_SCODE(RPC_E_CONNECTION_TERMINATED)
		CASE_SCODE(RPC_E_SERVER_DIED)
		CASE_SCODE(RPC_E_CLIENT_DIED)
		CASE_SCODE(RPC_E_INVALID_DATAPACKET)
		CASE_SCODE(RPC_E_CANTTRANSMIT_CALL)
		CASE_SCODE(RPC_E_CLIENT_CANTMARSHAL_DATA)
		CASE_SCODE(RPC_E_CLIENT_CANTUNMARSHAL_DATA)
		CASE_SCODE(RPC_E_SERVER_CANTMARSHAL_DATA)
		CASE_SCODE(RPC_E_SERVER_CANTUNMARSHAL_DATA)
		CASE_SCODE(RPC_E_INVALID_DATA)
		CASE_SCODE(RPC_E_INVALID_PARAMETER)
		CASE_SCODE(RPC_E_UNEXPECTED)

		/* SCODE's defined in MONIKER.H */
		CASE_SCODE(MK_E_CONNECTMANUALLY)
		CASE_SCODE(MK_E_EXCEEDEDDEADLINE)
		CASE_SCODE(MK_E_NEEDGENERIC)
		CASE_SCODE(MK_E_UNAVAILABLE)
		CASE_SCODE(MK_E_SYNTAX)
		CASE_SCODE(MK_E_NOOBJECT)
		CASE_SCODE(MK_E_INVALIDEXTENSION)
		CASE_SCODE(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED)
		CASE_SCODE(MK_E_NOTBINDABLE)
		CASE_SCODE(MK_E_NOTBOUND)
		CASE_SCODE(MK_E_CANTOPENFILE)
		CASE_SCODE(MK_E_MUSTBOTHERUSER)
		CASE_SCODE(MK_E_NOINVERSE)
		CASE_SCODE(MK_E_NOSTORAGE)
		CASE_SCODE(MK_S_REDUCED_TO_SELF)
		CASE_SCODE(MK_S_ME)
		CASE_SCODE(MK_S_HIM)
		CASE_SCODE(MK_S_US)
		CASE_SCODE(MK_S_MONIKERALREADYREGISTERED)

/* SCODE's defined in DISPATCH.H */
		CASE_SCODE(DISP_E_UNKNOWNINTERFACE)
		CASE_SCODE(DISP_E_MEMBERNOTFOUND)
		CASE_SCODE(DISP_E_PARAMNOTFOUND)
		CASE_SCODE(DISP_E_TYPEMISMATCH)
		CASE_SCODE(DISP_E_UNKNOWNNAME)
		CASE_SCODE(DISP_E_NONAMEDARGS)
		CASE_SCODE(DISP_E_BADVARTYPE)
		CASE_SCODE(DISP_E_EXCEPTION)
		CASE_SCODE(DISP_E_OVERFLOW)
		CASE_SCODE(DISP_E_BADINDEX)
		CASE_SCODE(DISP_E_UNKNOWNLCID)
		CASE_SCODE(DISP_E_ARRAYISLOCKED)
		CASE_SCODE(DISP_E_BADPARAMCOUNT)
		CASE_SCODE(DISP_E_PARAMNOTOPTIONAL)
		CASE_SCODE(DISP_E_BADCALLEE)
		CASE_SCODE(DISP_E_NOTACOLLECTION)
		CASE_SCODE(TYPE_E_IOERROR)
//        CASE_SCODE(TYPE_E_COMPILEERROR)
		CASE_SCODE(TYPE_E_CANTCREATETMPFILE)
//        CASE_SCODE(TYPE_E_ILLEGALINDEX)
//        CASE_SCODE(TYPE_E_IDNOTFOUND)
		CASE_SCODE(TYPE_E_BUFFERTOOSMALL)
//        CASE_SCODE(TYPE_E_READONLY)
		CASE_SCODE(TYPE_E_INVDATAREAD)
		CASE_SCODE(TYPE_E_UNSUPFORMAT)
//        CASE_SCODE(TYPE_E_ALREADYCONTAINSNAME)
//        CASE_SCODE(TYPE_E_NOMATCHINGARITY)
		CASE_SCODE(TYPE_E_REGISTRYACCESS)
		CASE_SCODE(TYPE_E_LIBNOTREGISTERED)
//        CASE_SCODE(TYPE_E_DUPLICATEDEFINITION)
//        CASE_SCODE(TYPE_E_USAGE)
//        CASE_SCODE(TYPE_E_DESTNOTKNOWN)
		CASE_SCODE(TYPE_E_UNDEFINEDTYPE)
		CASE_SCODE(TYPE_E_QUALIFIEDNAMEDISALLOWED)
		CASE_SCODE(TYPE_E_INVALIDSTATE)
		CASE_SCODE(TYPE_E_WRONGTYPEKIND)
		CASE_SCODE(TYPE_E_ELEMENTNOTFOUND)
		CASE_SCODE(TYPE_E_AMBIGUOUSNAME)
//        CASE_SCODE(TYPE_E_INVOKEFUNCTIONMISMATCH)
		CASE_SCODE(TYPE_E_DLLFUNCTIONNOTFOUND)
		CASE_SCODE(TYPE_E_BADMODULEKIND)
//        CASE_SCODE(TYPE_E_WRONGPLATFORM)
//        CASE_SCODE(TYPE_E_ALREADYBEINGLAIDOUT)
		CASE_SCODE(TYPE_E_CANTLOADLIBRARY)

		default:
			szErrName = "UNKNOWN SCODE" ;
	}

	wsprintf( sz, "%s (0x%lx)", (LPSTR)szErrName, sc);

	return (LPSTR)sz ;
}

static char * g_rgszVT[] =
{
	"Void",             //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
	"Null",             //VT_NULL            = 1,   /* [V]        SQL style Null              */
	"Integer",          //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
	"Long",             //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
	"Single",           //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
	"Double",           //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
	"Currency",         //VT_CY              = 6,   /* [V][T][P]  currency                    */
	"Date",             //VT_DATE            = 7,   /* [V][T][P]  date                        */
	"String",           //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
	"Object",           //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
	"SCODE",            //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
	"Boolean",          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
	"Variant",          //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
	"pIUnknown",        //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
	"Unicode",          //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
	"",                 //                   = 15,
	"BYTE",             //VT_I1              = 16,  /*    [T]     signed char                 */
	"char",             //VT_UI1             = 17,  /*    [T]     unsigned char               */
	"USHORT",           //VT_UI2             = 18,  /*    [T]     unsigned short              */
	"ULONG",            //VT_UI4             = 19,  /*    [T]     unsigned short              */
	"int64",            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
	"uint64",           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
	"int",              //VT_INT             = 22,  /*    [T]     signed machine int          */
	"UINT",             //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
	"VOID",             //VT_VOID            = 24,  /*    [T]     C style void                */
	"HRESULT",          //VT_HRESULT         = 25,  /*    [T]                                 */
	"PTR",              //VT_PTR             = 26,  /*    [T]     pointer type                */
	"SAFEARRAY",        //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
	"CARRAY",           //VT_CARRAY          = 28,  /*    [T]     C style array               */
	"USERDEFINED",      //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
	"LPSTR",            //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
	"LPWSTR",           //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
	"",                 //                   = 32,
	"",                 //                   = 33,
	"",                 //                   = 34,
	"",                 //                   = 35,
	"",                 //                   = 36,
	"",                 //                   = 37,
	"",                 //                   = 38,
	"",                 //                   = 39,
	"",                 //                   = 40,
	"",                 //                   = 41,
	"",                 //                   = 42,
	"",                 //                   = 43,
	"",                 //                   = 44,
	"",                 //                   = 45,
	"",                 //                   = 46,
	"",                 //                   = 47,
	"",                 //                   = 48,
	"",                 //                   = 49,
	"",                 //                   = 50,
	"",                 //                   = 51,
	"",                 //                   = 52,
	"",                 //                   = 53,
	"",                 //                   = 54,
	"",                 //                   = 55,
	"",                 //                   = 56,
	"",                 //                   = 57,
	"",                 //                   = 58,
	"",                 //                   = 59,
	"",                 //                   = 60,
	"",                 //                   = 61,
	"",                 //                   = 62,
	"",                 //                   = 63,
	"FILETIME",         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
	"BLOB",             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
	"STREAM",           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
	"STORAGE",          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
	"STREAMED_OBJECT",  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
	"STORED_OBJECT",    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
	"BLOB_OBJECT",      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
	"CF",               //VT_CF              = 71,  /*       [P]  Clipboard format            */
	"CLSID",            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};

LPSTR VTtoString( VARTYPE vt )
{
	static char szBuf[64];

	if (vt <= VT_CLSID)
		return (LPSTR)g_rgszVT[vt] ;

	if (vt & VT_VECTOR)
	{
		vt &= ~VT_VECTOR ;
		if (vt <= VT_CLSID)
			wsprintf( szBuf, "VECTOR of %s", (LPSTR)g_rgszVT[vt] ) ;
		else
			wsprintf( szBuf, "<Unknown %08lX>", vt & VT_VECTOR ) ;
		return (LPSTR)szBuf ;
	}

	if (vt & VT_ARRAY)
	{
		vt &= ~VT_ARRAY ;
		if (vt <= VT_CLSID)
			wsprintf( szBuf, "Array of %s", (LPSTR)g_rgszVT[vt] ) ;
		else
			wsprintf( szBuf, "<Unknown %08lX>", vt & VT_ARRAY ) ;
		return (LPSTR)szBuf ;
	}

	if (vt & VT_BYREF)
	{
		vt &= ~VT_BYREF ;
		if (vt <= VT_CLSID)
			wsprintf( szBuf, "%s BYREF ", (LPSTR)g_rgszVT[vt] ) ;
		else
			wsprintf( szBuf, "<Unknown %08lX>", vt & VT_BYREF ) ;
		return (LPSTR)szBuf ;
	}

	if (vt & VT_RESERVED)
	{
		vt &= ~VT_RESERVED ;
		if (vt <= VT_CLSID)
			wsprintf( szBuf, "RESERVED (%s)", (LPSTR)g_rgszVT[vt] ) ;
		else
			wsprintf( szBuf, "<Unknown %08lX>", vt & VT_RESERVED ) ;
		return (LPSTR)szBuf ;
	}

	wsprintf( szBuf, "<Unknown %08lX>", vt ) ;
	return (LPSTR)szBuf ;
}

LPSTR VTtoString2( VARTYPE vt )
{
	static char szBuf[64];

	vt &= ~0xF000 ;

	if (vt <= VT_CLSID)
		return (LPSTR)g_rgszVT[vt] ;

	wsprintf( szBuf, "<Unknown %08lX>", vt ) ;
	return (LPSTR)szBuf ;
}

LPSTR TYPEDESCtoString( TYPEDESC FAR* lptdesc )
{
	static char sz[64] ;

	lstrcpy( sz, VTtoString( lptdesc->vt ) ) ;
	if ((lptdesc->vt & 0x0FFF) == VT_PTR)
	{
		// lptdesc points to a TYPEDESC that specifies the thing pointed to
		lstrcat( sz, " to " ) ;
		lstrcat( sz, VTtoString( lptdesc->lptdesc->vt ) ) ;
		if ((lptdesc->lptdesc->vt & 0x0FFF) == VT_CARRAY)
		{
			lstrcat( sz,  " of " ) ;
			lstrcat( sz,  VTtoString( lptdesc->lptdesc->lpadesc->tdescElem.vt ) ) ;
			char szNum[16] ;
			wsprintf( szNum, "[%u]", lptdesc->lptdesc->lpadesc->cDims ) ;
			lstrcat( sz, szNum ) ;
		}
	}

	if ((lptdesc->vt & 0x0FFF) == VT_CARRAY)
	{
		lstrcat( sz,  " of " ) ;
		lstrcat( sz,  VTtoString( lptdesc->lpadesc->tdescElem.vt ) ) ;
		char szNum[16] ;
		wsprintf( szNum, "[%u]", lptdesc->lpadesc->cDims ) ;
		lstrcat( sz, szNum ) ;
	}
	return sz ;
}
