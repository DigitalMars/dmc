//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: cdebug.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"


DWORD CDebug::dwDebugLevel = 0;
BOOL CDebug::bOkToLog = FALSE;
CStdioFile CDebug::LogFile;


CDebug::CDebug()
//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------
{
  m_dwDebugLevel = CDebug::dwDebugLevel ;
  CDebug::dwDebugLevel ++;

  Reset();
} /* CDebug::CDebug()
   */


CDebug::~CDebug()
//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------
{
  CDebug::dwDebugLevel --;
} /* CDebug::~CDebug()
   */


BOOL CDebug::
  SetLogFileName(LPCSTR pszFileName)
//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------
{
  BOOL fRes;

  if (CDebug::bOkToLog) {
	 CDebug::LogFile.Close();
  }

  if (pszFileName != NULL) {
	fRes = CDebug::LogFile.Open(pszFileName, CFile::modeCreate |
								CFile::modeWrite | CFile::typeText);
	if (fRes) {
	  CDebug::bOkToLog = TRUE;
	}else{
	  CDebug::bOkToLog = FALSE;
	}
  }

  return fRes;
} /* SetLogFileName()
   */


void CDebug::
  CloseLogFile()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (CDebug::bOkToLog) {
	TRY
	{
		CDebug::LogFile.Close();
		CDebug::bOkToLog = FALSE;
	}
	CATCH(CFileException, e)
	{
		::MessageBox(NULL, (LPSTR)"Can't close log file",
					 (LPSTR)"DBGHNDLR", MB_OK | MB_ICONHAND);
	}
	END_CATCH
  }

} /* CloseLogFile()
   */


void CDebug::
  DebugOutput(CListBox* pListBox)
//----------------------------------------------------------------------------
// DebugOutput
//----------------------------------------------------------------------------
{
  if (pListBox != NULL) {
	if (IsWindow(pListBox->m_hWnd)) {
	  pListBox->SendMessage(WM_SETREDRAW, FALSE, 0L);
	  pListBox->AddString(m_szBuffer);
	  pListBox->SetCaretIndex(pListBox->GetCount());
	  pListBox->SendMessage(WM_SETREDRAW, TRUE, 0L);
	}
  }

  if (CDebug::bOkToLog) {
	TRY
	{
		CDebug::LogFile.WriteString(m_szBuffer);
		CDebug::LogFile.WriteString("\n");
		CDebug::LogFile.Flush();
	}
	CATCH(CFileException, e)
	{
		::MessageBox(NULL, (LPSTR)"Can't write to log file",
					 (LPSTR)"DBGHNDLR", MB_OK | MB_ICONHAND);
	}
	END_CATCH
  }

  Reset();

} /* DebugOutput()
   */


void CDebug::
  Reset(void)
//----------------------------------------------------------------------------
// DumpOutput
//----------------------------------------------------------------------------
{
  m_szBuffer[0] = '\0';
  m_bPrevFmt = FALSE;

  if (m_dwDebugLevel > 0) {
	lstrcpy(m_szBuffer, "|");
  }

  for (DWORD i=0; i<m_dwDebugLevel; i++) {
	lstrcat(m_szBuffer, "----");
  }

} /* Reset()
   */


CDebug& CDebug::
  operator <<(LPCSTR psz)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  m_bPrevFmt = FALSE;

  if (psz == NULL) {
	lstrcat(m_szBuffer, "NULL String");
  }else{
	lstrcat(m_szBuffer, psz);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(WORD w)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char sz[32];
  wsprintf((LPSTR)sz, "0x%.4X", w);
  lstrcat(m_szBuffer, (LPSTR)sz);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(DWORD dw)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char sz[32];
  wsprintf((LPSTR)sz, "0x%.8lX", dw);
  lstrcat(m_szBuffer, (LPSTR)sz);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(LONG l)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char sz[32];
  wsprintf((LPSTR)sz, "%ld", l);
  lstrcat(m_szBuffer, (LPSTR)sz);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(BOOL b)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (b) {
	lstrcat(m_szBuffer, (LPSTR)"TRUE");
  }else{
	lstrcat(m_szBuffer, (LPSTR)"FALSE");
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(IID iid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  LPSTR psz;

  if (iid == IID_IUnknown) {
	lstrcat(m_szBuffer, "IID_IUnknown");
  }
  else if (iid == IID_IClassFactory) {
	lstrcat(m_szBuffer, "IID_IClassFactory");
  }
  else if (iid == IID_IMalloc) {
	lstrcat(m_szBuffer, "IID_IMalloc");
  }
  else if (iid == IID_IMarshal) {
	lstrcat(m_szBuffer, "IID_IMarshal");
  }

  /* RPC Related interfaces */
  else if (iid == IID_IRpcChannel) {
	lstrcat(m_szBuffer, "IID_IRpcChannel");
  }
  else if (iid == IID_IRpcStub) {
	lstrcat(m_szBuffer, "IID_IRpcStub");
  }
  else if (iid == IID_IStubManager) {
	lstrcat(m_szBuffer, "IID_IStubManager");
  }
  else if (iid == IID_IRpcProxy) {
	lstrcat(m_szBuffer, "IID_IRpcProxy");
  }
  else if (iid == IID_IProxyManager) {
	lstrcat(m_szBuffer, "IID_IProxyManager");
  }
  else if (iid == IID_IPSFactory) {
	lstrcat(m_szBuffer, "IID_IPSFactory");
  }

  /* storage related interfaces */
  else if (iid == IID_ILockBytes) {
	lstrcat(m_szBuffer, "IID_ILockBytes");
  }
  else if (iid == IID_IStorage) {
	lstrcat(m_szBuffer, "IID_IStorage");
  }
  else if (iid == IID_IStream) {
	lstrcat(m_szBuffer, "IID_IStream");
  }
  else if (iid == IID_IEnumSTATSTG) {
	lstrcat(m_szBuffer, "IID_IEnumSTATSTG");
  }

  /* moniker related interfaces */
  else if (iid == IID_IBindCtx) {
	lstrcat(m_szBuffer, "IID_IBindCtx");
  }
  else if (iid == IID_IMoniker) {
	lstrcat(m_szBuffer, "IID_IMoniker");
  }
  else if (iid == IID_IRunningObjectTable) {
	lstrcat(m_szBuffer, "IID_IRunningObjectTable");
  }
  else if (iid == IID_IInternalMoniker) {
	lstrcat(m_szBuffer, "IID_IInternalMoniker");
  }

  /* storage related interfaces */
  else if (iid == IID_IRootStorage) {
	lstrcat(m_szBuffer, "IID_IRootStorage");
  }
  else if (iid == IID_IDfReserved1) {
	lstrcat(m_szBuffer, "IID_IDfReserved1");
  }
  else if (iid == IID_IDfReserved2) {
	lstrcat(m_szBuffer, "IID_IDfReserved2");
  }
  else if (iid == IID_IDfReserved3) {
	lstrcat(m_szBuffer, "IID_IDfReserved3");
  }

  /* concurrecny related interfaces */
  else if (iid == IID_IMessageFilter) {
	lstrcat(m_szBuffer, "IID_IMessageFilter");
  }
  else if (iid == IID_IStdMarshalInfo) {
	lstrcat(m_szBuffer, "IID_IStdMarshalInfo");
  }

  /* from oleguid.h */
  else if (iid == IID_IEnumUnknown) {
	lstrcat(m_szBuffer, "IID_IEnumUnknown");
  }
  else if (iid == IID_IEnumString) {
	lstrcat(m_szBuffer, "IID_IEnumString");
  }
  else if (iid == IID_IEnumMoniker) {
	lstrcat(m_szBuffer, "IID_IEnumMoniker");
  }
  else if (iid == IID_IEnumFORMATETC) {
	lstrcat(m_szBuffer, "IID_IEnumFORMATETC");
  }
  else if (iid == IID_IEnumOLEVERB) {
	lstrcat(m_szBuffer, "IID_IEnumOLEVERB");
  }
  else if (iid == IID_IEnumSTATDATA) {
	lstrcat(m_szBuffer, "IID_EnumSTATDATA");
  }
  else if (iid == IID_IEnumGeneric) {
	lstrcat(m_szBuffer, "IID_EnumGeneric");
  }
  else if (iid == IID_IEnumHolder) {
	lstrcat(m_szBuffer, "IID_EnumHolder");
  }
  else if (iid == IID_IEnumCallback) {
	lstrcat(m_szBuffer, "IID_EnumCallback");
  }
  else if (iid == IID_IPersistStream) {
	lstrcat(m_szBuffer, "IID_IPersistStream");
  }
  else if (iid == IID_IPersistStorage) {
	lstrcat(m_szBuffer, "IID_IPersistStorage");
  }
  else if (iid == IID_IPersistFile) {
	lstrcat(m_szBuffer, "IID_IPersistFile");
  }
  else if (iid == IID_IPersist) {
	lstrcat(m_szBuffer, "IID_IPersist");
  }
  else if (iid == IID_IViewObject) {
	lstrcat(m_szBuffer, "IID_IViewObject");
  }
  else if (iid == IID_IDataObject) {
	lstrcat(m_szBuffer, "IID_IDataObject");
  }
  else if (iid == IID_IAdviseSink) {
	lstrcat(m_szBuffer, "IID_IAdviseSink");
  }
  else if (iid == IID_IDataAdviseHolder) {
	lstrcat(m_szBuffer, "IID_IDataAdviseHolder");
  }
  else if (iid == IID_IOleAdviseHolder) {
	lstrcat(m_szBuffer, "IID_IOleAdviseHolder");
  }
  else if (iid == IID_IOleObject) {
	lstrcat(m_szBuffer, "IID_IOleObject");
  }
  else if (iid == IID_IOleInPlaceObject) {
	lstrcat(m_szBuffer, "IID_IOleInPlaceObject");
  }
  else if (iid == IID_IOleWindow) {
	lstrcat(m_szBuffer, "IID_IOleWindow");
  }
  else if (iid == IID_IOleInPlaceUIWindow) {
	lstrcat(m_szBuffer, "IID_IOleInPlaceUIWindow");
  }
  else if (iid == IID_IOleInPlaceFrame) {
	lstrcat(m_szBuffer, "IID_IOleInPlaceFrame");
  }
  else if (iid == IID_IOleInPlaceActiveObject) {
	lstrcat(m_szBuffer, "IID_IOleInPlaceActiveObject");
  }
  else if (iid == IID_IOleClientSite) {
	lstrcat(m_szBuffer, "IID_IOleClientSite");
  }
  else if (iid == IID_IOleInPlaceSite) {
	lstrcat(m_szBuffer, "IID_IOleInPlaceSite");
  }
  else if (iid == IID_IParseDisplayName) {
	lstrcat(m_szBuffer, "IID_IParseDisplayName");
  }
  else if (iid == IID_IOleContainer) {
	lstrcat(m_szBuffer, "IID_IOleContainer");
  }
  else if (iid == IID_IOleItemContainer) {
	lstrcat(m_szBuffer, "IID_IOleItemContainer");
  }
  else if (iid == IID_IOleLink) {
	lstrcat(m_szBuffer, "IID_IOleLink");
  }
  else if (iid == IID_IOleCache) {
	lstrcat(m_szBuffer, "IID_IOleCache");
  }
  else if (iid == IID_IOleManager) {
	lstrcat(m_szBuffer, "IID_IOleManager");
  }
  else if (iid == IID_IOlePresObj) {
	lstrcat(m_szBuffer, "IID_IOlePresObj");
  }
  else if (iid == IID_IDropSource) {
	lstrcat(m_szBuffer, "IID_IDropSource");
  }
  else if (iid == IID_IDropTarget) {
	lstrcat(m_szBuffer, "IID_IDropTarget");
  }
  else if (iid == IID_IDebug) {
	lstrcat(m_szBuffer, "IID_IDebug");
  }
  else if (iid == IID_IDebugStream) {
	lstrcat(m_szBuffer, "IID_IDebugStream");
  }
  else if (iid == IID_IAdviseSink2) {
	lstrcat(m_szBuffer, "IID_AdviseSink2");
  }
  else if (iid == IID_IViewObject2) {
	lstrcat(m_szBuffer, "IID_ViewObject2");
  }
  else if (iid == IID_IOleCache2) {
	lstrcat(m_szBuffer, "IID_OleCache2");
  }
  else if (iid == IID_IRunnableObject) {
	lstrcat(m_szBuffer, "IID_RunnableObject");
  }
  else if (iid == IID_IExternalConnection) {
	lstrcat(m_szBuffer, "IID_ExternalConnection");
  }
  else if (iid == IID_IStdMarshalInfo) {
	lstrcat(m_szBuffer, "IID_StdMarshalInfo");
  }
  else if (iid == IID_IOleCacheControl) {
	lstrcat(m_szBuffer, "IID_OleCacheControl");
  }
  else {

	StringFromIID(iid, &psz);
	lstrcat(m_szBuffer, psz);

	OleStdFree(psz);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPMONIKER pmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (pmk == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL");
  }else{
	LPSTR pszName;
	HRESULT hRes;
	IBindCtx  FAR  *pbc = NULL;

	hRes = CreateBindCtx(0, (LPBC FAR*)&pbc);
	hRes = pmk->GetDisplayName(pbc, NULL, (LPSTR FAR*)&pszName);
	pbc->Release();

	if (hRes == NOERROR) {
	  lstrcat(m_szBuffer, pszName);
	}else{
	  *this << hRes;
	}

	OleStdFree(pszName);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPRECT prc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (prc == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL");
  }else{
	char buf[128];

	wsprintf((LPSTR)buf, "{%d, %d, %d, %d}", prc->left, prc->top,
			 prc->right, prc->bottom);

	lstrcat(m_szBuffer, (LPSTR)buf);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(LPCRECT prc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (prc == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL");
  }else{
	char buf[128];

	wsprintf((LPSTR)buf, "{%d, %d, %d, %d}", prc->left, prc->top,
			 prc->right, prc->bottom);

	lstrcat(m_szBuffer, (LPSTR)buf);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(LPCRECTL prc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (prc == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL");
  }else{
	char buf[128];

	wsprintf((LPSTR)buf, "{%ld, %ld, %ld, %ld}", prc->left, prc->top,
			 prc->right, prc->bottom);

	lstrcat(m_szBuffer, (LPSTR)buf);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(SIZE size)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char buf[128];

  wsprintf((LPSTR)buf, "{%d, %d}", size.cx, size.cy);
  lstrcat(m_szBuffer, (LPSTR)buf);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPSIZEL psizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (psizel == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL");
  }else{
	char buf[128];

	wsprintf((LPSTR)buf, "{%ld, %ld}", psizel->cx, psizel->cy);

	lstrcat(m_szBuffer, (LPSTR)buf);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPFORMATETC petc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (petc == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL FormatEtc");
  }else{
	char buf[128];
	LPSTR pszFmt = GetFormat(petc->cfFormat);
	LPSTR pszPtd = GetTargetDevice(petc->ptd);

	wsprintf((LPSTR)buf, "{%s, %s, 0x%.8lX, %ld, 0x%.8lX}",
			 pszFmt, pszPtd, petc->dwAspect, petc->lindex, petc->tymed);

	lstrcat(m_szBuffer, (LPSTR)buf);

	OleStdFree(pszFmt);
	OleStdFree(pszPtd);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPSTGMEDIUM pmed)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  if (pmed == NULL) {
	lstrcat(m_szBuffer, (LPSTR)"NULL StgMedium");
  }else{
	char buf[128];
	LPSTR pszFmt = GetTymed(pmed->tymed);

	if (pmed->tymed == TYMED_HGLOBAL ||
		pmed->tymed == TYMED_MFPICT ||
		pmed->tymed == TYMED_GDI)
	{
	  wsprintf((LPSTR)buf, "{%s, 0x%.4X, 0x%.8lX}",
			   pszFmt, pmed->hGlobal, pmed->pUnkForRelease);
	}else{
	  wsprintf((LPSTR)buf, "{%s, 0x%.8lX, 0x%.8lX}",
			   pszFmt, pmed->pstg, pmed->pUnkForRelease);
	}

	lstrcat(m_szBuffer, (LPSTR)buf);
	OleStdFree(pszFmt);
  }

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPUNKNOWN punk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char sz[32];
  if (punk == NULL) {
	wsprintf((LPSTR)sz, "NULL");
  }else{
	wsprintf((LPSTR)sz, "0x%.8lX", punk);
  }
  lstrcat(m_szBuffer, (LPSTR)sz);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(const LPOLEINPLACEFRAMEINFO pInfo)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  char sz[FMTNAMESIZE];
  if (pInfo == NULL) {
	wsprintf((LPSTR)sz, "NULL");
  }else{
	wsprintf((LPSTR)sz, "{%u, %s, 0x%.4X, 0x%.4X, %d}", pInfo->cb,
			 ((pInfo->fMDIApp)?"TRUE":"FALSE"), pInfo->hwndFrame,
			 pInfo->haccel, pInfo->cAccelEntries);
  }
  lstrcat(m_szBuffer, (LPSTR)sz);

  return *this;
} /* operator <<()
   */


CDebug& CDebug::
  operator <<(HRESULT hRes)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_bPrevFmt) {
	lstrcat(m_szBuffer, ", ");
  }
  m_bPrevFmt = TRUE;

  SCODE sc = GetScode(hRes);
  BOOL bUnknown = FALSE;
  LPSTR lpStr;

#define CASE_SCODE(sc)\
  case sc: lpStr = (LPSTR)#sc; break;

  switch (sc) {

	/* SCODE's defined in SCODE.H */

	CASE_SCODE(S_OK)
	CASE_SCODE(S_FALSE)
	CASE_SCODE(E_UNEXPECTED)
	CASE_SCODE(E_OUTOFMEMORY)
	CASE_SCODE(E_INVALIDARG)
	CASE_SCODE(E_NOINTERFACE)
	CASE_SCODE(E_POINTER)
	CASE_SCODE(E_HANDLE)
	CASE_SCODE(E_ABORT)
	CASE_SCODE(E_FAIL)
	CASE_SCODE(E_ACCESSDENIED)

	/* SCODE's defined in OLE2.H */

	CASE_SCODE(OLE_E_OLEVERB)
	CASE_SCODE(OLE_E_ADVF)
	CASE_SCODE(OLE_E_ENUM_NOMORE)
	CASE_SCODE(OLE_E_ADVISENOTSUPPORTED)
	CASE_SCODE(OLE_E_NOCONNECTION)
	CASE_SCODE(OLE_E_NOTRUNNING)
	CASE_SCODE(OLE_E_NOCACHE)
	CASE_SCODE(OLE_E_BLANK)
	CASE_SCODE(OLE_E_CLASSDIFF)
	CASE_SCODE(OLE_E_CANT_GETMONIKER)
	CASE_SCODE(OLE_E_CANT_BINDTOSOURCE)
	CASE_SCODE(OLE_E_STATIC)
	CASE_SCODE(OLE_E_PROMPTSAVECANCELLED)
	CASE_SCODE(OLE_E_INVALIDRECT)
	CASE_SCODE(OLE_E_WRONGCOMPOBJ)
	CASE_SCODE(OLE_E_INVALIDHWND)
	CASE_SCODE(OLE_E_NOT_INPLACEACTIVE)
	CASE_SCODE(OLE_E_CANTCONVERT)
	CASE_SCODE(OLE_E_NOSTORAGE)

	CASE_SCODE(DV_E_FORMATETC)
	CASE_SCODE(DV_E_DVTARGETDEVICE)
	CASE_SCODE(DV_E_STGMEDIUM)
	CASE_SCODE(DV_E_STATDATA)
	CASE_SCODE(DV_E_LINDEX)
	CASE_SCODE(DV_E_TYMED)
	CASE_SCODE(DV_E_CLIPFORMAT)
	CASE_SCODE(DV_E_DVASPECT)
	CASE_SCODE(DV_E_DVTARGETDEVICE_SIZE)
	CASE_SCODE(DV_E_NOIVIEWOBJECT)

	CASE_SCODE(OLE_S_USEREG)
	CASE_SCODE(OLE_S_STATIC)
	CASE_SCODE(OLE_S_MAC_CLIPFORMAT)

	CASE_SCODE(CONVERT10_E_OLESTREAM_GET)
	CASE_SCODE(CONVERT10_E_OLESTREAM_PUT)
	CASE_SCODE(CONVERT10_E_OLESTREAM_FMT)
	CASE_SCODE(CONVERT10_E_OLESTREAM_BITMAP_TO_DIB)
	CASE_SCODE(CONVERT10_E_STG_FMT)
	CASE_SCODE(CONVERT10_E_STG_NO_STD_STREAM)
	CASE_SCODE(CONVERT10_E_STG_DIB_TO_BITMAP)
	CASE_SCODE(CONVERT10_S_NO_PRESENTATION)

	CASE_SCODE(CLIPBRD_E_CANT_OPEN)
	CASE_SCODE(CLIPBRD_E_CANT_EMPTY)
	CASE_SCODE(CLIPBRD_E_CANT_SET)
	CASE_SCODE(CLIPBRD_E_BAD_DATA)
	CASE_SCODE(CLIPBRD_E_CANT_CLOSE)

	CASE_SCODE(DRAGDROP_E_NOTREGISTERED)
	CASE_SCODE(DRAGDROP_E_ALREADYREGISTERED)
	CASE_SCODE(DRAGDROP_E_INVALIDHWND)
	CASE_SCODE(DRAGDROP_S_DROP)
	CASE_SCODE(DRAGDROP_S_CANCEL)
	CASE_SCODE(DRAGDROP_S_USEDEFAULTCURSORS)

	CASE_SCODE(OLEOBJ_E_NOVERBS)
	CASE_SCODE(OLEOBJ_E_INVALIDVERB)
	CASE_SCODE(OLEOBJ_S_INVALIDVERB)
	CASE_SCODE(OLEOBJ_S_CANNOT_DOVERB_NOW)
	CASE_SCODE(OLEOBJ_S_INVALIDHWND)
	CASE_SCODE(INPLACE_E_NOTUNDOABLE)
	CASE_SCODE(INPLACE_E_NOTOOLSPACE)
	CASE_SCODE(INPLACE_S_TRUNCATED)

	/* SCODE's defined in COMPOBJ.H */

	CASE_SCODE(CO_E_NOTINITIALIZED)
	CASE_SCODE(CO_E_ALREADYINITIALIZED)
	CASE_SCODE(CO_E_CANTDETERMINECLASS)
	CASE_SCODE(CO_E_CLASSSTRING)
	CASE_SCODE(CO_E_IIDSTRING)
	CASE_SCODE(CO_E_APPNOTFOUND)
	CASE_SCODE(CO_E_APPSINGLEUSE)
	CASE_SCODE(CO_E_ERRORINAPP)
	CASE_SCODE(CO_E_DLLNOTFOUND)
	CASE_SCODE(CO_E_ERRORINDLL)
	CASE_SCODE(CO_E_WRONGOSFORAPP)
	CASE_SCODE(CO_E_OBJNOTREG)
	CASE_SCODE(CO_E_OBJISREG)
	CASE_SCODE(CO_E_OBJNOTCONNECTED)
	CASE_SCODE(CO_E_APPDIDNTREG)

	CASE_SCODE(CLASS_E_NOAGGREGATION)
	CASE_SCODE(CLASS_E_CLASSNOTAVAILABLE)

	CASE_SCODE(REGDB_E_READREGDB)
	CASE_SCODE(REGDB_E_WRITEREGDB)
	CASE_SCODE(REGDB_E_KEYMISSING)
	CASE_SCODE(REGDB_E_INVALIDVALUE)
	CASE_SCODE(REGDB_E_CLASSNOTREG)
	CASE_SCODE(REGDB_E_IIDNOTREG)

	CASE_SCODE(RPC_E_CALL_REJECTED)
	CASE_SCODE(RPC_E_CALL_CANCELED)
	CASE_SCODE(RPC_E_CANTPOST_INSENDCALL)
	CASE_SCODE(RPC_E_CANTCALLOUT_INASYNCCALL)
	CASE_SCODE(RPC_E_CANTCALLOUT_INEXTERNALCALL)
	CASE_SCODE(RPC_E_CONNECTION_TERMINATED)
	CASE_SCODE(RPC_E_SERVER_DIED)
	CASE_SCODE(RPC_E_CLIENT_DIED)
	CASE_SCODE(RPC_E_INVALID_DATAPACKET)
	CASE_SCODE(RPC_E_CANTTRANSMIT_CALL)
	CASE_SCODE(RPC_E_CLIENT_CANTMARSHAL_DATA)
	CASE_SCODE(RPC_E_CLIENT_CANTUNMARSHAL_DATA)
	CASE_SCODE(RPC_E_SERVER_CANTMARSHAL_DATA)
	CASE_SCODE(RPC_E_SERVER_CANTUNMARSHAL_DATA)
	CASE_SCODE(RPC_E_INVALID_DATA)
	CASE_SCODE(RPC_E_INVALID_PARAMETER)
	CASE_SCODE(RPC_E_CANTCALLOUT_AGAIN)
	CASE_SCODE(RPC_E_UNEXPECTED)

	/* SCODE's defined in DVOBJ.H */

	CASE_SCODE(DATA_S_SAMEFORMATETC)
	CASE_SCODE(VIEW_E_DRAW)
	CASE_SCODE(VIEW_S_ALREADY_FROZEN)

	CASE_SCODE(CACHE_E_NOCACHE_UPDATED)
	CASE_SCODE(CACHE_S_FORMATETC_NOTSUPPORTED)
	CASE_SCODE(CACHE_S_SAMECACHE)
	CASE_SCODE(CACHE_S_SOMECACHES_NOTUPDATED)

	/* SCODE's defined in MONIKER.H */

	CASE_SCODE(MK_E_CONNECTMANUALLY)
	CASE_SCODE(MK_E_EXCEEDEDDEADLINE)
	CASE_SCODE(MK_E_NEEDGENERIC)
	CASE_SCODE(MK_E_UNAVAILABLE)
	CASE_SCODE(MK_E_SYNTAX)
	CASE_SCODE(MK_E_NOOBJECT)
	CASE_SCODE(MK_E_INVALIDEXTENSION)
	CASE_SCODE(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED)
	CASE_SCODE(MK_E_NOTBINDABLE)
	CASE_SCODE(MK_E_NOTBOUND)
	CASE_SCODE(MK_E_CANTOPENFILE)
	CASE_SCODE(MK_E_MUSTBOTHERUSER)
	CASE_SCODE(MK_E_NOINVERSE)
	CASE_SCODE(MK_E_NOSTORAGE)
	CASE_SCODE(MK_E_NOPREFIX)
	CASE_SCODE(MK_S_REDUCED_TO_SELF)
	CASE_SCODE(MK_S_ME)
	CASE_SCODE(MK_S_HIM)
	CASE_SCODE(MK_S_US)
	CASE_SCODE(MK_S_MONIKERALREADYREGISTERED)

	/* SCODE's defined in STORAGE.H */

	CASE_SCODE(STG_E_INVALIDFUNCTION)
	CASE_SCODE(STG_E_FILENOTFOUND)
	CASE_SCODE(STG_E_PATHNOTFOUND)
	CASE_SCODE(STG_E_TOOMANYOPENFILES)
	CASE_SCODE(STG_E_ACCESSDENIED)
	CASE_SCODE(STG_E_INVALIDHANDLE)
	CASE_SCODE(STG_E_INSUFFICIENTMEMORY)
	CASE_SCODE(STG_E_INVALIDPOINTER)
	CASE_SCODE(STG_E_NOMOREFILES)
	CASE_SCODE(STG_E_DISKISWRITEPROTECTED)
	CASE_SCODE(STG_E_SEEKERROR)
	CASE_SCODE(STG_E_WRITEFAULT)
	CASE_SCODE(STG_E_READFAULT)
	CASE_SCODE(STG_E_SHAREVIOLATION)
	CASE_SCODE(STG_E_LOCKVIOLATION)
	CASE_SCODE(STG_E_FILEALREADYEXISTS)
	CASE_SCODE(STG_E_INVALIDPARAMETER)
	CASE_SCODE(STG_E_MEDIUMFULL)
	CASE_SCODE(STG_E_ABNORMALAPIEXIT)
	CASE_SCODE(STG_E_INVALIDHEADER)
	CASE_SCODE(STG_E_INVALIDNAME)
	CASE_SCODE(STG_E_UNKNOWN)
	CASE_SCODE(STG_E_UNIMPLEMENTEDFUNCTION)
	CASE_SCODE(STG_E_INVALIDFLAG)
	CASE_SCODE(STG_E_INUSE)
	CASE_SCODE(STG_E_NOTCURRENT)
	CASE_SCODE(STG_E_REVERTED)
	CASE_SCODE(STG_E_CANTSAVE)
	CASE_SCODE(STG_E_OLDFORMAT)
	CASE_SCODE(STG_E_OLDDLL)
	CASE_SCODE(STG_E_SHAREREQUIRED)
	CASE_SCODE(STG_E_NOTFILEBASEDSTORAGE)
	CASE_SCODE(STG_E_EXTANTMARSHALLINGS)
	CASE_SCODE(STG_S_CONVERTED)

	default:
	  bUnknown = TRUE;
	  break;

  } /* switch() */

  if (bUnknown) {
	char strFmt[DEBUGBUFSIZE];
	sprintf((LPSTR)strFmt, "0x%.8lX", hRes);
	strcat(m_szBuffer, strFmt);
  }else{
	strcat(m_szBuffer, lpStr);
  }

  return *this;
} /* operator <<()
   */


LPSTR CDebug::
  GetFormat(CLIPFORMAT cfFormat)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  LPSTR szFmt;

  if ((szFmt = (LPSTR)OleStdAlloc(FMTNAMESIZE)) == NULL) {
	return NULL;
  }

  if (cfFormat == NULL) {
	lstrcpy(szFmt, "NULL Format");
	return szFmt;
  }

  if (GetClipboardFormatName(cfFormat, szFmt, FMTNAMESIZE-1) == 0) {
	switch (cfFormat)
	{
	  case CF_BITMAP:
		lstrcpy(szFmt, "CF_BITMAP");
		break;
	  case CF_DIB:
		lstrcpy(szFmt, "CF_DIB");
		break;
	  case CF_DIF:
		lstrcpy(szFmt, "CF_DIF");
		break;
	  case CF_DSPBITMAP:
		lstrcpy(szFmt, "CF_DSPBITMAP");
		break;
	  case CF_DSPMETAFILEPICT:
		lstrcpy(szFmt, "CF_DSPMETAFILEPICT");
		break;
	  case CF_DSPTEXT:
		lstrcpy(szFmt, "CF_DSPTEXT");
		break;
	  case CF_METAFILEPICT:
		lstrcpy(szFmt, "CF_METAFILEPICT");
		break;
	  case CF_OEMTEXT:
		lstrcpy(szFmt, "CF_OEMTEXT");
		break;
	  case CF_OWNERDISPLAY:
		lstrcpy(szFmt, "CF_OWNERDISPLAY");
		break;
	  case CF_PALETTE:
		lstrcpy(szFmt, "CF_PALETTE");
		break;
	  case CF_PENDATA:
		lstrcpy(szFmt, "CF_PENDATA");
		break;
	  case CF_RIFF:
		lstrcpy(szFmt, "CF_RIFF");
		break;
	  case CF_SYLK:
		lstrcpy(szFmt, "CF_SYLK");
		break;
	  case CF_TEXT:
		lstrcpy(szFmt, "CF_TEXT");
		break;
	  case CF_TIFF:
		lstrcpy(szFmt, "CF_TIFF");
		break;
	  case CF_WAVE:
		lstrcpy(szFmt, "CF_WAVE");
		break;
	  default:
		lstrcpy(szFmt, "Unknown Clipformat");
		break;

	} // switch()
  }

  return szFmt;

} /* GetFormat()
   */


LPSTR CDebug::
  GetTargetDevice(DVTARGETDEVICE FAR* ptd)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  LPSTR szFmt;
  BOOL fGood=FALSE;

  if ((szFmt = (LPSTR)OleStdAlloc(FMTNAMESIZE)) == NULL) {
	return NULL;
  }

  if (ptd == NULL) {
	lstrcpy(szFmt,"NULL ptd");
  }else{
	if (! IsBadReadPtr(ptd, 4)) {
	  if (! IsBadHugeReadPtr(ptd, ptd->tdSize)) {
		fGood = TRUE;
		lstrcpy(szFmt, (LPSTR)((LPSTR)ptd + ptd->tdDeviceNameOffset));
	  }
	}

	if (!fGood) {
	  lstrcpy(szFmt, (LPSTR)"Invalid ptd");
	}
  }

  return szFmt;
} /* GetTargetDevice()
   */


LPSTR CDebug::
  GetTymed(DWORD tymed)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  BOOL bPrevFmt=FALSE;
  LPSTR szFmt;

  if ((szFmt = (LPSTR)OleStdAlloc(FMTNAMESIZE)) == NULL) {
	return NULL;
  }

  szFmt[0] = '\0';

  if (tymed & TYMED_HGLOBAL) {
	lstrcat(szFmt, "HGLOBAL");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_FILE) {
	if (bPrevFmt) { lstrcat(szFmt, "|"); }
	lstrcat(szFmt, "FILE");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_ISTREAM) {
	if (bPrevFmt) { lstrcat(szFmt, "|"); }
	lstrcat(szFmt, "ISTREAM");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_ISTORAGE) {
	if (bPrevFmt) { lstrcat(szFmt, "|"); }
	lstrcat(szFmt, "ISTORAGE");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_GDI) {
	if (bPrevFmt) { lstrcat(szFmt, "|"); }
	lstrcat(szFmt, "GDI");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_MFPICT) {
	if (bPrevFmt) { lstrcat(szFmt, "|"); }
	lstrcat(szFmt, "MFPICT");
	bPrevFmt = TRUE;
  }

  if (tymed & TYMED_NULL) {
	lstrcat(szFmt, "TYMED_NULL");
  }

  return szFmt;
} /* GetTymed()
   */
