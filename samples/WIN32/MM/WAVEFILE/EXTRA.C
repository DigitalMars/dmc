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
/****************************************************************************
 *
 *  EXTRA.C
 *
 *  Routines for reading and managing extra chunks in a RIFF file besides
 *  the actual stream data.
 *
 ***************************************************************************/

#define   NOCOMPMAN
#define   NODRAWDIB
#define   NOVIDEO
#define   NOAVIFMT
#define   NOMMREG
#define   NOMCIWND
#define   NOAVICAP
#define   NOMSACM

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <string.h>
#include <stdarg.h>
#include <vfw.h>

#include "extra.h"

//
// Search the EXTRA chunks for a particular chunk ID and return it's data.
//
HRESULT ReadExtra(
	LPEXTRA extra,
	DWORD ckid,
	LPVOID lpData,
	LONG FAR *lpcbData)
{

#define lpdw ((DWORD FAR *) lp)

	LPBYTE lp = (LPBYTE) extra->lp;
	LONG cb = extra->cb;

	while (cb > 0) {
	if (lpdw[0] == ckid) {
		if (!lpData) {
			*lpcbData = lpdw[1];
			return AVIERR_OK;
		}

		hmemcpy(lpData, lp + 2 * sizeof(DWORD), min((LONG) lpdw[1], *lpcbData));
		*lpcbData = lpdw[1];

		return ResultFromScode(AVIERR_OK);
	}
	cb -= lpdw[1] + sizeof(DWORD) * 2;
	lp += lpdw[1] + sizeof(DWORD) * 2;
	}
#undef lpdw
	*lpcbData = 0;
	return ResultFromScode(AVIERR_NODATA);
}

//
// Write data for the given chunk ID into the extra data
//
HRESULT WriteExtra(
	LPEXTRA extra,
	DWORD ckid,
	LPVOID lpData,
	LONG cbData)
{
	LPBYTE lp;
	
	cbData += sizeof(DWORD) * 2;
	if (extra->lp) {
	lp = (LPBYTE) GlobalReAllocPtr(extra->lp, extra->cb + cbData, GMEM_MOVEABLE);
	} else {
	lp = (LPBYTE) GlobalAllocPtr(GMEM_MOVEABLE, cbData);
	}

	if (!lp)
	return ResultFromScode(AVIERR_MEMORY);

	// build RIFF chunk in block
	((DWORD FAR *) (lp + extra->cb))[0] = ckid;
	((DWORD FAR *) (lp + extra->cb))[1] = cbData - sizeof(DWORD) * 2;
	
	hmemcpy(lp + extra->cb + sizeof(DWORD) * 2,
		lpData,
		cbData - sizeof(DWORD) * 2);
	extra->lp = lp;
	extra->cb += cbData;
	
	return ResultFromScode(AVIERR_OK);
}

//
// Reads the data from the given chunk into the EXTRA pile.
//
HRESULT ReadIntoExtra(
	LPEXTRA extra,
	HMMIO hmmio,
	MMCKINFO FAR * lpck)
{
	LPBYTE	lp;
	LONG	cbData = lpck->cksize + sizeof(DWORD) * 2;
	
	if (extra->lp) {
	lp = (LPBYTE) GlobalReAllocPtr(extra->lp, extra->cb + cbData, GMEM_MOVEABLE);
	} else {
	lp = (LPBYTE) GlobalAllocPtr(GMEM_MOVEABLE, cbData);
	}

	if (!lp)
	return ResultFromScode(AVIERR_MEMORY);

	// build RIFF chunk in block
	((DWORD FAR *) (lp + extra->cb))[0] = lpck->ckid;
	((DWORD FAR *) (lp + extra->cb))[1] = lpck->cksize;

	cbData += (cbData & 1);

	mmioSeek(hmmio, lpck->dwDataOffset, SEEK_SET);
	if (mmioRead(hmmio, (HPSTR) lp + extra->cb + sizeof(DWORD) * 2, lpck->cksize) !=
		(LONG) lpck->cksize)
	return ResultFromScode(AVIERR_FILEREAD);
	
	extra->lp = lp;
	extra->cb += cbData;
	
	return ResultFromScode(AVIERR_OK);
}


//
// Look for a specific chunk.  Throw all of the extra stuff we didn't want
// into the EXTRA pile.
//
LONG FindChunkAndKeepExtras(
	LPEXTRA extra, HMMIO hmmio,
	MMCKINFO FAR* lpck,
	MMCKINFO FAR* lpckParent,
	UINT uFlags)
{
	FOURCC		ckidFind;	// chunk ID to find (or NULL)
	FOURCC		fccTypeFind;	// form/list type to find (or NULL)
	LONG		lRet;

	/* figure out what chunk id and form/list type to search for */
	if (uFlags & MMIO_FINDCHUNK)
		ckidFind = lpck->ckid, fccTypeFind = 0;
	else if (uFlags & MMIO_FINDRIFF)
		ckidFind = FOURCC_RIFF, fccTypeFind = lpck->fccType;
	else if (uFlags & MMIO_FINDLIST)
		ckidFind = FOURCC_LIST, fccTypeFind = lpck->fccType;
	else
		ckidFind = fccTypeFind = (FOURCC) -1; // keep looking indefinitely
	
	for (;;) {
		lRet = mmioDescend(hmmio, lpck, lpckParent, 0);
		if (lRet) {
			if (uFlags == 0 && lRet == MMIOERR_CHUNKNOTFOUND)
				lRet = 0;
			return lRet;
		}

		if ((!ckidFind || lpck->ckid == ckidFind) &&
			(!fccTypeFind || lpck->fccType == fccTypeFind))
			return 0;

		lRet = (LONG) ReadIntoExtra(extra, hmmio, lpck);
		if (lRet != AVIERR_OK)
			return lRet;
	}
}
