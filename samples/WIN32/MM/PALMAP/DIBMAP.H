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

typedef DWORD * LPHISTOGRAM;
// casting ensures values > 127 get zero-extended, not sign-extended
#define RGB16(r,g,b) (\
			((WORD)(UINT)	(((int)(UINT)(r) >> 3) << 10)	|	\
				(((int)(UINT)(g) >> 3) << 5)	|	\
				(((int)(UINT)(b) >> 3) << 0)))

LPHISTOGRAM InitHistogram(
	LPHISTOGRAM lpHistogram);
void FreeHistogram(
	LPHISTOGRAM lpHistogram);
HPALETTE HistogramPalette(
	LPHISTOGRAM lpHistogram,
	LPBYTE lp16to8,
	int nColors);
BOOL DibHistogram(
	LPBITMAPINFOHEADER lpbi,
	LPBYTE lpBits,
	int x,
	int y,
	int dx,
	int dy,
	LPHISTOGRAM lpHistogram);
LPBITMAPINFOHEADER DibReduce(
	LPBITMAPINFOHEADER lpbi,
	LPBYTE lpBits,
	HPALETTE hpal,
	LPBYTE lp16to8);
void NEAR PASCAL UseHistogram(
	LPHISTOGRAM lpHistogram);
DWORD near _fastcall GetHistogram(
	BYTE r,
	BYTE g,
	BYTE b);
void near _fastcall IncHistogram(
	WORD rgb16);
