/*** 
*CPoly.cpp
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
*  This module implements the CPoly and CPolyCF classes.
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
#include "cpoly.h"
#include "cenumpt.h"

#ifndef _MAC
extern CStatBar FAR* g_psb;
#else
extern "C" WindowPtr g_pwndClient;
#endif

extern unsigned int g_fQuit;

// our global list of polygons.
//
POLYLINK FAR* g_ppolylink = (POLYLINK FAR*)NULL;

// global count of polygons.
//
int g_cPoly = 0;


CPoly::CPoly()
{
    m_refs = 0;
    m_xorg = 0;
    m_yorg = 0;
    m_width = 0;
    m_cPoints = 0;

    m_red   = 0;
    m_green = 0;
    m_blue  = 0;

    m_ppointlink = NULL;
    m_ppointlinkLast = NULL;
}


/***
*CPoly *CPoly::Create(void)
*Purpose:
*  Create an instance of a CPoly object, and add it to the global
*  list of polygons.
*
*Entry:
*  None
*
*Exit:
*  returns a polygon object, NULL the allocation failed.
*
***********************************************************************/
CPoly FAR*
CPoly::Create()
{
    CPoly FAR* ppoly;
    POLYLINK FAR* ppolylink;

    if((ppolylink = new FAR POLYLINK) == (POLYLINK FAR*)NULL)
      return (CPoly FAR*)NULL;

    if((ppoly = new FAR CPoly()) == (CPoly FAR*)NULL)
      return (CPoly FAR*)NULL;

    ppoly->AddRef();

    // push the new polygon onto the front of the polygon list.
    //
    ++g_cPoly;

    ppolylink->ppoly = ppoly;

    ppolylink->next = g_ppolylink;
    g_ppolylink = ppolylink;

#ifndef _MAC
    SBprintf(g_psb, TSTR("#poly = %d"), g_cPoly);
#endif

    IncObjectCount();

    return ppoly;
}


//---------------------------------------------------------------------
//                     IUnknown Methods
//---------------------------------------------------------------------


STDMETHODIMP
CPoly::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(IsEqualIID(riid, IID_IUnknown) ||
       IsEqualIID(riid, IID_IDispatch)) {
      *ppv = this;
      AddRef();
      return NOERROR;       
    }
	       
    *ppv = NULL;
    return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(unsigned long)
CPoly::AddRef()
{
    return ++m_refs;
}


STDMETHODIMP_(unsigned long)
CPoly::Release()
{
    POLYLINK FAR* FAR* pppolylink, FAR* ppolylinkDead;

    if(--m_refs == 0){
      Reset(); // release all CPoints

      // remove ourselves from the global list of polygons
      //
      for( pppolylink = &g_ppolylink;
          *pppolylink != NULL;
	   pppolylink = &(*pppolylink)->next)
      {
	if((*pppolylink)->ppoly == this){
	  ppolylinkDead = *pppolylink;
	  *pppolylink = (*pppolylink)->next;
	  delete ppolylinkDead;
	  break;
	}
      }

      --g_cPoly;

#ifndef _MAC
      SBprintf(g_psb, TSTR("#poly = %d"), g_cPoly);
#endif

      delete this;

      DecObjectCount();

      return 0;
    }
    return m_refs;
}


//---------------------------------------------------------------------
//                     IDispatch Methods
//---------------------------------------------------------------------


/*
 * NOTE: Support for the following two methods is not available
 * in this version.
 *
 */

STDMETHODIMP
CPoly::GetTypeInfoCount(unsigned int FAR* pctinfo)
{
    *pctinfo = 0;
    return NOERROR;
}


STDMETHODIMP
CPoly::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    UNUSED(itinfo);
    UNUSED(lcid);
    UNUSED(pptinfo);

    return ResultFromScode(E_NOTIMPL);
}


/***
*HRESULT CPoly::GetIDsOfNames(OLECHAR**, unsigned int, LCID, long*)
*Purpose:
*  This method translates the given array of names to a corresponding
*  array of DISPIDs.
*
*  This method deferrs to a common implementation shared by
*  both the CPoly and CPoint objects. See the description of
*  'SPolyGetIDsOfNames()' in dispimpl.cpp for more information.
*
*Entry:
*  rgszNames = pointer to an array of names
*  cNames = the number of names in the rgszNames array
*  lcid = the callers locale ID
*
*Exit:
*  return value = HRESULT
*  rgdispid = array of DISPIDs corresponding to the rgszNames array
*    this array will contain -1 for each entry that is not known.
*
***********************************************************************/
STDMETHODIMP
CPoly::GetIDsOfNames(
    REFIID riid,
    OLECHAR FAR* FAR* rgszNames,
    unsigned int cNames,
    LCID lcid,
    DISPID FAR* rgdispid)
{
static PARAMDESC rgpdAddPoint[] = {
    {OLESTR("X")}, {OLESTR("Y")}
};
static MEMBERDESC rgmdCPoly[] = {
    {OLESTR("DRAW"),	   IDMEMBER_CPOLY_DRAW,		NULL,		0},
    {OLESTR("RESET"),	   IDMEMBER_CPOLY_RESET,	NULL,		0},
    {OLESTR("ADDPOINT"),   IDMEMBER_CPOLY_ADDPOINT,	rgpdAddPoint,	2},
    {OLESTR("ENUMPOINTS"), IDMEMBER_CPOLY_ENUMPOINTS,	NULL,		0},
    {OLESTR("GETXORIGIN"), IDMEMBER_CPOLY_GETXORIGIN,	NULL,		0},
    {OLESTR("SETXORIGIN"), IDMEMBER_CPOLY_SETXORIGIN,	NULL,		0},
    {OLESTR("GETYORIGIN"), IDMEMBER_CPOLY_GETYORIGIN,	NULL,		0},
    {OLESTR("SETYORIGIN"), IDMEMBER_CPOLY_SETYORIGIN,	NULL,		0},
    {OLESTR("GETWIDTH"),   IDMEMBER_CPOLY_GETWIDTH,	NULL,		0},
    {OLESTR("SETWIDTH"),   IDMEMBER_CPOLY_SETWIDTH,	NULL,		0},
    {OLESTR("get_red"),	   IDMEMBER_CPOLY_GETRED,	NULL,		0},
    {OLESTR("set_red"),	   IDMEMBER_CPOLY_SETRED,	NULL,		0},
    {OLESTR("get_green"),  IDMEMBER_CPOLY_GETGREEN,	NULL,		0},
    {OLESTR("set_green"),  IDMEMBER_CPOLY_SETGREEN,	NULL,		0},
    {OLESTR("get_blue"),   IDMEMBER_CPOLY_GETBLUE,	NULL,		0},
    {OLESTR("set_blue"),   IDMEMBER_CPOLY_SETBLUE,	NULL,		0},
    {OLESTR("DUMP"),	   IDMEMBER_CPOLY_DUMP,		NULL,		0},
    {OLESTR("Quit"),       IDMEMBER_CPOLY_QUIT,     NULL,       0}
};

    // this object only exposes a "default" interface.
    //
    if(!IsEqualIID(riid, IID_NULL))
      return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

    return SPolyGetIDsOfNames(
      rgmdCPoly, DIM(rgmdCPoly), rgszNames, cNames, lcid, rgdispid);
}


/***
*HRESULT CPoly::Invoke(...)
*Purpose:
*  Dispatch a method or property request for objects of type CPoly.
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
CPoly::Invoke(
    DISPID dispidMember,
    REFIID riid,
    LCID lcid,
    unsigned short wFlags,
    DISPPARAMS FAR* pdispparams,
    VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo,
    unsigned int FAR* puArgErr)
{
    HRESULT hresult;
    VARIANTARG varg0, varg1;
    VARIANT varResultDummy;

    UNUSED(lcid);
    UNUSED(pexcepinfo);

    if(wFlags & ~(DISPATCH_METHOD | DISPATCH_PROPERTYGET | DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
      return ResultFromScode(E_INVALIDARG);

    // this object only exposes a "default" interface.
    //
    if(!IsEqualIID(riid, IID_NULL))
      return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

    // This makes the following code a bit simpler if the caller
    // happens to be ignoring the return value. Some implementations
    // may choose to deal with this differently.
    //
    if(pvarResult == (VARIANT FAR*)NULL)
      pvarResult = &varResultDummy;

    VariantInit(&varg0);
    VariantInit(&varg1);

    // assume the return type is void, unless we find otherwise.
    VariantInit(pvarResult);

    switch(dispidMember){
    case IDMEMBER_CPOLY_DRAW:
      Draw();
      break;

    case IDMEMBER_CPOLY_RESET:
      Reset();
      break;

    case IDMEMBER_CPOLY_DUMP:
      Dump();
      break;

    case IDMEMBER_CPOLY_QUIT:
      Quit();
      break;

    case IDMEMBER_CPOLY_ADDPOINT:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
	return hresult;

      hresult = DispGetParam(pdispparams, 1, VT_I2, &varg1, puArgErr);
      if(hresult != NOERROR)
	return hresult;

      hresult = AddPoint(V_I2(&varg1), V_I2(&varg0));
      if(hresult != NOERROR)
	return hresult;
      break;

    case IDMEMBER_CPOLY_ENUMPOINTS:
      IEnumVARIANT FAR* penum;

      hresult = EnumPoints(&penum);
      if(hresult != NOERROR)
	return hresult;

      V_VT(pvarResult) = VT_UNKNOWN;
      hresult = penum->QueryInterface(
	IID_IUnknown, (void FAR* FAR*)&V_UNKNOWN(pvarResult));
      if(hresult != NOERROR)
	return hresult;
      penum->Release();
      break;

    case IDMEMBER_CPOLY_GETXORIGIN:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = m_xorg;
      break;

    case IDMEMBER_CPOLY_SETXORIGIN:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      m_xorg = V_I2(&varg0);
      break;

    case IDMEMBER_CPOLY_GETYORIGIN:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = m_yorg;
      break;

    case IDMEMBER_CPOLY_SETYORIGIN:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      m_yorg = V_I2(&varg0);
      break;

    case IDMEMBER_CPOLY_GETWIDTH:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = GetWidth();
      break;

    case IDMEMBER_CPOLY_SETWIDTH:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      SetWidth(V_I2(&varg0));
      break;

    case IDMEMBER_CPOLY_GETRED:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = get_red();
      break;

    case IDMEMBER_CPOLY_SETRED:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      set_red(V_I2(&varg0));
      break;

    case IDMEMBER_CPOLY_GETGREEN:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = get_green();
      break;

    case IDMEMBER_CPOLY_SETGREEN:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      set_green(V_I2(&varg0));
      break;

    case IDMEMBER_CPOLY_GETBLUE:
      V_VT(pvarResult) = VT_I2;
      V_I2(pvarResult) = get_blue();
      break;

    case IDMEMBER_CPOLY_SETBLUE:
      hresult = DispGetParam(pdispparams, 0, VT_I2, &varg0, puArgErr);
      if(hresult != NOERROR)
        return hresult;
      set_blue(V_I2(&varg0));
      break;

    default:
      return ResultFromScode(DISP_E_MEMBERNOTFOUND);
    }

    return NOERROR;
}


//---------------------------------------------------------------------
//                     Introduced Methods
//---------------------------------------------------------------------


/***
*void CPoly::Draw(void)
*Purpose:
*  Draw the polygon, using the current x/y origin and line width
*  properties.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void PASCAL
CPoly::Draw()
{
    short xorg, yorg;
    POINTLINK FAR* ppointlinkFirst, FAR* ppointlink;

    if((ppointlinkFirst = m_ppointlink) == (POINTLINK FAR*)NULL)
      return;

#ifdef _MAC /* { */

    short x, y;
    RGBColor rgb;
    WindowPtr pwndSaved;

    GetPort(&pwndSaved);
    SetPort(g_pwndClient);

    PenNormal();
    PenSize(m_width, m_width);

    rgb.red = m_red;
    rgb.green = m_green;
    rgb.blue = m_blue;
    RGBForeColor(&rgb);

    xorg = m_xorg;
    yorg = m_yorg;

    MoveTo(
      xorg + ppointlinkFirst->ppoint->m_x,
      yorg + ppointlinkFirst->ppoint->m_y);

    for(ppointlink = ppointlinkFirst->next;
	ppointlink != (POINTLINK FAR*)NULL;
	ppointlink = ppointlink->next)
    {
      x = xorg + ppointlink->ppoint->m_x;
      y = yorg + ppointlink->ppoint->m_y;
      LineTo(x, y);
    }

    LineTo(
      xorg + ppointlinkFirst->ppoint->m_x,
      yorg + ppointlinkFirst->ppoint->m_y);

    SetPort(pwndSaved);

#else /* }{ */

    HDC hdc;
    RECT rect;
    HPEN hpen, hpenOld;
extern HWND g_hwndClient;

    GetClientRect(g_hwndClient, &rect);
    xorg = m_xorg + (short) rect.left;
    yorg = m_yorg + (short) rect.top;

    hdc = GetDC(g_hwndClient);
    hpen = CreatePen(PS_SOLID, m_width, RGB(m_red, m_green, m_blue));
    hpenOld = (HPEN)SelectObject(hdc, hpen);
	
#ifdef WIN32
    MoveToEx(hdc,
      xorg + ppointlinkFirst->ppoint->m_x,
      yorg + ppointlinkFirst->ppoint->m_y, NULL);
#else
    MoveTo(hdc,
      xorg + ppointlinkFirst->ppoint->m_x,
      yorg + ppointlinkFirst->ppoint->m_y);
#endif

    for(ppointlink = ppointlinkFirst->next;
	ppointlink != (POINTLINK FAR*)NULL;
	ppointlink = ppointlink->next)
    {
      LineTo(hdc,
	xorg + ppointlink->ppoint->m_x,
	yorg + ppointlink->ppoint->m_y);
    }

    LineTo(hdc,
      xorg + ppointlinkFirst->ppoint->m_x,
      yorg + ppointlinkFirst->ppoint->m_y);

    SelectObject(hdc, hpenOld);
    DeleteObject(hpen);

    ReleaseDC(g_hwndClient, hdc);

#endif /* } */
}


/***
*void CPoly::Reset(void)
*Purpose:
*  Release all points referenced by this poly.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void PASCAL
CPoly::Reset()
{
    POINTLINK FAR* ppointlink, FAR* ppointlinkNext;

    for(ppointlink = m_ppointlink;
	ppointlink != (POINTLINK FAR*)NULL;
	ppointlink = ppointlinkNext)
    {
      ppointlinkNext = ppointlink->next;
      ppointlink->ppoint->Release();
      delete ppointlink;
    }

    m_cPoints = 0;
    m_ppointlink = NULL;
    m_ppointlinkLast = NULL;
}


/***
*HRESULT CPoly::AddPoint(short, short)
*Purpose:
*  Add a CPoint with the given coordinates to the end of our current
*  list of points.
*
*Entry:
*  x,y = the x and y coordinates of the new point.
*
*Exit:
*  return value = HRESULT
*
***********************************************************************/
HRESULT PASCAL
CPoly::AddPoint(short x, short y)
{
    CPoint FAR* ppoint;
    POINTLINK FAR* ppointlink;

    ppoint = CPoint::Create();
    if(ppoint == (CPoint FAR*)NULL)
      return ResultFromScode(E_OUTOFMEMORY);

    ppoint->SetX(x);
    ppoint->SetY(y);

    ppointlink = new FAR POINTLINK;
    if(ppointlink == (POINTLINK FAR*)NULL){
      delete ppoint;
      return ResultFromScode(E_OUTOFMEMORY);
    }

    ppointlink->ppoint = ppoint;
    ppointlink->next = (POINTLINK FAR*)NULL;

    if(m_ppointlinkLast == (POINTLINK FAR*)NULL){
      m_ppointlink = m_ppointlinkLast = ppointlink;
    }else{
      m_ppointlinkLast->next = ppointlink;
      m_ppointlinkLast = ppointlink;
    }

    ++m_cPoints;

    return NOERROR;
}


/***
*HRESULT CPoly::EnumPoints(IEnumVARIANT**);
*Purpose:
*  Return and enumerator for the points in this polygon.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*
*  *ppenum = pointer to an IEnumVARIANT for the points in this polygon
*
***********************************************************************/
HRESULT PASCAL
CPoly::EnumPoints(IEnumVARIANT FAR* FAR* ppenum)
{
    unsigned int i;
    VARIANT var;
    HRESULT hresult;
    SAFEARRAY FAR* psa;
    CEnumPoint FAR* penum;
    POINTLINK FAR* ppointlink;
    SAFEARRAYBOUND rgsabound[1];

    rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = m_cPoints;

    psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);
    if(psa == NULL){
      hresult = ResultFromScode(E_OUTOFMEMORY);
      goto LError0;
    }

    ppointlink = m_ppointlink;
    for(i = 0; i < m_cPoints; ++i){
      long ix[1];

      if(ppointlink == NULL){
        // this indicates an internal consistency error.
	// (this test should probably be an assertion)
        hresult = ResultFromScode(E_FAIL);
        goto LError1;
      }

      V_VT(&var) = VT_DISPATCH;
      hresult = ppointlink->ppoint->QueryInterface(
	IID_IDispatch, (void FAR* FAR*)&V_DISPATCH(&var));
      if(hresult != NOERROR)
        goto LError1;

      ix[0] = i;
      SafeArrayPutElement(psa, ix, &var);

      // SafeArrayPutElement adds a reference to the contents of the
      // variant, so we must free the variable we have.
      //
      VariantClear(&var);

      ppointlink = ppointlink->next;
    }

    hresult = CEnumPoint::Create(psa, &penum);
    if(hresult != NOERROR)
      goto LError1;

    *ppenum = penum;

    return NOERROR;

LError1:;
    // destroy the array if we were not successful creating the enumerator.
    SafeArrayDestroy(psa);

LError0:;
    return hresult;
}

short PASCAL
CPoly::GetXOrigin()
{
    return m_xorg;
}

void PASCAL
CPoly::SetXOrigin(short x)
{
    m_xorg = x;
}

short PASCAL
CPoly::GetYOrigin()
{
    return m_yorg;
}

void PASCAL
CPoly::SetYOrigin(short y)
{
    m_yorg = y;
}

short PASCAL
CPoly::GetWidth()
{
    return m_width;
}

void PASCAL
CPoly::SetWidth(short width)
{
    m_width = width;
}

short PASCAL
CPoly::get_red()
{
    return m_red;
}

void PASCAL
CPoly::set_red(short red)
{
    m_red = red;
}

short PASCAL
CPoly::get_green()
{
    return m_green;
}

void PASCAL
CPoly::set_green(short green)
{
    m_green = green;
}

short PASCAL
CPoly::get_blue()
{
    return m_blue;
}

void PASCAL
CPoly::set_blue(short blue)
{
    m_blue = blue;
}


/***
*void CPoly::Dump(void)
*Purpose:
*  Output a debug dump of this instance.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void PASCAL
CPoly::Dump()
{
#ifdef _MAC

    // REVIEW: implement for the mac

#else

    TCHAR buffer[80];
    POINTLINK FAR* ppointlink;

    wsprintf(buffer, TSTR("CPoly(0x%x) =\n"), (int)this);
    OutputDebugString(buffer);

    wsprintf(buffer,
      TSTR("    xorg = %d, yorg = %d, width = %d, rgb = {%d,%d,%d}\n    points = "),
      m_xorg, m_yorg, m_width,
      get_red(),
      get_green(),
      get_blue());

    OutputDebugString(buffer);

    for(ppointlink = m_ppointlink;
	ppointlink != (POINTLINK FAR*)NULL;
	ppointlink = ppointlink->next)
    {
      wsprintf(buffer, TSTR("{%d,%d}"),
        ppointlink->ppoint->GetX(),
        ppointlink->ppoint->GetY());
      OutputDebugString(buffer);

      wsprintf(buffer, TSTR(" "));
      OutputDebugString(buffer);
    }
    wsprintf(buffer, TSTR("\n"));
    OutputDebugString(buffer);

#endif
}

/***
*void CPoly::Quit(void)
*Purpose:
*  Exit when all objects are released.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void PASCAL
CPoly::Quit()
{
    g_fQuit = 1;
}

/***
*void CPoly::PolyDraw(void)
*Purpose:
*  Draw all polygons.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void
CPoly::PolyDraw()
{
    POLYLINK FAR* polylink;

    for(polylink = g_ppolylink;
	polylink != (POLYLINK FAR*)NULL;
	polylink = polylink->next)
    {
      polylink->ppoly->Draw();
    }
}


/***
*void PolyTerm(void)
*Purpose:
*  Release all polygons.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void
CPoly::PolyTerm()
{
    POLYLINK FAR* ppolylink;
    POLYLINK FAR* ppolylinkNext;

    for(ppolylink = g_ppolylink;
	ppolylink != (POLYLINK FAR*)NULL;
	ppolylink = ppolylinkNext)
    {
      ppolylinkNext = ppolylink->next;
      ppolylink->ppoly->Release();
      delete ppolylink;
    }
    g_ppolylink = NULL;
}


/***
*void PolyDump(void)
*Purpose:
*  Invoke the debug Dump() method on all polygons were currently
*  holding on to.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void
CPoly::PolyDump()
{
    POLYLINK FAR* ppolylink;

    if(g_ppolylink == (POLYLINK FAR*)NULL){
#ifndef _MAC
      OutputDebugString(TSTR("\t(none)\n"));
#endif
      return;
    }

    for(ppolylink = g_ppolylink;
	ppolylink != (POLYLINK FAR*)NULL;
	ppolylink = ppolylink->next)
    {
      ppolylink->ppoly->Dump();
    }
}


//---------------------------------------------------------------------
//             Implementation of the CPoly Class Factory 
//---------------------------------------------------------------------


CPolyCF::CPolyCF()
{
    m_refs = 0;
}

IClassFactory FAR*
CPolyCF::Create()
{
    CPolyCF FAR* pCF;

    if((pCF = new FAR CPolyCF()) == NULL)
      return NULL;
    pCF->AddRef();
    return pCF;
}

STDMETHODIMP
CPolyCF::QueryInterface(REFIID riid, void FAR* FAR* ppv) 
{
    if(!IsEqualIID(riid, IID_IUnknown)){
      if(!IsEqualIID(riid, IID_IClassFactory)){
	*ppv = NULL;
        return ResultFromScode(E_NOINTERFACE);
      }
    }

    *ppv = this;
    ++m_refs;
    return NOERROR;
}

STDMETHODIMP_(unsigned long)
CPolyCF::AddRef(void)
{
    return ++m_refs;
}

STDMETHODIMP_(unsigned long)
CPolyCF::Release(void)
{
    if(--m_refs == 0){
      delete this;
      return 0;
    }
    return m_refs;
}

STDMETHODIMP
CPolyCF::CreateInstance(
    IUnknown FAR* punkOuter,
    REFIID iid,
    void FAR* FAR* ppv)
{
    HRESULT hresult;
    CPoly FAR *ppoly;

    UNUSED(punkOuter);

#ifdef _MAC
    if(GetZone() != ApplicZone()){
#ifndef _MSC_VER
#ifndef ConstStr255Param
#define ConstStr255Param StringPtr
#endif
#endif
      DebugStr((ConstStr255Param)"\pZones do not match");
    }
	
#endif

    if((ppoly = CPoly::Create()) == NULL){
      *ppv = NULL;
      return ResultFromScode(E_OUTOFMEMORY);
    }
    hresult = ppoly->QueryInterface(iid, ppv);
    ppoly->Release();
    return hresult;
}

STDMETHODIMP
#ifdef _MAC
CPolyCF::LockServer(unsigned long fLock)
#else
CPolyCF::LockServer(BOOL fLock)
#endif
{
    UNUSED(fLock);

    return NOERROR;
}

