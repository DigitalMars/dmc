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
// CEnumArray (provides OLE enumerator for arbitrary items in an array)

CEnumArray::CEnumArray(size_t nSizeElem, REFIID iidEnum,
	const void FAR* pvEnum, UINT nSize, BOOL bNeedFree)
	: m_iidEnum(iidEnum)
{
	m_nSizeElem = nSizeElem;
	m_pClonedFrom = NULL;

	m_nCurPos = 0;
	m_nSize = nSize;
	m_pvEnum = (char FAR*)pvEnum;
	m_bNeedFree = bNeedFree;

#ifdef _AFXCTL
	m_pModuleState = _afxModuleAddrCurrent;
#endif

	ASSERT_VALID(this);
}

CEnumArray::~CEnumArray()
{
	ASSERT_VALID(this);

	// release the clone pointer (only for clones)
	if (m_pClonedFrom != NULL)
	{
		m_pClonedFrom->InternalRelease();
		ASSERT(!m_bNeedFree);
	}

	// release the pointer (should only happen on non-clones)
	if (m_bNeedFree)
	{
		ASSERT(m_pClonedFrom == NULL);
		delete _AfxGetPtrFromFarPtr(m_pvEnum);
	}
}

BOOL CEnumArray::OnNext(void FAR* pv)
{
	ASSERT_VALID(this);

	if (m_nCurPos >= m_nSize)
		return FALSE;

	_fmemcpy(pv, &m_pvEnum[m_nCurPos*m_nSizeElem], m_nSizeElem);
	++m_nCurPos;
	return TRUE;
}

BOOL CEnumArray::OnSkip()
{
	ASSERT_VALID(this);

	if (m_nCurPos >= m_nSize)
		return FALSE;

	return ++m_nCurPos < m_nSize;
}

void CEnumArray::OnReset()
{
	ASSERT_VALID(this);

	m_nCurPos = 0;
}

CEnumArray* CEnumArray::OnClone()
{
	ASSERT_VALID(this);

	// set up an exact copy of this object
	//  (derivatives may have to replace this code)
	CEnumArray* pClone;
	pClone = new CEnumArray(m_nSizeElem, m_iidEnum, m_pvEnum, m_nSize);
	ASSERT(pClone != NULL);
	ASSERT(!pClone->m_bNeedFree);   // clones should never free themselves
	pClone->m_nCurPos = m_nCurPos;

	// finally, return the clone to OLE
	ASSERT_VALID(pClone);
	return pClone;
}

/////////////////////////////////////////////////////////////////////////////
// special case IUnknown implementation because of m_iidEnum

STDMETHODIMP_(ULONG) CEnumArray::XEnumVOID::AddRef()
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CEnumArray::XEnumVOID::Release()
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CEnumArray::XEnumVOID::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)


	if (iid == pThis->m_iidEnum || iid == IID_IUnknown)
	{
		pThis->InternalAddRef();
		*ppvObj = &pThis->m_xEnumVOID;
		return NOERROR;
	}
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

/////////////////////////////////////////////////////////////////////////////
// CEnumArray::EnumVOID implementation

STDMETHODIMP CEnumArray::XEnumVOID::Next(
	ULONG celt, void FAR* reelt, ULONG FAR* pceltFetched)
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	ASSERT_VALID(pThis);

	if (pceltFetched != NULL)
		*pceltFetched = 0;

	ASSERT(celt > 0);
	ASSERT(celt == 1 || pceltFetched != NULL);

	char FAR* pchCur = (char FAR*)reelt;
	ULONG nFetched = 0;

	ULONG celtT = celt;
	SCODE sc = E_UNEXPECTED;
	TRY
	{
		while (celtT != 0 && pThis->OnNext((void FAR*)pchCur))
		{
			pchCur += pThis->m_nSizeElem;
			--celtT;
		}
		if (pceltFetched != NULL)
			*pceltFetched = celt - celtT;
		sc = celtT == 0 ? S_OK : S_FALSE;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP CEnumArray::XEnumVOID::Skip(ULONG celt)
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	ASSERT_VALID(pThis);

	ULONG celtT = celt;
	SCODE sc = E_UNEXPECTED;
	TRY
	{
		while (celtT != 0 && pThis->OnSkip())
			--celtT;
		sc = celtT == 0 ? S_OK : S_FALSE;
	}
	END_TRY

	return celtT != 0 ? ResultFromScode(S_FALSE) : NOERROR;
}

STDMETHODIMP CEnumArray::XEnumVOID::Reset()
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	ASSERT_VALID(pThis);


	pThis->OnReset();
	return NOERROR;
}

STDMETHODIMP CEnumArray::XEnumVOID::Clone(IEnumVOID FAR* FAR* ppenm)
{
	METHOD_MANAGE_STATE(CEnumArray, EnumVOID)
	ASSERT_VALID(pThis);

	*ppenm = NULL;

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		CEnumArray* pEnumHelper = pThis->OnClone();
		ASSERT_VALID(pEnumHelper);

		// we use an extra reference to keep the original object alive
		//  (the extra reference is removed in the clone's destructor)
		if (pThis->m_pClonedFrom != NULL)
			pEnumHelper->m_pClonedFrom = pThis->m_pClonedFrom;
		else
			pEnumHelper->m_pClonedFrom = pThis;
		pEnumHelper->m_pClonedFrom->InternalAddRef();
		*ppenm = &pEnumHelper->m_xEnumVOID;

		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
