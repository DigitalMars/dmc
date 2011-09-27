// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __AFXCOLL_H__
#define __AFXCOLL_H__

#ifndef __AFX_H__
	#include <afx.h>
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file

//CObject
	// Arrays
	class CByteArray;           // array of BYTE
	class CWordArray;           // array of WORD
	class CDWordArray;          // array of DWORD
	class CUIntArray;           // array of UINT
	class CPtrArray;            // array of void*
	class CObArray;             // array of CObject*

	// Lists
	class CPtrList;             // list of void*
	class CObList;              // list of CObject*

	// Maps (aka Dictionaries)
	class CMapWordToOb;         // map from WORD to CObject*
	class CMapWordToPtr;        // map from WORD to void*
	class CMapPtrToWord;        // map from void* to WORD
	class CMapPtrToPtr;         // map from void* to void*

	// Special String variants
	class CStringArray;         // array of CStrings
	class CStringList;          // list of CStrings
	class CMapStringToPtr;      // map from CString to void*
	class CMapStringToOb;       // map from CString to CObject*
	class CMapStringToString;   // map from CString to CString

/////////////////////////////////////////////////////////////////////////////

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA
