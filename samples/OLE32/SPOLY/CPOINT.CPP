/*** 
*cpoint.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  This module implements the CPoint and CPointCF classes.
*
*  This module is intended as a sample implementation of the IDispatch
*  interface, and its purpose is to demonstrate how an object can
*  expose methods and properties for programatic and cross-process
*  access via the IDispatch interface.
*
*Implementation Notes:
*
*****************************************************************************/

#include "spoly.h"
#include "cpoint.h"


CPoint::CPoint()
{
    m_x = 0;
    m_y = 0;
    m_refs = 0;
}

/***
*CPoint::Create(void)
*Purpose:
*  Create an instance of a CPoint object.
*
*Entry:
*  None
*
*Exit:
*  returns a CPoint*, NULL if creation failed.
*
***********************************************************************/
CPoint FAR*
CPoint::Create()
{
    CPoint FAR* ppoint;

    if((ppoint = new FAR CPoint()) == NULL)
      return NULL;
    ppoint->AddRef();
    return ppoint;
}


//---------------------------------------------------------------------
//                     IUnknown Methods
//---------------------------------------------------------------------


STDMETHODIMP
CPoint::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(!IsEqualIID(riid, IID_IUnknown))
      if(!IsEqualIID(riid, IID_IDispatch)) {
        *ppv = NULL;		      
        return ResultFromScode(E_NOINTERFACE);
    }

    *ppv = this;
    AddRef();
    return NOERROR;
}


STDMETHODIMP_(unsigned long)
CPoint::AddRef(void)
{
    return ++m_refs;
}


STDMETHODIMP_(unsigned long)
CPoint::Release(void)
{
    if(--m_refs == 0){
      delete this;
      return 0;
    }
    return m_refs;
}


//---------------------------------------------------------------------
//                     IDispatch methods
//---------------------------------------------------------------------

/*
 * NOTE: Support for the following two methods is not available
 * in this version.
 *
 */

STDMETHODIMP
CPoint::GetTypeInfoCount(unsigned int FAR* pctinfo)
{
    *pctinfo = 0;
    return NOERROR;
}


STDMETHODIMP
CPoint::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    UNUSED(itinfo);
    UNUSED(lcid);
    UNUSED(pptinfo);

    return ResultFromScode(E_NOTIMPL);
}


/***
*HRESULT CPoint::GetIDsOfNames(REFIID, OLECHAR**, unsigned int, LCID, long*)
*Purpose:
*  This method translates the given array of names to a corresponding
*  array of DISPIDs.
*
*  Index 0 of the name array is the member name, and indices 1-N if
*  present represent named parameters on that member.
*
*  The local ID ('lcid') is unused by this naive implementation. A more
*  sophisticated implementation, sensitive to localization and natural
*  language support would use the locale ID to interpret the given names
*  in a correct locale specific context.
*
*Entry:
*  rgszNames = pointer to an array of names
*  cNames = the number of names in the rgszNames array
*  lcid = the callers locale ID
*
*Exit:
*  return value = HRESULT
*  rgid = array of name IDs corresponding to the rgszNames array
*    this array will contain -1 for each entry that is not known.
*
***********************************************************************/
STDMETHODIMP
CPoint::GetIDsOfNames(
    REFIID riid,
    OLECHAR FAR* FAR* rgszNames,
    unsigned int cNames,
    LCID lcid,
    DISPID FAR* rgdispid)
{
static MEMBERDESC rgmdCPoint[] = {
    {OLESTR("GETX"),	IDMEMBER_CPOINT_GETX,	NULL,	0},
    {OLESTR("SETX"),	IDMEMBER_CPOINT_SETX,	NULL,	0},
    {OLESTR("GETY"),	IDMEMBER_CPOINT_GETY,	NULL,	0},
    {OLESTR("SETY"),	IDMEMBER_CPOINT_SETY,	NULL,	0}
};

    // this object only exposed the "default" interface.
    //
    if(!IsEqualIID(riid, IID_NULL))
      return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

    return SPolyGetIDsOfNames(
      rgmdCPoint, DIM(rgmdCPoint), rgszNames, cNames, lcid, rgdispid);
}


/***
*HRESULT CPoint::Invoke(...)
*Purpose:
*  Dispatch a method or property request for objects of type CPoint.
*
*  see the IDispatch document for more information, and a general
*  description of this method.
*
*Entry:
*  dispidMember = the DISPID of the member being requested
*
*  riid = reference to the interface ID of the interface on this object
*    that the requested member belongs to. IID_NULL means to interpret
*    the member as belonging to the implementation defined "default"
*    or "primary" interface.
*
*  lcid = the caller's locale ID
*
*  wFlags = flags indicating the type of access being requested
*
*  pdispparams = pointer to the DISPPARAMS struct containing the
*    requested members arguments (if any) and its named parameter
*    DISPIDs (if any).
*
*Exit:
*  return value = HRESULT
*   see the IDispatch spec for a description of possible success codes.
*
*  pvarResult = pointer to a caller allocated VARIANT containing
*    the members return value (if any).
*
*  pexcepinfo = caller allocated exception info structure, this will
*    be filled in only if an exception was raised that must be passed
*    up through Invoke to an enclosing handler.
*
*  puArgErr = pointer to a caller allocated UINT, that will contain the
*    index of the offending argument if a DISP_E_TYPEMISMATCH error
*    was returned indicating that one of the arguments was of an
*    incorrect type and/or could not be reasonably coerced to a proper
*    type.
*
***********************************************************************/
STDMETHODIMP
CPoint::Invoke(
    DISPID dispidMember,
    REFIID riid,
    LCID lcid,
    unsigned short wFlags,
    DISPPARAMS FAR* pdispparams,
    VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo,
    unsigned int FAR* puArgErr)
{
    unsigned int uArgErr;
    HRESULT hresult;
    VARIANTARG varg0;
    VARIANT varResultDummy;

    UNUSED(lcid);
    UNUSED(pexcepinfo);

    // make sure the wFlags are legal
    if(wFlags & ~(DISPATCH_METHOD | DISPATCH_PROPERTYGET | DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
      return ResultFromScode(E_INVALIDARG);

    // this object only exposes a "default" interface.
    //
    if(!IsEqualIID(riid, IID_NULL))
      return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

    // this makes the following code a bit simpler
    if(puArgErr == NULL)
      puArgErr = &uArgErr;
    if(pvarResult == NULL)
      pvarResult = &varResultDummy;

    VariantInit(&varg0);

    // assume the return type is void, unless we find otherwise.
    VariantInit(pvarResult);

    switch(dispidMember){
    case IDMEMBER_CPOINT_GETX:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = GetX();
      break;

    case IDMEMBER_CPOINT_SETX:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      SetX(V_I2(&varg0));
      break;

    case IDMEMBER_CPOINT_GETY:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = GetY();
      break;

    case IDMEMBER_CPOINT_SETY:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      SetY(V_I2(&varg0));
      break;

    default:
      return ResultFromScode(DISP_E_MEMBERNOTFOUND);
    }
    return NOERROR;
}

short PASCAL
CPoint::GetX()
{
    return m_x;
}

void PASCAL
CPoint::SetX(short x)
{
    m_x = x;
}

short PASCAL
CPoint::GetY()
{
    return m_y;
}

void PASCAL
CPoint::SetY(short y)
{
    m_y = y;
}


//---------------------------------------------------------------------
//         Implementation of the CPoint Class Factory
//---------------------------------------------------------------------

CPointCF::CPointCF()
{
    m_refs = 0;
}

IClassFactory FAR*
CPointCF::Create()
{
    CPointCF FAR* pCF;

    if((pCF = new FAR CPointCF()) == NULL)
      return NULL;
    pCF->AddRef();
    return pCF;
}

STDMETHODIMP
CPointCF::QueryInterface(REFIID riid, void FAR* FAR* ppv) 
{ 

    if(!IsEqualIID(riid, IID_IUnknown))
      if(!IsEqualIID(riid, IID_IClassFactory)) {
        *ppv = NULL;		      
        return ResultFromScode(E_NOINTERFACE);
    }
	
    *ppv = this;
    ++m_refs;
    return NOERROR;
}

STDMETHODIMP_(unsigned long)
CPointCF::AddRef(void)
{
    return ++m_refs;
}

STDMETHODIMP_(unsigned long)
CPointCF::Release(void)
{
    if(--m_refs == 0){
      delete this;
      return 0;
    }
    return m_refs;
}

STDMETHODIMP
CPointCF::CreateInstance(
    IUnknown FAR* punkOuter,
    REFIID riid,
    void FAR* FAR* ppv)
{
    HRESULT hresult;
    CPoint FAR *ppoint;

    UNUSED(punkOuter);

    if((ppoint = CPoint::Create()) == NULL){
      *ppv = NULL;
      return ResultFromScode(E_OUTOFMEMORY);
    }
    hresult = ppoint->QueryInterface(riid, ppv);
    ppoint->Release();
    return hresult;
}

STDMETHODIMP
#ifdef _MAC
CPointCF::LockServer(unsigned long fLock)
#else
CPointCF::LockServer(BOOL fLock)
#endif
{
    UNUSED(fLock);

    return NOERROR;
}

