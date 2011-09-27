// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"

#ifdef AFX_OLE5_SEG
#pragma code_seg(AFX_OLE5_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Helpers and main implementation for CCmdTarget::IDispatch

MEMBERID CCmdTarget::MemberIDFromName(AFX_DISPMAP FAR* pDispMap, LPCSTR lpszName)
{
	// search all maps and their base maps
	UINT nInherit = 0;
	while (pDispMap != NULL)
	{
		// search all entries in this map
		AFX_DISPMAP_ENTRY FAR* pEntry = pDispMap->lpEntries;
		UINT nIndex = 0;
		while (pEntry->nPropOffset != -1)
		{
			if (pEntry->vt != VT_MFCVALUE &&
				lstrcmpi(pEntry->szName, lpszName) == 0)
			{
				if (pEntry->lDispID == DISPID_UNKNOWN)
				{
					// the MEMBERID is combination of nIndex & nInherit
					ASSERT(MAKELONG(nIndex+1, nInherit) != DISPID_UNKNOWN);
					return MAKELONG(nIndex+1, nInherit);
				}
				// the MEMBERID is specified as the lDispID
				return pEntry->lDispID;
			}
			++pEntry;
			++nIndex;
		}
		pDispMap = pDispMap->lpBaseDispMap;
		++nInherit;
	}
	return DISPID_UNKNOWN;  // name not found
}

AFX_DISPMAP_ENTRY FAR* CCmdTarget::GetDispEntry(MEMBERID memid)
{
	AFX_DISPMAP FAR* pDerivMap = GetDispatchMap();
	AFX_DISPMAP FAR* pDispMap;
	AFX_DISPMAP_ENTRY FAR* pEntry;
	UINT nTest;

	if (memid == DISPID_VALUE)
	{
		// DISPID_VALUE is a special alias (look for special alias entry)
		pDispMap = pDerivMap;
		while (pDispMap != NULL)
		{
			// search for special entry with vt == VT_MFCVALUE
			pEntry = pDispMap->lpEntries;
			while (pEntry->nPropOffset != -1)
			{
				if (pEntry->vt == VT_MFCVALUE)
				{
					memid = pEntry->lDispID;
					if (memid == DISPID_UNKNOWN)
					{
						// attempt to map alias name to member ID
						memid = MemberIDFromName(pDerivMap, pEntry->szName);
						if (memid == DISPID_UNKNOWN)
							return NULL;
					}
					// break out and map the member ID to an entry
					goto LookupDispID;
				}
				++pEntry;
			}
			pDispMap = pDispMap->lpBaseDispMap;
		}
	}

LookupDispID:
	pEntry = NULL;
	if ((long)memid > 0)
	{
		// find AFX_DISPMAP corresponding to HIWORD(memid)
		nTest = 0;
		pDispMap = pDerivMap;
		while (pDispMap != NULL && nTest < (UINT)HIWORD(memid))
		{
			pDispMap = pDispMap->lpBaseDispMap;
			++nTest;
		}
		if (pDispMap != NULL)
		{
			// find AFX_DISPMAP_ENTRY corresponding to LOWORD(memid)
			nTest = 0;
			pEntry = pDispMap->lpEntries;
			while (pEntry->nPropOffset != -1 && nTest < (UINT)LOWORD(memid)-1)
			{
				++pEntry;
				++nTest;
			}
			if (pEntry->nPropOffset == -1)
				pEntry = NULL;
		}
	}
	if (pEntry == NULL)
	{
		// second pass, look for DISP_XXX_ID entries
		pDispMap = pDerivMap;
		while (pDispMap != NULL)
		{
			// find AFX_DISPMAP_ENTRY where (pEntry->lDispID == memid)
			pEntry = pDispMap->lpEntries;
			while (pEntry->nPropOffset != -1 && pEntry->lDispID != memid)
				++pEntry;
			if (pEntry->nPropOffset != -1)
				break;

			// try base class
			pEntry = NULL;
			pDispMap = pDispMap->lpBaseDispMap;
		}
	}

	// return matching entry
	return pEntry;
}

void CCmdTarget::GetStandardProp(AFX_DISPMAP_ENTRY FAR* pEntry,
	LPVOID lpvResult, UINT FAR* puArgErr)
{
	ASSERT(pEntry != NULL);
	ASSERT(*puArgErr != 0);
	VARIANTARG FAR* pvarResult = (VARIANTARG FAR*)lpvResult;

	// it is a DISPATCH_PROPERTYGET (for standard, non-function property)
	void* pProp = (BYTE*)this + pEntry->nPropOffset;
	if (pEntry->vt != VT_VARIANT)
		pvarResult->vt = pEntry->vt;
	switch (pEntry->vt)
	{
	case VT_I2:
		pvarResult->iVal = *(short*)pProp;
		break;
	case VT_I4:
		pvarResult->lVal = *(long*)pProp;
		break;
	case VT_R4:
		*(_AFXFLOAT FAR*)&pvarResult->fltVal = *(_AFXFLOAT*)pProp;
		break;
	case VT_R8:
		*(_AFXDOUBLE FAR*)&pvarResult->dblVal = *(_AFXDOUBLE*)pProp;
		break;
	case VT_DATE:
		*(_AFXDOUBLE FAR*)&pvarResult->date = *(_AFXDOUBLE*)pProp;
		break;
	case VT_CY:
		pvarResult->cyVal = *(CY*)pProp;
		break;
	case VT_BSTR:
		{
			CString* pString = (CString*)pProp;
			pvarResult->bstrVal =
				::SysAllocStringLen(*pString, pString->GetLength());
		}
		break;
	case VT_DISPATCH:
		pvarResult->pdispVal = *(LPDISPATCH*)pProp;
		if (pvarResult->pdispVal != NULL)
			pvarResult->pdispVal->AddRef();
		break;
	case VT_ERROR:
		pvarResult->scode = *(SCODE*)pProp;
		break;
	case VT_BOOL:
		pvarResult->bool = (*(BOOL*)pProp != 0 ? -1 : 0);
		break;
	case VT_VARIANT:
		if (VariantCopy(pvarResult, (LPVARIANTARG)pProp) != NOERROR)
			*puArgErr = 0;
		break;
	case VT_UNKNOWN:
		pvarResult->punkVal = *(LPDISPATCH*)pProp;
		if (pvarResult->punkVal != NULL)
			pvarResult->punkVal->AddRef();
		break;

	default:
		*puArgErr = 0;
	}
}

long CCmdTarget::SetStandardProp(AFX_DISPMAP_ENTRY FAR* pEntry,
	LPVOID lpvdispparams, UINT FAR* puArgErr)
{
	ASSERT(pEntry != NULL);
	ASSERT(*puArgErr != 0);
	DISPPARAMS FAR* pDispParams = (DISPPARAMS FAR*)lpvdispparams;

	// it is a DISPATCH_PROPERTYSET (for standard, non-function property)
	SCODE sc = S_OK;
	VARIANTARG va;
	VariantInit(&va);
	VARIANTARG FAR* pArg = &pDispParams->rgvarg[0];
	if (pEntry->vt != VT_VARIANT && pArg->vt != pEntry->vt)
	{
		// argument is not of appropriate type, attempt to coerce it
		sc = GetScode(VariantChangeType(&va, pArg, 0, pEntry->vt));
		if (FAILED(sc))
		{
			// argument could not be coerced
			*puArgErr = 0;
			return sc;
		}
		ASSERT(va.vt == pEntry->vt);
		pArg = &va;
	}

	void* pProp = (BYTE*)this + pEntry->nPropOffset;
	switch (pEntry->vt)
	{
	case VT_I2:
		*(short*)pProp = pArg->iVal;
		break;
	case VT_I4:
		*(long*)pProp = pArg->lVal;
		break;
	case VT_R4:
		*(_AFXFLOAT*)pProp = *(_AFXFLOAT FAR*)&pArg->fltVal;
		break;
	case VT_R8:
		*(_AFXDOUBLE*)pProp = *(_AFXDOUBLE FAR*)&pArg->dblVal;
		break;
	case VT_DATE:
		*(_AFXDOUBLE*)pProp = *(_AFXDOUBLE FAR*)&pArg->date;
		break;
	case VT_CY:
		*(CY*)pProp = pArg->cyVal;
		break;
	case VT_BSTR:
		{
			int nLen = ::SysStringLen(pArg->bstrVal);
			char* psz = ((CString*)pProp)->GetBufferSetLength(nLen);
			ASSERT(psz != NULL);
			_fmemcpy(psz, pArg->bstrVal, nLen);
		}
		break;
	case VT_DISPATCH:
		if (pArg->pdispVal != NULL)
			pArg->pdispVal->AddRef();
		_AfxRelease((LPUNKNOWN*)pProp);
		*(LPDISPATCH*)pProp = pArg->pdispVal;
		break;
	case VT_ERROR:
		*(SCODE*)pProp = pArg->scode;
		break;
	case VT_BOOL:
		*(BOOL*)pProp = (pArg->bool != 0);
		break;
	case VT_VARIANT:
		if (VariantCopy((LPVARIANTARG)pProp, pArg) != NOERROR)
			*puArgErr = 0;
		break;
	case VT_UNKNOWN:
		if (pArg->punkVal != NULL)
			pArg->punkVal->AddRef();
		_AfxRelease((LPUNKNOWN*)pProp);
		*(LPUNKNOWN*)pProp = pArg->punkVal;
		break;

	default:
		*puArgErr = 0;
	}
	VariantClear(&va);

	// if property was a DISP_PROPERTY_NOTIFY type, call pfnSet after setting
	if (pEntry->pfnSet != NULL)
		(this->*pEntry->pfnSet)();

	return sc;
}

/////////////////////////////////////////////////////////////////////////////
// Standard automation methods

void CCmdTarget::GetNotSupported()
{
	AfxThrowOleDispatchException(
		AFX_IDP_GET_NOT_SUPPORTED, AFX_IDP_GET_NOT_SUPPORTED);
}

void CCmdTarget::SetNotSupported()
{
	AfxThrowOleDispatchException(
		AFX_IDP_SET_NOT_SUPPORTED, AFX_IDP_SET_NOT_SUPPORTED);
}

/////////////////////////////////////////////////////////////////////////////
// Wiring to CCmdTarget

// enable this object for OLE automation, called from derived class ctor
void CCmdTarget::EnableAutomation()
{
	ASSERT_VALID(this);
	ASSERT(GetDispatchMap() != NULL);   // must have DECLARE_DISPATCH_MAP

	// construct an COleDispatchImpl instance just to get to the vtable
	COleDispatchImpl dispatch;

	// vtable pointer should be already set to same or NULL
	ASSERT(*(DWORD*)&dispatch == m_xDispatch || m_xDispatch == NULL);
	// sizeof(COleDispatchImpl) should be just a DWORD (far vtable pointer)
	ASSERT(sizeof(m_xDispatch) == sizeof(COleDispatchImpl));

	// copy the vtable to make sure it is initialized
	m_xDispatch = *(DWORD*)&dispatch;
}

// return addref'd IDispatch part of CCmdTarget object
LPDISPATCH CCmdTarget::GetIDispatch(BOOL bAddRef)
{
	ASSERT_VALID(this);
	ASSERT(m_xDispatch != 0);   // forgot to call EnableAutomation?

	// AddRef the object if requested
	if (bAddRef)
		InternalAddRef();

	// return pointer to IDispatch implementation
	return (LPDISPATCH)&m_xDispatch;
}

// retrieve CCmdTarget* from IDispatch* (return NULL if not MFC IDispatch)
CCmdTarget* CCmdTarget::FromIDispatch(LPDISPATCH lpDispatch)
{
	// construct an COleDispatchImpl instance just to get to the vtable
	COleDispatchImpl dispatch;

	ASSERT(*(DWORD FAR*)&dispatch != 0);    // null vtable ptr?
	if (*(DWORD FAR*)lpDispatch != *(DWORD*)&dispatch)
		return NULL;    // not our IDispatch*

	// vtable ptrs match, so must have originally been retrieved with
	//  CCmdTarget::GetIDispatch.
	CCmdTarget* pTarget = (CCmdTarget*)((BYTE*)
		_AfxGetPtrFromFarPtr(lpDispatch) - offsetof(CCmdTarget, m_xDispatch));
	ASSERT_VALID(pTarget);
	return pTarget;
}

void COleDispatchImpl::Disconnect()
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	ASSERT_VALID(pThis);

	pThis->ExternalDisconnect();    // always disconnect the object
}

///////////////////////////////////////////////////////////////////////////////
// OLE 2.0 BSTR support

BSTR CString::AllocSysString()
{
	BSTR bstr = ::SysAllocStringLen(m_pchData, m_nDataLength);
	if (bstr == NULL)
		AfxThrowMemoryException();

	return bstr;
}

BSTR CString::SetSysString(BSTR FAR* pbstr)
{
	ASSERT(AfxIsValidAddress(pbstr, sizeof(BSTR)));

	if (!::SysReAllocStringLen(pbstr, m_pchData, m_nDataLength))
		AfxThrowMemoryException();

	ASSERT(*pbstr != NULL);
	return *pbstr;
}

/////////////////////////////////////////////////////////////////////////////
// Specifics of METHOD->C++ member function invocation
//  (very compiler/platform specific)

// calculate size of pushed arguments + retval reference
static UINT NEAR PASCAL _AfxDispatchSizeArgs(
	BYTE FAR* pbParams, VARTYPE vtRet, BOOL bIncludeOverhead)
{
	// size of arguments on stack when pushed by value
	static UINT BASED_CODE rgnByValue[] =
	{
		0,                          // PT_EMPTY
		0,                          // PT_NULL
		sizeof(WORD),               // PT_SHORT
		sizeof(DWORD),              // PT_LONG
		sizeof(float),              // PT_FLOAT
		sizeof(double),             // PT_DOUBLE
		sizeof(CY),                 // PT_CY
		sizeof(DATE),               // PT_DATE
		sizeof(LPCSTR),             // PT_LPCSTR
		sizeof(LPDISPATCH),         // PT_DISPATCH
		sizeof(SCODE),              // PT_SCODE
		sizeof(BOOL),               // PT_BOOL
		sizeof(const VARIANTARG FAR*),  // PT_VARIANT
		sizeof(LPUNKNOWN)           // PT_UNKNOWN
	};
	// size of arguments on stack when pushed by reference
	static UINT BASED_CODE rgnByRef[] =
	{
		0,                          // PT_PEMPTY
		0,                          // PT_PNULL
		sizeof(WORD FAR*),          // PT_PSHORT
		sizeof(DWORD FAR*),         // PT_PLONG
		sizeof(float FAR*),         // PT_PFLOAT
		sizeof(double FAR*),        // PT_PDOUBLE
		sizeof(CY FAR*),            // PT_PCY
		sizeof(DATE FAR*),          // PT_PDATE
		sizeof(BSTR FAR*),          // PT_PBSTR
		sizeof(LPDISPATCH FAR*),    // PT_PDISPATCH
		sizeof(SCODE FAR*),         // PT_PSCODE
		sizeof(BOOL FAR*),          // PT_PBOOL
		sizeof(VARIANTARG FAR*),    // PT_PVARIANT
		sizeof(LPUNKNOWN FAR*)      // PT_PUNKNOWN
	};
	static UINT BASED_CODE rgnRetVal[] =
	{
		0,                          // RT_EMPTY
		0,                          // RT_NULL
		0,                          // RT_I2
		0,                          // RT_I4
		sizeof(float),              // RT_R4
		sizeof(double),             // RT_R8
		sizeof(CY*),                // RT_CY
		sizeof(DATE),               // RT_DATE (same as VT_R8)
		0,                          // RT_BSTR
		0,                          // RT_DISPATCH
		0,                          // RT_ERROR
		0,                          // RT_BOOL
		sizeof(VARIANTARG*),        // RT_VARIANT
		0                           // RT_UNKNOWN
	};

	UINT nCount = 0;
	if (bIncludeOverhead)
	{
		nCount += sizeof(CCmdTarget*);  // sizeof 'this' pointer

		// count bytes in return value
		ASSERT((UINT)vtRet < sizeof(rgnRetVal)/sizeof(rgnRetVal[0]));
		nCount += rgnRetVal[vtRet];
	}

	// count arguments
	ASSERT(pbParams != NULL);
	while (*pbParams != 0)
	{
		UINT BASED_CODE* rgnBytes;
		if (*pbParams != VT_MFCMARKER)
		{
			if (*pbParams & VT_MFCBYREF)
				rgnBytes = rgnByRef;
			else
				rgnBytes = rgnByValue;
			ASSERT((*pbParams & ~VT_MFCBYREF) <
				sizeof(rgnByValue)/sizeof(rgnByValue[0]));
			nCount += rgnBytes[*pbParams & ~VT_MFCBYREF];
		}
		++pbParams;
	}
	return nCount;
}

// special return value struct/buffer, union of all return values that are
//  passed as a reference parameter.  The references must be passed as
//  ambient, but they come as FAR from IDispatch interface.
union AFX_VARRETVAL
{
	VARIANTARG vaVal;
	CY cyVal;
	float fltVal;
	double dblVal;
};

// push arguments on stack appropriate for C++ call (compiler dependent)
static SCODE NEAR PASCAL _AfxDispatchPushArgs(
	BYTE BASED_STACK* pStack, BYTE FAR* pbParams,
	AFX_VARRETVAL BASED_STACK* retVal, VARTYPE vtRet, CCmdTarget* pTarget,
	DISPPARAMS FAR* pDispParams, UINT FAR* puArgErr,
	LPVARIANT rgTempVars)
{
	ASSERT(pStack != NULL);
	ASSERT(retVal != NULL);
	ASSERT(pTarget != NULL);
	ASSERT(pDispParams != NULL);
	ASSERT(puArgErr != NULL);

	// push the 'this' pointer
	*(CCmdTarget* BASED_STACK*)pStack = pTarget;
	pStack += sizeof(CCmdTarget*);

	// push any necessary return value temporaries on the stack (pre args)
	//  (necessary for float values)
	if (vtRet == VT_R4 || vtRet == VT_R8 || vtRet == VT_DATE)
	{
		// floating point calling sequence (SS relative pointer is pushed)
		*(void BASED_STACK* BASED_STACK*)pStack = retVal;
		pStack += sizeof(void BASED_STACK*);
	}

	// we need to start at the end of the argument list
	//  (this is because C++ member functions push arguments first to last,
	//   but the stack grows down on Intel machines)
	pStack += _AfxDispatchSizeArgs(pbParams, VT_EMPTY, FALSE);
	BYTE BASED_STACK* pStackSave = pStack;

	// push the arguments
	VARIANTARG FAR* pArgs = pDispParams->rgvarg;
	BOOL bNamedArgs = FALSE;
	int iArg = pDispParams->cArgs; // start with positional arguments
	int iArgMin = pDispParams->cNamedArgs;

	ASSERT(pbParams != NULL);
	for (BYTE FAR* pb = pbParams; *pb != '\0'; ++pb)
	{
		--iArg; // move to next arg

		// convert MFC parameter type to IDispatch VARTYPE
		VARTYPE vt = *pb;
		if (vt != VT_MFCMARKER && (vt & VT_MFCBYREF))
			vt = (vt & ~VT_MFCBYREF) | VT_BYREF;

		VARIANTARG FAR* pArg;
		if (iArg >= iArgMin)
		{
			// hit named args before using all positional args?
			if (vt == VT_MFCMARKER)
				break;

			// argument specified by caller -- use it
			pArg = &pArgs[iArg];
			if (vt != VT_VARIANT && vt != pArg->vt)
			{
				// argument is not of appropriate type, attempt to coerce it
				LPVARIANT pArgTemp = &rgTempVars[iArg];
				ASSERT(pArgTemp->vt == VT_EMPTY);
				SCODE sc = GetScode(VariantChangeType(pArgTemp, pArg, 0, vt));
				if (FAILED(sc))
				{
					// argument could not be coerced
					*puArgErr = iArg;
					return sc;
				}
				ASSERT(pArgTemp->vt == vt);
				pArg = pArgTemp;
			}
		}
		else
		{
			if (vt == VT_MFCMARKER)
			{
				// start processing named arguments
				iArg = pDispParams->cNamedArgs;
				iArgMin = 0;
				bNamedArgs = TRUE;
				continue;
			}

			if (bNamedArgs || vt != VT_VARIANT)
				break;  // function not expecting optional argument

			// argument not specified by caller -- provide default variant
			static VARIANT NEAR vaDefault;
			vaDefault.vt = VT_ERROR;
			vaDefault.scode = DISP_E_PARAMNOTFOUND;
			pArg = &vaDefault;
		}

		// push argument value on the stack
		switch (vt)
		{
		// by value parameters
		case VT_I2:
			pStack -= sizeof(short);
			*(short BASED_STACK*)pStack = pArg->iVal;
			break;
		case VT_I4:
			pStack -= sizeof(long);
			*(long BASED_STACK*)pStack = pArg->lVal;
			break;
		case VT_R4:
			pStack -= sizeof(_AFXFLOAT);
			*(_AFXFLOAT BASED_STACK*)pStack = *(_AFXFLOAT FAR*)&pArg->fltVal;
			break;
		case VT_R8:
			pStack -= sizeof(_AFXDOUBLE);
			*(_AFXDOUBLE BASED_STACK*)pStack = *(_AFXDOUBLE FAR*)&pArg->dblVal;
			break;
		case VT_DATE:
			pStack -= sizeof(_AFXDOUBLE);
			*(_AFXDOUBLE BASED_STACK*)pStack = *(_AFXDOUBLE FAR*)&pArg->date;
			break;
		case VT_CY:
			pStack -= sizeof(CY);
			*(CY BASED_STACK*)pStack = pArg->cyVal;
			break;
		case VT_BSTR:
			pStack -= sizeof(LPCSTR);
			*(LPCSTR BASED_STACK*)pStack = pArg->bstrVal;
			break;
		case VT_DISPATCH:
			pStack -= sizeof(LPDISPATCH);
			*(LPDISPATCH BASED_STACK*)pStack = pArg->pdispVal;
			break;
		case VT_ERROR:
			pStack -= sizeof(SCODE);
			*(SCODE BASED_STACK*)pStack = pArg->scode;
			break;
		case VT_BOOL:
			pStack -= sizeof(BOOL);
			*(BOOL BASED_STACK*)pStack = (pArg->bool != 0);
			break;
		case VT_VARIANT:
			pStack -= sizeof(VARIANTARG FAR*);
			*(VARIANTARG FAR* BASED_STACK*)pStack = pArg;
			break;
		case VT_UNKNOWN:
			pStack -= sizeof(LPUNKNOWN);
			*(LPUNKNOWN BASED_STACK*)pStack = pArg->punkVal;
			break;

		// by reference parameters
		case VT_I2|VT_BYREF:
			pStack -= sizeof(short FAR*);
			*(short FAR* BASED_STACK*)pStack = pArg->piVal;
			break;
		case VT_I4|VT_BYREF:
			pStack -= sizeof(long FAR*);
			*(long FAR* BASED_STACK*)pStack = pArg->plVal;
			break;
		case VT_R4|VT_BYREF:
			pStack -= sizeof(float FAR*);
			*(float FAR* BASED_STACK*)pStack = pArg->pfltVal;
			break;
		case VT_R8|VT_BYREF:
			pStack -= sizeof(double FAR*);
			*(double FAR* BASED_STACK*)pStack = pArg->pdblVal;
			break;
		case VT_DATE|VT_BYREF:
			pStack -= sizeof(DATE FAR*);
			*(DATE FAR* BASED_STACK*)pStack = pArg->pdate;
			break;
		case VT_CY|VT_BYREF:
			pStack -= sizeof(CY FAR*);
			*(CY FAR* BASED_STACK*)pStack = pArg->pcyVal;
			break;
		case VT_BSTR|VT_BYREF:
			pStack -= sizeof(BSTR FAR*);
			*(BSTR FAR* BASED_STACK*)pStack = pArg->pbstrVal;
			break;
		case VT_DISPATCH|VT_BYREF:
			pStack -= sizeof(LPDISPATCH FAR*);
			*(LPDISPATCH FAR* BASED_STACK*)pStack = pArg->ppdispVal;
			break;
		case VT_ERROR|VT_BYREF:
			pStack -= sizeof(SCODE FAR*);
			*(SCODE FAR* BASED_STACK*)pStack = pArg->pscode;
			break;
		case VT_BOOL|VT_BYREF:
			pStack -= sizeof(VARIANT_BOOL FAR*);
			*(VARIANT_BOOL FAR* BASED_STACK*)pStack = pArg->pbool;
			break;
		case VT_VARIANT|VT_BYREF:
			pStack -= sizeof(VARIANTARG FAR*);
			*(VARIANTARG FAR* BASED_STACK*)pStack = pArg->pvarVal;
			break;
		case VT_UNKNOWN|VT_BYREF:
			pStack -= sizeof(LPUNKNOWN FAR*);
			*(LPUNKNOWN FAR* BASED_STACK*)pStack = pArg->ppunkVal;
			break;

		default:
			ASSERT(FALSE);
		}
	}

	pStack = pStackSave;    // restore stack to end of arguments

	// check that all source arguments were consumed
	if (iArg > 0)
	{
		*puArgErr = iArg;
		return DISP_E_BADPARAMCOUNT;
	}

	// check that all target arguments were filled
	if (*pb != '\0')
	{
		*puArgErr = pDispParams->cArgs;
		return DISP_E_PARAMNOTOPTIONAL;
	}

	// push any necessary return value stuff on the stack (post args)
	//  (an ambient pointer is pushed to stack relative data)
	if (vtRet == VT_CY || vtRet == VT_VARIANT)
	{
		*(void* BASED_STACK*)pStack = (void*)retVal;
		pStack += sizeof(void*);
	}
	return S_OK;
}

// indirect call helpers (see OLECALL_.ASM for implementation)
extern "C" DWORD PASCAL _AfxDispatchCall(AFX_PMSG pfn,
	void BASED_STACK* pArgs, UINT nSizeArgs);

extern "C" BYTE BASED_STACK* PASCAL _AfxStackAlloc(size_t nSize,
	BYTE BASED_STACK* BASED_STACK* ppOldStack);

extern "C" void PASCAL _AfxStackRestore(BYTE BASED_STACK* pOldStack);

// invoke standard method given IDispatch parameters/return value, etc.
long CCmdTarget::InvokeHelper(AFX_DISPMAP_ENTRY FAR* pEntry, WORD wFlags,
	LPVOID lpvResult, LPVOID lpvdispparams, UINT FAR* puArgErr)
{
	ASSERT(pEntry != NULL);
	ASSERT(pEntry->pfn != NULL);
	VARIANTARG FAR* pvarResult = (VARIANTARG FAR*)lpvResult;
	DISPPARAMS FAR* pDispParams = (DISPPARAMS FAR*)lpvdispparams;

	// get default function and parameters
	BYTE FAR* pbParams = pEntry->pbParams;
	AFX_PMSG pfn = pEntry->pfn;
	VARTYPE vtRet = pEntry->vt;

	// make DISPATCH_PROPERTYPUT look like call with one extra parameter
	BYTE rgbPropSetParams[sizeof(pEntry->pbParams)+2];
	if (wFlags & (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF))
	{
		ASSERT(!(pEntry->vt & VT_BYREF));
		int nLen = lstrlen((LPCSTR)pEntry->pbParams);
		lstrcpy((LPSTR)rgbPropSetParams, (LPCSTR)pEntry->pbParams);
		rgbPropSetParams[nLen++] = (BYTE)VT_MFCMARKER;
			// VT_MFCVALUE serves serves as marker denoting start of named params
		rgbPropSetParams[nLen++] = (BYTE)pEntry->vt;
		rgbPropSetParams[nLen] = 0;
		pbParams = rgbPropSetParams;

		if (pEntry->pfnSet != NULL)
		{
			pfn = pEntry->pfnSet;   // call "set" function instead of "get"
			vtRet = VT_EMPTY;
		}
	}

	// allocate temporary space for VARIANT temps created by VariantChangeType
	VARIANT BASED_STACK* rgTempVars = (VARIANT BASED_STACK*)
		_AfxStackAlloc(pDispParams->cArgs * sizeof(VARIANT), NULL);
	if (rgTempVars == NULL)
	{
		TRACE0("Error: stack overflow in IDispatch::Invoke!\n");
		AfxThrowMemoryException();
	}
	_fmemset(rgTempVars, 0, pDispParams->cArgs * sizeof(VARIANT));

	// allocate memory on the stack for calling arguments
	UINT nSizeArgs = _AfxDispatchSizeArgs(pbParams, vtRet, TRUE);
	ASSERT(nSizeArgs != 0);
	BYTE BASED_STACK* pOldStack;
	BYTE BASED_STACK* pStack = _AfxStackAlloc(nSizeArgs,
		(BYTE BASED_STACK* BASED_STACK*)&pOldStack);
	if (pStack == NULL)
	{
		TRACE0("Error: stack overflow in IDispatch::Invoke!\n");
		AfxThrowMemoryException();
	}

	// push all the args on to the stack allocated memory
	AFX_VARRETVAL retVal;
	SCODE sc = _AfxDispatchPushArgs(pStack, pbParams,
		(AFX_VARRETVAL BASED_STACK*)LOWORD((DWORD)(LPVOID)&retVal), vtRet,
		this, pDispParams, puArgErr, rgTempVars);

	// cleanup on failure (*puArgErr has argument in error)
	if (sc != S_OK)
	{
		_AfxStackRestore(pOldStack);
		return (long)sc;
	}

	DWORD dwRet;
	if (sc == S_OK)
	{
		TRY
		{
			dwRet = _AfxDispatchCall(pfn, pStack, nSizeArgs);
			// call the member function with the stack-pointer pointing to pArgs
			//	(some implementations may have to copy pArgs to the stack)
		}
		CATCH_ALL(e)
		{
			// free temporaries created by VariantChangeType
			for (UINT iArg = 0; iArg < pDispParams->cArgs; ++iArg)
				VariantClear(&rgTempVars[iArg]);

			THROW_LAST();
		}
		END_CATCH_ALL
	}

	// free temporaries created by VariantChangeType
	for (UINT iArg = 0; iArg < pDispParams->cArgs; ++iArg)
		VariantClear(&rgTempVars[iArg]);

	// cleanup on failure (*puArgErr has argument in error)
	if (sc != S_OK)
	{
		_AfxStackRestore(pOldStack);
		return (long)sc;
	}

	// property puts don't touch the return value
	if (pvarResult != NULL)
	{
		// build return value VARIANTARG from dwRet
		//  (Note: some types return result in dwRet, the rest are in retVal)
		VariantClear(pvarResult);
		if (vtRet != VT_VARIANT)
			pvarResult->vt = vtRet;

		switch (vtRet)
		{
		case VT_I2:
			pvarResult->iVal = (short)dwRet;
			break;
		case VT_I4:
			pvarResult->lVal = (long)dwRet;
			break;
		case VT_R4:
			*(_AFXFLOAT FAR*)&pvarResult->fltVal = *(_AFXFLOAT*)dwRet;
			break;
		case VT_R8:
			*(_AFXDOUBLE FAR*)&pvarResult->dblVal = *(_AFXDOUBLE*)dwRet;
			break;
		case VT_CY:
			pvarResult->cyVal = retVal.cyVal;
			break;
		case VT_DATE:
			*(_AFXDOUBLE FAR*)&pvarResult->date = *(_AFXDOUBLE*)dwRet;
			break;
		case VT_BSTR:
			pvarResult->bstrVal = (BSTR)dwRet;
			break;
		case VT_DISPATCH:
			pvarResult->pdispVal = (LPDISPATCH)dwRet;   // already AddRef
			break;
		case VT_ERROR:
			pvarResult->scode = (SCODE)dwRet;
			break;
		case VT_BOOL:
			pvarResult->bool = ((BOOL)dwRet != 0 ? -1 : 0);
			break;
		case VT_VARIANT:
			*pvarResult = retVal.vaVal;
			break;
		case VT_UNKNOWN:
			pvarResult->punkVal = (LPUNKNOWN)dwRet; // already AddRef
			break;
		}
	}
	else
	{
		// free unused return value
		switch (vtRet)
		{
		case VT_BSTR:
			if ((BSTR)dwRet != NULL)
				SysFreeString((BSTR)dwRet);
			break;
		case VT_DISPATCH:
		case VT_UNKNOWN:
			if ((LPUNKNOWN)dwRet != 0)
				((LPUNKNOWN)dwRet)->Release();
			break;
		case VT_VARIANT:
			VariantClear(&retVal.vaVal);
			break;
		}
	}

	// restore stack before returning
	_AfxStackRestore(pOldStack);
	return (long)S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget::XDispatch implementation

STDMETHODIMP_(ULONG) COleDispatchImpl::AddRef()
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDispatchImpl::Release()
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleDispatchImpl::QueryInterface(REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleDispatchImpl::GetTypeInfoCount(UINT FAR* pctinfo)
{
	*pctinfo = 0;
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP COleDispatchImpl::GetTypeInfo(UINT itinfo, LCID lcid,
	ITypeInfo FAR* FAR* pptinfo)
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	ASSERT_VALID(pThis);

	*pptinfo = NULL;
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP COleDispatchImpl::GetIDsOfNames(
	REFIID riid, char FAR* FAR* rgszNames, UINT cNames, LCID lcid,
	DISPID FAR* rgdispid)
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	ASSERT_VALID(pThis);

	SCODE sc = S_OK;

	// check arguments
	if (riid != IID_NULL)
		return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

	// fill in the member name
	AFX_DISPMAP FAR* pDerivMap = pThis->GetDispatchMap();
	rgdispid[0] = pThis->MemberIDFromName(pDerivMap, rgszNames[0]);
	if (rgdispid[0] == DISPID_UNKNOWN)
		sc = DISP_E_UNKNOWNNAME;

	// argument names are always DISPID_UNKNOWN (for this implementation)
	for (UINT nIndex = 1; nIndex < cNames; nIndex++)
		rgdispid[nIndex] = DISPID_UNKNOWN;

	return ResultFromScode(sc);
}

STDMETHODIMP COleDispatchImpl::Invoke(
	DISPID dispid, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* pvarResult,
	EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr)
{
	METHOD_PROLOGUE(CCmdTarget, Dispatch)
	ASSERT_VALID(pThis);

	// check arguments
	if (riid != IID_NULL)
		return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

	// copy param block for safety
	DISPPARAMS params = *pDispParams;
	pDispParams = &params;

	// most of the time, named arguments are not supported
	if (pDispParams->cNamedArgs != 0)
	{
		// only special PROPERTYPUT named argument is allowed
		if (pDispParams->cNamedArgs != 1 ||
			pDispParams->rgdispidNamedArgs[0] != DISPID_PROPERTYPUT)
		{
			return ResultFromScode(DISP_E_NONAMEDARGS);
		}
	}

	// get entry for the member ID
	AFX_DISPMAP_ENTRY FAR* pEntry = pThis->GetDispEntry(dispid);
	if (pEntry == NULL)
		return ResultFromScode(DISP_E_MEMBERNOTFOUND);

	// treat member calls on properties just like property get/set
	if (((wFlags & (DISPATCH_PROPERTYGET|DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF|
		DISPATCH_METHOD)) == DISPATCH_METHOD) &&
		((pEntry->pfn == NULL && pEntry->pfnSet == NULL) ||
		 (pEntry->pfn != NULL && pEntry->pfnSet != NULL)))
	{
		// the entry describes a property but a method call is being
		//  attempted -- change it to a property get/set based on the
		//  number of parameters being passed.
		wFlags &= ~DISPATCH_METHOD;
		UINT nExpectedArgs = (UINT)lstrlen((LPCSTR)pEntry->pbParams);
		if (pDispParams->cArgs <= nExpectedArgs)
		{
			// no extra param -- so treat as property get
			wFlags |= DISPATCH_PROPERTYGET;
		}
		else
		{
			// extra params -- treat as property set
			wFlags |= DISPATCH_PROPERTYPUTREF;
			pDispParams->cNamedArgs = 1;
		}
	}

	// property puts should not require a return value
	if (wFlags & (DISPATCH_PROPERTYPUTREF|DISPATCH_PROPERTYPUT))
	{
		pvarResult = NULL;
		// catch attempt to do property set on method
		if (pEntry->pfn != NULL && pEntry->pfnSet == NULL)
			return ResultFromScode(DISP_E_TYPEMISMATCH);
	}

	UINT uArgErr = -1;  // no error yet
	SCODE sc = S_OK;

	// handle special cases of DISPATCH_PROPERTYPUT
	VARIANT FAR* pvarParamSave = NULL;
	VARIANT vaParamSave;
	DISPPARAMS paramsTemp;
	VARIANT vaTemp;
	VariantInit(&vaTemp);

	if (wFlags == DISPATCH_PROPERTYPUT && dispid != DISPID_VALUE)
	{
		// with PROPERTYPUT (no REF), the right hand side may need fixup
		if (pDispParams->rgvarg[0].vt == VT_DISPATCH &&
			pDispParams->rgvarg[0].pdispVal != NULL)
		{
			// remember old value for restore later
			pvarParamSave = &pDispParams->rgvarg[0];
			vaParamSave = pDispParams->rgvarg[0];
			VariantInit(&pDispParams->rgvarg[0]);

			// get default value of right hand side
			_fmemset(&paramsTemp, 0, sizeof(DISPPARAMS));
			sc = GetScode(pDispParams->rgvarg[0].pdispVal->Invoke(
				DISPID_VALUE, riid, lcid, DISPATCH_PROPERTYGET, &paramsTemp,
				&pDispParams->rgvarg[0], pexcepinfo, puArgErr));
		}

		// special handling for PROPERTYPUT (no REF), left hand side
		if (sc == S_OK && pEntry->vt == VT_DISPATCH)
		{
			_fmemset(&paramsTemp, 0, sizeof(DISPPARAMS));

			// parameters are distributed depending on what the Get expects
			if (pEntry->pbParams[0] == 0)
			{
				// paramsTemp is already setup for no parameters
				sc = GetScode(Invoke(dispid, riid, lcid,
					DISPATCH_PROPERTYGET|DISPATCH_METHOD, &paramsTemp,
					&vaTemp, pexcepinfo, puArgErr));
				if (sc == S_OK &&
					(vaTemp.vt != VT_DISPATCH || vaTemp.pdispVal == NULL))
					sc = DISP_E_TYPEMISMATCH;
				else if (sc == NOERROR)
				{
					ASSERT(vaTemp.vt == VT_DISPATCH && vaTemp.pdispVal != NULL);
					// we have the result, now call put on the default property
					sc = GetScode(vaTemp.pdispVal->Invoke(
						DISPID_VALUE, riid, lcid, wFlags, pDispParams,
						pvarResult, pexcepinfo, puArgErr));
				}
			}
			else
			{
				// pass all but named params
				paramsTemp.rgvarg = &pDispParams->rgvarg[1];
				paramsTemp.cArgs = pDispParams->cArgs - 1;
				sc = GetScode(Invoke(dispid, riid, lcid,
					DISPATCH_PROPERTYGET|DISPATCH_METHOD, &paramsTemp,
					&vaTemp, pexcepinfo, puArgErr));
				if (sc == S_OK &&
					(vaTemp.vt != VT_DISPATCH || vaTemp.pdispVal == NULL))
					sc = DISP_E_TYPEMISMATCH;
				else if (sc == NOERROR)
				{
					ASSERT(vaTemp.vt == VT_DISPATCH && vaTemp.pdispVal != NULL);
					// we have the result, now call put on the default property
					paramsTemp = *pDispParams;
					paramsTemp.cArgs = paramsTemp.cNamedArgs;
					sc = GetScode(vaTemp.pdispVal->Invoke(
						DISPID_VALUE, riid, lcid, wFlags, &paramsTemp,
						pvarResult, pexcepinfo, puArgErr));
				}
			}
			VariantClear(&vaTemp);

			if (sc != DISP_E_MEMBERNOTFOUND)
				goto Cleanup;
		}

		if (sc != NOERROR && sc != DISP_E_MEMBERNOTFOUND)
			goto Cleanup;
	}

	// ignore DISP_E_MEMBERNOTFOUND from above
	ASSERT(sc == DISP_E_MEMBERNOTFOUND || sc == NOERROR);

	// undo implied default value on right hand side on error
	if (sc != S_OK && pvarParamSave != NULL)
	{
		// default value stuff failed -- so try without default value
		pvarParamSave = NULL;
		VariantClear(&pDispParams->rgvarg[0]);
		pDispParams->rgvarg[0] = vaParamSave;
	}
	sc = S_OK;

	// check arguments against this entry
	UINT nOrigArgs; nOrigArgs = pDispParams->cArgs;
	if (wFlags & (DISPATCH_PROPERTYGET|DISPATCH_METHOD))
	{
		if (!(wFlags & DISPATCH_METHOD))
		{
			if (pEntry->vt == VT_EMPTY)
				return ResultFromScode(DISP_E_BADPARAMCOUNT);
			if (pvarResult == NULL)
				return ResultFromScode(DISP_E_PARAMNOTOPTIONAL);
		}
		if (pEntry->pbParams[0] == 0 && pDispParams->cArgs > 0)
		{
			if (pEntry->vt != VT_DISPATCH)
				return ResultFromScode(DISP_E_BADPARAMCOUNT);

			// it is VT_DISPATCH property/method but too many arguments
			// supplied -- transfer those arguments to the default property of
			// the return value after getting the return value from this call.
			// This is referred to as collection lookup.
			pDispParams->cArgs = 0;
			if (pvarResult == NULL)
				pvarResult = &vaTemp;
		}
	}
	
	// make sure that parameters are not passed to a simple property
	if (pDispParams->cArgs > 1 &&
		(wFlags & (DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF)) &&
		pEntry->pfn == NULL)
	{
		sc = DISP_E_BADPARAMCOUNT;
		goto Cleanup;
	}

	// make sure that pvarResult is set for simple property get/set
	if (pEntry->pfn == NULL && pDispParams->cArgs == 0 && pvarResult == NULL)
	{
		sc = DISP_E_PARAMNOTOPTIONAL;
		goto Cleanup;
	}
	
	TRY
	{
		if (pEntry->pfn == NULL)
		{
			// do standard property get/set
			if (pDispParams->cArgs == 0)
				pThis->GetStandardProp(pEntry, pvarResult, &uArgErr);
			else
				sc = pThis->SetStandardProp(pEntry, pDispParams, &uArgErr);
		}
		else
		{
			// do standard method call
			sc = (SCODE)pThis->InvokeHelper(pEntry, wFlags,
				pvarResult, pDispParams, &uArgErr);
		}
	}
	CATCH_ALL(e)
	{
		if (pexcepinfo != NULL)
		{
			// fill exception with translation of MFC exception
			COleDispatchException::Process(pexcepinfo, e);
		}
		return ResultFromScode(DISP_E_EXCEPTION);
	}
	END_CATCH_ALL

	// handle special DISPATCH_PROPERTYGET collection lookup case
	if (sc == NOERROR && nOrigArgs > pDispParams->cArgs)
	{
		ASSERT(wFlags & (DISPATCH_PROPERTYGET|DISPATCH_METHOD));
		ASSERT(pvarResult != NULL);
		// must be non-NULL dispatch, otherwise type mismatch
		if (pvarResult->vt != VT_DISPATCH || pvarResult->pdispVal == NULL)
		{
			sc = DISP_E_TYPEMISMATCH;
			goto Cleanup;
		}
		// otherwise, valid VT_DISPATCH was returned
		pDispParams->cArgs = nOrigArgs;
		LPDISPATCH lpTemp = pvarResult->pdispVal;
		if (pvarResult != &vaTemp)
			VariantInit(pvarResult);
		else
			pvarResult = NULL;
		sc = GetScode(lpTemp->Invoke(DISPID_VALUE, riid, lcid, wFlags,
			pDispParams, pvarResult, pexcepinfo, puArgErr));
		lpTemp->Release();
	}

Cleanup:
	// restore any arguments which were modified
	if (pvarParamSave != NULL)
	{
		VariantClear(&pDispParams->rgvarg[0]);
		pDispParams->rgvarg[0] = vaParamSave;
	}

	// fill error argument if one is available
	if (sc != S_OK && puArgErr != NULL && uArgErr != -1)
		*puArgErr = uArgErr;

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
// IDispatch specific exception

IMPLEMENT_DYNAMIC(COleDispatchException, CException)

void PASCAL COleDispatchException::Process(
	EXCEPINFO FAR* pInfo, const CException* pAnyException)
{
	ASSERT(AfxIsValidAddress(pInfo, sizeof(EXCEPINFO)));
	ASSERT_VALID(pAnyException);

	// zero default & reserved members
	_fmemset(pInfo, 0, sizeof(EXCEPINFO));

	TCHAR szDescription[256];
	LPCTSTR pszDescription = szDescription;
	if (pAnyException->IsKindOf(RUNTIME_CLASS(COleDispatchException)))
	{
		// specific IDispatch style exception
		COleDispatchException* e = (COleDispatchException*)pAnyException;
		pszDescription = e->m_strDescription;
		pInfo->wCode = e->m_wCode;
		pInfo->dwHelpContext = e->m_dwHelpContext;
		pInfo->scode = e->m_scError;
	}
	else if (pAnyException->IsKindOf(RUNTIME_CLASS(CMemoryException)))
	{
		// failed memory allocation
		_AfxLoadString(AFX_IDP_FAILED_MEMORY_ALLOC, szDescription);
		pInfo->wCode = AFX_IDP_FAILED_MEMORY_ALLOC;
	}
	else
	{
		// other unknown/uncommon error
		_AfxLoadString(AFX_IDP_INTERNAL_FAILURE, szDescription);
		pInfo->wCode = AFX_IDP_INTERNAL_FAILURE;
	}

	// build up rest of EXCEPINFO struct
	pInfo->bstrSource = ::SysAllocString((LPSTR)AfxGetAppName());
	pInfo->bstrDescription = ::SysAllocString(pszDescription);
	if (pInfo->dwHelpContext != 0)
	{
		pInfo->bstrHelpFile =
			::SysAllocString((LPSTR)AfxGetApp()->m_pszHelpFilePath);
	}
}

COleDispatchException::COleDispatchException(
	LPCSTR lpszDescription, UINT nHelpID, WORD wCode)
{
	m_dwHelpContext = nHelpID != 0 ? HID_BASE_DISPATCH+nHelpID : 0;
	m_wCode = wCode;
	if (lpszDescription != NULL)
		m_strDescription = lpszDescription;
	m_scError = E_UNEXPECTED;
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, LPCSTR lpszDescription,
	UINT nHelpID)
{
	ASSERT(AfxIsValidString(lpszDescription));
	THROW(new COleDispatchException(lpszDescription, nHelpID, wCode));
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID,
	UINT nHelpID)
{
	char szBuffer[256];
	_AfxLoadString(nDescriptionID, szBuffer);
	if (nHelpID == -1)
		nHelpID = nDescriptionID;
	THROW(new COleDispatchException(szBuffer, nHelpID, wCode));
}

/////////////////////////////////////////////////////////////////////////////
