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

unsigned long g_dwPolyCF = 0;
unsigned long g_dwPointCF = 0;

IClassFactory FAR* g_ppolyCF = NULL;
IClassFactory FAR* g_ppointCF = NULL;

unsigned int g_fObjectsRevoked = 0;
unsigned int g_cObjects = 0;
unsigned int g_fQuit = 0;

#ifdef _MAC
struct regentry{
    char *szKey;
    char *szValue;
} g_rgregentry[] = {

      { "CLSID\\{00020464-0000-0000-C000-000000000046}",
	"OLE Automation SPoly2 1.0 Application" }

    , { "CLSID\\{00020464-0000-0000-C000-000000000046}\\LocalServer",
        "SPL2" }

    , { "CLSID\\{00020464-0000-0000-C000-000000000046}\\ProgID",
	"SPoly2.Application" }

    , { "CLSID\\{00020464-0000-0000-C000-000000000046}\\InprocHandler",
	"OLE2:Def$DefFSet" }

    , { "SPL2", "{00020464-0000-0000-C000-000000000046}" }

    , { "SPoly2.Application\\CLSID",
	"{00020464-0000-0000-C000-000000000046}" }

};

HRESULT
EnsureRegistration()
{
    HKEY hkey;

    if(RegOpenKey(HKEY_CLASSES_ROOT, "SPL2", &hkey) == NOERROR){
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
      CLSID_CPoint2,
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
      CLSID_CPoly2,
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

#ifdef _MAC
#if defined(_MSC_VER)
int pascal
#else
pascal int
#endif
stricmp(char *first, char *last)
{
    unsigned short f, l;

    do{
	f = tolower(*first++);
	l = tolower(*last++);
    }while(f && f == l);

    return f - l;
}
#endif

