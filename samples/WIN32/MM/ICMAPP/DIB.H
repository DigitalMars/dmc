/*----------------------------------------------------------------------------*\
|
|   DIB.H
|
|   Routines for dealing with Device independent bitmaps                       
|
\*----------------------------------------------------------------------------*/
/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 * 
 **************************************************************************/

typedef HANDLE HDIB;

/*----------------------------------------------------------------------------*\
|   Macros                                                                     |
\*----------------------------------------------------------------------------*/

#define DibPtr(lpbi)            (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed)
#define DibColors(lpbi)         ((LPRGBQUAD)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

/*----------------------------------------------------------------------------*\
|   Functions                                                                     |
\*----------------------------------------------------------------------------*/

HDIB OpenDIB(LPSTR);
BOOL WriteDIB(LPSTR,HDIB);
HDIB DibFromBitmap(HBITMAP, DWORD, WORD, HPALETTE, WORD);
