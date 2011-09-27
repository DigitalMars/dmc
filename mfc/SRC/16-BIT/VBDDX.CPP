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

#ifdef AFX_VBX_SEG
#pragma code_seg(AFX_VBX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VB specific DDX procs

// VB controls are special
void AFXAPI DDX_VBControl(CDataExchange* pDX, int nIDC, CVBControl*& rpControl)
{
	if (rpControl == NULL)
	{
		ASSERT(!pDX->m_bSaveAndValidate);
		rpControl = pDX->PrepareVBCtrl(nIDC);
	}
}

CVBControl* CDataExchange::PrepareVBCtrl(int nIDC)
{
	ASSERT(nIDC != 0);
	ASSERT(nIDC != -1); // not allowed
	HWND hWndCtrl = ::GetDlgItem(m_pDlgWnd->m_hWnd, nIDC);
	if (hWndCtrl == NULL)
	{
		TRACE1("Error: no data exchange control with ID 0x%04X\n", nIDC);
		ASSERT(FALSE);
		AfxThrowNotSupportedException();
	}
	m_hWndLastControl = hWndCtrl;
	m_bEditLastControl = FALSE; // not an edit item by default
	ASSERT(hWndCtrl != NULL);       // never return NULL handle
	CWnd* pWnd = CWnd::FromHandle(hWndCtrl);
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CVBControl)));
	return (CVBControl*)pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// Simple formatting to text item

void AFXAPI DDX_VBText(CDataExchange* pDX, int nIDC, int nPropIndex,
	CString& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
	ASSERT(pControl->GetPropType(nPropIndex) == DT_HSZ);
	if (pDX->m_bSaveAndValidate)
		value = pControl->GetStrProperty(nPropIndex);
	else
		pControl->SetStrProperty(nPropIndex, value);
}

void AFXAPI DDX_VBTextRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	CString& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
		ASSERT(pControl->GetPropType(nPropIndex) == DT_HSZ);
		value = pControl->GetStrProperty(nPropIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////
// non-text properties

void AFXAPI DDX_VBBool(CDataExchange* pDX, int nIDC, int nPropIndex,
	BOOL& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
	ASSERT(pControl->GetPropType(nPropIndex) == DT_BOOL);
	if (pDX->m_bSaveAndValidate)
		value = (BOOL)pControl->GetNumProperty(nPropIndex);
	else
		pControl->SetNumProperty(nPropIndex, value);
}

void AFXAPI DDX_VBBoolRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	BOOL& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
		ASSERT(pControl->GetPropType(nPropIndex) == DT_BOOL);
		value = (BOOL)pControl->GetNumProperty(nPropIndex);
	}
}

void AFXAPI DDX_VBInt(CDataExchange* pDX, int nIDC, int nPropIndex,
	int& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
#ifdef _DEBUG
	UINT nPropType = pControl->GetPropType(nPropIndex);
	ASSERT(nPropType == DT_ENUM || nPropType == DT_SHORT ||
		   nPropType == DT_XPOS || nPropType == DT_XSIZE ||
		   nPropType == DT_YPOS || nPropType == DT_YSIZE);
#endif
	if (pDX->m_bSaveAndValidate)
		value = (int)pControl->GetNumProperty(nPropIndex);
	else
		pControl->SetNumProperty(nPropIndex, value);
}

void AFXAPI DDX_VBIntRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	int& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
#ifdef _DEBUG
		UINT nPropType = pControl->GetPropType(nPropIndex);
		ASSERT(nPropType == DT_ENUM || nPropType == DT_SHORT ||
			   nPropType == DT_XPOS || nPropType == DT_XSIZE ||
			   nPropType == DT_YPOS || nPropType == DT_YSIZE);
#endif
		value = (int)pControl->GetNumProperty(nPropIndex);
	}
}

void AFXAPI DDX_VBLong(CDataExchange* pDX, int nIDC, int nPropIndex,
	LONG& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
#ifdef _DEBUG
	UINT nPropType = pControl->GetPropType(nPropIndex);
	ASSERT(nPropType == DT_LONG);
#endif
	if (pDX->m_bSaveAndValidate)
		value = pControl->GetNumProperty(nPropIndex);
	else
		pControl->SetNumProperty(nPropIndex, value);
}

void AFXAPI DDX_VBLongRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	LONG& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
#ifdef _DEBUG
		UINT nPropType = pControl->GetPropType(nPropIndex);
		ASSERT(nPropType == DT_LONG);
#endif
		value = pControl->GetNumProperty(nPropIndex);
	}
}

void AFXAPI DDX_VBColor(CDataExchange* pDX, int nIDC, int nPropIndex,
	COLORREF& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
	ASSERT(pControl->GetPropType(nPropIndex) == DT_COLOR);
	if (pDX->m_bSaveAndValidate)
		value = (COLORREF)pControl->GetNumProperty(nPropIndex);
	else
		pControl->SetNumProperty(nPropIndex, (DWORD)value);
}

void AFXAPI DDX_VBColorRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	COLORREF& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
		ASSERT(pControl->GetPropType(nPropIndex) == DT_COLOR);
		value = (COLORREF)pControl->GetNumProperty(nPropIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////
