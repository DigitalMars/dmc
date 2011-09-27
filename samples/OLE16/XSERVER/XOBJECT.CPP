//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: xobject.cpp
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"

#define NUM_CACHES  2

STATDATA g_sdCacheList[NUM_CACHES] =
{
  CF_METAFILEPICT, NULL, DVASPECT_CONTENT, -1, TYMED_MFPICT, 0, NULL, 0,
  CF_METAFILEPICT, NULL, DVASPECT_ICON,    -1, TYMED_MFPICT, 0, NULL, 0
};


CXObject::CXObject(LPUNKNOWN pUnkOuter)
 : m_Unknown(this), m_PersistStorage(this), m_ViewObject2(this),
   m_OleObject(this), m_DataObject(this), m_OleCache2(this),
   m_RunnableObject(this), m_ExternalConnection(this), m_StdMarshalInfo(this),
   m_Dispatch(this)
//----------------------------------------------------------------------------
// Constructor:
//----------------------------------------------------------------------------
{
  g_dwObjectCount++ ;

  m_dwRefs = 1;
  m_pStg = NULL;
  m_pClientSite = NULL;

  m_bIsDirty = TRUE;
  m_bIsLocked = m_bIsRunning = m_bDebug = FALSE;
  m_fNoScribbleMode = m_fSaveWithSameAsLoad = FALSE;

  m_Size.cx = 192;
  m_Size.cy = 128;
  DeviceToHiMetric(&m_Size);

  GetObject(GetStockObject(ANSI_VAR_FONT), sizeof(LOGFONT), &m_lf);

  HDC hDC = GetDC(NULL);
  m_lf.lfHeight = -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72);
  ReleaseDC(NULL, hDC);

  m_lf.lfWidth = 0;
  m_lf.lfEscapement = 0;
  m_lf.lfOrientation = 0;
  m_lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
  m_lf.lfClipPrecision = CLIP_STROKE_PRECIS;
  m_lf.lfQuality = DRAFT_QUALITY;
  m_lf.lfPitchAndFamily = FF_ROMAN | VARIABLE_PITCH ;
  lstrcpy(m_lf.lfFaceName, "Times New Roman");

  m_dwStrong = m_dwRegister = 0;

  m_cfFormat = RegisterClipboardFormat("XServer");

  m_pViewObject2 = NULL;
  m_pOleCache2 = NULL;
  m_pDataObject = NULL;
  m_pPersistStg = NULL;

  m_pOleAdviseHolder = NULL;
  m_pDataAdviseHolder = NULL;
  m_pViewAdviseSink = NULL;
  m_dwAspects = 0;
  m_advf = 0;

  if (pUnkOuter == NULL) {
	m_pUnkOuter = &m_Unknown;
  }else{
	m_pUnkOuter = pUnkOuter;
  }

} /* CXObject::CXObject()
   */


CXObject::~CXObject()
//----------------------------------------------------------------------------
// Destructor:
//----------------------------------------------------------------------------
{
  ULONG dwRefs;

  if (m_pViewAdviseSink != NULL) {
	dwRefs = m_pViewAdviseSink->Release();
	m_pViewAdviseSink = NULL;
  }

  if (m_pOleAdviseHolder != NULL) {
	dwRefs = m_pOleAdviseHolder->Release();
	m_pOleAdviseHolder = NULL;
  }

  if (m_pDataAdviseHolder != NULL) {
	dwRefs = m_pDataAdviseHolder->Release();
	m_pDataAdviseHolder = NULL;
  }

  if (m_pClientSite != NULL) {
	dwRefs = m_pClientSite->Release();
	m_pClientSite = NULL;
  }

  if (m_pPersistStg != NULL) {
	dwRefs = m_pPersistStg->Release();
	m_pPersistStg = NULL;
  }

  if (m_pDataObject != NULL) {
	dwRefs = m_pDataObject->Release();
	m_pDataObject = NULL;
  }

  if (m_pViewObject2 != NULL) {
	dwRefs = m_pViewObject2->Release();
	m_pViewObject2 = NULL;
  }

  if (m_pOleCache2 != NULL) {
	dwRefs = m_pOleCache2->Release();
	m_pOleCache2 = NULL;
  }

  if (m_pStg != NULL) {
	dwRefs = m_pStg->Release();
	m_pStg = NULL;
  }

  g_dwObjectCount-- ;

} /* CXObject::~CXObject()
   */


STDMETHODIMP CXObject::
  CreateObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
//----------------------------------------------------------------------------
// CreateObject:
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);
  ULONG dwRefs;
  CLSID clsid_D, clsid_ND;

  hRes = CLSIDFromString("{00010042-0000-0000-C000-000000000046}", &clsid_D);
  hRes = CLSIDFromString("{00010043-0000-0000-C000-000000000046}", &clsid_ND);

  m_ClsId = rclsid;

  if (m_ClsId == clsid_D) {
	m_bDebug = TRUE;
  }else if (m_ClsId == clsid_ND) {
	m_bDebug = FALSE;
  }else{
	if (ppv != NULL) {
	  *ppv = NULL;
	}
	return ResultFromScode(CLASS_E_CLASSNOTAVAILABLE);
  }

  hRes = CreateOleAdviseHolder(&m_pOleAdviseHolder);
  hRes = CreateDataAdviseHolder(&m_pDataAdviseHolder);

  if ((hRes =  m_Unknown.QueryInterface(riid, ppv)) == NOERROR) {
	dwRefs = ((LPUNKNOWN)*ppv)->Release();

	hRes = CreateDataCache(NULL, m_ClsId, IID_IOleCache2,
						   (LPLPVOID)&m_pOleCache2);

	if (hRes == NOERROR) {
	  hRes = m_pOleCache2->QueryInterface(IID_IViewObject2,
										  (LPLPVOID)&m_pViewObject2);
	}

	if (hRes == NOERROR) {
	  hRes = m_pOleCache2->QueryInterface(IID_IDataObject,
										  (LPLPVOID)&m_pDataObject);
	}

	if (hRes == NOERROR) {
	  hRes = m_pOleCache2->QueryInterface(IID_IPersistStorage,
										  (LPLPVOID)&m_pPersistStg);
	}
  }

  return hRes;

} /* CreateObject()
   */


STDMETHODIMP_(VOID) CXObject::
  SendViewChange(DWORD dwAspects)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_pViewAdviseSink != NULL) {
	if ((m_dwAspects & dwAspects) != 0) {
	  m_pViewAdviseSink->OnViewChange(dwAspects, (LONG)-1);

	  if (m_advf & ADVF_ONLYONCE) {
		m_pViewAdviseSink->Release();
		m_pViewAdviseSink = NULL;
	  }
	}
  }

} /* SendViewChange()
   */


STDMETHODIMP CXObject::
  HandsOffStorage()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_pStg != NULL) {
	m_pStg->Release();
	m_pStg = NULL;
  }
  return NOERROR;

} /* HandsOffStorage()
   */


STDMETHODIMP CXObject::
  RememberStorage(LPSTORAGE pStg, BOOL bCheckInitial /*= TRUE*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (bCheckInitial && (m_pStg != NULL)) {
	return ResultFromScode(CO_E_ALREADYINITIALIZED);
  }

  HandsOffStorage();
  if (pStg != NULL) {
	(m_pStg = pStg)->AddRef();
  }

  return NOERROR;

} /* RememberStorage()
   */


STDMETHODIMP CXObject::
  LoadNativeData(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_INVALIDARG);
  LPSTREAM pStm=NULL;
  ULONG cbRead;

  if (pStg == NULL) {
	return hRes;
  }

  if (m_pStg != NULL) {
	return ResultFromScode(CO_E_ALREADYINITIALIZED);
  }

  if (GetConvertStg(pStg) == NOERROR) {
	SetConvertStg(pStg, FALSE);
  }

  if ((hRes = OpenOrCreateIStream("CONTENTS", pStg, &pStm)) != NOERROR) {
	return hRes;
  }

  hRes = pStm->Read((LPVOID)&m_Size, sizeof(SIZEL), &cbRead);
  hRes = pStm->Read((LPVOID)&m_lf, sizeof(LOGFONT), &cbRead);

  if (pStm != NULL) {
	pStm->Release();
  }

  if (hRes == NOERROR) {
	hRes = RememberStorage(pStg);
	m_bIsDirty = FALSE;
  }

  return (hRes == NOERROR) ? NOERROR : ResultFromScode(E_FAIL);

} /* LoadNativeData()
   */


STDMETHODIMP CXObject::
  SaveNativeData(LPSTORAGE pStg, BOOL fSameAsLoad)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_INVALIDARG);
  LPSTREAM pStm=NULL;
  DWORD grfUpdf = UPDFCACHE_IFBLANK | UPDFCACHE_ONSAVECACHE;
  LPSTR pszUserType;
  ULONG cbWrote;

  if (pStg == NULL) {
	return hRes;
  }

  if ((hRes = OpenOrCreateIStream("CONTENTS", pStg, &pStm)) != NOERROR) {
	return hRes;
  }

  hRes = pStm->Write((LPVOID)&m_Size, sizeof(SIZEL), &cbWrote);
  hRes = pStm->Write((LPVOID)&m_lf, sizeof(LOGFONT), &cbWrote);

  if (pStm != NULL) {
	pStm->Release();
  }

  hRes = m_OleCache2.UpdateCache(&m_DataObject, grfUpdf, 0);

  hRes = WriteClassStg(pStg, m_ClsId);

  hRes = OleRegGetUserType(m_ClsId, USERCLASSTYPE_FULL, &pszUserType);
  hRes = WriteFmtUserTypeStg(pStg, m_cfFormat, pszUserType);

  OleStdFree(pszUserType);

  m_fSaveWithSameAsLoad = fSameAsLoad;
  m_fNoScribbleMode = TRUE;

  return (hRes == NOERROR) ? NOERROR : ResultFromScode(E_FAIL);

} /* SaveNativeData()
   */


STDMETHODIMP CXObject::
  SaveCompleted(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;

  if ((hRes = RememberStorage(pStg, FALSE)) != NOERROR) {
	return hRes;
  }

  if (pStg != NULL || m_fSaveWithSameAsLoad) {
	if (m_fNoScribbleMode) {
	  if (m_pOleAdviseHolder != NULL) {
		m_pOleAdviseHolder->SendOnSave();
	  }

	  m_bIsDirty = FALSE;
	}

	m_fSaveWithSameAsLoad = FALSE;
  }

  m_fNoScribbleMode = FALSE;

  return NOERROR;

} /* SaveCompleted()
   */


STDMETHODIMP CXObject::
  SetClientSite(LPOLECLIENTSITE pClientSite)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = NOERROR;

  if (pClientSite == NULL) {
	if (m_pClientSite != NULL) {
	  m_pClientSite->Release();
	  m_pClientSite = NULL;
	}

  }else if ((m_pClientSite == NULL) && (pClientSite != NULL)) {
	(m_pClientSite = pClientSite)->AddRef();

  }else{
	hRes = ResultFromScode(E_FAIL);
  }

  return hRes;

} /* SetClientSite()
   */


STDMETHODIMP CXObject::
  Close(DWORD dwSaveOption)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = NOERROR;

  if (dwSaveOption != OLECLOSE_NOSAVE) {
	if (m_bIsDirty && (m_pClientSite != NULL)) {
	  hRes = m_pClientSite->SaveObject();
	}
  }

  if (m_RunnableObject.IsRunning()) {
	hRes = SetRunningState(FALSE);

	if (m_bIsLocked) {
	  m_bIsLocked = FALSE;
	  hRes = m_RunnableObject.LockRunning(FALSE, TRUE);

	  if (m_pClientSite != NULL) {
		hRes = m_pClientSite->OnShowWindow(FALSE);
	  }
	}
  }

  return hRes;

} /* Close()
   */


STDMETHODIMP CXObject::
  SetRunningState(BOOL bRun)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = NOERROR;
  LPOLECACHECONTROL pOleCacheControl=NULL;

  if (m_bIsRunning == bRun) {
	return hRes;
  }

  m_bIsRunning = bRun;

  hRes = SendChangeNotification();
  hRes = m_pOleCache2->QueryInterface(IID_IOleCacheControl,
									  (LPVOID FAR*)&pOleCacheControl);

  if (m_bIsRunning) {
	hRes = pOleCacheControl->OnRun(&m_DataObject);
	hRes = RegisterAsRunning();
  }else{
	hRes = pOleCacheControl->OnStop();
	hRes = OleStdRevokeAsRunning(&m_dwRegister);
	hRes = m_pOleAdviseHolder->SendOnClose();
  }

  if (pOleCacheControl != NULL) {
	pOleCacheControl->Release();
  }

  if (m_pClientSite != NULL) {
	LPOLECONTAINER pContainer;
	if ((hRes = m_pClientSite->GetContainer(&pContainer)) == NOERROR) {
	  if (SUCCEEDED(hRes) && (pContainer != NULL)) {
		pContainer->LockContainer(bRun);
		pContainer->Release();
	  }
	}
  }

  return NOERROR; /* this routine can't really fail !*/

} /* SetRunningState()
   */


STDMETHODIMP CXObject::
  GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (ppmk == NULL) {
	return ResultFromScode(E_INVALIDARG);
  }

  switch(dwAssign) {
	case OLEGETMONIKER_UNASSIGN:
	  return ResultFromScode(E_INVALIDARG);
	  break;

	default:
	  if (m_pClientSite != NULL) {
		return m_pClientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
	  }
	  break;
  }

  return ResultFromScode(E_FAIL);

} /* GetMoniker()
   */


STDMETHODIMP CXObject::
  DoVerb(LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite,
		 LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = NOERROR;

	if (lindex != 0 && lindex != -1) {
		return ResultFromScode(DV_E_LINDEX);
  }

  switch (iVerb) {
	case OLEIVERB_PRIMARY: /* Enter User Visible State */
	case OLEIVERB_OPEN:
	case OLEIVERB_SHOW:

	  if (m_bIsRunning == FALSE) {
		hRes = m_RunnableObject.Run(NULL);
	  }

	  if (m_bIsLocked == FALSE) {
		m_bIsLocked = TRUE;
		hRes = m_RunnableObject.LockRunning(TRUE, FALSE);
		hRes = SendChangeNotification();
	  }

	  if (m_pClientSite != NULL) {
		m_pClientSite->OnShowWindow(TRUE);
		m_pClientSite->ShowObject();
	  }
	  break;

	case 1: /* Enter Loaded State */
	  if ((m_pClientSite == pActiveSite) && m_bIsRunning) {
		hRes = m_OleObject.Close(OLECLOSE_SAVEIFDIRTY);
	  }
	  break;

	case OLEIVERB_HIDE:   /* force into the running state */
	  if (m_bIsRunning == FALSE) {
		hRes = m_RunnableObject.Run(NULL);

	  }else if (m_bIsLocked) {
		m_bIsLocked = FALSE;
		hRes = m_RunnableObject.LockRunning(FALSE, FALSE);
		hRes = SendChangeNotification();

		if (m_pClientSite != NULL) {
		  m_pClientSite->OnShowWindow(FALSE);
		}
	  }
	  break;

	default:
	  hRes = ResultFromScode(OLEOBJ_S_INVALIDVERB);
	  break;
  }

  return hRes;

} /* DoVerb()
   */


STDMETHODIMP CXObject::
  Update(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
	BOOL    bStartedNow = FALSE;
  DWORD grfUpdf = UPDFCACHE_IFBLANK | UPDFCACHE_ONSAVECACHE;
  HRESULT hRes;

  if (m_bIsRunning == FALSE) {
	hRes = m_RunnableObject.Run(NULL);
	bStartedNow = TRUE;
  }

  hRes = m_OleCache2.UpdateCache(&m_DataObject, grfUpdf, 0);

	if (bStartedNow) {
	m_OleObject.Close(OLECLOSE_SAVEIFDIRTY);
  }

  return hRes;

} /* Update()
   */


STDMETHODIMP CXObject::
  SendChangeNotification()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  m_pDataAdviseHolder->SendOnDataChange(&m_DataObject, 0, 0);
  SendViewChange(m_dwAspects);
  return NOERROR;

} /* SendChangeNotification()
   */


STDMETHODIMP CXObject::
  SetExtent(LPSIZEL psizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (psizel == NULL) {
	return hRes;
  }

  m_Size = *psizel;
  m_bIsDirty = TRUE;

  return SendChangeNotification();

} /* SetExtent()
   */


STDMETHODIMP CXObject::
  GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE FAR* ptd,
			LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (lpsizel != NULL) {
	if (dwDrawAspect == DVASPECT_CONTENT) {
	  lpsizel->cx = m_Size.cx;
	  lpsizel->cy = m_Size.cy;
	  hRes = NULL;
	}else{
	  if (m_pViewObject2 != NULL) {
		hRes = m_pViewObject2->GetExtent(dwDrawAspect, lindex, ptd, lpsizel);
	  }else{
		hRes = ResultFromScode(E_FAIL);
	  }
	}
  }

  return hRes;

} /* GetExtent()
   */


STDMETHODIMP CXObject::
  GetData(LPFORMATETC petc, LPSTGMEDIUM pmed)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);
  HANDLE hGlobal=NULL;

  if ((petc == NULL) || (pmed == NULL)) {
	return ResultFromScode(E_INVALIDARG);
  }

  pmed->pstg = NULL;
  pmed->tymed = TYMED_NULL;
  pmed->pUnkForRelease = NULL;

  if (QueryGetData(petc) != NOERROR) {
	return ResultFromScode(DV_E_FORMATETC);
  }

  if ((petc->cfFormat == CF_METAFILEPICT) && (petc->tymed & TYMED_MFPICT)) {

	switch (petc->dwAspect) {
	  case DVASPECT_CONTENT:
		hGlobal = OleStdGetMetafile(&m_ViewObject2, petc->dwAspect,petc->ptd);
		break;

	  case DVASPECT_ICON:
		/* Check the cache for the icon first */
		if ((hRes = m_pDataObject->GetData(petc, pmed)) == NOERROR) {
		  return NOERROR;
		}

		hGlobal = OleGetIconOfClass(m_ClsId, NULL, TRUE);
		break;
	}

	if (hGlobal != NULL) {
	  pmed->hGlobal = hGlobal;
	  pmed->tymed = TYMED_MFPICT;
	  hRes = NOERROR;

	}
  }

  return hRes;

} /* GetData()
   */


STDMETHODIMP CXObject::
  QueryGetData(LPFORMATETC petc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{

  if (petc == NULL) {
	return ResultFromScode(E_INVALIDARG);
  }

  if (petc->lindex != -1) {
	return ResultFromScode(DV_E_LINDEX);
  }

  if ((petc->cfFormat == CF_METAFILEPICT) && (petc->tymed & TYMED_MFPICT)) {
	if ((petc->dwAspect == DVASPECT_CONTENT) ||
		(petc->dwAspect == DVASPECT_ICON))
	{
	  return NOERROR;
	}

  /* this is a check for Wildcard formatetc only used for DAdvise
   */
  }else if ((petc->cfFormat == NULL) && (petc->ptd == NULL) &&
			(petc->dwAspect == -1L) && (petc->tymed == -1L))
  {
	return NOERROR;
  }

  return ResultFromScode(DV_E_FORMATETC);

} /* QueryGetData()
   */


STDMETHODIMP CXObject::
  GetCanonicalFormatEtc(LPFORMATETC petcIn, LPFORMATETC petcOut)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (petcOut == NULL) {
	return hRes;
  }

  petcOut->ptd = NULL;

  if (petcIn == NULL) {
	return hRes;
  }

  if ((hRes = QueryGetData(petcIn)) != NOERROR) {
	return hRes;
  }

  *petcOut = *petcIn ;
  petcOut->ptd = NULL ;

  return ((petcIn->ptd != NULL) ?
		   hRes = NOERROR : ResultFromScode(DATA_S_SAMEFORMATETC));

} /* GetCanonicalFormatEtc()
   */


STDMETHODIMP CXObject::
  Draw (DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
		DVTARGETDEVICE FAR * ptd, HDC hicTargetDev, HDC hdcDraw,
		LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
		BOOL (CALLBACK * pfnContinue) (DWORD), DWORD dwContinue)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);

  if (dwDrawAspect == DVASPECT_CONTENT) {
	hRes = FastDraw(hdcDraw, lprcBounds->left, lprcBounds->top,
					lprcBounds->right, lprcBounds->bottom);

  }else{
	if (m_pViewObject2 != NULL) {
	  hRes = m_pViewObject2->Draw(dwDrawAspect, lindex, pvAspect, ptd,
								  hicTargetDev, hdcDraw, lprcBounds,
								  lprcWBounds, pfnContinue, dwContinue);
	}
  }

  return hRes;

} /* Draw()
   */


STDMETHODIMP CXObject::
  FastDraw (HDC hDC,  long left, long top, long right, long bottom)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  char szMsg[32];
  HRGN hrgnClip;
  RECT rc;
  LOGFONT lf = m_lf;
  HFONT hOrgFont, hFont;
  HBRUSH hOrgBrush, hBrush;

  int iOldDC = SaveDC(hDC);


  if (GetDeviceCaps(hDC, TECHNOLOGY) != DT_METAFILE) {
	SetRect(&rc, (int)left, (int)top, (int)right, (int)bottom);

	LPtoDP(hDC, (LPPOINT)&rc, 2);
	SetMapMode(hDC, MM_ANISOTROPIC);

	SetViewportOrg(hDC, rc.left, rc.top);
	SetViewportExt(hDC, rc.right - rc.left, rc.bottom - rc.top);
	hBrush = CreateSolidBrush(RGB(64,128,128));
	SetTextColor(hDC, RGB(0,0,128));

	hrgnClip = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	SelectClipRgn(hDC, hrgnClip);

  }else{
	hBrush = CreateSolidBrush(RGB(192,192,192));
	SetTextColor(hDC, RGB(128,0,128));
  }

  SetRect(&rc, 0, 0, (int)m_Size.cx, (int)m_Size.cy);

  SetWindowOrg(hDC, rc.left, rc.top);
  SetWindowExt(hDC, rc.right - rc.left, rc.bottom - rc.top);

  SetBkColor(hDC, RGB(255,255,255));
  SetBkMode(hDC, TRANSPARENT);

  hOrgBrush = SelectObject(hDC, hBrush);
  SelectObject(hDC, GetStockObject(BLACK_PEN));

  lf.lfHeight = -MulDiv(-18, HIMETRIC_PER_INCH, giYppli);
  hFont = CreateFontIndirect(&lf);
  hOrgFont = SelectObject(hDC, hFont);

  FillRect(hDC, &rc, hBrush);
  Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);

  MoveTo(hDC, rc.left, rc.top);
  LineTo(hDC, rc.right, rc.bottom);

  SetTextAlign(hDC, TA_LEFT | TA_BOTTOM);

  if (m_bIsRunning == FALSE) {
	strcpy(szMsg, "Loaded");
  }else if (m_bIsLocked) {
	strcpy(szMsg, "Visible");
  }else{
	strcpy(szMsg, "Running/Invisible");
  }

  TextOut(hDC, rc.left, rc.bottom, szMsg, strlen(szMsg));

  SetTextAlign(hDC, TA_CENTER | TA_BASELINE);
  wsprintf(szMsg, "{%ld,%ld}", m_Size.cx, m_Size.cy);
  TextOut(hDC, (rc.right - rc.left) / 2, (rc.bottom - rc.top) / 2,
		  szMsg, strlen(szMsg));

  if (GetDeviceCaps(hDC, TECHNOLOGY) == DT_METAFILE) {
	SetTextAlign(hDC, TA_LEFT | TA_TOP);
	TextOut(hDC, rc.left, rc.top, "TYMED_MFPICT", strlen("TYMED_MFPICT"));
  }else{
	DeleteObject(hrgnClip);
  }

  SelectObject(hDC, hOrgBrush);
  SelectObject(hDC, hOrgFont);

  DeleteObject(hBrush);
  DeleteObject(hFont);

  RestoreDC(hDC, iOldDC);

  return NOERROR;

} /* FastDraw()
   */


STDMETHODIMP CXObject::
  DAdvise(LPFORMATETC petc, DWORD advf, LPADVISESINK pAdvSink,
		  LPDWORD pdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);

  if (m_pDataAdviseHolder == NULL) {
	return hRes;
  }

  if ((hRes = m_DataObject.QueryGetData(petc)) == NOERROR) {
	hRes = m_pDataAdviseHolder->Advise(&m_DataObject, petc, advf,
									   pAdvSink, pdwConnection);
  }

  return hRes;

} /* DAdvise()
   */


STDMETHODIMP CXObject::
  RegisterAsRunning()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = NOERROR;
  LPMONIKER pmkFull;

  if (m_pClientSite != NULL) {
	hRes = m_pClientSite->GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
									 OLEWHICHMK_OBJFULL, &pmkFull);
	if (hRes == NOERROR) {
	  hRes = OleStdRegisterAsRunning(&m_Unknown, pmkFull, &m_dwRegister);
	  m_pOleAdviseHolder->SendOnRename(pmkFull);
	}

	if (SUCCEEDED(hRes) && (pmkFull != NULL)) {
	  pmkFull->Release();
	}
  }

  return hRes;

} /* RegisterAsRunning()
   */


STDMETHODIMP CXObject::
  Cache(LPFORMATETC petc, DWORD advf, LPDWORD lpdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if(petc == NULL) {
	return ResultFromScode(E_INVALIDARG);
  }

  if (lpdwConnection != NULL) {
	*lpdwConnection = NULL;
  }

  if (petc->lindex != -1)   {
	return ResultFromScode(DV_E_LINDEX);
  }

  if ((petc->cfFormat == CF_METAFILEPICT) || (petc->cfFormat == NULL)) {

	if (((petc->dwAspect == DVASPECT_CONTENT) &&
		 (advf & ADVFCACHE_FORCEBUILTIN)) ||
		(petc->dwAspect == DVASPECT_ICON))
	  {
	  if (m_pOleCache2 != NULL) {
		return m_pOleCache2->Cache(petc, advf,lpdwConnection);
	  }
	}else{
	  return NOERROR;
	}
  }

  return ResultFromScode(DV_E_CLIPFORMAT);

} /* Cache()
   */


STDMETHODIMP CXObject::
  Uncache(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (dwConnection == 0) {
	return NOERROR;
  }

  if (m_pOleCache2 != NULL) {
	return m_pOleCache2->Uncache(dwConnection);
  }

  return ResultFromScode(OLE_E_NOCONNECTION);

} /* Uncache()
   */


STDMETHODIMP CXObject::
  EnumCache(LPENUMSTATDATA FAR* ppenumStatData)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  g_sdCacheList[0].advf = g_sdCacheList[0].dwConnection = 0;
  g_sdCacheList[1].advf = g_sdCacheList[1].dwConnection = 0;

  if (m_pOleCache2 != NULL) {
	LPENUMSTATDATA penum;
	STATDATA statdata;

	if (SUCCEEDED(m_pOleCache2->EnumCache(&penum)) && (penum != NULL)) {
	  while (penum->Next(1, &statdata, NULL) == NOERROR) {

		switch (statdata.formatetc.dwAspect) {
		  case DVASPECT_CONTENT:
			g_sdCacheList[0].advf = statdata.advf;
			g_sdCacheList[0].dwConnection = statdata.dwConnection;
			break;
		  case DVASPECT_ICON:
			g_sdCacheList[1].advf = statdata.advf;
			g_sdCacheList[1].dwConnection = statdata.dwConnection;
			break;
		  default:
			break;
		}

		OleStdFree(statdata.formatetc.ptd);
		if (statdata.pAdvSink != NULL) {
		  statdata.pAdvSink->Release();
		}
	  }
	  penum->Release();
	}
  }

  CEnumStatData *penum = new CEnumStatData(NUM_CACHES, g_sdCacheList,m_bDebug);
  if (penum == NULL) {
	return ResultFromScode(E_OUTOFMEMORY);
  }

  *ppenumStatData = (LPENUMSTATDATA)penum;
  return NOERROR;

} /* EnumCache()
   */


STDMETHODIMP CXObject::
  SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium, BOOL fRelease)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pOleCache2 != NULL) {

	if (pformatetc->dwAspect == DVASPECT_ICON) {
	  hRes = m_pOleCache2->SetData(pformatetc, pmedium, fRelease);

	}else{
	  hRes = QueryGetData(pformatetc);

	  if (SUCCEEDED(hRes) && fRelease) {
		ReleaseStgMedium(pmedium);
	  }
	}
  }

  return hRes;

} /* SetData()
   */
