// catalog2View.cpp : implementation of the CCatalog2View class
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
#include "catalog2.h"

#include "catsets.h"
#include "cat2Doc.h"
#include "cat2View.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatalog2View

IMPLEMENT_DYNCREATE(CCatalog2View, CListView)

BEGIN_MESSAGE_MAP(CCatalog2View, CListView)
	//{{AFX_MSG_MAP(CCatalog2View)
	ON_COMMAND(ID_VIEW_TABLES, OnViewTablelevel)
	ON_COMMAND(ID_VIEW_COLUMNINFO, OnViewColumnlevel)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLUMNINFO, OnUpdateViewColumnlevel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TABLES, OnUpdateViewTablelevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatalog2View construction/destruction

CCatalog2View::CCatalog2View()
{
	m_strTableName = _T("");
}

CCatalog2View::~CCatalog2View()
{
}

BOOL CCatalog2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// set list view control to report, single selection
	cs.style &= ~(LVS_LIST | LVS_ICON | LVS_SMALLICON);
	cs.style |= LVS_REPORT;
	cs.style |= LVS_SINGLESEL;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCatalog2View drawing

void CCatalog2View::OnDraw(CDC* pDC)
{
	CCatalog2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCatalog2View diagnostics

#ifdef _DEBUG
void CCatalog2View::AssertValid() const
{
	CListView::AssertValid();
}

void CCatalog2View::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCatalog2Doc* CCatalog2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCatalog2Doc)));
	return (CCatalog2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCatalog2View message handlers

void CCatalog2View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// delete all items and columns
	CListCtrl& control = GetListCtrl();
	control.DeleteAllItems();
	while (control.DeleteColumn(0));

	// set up view based on the document's level
	switch (pDoc->m_nLevel)
	{
		case CCatalog2Doc::levelNone:

			// set the document title
			pDoc->SetTitle(pDoc->GetDSN());
			break;

		case CCatalog2Doc::levelTable:
		{
			// set the document title
			CString	strDataSource = pDoc->GetDSN();
			strDataSource += _T(" [Tables]");
			pDoc->SetTitle(strDataSource);

			// add columns to display
			control.InsertColumn(0,_T("Name"),LVCFMT_LEFT,100,-1);
			control.InsertColumn(1,_T("Type"),LVCFMT_LEFT,100,1);
			control.InsertColumn(2,_T("Qualifier"),LVCFMT_LEFT,100,2);
			control.InsertColumn(3,_T("Owner"),LVCFMT_LEFT,100,3);
			control.InsertColumn(4,_T("Remarks"),LVCFMT_LEFT,100,4);

			// traverse the table recordset
			// displaying the table information
			int	item = 0;
			pDoc->m_pTableset->MoveFirst();
			while (!pDoc->m_pTableset->IsEOF())
			{
				control.InsertItem(item,
					pDoc->m_pTableset->m_strTableName);
				control.SetItem(item,1,LVIF_TEXT,
					pDoc->m_pTableset->m_strTableType,0,0,0,0);
				control.SetItem(item,2,LVIF_TEXT,
					pDoc->m_pTableset->m_strTableQualifier,0,0,0,0);
				control.SetItem(item,3,LVIF_TEXT,
					pDoc->m_pTableset->m_strTableOwner,0,0,0,0);
				control.SetItem(item,4,LVIF_TEXT,
					pDoc->m_pTableset->m_strRemarks,0,0,0,0);
				item++;
				pDoc->m_pTableset->MoveNext();
			}
			break;
		}

		case CCatalog2Doc::levelColumn:
		{
			int column;

			// set the document title
			CString	strDataSource = pDoc->GetDSN();
			strDataSource += _T(" - ");
			strDataSource += m_strTableName;
			strDataSource += _T(" [Column Info]");
			pDoc->SetTitle(strDataSource);

			// add columns to display
			// respect the column info settings values
			column = 0;
			control.InsertColumn(column++,_T("Name"),LVCFMT_LEFT,100,-1);
			control.InsertColumn(column,_T("Type"),LVCFMT_LEFT,100,column++);
			if (pDoc->m_bLength)
				control.InsertColumn(column,_T("Length"),LVCFMT_LEFT,80,column++);
			if (pDoc->m_bPrecision)
			{
				control.InsertColumn(column,_T("Precision"),LVCFMT_LEFT,80,column++);
				control.InsertColumn(column,_T("Scale"),LVCFMT_LEFT,50,column++);
				control.InsertColumn(column,_T("Radix"),LVCFMT_LEFT,50,column++);
			}
			if (pDoc->m_bNullability)
				control.InsertColumn(column,_T("Nullable"),LVCFMT_LEFT,50,column++);

			// traverse the column info recordset
			// respect the column info settings values
			int	item = 0;
			pDoc->m_pColumnset->MoveFirst();
			while (!pDoc->m_pColumnset->IsEOF())
			{
				CString	strValue;

				// always insert the column name
				control.InsertItem(item,
					pDoc->m_pColumnset->m_strColumnName);

				// always insert the column type
				column = 1;
				control.SetItem(item,column++,LVIF_TEXT,
					pDoc->m_pColumnset->m_strTypeName,0,0,0,0);

				// only show type if requested
				if (pDoc->m_bLength)
				{
					strValue.Format(_T("%d"),pDoc->m_pColumnset->m_nLength);
					control.SetItem(item,column++,LVIF_TEXT,strValue,0,0,0,0);
				}

				// only show precision,scale,radix if requested
				if (pDoc->m_bPrecision)
				{
					// precision
					strValue.Format(_T("%d"),pDoc->m_pColumnset->m_nPrecision);
					control.SetItem(item,column++,LVIF_TEXT,strValue,0,0,0,0);

					// scale
					if (!pDoc->m_pColumnset->IsFieldNull(
						&(pDoc->m_pColumnset->m_nScale)))
					{
						strValue.Format(_T("%d"),pDoc->m_pColumnset->m_nScale);
						control.SetItem(item,column++,LVIF_TEXT,strValue,0,0,0,0);
					}
					else
						control.SetItem(item,column++,LVIF_TEXT,_T("<na>"),0,0,0,0);

					// radix
					if (!pDoc->m_pColumnset->IsFieldNull(
						&(pDoc->m_pColumnset->m_nRadix)))
					{
						strValue.Format(_T("%d"),pDoc->m_pColumnset->m_nRadix);
						control.SetItem(item,column++,LVIF_TEXT,strValue,0,0,0,0);
					}
					else
						control.SetItem(item,column++,LVIF_TEXT,_T("<na>"),0,0,0,0);
				}

				// only show nullability if requested
				if (pDoc->m_bNullability)
				{
					if (pDoc->m_pColumnset->m_fNullable == SQL_NO_NULLS)
						control.SetItem(item,column++,LVIF_TEXT,_T("No"),0,0,0,0);
					else if (pDoc->m_pColumnset->m_fNullable == SQL_NULLABLE)
						control.SetItem(item,column++,LVIF_TEXT,_T("Yes"),0,0,0,0);
					else
						control.SetItem(item,column++,LVIF_TEXT,_T("Unknown"),0,0,0,0);
				}

				item++;
				pDoc->m_pColumnset->MoveNext();
			}
			break;
		}
	}
}

void CCatalog2View::OnViewTablelevel()
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->SetLevel(CCatalog2Doc::levelTable);
}

void CCatalog2View::OnViewColumnlevel()
{
	int i;
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// determine list control selection
	CListCtrl&	control = GetListCtrl();
	int	nCount = control.GetItemCount();
	for (i = 0; i < nCount; i++)
	{
		if (control.GetItemState(i,LVIS_SELECTED))
			break;
	}
	if (i < nCount)
	{
		// pull table name to send to document
		m_strTableName = control.GetItemText(i,0);
	  
#ifndef _UNICODE
		LPCSTR lpszName;
		lpszName = m_strTableName;
#else
		LPSTR lpszName;
		char rgchTableName[257];
		lpszName = rgchTableName;
		int nSize;
		nSize = ::WideCharToMultiByte(CP_ACP,0,m_strTableName,
			-1, lpszName, 257, NULL, NULL);
		// Notify on failure
		ASSERT(nSize);
#endif	// _UNICODE
	  
		pDoc->FetchColumnInfo(lpszName);
		pDoc->SetLevel(CCatalog2Doc::levelColumn);
	}
}

void CCatalog2View::OnFileOpen()
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->OnOpenDocument())
		pDoc->SetLevel(CCatalog2Doc::levelTable);
	else
		pDoc->SetLevel(CCatalog2Doc::levelNone);
}

void CCatalog2View::OnUpdateViewColumnlevel(CCmdUI* pCmdUI)
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_nLevel == CCatalog2Doc::levelTable &&
		GetListCtrl().GetSelectedCount())
	{
		pCmdUI->Enable();
	}
	else
		pCmdUI->Enable(FALSE);
}

void CCatalog2View::OnUpdateViewTablelevel(CCmdUI* pCmdUI)
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_nLevel == CCatalog2Doc::levelColumn)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

BOOL CCatalog2View::OnChildNotify(UINT message, WPARAM wParam,
 LPARAM lParam, LRESULT* pLResult)
{
	CCatalog2Doc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// handle double click if at table view level
	if (pDoc->m_nLevel == CCatalog2Doc::levelTable)
	{
		if (message == WM_NOTIFY &&
			((NMHDR*)lParam)->code == NM_DBLCLK)
		{
			OnViewColumnlevel();
			return 0;
		}
	}
	return CListView::OnChildNotify(message,wParam,lParam,pLResult);
}
