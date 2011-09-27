/*** 
*crempoly.cpp
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
*  This file contains the implementation of CRemPoly, the remote polygon
*  class. This class presents a standard C++ vtable interface to the
*  rest of the application, and hides the details of talking to the
*  actual remote CPoly class exposed by the SPoly server. Each of
*  the introduced methods is simply a cover for an IDispatch invocation
*  of the actual method on the remote object.
*
*Implementation Notes:
*
*****************************************************************************/

#include "dispdemo.h"
#include "crempoly.h"

extern int g_fTrace;

// method names on the CPoly class.
//
OLECHAR FAR* CRemPoly::m_rgszMethods[] = {
    OLESTR("draw"),
    OLESTR("dump"),
    OLESTR("reset"),
    OLESTR("addpoint"),
    OLESTR("enumpoints"),
    OLESTR("getxorigin"),
    OLESTR("setxorigin"),
    OLESTR("getyorigin"),
    OLESTR("setyorigin"),
    OLESTR("getwidth"),
    OLESTR("setwidth"),
    OLESTR("get_red"),
    OLESTR("set_red"),
    OLESTR("get_green"),
    OLESTR("set_green"),
    OLESTR("get_blue"),
    OLESTR("set_blue")
};

#ifdef _MAC
# define IfMac(X) (X)
# define IfWin(X)
#else
# define IfMac(X)
# define IfWin(X) (X)
#endif


CRemPoly::CRemPoly()
{
    m_refs = 0;
    m_pdisp = (IDispatch FAR*)NULL;
}


// A useful pre-initialized DISPATCHPARAMS, used on all the methods that
// take 0 arguments.
//
DISPPARAMS NEAR g_dispparamsNoArgs = {NULL, NULL, 0, 0};


/***
*HRESULT CRemPoly::Create(clsid, CRemPoly**)
*
*Purpose:
*  This function creates an instance of the CRemPoly class, connects
*  it to the IDispatch interface of the remote CPoly class, and learns
*  the DISPIDs for the members (that we know about) exposed by that
*  class.
*
*Entry:
*  clsid = The CLSID of the CPoly we are to create. (taking this as a
*    param is a bit weird, but allows us to connect to several remote
*    versions.
*
*Exit:
*  return value = HRESULT
*
*  *pprempoly = pointer to the newly created CRemPoly, if successfyl.
*
***********************************************************************/
HRESULT
CRemPoly::Create(CLSID clsid, CRemPoly FAR* FAR* pprempoly)
{
    int i;
    HRESULT hresult;
    IUnknown FAR* punk;
    CRemPoly FAR* prempoly;


    prempoly = new FAR CRemPoly();
    if(prempoly == (CRemPoly FAR*)NULL){
      hresult = ResultFromScode(E_OUTOFMEMORY);
      goto LError;
    }
    prempoly->AddRef();
      
    // create an instance of the remote CPoly class.
    //
    IfMac(DbPrintf("CoCreateInstance(CLSID_CPoly)"));
    hresult = CoCreateInstance(
      clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (void FAR* FAR*)&punk);
    if(hresult != NOERROR){
      IfMac(DbPrintf("CoCreateInstance() = 0x%x", hresult));
      IfWin(MessageBox(NULL, TSTR("Unable to create polygon object"), 
	               NULL, MB_OK));
      goto LFreeCRemPoly;
    }

    // were going to talk to this remote instance via IDispatch.
    //
    IfMac(DbPrintf("QueryInterface(IID_IDispatch)"));
    hresult = punk->QueryInterface(
      IID_IDispatch, (void FAR* FAR*)&prempoly->m_pdisp);
    if(hresult != NOERROR){
      IfMac(DbPrintf("QueryInterface(IID_IDispatch) = 0x%x", hresult));
      IfWin(MessageBox(NULL, TSTR("Unable to QueryInterface to IDispatch"),
	               NULL, MB_OK));
      goto LReleaseUnk;
    }

    // We learn *all* the member IDs up front. A more sophisticated
    // implementation might defer learning about the IDs for a given
    // method until the first time the method is invoked, thereby
    // amortizing the creation costs.
    //
    IfMac(DbPrintf("GetIDsOfNames()"));
    for(i = 0; i < IMETH_CREMPOLY_MAX; ++i){
      hresult = prempoly->m_pdisp->GetIDsOfNames(
	IID_NULL,
        &prempoly->m_rgszMethods[i],
	1, LOCALE_USER_DEFAULT,
	&prempoly->m_rgdispid[i]);
      if(hresult != NOERROR){
	IfMac(DbPrintf("GetIDsOfNames() = 0x%x", hresult));
	IfWin(MessageBox(NULL, TSTR("Unrecognized member name"),
		         NULL, MB_OK));
	goto LReleaseUnk;
      }
    }

    punk->Release();

    *pprempoly = prempoly;

    IfMac(DbPrintf("Object created."));

    return NOERROR;

LReleaseUnk:;
    punk->Release();

LFreeCRemPoly:;
    prempoly->Release();

LError:;
    return hresult;
}


//---------------------------------------------------------------------
//                     IUnknown methods
//---------------------------------------------------------------------


/***
*HRESULT CRemPoly::QueryInterface(REFIID, void**)
*
*Purpose:
*  Standard Ole2 implementation of QueryInterface. This class
*  supports the IUnknown interface, and introduces a number of
*  nonvirtual members.
*
*Entry:
*  riid = reference to the requested interface id
*
*Exit:
*  return value = HRESULT
*  *ppv = pointer to the requested interface, if successful.
*
***********************************************************************/
STDMETHODIMP
CRemPoly::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(IsEqualIID(riid, IID_IUnknown)){
      *ppv = this;
      AddRef();
      return NOERROR;
    }
    *ppv = NULL;
    return ResultFromScode(E_NOINTERFACE);
}


/***
*unsigned long CRemPoly::AddRef(void)
*
*Purpose:
*  Add a reference to the instance.
*
*Entry:
*  None
*
*Exit:
*  return value = unsigned long. The resulting reference count.
*
***********************************************************************/
STDMETHODIMP_(unsigned long)
CRemPoly::AddRef(void)
{
    return ++m_refs;
}


/***
*unsigned long CRemPoly::Release(void)
*
*Purpose:
*  Release a reference to the instance. If the reference count goes
*  to zero, delete the instance.
*
*Entry:
*  None
*
*Exit:
*  return value = unsigned long. The resulting reference count.
*
***********************************************************************/
STDMETHODIMP_(unsigned long)
CRemPoly::Release(void)
{
    if(--m_refs == 0){
      if(m_pdisp != (IDispatch FAR*)NULL){
        m_pdisp->Release();
      }
      delete this;
      return 0;
    }
    return m_refs;
}


//---------------------------------------------------------------------
//                    Introduced methods
//---------------------------------------------------------------------


/*
 * Each of these methods is simply a cover for an IDispatch Invocation
 * of the actual method on the remote CPoly class. This allows CRemPoly
 * to present an interface that looks and acts just like the CPoly
 * object, even though the actual work is being done in another process.
 *
 */

/***
*HRESULT CRemPoly::Draw(void)
*
*Purpose:
*  Invoke the Draw method on the remote CPoly instance.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::Draw()
{
    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_DRAW],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, NULL, NULL, NULL);
}


/***
*HRESULT CRemPoly::Dump(void)
*
*Purpose:
*  Invoke the Dump() method on the remote CPoly instance. This method
*  dumps the contained CPoints and writes the properties of the remote
*  CPoly instance to the debug window.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::Dump()
{
    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_DUMP],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, NULL, NULL, NULL);
}


/***
*HRESULT CRemPoly::Reset(void)
*
*Purpose:
*  Invoke the Reset() method on the remote CPoly instance. The Reset()
*  method causes the remote CPoly to release all contained CPoints.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::Reset()
{
    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_RESET],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, NULL, NULL, NULL);
}


/***
*HRESULT CRemPoly::AddPoint(short, short)
*
*Purpose:
*  Invoke the AddPoint method in the remote CPoly object to add a
*  new point with the given coordinates to this instance.
*
*Entry:
*  x,y = the x and y coordinates of the new point.
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::AddPoint(short x, short y)
{
    HRESULT hresult;
    VARIANTARG varg[2];
    DISPPARAMS dispparams;

    V_VT(&varg[0]) = VT_I2;
    V_I2(&varg[0]) = y;

    V_VT(&varg[1]) = VT_I2;
    V_I2(&varg[1]) = x;

    dispparams.cArgs = 2;
    dispparams.rgvarg = varg;
    dispparams.cNamedArgs = 0;
    dispparams.rgdispidNamedArgs = NULL;

    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_ADDPOINT],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, NULL, NULL, NULL);

    return hresult;
}


/***
*HRESULT CRemPoly::EnumPoints(IEnumVARIANT**)
*Purpose:
*  Inoke the EnumPoints() method in the remote object to
*  get a enumerator for the points contained in the current poly.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
*  *ppenum = pointer to the point enumerator
*
***********************************************************************/
HRESULT
CRemPoly::EnumPoints(IEnumVARIANT FAR* FAR* ppenum)
{
    HRESULT hresult;
    IEnumVARIANT FAR* penum;
    VARIANT varResult, FAR* pvarResult;


    pvarResult = &varResult;
    VariantInit(pvarResult);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_ENUMPOINTS],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, pvarResult, NULL, NULL);

    if(hresult != NOERROR)
      return hresult;

    if(V_VT(pvarResult) != VT_UNKNOWN)
      return ResultFromScode(E_FAIL);

    hresult = V_UNKNOWN(pvarResult)->QueryInterface(
      IID_IEnumVARIANT, (void FAR* FAR*)&penum);

    if(hresult == NOERROR)
      *ppenum = penum;

    VariantClear(pvarResult);

    return NOERROR;
}


/***
*HRESULT CRemPoly::GetXOrigin(short*)
*
*Purpose:
*  Invoke the GetXOrigin() method on the remote object to extract
*  the current value of the XOrigin property.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
*  *pxorg = the current X origin of the polygon.
*
***********************************************************************/
HRESULT
CRemPoly::GetXOrigin(short FAR* pxorg)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_GETXORIGIN],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return hresult;

    *pxorg = V_I2(&varResult);
    VariantClear(&varResult);
    
    return NOERROR;
}


/***
*HRESULT CRemPoly::SetXOrigin(short)
*
*Purpose:
*  Invoke the SetXOrigin method on the remote object to set the
*  XOrigin property of the polygon to the given value.
*
*Entry:
*  xorg = the new X origin
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::SetXOrigin(short xorg)
{
    VARIANTARG varg;
    DISPPARAMS dispparams;


    V_VT(&varg) = VT_I2;
    V_I2(&varg) = xorg;

    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;

    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_SETXORIGIN],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, NULL, NULL, NULL);
}


/***
*HRESULT CRemPoly::GetYOrigin(short*)
*
*Purpose:
*  Invoke the GetYOrigin() method on the remote object to extract
*  the current value of the YOrigin property.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
*  *pyorg = the current Y origin of the polygon
*
***********************************************************************/
HRESULT
CRemPoly::GetYOrigin(short FAR* pyorg)
{
    HRESULT hresult;
    VARIANT varResult;


    VariantInit(&varResult);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_GETYORIGIN],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return hresult;

    *pyorg = V_I2(&varResult);
    VariantClear(&varResult);
    
    return hresult;
}


/***
*HRESULT CRemPoly::SetYOrigin(short)
*
*Purpose:
*  Invoke the SetYOrigin method on the remote object to set the
*  YOrigin property of the polygon to the given value.
*
*Entry:
*  yorg = the new Y origin
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::SetYOrigin(short yorg)
{
    VARIANTARG varg;
    DISPPARAMS dispparams;

    V_VT(&varg) = VT_I2;
    V_I2(&varg) = yorg;

    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;

    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_SETYORIGIN],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, NULL, NULL, NULL);
}


/***
*HRESULT CRemPoly::GetWidth(short*)
*
*Purpose:
*  Invoke the GetWidth() method on the remote object to extract
*  the current value of the line width property.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
*  *pwidth = short, the current line width of the polygon
*
***********************************************************************/
HRESULT
CRemPoly::GetWidth(short FAR* pwidth)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_GETWIDTH],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return hresult;

    *pwidth = V_I2(&varResult);
    VariantClear(&varResult);
    return NOERROR;
}


/***
*HRESULT CRemPoly::SetWidth(short)
*
*Purpose:
*  Invoke the SetWidth method on the remote object to set the
*  line width property of the polygon to the given value.
*
*Entry:
*  width = the new value for the line width property.
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT
CRemPoly::SetWidth(short width)
{
    VARIANTARG varg;
    DISPPARAMS dispparams;


    V_VT(&varg) = VT_I2;
    V_I2(&varg) = width;

    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;

    return m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMPOLY_SETWIDTH],
      IID_NULL,
      LOCALE_USER_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, NULL, NULL, NULL);
}


HRESULT CRemPoly::get_red(short FAR* psRed)
{
    return get_i2(m_rgdispid[IMETH_CREMPOLY_GETRED], psRed);
}

HRESULT CRemPoly::set_red(short sRed)
{
    return set_i2(m_rgdispid[IMETH_CREMPOLY_SETRED], sRed);
}


HRESULT CRemPoly::get_green(short FAR* psGreen)
{
    return get_i2(m_rgdispid[IMETH_CREMPOLY_GETGREEN], psGreen);
}

HRESULT CRemPoly::set_green(short sGreen)
{
    return set_i2(m_rgdispid[IMETH_CREMPOLY_SETGREEN], sGreen);
}


HRESULT CRemPoly::get_blue(short FAR* psBlue)
{
    return get_i2(m_rgdispid[IMETH_CREMPOLY_GETBLUE], psBlue);
}

HRESULT CRemPoly::set_blue(short sBlue)
{
    return set_i2(m_rgdispid[IMETH_CREMPOLY_SETBLUE], sBlue);
}

HRESULT
CRemPoly::get_i2(DISPID dispid, short FAR* ps)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);

    hresult = m_pdisp->Invoke(
      dispid,
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs,
      &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return hresult;

    hresult = VariantChangeType(&varResult, &varResult, 0, VT_I2);
    if(hresult != NOERROR){
      VariantClear(&varResult);
      return hresult;
    }

    *ps = V_I2(&varResult);
    VariantClear(&varResult);
    return NOERROR;
}

HRESULT
CRemPoly::set_i2(DISPID dispid, short s)
{
    VARIANTARG varg;
    DISPPARAMS dispparams;

    V_VT(&varg) = VT_I2;
    V_I2(&varg) = s;

    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;

    return m_pdisp->Invoke(
      dispid,
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, NULL, NULL, NULL);
}


/***
*void DoPoly(CLSID) 
*
*Purpose:
*  This function simply exercises our CRemPoly class by creating an
*  instance and invoking a number of its methods.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
STDAPI
DoPoly(CLSID clsid)
{
    HRESULT hr;
    int numpoly, i, j;

static struct {
    short x;
    short y;
} rgptPoly[] = {
      { 25,   0}
    , { 75,   0}
    , {100,  25}
    , {100,  75}
    , { 75, 100}
    , { 25, 100}
    , {  0,  75}
    , {  0,  25}
};

static struct {
    short red;
    short green;
    short blue;
} rgrgbColors[] = {
#ifdef _MAC
      {     0,      0,      0}
    , {     0,      0, 0x7fff}
    , {     0, 0x7fff,      0}
    , {0x7fff,      0,      0}
    , {0x7fff,      0, 0x7fff}
    , {0x7fff, 0x7fff,      0}
    , {0x7fff, 0x7fff, 0x7fff}
#else
      {  0,   0,   0}
    , {  0,   0, 127}
    , {  0, 127,   0}
    , {127,   0,   0}
    , {127,   0, 127}
    , {127, 127,   0}
    , {127, 127, 127}
#endif
};

    CRemPoly FAR* rgprempoly[DIM(rgrgbColors)];

    numpoly = DIM(rgprempoly);

    // init
    for(i = 0; i < numpoly; ++i)
      rgprempoly[i] = (CRemPoly FAR*)NULL;

    for(i = 0; i < numpoly; ++i){
      hr = CRemPoly::Create(clsid, &rgprempoly[i]);
      if(hr != NOERROR)
        goto LError0;
      IfMac(DbPrintf("CRemPoly::Create()"));

      for(j = 0; j < DIM(rgptPoly); ++j){
        short x = rgptPoly[j].x;
	short y = rgptPoly[j].y;
        IfMac(DbPrintf("CRemPoly::AddPoint(%d,%d)", x, y));
        hr = rgprempoly[i]->AddPoint(x, y);
	ASSERT(hr == NOERROR);
      }

      for(j = 0; j < DIM(rgrgbColors); ++j){
        hr = rgprempoly[i]->SetWidth(i + j);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::SetWidth()"));

        hr = rgprempoly[i]->set_red(rgrgbColors[j].red);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::set_red()"));

        hr = rgprempoly[i]->set_green(rgrgbColors[j].green);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::set_green()"));

        hr = rgprempoly[i]->set_blue(rgrgbColors[j].blue);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::set_blue()"));

        hr = rgprempoly[i]->SetXOrigin((2*i) + j << 4);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::SetXOrigin()"));

        hr = rgprempoly[i]->SetYOrigin(j << 4);
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::SetYOrigin()"));

        hr = rgprempoly[i]->Draw();
	ASSERT(hr == NOERROR);
	IfMac(DbPrintf("CRemPoly::Draw()"));
      }
    }

    hr = NOERROR;

LError0:;
    for(i = 0; i < numpoly; ++i){
      if(rgprempoly[i] != (CRemPoly FAR*)NULL){
	rgprempoly[i]->Release();
      }
    }

    return hr;
}

