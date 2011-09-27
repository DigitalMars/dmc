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

typedef struct FARSTRUCT {
	LPVOID	lp;
	LONG	cb;
} EXTRA, FAR * LPEXTRA;

HRESULT ReadExtra(
	LPEXTRA extra,
	DWORD ckid,
	LPVOID lpData,
	LONG FAR *lpcbData);
HRESULT WriteExtra(
	LPEXTRA extra,
	DWORD ckid,
	LPVOID lpData,
	LONG cbData);

HRESULT ReadIntoExtra(
	LPEXTRA extra,
	HMMIO hmmio,
	MMCKINFO FAR *lpck);

LONG FindChunkAndKeepExtras(
	LPEXTRA extra, 
	HMMIO hmmio, 
	MMCKINFO FAR* lpck, 
	MMCKINFO FAR* lpckParent,
	UINT uFlags);
