/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/
// batchvw.cpp : implementation of the CBatchView class
//

#define INC_OLE2
#include "stdafx.h"
#include "windowsx.h"
#include "batch.h"
#include "batchdoc.h"
#include "batchvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchView

IMPLEMENT_DYNCREATE(CBatchView, CView)

BEGIN_MESSAGE_MAP(CBatchView, CView)
	//{{AFX_MSG_MAP(CBatchView)
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchView construction/destruction

CBatchView::CBatchView()
{
	// TODO: add construction code here
}

CBatchView::~CBatchView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBatchView drawing
void CBatchView::OnDraw(CDC *pDc)
{
}

/////////////////////////////////////////////////////////////////////////////
// CBatchView diagnostics

#ifdef _DEBUG
void CBatchView::AssertValid() const
{
	CView::AssertValid();
}

void CBatchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBatchDoc* CBatchView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBatchDoc)));
	return (CBatchDoc*)m_pDocument;
}
#endif //_DEBUG

//
// Size ourself, and then size our listbox to snugly fit inside.
//
void CBatchView::OnSize(UINT nType, int cx, int cy)
{
	CBatchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CView::OnSize(nType, cx, cy);
	if (pDoc->ListBox.m_hWnd)
		pDoc->ListBox.MoveWindow(0, 0, cx, cy);
}

//
// Somebody has dropped a filename on us.  Add it to our list.
//
void CBatchView::OnDropFiles(HDROP hDropInfo) 
{
	CBatchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: Add your message handler code here and/or call default
	
	SetActiveWindow();      // activate us first !
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		pDoc->FileList.AddTail(szFileName);
	}
	::DragFinish(hDropInfo);
	pDoc->FixListBox();
	Invalidate();
}

//
// Brand new window.  Create a listbox for it, which contains all the text we'll
// print.
//
void CBatchView::OnInitialUpdate() 
{
	CBatchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RECT rc;
	
	CView::OnInitialUpdate();
	DragAcceptFiles();
	GetClientRect(&rc);
	pDoc->ListBox.Create(LBS_NOINTEGRALHEIGHT | LBS_EXTENDEDSEL |
		LBS_MULTIPLESEL | LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT |
		WS_VISIBLE | WS_CHILD | WS_VSCROLL, rc, this, 42);
	pDoc->FixListBox();

}


BOOL CBatchView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnCommand(wParam, lParam);
}

LRESULT CBatchView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CBatchDoc* pDoc;
	
	switch(message) {
		case WM_VKEYTOITEM:
			switch (LOWORD(wParam)) {

				// Delete the selection
				case VK_DELETE:
					pDoc = GetDocument();
					ASSERT_VALID(pDoc);
					pDoc->DeleteSelection();
			}
	}			
	return CView::WindowProc(message, wParam, lParam);
}
