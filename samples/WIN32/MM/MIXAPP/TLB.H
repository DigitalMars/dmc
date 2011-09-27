//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  tlb.h
//
//==========================================================================;

#ifndef _INC_TLB
#define _INC_TLB                    // #defined if file has been included

#ifndef RC_INVOKED
#pragma warning(disable:4103)
#pragma pack(1)                     // assume byte packing throughout
#endif

#ifndef EXTERN_C
#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C extern
#endif
#endif

#ifdef __cplusplus
extern "C"                          // assume C declarations for C++
{
#endif

//
//  for compiling Unicode
//
#ifndef SIZEOF
#ifdef UNICODE
    #define SIZEOF(x)       (sizeof(x)/sizeof(WCHAR))
#else
    #define SIZEOF(x)       sizeof(x)
#endif
#endif


//
//
//
//
typedef struct tTABBEDLISTBOX
{
    HWND            hlb;

    int             nFontHeight;
    RECT            rc;

    UINT            uTabStops;
    PINT            panTabs;
    PINT            panTitleTabs;

    UINT            cchTitleText;
    PTSTR           pszTitleText;

} TABBEDLISTBOX, *PTABBEDLISTBOX;


#define TLB_MAX_TAB_STOPS           20      // max number of columns
#define TLB_MAX_TITLE_CHARS         512


//
//
//
//
//
BOOL FAR PASCAL TlbPaint
(
    PTABBEDLISTBOX          ptlb,
    HWND                    hwnd,
    HDC                     hdc
);

BOOL FAR PASCAL TlbMove
(
    PTABBEDLISTBOX          ptlb,
    PRECT                   prc,
    BOOL                    fRedraw
);

HFONT FAR PASCAL TlbSetFont
(
    PTABBEDLISTBOX          ptlb,
    HFONT                   hfont,
    BOOL                    fRedraw
);

BOOL FAR PASCAL TlbSetTitleAndTabs
(
    PTABBEDLISTBOX          ptlb,
    PTSTR                   pszTitleFormat,
    BOOL                    fRedraw
);

PTABBEDLISTBOX FAR PASCAL TlbDestroy
(
    PTABBEDLISTBOX          ptlb
);

PTABBEDLISTBOX FAR PASCAL TlbCreate
(
    HWND                    hwnd,
    int                     nId,
    PRECT                   prc
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#ifndef RC_INVOKED
#pragma pack()                      // revert to default packing
#endif

#ifdef __cplusplus
}                                   // end of extern "C" {
#endif

#endif // _INC_TLB
