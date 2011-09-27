// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////

/* Included in this file:
 *  Most Windows objects are represented with a HANDLE, including
 *      the most important ones, HWND, HDC, HPEN, HFONT etc.
 *  We want C++ objects to wrap these handle based objects whenever we can.
 *  Since Windows objects can be created outside of C++ (eg: calling
 *      ::CreateWindow will return an HWND with no C++ wrapper) we must
 *      support a reasonably uniform mapping from permanent handles
 *      (i.e. the ones allocated in C++) and temporary handles (i.e.
 *      the ones allocated in C, but passed through a C++ interface.
 *  We keep two dictionaries for this purpose.  The permanent dictionary
 *      stores those C++ objects that have been explicitly created by
 *      the developer.  The C++ constructor for the wrapper class will
 *      insert the mapping into the permanent dictionary and the C++
 *      destructor will remove it and possibly free up the associated
 *      Windows object.
 *  When a handle passes through a C++ interface that doesn't exist in
 *      the permanent dictionary, we allocate a temporary wrapping object
 *      and store that mapping into the temporary dictionary.
 *  At idle time the temporary wrapping objects are flushed (since you better
 *      not be holding onto something you didn't create).
 */

#include <stddef.h>     // for offsetof()

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

typedef WORD MAPTYPE;

// forward class declarations
#ifndef _AFXDLL
#ifdef AFX_CLASS_MODEL
class NEAR CHandleMap
#else
class CHandleMap
#endif
#else
class CHandleMap
#endif
{
private:    // implementation
	CMapWordToPtr m_permanentMap;
	CMapWordToPtr m_temporaryMap;
	CRuntimeClass*  m_pClass;
	size_t m_nOffset;       // offset of handles in the object
	int m_nHandles;         // 1 or 2 (for CDC)

// Constructors
public:
	CHandleMap(CRuntimeClass* pClass, size_t nOffset, int nHandles = 1);

// Operations
public:
	CObject* FromHandle(HANDLE h);
	void     DeleteTemp();

	void    SetPermanent(HANDLE h, CObject* permOb);
	void    RemoveHandle(HANDLE h);

	BOOL    LookupPermanent(HANDLE h, CObject*& pObject)
				{ return m_permanentMap.Lookup((MAPTYPE)h, (void*&)pObject); }
	BOOL    LookupTemporary(HANDLE h, CObject*& pObject)
				{ return m_temporaryMap.Lookup((MAPTYPE)h, (void*&)pObject); }
};

/////////////////////////////////////////////////////////////////////////////
// non-debug inline versions

#ifndef _DEBUG
inline void CHandleMap::SetPermanent(HANDLE h, CObject* permOb)
{
	m_permanentMap[(MAPTYPE)h] = permOb;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef AFX_CORE1_SEG
#pragma code_seg()
#endif
