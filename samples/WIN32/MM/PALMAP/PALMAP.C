/****************************************************************************
 *
 *  PALMAP.C
 *
 *  Stream handler to map to a palette.
 *
 ***************************************************************************/

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

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <vfw.h>
#include <string.h>

#include "dibmap.h"
#include "palmap.h"
#include "palmap.rc"

#define	INITGUID
#include <initguid.h>

// Bring in the external GUIDs we need. Apparently compobj.lib doesn't define
DEFINE_OLEGUID(IID_IUnknown,		0x00000000L, 0, 0);


HINSTANCE	ghMod;

STDAPI AVIStreamMakePalette(
	PAVISTREAM pavi,
	LONG lSkip,
	HPALETTE FAR *lphpal,
	LPBYTE lp16to8,
	int nColors)
{
	LPHISTOGRAM lpHist = NULL;
	LONG	l, lEnd;
	LONG	lRet = AVIERR_OK;
	PGETFRAME	pgf = NULL;

	if (!pavi || !lphpal || nColors < 2 || nColors > 256)
		return ResultFromScode(AVIERR_BADPARAM);

	if (lSkip < 1)
		lSkip = 1;

	lpHist = InitHistogram(NULL);
	if (!lpHist)
		return ResultFromScode(AVIERR_MEMORY);

	pgf = AVIStreamGetFrameOpen(pavi, NULL);
	
	l = AVIStreamStart(pavi);
	lEnd = l + AVIStreamLength(pavi);
	for (l = AVIStreamStart(pavi), lEnd = l + AVIStreamLength(pavi);
		l < lEnd;
		l += lSkip) {
		LPBITMAPINFOHEADER lpbi;

		lpbi = AVIStreamGetFrame(pgf, l);

		if (!lpbi) {
			lRet = AVIERR_INTERNAL;
			goto error;
		}
	
		DibHistogram(lpbi, NULL, 0, 0, -1, -1, lpHist);
	}

	*lphpal = HistogramPalette(lpHist, lp16to8, nColors);

	if (!*lphpal)
		lRet = AVIERR_MEMORY;

error:
	if (pgf)
		AVIStreamGetFrameClose(pgf);
	
	if (lpHist)
		FreeHistogram(lpHist);

	return ResultFromScode(lRet);
}



typedef struct {
	IAVIStreamVtbl FAR *	lpVtbl;

	ULONG			ulRefCount;

	//
	// instance data
	//
	PAVISTREAM		pavi;
	PGETFRAME		pgf;
	AVISTREAMINFO		sinfo;
	HPALETTE		hpal;
	LPBYTE			lp16to8;
	LONG			lLastFrame;
	LPBITMAPINFOHEADER	lpdibLast;
} PALMAPSTREAM, FAR*PPALMAPSTREAM;

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE PalMapStreamQueryInterface(
	PAVISTREAM ps,
	REFIID riid,
	LPVOID FAR* ppvObj);
HRESULT STDMETHODCALLTYPE PalMapStreamCreate(
	PAVISTREAM ps,
	LONG lParam1,
	LONG lParam2);
ULONG	STDMETHODCALLTYPE PalMapStreamAddRef(
	PAVISTREAM ps);
ULONG	STDMETHODCALLTYPE PalMapStreamRelease(
	PAVISTREAM ps);
HRESULT STDMETHODCALLTYPE PalMapStreamInfo(
	PAVISTREAM ps,
	AVISTREAMINFOW FAR * psi,
	LONG lSize);
LONG	STDMETHODCALLTYPE PalMapStreamFindKeyFrame(
	PAVISTREAM ps,
	LONG lPos,
	LONG lFlags);
HRESULT STDMETHODCALLTYPE PalMapStreamReadFormat(
	PAVISTREAM ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG FAR *lpcbFormat);
HRESULT STDMETHODCALLTYPE PalMapStreamSetFormat(
	PAVISTREAM ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG cbFormat);
HRESULT STDMETHODCALLTYPE PalMapStreamRead(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples,
	LPVOID lpBuffer,
	LONG cbBuffer,
	LONG FAR * plBytes,
	LONG FAR * plSamples);
HRESULT STDMETHODCALLTYPE PalMapStreamWrite(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples,
	LPVOID lpBuffer,
	LONG cbBuffer,
	DWORD dwFlags,
	LONG FAR *plSampWritten,
	LONG FAR *plBytesWritten);
HRESULT STDMETHODCALLTYPE PalMapStreamDelete(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples);
HRESULT STDMETHODCALLTYPE PalMapStreamReadData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG FAR *lpcb);
HRESULT STDMETHODCALLTYPE PalMapStreamWriteData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG cb);
HRESULT STDMETHODCALLTYPE PalMapStreamSetInfo(
        PAVISTREAM ps,
        AVISTREAMINFOW FAR * psi,
        LONG lSize);

IAVIStreamVtbl PalMapStreamHandler = {
	PalMapStreamQueryInterface,
	PalMapStreamAddRef,
	PalMapStreamRelease,
	PalMapStreamCreate,
	PalMapStreamInfo,
	PalMapStreamFindKeyFrame,
	PalMapStreamReadFormat,
	PalMapStreamSetFormat,
	PalMapStreamRead,
	PalMapStreamWrite,
	PalMapStreamDelete,
	PalMapStreamReadData,
	PalMapStreamWriteData,
        PalMapStreamSetInfo
};

STDAPI AVICreateMappedStream(PAVISTREAM FAR *ppsMapped,
	PAVISTREAM ps,
	int nColors)
{
	PPALMAPSTREAM	pavi;
	HRESULT		hr;

	*ppsMapped = 0;
	
	pavi = (PPALMAPSTREAM) GlobalAllocPtr(GHND, sizeof(PALMAPSTREAM));
	if (pavi == NULL)
		return ResultFromScode(AVIERR_MEMORY);

	pavi->lpVtbl = &PalMapStreamHandler;
	
	hr = (pavi->lpVtbl->Create)((PAVISTREAM) pavi, (LONG) ps, nColors);

	if (hr != NOERROR) {
		(pavi->lpVtbl->Release)((PAVISTREAM) pavi);
		return hr;
	}
	
	*ppsMapped = (PAVISTREAM) pavi;

	return AVIERR_OK;


}

///////////////////////////////////////////////////////////////////////////
//
//  PalMapStreamOpen()
//
//  open a single stream of a particular type from a AVI file.
//
//  params:
//      szFile      - PAVISTREAM
//      fccType     - must be streamtypeVIDEO
//      lParam	    - nColors
//
//  returns:
//      a PAVISTREAM for the specifed stream or NULL.
//
///////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE PalMapStreamCreate(
	PAVISTREAM ps,
	LONG lParam1,
	LONG lParam2)
{
	PPALMAPSTREAM	pavi = (PPALMAPSTREAM) ps;
	TCHAR		achTemp[128];
	TCHAR		achTemplate[64];

	LONG		lRet = AVIERR_OK;

	pavi->ulRefCount = 1;
	
	AVIStreamAddRef((PAVISTREAM) lParam1);

	pavi->pavi = (PAVISTREAM) lParam1;
	
	AVIStreamInfo(pavi->pavi, &pavi->sinfo, sizeof(pavi->sinfo));

	if (pavi->sinfo.fccType != streamtypeVIDEO) {
	lRet = AVIERR_INTERNAL;
	goto error;
	}

	pavi->pgf = AVIStreamGetFrameOpen(pavi->pavi, NULL);

	if (!pavi->pgf) {
	lRet = AVIERR_INTERNAL;
	goto error;
	}

	pavi->sinfo.fccHandler = 0;

	// Fix up stream name
	LoadString(ghMod, IDS_STREAMNAME, achTemplate,
			sizeof(achTemplate) / sizeof(TCHAR));
	wsprintf(achTemp, achTemplate, pavi->sinfo.szName, lParam2);
	lstrcpyn(pavi->sinfo.szName, achTemp,
			sizeof(pavi->sinfo.szName) / sizeof(TCHAR));
	pavi->sinfo.szName[sizeof(pavi->sinfo.szName) / sizeof(TCHAR) - 1] =
                TEXT('\0');

	// default to 256 colors
	if (lParam2 < 2 || lParam2 > 256)
	lParam2 = 256;

	pavi->lp16to8 = GlobalAllocPtr(GMEM_MOVEABLE, 32768L);
	if (!pavi->lp16to8) {
	lRet = AVIERR_MEMORY;
	goto error;
	}
	
	lRet = GetScode(AVIStreamMakePalette(pavi->pavi,
				AVIStreamLength(pavi->pavi) / 30,
				&pavi->hpal, pavi->lp16to8,
				(int) lParam2));

	pavi->lLastFrame = -1;
	
error:
	return ResultFromScode(lRet);
}

///////////////////////////////////////////////////////////////////////////
//
//  PalMapStreamQueryInterface()
//
//      let other people know what interfaces we support
//
///////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE PalMapStreamQueryInterface(
	PAVISTREAM ps,
	REFIID riid,
	LPVOID FAR* ppvObj)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	
	if (IsEqualIID(riid, &IID_IUnknown))
		*ppvObj = ps;
	else if (IsEqualIID(riid, &IID_IAVIStream))
		*ppvObj = ps;
	else
		return ResultFromScode(E_NOINTERFACE);
	
	pavi->ulRefCount++;
	return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
//
//  PalMapStreamAddRef()
//
//      increase the reference count of the stream
//
///////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE PalMapStreamAddRef(
	PAVISTREAM ps)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;

	return ++pavi->ulRefCount;
}

///////////////////////////////////////////////////////////////////////////
//
//  PalMapStreamRelease()
//
//      close a PalMapStream stream
//
///////////////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE PalMapStreamRelease(
	PAVISTREAM ps)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;

	if (!pavi)
		return 0;
	
	if (--pavi->ulRefCount)
		return pavi->ulRefCount;

	if (pavi->pgf)
		AVIStreamGetFrameClose(pavi->pgf);
	pavi->pgf = 0;
	
	if (pavi->pavi)
		AVIStreamClose(pavi->pavi);
	pavi->pavi = 0;

	if (pavi->lp16to8) {
		GlobalFreePtr(pavi->lp16to8);
		pavi->lp16to8 = 0;
	}

	if (pavi->hpal) {
		DeletePalette(pavi->hpal);
		pavi->hpal = 0;
	}
	
	if (pavi->lpdibLast) {
		GlobalFreePtr(pavi->lpdibLast);
		pavi->lpdibLast = 0;
	}
	
	GlobalFreePtr(pavi);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE PalMapStreamReadFormat(
	PAVISTREAM ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG FAR *lpcbFormat)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	LONG			lSize;
	
	PalMapStreamRead(ps, lPos, 1, NULL, 0, NULL, NULL);

	if (pavi->lpdibLast == 0)
		return ResultFromScode(AVIERR_INTERNAL);
	lSize = pavi->lpdibLast->biSize
		+ pavi->lpdibLast->biClrUsed * sizeof(RGBQUAD);

	if (lpFormat)
		hmemcpy(lpFormat, pavi->lpdibLast,
			min(*lpcbFormat, lSize));

	*lpcbFormat = lSize;
	
	return 0;
}

LONG STDMETHODCALLTYPE PalMapStreamFindKeyFrame(
	PAVISTREAM ps,
	LONG lPos,
	LONG lFlags)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;

	return lPos;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE PalMapStreamInfo(
	PAVISTREAM ps,
	AVISTREAMINFOW FAR * psi,        // OLE interfaces are ALWAYS UNICODE
	LONG lSize)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;

	if (psi)
	hmemcpy(psi, &pavi->sinfo, min(lSize, sizeof(pavi->sinfo)));
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE PalMapStreamRead(
	PAVISTREAM	ps,
	LONG		lStart,
	LONG		lSamples,
	LPVOID		lpBuffer,
	LONG		cbBuffer,
	LONG FAR *	plBytes,
	LONG FAR *	plSamples)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;

	LPBITMAPINFOHEADER	lpbi;
	LPVOID			lp;

	if (lStart != pavi->lLastFrame) {
		pavi->lLastFrame = -1;
		lpbi = AVIStreamGetFrame(pavi->pgf, lStart);
	
		if (!lpbi)
			goto ReadNothing;

		if (pavi->lpdibLast) {
			GlobalFreePtr(pavi->lpdibLast);
			pavi->lpdibLast = 0;
		}

		pavi->lpdibLast = DibReduce(lpbi, NULL, pavi->hpal, pavi->lp16to8);
		pavi->lLastFrame = lStart;
	}

	lpbi = pavi->lpdibLast;	
	//
	// a NULL buffer means return the size buffer needed to read
	// the given sample.
	//
	lp = (LPBYTE) lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);
	
	if (plBytes)
		*plBytes = lpbi->biSizeImage;

	if (plSamples)
		*plSamples = 1;

	if (lpBuffer) {
		if (cbBuffer >= (LONG) lpbi->biSizeImage)
			hmemcpy(lpBuffer, lp, lpbi->biSizeImage);
		else
			goto ReadNothing;
	}
	
	return 0;
	
ReadNothing:
	if (plBytes)
		*plBytes = 0;

	if (plSamples)
		*plSamples = 0;

	return ResultFromScode(AVIERR_BUFFERTOOSMALL);
}



//
//
// Extra unimplemented functions.....
//
//
//
HRESULT STDMETHODCALLTYPE PalMapStreamReadData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG FAR *lpcb)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

HRESULT STDMETHODCALLTYPE PalMapStreamSetFormat(
	PAVISTREAM ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG cbFormat)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

HRESULT STDMETHODCALLTYPE PalMapStreamWriteData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG cb)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

HRESULT STDMETHODCALLTYPE PalMapStreamWrite(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples,
	LPVOID lpBuffer,
	LONG cbBuffer,
	DWORD dwFlags,
	LONG FAR *plSampWritten,
	LONG FAR *plBytesWritten)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

HRESULT STDMETHODCALLTYPE PalMapStreamDelete(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples)
{
	PPALMAPSTREAM pavi = (PPALMAPSTREAM) ps;
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

HRESULT STDMETHODCALLTYPE PalMapStreamSetInfo(
   PAVISTREAM ps,
   AVISTREAMINFOW FAR * psi,
   LONG lSize)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

EXTERN_C int CALLBACK WEP(
	BOOL fSystemExit)
{
	return TRUE;
}


EXTERN_C BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID);
EXTERN_C BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD dwReason,
	LPVOID lpReserved )
{
	switch( dwReason)
	{
		case DLL_PROCESS_ATTACH:
			if(ghMod == NULL)
				ghMod = (HMODULE)hModule;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
