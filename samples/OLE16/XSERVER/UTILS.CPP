//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module: utils.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"

/* For Bounds Stuff */
int giXppli;    /* pixels per logical inch along width */
int giYppli;    /* pixels per logical inch along height */


STDAPI
  OpenOrCreateIStream(LPCSTR pname, LPSTORAGE prootstg, LPSTREAM FAR* ppstm)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;
  DWORD grfMode = STGM_SHARE_EXCLUSIVE | STGM_READWRITE ;

  if ((ppstm == NULL) || (prootstg == NULL) || (ppstm == NULL)) {
	return ResultFromScode(E_INVALIDARG);
  }

  *ppstm = NULL;

  hRes = prootstg->OpenStream(pname, NULL, grfMode, 0, ppstm);
  if (hRes != NOERROR) {
	hRes = prootstg->CreateStream(pname, grfMode, 0, 0, ppstm);
  }

  return hRes ;

} /* OpenOrCreateIStream()
   */


STDAPI_(LPVOID)
  OleStdAlloc(ULONG cb)
//----------------------------------------------------------------------------
// OleStdAlloc:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  LPVOID pMem;
  ULONG dwRefs;
  HRESULT hRes;

  hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
  if (hRes != NOERROR) {
	return NULL;
  }

  pMem = pMalloc->Alloc(cb);
  dwRefs = pMalloc->Release();

  return pMem;
} /* OleStdAlloc()
   */


STDAPI_(LPVOID)
  OleStdRealloc(void FAR* pv, ULONG cb)
//----------------------------------------------------------------------------
// OleStdRealloc:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  LPVOID pMem;
  ULONG dwRefs;
  HRESULT hRes;

  hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
  if (hRes != NOERROR) {
	return NULL;
  }

  pMem = pMalloc->Realloc(pv, cb);
  dwRefs = pMalloc->Release();

  return pMem;

} /* OleStdRealloc()
   */


STDAPI_(VOID)
  OleStdFree(void FAR* pv)
//----------------------------------------------------------------------------
// OleStdFree:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  ULONG dwRefs;
  HRESULT hRes;

  if (pv != NULL) {
	hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
	if (hRes == NOERROR) {
	  pMalloc->Free(pv);
	  dwRefs = pMalloc->Release();
	}
  }

} /* OleStdFree()
   */


STDAPI_(VOID)
  HiMetricToDevice(LPSIZEL psizel)
//----------------------------------------------------------------------------
// HiMetricToDevice:
//----------------------------------------------------------------------------
{
  psizel->cx  = (long)MulDiv(giXppli, (int)psizel->cx, HIMETRIC_PER_INCH);
  psizel->cy  = (long)MulDiv(giYppli, (int)psizel->cy, HIMETRIC_PER_INCH);

} /* HiMetricToDevice()
   */


STDAPI_(VOID)
  DeviceToHiMetric(LPSIZEL psizel)
//----------------------------------------------------------------------------
// DeviceToHiMetric:
//----------------------------------------------------------------------------
{
  psizel->cx = (long)MulDiv((int)psizel->cx, HIMETRIC_PER_INCH, giXppli);
  psizel->cy = (long)MulDiv((int)psizel->cy, HIMETRIC_PER_INCH, giYppli);

  if (psizel->cx < 0) psizel->cx *= -1;
  if (psizel->cy < 0) psizel->cy *= -1;

} /* DeviceToHiMetric()
   */


STDAPI_(DVTARGETDEVICE FAR*)
  OleStdCopyTargetDevice(DVTARGETDEVICE FAR* ptdSrc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  DVTARGETDEVICE FAR* ptdDest = NULL;

  if (ptdSrc == NULL) {
	return NULL;
  }

  if ((ptdDest = (DVTARGETDEVICE FAR*)OleStdAlloc(ptdSrc->tdSize)) != NULL) {
	_fmemcpy(ptdDest, ptdSrc, (size_t)ptdSrc->tdSize);
  }

  return ptdDest;

} /* OleStdCopyTargetDevice()
   */


STDAPI_(BOOL)
  OleStdCopyFormatEtc(LPFORMATETC petcDest, LPFORMATETC petcSrc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if ((petcDest == NULL) || (petcSrc == NULL)) {
	return FALSE;
  }

  petcDest->cfFormat = petcSrc->cfFormat;
  petcDest->ptd      = OleStdCopyTargetDevice(petcSrc->ptd);
  petcDest->dwAspect = petcSrc->dwAspect;
  petcDest->lindex   = petcSrc->lindex;
  petcDest->tymed    = petcSrc->tymed;

  return TRUE;

} /* OleStdCopyFormatEtc()
   */


STDAPI_(BOOL)
  OleStdCopyStatData(LPSTATDATA pDest, LPSTATDATA pSrc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if ((pDest == NULL) || (pSrc == NULL)) {
	return FALSE;
  }

  if (OleStdCopyFormatEtc(&pDest->formatetc, &pSrc->formatetc) == FALSE) {
	return FALSE;
  }

  pDest->advf = pSrc->advf;
  pDest->pAdvSink = pSrc->pAdvSink;
  pDest->dwConnection = pSrc->dwConnection;

  if (pDest->pAdvSink != NULL) {
	pDest->pAdvSink->AddRef();
  }

  return TRUE;

} /* OleStdCopyStatData()
   */


STDAPI_(HGLOBAL)
  OleStdGetMetafile(LPVIEWOBJECT2 pViewObject, DWORD dwDrawAspect,
				 DVTARGETDEVICE FAR* ptd)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HDC hMetaDC;
  HMETAFILE hMF=NULL;
  HRESULT hRes;
  SIZEL sizel;
  RECTL rectl;
  HANDLE hPict;
  LPMETAFILEPICT lpPict;

  if (pViewObject == NULL) {
	return NULL;
  }

  hRes = pViewObject->GetExtent(dwDrawAspect, -1, ptd, &sizel);
  if (hRes != NOERROR) {
	goto errReturn;
  }

  hMetaDC = CreateMetaFile(NULL);

  SetWindowOrg(hMetaDC, 0, 0);
  SetWindowExt(hMetaDC, (int)sizel.cx, (int)sizel.cy);

  rectl.left = rectl.top = 0;
  rectl.right = sizel.cx ;
  rectl.bottom = sizel.cy ;

  hRes = pViewObject->Draw(dwDrawAspect, -1, NULL, ptd, NULL,
						   hMetaDC, &rectl, &rectl, NULL, 0);

  hMF = CloseMetaFile(hMetaDC);
  if (hRes != NOERROR) {
	goto errReturn;
  }

  hPict = GlobalAlloc(GHND|GMEM_SHARE, sizeof(METAFILEPICT));
  if (hPict && (lpPict = (LPMETAFILEPICT)GlobalLock(hPict))){
	lpPict->hMF  = hMF;
	lpPict->xExt = (int)sizel.cx ;
	lpPict->yExt = (int)sizel.cy ;
	lpPict->mm   = MM_ANISOTROPIC;
	GlobalUnlock(hPict);
  }

  return hPict;

errReturn:
  if (hMF != NULL) {
	DeleteMetaFile(hMF);
  }
  return NULL;

} /* OleStdGetMetafile()
   */


STDAPI
  OleStdRegisterAsRunning(LPUNKNOWN lpUnk, LPMONIKER lpmkFull,
					   DWORD FAR* lpdwRegister)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  LPRUNNINGOBJECTTABLE lpROT;
  HRESULT hRes;
  DWORD dwOldRegister = *lpdwRegister;

  hRes = GetRunningObjectTable(0,(LPRUNNINGOBJECTTABLE FAR*)&lpROT);
  if (hRes == NOERROR) {

	if (lpmkFull) {
	  hRes = lpROT->Register(0, lpUnk, lpmkFull, lpdwRegister);
	}

	if (dwOldRegister != 0) {
	  hRes = lpROT->Revoke(dwOldRegister);
	}

	lpROT->Release();
  }

  return hRes;

} /* OleStdRegisterAsRunning()
   */


STDAPI
  OleStdRevokeAsRunning(DWORD FAR* lpdwRegister)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  LPRUNNINGOBJECTTABLE lpROT;
  HRESULT hRes;

  if (*lpdwRegister != 0) {
	hRes = GetRunningObjectTable(0,(LPRUNNINGOBJECTTABLE FAR*)&lpROT);
	if (hRes == NOERROR) {

	  hRes = lpROT->Revoke(*lpdwRegister);
	  *lpdwRegister = 0;

	  lpROT->Release();
	}
  }

  return hRes;

} /* OleStdRevokeAsRunning()
   */
