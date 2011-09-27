// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Runtime Typing

// special runtime-class structure for CObject (no base class)
static char BASED_CODE szCObject[] = "CObject";
struct CRuntimeClass AFXAPI_DATA CObject::classCObject =
	{ szCObject, sizeof(CObject), 0xffff, NULL, NULL };
static AFX_CLASSINIT _init_CObject(&CObject::classCObject);

CRuntimeClass* CObject::GetRuntimeClass() const
{
	return &CObject::classCObject;
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
	ASSERT(this != NULL);
	// it better be in valid memory, at least for CObject size
	ASSERT(AfxIsValidAddress(this, sizeof(CObject)));

	// simple SI case
	register CRuntimeClass* pClassThis = GetRuntimeClass();
	ASSERT(pClass != NULL);
	ASSERT(pClassThis != NULL);
	while (pClassThis != NULL)
	{
		if (pClassThis == pClass)
			return TRUE;
		pClassThis = pClassThis->m_pBaseClass;
	}
	return FALSE;       // walked to the top, no match
}

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Support

#ifdef _DEBUG
extern "C" void AFXAPI AfxAssertValidObject(const CObject* pOb,
				LPCSTR lpszFileName, int nLine)
{
	if (pOb == NULL)
	{
		TRACE0("ASSERT_VALID fails with NULL pointer\n");
		AfxAssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}
	if (!AfxIsValidAddress(pOb, sizeof(CObject)))
	{
		TRACE0("ASSERT_VALID fails with illegal pointer\n");
		AfxAssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}

#ifndef _M_I86SM
	// check to make sure the far VTable pointer is valid
	ASSERT(sizeof(CObject) == sizeof(void FAR*));
	if (!AfxIsValidAddress(*(void FAR**)pOb, sizeof(void FAR*), FALSE))
	{
		TRACE0("ASSERT_VALID fails with illegal vtable pointer\n");
		AfxAssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}
#endif

	if (!AfxIsValidAddress(pOb, pOb->GetRuntimeClass()->m_nObjectSize))
	{
		TRACE0("ASSERT_VALID fails with illegal pointer\n");
		AfxAssertFailedLine(lpszFileName, nLine);
		return;     // quick escape
	}
	pOb->AssertValid();
}
#endif //_DEBUG


void CObject::AssertValid() const
{
	ASSERT(this != NULL);
}

void CObject::Dump(CDumpContext& dc) const
{
#ifdef _DEBUG
	AFX_DUMP1(dc, "a ", GetRuntimeClass()->m_lpszClassName);
	AFX_DUMP1(dc, " at ", (void*) this);
	AFX_DUMP0(dc, " ");
#else
	dc;
#endif //_DEBUG
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Allocation/Creation

CObject* CRuntimeClass::CreateObject()
{
	void* pObject = NULL;
	TRY
	{
		pObject = CObject::operator new(m_nObjectSize);
		if (ConstructObject(pObject))
			return (CObject*) pObject;
	}
	CATCH_ALL(e)
	{
		// fall through
	}
	END_CATCH_ALL

	TRACE0("Warning: CRuntimeClass::CreateObject failed\n");
	if (pObject != NULL)
		CObject::operator delete(pObject);  // clean up
	return NULL;
}

BOOL CRuntimeClass::ConstructObject(void* pThis)
// dynamically construct an instance of this class in the memory pointed
//    to by 'pThis'.  Return FALSE if can't construct (i.e. an abstract class)
{
	ASSERT(AfxIsValidAddress(pThis, m_nObjectSize));

	if (m_pfnConstruct != NULL)
	{
		(*m_pfnConstruct)(pThis);
		return TRUE;
	}
	else
	{
		TRACE1("Error: Trying to construct object which is not "
			   "DECLARE_DYNCREATE \nor DECLARE_SERIAL: %hs.\n",
			m_lpszClassName);
		return FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////
// Class loader & class serialization

BOOL CObject::IsSerializable() const
{
	return (GetRuntimeClass()->m_wSchema != 0xffff);
}

#ifdef _AFXDLL
// for the shared DLL, we build a sub-list of the CRuntimeClasses during
//   a single DLL or EXE init.  This sub-list gets moved from the
//   global 'pFirstClass' to some other place once the DLL or EXE
//   is properly initialized.
#endif

CRuntimeClass* AFXAPI_DATA CRuntimeClass::pFirstClass = NULL;

AFX_CLASSINIT::AFX_CLASSINIT(register CRuntimeClass* pNewClass)
{
	ASSERT(pNewClass->m_pNextClass == NULL);
	pNewClass->m_pNextClass = CRuntimeClass::pFirstClass;
	CRuntimeClass::pFirstClass = pNewClass;
}

#ifdef _DEBUG
void AFXAPI AfxDoForAllClasses(void (*pfn)(const CRuntimeClass*, void*),
	void* pContext)
{
	CRuntimeClass* pClass;

#ifndef _AFXDLL
	// just walk through the simple list of registered classes
	for (pClass = CRuntimeClass::pFirstClass; pClass != NULL;
		pClass = pClass->m_pNextClass)
	{
		(*pfn)(pClass, pContext);
	}
#else
	ASSERT(CRuntimeClass::pFirstClass == NULL); // should be none unattached
	// first walk through the app specific classes
	for (pClass = _AfxGetAppData()->pFirstAppClass; pClass != NULL;
		pClass = pClass->m_pNextClass)
	{
		(*pfn)(pClass, pContext);
	}
	// now walk through the classes in different DLLs
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		for (pClass = pDLL->m_pFirstSharedClass; pClass != NULL;
			pClass = pClass->m_pNextClass)
		{
			(*pfn)(pClass, pContext);
		}
	}
#endif //_AFXDLL
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Non-diagnostic memory routines
//

#ifndef _AFXDLL

#ifndef _WINDOWS
#include <new.h>
#define _AfxSetNewHandler(pnh)  _set_new_handler(pnh)
#endif

int cdecl AfxNewHandler(size_t /* nSize */)
{
	//  AFX memory allocation will never return "NULL" it will always throw
	//      a memory exception instead
	AfxThrowMemoryException();
	return 0;
}

// hook in our own new_handler
static BOOL AfxInitialize()
{
	(void)_afx_version();
#ifdef _DEBUG
	// Force reference of the following symbols for CodeView
	(void)afxTraceEnabled;
	(void)afxMemDF;
#ifdef _WINDOWS
	(void)afxTraceFlags;
#endif
	if (!AfxDiagnosticInit())
		return FALSE;
#endif //_DEBUG

	_AfxSetNewHandler(AfxNewHandler);
	return TRUE;
}

static BOOL NEAR bInitialized = AfxInitialize();
		// a way to force initialization

#endif //_AFXDLL

/////////////////////////////////////////////////////////////////////////////
