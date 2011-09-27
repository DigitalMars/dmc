/***
*misc.cpp
*
*  Copyright (C) 1992-93, Microsoft Corporation.  All Rights Reserved.
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

#if _MAC
# include <string.h>
# include <ctype.h>
#endif

unsigned long g_dwPolyCF = 0;
unsigned long g_dwPointCF = 0;

IClassFactory FAR* g_ppolyCF = NULL;
IClassFactory FAR* g_ppointCF = NULL;


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

	/*
	 * NOTE: The following is temporary for MacOle Beta 1
	 *
	 */

	, { "Interface\\{00020400-0000-0000-C000-000000000046}",
		"IDispatch" }

	, { "Interface\\{00020400-0000-0000-C000-000000000046}\\ProxyStubClsid",
		"{00020420-0000-0000-C000-000000000046}" }

	, { "Interface\\{00020404-0000-0000-C000-000000000046}",
		"IEnumVARIANT" }

	, { "Interface\\{00020404-0000-0000-C000-000000000046}\\ProxyStubClsid",
		"{00020421-0000-0000-C000-000000000046}" }

	, { "Interface\\{00020401-0000-0000-C000-000000000046}",
		"ITypeInfo" }

	, { "Interface\\{00020401-0000-0000-C000-000000000046}\\ProxyStubClsid",
		"{00020422-0000-0000-C000-000000000046}" }

	, { "Interface\\{00020402-0000-0000-C000-000000000046}",
		"ITypeLib" }

	, { "Interface\\{00020403-0000-0000-C000-000000000046}",
		"ITypeComp" }

	, { "Interface\\{00020405-0000-0000-C000-000000000046}",
		"ICreateTypeInfo" }

	, { "Interface\\{00020406-0000-0000-C000-000000000046}",
		"ICreateTypeLib" }

	, { "CLSID\\{00020420-0000-0000-C000-000000000046}",
		"PSDispatch" }

	, { "CLSID\\{00020420-0000-0000-C000-000000000046}\\InprocServer",
		"OLE2:DISP$DispFSet" }

	, { "CLSID\\{00020422-0000-0000-C000-000000000046}",
		"PSTypeInfo" }

	, { "CLSID\\{00020422-0000-0000-C000-000000000046}\\InprocServer",
		"OLE2:DISP$DispFSet" }

	, { "CLSID\\{00020421-0000-0000-C000-000000000046}",
		"PSEnumVARIANT" }

	, { "CLSID\\{00020421-0000-0000-C000-000000000046}\\InprocServer",
		"OLE2:DISP$DispFSet" }

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
	if(hresult != NOERROR)
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
	if(hresult != NOERROR)
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
UninitOle()
{
	// Tell Ole to release our class factories.
	//
	if(g_dwPointCF != 0L)
	  CoRevokeClassObject(g_dwPointCF);

	if(g_dwPolyCF != 0L)
	  CoRevokeClassObject(g_dwPolyCF);

	OleUninitialize();

	return NOERROR;
}

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
	OutputDebugString(buf);
	DebugBreak();
#endif
}


#ifdef WIN32

//---------------------------------------------------------------------
//           ASCII NLS Wrapper Functions (for Win32)
//---------------------------------------------------------------------

static int WINAPI
CompareStringA(
	LCID lcid,
	unsigned long dwFlags,
	LPSTR lpStr1, int cch1,
	LPSTR lpStr2, int cch2)
{
	WCHAR lpwStr1[32], lpwStr2[32];

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpStr1, cch1, lpwStr1, 32);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpStr2, cch2, lpwStr2, 32);
	return(CompareStringW(lcid, dwFlags, lpwStr1, cch1, lpwStr2, cch2));
}

#endif

#ifdef _MAC
pascal int stricmp(char *first, char *last)
{
	unsigned short f, l;

	do{
		f = tolower(*first++);
		l = tolower(*last++);
	}while(f && f == l);

	return f - l;
}
#endif
