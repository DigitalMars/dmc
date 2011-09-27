
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
 * TRACK.H
 *
 * This module implements a general rectangle tracking service
 */

/* TrackRect() flags */

#define TF_LEFT                         0x0001
#define TF_TOP                          0x0002
#define TF_RIGHT                        0x0004
#define TF_BOTTOM                       0x0008
#define TF_MOVE                         0x000F

#define TF_SETPOINTERPOS        0x0010
#define TF_ALLINBOUNDARY        0x0080

BOOL TrackRect(HANDLE hInst, HWND hwnd, INT left, INT top, INT right,
        INT bottom, INT cxMin, INT cyMin, DWORD fs, LPRECT prcResult);
