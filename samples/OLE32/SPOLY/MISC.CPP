/*** 
*misc.cpp
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
*
*Implementation Notes:
*
*****************************************************************************/



#include "spoly.h"
#include "cpoint.h"
#include "cpoly.h"

#include <stdio.h>

#ifdef _MAC
# include <string.h>
# include <ctype.h>
#endif
		      
#ifdef WIN32  // Use native CompareString operation
  #undef CompareString
  #ifdef UNICODE
    #define CompareString CompareStringW
  #else
    // CONSIDER: write a wrapper for CompareStringW if not available
  #endif
#else	  
  #define CompareString CompareStringA
#endif	  

unsigned long g_dwPolyCF = 0;
unsigned long g_dwPointCF = 0;

IClassFactory FAR* g_ppolyCF = NULL;
IClassFactory FAR* g_ppointCF = NULL;

unsigned int g_fObjectsRevoked = 0;
unsigned long g_cObjects = 0;
unsigned int g_fQuit = 0;

#ifdef _MAC
struct regentry{
    char *szKey;
    char *szValue;
} g_rgregentry[] = {

      { "CLSID\\{00020462-0000-0000-C000-000000000046}",
	"OLE Automation SPoly 1.0 Application" }

    , { "CLSID\\{00020462-0000-0000-C000-000000000046}\\LocalServer",
	"SPLy" }

    , { "CLSID\\{00020462-0000-0000-C000-000000000046}\\ProgID",
	"SPoly.Application" }

    , { "CLSID\\{00020462-0000-0000-C000-000000000046}\\InprocHandler",
	"OLE2:Def$DefFSet" }

    , { "SPLy", "{00020462-0000-0000-C000-000000000046}" }

    , { "SPoly.Application\\CLSID",
	"{00020462-0000-0000-C000-000000000046}" }

};

HRESULT
EnsureRegistration()
{
    HKEY hkey;

    if(RegOpenKey(HKEY_CLASSES_ROOT, "SPLy", &hkey) == NOERROR){
      RegCloseKey(hkey);
      return NOERROR;
    }

    for(int i = 0; i < DIM(g_rgregentry); ++i){
      if(RegSetValue(HKEY_CLASSES_ROOT, g_rgregentry[i].szKey, REG_SZ, g_rgregentry[i].szValue, 0) != ERROR_SUCCESS)
        return ResultFromScode(E_FAIL);
    }

    return NOERROR;
}
#endif


/***
*HRESULT InitOle(void)
*Purpose:
*  Initialize Ole, and register our class factories.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
STDAPI
InitOle()
{
    HRESULT hresult;

    if((hresult = OleInitialize(NULL)) != NOERROR)
      goto LError0;

#ifdef _MAC
    if((hresult = EnsureRegistration()) != NOERROR)
      goto LError0;
#endif

    // Register the CPoint Class Factory
    //
    if((g_ppointCF = CPointCF::Create()) == NULL){
      hresult = ResultFromScode(E_OUTOFMEMORY);
      goto LError1;
    }

    hresult = CoRegisterClassObject(
      CLSID_CPoint,
      g_ppointCF,
      CLSCTX_LOCAL_SERVER,
      REGCLS_MULTIPLEUSE,
      &g_dwPointCF);
    if(FAILED(hresult))
      goto LError1;

    // Register the CPoly Class Factory.
    //
    if((g_ppolyCF = CPolyCF::Create()) == NULL){
      hresult = ResultFromScode(E_OUTOFMEMORY);
      goto LError1;
    }

    hresult = CoRegisterClassObject(
      CLSID_CPoly,
      g_ppolyCF,
      CLSCTX_LOCAL_SERVER,
      REGCLS_MULTIPLEUSE,
      &g_dwPolyCF);
    if(FAILED(hresult))
      goto LError1;

    g_ppolyCF->Release();

    g_ppointCF->Release();

    return NOERROR;


LError1:;
    if(g_ppolyCF != NULL)
      g_ppolyCF->Release();

    if(g_ppointCF != NULL)
      g_ppointCF->Release();

    UninitOle();

LError0:;
    return hresult;
}

STDAPI 
Revoke()
{
    if (!g_fObjectsRevoked) {
      // Tell Ole to release our class factories.
      //
      if(g_dwPointCF != 0L)
        CoRevokeClassObject(g_dwPointCF);

      if(g_dwPolyCF != 0L)
        CoRevokeClassObject(g_dwPolyCF);

      g_fObjectsRevoked = 1;
    }

    return NOERROR;
}

STDAPI
UninitOle()
{
    Revoke();
    OleUninitialize();

    return NOERROR;
}

/***
*HRESULT SPolyGetIDsOfNames(MEMBERDESC*, unsigned int, char**, unsigned int, LCID, long*)
*Purpose:
*  This is the table driven implementation of IDispatch::GetIDsOfNames
*  deferred to by both the CPoint and CPoly objects.
*
*Entry:
*  rgmd = pointer to an array of method descriptors
*  cMethods = number of elements in the array of method descriptors
*  rgszNames = pointer to an array of names
*  cNames = the number of names in the rgszNames array
*  lcid = the callers locale ID
*
*Exit:
*  return value = HRESULT
*  rgdispid = array of name IDs corresponding to the rgszNames array
*    this array will contain -1 for each entry that is not known.
*
***********************************************************************/
STDAPI
SPolyGetIDsOfNames(
    MEMBERDESC FAR* rgmd,
    unsigned int cMethods,
    OLECHAR FAR* FAR* rgszNames,
    unsigned int cNames,
    LCID lcid,
    DISPID FAR* rgdispid)
{
    HRESULT hresult;
    PARAMDESC FAR* ppd;
    MEMBERDESC FAR* pmd;
    unsigned int iName, iTry, cParams;

    hresult = NOERROR;

    // first lookup the member name.
    //
    for(pmd = rgmd;; ++pmd){
      if(pmd == &rgmd[cMethods])
        goto LMemberNotFound;

#if defined(WIN32) && !defined(UNICODE)
      // CompareStringW is not available on Windows 95
      if(wcsicmp(rgszNames[0], pmd->szName) == 0)
#else
      if(CompareString(lcid, NORM_IGNORECASE, rgszNames[0], -1, pmd->szName, -1) == 2)
#endif
      {
	rgdispid[0] = pmd->id;
	break;
      }
    }

    // Lookup the named parameters, if there are any.
    if(cNames > 1){
      cParams = pmd->cParams;
      for(iName = 1; iName < cNames; ++iName){
	for(iTry = 0;; ++iTry){
	  if(iTry == cParams){
	    hresult = ResultFromScode(DISP_E_UNKNOWNNAME);
	    rgdispid[iName] = -1;
	    break;
	  }
	  ppd = &pmd->rgpd[iTry];

#if defined(WIN32) && !defined(UNICODE)
          // CompareStringW is not available on Windows 95
          if(wcsicmp(rgszNames[iName], ppd->szName) == 0)
#else
	  if(CompareString(lcid, NORM_IGNORECASE, rgszNames[iName], -1, ppd->szName, -1) == 2)
#endif
	  {
	    // The DISPID for a named parameter is defined to be its
	    // zero based positional index.  This routine assumes that
	    // that PARAMDESC array was declared in correct textual order.
	    //
	    rgdispid[iName] = iTry;
	    break;
	  }
        }
      }
    }

    return hresult;

LMemberNotFound:;
    // If the member name is unknown, everything is unknown.
    for(iName = 0; iName < cNames; ++iName)
      rgdispid[iName] = -1;
    return ResultFromScode(DISP_E_UNKNOWNNAME);
}


// disable unicode expansion for assertions
#undef UNICODE

void
Assert(int fCond, char FAR* file, int line, char FAR* message)
{
    char * fmt;
    char buf[128];

    if(fCond)
      return;

    fmt = (message == NULL)
      ? "Assertion failed: %s(%d)"
      : "Assertion failed: %s(%d) '%s'";
    sprintf(buf, fmt, file, line, message);

#ifdef _MAC
    DebugStr(c2pstr(buf));
#else
#ifdef WIN32
    OutputDebugStringA(buf);
#else //WIN32
    OutputDebugString(buf);
#endif //WIN32
    DebugBreak();
#endif
}

void IncObjectCount()
{
    g_cObjects++;
}

void DecObjectCount()
{
    g_cObjects--;

    if (!g_cObjects && g_fQuit) {
      Revoke();
#ifndef _MAC
      PostQuitMessage(0);
#endif // !_MAC
    }
}

