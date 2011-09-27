//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: dispatch.cpp
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"

//---------------------------------------------------------------------
// CXObject::CDispatch type data definitions
//---------------------------------------------------------------------

static PARAMDATA NEAR rgpdataFastDraw[] =
{
	{ "hdc",    VT_I2 },
	{ "left",   VT_I4 },
	{ "top",    VT_I4 },
	{ "right",  VT_I4 },
	{ "bottom", VT_I4 }
};

static PARAMDATA NEAR rgpdataSetExtent[] =
{
	{ "cx", VT_I4 },
	{ "cy", VT_I4 }
};

static PARAMDATA NEAR rgpdataGetExtent[] =
{
	{ "pcx",    VT_I4 | VT_BYREF },
	{ "pcy",    VT_I4 | VT_BYREF }
};

static PARAMDATA NEAR rgpdataShowObject[] =
{
	{ "fShow",  VT_I2}
};

static METHODDATA NEAR rgmdataCXObject[] =
{
  /* HRESULT CXObject::FastDraw(HDC hDC, long, long, long, long ) */
  {
	"FastDraw",
	rgpdataFastDraw,
	IDMEMBER_FASTDRAW,
	IMETH_CXOBJECT_CDISPATCH_FASTDRAW,
	CC_CDECL,
	  DIM(rgpdataFastDraw),
	DISPATCH_METHOD,
	VT_ERROR
  },


  /* HRESULT CXObject::SetExtent(long cx, long cy) */
  {
	"SetExtent",
	  rgpdataSetExtent,
	IDMEMBER_SETEXTENT,
	IMETH_CXOBJECT_CDISPATCH_SETEXTENT,
	CC_CDECL,
	  DIM(rgpdataSetExtent),
	  DISPATCH_METHOD,
	  VT_ERROR
  },

  /* HRESULT CXObject::GetExtent(long FAR *pcx, long FAR *pcy) */
  {
	"GetExtent",
	  rgpdataGetExtent,
	IDMEMBER_GETEXTENT,
	IMETH_CXOBJECT_CDISPATCH_GETEXTENT,
	CC_CDECL,
	  DIM(rgpdataGetExtent),
	  DISPATCH_METHOD,
	  VT_ERROR
  },

  /* HRESULT CXObject::ShowObject(BOOL fShow) */
  {
	"ShowObject",
	  rgpdataShowObject,
	IDMEMBER_SHOWOBJECT,
	IMETH_CXOBJECT_CDISPATCH_SHOWOBJECT,
	CC_CDECL,
	  DIM(rgpdataShowObject),
	  DISPATCH_METHOD,
	  VT_ERROR
  }
};

INTERFACEDATA NEAR g_idataCXObject =
{
  rgmdataCXObject, DIM(rgmdataCXObject)
};


//---------------------------------------------------------------------
// CXObject::CDispatch methods
//---------------------------------------------------------------------


NC(CXObject, CDispatch)::
  CDispatch(CXObject *pObject)
//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------
{
  m_pObject = pObject;

  HRESULT hRes = CreateDispTypeInfo(&g_idataCXObject, LOCALE_SYSTEM_DEFAULT,
									&m_ptinfo);
} /* CDispatch()
   */


NC(CXObject, CDispatch)::
  ~CDispatch()
//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------
{
  if (m_ptinfo != NULL) {
	m_ptinfo->Release();
	m_ptinfo = NULL;
  }

} /* ~CDispatch()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CDispatch)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CDispatch)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  GetTypeInfoCount(unsigned int FAR* pctinfo)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::GetTypeInfoCount(" << (LPUNKNOWN)pctinfo << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = NOERROR;
  *pctinfo = 1;

  db << "IDispatch::GetTypeInfoCount(" << (WORD)*pctinfo
	 << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* GetTypeInfoCount()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::GetTypeInfo(" << (WORD)itinfo << lcid << (LPUNKNOWN)pptinfo
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(DISP_E_BADINDEX);

  if (itinfo == 0) {
	m_ptinfo->AddRef();
	*pptinfo = m_ptinfo;
	hRes = NOERROR;
  }

  db << "IDispatch::GetTypeInfo(" << (WORD)itinfo << lcid
	 << (LPUNKNOWN)*pptinfo << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* GetTypeInfo()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  GetIDsOfNames(REFIID riid, TCHAR FAR* FAR* rgszNames, unsigned int cNames,
				LCID lcid, DISPID FAR* rgdispid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::GetIDsOfNames(" << riid << (LPUNKNOWN)rgszNames
	 << (WORD)cNames << lcid << (LPUNKNOWN) rgdispid << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(DISP_E_UNKNOWNINTERFACE);

  if(riid == IID_NULL) {
	hRes = DispGetIDsOfNames(m_ptinfo, rgszNames, cNames, rgdispid);
  }

  db << "IDispatch::GetIDsOfNames() Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* GetIDsOfNames()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  Invoke(DISPID dispidMember, REFIID riid, LCID lcid, unsigned short wFlags,
		 DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
		 EXCEPINFO FAR* pexcepinfo, unsigned int FAR* puArgErr)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::Invoke(" << dispidMember << riid << lcid
	 << wFlags << (LPUNKNOWN)pdispparams << (LPUNKNOWN)pvarResult
	 << (LPUNKNOWN)pvarResult << (LPUNKNOWN)pexcepinfo
	 << (LPUNKNOWN)puArgErr << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(DISP_E_UNKNOWNINTERFACE);

  if (riid == IID_NULL) {
	hRes = DispInvoke(this, m_ptinfo, dispidMember, wFlags, pdispparams,
					  pvarResult, pexcepinfo, puArgErr);
  }

  db << "IDispatch::Invoke(" << dispidMember << riid << lcid
	 << wFlags << (LPUNKNOWN)pdispparams << (LPUNKNOWN)pvarResult
	 << (LPUNKNOWN)pvarResult << (LPUNKNOWN)pexcepinfo
	 << (WORD)*puArgErr << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* Invoke()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  FastDraw (HDC hDC, long left, long top, long right, long bottom)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::FastDraw(" << (WORD)hDC << left << top << right
	 << bottom << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->FastDraw(hDC, left, top, right, bottom);

  db << "IDispatch::FastDraw() Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* FastDraw()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  SetExtent (long cx, long cy)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::SetExtent(" << cx << cy << ") Called" ;
  db.DebugOutput();

  SIZEL sizel;

  sizel.cx = cx;
  sizel.cy = cy;

  HRESULT hRes = m_pObject->SetExtent(&sizel);

  db << "IDispatch::SetExtent() Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* SetExtent()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  GetExtent (long FAR *pcx, long FAR *pcy)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::GetExtent(" << (LPUNKNOWN)pcx << (LPUNKNOWN)pcy
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);
  SIZEL sizel;

  if (pcx != NULL && pcy != NULL) {
	hRes = m_pObject->GetExtent(DVASPECT_CONTENT, -1, NULL, &sizel);

	*pcx = sizel.cx;
	*pcy = sizel.cy;
  }

  db << "IDispatch::GetExtent(" << *pcx << *pcy << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* GetExtent()
   */


STDMETHODIMP NC(CXObject, CDispatch)::
  ShowObject(BOOL fShow)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDispatch::ShowObject(" << fShow << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (m_pObject->m_pClientSite != NULL) {
	m_pObject->m_pClientSite->OnShowWindow(fShow);

	if (fShow) {
	  m_pObject->m_pClientSite->ShowObject();
	}
  }

  db << "IDispatch::ShowObject() Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* ShowObject()
   */
