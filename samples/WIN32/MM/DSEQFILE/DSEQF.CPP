/****************************************************************************
 *
 *  DSEQF.CPP
 *
 *  routines for reading DIB sequences
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
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <vfw.h>

#include "handler.h"
#include "handler.rc"


static DWORD NEAR PASCAL dseqParseFileName(
        LPSTR lpszFileName,
        LPSTR lpszTemplate,
        DWORD FAR * lpdwMaxValue);

#ifdef DEBUG
        static void CDECL dprintf(LPSTR, ...);
        #define DPF dprintf
#else
        #define DPF ; / ## /
#endif

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*      -       -       -       -       -       -       -       -       */

UINT    uUseCount;
BOOL    fLocked;

/*      -       -       -       -       -       -       -       -       */

//
// External function called by the Class Factory to create an instance of
// the DIB sequence reader/writer
//
HRESULT CAVIFile::Create(
IUnknown FAR*   pUnknownOuter,
const IID FAR&  riid,
void FAR* FAR*  ppv)
{
        IUnknown FAR*   pUnknown;
        CAVIFile FAR*   pAVIFile;
        HRESULT hresult;

        pAVIFile = new FAR CAVIFile(pUnknownOuter, &pUnknown);
        if (!pAVIFile)
                return ResultFromScode(E_OUTOFMEMORY);
        hresult = pUnknown->QueryInterface(riid, ppv);
        if (FAILED(GetScode(hresult)))
                delete pAVIFile;
        return hresult;
}

/*      -       -       -       -       -       -       -       -       */

//
// Random C++ stuff: constructors & such...
//
CAVIFile::CAVIFile(
IUnknown FAR*   pUnknownOuter,
IUnknown FAR* FAR*      ppUnknown) :
        m_Unknown(this),
        m_AVIFile(this),
        m_Persist(this),
#ifdef CUSTOMMARSHAL
        m_Marshal(this),
#endif
        m_AVIStream(this)
{
        if (pUnknownOuter)
                m_pUnknownOuter = pUnknownOuter;
        else
                m_pUnknownOuter = &m_Unknown;
        *ppUnknown = &m_Unknown;
}

/*      -       -       -       -       -       -       -       -       */

CAVIFile::CUnknownImpl::CUnknownImpl(
CAVIFile FAR*   pAVIFile)
{
        m_pAVIFile = pAVIFile;
        m_refs = 0;
}

/*      -       -       -       -       -       -       -       -       */

//
// This QueryInterface function allows a caller to move between the various
// interfaces the object presents
//
STDMETHODIMP CAVIFile::CUnknownImpl::QueryInterface(
const IID FAR&  iid,
void FAR* FAR*  ppv)
{
        if (iid == IID_IUnknown)
                *ppv = &m_pAVIFile->m_Unknown;
        else if (iid == IID_IAVIFile)
                *ppv = &m_pAVIFile->m_AVIFile;
        else if (iid == IID_IAVIStream)
                *ppv = &m_pAVIFile->m_AVIStream;
        else if (iid == IID_IPersistFile)
                *ppv = &m_pAVIFile->m_Persist;
#ifdef CUSTOMMARSHAL
        else if (iid == IID_IMarshal)
                *ppv = &m_pAVIFile->m_Marshal;
#endif
        else
                return ResultFromScode(E_NOINTERFACE);
        AddRef();
        return NULL;
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CUnknownImpl::AddRef()
{
        uUseCount++;
        return ++m_refs;
}

/*      -       -       -       -       -       -       -       -       */

//
// All calls to AddRef, Release, QueryInterface for the file or stream
// functions are redirected to the Unknown implementation...
//
CAVIFile::CAVIFileImpl::CAVIFileImpl(
CAVIFile FAR*   pAVIFile)
{
        m_pAVIFile = pAVIFile;
}

/*      -       -       -       -       -       -       -       -       */

CAVIFile::CAVIFileImpl::~CAVIFileImpl()
{
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP CAVIFile::CAVIFileImpl::QueryInterface(
const IID FAR&  iid,
void FAR* FAR*  ppv)
{
        return m_pAVIFile->m_pUnknownOuter->QueryInterface(iid, ppv);
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CAVIFileImpl::AddRef()
{
        return m_pAVIFile->m_pUnknownOuter->AddRef();
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CAVIFileImpl::Release()
{
        return m_pAVIFile->m_pUnknownOuter->Release();
}

/*      -       -       -       -       -       -       -       -       */

CAVIFile::CAVIStreamImpl::CAVIStreamImpl(
CAVIFile FAR*   pAVIFile)
{
        m_pAVIFile = pAVIFile;
}

/*      -       -       -       -       -       -       -       -       */

CAVIFile::CAVIStreamImpl::~CAVIStreamImpl()
{
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP CAVIFile::CAVIStreamImpl::QueryInterface(
const IID FAR&  iid,
void FAR* FAR*  ppv)
{
        return m_pAVIFile->m_pUnknownOuter->QueryInterface(iid, ppv);
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CAVIStreamImpl::AddRef()
{
        return m_pAVIFile->m_pUnknownOuter->AddRef();
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CAVIStreamImpl::Release()
{
        return m_pAVIFile->m_pUnknownOuter->Release();
}


// --- IPersistFile implementation --------------------------------------

CAVIFile::CPersistFileImpl::CPersistFileImpl(CAVIFile FAR* pAVIFile)
{
    m_pAVIFile = pAVIFile;
}

STDMETHODIMP
CAVIFile::CPersistFileImpl::QueryInterface(REFIID riid, LPVOID FAR* ppv)
{
        return m_pAVIFile->m_pUnknownOuter->QueryInterface(riid, ppv);
}

STDMETHODIMP_(ULONG)
CAVIFile::CPersistFileImpl::AddRef()
{
        return m_pAVIFile->m_pUnknownOuter->AddRef();
}

STDMETHODIMP_(ULONG)
CAVIFile::CPersistFileImpl::Release()
{
        return m_pAVIFile->m_pUnknownOuter->Release();
}

// *** IPersist methods ***
STDMETHODIMP
CAVIFile::CPersistFileImpl::GetClassID (LPCLSID lpClassID)
{
        *lpClassID = CLSID_DIBSEQFileReader;

        return NOERROR;
}

// *** IPersistFile methods ***
STDMETHODIMP
CAVIFile::CPersistFileImpl::IsDirty ()
{
    if (m_pAVIFile->fDirty) {
        return NOERROR;
    } else {
        return ResultFromScode(S_FALSE);
    }
}

/*      -       -       -       -       -       -       -       -       */

//
// This function takes the name of the first file in a DIB sequence, and
// returns a printf() specifier which can be used to create the names in
// the sequence, along with minimum and maximum values that can be used.
//
//
// Examples:
//  lpszFileName = "FOO0047.DIB"
//       -> lpszTemplate = "FOO%04d.DIB", dwMaxValue = 9999, return = 47
//
//  lpszFileName = "TEST01.DIB"
//       -> lpszTemplate = "TEST%01d.DIB", dwMaxValue = 9, return = 1
//
//  lpszFileName = "TEST1.DIB"
//       -> lpszTemplate = "TEST%d.DIB", dwMaxValue = 9999, return = 1
//
//  lpszFileName = "SINGLE.DIB"
//       -> lpszTemplate = "SINGLE.DIB", dwMaxValue = 0, return = 0
//
static DWORD NEAR PASCAL dseqParseFileName(
LPSTR lpszFileName,
LPSTR lpszTemplate,
DWORD FAR * lpdwMaxValue)
{
        char    achTemp[_MAX_PATH];
        DWORD   dwFirst;
        WORD    wFieldWidth;
        DWORD   dwMult;
        BOOL    fLeadingZero = FALSE;

        LPSTR   lp;
        LPSTR   lp2;
        LPSTR   lpExt;

        /* Find end of string */
        lp2 = lpszFileName;
        lp = achTemp;
        while (*lp2)
        *lp++ = *lp2++;

        *lp = '\0';

        /* Make lp2 point at last character of base filename (w/o extension) */
        /* Make lpExt point at the extension (without the dot) */
        for (lp2 = lp; *lp2 != '.'; lp2--)
        {
                lpExt = lp2;
                if ((lp2 == achTemp) || (*lp2 == '\\')
                        || (*lp2 == ':') || (*lp2 == '!'))
                {
                        /* There is no extension */
                        lp2 = lp;
                        lpExt = lp;
                        break;
                }
        }

        lp2--;

        // Count the number of numeric characters here....
        dwFirst = 0;
        wFieldWidth = 0;
        dwMult = 1;
        while (lp2 >= achTemp && (*lp2 >= '0') && (*lp2 <= '9')) {
        fLeadingZero = (*lp2 == '0');
        dwFirst += dwMult * (*(lp2--) - '0');
        dwMult *= 10;
        wFieldWidth++;
        }

        *lpdwMaxValue = dwMult - 1;

        lp2++;
        *lp2 = '\0';

        // Make the format specifier....
        if (wFieldWidth) {
        if (fLeadingZero) {
                wsprintf((LPSTR) lpszTemplate,"%s%%0%ulu.%s",
                        (LPSTR) achTemp, wFieldWidth,(LPSTR) lpExt);
        } else {
                wsprintf((LPSTR) lpszTemplate,"%s%%lu.%s",
                        (LPSTR) achTemp, (LPSTR) lpExt);
                *lpdwMaxValue = 999999L;
                // !!! This should really be based on the number of
                // characters left after the base name....
        }
        } else
        wsprintf((LPSTR) lpszTemplate,"%s.%s",
                        (LPSTR) achTemp, (LPSTR) lpExt);
        DPF("First = %lu, Width = %u, Template = '%s'\n",dwFirst, wFieldWidth, lpszTemplate);

        return dwFirst;
}

/*      -       -       -       -       -       -       -       -       */

#define SLASH(c) ((c) == '/' || (c) == '\\')

/*--------------------------------------------------------------+
| FileName  - return a pointer to the filename part of szPath   |
|             with no preceding path.                           |
+--------------------------------------------------------------*/
LPSTR FAR FileName(
LPCSTR lszPath)
{
        LPCSTR  lszCur;

        for (lszCur = lszPath + lstrlen(lszPath); lszCur > lszPath && !SLASH(*lszCur) && *lszCur != ':';)
                lszCur = AnsiPrev(lszPath, lszCur);
        if (lszCur == lszPath)
                return (LPSTR)lszCur;
        else
                return (LPSTR)(lszCur + 1);
}

/*      -       -       -       -       -       -       -       -       */

//
// "Open" a DIB sequence, by parsing the filename and counting the number
// of frames actually present....
//
STDMETHODIMP
CAVIFile::CPersistFileImpl::Load (LPCOLESTR szFile, DWORD mode)
{
        CAVIFile FAR    *p = m_pAVIFile;
        UINT            ui;
        char            ach[80];
        char            szFileA[MAX_PATH];

        p->mode = mode;

        //
        // Parse the filename
        //
        wsprintf(szFileA, "%ls", szFile);
        p->dwFirstFrame = dseqParseFileName(szFileA,
                                p->achFilenameTemplate,
                                &p->dwMaxValue);

        //
        // Initialize the variables that keep track of what frame is cached
        //
        p->lCurFrame = -1;
        p->lpFrame = NULL;
        p->cbFrame = 0;
        p->cbFrameBuffer = 0;
        p->lpFormat = NULL;
        p->cbFormat = 0;
        p->cbFormatBuffer = 0;

        //
        // Build a stream header....
        //
        p->sinfo.fccType = streamtypeVIDEO;
        p->sinfo.fccHandler = 0;
        p->sinfo.dwFlags = 0;
        p->sinfo.wPriority = 0;
        p->sinfo.wLanguage = 0;
        p->sinfo.dwInitialFrames = 0;
        p->sinfo.dwScale = 1;
        p->sinfo.dwRate = 15;
        p->sinfo.dwStart = 0;
        p->sinfo.dwLength = 0;
        p->sinfo.dwSuggestedBufferSize = 0;
        p->sinfo.dwSampleSize = 0;

        LoadString(ghModule, IDS_STREAMNAME, ach, sizeof(ach));
        {
            char TempFileName[80];
            char TempName[80];
            wsprintf(TempFileName, "%ls", szFile);
            wsprintf(TempName, ach, FileName(TempFileName));
            wsprintfW(p->sinfo.szName, L"%hs", TempName);
        }

        //
        // ... and a file header.
        //
        _fmemset(&p->finfo, 0, sizeof(p->finfo));
        p->finfo.dwRate = 15;
        p->finfo.dwScale = 1;
        p->finfo.dwStreams = 1;
        p->finfo.dwWidth = 0;
        p->finfo.dwHeight = 0;
        LoadString(ghModule, IDS_FILETYPE,
                p->finfo.szFileType,
                sizeof(p->finfo.szFileType));

        p->finfo.dwCaps = AVIFILECAPS_CANREAD |
                                AVIFILECAPS_CANWRITE |
                                AVIFILECAPS_ALLKEYFRAMES;

        if (mode & OF_CREATE) {
        //
        // They're creating a "new" sequence
        //
        p->fStreamPresent = FALSE;
        } else {
        char            ach[_MAX_PATH];
        OFSTRUCT        of;
        DWORD           dwFrame;
        HRESULT         hr;

        //
        // They're opening an existing sequence, so we have to actually
        // count how many files are present
        //
        p->fStreamPresent = TRUE;

        ui = SetErrorMode(SEM_NOOPENFILEERRORBOX);

        for (dwFrame = 0; TRUE; dwFrame++) {
                if (dwFrame > p->dwMaxValue)
                break;

                wsprintf(ach,p->achFilenameTemplate, dwFrame + p->dwFirstFrame);

                // DPF("DIBSEQ: Checking frame %lu from '%s'\n",dwFrame,(LPSTR) ach);

/****************************************************************************/
/* DOS share has a bug.  If the file we're testing for existence is open    */
/* already by someone else, we have to give it the same flag for SHARE as   */
/* the other person is using.  So we have to try both on and off.  Only one */
/* of these will return TRUE but if one of them does, the file exists.  Also*/
/* we have to turn off the system model error box for share violations.     */
/****************************************************************************/

                if (OpenFile((LPSTR)ach, &of, OF_EXIST) == HFILE_ERROR &&
                        OpenFile((LPSTR)ach, &of, OF_EXIST | OF_SHARE_DENY_NONE) ==
                        HFILE_ERROR)
                break;
        }

        SetErrorMode(ui);

        if (dwFrame == 0)
                goto error;

        //
        // Fix up the length in the header structures
        //
        p->sinfo.dwLength = dwFrame;
        p->finfo.dwLength = dwFrame;

        //
        // Load the first frame, so we'll be ready...
        //
        hr = p->LoadFrame(0);

        if (FAILED(GetScode(hr)))
                return hr;

        p->finfo.dwSuggestedBufferSize = p->cbFrame;
        p->sinfo.dwSuggestedBufferSize = p->cbFrame;

        p->finfo.dwWidth = ((LPBITMAPINFOHEADER) p->lpFormat)->biWidth;
        p->finfo.dwHeight = ((LPBITMAPINFOHEADER) p->lpFormat)->biHeight;

        p->sinfo.dwFlags = AVISTREAMINFO_FORMATCHANGES;

        SetRect(&p->sinfo.rcFrame,
                0, 0, (int) p->finfo.dwWidth, (int) p->finfo.dwHeight);
        }

        //
        // all done return success.
        //
        return ResultFromScode(0); // success

error:
        return ResultFromScode(AVIERR_FILEREAD);


}


STDMETHODIMP
CAVIFile::CPersistFileImpl::Save (LPCOLESTR lpszFileName, BOOL fRemember)
{
        return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP
CAVIFile::CPersistFileImpl::SaveCompleted (LPCOLESTR lpszFileName)
{
        return NOERROR;
}

STDMETHODIMP
CAVIFile::CPersistFileImpl::GetCurFile (LPOLESTR FAR * lplpszFileName)
{
        return ResultFromScode(AVIERR_UNSUPPORTED);
}



// -------------------- IAVIFile Implementation-----------------------


//
// The GetStream method returns an interface pointer to the video stream,
// assuming one exists.
//
STDMETHODIMP CAVIFile::CAVIFileImpl::GetStream(
PAVISTREAM FAR * ppavi,
DWORD fccType,
LONG lParam)
{
        CAVIFile FAR *  p = m_pAVIFile;
        int             iStreamWant;

        iStreamWant = (int)lParam;

        if (!p->fStreamPresent)
        return ResultFromScode(-1);

        // We only support one stream
        if (lParam != 0)
                return ResultFromScode(-1);

        // We only support a video stream
        if (fccType && fccType != streamtypeVIDEO)
        return ResultFromScode(-1);

        //
        // Be sure to keep the reference count up to date...
        //
        AddRef();

        *ppavi = (PAVISTREAM) &(p->m_AVIStream);
        return ResultFromScode(AVIERR_OK);
}


//
// If they opened the file with the OF_CREATE flag, they will use this
// method to create the video stream.
//
STDMETHODIMP CAVIFile::CAVIFileImpl::CreateStream(
PAVISTREAM FAR *ppstream,
AVISTREAMINFOW FAR *psi
)
{
        CAVIFile FAR *  p = m_pAVIFile;

        // If the stream was already there, we fail.
        if (p->fStreamPresent)
        return ResultFromScode(AVIERR_UNSUPPORTED);

        p->sinfo = *psi;
        p->sinfo.dwLength = 0;

        *ppstream = (PAVISTREAM) &(p->m_AVIStream);
        p->fStreamPresent = TRUE;

        // Keep the reference count correct
        AddRef();

        return ResultFromScode(AVIERR_OK);
}


STDMETHODIMP CAVIFile::CAVIFileImpl::WriteData(
DWORD ckid,
LPVOID lpData,
LONG cbData)
{
        CAVIFile FAR *  p = m_pAVIFile;

        return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP CAVIFile::CAVIFileImpl::ReadData(
DWORD ckid,
LPVOID lpData,
LONG FAR *lpcbData)
{
        CAVIFile FAR *  p = m_pAVIFile;

        return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP CAVIFile::CAVIFileImpl::EndRecord(void)
{
        return ResultFromScode(AVIERR_OK);
}


STDMETHODIMP CAVIFile::CAVIFileImpl::Info(AVIFILEINFOW FAR * pfi, LONG lSize)
{
        CAVIFile FAR *  p = m_pAVIFile;

        hmemcpy(pfi, &p->finfo, min(lSize,sizeof(p->finfo)));
        return 0;
}



STDMETHODIMP CAVIFile::CAVIStreamImpl::Create(
LONG lParam1,
LONG lParam2)
{
        return ResultFromScode(AVIERR_UNSUPPORTED);
}


//
// Returns where the last key frame before the given frame is.
//
// For now, we assume each DIB is a key frame.
//
STDMETHODIMP_(LONG) CAVIFile::CAVIStreamImpl::FindSample(
LONG lPos,
LONG lFlags)
{
        CAVIFile FAR * p = m_pAVIFile;

        // some minimal error checking....
        if (lPos < 0 || lPos >= (LONG) p->sinfo.dwLength)
        return -1;

        // !!! Can we really assume every frame is non-empty and a key frame?
        // !!! Who knows where format changes are? Let's assume everywhere!

        return lPos;
}

#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8) /* ULONG aligned ! */
#define DIBWIDTHBYTES(bi)       (int)WIDTHBYTES((int)(bi).biWidth * (int)(bi).biBitCount)
#define BFT_BITMAP              0x4d42   /* 'BM' */

//
// Helper function to load a given frame into our cache.
//
// This is where the actual work is done; all other functions just return
// the current format or frame out of the cache.
//
HRESULT NEAR PASCAL CAVIFile::LoadFrame(
LONG lPos)
{
        char            ach[_MAX_PATH];
        HMMIO           hmmio;
        BITMAPFILEHEADER        bfh;
        BITMAPINFOHEADER        bih;
        SCODE           sc = 0;
        UINT            ui;

        //
        // Check if we've already loaded this frame...
        //
        if (lPos == lCurFrame)
        return 0;

        //
        // Build the filename by printing using our template
        //
        wsprintf(ach, achFilenameTemplate, dwFirstFrame + lPos);

        // No system error box, please.
        ui = SetErrorMode(SEM_NOOPENFILEERRORBOX);

        //
        // Go try to read the frame... Because of SHARE we have to try
        // opening it two different ways.
        //
        hmmio = mmioOpen(ach, NULL, MMIO_READ | OF_SHARE_DENY_WRITE);
        if (!hmmio) {
                hmmio = mmioOpen(ach, NULL, MMIO_READ);
        if (!hmmio)
                return ResultFromScode(AVIERR_FILEOPEN);
        }

        SetErrorMode(ui);

        //
        // Read the BitmapFileHeader...
        //
        if (mmioRead(hmmio, (LPSTR) &bfh, sizeof(bfh)) != sizeof(bfh)) {
        sc = AVIERR_FILEREAD;
        goto error;
        }

        if (bfh.bfType != BFT_BITMAP) {
        sc = AVIERR_BADFORMAT;
        goto error;
        }

        //
        // Read the BitmapInfoHeader...
        //
        if (mmioRead(hmmio, (LPSTR) &bih, sizeof(bih)) != sizeof(bih)) {
        sc = AVIERR_FILEREAD;
        goto error;
        }

        if (bih.biSize < sizeof(bih)) {
        sc = AVIERR_BADFORMAT;
        goto error;
        }

        // Check that the width and height match....
        if ((finfo.dwWidth && finfo.dwWidth != (DWORD) bih.biWidth) ||
        (finfo.dwHeight && finfo.dwHeight != (DWORD) bih.biHeight)) {
        sc = AVIERR_BADFORMAT;
        goto error;
        }

        // Fix up some fields in the header...
        if (bih.biSizeImage == 0) {
        bih.biSizeImage = DIBWIDTHBYTES(bih) * bih.biHeight;
        }

        if (bih.biClrUsed == 0 && bih.biBitCount <= 8 && bih.biCompression <= BI_RLE8)
        bih.biClrUsed = 1 << bih.biBitCount;

        cbFormat = bih.biSize + bih.biClrUsed * sizeof(RGBQUAD);

        // Allocate space for the format
        if (cbFormat > cbFormatBuffer) {
        if (lpFormat) {
                GlobalFreePtr(lpFormat);
                lpFormat = 0;
                cbFormatBuffer = 0;
        }

        lpFormat = GlobalAllocPtr(GMEM_MOVEABLE | GMEM_DDESHARE, cbFormat);
        if (!lpFormat) {
                sc = AVIERR_MEMORY;
                goto error;
        }

        cbFormatBuffer = cbFormat;
        }

        *((LPBITMAPINFOHEADER) lpFormat) = bih;

        // If the format is bigger than a BITMAPINFOHEADER, read the rest....
        if (cbFormat > sizeof(bih)) {
        if (mmioRead(hmmio, (LPSTR) lpFormat + sizeof(bih),
                cbFormat - (LONG)sizeof(bih))
                != cbFormat - (LONG)sizeof(bih))
        {
                sc = AVIERR_FILEREAD;
                goto error;
        }
        }

        //
        // Allocate enough space to read the frame in...
        //
        if (bih.biSizeImage > (DWORD) cbFrameBuffer) {
        if (lpFrame) {
                GlobalFreePtr(lpFrame);
                lpFrame = 0;
                cbFrameBuffer = 0;
        }

        lpFrame = GlobalAllocPtr(GMEM_MOVEABLE | GMEM_DDESHARE, bih.biSizeImage);
        if (!lpFrame) {
                sc = AVIERR_MEMORY;
                goto error;
        }

        cbFrameBuffer = bih.biSizeImage;
        }

        cbFrame = bih.biSizeImage;

        //
        // and actually read the frame....
        //
        if (mmioRead(hmmio, (LPSTR) lpFrame, cbFrame) != cbFrame) {
        sc = AVIERR_FILEREAD;
        goto error;
        }

        lCurFrame = lPos;

error:
        mmioClose(hmmio, 0);

        return ResultFromScode(sc);
}

//
// The ReadFormat method returns the format of the specified frame....
//
STDMETHODIMP CAVIFile::CAVIStreamImpl::ReadFormat(
LONG lPos,
LPVOID lpFormat,
LONG FAR *lpcbFormat)
{
        CAVIFile FAR * p = m_pAVIFile;
        HRESULT hr;

        //
        // Try to get the correct frame
        //
        hr = p->LoadFrame(lPos);

        if (hr != 0)
        return hr;

        // No buffer to fill in, this means return the size needed.
        if (lpFormat == NULL || *lpcbFormat == 0) {
                *lpcbFormat = p->cbFormat;
        return 0;
        }

        //
        // and return as much of the format as will fit.
        //
        hmemcpy(lpFormat, p->lpFormat, min(*lpcbFormat, p->cbFormat));
        *lpcbFormat = p->cbFormat;
        return 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


STDMETHODIMP CAVIFile::CAVIStreamImpl::Info(AVISTREAMINFOW FAR * psi, LONG lSize)
{
        CAVIFile FAR * p = m_pAVIFile;

        hmemcpy(psi,&p->sinfo, min(lSize,sizeof(p->sinfo)));
//      return sizeof(p->sinfo);
        return 0;
}

STDMETHODIMP_(ULONG) CAVIFile::CUnknownImpl::Release()
{
        CAVIFile FAR * p = m_pAVIFile;

        uUseCount--;
        if (!--m_refs) {
        LONG lRet = AVIERR_OK;

        if (p->fDirty) {
        }


        goto success;

        success:
        if (p->lpFormat)
                GlobalFreePtr(p->lpFormat);

        p->lpFormat = NULL;
        p->cbFormat = 0;

        if (p->lpFrame)
                GlobalFreePtr(p->lpFrame);

        p->lpFrame = NULL;
        p->cbFrame = 0;

        delete this;
        return 0;
        }
        return m_refs;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


STDMETHODIMP CAVIFile::CAVIStreamImpl::Read(
LONG            lStart,
LONG            lSamples,
LPVOID          lpBuffer,
LONG            cbBuffer,
LONG FAR *      plBytes,
LONG FAR *      plSamples)
{
        CAVIFile FAR *  p = m_pAVIFile;
        HRESULT     hr;

        if (lStart < 0 || lStart >= (LONG) p->sinfo.dwLength) {
        if (plBytes)
            *plBytes = 0;
        if (plSamples)
            *plSamples = 0;
        return 0;
        }

        // We always read one frame at a time...
        lSamples = 1;

        // Load it into the cache....
        hr = p->LoadFrame(lStart);

        if (hr != 0)
        return hr;

        //
        // a NULL buffer means return the size buffer needed to read
        // the given sample.
        //
        if (lpBuffer == NULL || cbBuffer == 0) {
        if (plBytes)
            *plBytes =  p->cbFrame;
        if (plSamples)
            *plSamples = lSamples;
        return 0;
        }

        //
        // They didn't give us enough space for the frame, so complain
        //
        if (cbBuffer < p->cbFrame) {
        if (plBytes)
            *plBytes = p->cbFrame;

        return ResultFromScode(AVIERR_BUFFERTOOSMALL);
        }

        //
        // Copy the frame into the caller's buffer
        //
        hmemcpy(lpBuffer, p->lpFrame, p->cbFrame);

        //
        // success return number of bytes and number of samples read
        //
        if (plBytes)
                *plBytes = p->cbFrame;

        if (plSamples)
                *plSamples = lSamples;

        return ResultFromScode(AVIERR_OK);
}


STDMETHODIMP CAVIFile::CAVIStreamImpl::SetFormat(
LONG lPos,
LPVOID lpFormat,
LONG cbFormat)
{
        CAVIFile FAR * p = m_pAVIFile;

        // Keep track of the format....
        p->cbFormat = cbFormat;
        p->lpFormat = (LPVOID) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_DDESHARE, cbFormat);

        if (p->lpFormat == NULL)
        return ResultFromScode(AVIERR_MEMORY);

        hmemcpy(p->lpFormat, lpFormat, cbFormat);

        p->finfo.dwWidth = ((LPBITMAPINFOHEADER) p->lpFormat)->biWidth;
        p->finfo.dwHeight = ((LPBITMAPINFOHEADER) p->lpFormat)->biHeight;

        SetRect(&p->sinfo.rcFrame,
            0, 0, (int) p->finfo.dwWidth, (int) p->finfo.dwHeight);

        return 0L;
}

//
// Helper function to save a single frame
//
HRESULT NEAR PASCAL CAVIFile::WriteFrame(
LONG lPos,
LPVOID lp,
LONG cb)
{
        char            ach[_MAX_PATH];
        HMMIO           hmmio;
        BITMAPFILEHEADER        bfh;

        //
        // If they're overwriting the cached frame, invalidate the cache
        //
        if (lPos == lCurFrame)
        lCurFrame = -1;


        //
        // Build the filename to write to
        //
        wsprintf(ach, achFilenameTemplate, dwFirstFrame + lPos);

        // and write it.
        hmmio = mmioOpen(ach, NULL, MMIO_WRITE | MMIO_CREATE | OF_SHARE_EXCLUSIVE);

        if (!hmmio)
        return ResultFromScode(AVIERR_FILEOPEN);

        //
        // Write the BitmapFileHeader
        //
        bfh.bfType = BFT_BITMAP;
        bfh.bfOffBits = sizeof(bfh) + cbFormat;
        bfh.bfSize = bfh.bfOffBits + cb;

        if (mmioWrite(hmmio, (LPSTR) &bfh, sizeof(bfh)) != sizeof(bfh)) {
error:
        mmioClose(hmmio, 0);
        return ResultFromScode(AVIERR_FILEWRITE);
        }

        ((LPBITMAPINFOHEADER) lpFormat)->biSizeImage = cb;

        //
        // Write the DIB format
        //
        if (mmioWrite(hmmio, (LPSTR) lpFormat, cbFormat) != cbFormat)
        goto error;

        //
        // Write the data
        //
        if (mmioWrite(hmmio, (LPSTR) lp, cb) != cb)
        goto error;

        //
        // Flush things so that we can be sure everything is written out
        //
        if (mmioFlush(hmmio, 0) != 0)
        goto error;

        mmioClose(hmmio, 0);

        return 0;
}

STDMETHODIMP CAVIFile::CAVIStreamImpl::Write(
LONG lStart,
LONG lSamples,
LPVOID lpData,
LONG cbData,
DWORD dwFlags,
LONG FAR *plSampWritten,
LONG FAR *plBytesWritten)
{
        CAVIFile FAR *  p = m_pAVIFile;
        HRESULT     hr;

        if ((p->mode & (OF_WRITE | OF_READWRITE)) == 0)
        return ResultFromScode(AVIERR_READONLY);

        if (p->lpFormat == NULL)
        return ResultFromScode(AVIERR_UNSUPPORTED);

        // < 0 means "at end"
        if (lStart < 0)
        lStart = p->sinfo.dwStart + p->sinfo.dwLength;

        if (lStart > (LONG) (p->sinfo.dwStart + p->sinfo.dwLength))
        return ResultFromScode(AVIERR_BADPARAM);

        // !!! Die if we've reached the limit of our numbers....
        if ((DWORD) lStart + p->dwFirstFrame > p->dwMaxValue)
        return ResultFromScode(AVIERR_FILEWRITE);

        if (lSamples != 1)
        return ResultFromScode(AVIERR_BADPARAM);

        // only allow key frames!
        if (!(dwFlags & AVIIF_KEYFRAME)) {
        DPF("Tried to write a non-key frame to a DIB sequence!\n");
        return ResultFromScode(AVIERR_UNSUPPORTED);
        }

        hr = p->WriteFrame(lStart, lpData, cbData);

        if (hr != AVIERR_OK)
        return hr;

        p->fDirty = TRUE;

        p->sinfo.dwLength =
                max((LONG) p->sinfo.dwLength,
                                  lStart + lSamples);

        p->finfo.dwLength = p->sinfo.dwLength;

        p->finfo.dwSuggestedBufferSize =
                        max(p->finfo.dwSuggestedBufferSize, (DWORD) cbData);
        p->sinfo.dwSuggestedBufferSize =
                        p->finfo.dwSuggestedBufferSize;

        if (plSampWritten)
        *plSampWritten = lSamples;

        if (plBytesWritten)
        *plBytesWritten = cbData;

        return ResultFromScode(AVIERR_OK);
}

// these both are for saving. we don't support saving

STDMETHODIMP
CAVIFile::CAVIFileImpl::DeleteStream(DWORD fccType, LONG lParam)
{
    return ResultFromScode(E_FAIL);
}

STDMETHODIMP
CAVIFile::CAVIStreamImpl::SetInfo(
                AVISTREAMINFOW FAR * lpInfo,
                LONG cbInfo
)
{
    return ResultFromScode(E_FAIL);
}


STDMETHODIMP CAVIFile::CAVIStreamImpl::Delete(
LONG lStart,
LONG lSamples)
{
        CAVIFile FAR * p = m_pAVIFile;

        return ResultFromScode(AVIERR_UNSUPPORTED);
}


// Should these just map to Read/WriteData? !!!

STDMETHODIMP CAVIFile::CAVIStreamImpl::ReadData(
DWORD fcc,
LPVOID lp,
LONG FAR *lpcb)
{
        CAVIFile FAR * p = m_pAVIFile;

        return ResultFromScode(AVIERR_UNSUPPORTED);
}

STDMETHODIMP CAVIFile::CAVIStreamImpl::WriteData(
DWORD fcc,
LPVOID lp,
LONG cb)
{
        CAVIFile FAR * p = m_pAVIFile;

        return ResultFromScode(AVIERR_UNSUPPORTED);
}


#ifdef CUSTOMMARSHAL
// The code below supports custom marshalling.

// !!! Need good explanation here!

CAVIFile::CMarshalImpl::CMarshalImpl(
CAVIFile FAR*   pAVIFile)
{
        m_pAVIFile = pAVIFile;
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP CAVIFile::CMarshalImpl::QueryInterface(
const IID FAR&  iid,
void FAR* FAR*  ppv)
{
        return m_pAVIFile->m_pUnknownOuter->QueryInterface(iid, ppv);
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CMarshalImpl::AddRef()
{
        return m_pAVIFile->m_pUnknownOuter->AddRef();
}

/*      -       -       -       -       -       -       -       -       */

STDMETHODIMP_(ULONG) CAVIFile::CMarshalImpl::Release()
{
        return m_pAVIFile->m_pUnknownOuter->Release();
}

// *** IMarshal methods ***
STDMETHODIMP CAVIFile::CMarshalImpl::GetUnmarshalClass (
THIS_ REFIID riid, LPVOID pv,
DWORD dwDestContext, LPVOID pvDestContext,
DWORD mshlflags, LPCLSID pCid)
{
        HRESULT hr = NOERROR;

        DPF("UnMarshalClass called\n");

        if (dwDestContext != MSHCTX_LOCAL) {
        LPMARSHAL       pMarshal;

        DPF("Marshal context is %lu: delegating...\n", dwDestContext);

        hr = CoGetStandardMarshal(riid, NULL, dwDestContext, pvDestContext, mshlflags,
                        &pMarshal);

        if (hr != NOERROR)
                return hr;

        hr = pMarshal->GetUnmarshalClass(riid, pv, dwDestContext, pvDestContext,
                        mshlflags, pCid);

        pMarshal->Release();

        return hr;
        }

        *pCid = CLSID_AVISimpleUnMarshal;

        return hr;
}

STDMETHODIMP CAVIFile::CMarshalImpl::GetMarshalSizeMax (
THIS_ REFIID riid, LPVOID pv,
DWORD dwDestContext, LPVOID pvDestContext,
DWORD mshlflags, LPDWORD pSize)
{
        HRESULT hr = NOERROR;
        IUnknown FAR * pUnk = &m_pAVIFile->m_Unknown;

        if (dwDestContext != MSHCTX_LOCAL) {
        LPMARSHAL       pMarshal;

        hr = CoGetStandardMarshal(riid, NULL, dwDestContext, pvDestContext, mshlflags,
                &pMarshal);

        if (hr != NOERROR)
                return hr;

        hr = pMarshal->GetMarshalSizeMax(riid, pv, dwDestContext, pvDestContext,
                        mshlflags, pSize);

        pMarshal->Release();

        return hr;
        }

        *pSize = sizeof(pUnk);

        return hr;
}

STDMETHODIMP CAVIFile::CMarshalImpl::MarshalInterface (
THIS_ LPSTREAM pStm, REFIID riid,
LPVOID pv, DWORD dwDestContext, LPVOID pvDestContext,
DWORD mshlflags)
{
        HRESULT hr = NOERROR;
        IUnknown FAR * pUnk = &m_pAVIFile->m_Unknown;

        DPF("MarshalInterface called\n");

        if (dwDestContext != MSHCTX_LOCAL) {
        LPMARSHAL pMarshal;

        DPF("Marshal context is %lu: delegating...\n", dwDestContext);

        hr = CoGetStandardMarshal(riid, NULL, dwDestContext, pvDestContext, mshlflags,
                &pMarshal);

        if (hr != NOERROR)
                return hr;

        hr = pMarshal->MarshalInterface(pStm, riid, pv, dwDestContext, pvDestContext,
                        mshlflags);

        pMarshal->Release();

        return hr;
        }


        if ((riid != IID_IAVIStream && riid != IID_IAVIFile && riid != IID_IUnknown))
                return ResultFromScode(E_INVALIDARG);

        if ((hr = pStm->Write(&pUnk, sizeof(pUnk), NULL)) == NOERROR)
        AddRef();

        DPF("Returns %lx\n", (DWORD) (LPVOID) hr);
        return hr;
}

STDMETHODIMP CAVIFile::CMarshalImpl::UnmarshalInterface (
THIS_ LPSTREAM pStm, REFIID riid,
LPVOID FAR* ppv)
{
        HRESULT hr = ResultFromScode(E_FAIL);

        DPF("UnMarshalInterface called!!!\n");
        return hr;
}

STDMETHODIMP CAVIFile::CMarshalImpl::ReleaseMarshalData (
THIS_ LPSTREAM pStm)
{
        HRESULT hr = NOERROR;
        IUnknown FAR * pUnk;

        hr = pStm->Read(&pUnk,sizeof(pUnk),NULL);
        if (hr == NOERROR)
        pUnk->Release();

        return hr;
}

STDMETHODIMP CAVIFile::CMarshalImpl::DisconnectObject (
THIS_ DWORD dwReserved)
{
        HRESULT hr = NOERROR;

        return hr;
}
#endif

/*****************************************************************************
 *
 * dprintf() is called by the DPF macro if DEBUG is defined at compile time.
 *
 * The messages will be send to COM1: like any debug message. To
 * enable debug output, add the following to WIN.INI :
 *
 * [debug]
 * ICSAMPLE=1
 *
 ****************************************************************************/

#ifdef DEBUG

#define MODNAME "DSEQFILE"
static BOOL fDebug = -1;

static void cdecl dprintf(
LPSTR szFormat, ...)
{
        char ach[128];

        va_list va;
        if (fDebug == -1)
                fDebug = GetProfileIntA("Debug",MODNAME, FALSE);

        if (!fDebug)
                return;

        va_start(va, szFormat);
        if (szFormat[0] == '!')
                ach[0]=0, szFormat++;
        else
                lstrcpyA(ach, MODNAME ": ");

        wvsprintfA(ach+lstrlenA(ach),szFormat,(LPSTR)va);
        va_end(va);
//      lstrcatA(ach, "\r\r\n");

        OutputDebugStringA(ach);
}

#endif
