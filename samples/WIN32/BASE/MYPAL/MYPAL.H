
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// Porting macros
#ifdef WIN32
typedef POINTS  MPOINT;
#define LONG2POINT(l, pt)  ((pt).x = (SHORT)LOWORD(l), (pt).y = (SHORT)HIWORD(l))
#else
typedef POINT   MPOINT;
#define LONG2POINT(l, pt)  ((pt).x = (INT)LOWORD(l), (pt).y = (INT)HIWORD(l))
#endif


#define PALETTESIZE     256    /* Number of entries in the system palette     */

typedef struct tagRGPT {       /* Values for setting Min Max info             */
    INT iInfo[10];
} RGPT;
typedef RGPT FAR *LPRGPT;

extern VOID ShowColor(HWND, HDC);
