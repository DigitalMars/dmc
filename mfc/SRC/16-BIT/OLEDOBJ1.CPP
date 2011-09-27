// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"

#ifdef AFX_OLE3_SEG
#pragma code_seg(AFX_OLE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleDataObject constructors

COleDataObject::COleDataObject()
{
	m_lpEnumerator = NULL;
	m_lpDataObject = NULL;
	m_bAutoRelease = TRUE;
}

void COleDataObject::Attach(LPDATAOBJECT lpDataObject, BOOL bAutoRelease)
{
	ASSERT(lpDataObject != NULL);

	Release();  // detach previous
	m_lpDataObject = lpDataObject;
	m_bAutoRelease = bAutoRelease;
}

void COleDataObject::Release()
{
	_AfxRelease((LPUNKNOWN*)&m_lpEnumerator);

	if (m_lpDataObject != NULL)
	{
		if (m_bAutoRelease)
			m_lpDataObject->Release();
		m_lpDataObject = NULL;
	}
}

LPDATAOBJECT COleDataObject::Detach()
{
	LPDATAOBJECT lpDataObject = m_lpDataObject;
	m_lpDataObject = NULL;  // detach without Release
	return lpDataObject;
}

/////////////////////////////////////////////////////////////////////////////
// COleDataObject attributes

void COleDataObject::BeginEnumFormats()
{
	ASSERT(m_lpDataObject != NULL);

	// release old enumerator
	_AfxRelease((LPUNKNOWN*)&m_lpEnumerator);

	// get the new enumerator
	HRESULT hr = m_lpDataObject->EnumFormatEtc(DATADIR_GET, &m_lpEnumerator);
	ASSERT(hr != NOERROR || m_lpEnumerator != NULL);
}

BOOL COleDataObject::GetNextFormat(LPFORMATETC lpFormatEtc)
{
	ASSERT(m_lpDataObject != NULL);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// return false if enumerator is already NULL
	if (m_lpEnumerator == NULL)
		return FALSE;

	// attempt to retrieve the next format with the enumerator
	HRESULT hr = m_lpEnumerator->Next(1, lpFormatEtc, NULL);

	// if enumerator fails, stop the enumeration
	if (hr != NOERROR)
	{
		_AfxRelease((LPUNKNOWN*)&m_lpEnumerator);
		return FALSE;   // enumeration has ended
	}
	// otherwise, continue
	return TRUE;
}

CFile* COleDataObject::GetFileData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT(m_lpDataObject != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to get the data
	STGMEDIUM stgMedium;
	HRESULT hr = m_lpDataObject->GetData(lpFormatEtc, &stgMedium);
	if (FAILED(hr))
		return FALSE;

	// STGMEDIUMs with pUnkForRelease need to be copied first
	if (stgMedium.pUnkForRelease != NULL)
	{
		STGMEDIUM stgMediumDest;
		stgMediumDest.tymed = TYMED_NULL;
		stgMediumDest.pUnkForRelease = NULL;
		if (!_AfxCopyStgMedium(lpFormatEtc->cfFormat, &stgMediumDest, &stgMedium))
		{
			::ReleaseStgMedium(&stgMedium);
			return FALSE;
		}
		// release original and replace with new
		::ReleaseStgMedium(&stgMedium);
		stgMedium = stgMediumDest;
	}

	// convert it to a file, depending on data
	CString strFileName;
	CFile* pFile = NULL;
	TRY
	{
		switch (stgMedium.tymed)
		{
		case TYMED_FILE:
			strFileName = stgMedium.lpszFileName;
			pFile = new CFile;
			if (!pFile->Open(strFileName,
				CFile::modeReadWrite|CFile::shareExclusive))
			{
				delete pFile;
				pFile = NULL;
				break;
			}
			// caller is responsible for deleting the actual file,
			//  but we free the file name.
			_AfxFreeTaskMem(stgMedium.lpszFileName);
			break;

		case TYMED_MFPICT:
		case TYMED_HGLOBAL:
			pFile = new CSharedFile;
			((CSharedFile*)pFile)->SetHandle(stgMedium.hGlobal);
			break;

		case TYMED_ISTREAM:
			pFile = new COleStreamFile(stgMedium.pstm);
			break;

		default:
			// type not supported, so return error
			::ReleaseStgMedium(&stgMedium);
			break;
		}
	}
	CATCH_ALL(e)
	{
		delete pFile;
		pFile = NULL;
	}
	END_CATCH_ALL

	// store newly created CFile* and return
	return pFile;
}

HGLOBAL COleDataObject::GetGlobalData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT(m_lpDataObject != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	BOOL bFillFormatEtc = (lpFormatEtc == NULL);
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
	if (bFillFormatEtc)
		lpFormatEtc->tymed = TYMED_HGLOBAL;
	ASSERT((lpFormatEtc->tymed & (TYMED_HGLOBAL|TYMED_MFPICT)) != 0);

	// attempt to get the data
	STGMEDIUM stgMedium;
	HRESULT hr = m_lpDataObject->GetData(lpFormatEtc, &stgMedium);
	if (FAILED(hr))
		return FALSE;

	// handle just hGlobal types
	switch (stgMedium.tymed)
	{
	case TYMED_MFPICT:
	case TYMED_HGLOBAL:
		if (stgMedium.pUnkForRelease != NULL)
		{
			STGMEDIUM stgMediumDest;
			stgMediumDest.tymed = TYMED_NULL;
			stgMediumDest.pUnkForRelease = NULL;
			if (!_AfxCopyStgMedium(lpFormatEtc->cfFormat, &stgMediumDest, &stgMedium))
			{
				::ReleaseStgMedium(&stgMedium);
				return NULL;
			}
			::ReleaseStgMedium(&stgMedium);
			return stgMediumDest.hGlobal;
		}
		return stgMedium.hGlobal;

	// else -- falls through to error condition...
	}

	::ReleaseStgMedium(&stgMedium);
	return NULL;
}

BOOL COleDataObject::GetData(CLIPFORMAT cfFormat, LPSTGMEDIUM lpStgMedium,
	LPFORMATETC lpFormatEtc)
{
	ASSERT(m_lpDataObject != NULL);
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to get the data
	HRESULT hr = m_lpDataObject->GetData(lpFormatEtc, lpStgMedium);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

BOOL COleDataObject::IsDataAvailable(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT(m_lpDataObject != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to get the data
	return m_lpDataObject->QueryGetData(lpFormatEtc) == NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// clipboard API wrappers

BOOL COleDataObject::AttachClipboard()
{
	ASSERT(AfxIsValidAddress(this, sizeof(COleDataObject)));
	ASSERT(m_lpDataObject == NULL); // need to call release?

	// get clipboard using OLE API
	LPDATAOBJECT lpDataObject;
	HRESULT hr = ::OleGetClipboard(&lpDataObject);
	if (hr != NOERROR)
		return FALSE;

	// attach COleDataObject wrapper to IDataObject from clipboard
	Attach(lpDataObject, TRUE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE client APIs
static char BASED_CODE _szAfxOleInl[] = "afxole.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxOleInl
#define _AFXOLEDOBJ_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
