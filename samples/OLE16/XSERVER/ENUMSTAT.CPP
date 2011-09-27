//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module:
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"


CEnumStatData::CEnumStatData(ULONG Count, LPSTATDATA pStat, BOOL bDebug,
							 ULONG wStartIndex /*= 0*/)
//----------------------------------------------------------------------------
// Constructor:
//----------------------------------------------------------------------------
{
  DWORD dwsize;

  g_dwObjectCount++ ;

  m_dwRefs = 1;
  m_Count = Count;
  m_Index = wStartIndex;
  m_bDebug = bDebug;

  dwsize = sizeof(STATDATA) * m_Count;
  if ((m_pStat = (LPSTATDATA)OleStdAlloc(dwsize)) == NULL) {
	delete this;
  }

  for (WORD i=0; i<m_Count; i++) {
	OleStdCopyStatData(&m_pStat[i], &pStat[i]);
  }
} /* CEnumStatData::CEnumStatData()
   */



CEnumStatData::~CEnumStatData()
//----------------------------------------------------------------------------
// Destructor:
//----------------------------------------------------------------------------
{
  if (m_pStat != NULL) {

	for (WORD i=0; i<m_Count; i++) {
	  OleStdFree(m_pStat[i].formatetc.ptd);
	  if (m_pStat[i].pAdvSink != NULL) {
		m_pStat[i].pAdvSink->Release();
	  }
	}

	OleStdFree(m_pStat);
	m_pStat = NULL;
	m_Count = 0;
  }

  g_dwObjectCount-- ;

} /* CEnumStatData::~CEnumStatData()
   */


STDMETHODIMP CEnumStatData::
  QueryInterface(REFIID riid, LPLPVOID ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::QueryInterface(" << riid << (LPVOID)ppobj
	 << ") Called" ;
  db.DebugOutput();

  ULONG dwRefs;
  HRESULT hRes = ResultFromScode(E_NOINTERFACE);

  *ppobj = NULL;

  if ((riid == IID_IUnknown) || (riid == IID_IEnumSTATDATA)) {
	*ppobj = (LPVOID)this;
  }

  if (*ppobj != NULL) {
	dwRefs = ((LPUNKNOWN)*ppobj)->AddRef();
	hRes = NOERROR;
  }

  if (hRes == NOERROR) {
	db << "IEnumSTATDATA::QueryInterface(" << riid << (LPUNKNOWN)*ppobj
	   << ") Returning " << hRes ;
  }else{
	db << "IEnumSTATDATA::QueryInterface() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;

} /* CEnumStatData::QueryInterface()
   */


STDMETHODIMP_(ULONG) CEnumStatData::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::AddRef()  Called";
  db.DebugOutput();

  ULONG dwRefs = ++m_dwRefs;

  db << "IEnumSTATDATA::AddRef()  Returning " << dwRefs;
  db.DebugOutput();

  return dwRefs;
} /* CEnumStatData::AddRef()
   */


STDMETHODIMP_(ULONG) CEnumStatData::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::Release() Called";
  db.DebugOutput();

  ULONG dwRefs = --m_dwRefs;

  if (dwRefs == 0) {
	delete this;
  }

  db << "IEnumSTATDATA::Release() Returning " << dwRefs;
  db.DebugOutput();
  return dwRefs;

} /* CEnumStatData::Release()
   */


STDMETHODIMP CEnumStatData::
  Next(ULONG celt, STATDATA FAR * rgelt, ULONG FAR* pceltFetched)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::Next(" << celt << (LPVOID)rgelt
	 << (LPUNKNOWN)pceltFetched << ") Called";
  db.DebugOutput();

  ULONG i=0, wOffset;
  HRESULT hRes=NOERROR;

  while (i < celt) {
	wOffset = m_Index + i;

	if (wOffset < m_Count) {
	  OleStdCopyStatData(&rgelt[i], &m_pStat[wOffset]);
	  i++;
	}else{
	  break;
	}
  }

  m_Index += i;

  if (pceltFetched != NULL) {
	*pceltFetched = i;
  }

  if (i != celt) {
	hRes = ResultFromScode(S_FALSE);
  }

  if (pceltFetched != NULL) {
	db << "IEnumSTATDATA::Next(" << celt << (LPVOID)rgelt << *pceltFetched
	   << ") Returning " << hRes;
  }else{
	db << "IEnumSTATDATA::Next() Returning " << hRes;
  }
  db.DebugOutput();
  return hRes;

} /* Next()
   */


STDMETHODIMP CEnumStatData::
  Skip(ULONG celt)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::Skip(" << celt << ") Called";
  db.DebugOutput();

  ULONG i=0, wOffset;
  HRESULT hRes = NOERROR;

  while (i < celt) {
	wOffset = m_Index + i;

	if (wOffset < m_Count) {
	  i++;
	}else{
	  break;
	}
  }

  m_Index += i;

  if (i != celt) {
	hRes = ResultFromScode(S_FALSE);
  }

  db << "IEnumSTATDATA::Skip() Returning " << hRes;
  db.DebugOutput();
  return hRes;

} /* Skip()
   */


STDMETHODIMP CEnumStatData::
  Reset()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::Reset() Called";
  db.DebugOutput();

  HRESULT hRes = NOERROR;
  m_Index = 0;

  db << "IEnumSTATDATA::Release() Returning " << hRes;
  db.DebugOutput();
  return hRes;

} /* Reset()
   */


STDMETHODIMP CEnumStatData::
  Clone(IEnumSTATDATA FAR* FAR* ppenum)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_bDebug);

  db << "IEnumSTATDATA::Clone(" << (LPVOID)ppenum << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_OUTOFMEMORY);

  if (ppenum == NULL) {
	hRes = ResultFromScode(E_INVALIDARG);
	goto errReturn;
  }

  *ppenum = new CEnumStatData(m_Count, m_pStat, m_bDebug, m_Index);
  if (*ppenum != NULL) {
	hRes = NOERROR;
  }

errReturn:

  if (hRes == NOERROR) {
	db << "IEnumSTATDATA::Clone(" << (LPUNKNOWN)*ppenum << ") Returning "
	   << hRes;
  }else{
	db << "IEnumSTATDATA::Clone() Returning " << hRes;
  }

  db.DebugOutput();
  return hRes;

} /* Clone()
   */
