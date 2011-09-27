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
 *  WAVEFILE.C
 *
 *  An implementation in C of an AVI File Handler to read standard windows
 *  WAV files as if they were an AVI file with one audio stream.
 *
 ***************************************************************************/

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <vfw.h>
#include "MulDiv32.h"
#include "wavefile.h"

#define formtypeWAVE	mmioFOURCC('W', 'A', 'V', 'E')
#define ckidWAVEFORMAT	mmioFOURCC('f', 'm', 't', ' ')
#define ckidWAVEDATA	mmioFOURCC('d', 'a', 't', 'a')

LPSTR FAR FileName(LPCSTR lszPath);
//
// Function prototypes and Vtbl for the Unknown interface
//
STDMETHODIMP WaveUnknownQueryInterface(LPUNKNOWN pu, REFIID iid, void FAR* FAR* ppv);
STDMETHODIMP_(ULONG) WaveUnknownAddRef(LPUNKNOWN pu);
STDMETHODIMP_(ULONG) WaveUnknownRelease(LPUNKNOWN pu);

IUnknownVtbl UnknownVtbl = {
	WaveUnknownQueryInterface,
	WaveUnknownAddRef,
	WaveUnknownRelease
};

//
// Function prototypes and Vtbl for the AVIFile interface
//
STDMETHODIMP WaveFileQueryInterface(PAVIFILE pf, REFIID iid, void FAR* FAR* ppv);
STDMETHODIMP_(ULONG) WaveFileAddRef(PAVIFILE pf);
STDMETHODIMP_(ULONG) WaveFileRelease(PAVIFILE pf);
STDMETHODIMP WaveFileInfo(PAVIFILE pf, AVIFILEINFOW FAR * pfi, LONG lSize);
STDMETHODIMP WaveFileGetStream(PAVIFILE pf, PAVISTREAM FAR * ppavi, DWORD fccType, LONG lParam);
STDMETHODIMP WaveFileCreateStream(PAVIFILE pf, PAVISTREAM FAR *ppstream, AVISTREAMINFOW FAR *psi);
STDMETHODIMP WaveFileWriteData(PAVIFILE pf, DWORD ckid, LPVOID lpData, LONG cbData);
STDMETHODIMP WaveFileReadData(PAVIFILE pf, DWORD ckid, LPVOID lpData, LONG FAR *lpcbData);
STDMETHODIMP WaveFileEndRecord(PAVIFILE pf);
STDMETHODIMP WaveFileDeleteStream(PAVIFILE pf, DWORD fccType, LONG lParam);


IAVIFileVtbl FileVtbl = {
	WaveFileQueryInterface,
	WaveFileAddRef,
	WaveFileRelease,
	WaveFileInfo,
	WaveFileGetStream,
	WaveFileCreateStream,
	WaveFileWriteData,
	WaveFileReadData,
	WaveFileEndRecord,
	WaveFileDeleteStream
};


STDMETHODIMP WavePersistQueryInterface(LPPERSISTFILE pf, REFIID iid, void FAR* FAR* ppv);
STDMETHODIMP_(ULONG) WavePersistAddRef(LPPERSISTFILE pf);
STDMETHODIMP_(ULONG) WavePersistRelease(LPPERSISTFILE pf);
STDMETHODIMP WavePersistGetClassID (LPPERSISTFILE ppf, LPCLSID lpClassID);
STDMETHODIMP WavePersistIsDirty (LPPERSISTFILE ppf);
STDMETHODIMP WavePersistLoad (LPPERSISTFILE ppf,
			      LPCOLESTR lpszFileName, DWORD grfMode);
STDMETHODIMP WavePersistSave (LPPERSISTFILE ppf,
			      LPCOLESTR lpszFileName, BOOL fRemember);
STDMETHODIMP WavePersistSaveCompleted (LPPERSISTFILE ppf,
				       LPCOLESTR lpszFileName);
STDMETHODIMP WavePersistGetCurFile (LPPERSISTFILE ppf,
				    LPOLESTR FAR * lplpszFileName);


IPersistFileVtbl PersistVtbl = {
	WavePersistQueryInterface,
	WavePersistAddRef,
	WavePersistRelease,
	WavePersistGetClassID,
	WavePersistIsDirty,
	WavePersistLoad,
	WavePersistSave,
	WavePersistSaveCompleted,
	WavePersistGetCurFile
};

//
// Function prototypes and Vtbl for the AVIStream interface
//
STDMETHODIMP WaveStreamQueryInterface(PAVISTREAM ps, REFIID riid, LPVOID FAR* ppvObj);
STDMETHODIMP WaveStreamCreate(PAVISTREAM ps, LONG lParam1, LONG lParam2);
STDMETHODIMP_(ULONG) WaveStreamAddRef(PAVISTREAM ps);
STDMETHODIMP_(ULONG) WaveStreamRelease(PAVISTREAM ps);
STDMETHODIMP WaveStreamInfo(PAVISTREAM ps, AVISTREAMINFOW FAR * psi, LONG lSize);
STDMETHODIMP_(LONG) WaveStreamFindSample(PAVISTREAM ps, LONG lPos, LONG lFlags);
STDMETHODIMP WaveStreamReadFormat(PAVISTREAM ps, LONG lPos, LPVOID lpFormat, LONG FAR *lpcbFormat);
STDMETHODIMP WaveStreamSetFormat(PAVISTREAM ps, LONG lPos, LPVOID lpFormat, LONG cbFormat);
STDMETHODIMP WaveStreamRead(PAVISTREAM ps, LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG FAR * plBytes,LONG FAR * plSamples);
STDMETHODIMP WaveStreamWrite(PAVISTREAM ps, LONG lStart, LONG lSamples, LPVOID lpData, LONG cbData, DWORD dwFlags, LONG FAR *plSampWritten, LONG FAR *plBytesWritten);
STDMETHODIMP WaveStreamDelete(PAVISTREAM ps, LONG lStart, LONG lSamples);
STDMETHODIMP WaveStreamReadData(PAVISTREAM ps, DWORD fcc, LPVOID lp,LONG FAR *lpcb);
STDMETHODIMP WaveStreamWriteData(PAVISTREAM ps, DWORD fcc, LPVOID lp,LONG cb);
STDMETHODIMP WaveStreamSetInfo(PAVISTREAM ps, AVISTREAMINFOW FAR * psi, LONG lSize);

IAVIStreamVtbl StreamVtbl = {
	WaveStreamQueryInterface,
	WaveStreamAddRef,
	WaveStreamRelease,
	WaveStreamCreate,
	WaveStreamInfo,
	WaveStreamFindSample,
	WaveStreamReadFormat,
	WaveStreamSetFormat,
	WaveStreamRead,
	WaveStreamWrite,
	WaveStreamDelete,
	WaveStreamReadData,
	WaveStreamWriteData,
	WaveStreamSetInfo
};


#if !defined UNICODE

int LoadUnicodeString(HINSTANCE hinst, UINT wID, LPWSTR lpBuffer, int cchBuffer)
{
    char    ach[128];
    int	    i;

    i = LoadString(hinst, wID, ach, sizeof(ach));

    if (i > 0)
	MultiByteToWideChar(CP_ACP, 0, ach, -1, lpBuffer, cchBuffer);

    return i;
}

#else
#define LoadUnicodeString   LoadString
#endif


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*	-	-	-	-	-	-	-	-	*/

UINT	uUseCount;	// the reference count for our objects
UINT	uLockCount;	// our lock count for LockServer

/*	-	-	-	-	-	-	-	-	*/

//
// Create a new instance.  Since this is a C implementation we have to
// allocate space for our structure ourselves.
//
HRESULT WaveFileCreate(
	IUnknown FAR*	pUnknownOuter,
	REFIID		riid,
	void FAR* FAR*	ppv)
{
	IUnknown FAR*	pUnknown;
	LPWAVESTUFF	pWaveStuff;
	HRESULT	hresult;

	// Allocate space for our structure
	pWaveStuff = (LPWAVESTUFF)GlobalAllocPtr(GMEM_MOVEABLE,
		sizeof(WAVESTUFF));
	if (!pWaveStuff)
		return ResultFromScode(E_OUTOFMEMORY);

	// Initialize the Vtbls
	pWaveStuff->AVIFile = &FileVtbl;
	pWaveStuff->AVIStream = &StreamVtbl;
	pWaveStuff->Unknown = &UnknownVtbl;
	pWaveStuff->Persist = &PersistVtbl;

	// Set up our controlling object
	pUnknown = (IUnknown FAR *)&pWaveStuff->Unknown;
	if (pUnknownOuter)
		pWaveStuff->pUnknownOuter = pUnknownOuter;
	else
		pWaveStuff->pUnknownOuter =(IUnknown FAR *)&pWaveStuff->Unknown;

	// Initial the things in our structure
	pWaveStuff->refs = 0;
	pWaveStuff->hmmio = NULL;
	pWaveStuff->lpFormat = NULL;
	pWaveStuff->cbFormat = 0L;
	pWaveStuff->fDirty = FALSE;
	pWaveStuff->extra.lp = NULL;
	pWaveStuff->extra.cb = 0L;

	// Call our Query interface to increment our ref count and get a
	// pointer to our interface to return.
	hresult = pUnknown->lpVtbl->QueryInterface(pUnknown, riid, ppv);

	if (FAILED(GetScode(hresult)))
		GlobalFreePtr(pWaveStuff);
	return hresult;
}

/*	-	-	-	-	-	-	-	-	*/

//
// Query interface from all three interfaces comes here.  We support the
// Unknown interface, AVIStream and AVIFile.
//
STDMETHODIMP WaveUnknownQueryInterface(
	LPUNKNOWN	pu,
	REFIID		iid,
	void FAR* FAR*	ppv)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_UNKNOWN(pu);

	if (IsEqualIID(iid, &IID_IUnknown))
		*ppv = (LPVOID)&pWaveStuff->Unknown;
	else if (IsEqualIID(iid, &IID_IAVIFile))
		*ppv = (LPVOID)&pWaveStuff->AVIFile;
	else if (IsEqualIID(iid, &IID_IAVIStream))
		*ppv = (LPVOID)&pWaveStuff->AVIStream;
	else if (IsEqualIID(iid, &IID_IPersistFile))
		*ppv = (LPVOID)&pWaveStuff->Persist;
	else
		return ResultFromScode(E_NOINTERFACE);
	pu->lpVtbl->AddRef(pu);
	return NOERROR;
}

/*	-	-	-	-	-	-	-	-	*/

//
// Increase our reference count.  AddRef for all three interfaces comes here.
//
STDMETHODIMP_(ULONG) WaveUnknownAddRef(
	LPUNKNOWN	pu)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_UNKNOWN(pu);

	uUseCount++;
	return ++pWaveStuff->refs;
}

/*	-	-	-	-	-	-	-	-	*/

//
// Decrease our reference count.  Release for all three interfaces comes here.
//
STDMETHODIMP_(ULONG) WaveUnknownRelease(
	LPUNKNOWN pu)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_UNKNOWN(pu);

	uUseCount--;

	//
	// Ref count is zero.  Close the file.  If we've been writing to it, it's
	// clean-up time!
	//
	if (!--p->refs) {
	LONG lRet = AVIERR_OK;
	
	if (p->fDirty) {
		MMCKINFO ckRIFF;
		MMCKINFO ck;

		mmioSeek(p->hmmio, 0, SEEK_SET);

		/* create the output file RIFF chunk of form type 'WAVE' */
		ckRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		ckRIFF.cksize = 0L;	// let MMIO figure out ck. size
		if (mmioCreateChunk(p->hmmio, &ckRIFF, MMIO_CREATERIFF) != 0)
			goto ERROR_CANNOT_WRITE;	// cannot write file, probably

		ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
		ck.cksize = p->cbFormat;		// we know the size of this ck.
		if (mmioCreateChunk(p->hmmio, &ck, 0) != 0)
		goto ERROR_CANNOT_WRITE;	// cannot write file, probably

		if (mmioWrite(p->hmmio, (HPSTR) p->lpFormat, p->cbFormat) != p->cbFormat)
		goto ERROR_CANNOT_WRITE;	// cannot write file, probably

		/* ascend out of the 'fmt' chunk, back into 'RIFF' chunk */
		if (mmioAscend(p->hmmio, &ck, 0) != 0)
		goto ERROR_CANNOT_WRITE;	// cannot write file, probably

		// If there was extra stuff here, we need to fill it!
		if (mmioSeek(p->hmmio, 0, SEEK_CUR)
			+ 2 * (LRESULT)sizeof(DWORD)
			!= (LRESULT) p->ckData.dwDataOffset) {
			/* create the 'data' chunk that holds the waveform samples */
			ck.ckid = mmioFOURCC('J', 'U', 'N', 'K');
			ck.cksize = 0;
			if (mmioCreateChunk(p->hmmio, &ck, 0) != 0)
				goto ERROR_CANNOT_WRITE;	// cannot write file, probably

			mmioSeek(p->hmmio,
				p->ckData.dwDataOffset - 2 * sizeof(DWORD),
				SEEK_SET);

			if (mmioAscend(p->hmmio, &ck, 0) != 0)
				goto ERROR_CANNOT_WRITE;	// cannot write file, probably
		}

		/* create the 'data' chunk that holds the waveform samples */
		ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ck.cksize = p->ckData.cksize;
		if (mmioCreateChunk(p->hmmio, &ck, 0) != 0)
		goto ERROR_CANNOT_WRITE;	// cannot write file, probably

		mmioSeek(p->hmmio, p->ckData.cksize, SEEK_CUR);

		mmioAscend(p->hmmio, &ck, 0);

		if (p->extra.cb) {
		if (mmioWrite(p->hmmio, (HPSTR) p->extra.lp, p->extra.cb) != p->extra.cb)
			goto ERROR_CANNOT_WRITE;
		}

		if (mmioAscend(p->hmmio, &ckRIFF, 0) != 0)
		goto ERROR_CANNOT_WRITE;

		if (mmioFlush(p->hmmio, 0) != 0)
		goto ERROR_CANNOT_WRITE;
	}


	goto success;

	ERROR_CANNOT_WRITE:
	lRet = AVIERR_FILEWRITE;

	success:
	if (p->hmmio)
		mmioClose(p->hmmio, 0);

	if (p->lpFormat)
		GlobalFreePtr(p->lpFormat);

	// Free the memory for our structure.
	GlobalFreePtr(p);
	return 0;
	}
	return p->refs;
}


//
// Use our controlling object to call QueryInterface on Unknown
//
STDMETHODIMP WaveFileQueryInterface(
	PAVIFILE	pf,
	REFIID		iid,
	void FAR* FAR*	ppv)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_FILE(pf);

	return pWaveStuff->pUnknownOuter->lpVtbl->QueryInterface(
		pWaveStuff->pUnknownOuter, iid, ppv);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call AddRef on Unknown
//
STDMETHODIMP_(ULONG) WaveFileAddRef(
	PAVIFILE	pf)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_FILE(pf);

	return pWaveStuff->pUnknownOuter->lpVtbl->AddRef(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call Release on Unknown
//
STDMETHODIMP_(ULONG) WaveFileRelease(
	PAVIFILE	pf)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_FILE(pf);

	return pWaveStuff->pUnknownOuter->lpVtbl->Release(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/


//
// Use our controlling object to call QueryInterface on Unknown
//
STDMETHODIMP WavePersistQueryInterface(
	LPPERSISTFILE	ppf,
	REFIID		iid,
	void FAR* FAR*	ppv)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_PERSIST(ppf);

	return pWaveStuff->pUnknownOuter->lpVtbl->QueryInterface(
		pWaveStuff->pUnknownOuter, iid, ppv);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call AddRef on Unknown
//
STDMETHODIMP_(ULONG) WavePersistAddRef(
	LPPERSISTFILE	ppf)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_PERSIST(ppf);

	return pWaveStuff->pUnknownOuter->lpVtbl->AddRef(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call Release on Unknown
//
STDMETHODIMP_(ULONG) WavePersistRelease(
	LPPERSISTFILE	ppf)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_PERSIST(ppf);

	return pWaveStuff->pUnknownOuter->lpVtbl->Release(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/



//
// Use our controlling object to call QueryInterface on Unknown
//
STDMETHODIMP WaveStreamQueryInterface(
	PAVISTREAM	ps,
	REFIID		iid,
	void FAR* FAR*	ppv)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_STREAM(ps);

	return pWaveStuff->pUnknownOuter->lpVtbl->QueryInterface(
		pWaveStuff->pUnknownOuter, iid, ppv);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call AddRef on Unknown
//
STDMETHODIMP_(ULONG) WaveStreamAddRef(
	PAVISTREAM	ps)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_STREAM(ps);

	return pWaveStuff->pUnknownOuter->lpVtbl->AddRef(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/

//
// Use our controlling object to call Release on Unknown
//
STDMETHODIMP_(ULONG) WaveStreamRelease(
	PAVISTREAM	ps)
{
	// Get a pointer to our structure
	LPWAVESTUFF pWaveStuff = WAVESTUFF_FROM_STREAM(ps);

	return pWaveStuff->pUnknownOuter->lpVtbl->Release(
		pWaveStuff->pUnknownOuter);
}

/*	-	-	-	-	-	-	-	-	*/

#define SLASH(c)	((c) == '/' || (c) == '\\')

/*--------------------------------------------------------------+
| FileName  - return a pointer to the filename part of szPath   |
|             with no preceding path.                           |
+--------------------------------------------------------------*/
LPSTR FAR FileName(
	LPCSTR lszPath)
{
	LPCSTR lszCur;

	for (lszCur = lszPath + lstrlen(lszPath); lszCur > lszPath && !SLASH(*lszCur) && *lszCur != ':';)
	lszCur = AnsiPrev(lszPath, lszCur);
	if (lszCur == lszPath)
	return (LPSTR)lszCur;
	else
	return (LPSTR)(lszCur + 1);
}

/*	-	-	-	-	-	-	-	-	*/

//
// The Open Method for our File interface - Open a WAVE file
//
STDMETHODIMP WaveFileOpen(
	PAVIFILE pf,
	LPCSTR szFile,
	UINT mode)
{
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);
	UINT	ui;
	char	ach[80];

	// !!! Assumptions about the AVIFILE.DLL (which calls us):
	// We will only see READWRITE mode, never only WRITE mode.

	// force the share flags to the 'correct' values
	// If we're writing, use Exclusive mode.  If we're reading, use DenyWrite.
	if (mode & OF_READWRITE) {
		mode = (mode & ~(MMIO_SHAREMODE)) | OF_SHARE_EXCLUSIVE;
	} else {
		mode = (mode & ~(MMIO_SHAREMODE)) | OF_SHARE_DENY_WRITE;
	}

	//
	// try to open the actual file, first with share, then without.
	// You may need to use specific flags in order to open a file
	// that's already open by somebody else.
	//

	// If the first attempt fails, no system error box, please.
	ui = SetErrorMode(SEM_NOOPENFILEERRORBOX);
	p->hmmio = mmioOpen((LPSTR) szFile, NULL, MMIO_ALLOCBUF | mode);
	if (!p->hmmio && ((mode & MMIO_RWMODE) == OF_READ)) {
	// if the open fails, try again without the share flags.
		mode &= ~(MMIO_SHAREMODE);
		p->hmmio = mmioOpen((LPSTR) szFile, NULL, MMIO_ALLOCBUF | mode);
	}
	SetErrorMode(ui);
	
	//
	// Now set up our structure
	//

	p->mode = mode;
	
	if (!p->hmmio)
		goto error;

	_fmemset(&p->avistream, 0, sizeof(p->avistream));

// If this is defined, we pretend that the data is at FPSHACK "frames"
// per second in the main header, otherwise we use the sample
// rate of the audio, which looks somewhat strange in MPlayer.
#define FPSHACK	1000
	
	_fmemset(&p->avihdr, 0, sizeof(p->avihdr));

#ifdef FPSHACK
	//
	// Initialize our AVIFILEHEADER
	//
	p->avihdr.dwRate = FPSHACK;
	p->avihdr.dwScale = 1;
#endif
	
	p->avihdr.dwStreams = 1;
	LoadUnicodeString(ghModule, IDS_FILETYPE, p->avihdr.szFileType,
	sizeof(p->avihdr.szFileType));
	
	//
	// Initialize our AVISTREAMHEADER
	//
	LoadString(ghModule, IDS_STREAMNAME, ach, sizeof(ach));
#if !defined UNICODE
	{
	    char    achTemp[64];

	    wsprintf(achTemp, ach, FileName(szFile));

	    MultiByteToWideChar(CP_ACP, 0, achTemp, -1,
			p->avistream.szName, 64);
	}
#else
	wsprintf(p->avistream.szName, ach, FileName(szFile));
#endif

	if (mode & OF_CREATE) {	// Brand new file
		p->avistream.fccType = streamtypeAUDIO;
		p->avistream.fccHandler = 0;
		p->avistream.dwFlags = 0;
		p->avistream.wPriority = 0;
		p->avistream.wLanguage = 0;
		p->avistream.dwInitialFrames = 0;
		p->avistream.dwScale = 0;
		p->avistream.dwRate = 0;
		p->avistream.dwStart = 0;
		p->avistream.dwLength = 0;
		p->avistream.dwSuggestedBufferSize = 0;
		p->avistream.dwSampleSize = 0;
	
		p->fDirty = TRUE;
	
	} else {		// read the existing file to get info

		MMCKINFO	ck;
		MMCKINFO	ckRIFF;
		/* Read RIFF chunk */
		if (mmioDescend(p->hmmio, &ckRIFF, NULL, 0) != 0)
			goto error;

		if (ckRIFF.ckid != FOURCC_RIFF)
			goto error;

		if (ckRIFF.fccType != formtypeWAVE)
			goto error;

		/* Read WAVE format chunk */
		ck.ckid = ckidWAVEFORMAT;
		if (FindChunkAndKeepExtras(&p->extra, p->hmmio, &ck, &ckRIFF, MMIO_FINDCHUNK))
			goto error;

		p->cbFormat = ck.cksize;
		p->lpFormat = (LPWAVEFORMAT) GlobalAllocPtr(GMEM_MOVEABLE, ck.cksize);

		if (p->lpFormat == NULL)
			goto error;

		if (mmioRead(p->hmmio,
			(HPSTR) p->lpFormat,
			(LONG)ck.cksize) != (LONG)ck.cksize)
			goto error;


		/* Ascend out of stream header */
		if (mmioAscend(p->hmmio, &ck, 0) != 0)
			goto error;

		/* Find big data chunk */
		p->ckData.ckid = ckidWAVEDATA;
		if (FindChunkAndKeepExtras(&p->extra, p->hmmio, &p->ckData, &ckRIFF, MMIO_FINDCHUNK))
			goto error;

		p->fDirty = FALSE;
	
		p->avistream.fccType = streamtypeAUDIO;
		p->avistream.fccHandler = 0;
		p->avistream.dwFlags = 0;
		p->avistream.wPriority = 0;
		p->avistream.wLanguage = 0;
		p->avistream.dwInitialFrames = 0;
		p->avistream.dwScale = p->lpFormat->nBlockAlign;
		p->avistream.dwRate = p->lpFormat->nAvgBytesPerSec;
		p->avistream.dwStart = 0;
		p->avistream.dwLength = p->ckData.cksize / p->lpFormat->nBlockAlign;
		p->avistream.dwSuggestedBufferSize = 0;
		p->avistream.dwSampleSize = p->lpFormat->nBlockAlign;
	
#ifdef FPSHACK
		p->avihdr.dwLength = MulDiv32(p->avistream.dwLength,
					p->avistream.dwScale * FPSHACK,
					p->avistream.dwRate);
#else
		p->avihdr.dwScale = 1;
		p->avihdr.dwRate = p->lpFormat->nSamplesPerSec;
		p->avihdr.dwLength = MulDiv32(p->ckData.cksize,
					p->lpFormat->nSamplesPerSec,
					p->lpFormat->nAvgBytesPerSec);
#endif


		mmioAscend(p->hmmio, &p->ckData, 0);
	
		// Read extra data at end of file....
		if (FindChunkAndKeepExtras(&p->extra, p->hmmio, &ckRIFF, &ck, 0)
			!= AVIERR_OK)
			goto error;
	}
	
	//
	// all done return success.
	//
	return ResultFromScode(0); // success
	
error:
	return ResultFromScode(AVIERR_FILEREAD);
}


//
// Get a stream from the file... Each WAVE file has exactly 1 audio stream.
//
STDMETHODIMP WaveFileGetStream(
	PAVIFILE pf,
	PAVISTREAM FAR * ppavi,
	DWORD fccType,
	LONG lParam)
{
	int iStreamWant;
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	iStreamWant = (int)lParam;

	if (p->lpFormat == NULL)
		return ResultFromScode(AVIERR_BADPARAM);
	
	// We only support one stream
	if (iStreamWant != 0)
		return ResultFromScode(AVIERR_BADPARAM);

	// We only support audio streams
	if (fccType && fccType != streamtypeAUDIO)
		return ResultFromScode(AVIERR_BADPARAM);

	// increase the reference count
	p->AVIStream->AddRef((PAVISTREAM)&p->AVIStream);
	
	// Return a pointer to our stream Vtbl
	*ppavi = (PAVISTREAM) &(p->AVIStream);
	return ResultFromScode(AVIERR_OK);
}


STDMETHODIMP WaveFileDeleteStream(PAVIFILE pf, DWORD fccType, LONG lParam)
{
	int iStreamWant;
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	iStreamWant = (int)lParam;

	if (p->lpFormat == NULL)
		return ResultFromScode(AVIERR_BADPARAM);
	
	// We only support one stream
	if (iStreamWant != 0)
		return ResultFromScode(AVIERR_BADPARAM);

	// We only support audio streams
	if (fccType && fccType != streamtypeAUDIO)
		return ResultFromScode(AVIERR_BADPARAM);


	GlobalFreePtr(p->lpFormat);
	p->lpFormat = NULL;

	return NOERROR;
}

//
// We don't support the Save Method of the File Interface (We don't save)
//
STDMETHODIMP WaveFileSave(
	PAVIFILE pf,
	LPCSTR szFile,
	AVICOMPRESSOPTIONS FAR *lpOptions,
	AVISAVECALLBACK lpfnCallback)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

//
// Method to create a stream in a WAVE file.  We only support this for blank
// WAVE files.
//
STDMETHODIMP WaveFileCreateStream(
	PAVIFILE pf,
	PAVISTREAM FAR *ppstream,
	AVISTREAMINFOW FAR *psi)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	// We can't add a second stream to a file
	if (p->lpFormat)
		return ResultFromScode(AVIERR_UNSUPPORTED);

	// We only like audio....
	if (psi->fccType != streamtypeAUDIO)
		return ResultFromScode(AVIERR_UNSUPPORTED);
	
	// Increase our reference count.
	p->AVIStream->AddRef((PAVISTREAM)&p->AVIStream);

	p->cbFormat = 0;
	p->lpFormat = NULL;

	// Return a pointer to our stream Vtbl.
	*ppstream = (PAVISTREAM) &(p->AVIStream);
	
	return ResultFromScode(AVIERR_OK);
}

//
// The WriteData Method of the File interface
//
STDMETHODIMP WaveFileWriteData(
	PAVIFILE pf,
	DWORD ckid,
	LPVOID lpData,
	LONG cbData)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	// Write the data in the Wave File.
	return ResultFromScode(WriteExtra(&p->extra, ckid, lpData, cbData));
}

//
// The ReadData Method of the File interface
//
STDMETHODIMP WaveFileReadData(
	PAVIFILE pf,
	DWORD ckid,
	LPVOID lpData,
	LONG FAR *lpcbData)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	// Read the data from the file
	return ResultFromScode(ReadExtra(&p->extra, ckid, lpData, lpcbData));
}

//
// The EndRecord Method of the File interface.. this doesn't need to do
// anything.. (no concept of interleaving or packaging streams)
//
STDMETHODIMP WaveFileEndRecord(
	PAVIFILE pf)
{
	return ResultFromScode(AVIERR_OK);
}


//
// The Info Method of the File interface
//
STDMETHODIMP WaveFileInfo(
	PAVIFILE pf,
	AVIFILEINFOW FAR * pfi,
	LONG lSize)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_FILE(pf);

	// Return an AVIFILEHEADER.
	hmemcpy(pfi, &p->avihdr, min(lSize, sizeof(p->avihdr)));
	return 0;
}



//
// The Create Method of the Stream interface. We can't create streams that
// aren't attached to the file.
//
STDMETHODIMP WaveStreamCreate(
	PAVISTREAM	ps,
	LONG lParam1,
	LONG lParam2)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}


//
// The FindSample Method of the Stream interface
//
STDMETHODIMP_(LONG) WaveStreamFindSample(
	PAVISTREAM	ps,
	LONG lPos, LONG lFlags)
{
	if (lFlags & FIND_FORMAT) {
		if ((lFlags & FIND_NEXT) && lPos > 0)
			return -1;
		else
			return 0;
	}

	return lPos;
}


//
// The ReadFormat Method of the Stream interface
//
STDMETHODIMP WaveStreamReadFormat(
	PAVISTREAM	ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG FAR *lpcbFormat)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_STREAM(ps);

	// No buffer to fill in, this means return the size needed.
	if (lpFormat == NULL || *lpcbFormat == 0) {
		*lpcbFormat = p->cbFormat;
		return 0;
	}

	// Give them the WAVE format.
	hmemcpy(lpFormat, p->lpFormat, min(*lpcbFormat, p->cbFormat));

	// Our buffer is too small
	if (*lpcbFormat < p->cbFormat)
		return ResultFromScode(AVIERR_BUFFERTOOSMALL);

	*lpcbFormat = p->cbFormat;

	return 0;
}

//
// The Info Method of the Stream interface
//
STDMETHODIMP WaveStreamInfo(
	PAVISTREAM	ps,
	AVISTREAMINFOW FAR * psi,
	LONG lSize)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_STREAM(ps);

	// give them an AVISTREAMINFO
	hmemcpy(psi, &p->avistream, min(lSize, sizeof(p->avistream)));
	return 0;
}


STDMETHODIMP WaveStreamSetInfo(PAVISTREAM ps, AVISTREAMINFOW FAR * psi, LONG lSize)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*
		invalid lPos return error

		if lPos + lSamples is invalid trim lSamples to fit.

		lpBuffer == NULL

			cbBuffer == 0 && lSamples > 0
				return size of lSamples sample.
			else
				return the exactly the number of bytes and sample
				you would have read if lpBuffer was not zero.

			NOTE return means fill in *plBytes and *plSamples.

		lpBuffer != NULL

			lSamples == -1      read convenient amount (just fill buffer)
			lSamples == 0       fill buffer with as many samples that will fit.
			lSamples >  0       read lSamples (or as much will fit in cbBuffer)

			fill in *plBytes   with bytes actualy read
			fill in *plSamples with samples actualy read

*/

//
// The Read Method for the Stream Interface - Read some wave data
STDMETHODIMP WaveStreamRead(
	PAVISTREAM	ps,
	LONG		lStart,
	LONG		lSamples,
	LPVOID		lpBuffer,
	LONG		cbBuffer,
	LONG FAR *	plBytes,
	LONG FAR *	plSamples)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_STREAM(ps);

	LONG	lSampleSize;
	LONG	lSeek;
	LONG	lRead;

	// Invalid position
	if (lStart < 0 || lStart > (LONG) p->avistream.dwLength) {
ack:
		if (plBytes)
			*plBytes = 0;
		if (plSamples)
			*plSamples = 0;
		return 0;
	}
	
	// Can't read quite this much data
	if (lSamples + lStart > (LONG) p->avistream.dwLength)
		lSamples = p->avistream.dwLength - lStart;
	
	lSampleSize = p->avistream.dwSampleSize;

	// We have fixed-length samples

	if (lpBuffer == NULL) {
		if (cbBuffer > 0 && lSamples > 0)
			// Trim how many samples we'd really be able to read
			lSamples = min(lSamples, cbBuffer / lSampleSize);
		else if (lSamples <= 0)
	    		// Use as many as will fit
			lSamples = cbBuffer / lSampleSize;
	} else {
		if (lSamples > 0)
			// Trim how many samples we'd really be able to read
			lSamples = min(lSamples, cbBuffer / lSampleSize);
		else
			// Use as many as will fit
			lSamples = cbBuffer / lSampleSize;
	}

	//
	// a NULL buffer means return the size buffer needed to read
	// the given sample.
	//
	if (lpBuffer == NULL || cbBuffer == 0) {
		if (plBytes)
			*plBytes = lSamples * lSampleSize;;
		if (plSamples)
			*plSamples = lSamples;
		return 0;
	}

	// Buffer too small!
	if (cbBuffer < lSampleSize)
		goto ack;

	// Seek and read

	cbBuffer = lSamples * lSampleSize;

	lSeek = p->ckData.dwDataOffset + lSampleSize * lStart;
	lRead = lSamples * lSampleSize;
	
	if (mmioSeek(p->hmmio, lSeek, SEEK_SET) != lSeek)
		goto ack;

	if (mmioRead(p->hmmio, (HPSTR) lpBuffer, lRead) != lRead)
		goto ack;
	
	//
	// success return number of bytes and number of samples read
	//
	if (plBytes)
		*plBytes = lRead;

	if (plSamples)
		*plSamples = lSamples;

	return ResultFromScode(AVIERR_OK);
}


//
// The SetFormat Method of the Stream interface	- called on an empty WAVE file
// before writing data to it.
//
STDMETHODIMP WaveStreamSetFormat(
	PAVISTREAM ps,
	LONG lPos,
	LPVOID lpFormat,
	LONG cbFormat)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_STREAM(ps);

	// We can only do this to an empty wave file
	if (p->lpFormat) {
		if (cbFormat != p->cbFormat ||
			_fmemcmp(lpFormat, p->lpFormat, (int) cbFormat))
			return ResultFromScode(AVIERR_UNSUPPORTED);
	
		return NOERROR;
	}
	
	// Go ahead and set the format!

	p->cbFormat = cbFormat;
	p->lpFormat = (LPWAVEFORMAT) GlobalAllocPtr(GMEM_MOVEABLE, cbFormat);

	if (p->lpFormat == NULL)
		return ResultFromScode(AVIERR_MEMORY);

	hmemcpy(p->lpFormat, lpFormat, cbFormat);

	p->ckData.dwDataOffset = cbFormat + 7 * sizeof(DWORD);
	p->ckData.cksize = 0;
	p->avistream.dwScale = p->lpFormat->nBlockAlign;
	p->avistream.dwRate = p->lpFormat->nAvgBytesPerSec;
	p->avistream.dwLength = 0;
	p->avistream.dwSampleSize = p->lpFormat->nBlockAlign;

#ifndef FPSHACK
	p->avihdr.dwScale = 1;
	p->avihdr.dwRate = p->lpFormat->nSamplesPerSec;
#endif
	return ResultFromScode(AVIERR_OK);
}

//
// The Write Method of the Stream interface - write some wave data
//
STDMETHODIMP WaveStreamWrite(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples,
	LPVOID lpData,
	LONG cbData,
	DWORD dwFlags,
	LONG FAR *plSampWritten,
	LONG FAR *plBytesWritten)
{
	// Get a pointer to our structure
	LPWAVESTUFF p = WAVESTUFF_FROM_STREAM(ps);

	if ((p->mode & (OF_WRITE | OF_READWRITE)) == 0)
		return ResultFromScode(AVIERR_READONLY);

	// < 0 means "at end"
	if (lStart < 0)
		// !!!
		lStart = p->avistream.dwStart + p->avistream.dwLength;

	p->fDirty = TRUE;

	mmioSeek(p->hmmio,
		p->ckData.dwDataOffset +
		lStart * p->avistream.dwSampleSize,
		SEEK_SET);

	if (mmioWrite(p->hmmio, (HPSTR) lpData, cbData) != cbData)
		return ResultFromScode(AVIERR_FILEWRITE);

	p->avistream.dwLength = max((LONG) p->avistream.dwLength,
					lStart + lSamples);

	p->ckData.cksize = max(p->ckData.cksize,
				lStart * p->avistream.dwSampleSize + cbData);

#ifdef FPSHACK
	p->avihdr.dwLength = MulDiv32(p->avistream.dwLength * FPSHACK,
				p->avistream.dwScale,
				p->avistream.dwRate);
#else
	p->avihdr.dwLength = MulDiv32(p->ckData.cksize,
				p->lpFormat->nSamplesPerSec,
				p->lpFormat->nAvgBytesPerSec);
#endif

	
	if (plSampWritten)
		*plSampWritten = lSamples;

	if (plBytesWritten)
		*plBytesWritten = cbData;
	
	return ResultFromScode(AVIERR_OK);
}

//
// The Delete Method of the Stream interface - we don't cut from wave files
//
STDMETHODIMP WaveStreamDelete(
	PAVISTREAM ps,
	LONG lStart,
	LONG lSamples)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}


//
// We also don't support ReadData and WriteData for the Stream Interface
//

STDMETHODIMP WaveStreamReadData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG FAR *lpcb)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP WaveStreamWriteData(
	PAVISTREAM ps,
	DWORD fcc,
	LPVOID lp,
	LONG cb)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}


STDMETHODIMP WaveFileReserved(
	PAVIFILE pf)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP WaveStreamReserved(
	PAVISTREAM ps)
{
	return ResultFromScode(AVIERR_UNSUPPORTED);
}

/*      -       -       -       -       -       -       -       -       */

// *** IPersist methods ***
STDMETHODIMP WavePersistGetClassID (LPPERSISTFILE ppf, LPCLSID lpClassID)
{
    // Get a pointer to our structure
    LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);

    hmemcpy(lpClassID, &CLSID_AVIWaveFileReader, sizeof(CLSID));
    return NOERROR;
}

// *** IPersistFile methods ***
STDMETHODIMP WavePersistIsDirty (LPPERSISTFILE ppf)
{
    // Get a pointer to our structure
    LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);

    return pfile->fDirty ? NOERROR : ResultFromScode(S_FALSE);
}

STDMETHODIMP WavePersistLoad (LPPERSISTFILE ppf,
			      LPCOLESTR lpszFileName, DWORD grfMode)
{
    // Get a pointer to our structure
    LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);


#if !defined UNICODE
    char    achTemp[256];

    // Internally, we're using ANSI, but this interface is defined
    // to always accept UNICODE under WIN32, so we have to convert.
    WideCharToMultiByte(CP_ACP, 0, lpszFileName, -1,
			achTemp, sizeof(achTemp), NULL, NULL);
#else
    #define achTemp	lpszFileName
#endif

    return WaveFileOpen((PAVIFILE) &pfile->AVIFile, achTemp, (UINT) grfMode);
}

STDMETHODIMP WavePersistSave (LPPERSISTFILE ppf,
			      LPCOLESTR lpszFileName, BOOL fRemember)
{
	// Get a pointer to our structure
	LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);


    return ResultFromScode(E_FAIL);
}

STDMETHODIMP WavePersistSaveCompleted (LPPERSISTFILE ppf,
				       LPCOLESTR lpszFileName)
{
	// Get a pointer to our structure
	LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);


    return NOERROR;
}

STDMETHODIMP WavePersistGetCurFile (LPPERSISTFILE ppf,
				    LPOLESTR FAR * lplpszFileName)
{
    // Get a pointer to our structure
    LPWAVESTUFF pfile = WAVESTUFF_FROM_PERSIST(ppf);

    return ResultFromScode(E_FAIL);
}
