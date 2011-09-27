// scribitm.cpp : implementation of the CScribItem class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "scribble.h"

#include "scribdoc.h"
#include "scribitm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScribItem implementation

IMPLEMENT_DYNAMIC(CScribItem, CDocObjectServerItem)

CScribItem::CScribItem(CScribDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	// TODO: add one-time construction code here
	//  (eg, adding additional clipboard formats to the item's data source)
}

CScribItem::~CScribItem()
{
	// TODO: add cleanup code here
}

void CScribItem::Serialize(CArchive& ar)
{
	// CScribItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CScribDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CScribItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// This implementation of CScribItem::OnGetExtent only handles
	//  the "content" aspect indicated by DVASPECT_CONTENT.

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CScribItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  The default implementation
	//  here simply returns a hard-coded number of units.

	CScribDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	rSize = pDoc->GetDocSize();
	CClientDC dc(NULL);

	// set a MM_LOENGLISH based on logical inches
	//  (we can't use MM_LOENGLISH because MM_LOENGLISH uses physical inches)
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetViewportExt(dc.GetDeviceCaps(LOGPIXELSX), dc.GetDeviceCaps(LOGPIXELSY));
	dc.SetWindowExt(100, -100);
	dc.LPtoHIMETRIC(&rSize);

	return TRUE;
}

BOOL CScribItem::OnDraw(CDC* pDC, CSize& /* rSize */)
{
	CScribDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->SetMapMode(MM_ANISOTROPIC);
	CSize sizeDoc = pDoc->GetDocSize();
	sizeDoc.cy = -sizeDoc.cy;
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(sizeDoc);

	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	POSITION pos = strokeList.GetHeadPosition();
	while (pos != NULL)
	{
		strokeList.GetNext(pos)->DrawStroke(pDC);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CScribItem diagnostics

#ifdef _DEBUG
void CScribItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CScribItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

