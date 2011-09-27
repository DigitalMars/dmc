/*****************************************************************************\
*                                                                             *
* SKBAPI.H -    Screen Keyboard Public Header File                            *
*                                                                             *
*                                                                             *
*                                                                             *
*               Copyright (c) 1992-1995, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/

#ifndef _INC_WINDOWS
#include <windows.h>       /* windows.h must be preincluded */
#endif /* _INC_WINDOWS */

#ifndef _INC_SKBAPI        /* prevent multiple includes */
#define _INC_SKBAPI

/****** Screen Keyboard *****************************************************/

/* Note: WM_SKB is not supported in Pen Services 2.0 */
#ifndef WM_SKB                 /* also defined in penwin.h */
#define WM_SKB                 (WM_PENWINFIRST+4)
#endif

/* wCommand values */
#define SKB_QUERY              0x0000
#define SKB_SHOW               0x0001
#define SKB_HIDE               0x0002
#define SKB_CENTER             0x0010
#define SKB_MOVE               0x0020
#define SKB_MINIMIZE           0x0040

/* wPad values */
#define SKB_FULL               0x0100
#define SKB_BASIC              0x0200
#define SKB_NUMPAD             0x0400
#define SKB_TELPAD             0x0800
#define SKB_DEFAULT            SKB_BASIC
#define SKB_CURRENT            0x0000

/* Note: WM_SKB is not supported in Pen Services 2.0 */
/* WM_SKB wParam values */
#define SKN_CHANGED            1

/* Note: WM_SKB is not supported in Pen Services 2.0 */
/* HIWORD of lParam in WM_SKB is a combination of the following */
#define SKN_POSCHANGED         0x0001
#define SKN_PADCHANGED         0x0002
#define SKN_MINCHANGED         0x0004
#define SKN_VISCHANGED         0x0008
#define SKN_TERMINATED         0xffff

typedef struct tagSKBINFO
   {
   HWND hwnd;
   UINT nPad;
   BOOL fVisible;
   BOOL fMinimized;
   RECT rect;
   DWORD dwReserved;
   }
   SKBINFO, FAR *LPSKBINFO;

#endif /* _INC_SKBAPI */
