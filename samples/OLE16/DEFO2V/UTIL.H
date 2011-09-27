// util.h
//
// Header file for utiltiy functions used by DEFO2V
//
// Copyright (c) 1993 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 6, 1993
//
// InterNet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 6, 1993  cek     First implementation.
//
#ifndef _UTIL_H_
#define _UTIL_H_

/// Utilities
#define RCF_NORMAL      0x0000
#define RCF_ITALIC      0x0001
#define RCF_UNDERLINE   0x0002
#define RCF_STRIKEOUT   0x0004
#define RCF_BOLD        0x0008
#define RCF_NODEFAULT   0x0010
#define RCF_TRUETYPEONLY 0x0011

HFONT WINAPI ReallyCreateFont( HDC hDC, LPSTR lpszFace, LPSTR lpszStyle, UINT nPointSize, UINT uiFlags ) ;


void WINAPI DlgCenter( HWND hwndCenter, HWND hwndWithin, BOOL fClient ) ;

int WINAPI GetTextMetricsCorrectly( HDC hDC, LPTEXTMETRIC lpTM ) ;

typedef struct _far tagCOLUMNSTRUCT
{
   int   nLeft ;       // starting x position of the column
   int   nRight ;      // ending x position of the column
   UINT  uiFlags ;      // format flags

} COLUMNSTRUCT, *PCOLUMNSTRUCT, FAR *LPCOLUMNSTRUCT ;

void WINAPI ColumnTextOut( HDC hdc, int nX, int nY, LPSTR lpszIN, int cColumns, LPCOLUMNSTRUCT rgColumns ) ;
LPSTR HRtoString( HRESULT hr ) ;
LPSTR VTtoString( VARTYPE vt ) ;
LPSTR VTtoString2( VARTYPE vt ) ;
LPSTR TYPEDESCtoString( TYPEDESC FAR* lptdesc ) ;




#endif
