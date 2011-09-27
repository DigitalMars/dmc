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

#ifdef AFX_OLE_SEG
#pragma code_seg(AFX_OLE_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleDocument printing support

BOOL COleDocument::ApplyPrintDevice(const DVTARGETDEVICE FAR* ptd)
{
	ASSERT_VALID(this);
	ASSERT(ptd == NULL || AfxIsValidAddress(ptd, (size_t)ptd->tdSize, FALSE));

	// allocate copy of target device
	if (ptd != NULL)
	{
		DVTARGETDEVICE FAR* ptdNew =
			::OleStdCopyTargetDevice((DVTARGETDEVICE FAR*)ptd);
		if (ptdNew == NULL)
			return FALSE;
		ptd = ptdNew;
	}
	// remove old target device from memory
	OleStdDeleteTargetDevice(m_ptd);
	m_ptd = (DVTARGETDEVICE FAR*)ptd;

	// Note: updating all the client items does not refresh the pres. cache
	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		// update all the client items with new target device
		pItem->SetPrintDevice(ptd);
	}
	return TRUE;
}

BOOL COleDocument::ApplyPrintDevice(const PRINTDLG* ppd)
{
	ASSERT_VALID(this);
	ASSERT(ppd == NULL || AfxIsValidAddress(ppd, sizeof(*ppd), FALSE));
	DVTARGETDEVICE FAR* ptd = NULL;
	if (ppd != NULL)
		ptd = ::OleStdCreateTargetDevice((PRINTDLG*)ppd);

	BOOL bResult = ApplyPrintDevice(ptd);
	::OleStdDeleteTargetDevice(ptd);
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
