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

//
// To use this function:
//
// ps should be a PAVISTREAM representing a video stream that you want
// to construct a single palette for.
//
// The function will read lots of video frames from stream ps, construct
// a palette containing nColors colors, and return a new stream pointer in
// ppsMapped.
//
// The new stream will be an 8-bit video stream, which you can then
// use like any other stream.  Specifically, you can pass it to AVISave.
//
STDAPI AVICreateMappedStream(
	PAVISTREAM FAR *ppsMapped,
	PAVISTREAM ps,
	int nColors);

//
// This function is called internally by AVICreateMappedStream, but you
// might conceivably want to call it yourself.
//
STDAPI AVIStreamMakePalette(
	PAVISTREAM pavi, 
	LONG lSkip,
	HPALETTE FAR *lphpal,
	LPBYTE lp16to8,
	int nColors);
