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
// VB float properties

void AFXAPI DDX_VBFloat(CDataExchange* pDX, int nIDC, int nPropIndex,
	float& value)
{
	CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
	ASSERT(pControl->GetPropType(nPropIndex) == DT_REAL);
	if (pDX->m_bSaveAndValidate)
		value = pControl->GetFloatProperty(nPropIndex);
	else
		pControl->SetFloatProperty(nPropIndex, value);
}

void AFXAPI DDX_VBFloatRO(CDataExchange* pDX, int nIDC, int nPropIndex,
	float& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CVBControl* pControl = pDX->PrepareVBCtrl(nIDC);
		ASSERT(pControl->GetPropType(nPropIndex) == DT_REAL);
		value = pControl->GetFloatProperty(nPropIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////
