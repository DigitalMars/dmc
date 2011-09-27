// This is a part of the Microsoft Classes C++ Class Library.
// Copyright (C) 1992-1993 Microsoft Corporation,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <stdarg.h>     // for va_args in Trace
#include "vbctrl.h"

#ifdef AFX_VBX_SEG
#pragma code_seg(AFX_VBX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IPROP_STD_VISIBLE       0x000A      // From VBAPI.H

/////////////////////////////////////////////////////////////////////////////
// Constructors

CVBControl::CVBControl()
{
	m_pModel = NULL;
	m_hbrBkgnd = NULL;          // I.e. No brush yet
	m_hFontCreated = NULL;      // no font created by this control
	m_hCtl = NULL;
	m_hcurMouse = NULL;
	m_nCursorID = 0;
	m_nError = 0;
	m_bInPostNcDestroy = FALSE;
	m_bLoading = FALSE;
}

BOOL CVBControl::ParseWindowText(LPCSTR lpszWindowName, CString& strFileName,
		CString& strClassName, CString& strCaption)
{
	if (!AfxExtractSubString(strFileName, lpszWindowName, 0, ';') ||
	   !AfxExtractSubString(strClassName, lpszWindowName, 1, ';'))
		return FALSE;           // not enough info!

	// Skip past first two sections... to find caption (+2 for two ';'s)
	int nCaptionPosition = strFileName.GetLength() +
		strClassName.GetLength() + 2;

	ASSERT(lpszWindowName != NULL);

	if (nCaptionPosition > lstrlen(lpszWindowName))
		nCaptionPosition = lstrlen(lpszWindowName);

	strCaption = lpszWindowName + nCaptionPosition; // rest is caption

	return TRUE;
}

//
// Manual Creation
//
BOOL CVBControl::Create(LPCSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pFile, BOOL bAutoDelete)
{
	ASSERT(nID != 0);

	// dwStyle MUST be null if a Form File is used
	ASSERT(pFile == NULL || dwStyle == NULL);

	m_rectCreate = rect;                // Save create size

	CString strFileName;
	CString strClassName;
	CString strCaption;

	if (!ParseWindowText(lpszWindowName, strFileName, strClassName, strCaption))
		return FALSE;           // not enough info

	if (strCaption.GetLength() == 0)
		strCaption = ' ';

	if ((m_pModel = LoadControl(strFileName, strClassName)) == NULL)
		return FALSE;           // Could not find control Model

	if (!CommonInit())
		return FALSE;

	m_bAutoDelete = bAutoDelete;    // Do Not Delete when NCDestroy received
	m_bLoading = TRUE;              // Do not send events or warn on writes
										// to NoRuntimeW properties

	if (GetModelFlags() & MODEL_fInitMsg)
		CallControlProc(VBM_INITIALIZE, 0, 0);      // inform control of init

	if (!LoadProperties(pFile, TRUE))   // Load PreHwnd props
		return FALSE;

	_afxNewVBControl = this;  // set up control creation to use this object

	CString name = _afxVBPrefix;
	name += CString(GetVBXClass());

	if (dwStyle == NULL)    // NULL means use default
		dwStyle = GetModelStyles() | WS_CHILD | WS_TABSTOP;
	else
		dwStyle |= WS_CHILD;    // Guarantee WS_CHILD is set

	// Standard Visible Property requires Visible Creation
	if (GetStdPropIndex(IPROP_STD_VISIBLE) != -1)
		dwStyle |= WS_VISIBLE;
	else if (m_bCreatedInDesignMode)
		dwStyle |= WS_VISIBLE;

	if (!::CreateWindowEx(0, name, strCaption, dwStyle,
		0, 0, 0, 0,     // size must be zero at this time
		pParentWnd->GetSafeHwnd(), (HMENU)nID,
		AfxGetInstanceHandle(), NULL))
	{
		_afxNewVBControl = NULL;        // handle error cases
		return FALSE;
	}

	ASSERT(_afxNewVBControl == NULL); // should be cleared when control created
	ASSERT(m_hWnd != NULL);
	SetWindowText(strCaption); // Really set the window text

	_afxNewVBControl = NULL;        // handle error cases

	SetWindowText(strCaption);  // Really set the window text

	if (m_hWnd == NULL)
		return FALSE;       // Error in creation

	if (!LoadProperties(pFile, FALSE))  // Load PostHwnd props
		return FALSE;

	m_bLoading = FALSE; // back to normal processing

	if (GetModelFlags() & MODEL_fLoadMsg)
		CallControlProc(VBM_CREATED, 0, 0); // inform control of init

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Destruction

void CVBControl::PostNcDestroy()
{
	if (!m_bRecreating)         // if control is being recreated, do not delete
	{
		DeallocateHCTL();
		if (m_pModel != NULL)
			ReferenceFile(FALSE); // remove reference count

		if (m_bAutoDelete)      // otherwise, self-destruct if it was
		{
			m_bInPostNcDestroy = TRUE;
			delete this; // created automatically
		}
	}
	else
	{
		m_bRecreating = FALSE;
	}
}

CVBControl::~CVBControl()
{
	if (!m_bInPostNcDestroy)
	{
		m_bAutoDelete = FALSE; // do not allow PostNcDestroy to call us
		DestroyWindow();       // Kill off Window
	}

	if (m_hbrBkgnd != NULL)
		DeleteObject(m_hbrBkgnd);
	m_hbrBkgnd = NULL;

	if (m_hFontCreated != NULL)
		DeleteObject(m_hFontCreated);
	m_hFontCreated = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Operations


void CVBControl::OpenChannel(CFile* pFile, WORD wChannel)
{
	ASSERT_VALID(pFile);
	_afxVBState.fileMap.SetAt(wChannel, pFile);
}

BOOL CVBControl::CloseChannel(WORD wChannel)
{
	return _afxVBState.fileMap.RemoveKey(wChannel);
}

CFile* CVBControl::GetChannel(WORD wChannel)
{
	return (CFile*)_afxVBState.fileMap[wChannel];
}

/////////////////////////////////////////////////////////////////////////////
// Implementation for lazy link to Floating point

int CVBControl::ConvertFontSizeToTwips(LONG lFontSize)
{
	// Must call CVBControl::EnableVBXFloat() to initialize function pointer
	ASSERT(_afxConvertFontSizeToTwipsProc != NULL);
	return (*_afxConvertFontSizeToTwipsProc)(lFontSize);
}

long CVBControl::ConvertTwipsToFontSize(int nTwips)
{
	// Must call CVBControl::EnableVBXFloat() to initialize function pointer
	ASSERT(_afxConvertTwipsToFontSizeProc != NULL);
	return (*_afxConvertTwipsToFontSizeProc)(nTwips);
}


#ifdef _DEBUG
void CVBControl::AssertValid() const
{
	CWnd::AssertValid();
	if (m_hCtl)
		ASSERT(m_pModel != NULL);

	// check global state
	_afxVBState.fileMap.AssertValid();
	_afxVBState.vbxFileList.AssertValid();
}

void CVBControl::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
	AFX_DUMP1(dc, "\nm_pModel = ", m_pModel);

	AFX_DUMP1(dc, "\nm_bRecreating = ", m_bRecreating);
	AFX_DUMP1(dc, "\nm_bAutoDelete = ", m_bAutoDelete);
	AFX_DUMP1(dc, "\nm_bInPostNcDestroy = ", m_bInPostNcDestroy);
	AFX_DUMP1(dc, "\nm_nCursorID = ", m_nCursorID);
	AFX_DUMP1(dc, "\nm_hbrBkgnd = ", (UINT)m_hbrBkgnd);
	AFX_DUMP1(dc, "\nm_hFontCreated = ", (UINT)m_hFontCreated);
	AFX_DUMP1(dc, "\nm_hcurMouse = ", (UINT)m_hcurMouse);
	AFX_DUMP1(dc, "\nm_hCtl = ", m_hCtl);
	AFX_DUMP1(dc, "\nm_clrBkgnd = ", (void FAR*)m_clrBkgnd);
	AFX_DUMP1(dc, "\nm_clrFore = ", (void FAR*)m_clrFore);
	AFX_DUMP1(dc, "\nm_tag = ", m_strTag);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Include Debug VBX Trace Routine

/////////////////////////////////////////////////////////////////////////////
// Special Trace routine
// allows us to have one lib for both debug and non-debug builds

extern LPCSTR BASED_CODE _afxVBXDebugFormatStrings[];

void CDECL CVBControl::Trace(BOOL bFatal, UINT nFormatIndex, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, nFormatIndex);

	LPCSTR lpszFormat = _afxVBXDebugFormatStrings[nFormatIndex];

#ifndef _AFXDLL
	// output like a Trace
	int nBuf;
	char szBuffer[512];

	nBuf = wvsprintf(szBuffer, lpszFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

	static char BASED_CODE szColon[] = ": ";
	if ((afxTraceFlags & 1) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << szColon;
	afxDump << szBuffer;
#else
	// under the DLL version call the App supplied trace
	(_AfxGetAppDebug()->lpfnTraceV)(lpszFormat, args);
#endif //_AFXDLL

	if (bFatal)
		ASSERT(FALSE);   // Fatal Error, Do Not Continue
#endif //_DEBUG
}

BOOL CVBControl::EnableMemoryTracking(BOOL bTracking)
{
#ifdef _DEBUG
	return AfxEnableMemoryTracking(bTracking);
#else
	return FALSE;
#endif
}

// use the correct version of new and delete for calls
// within the library code
CVBControl* CVBControl::NEW()
{
	return new CVBControl();
}

void CVBControl::DELETE()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
