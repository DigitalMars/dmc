// TreeView.cpp : implementation of the CDaoTreeView class
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
#include "DaoView.h"
#include "dragitem.h"
#include "dlgsql.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaoTreeView

IMPLEMENT_DYNCREATE(CDaoTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CDaoTreeView, CTreeView)
	//{{AFX_MSG_MAP(CDaoTreeView)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_EDIT_NEWQUERY, OnNewQuery)
	ON_COMMAND(ID_QUERY_EDIT, OnQueryEdit)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_TABLE_SCHEMA,ID_QUERY_RUN, OnPopupCommand)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN,OnKeyDown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED,OnNodeSelect)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG,OnBeginDrag)
	ON_NOTIFY_REFLECT(NM_RCLICK,OnRightClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaoTreeView construction/destruction

CDaoTreeView::CDaoTreeView() : CTreeView()
{
	m_nIDClipFormat = RegisterClipboardFormat(_T("DaoView"));
	m_pDB = NULL;
	m_bNoNotifications = FALSE;
}

CDaoTreeView::~CDaoTreeView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CDaoTreeView diagnostics

#ifdef _DEBUG
void CDaoTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDaoTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

#endif //_DEBUG
CDaoViewDoc* CDaoTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDaoViewDoc)));
	return (CDaoViewDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CDaoTreeView message handlers

int CDaoTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS;
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetDocument()->m_pTreeView = this;

	// Create the Image List
	m_ctlImage.Create(IDB_IMAGELIST3,16,0,RGB(255,0,255));
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	CTreeCtrlEx& ctlTree = (CTreeCtrlEx&) GetTreeCtrl();
	ctlTree.SetImageList(&m_ctlImage);

	m_dropTarget.Register(this);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDaoTreeView implementation functions

void CDaoTreeView::PopulateTree() 
{
	CTreeCtrlEx& ctlTree = (CTreeCtrlEx&) GetTreeCtrl();

	m_bNoNotifications = TRUE;
	
	ctlTree.DeleteAllItems();
	UpdateWindow();

	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	// Insert root node by call to tree object this time
	tDatabase = ctlTree.GetRootItem().AddTail(m_pDB->GetName(),IID_DATABASE);

	// From now on call via iterators
	tTables = tDatabase.AddTail(_T("Tables"),IID_TABLES);
	tRelations = tDatabase.AddTail(_T("Relations"),IID_RELATIONS);
	tQueryDefs = tDatabase.AddTail(_T("QueryDefs"),IID_QUERYDEFS);

	BOOL bShowSystemObjects = ((CDaoViewApp *)AfxGetApp())->m_bShowSystemObjects;

	CDaoTableDefInfo tabInfo;
	CDaoFieldInfo fieldInfo;
	CDaoIndexInfo indexInfo;
	try
	{
		int nTableDefCount = m_pDB->GetTableDefCount();
		for (int i = 0; i < nTableDefCount; i++)
		{
			m_pDB->GetTableDefInfo(i,tabInfo);
			if (!bShowSystemObjects)
				if (tabInfo.m_lAttributes & dbSystemObject)
					continue;
			AddItem(IID_TABLE,tabInfo.m_strName);
		}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}

	try
	{
		CDaoQueryDefInfo queryInfo;
		int nQueryDefCount= m_pDB->GetQueryDefCount();
		for (int i = 0; i < nQueryDefCount; i++)
		{
			m_pDB->GetQueryDefInfo(i,queryInfo);
			if (!bShowSystemObjects)
				if (queryInfo.m_nType == 5)
					continue;
			tQueryDefs.AddTail(queryInfo.m_strName,IID_QUERYDEF);
		}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}

	try
	{
		CDaoRelationInfo relInfo;
		int nRelationCount = m_pDB->GetRelationCount();
		for (int i = 0;i < nRelationCount;i++)
		{
			m_pDB->GetRelationInfo(i,relInfo);
			if (!bShowSystemObjects)
				if (relInfo.m_lAttributes & dbSystemObject)
					continue;
			tRelations.AddTail(relInfo.m_strName,IID_RELATION);
		}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}

	tDatabase.Expand();
	m_bNoNotifications = FALSE;
	UpdateWindow();
}

void CDaoTreeView::AddItem(WORD nItemType, LPCTSTR lpszName)
{
	switch (nItemType)
	{
		case IID_TABLE:
		{
			tTable = tTables.AddTail(lpszName,IID_TABLE);
			tFields = tTable.AddTail(_T("Fields"),IID_FIELDS);
			tIndexes = tTable.AddTail(_T("Indexes"),IID_INDEXES);
			CDaoTableDef td(m_pDB);
			try
			{
				td.Open(lpszName);
				CDaoFieldInfo fieldInfo;
				int nFieldCount = td.GetFieldCount();
				for (int j=0; j < nFieldCount; j++)
				{
					td.GetFieldInfo(j,fieldInfo);
					 tFields.AddTail(fieldInfo.m_strName,IID_FIELD);
				}
				CDaoIndexInfo indexInfo;
				int nIndexCount = td.GetIndexCount();
				for(j=0;j < nIndexCount;j++)
				{
					td.GetIndexInfo(j,indexInfo);
					 tIndexes.AddTail(indexInfo.m_strName,IID_INDEX);
				}
			}
			catch(CDaoException* e)
			{
				// ... Do nothing.  Used to catch security violations opening tables.
				e->Delete();
			}
			td.Close();
			break;
		}
		case IID_QUERYDEF:
		{
			tQueryDefs.AddTail(lpszName,nItemType);
			break;
		}
	}
}

void CDaoTreeView::DeleteItem(CTreeCursor& itemDelete)
{
	switch (itemDelete.GetImageID())
	{
		case IID_TABLE:
		{
			try
			{
				m_pDB->DeleteTableDef(itemDelete.GetText());
				itemDelete.Delete();
			}
			catch (CDaoException* e)
			{
				MessageBox(
					_T("DaoView - Warning"),
					e->m_pErrorInfo->m_strDescription);
				e->Delete();
			}
			break;
		}	
		case IID_QUERYDEF:
		{
			try
			{
				m_pDB->DeleteQueryDef(itemDelete.GetText());
				itemDelete.Delete();
			}
			catch (CDaoException* e)
			{
				MessageBox(
					_T("DaoView - Warning"),
					e->m_pErrorInfo->m_strDescription);
				e->Delete();
			}
			break;
		}
	}
}

void CDaoTreeView::OnNodeSelect(NMHDR *pNotifyStruct,LRESULT *result)
{
	*result = 0;

	if (m_bNoNotifications)
		return;

	CTreeCtrlEx& ctlTree = (CTreeCtrlEx&) GetTreeCtrl();

	m_ItemSel = ctlTree.GetSelectedItem();
	
	UINT nImageID = m_ItemSel.GetImageID();
	GetDocument()->m_pListView->SetRedraw(FALSE);
	switch (nImageID)
	{
		case IID_DATABASE:
			GetDocument()->m_pListView->ShowDatabase();
			break;
		case IID_TABLES:
			GetDocument()->m_pListView->ShowTableSchema();
			break;
		case IID_TABLE:
			GetDocument()->m_pListView->ShowTableSchema(m_ItemSel.GetText());
			break;
		case IID_FIELDS:
			GetDocument()->m_pListView->ShowFields(m_ItemSel.GetParent().GetText());
			break;
		case IID_FIELD:
			GetDocument()->m_pListView->ShowFields(m_ItemSel.GetParent().GetParent().GetText(),m_ItemSel.GetText());
			break;
		case IID_QUERYDEFS:
			GetDocument()->m_pListView->ShowQuerySchema();
			break;
		case IID_QUERYDEF:
			GetDocument()->m_pListView->ShowQuerySchema(m_ItemSel.GetText());
			break;
		case IID_RELATIONS:
			GetDocument()->m_pListView->ShowRelations();
			break;
		case IID_RELATION:
			GetDocument()->m_pListView->ShowRelations(m_ItemSel.GetText());
			break;
		case IID_INDEXES:
			GetDocument()->m_pListView->ShowIndexes(m_ItemSel.GetParent().GetText());
			break;
		case IID_INDEX:
			GetDocument()->m_pListView->ShowIndexes(m_ItemSel.GetParent().GetParent().GetText(),m_ItemSel.GetText());
			break;
	}
	GetDocument()->m_pListView->SetRedraw(TRUE);
}

void CDaoTreeView::OnRightClick(NMHDR *pNotifyStruct,LRESULT *result)
{
	CTreeCtrlEx& ctlTree = (CTreeCtrlEx&) GetTreeCtrl();

	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	m_ItemSel = ctlTree.HitTest(curPoint, &nFlags);

	UINT nImageID = m_ItemSel.GetImageID();
	switch (nImageID)
	{
		case IID_TABLE:
			DoPopupMenu(IDR_POPUP_TABLE);
			break;
		case IID_QUERYDEF:
			DoPopupMenu(IDR_POPUP_QUERY);
			break;
	}
	*result = 0;
}

void CDaoTreeView::DoPopupMenu(UINT nMenuID)
{
	CMenu popMenu;

	popMenu.LoadMenu(nMenuID);
	
	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);
}

void CDaoTreeView::OnPopupCommand(UINT nMenuID)
{
	CWaitCursor w;
	GetDocument()->m_pListView->SetRedraw(FALSE);
	switch (nMenuID)
	{
		case ID_TABLE_SCHEMA:
			GetDocument()->m_pListView->ShowTableSchema(m_ItemSel.GetText());
			break;
		case ID_TABLE_DATA:
			GetDocument()->m_pListView->ShowTableData(m_ItemSel.GetText());
			break;
		case ID_QUERY_DEFINITION:
			GetDocument()->m_pListView->ShowQuerySchema(m_ItemSel.GetText());
			break;
		case ID_QUERY_RUN:
			GetDocument()->m_pListView->RunQueryDef(m_ItemSel.GetText());
			break;
	}
	GetDocument()->m_pListView->SetRedraw(TRUE);
}


void CDaoTreeView::OnBeginDrag(NMHDR *pNotifyStruct,LRESULT *result)
{
	int nImageID = m_ItemSel.GetImageID();

	switch (nImageID)
	{
		case IID_TABLE:
		case IID_QUERYDEF:
			CSharedFile globFile;
			CArchive ar(&globFile,CArchive::store);
			CDragItem dragItem(nImageID,m_pDB->GetName(),m_pDB->GetConnect(),m_ItemSel.GetText());
			dragItem.Serialize(ar);
			ar.Close();

			COleDataSource srcItem;

			srcItem.CacheGlobalData(m_nIDClipFormat,globFile.Detach());
			srcItem.DoDragDrop();
	}

}


DROPEFFECT CDaoTreeView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if (m_pDB)
		if (pDataObject->IsDataAvailable(m_nIDClipFormat))
			return DROPEFFECT_COPY;

	return CTreeView::OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CDaoTreeView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if (m_pDB)
		if (pDataObject->IsDataAvailable(m_nIDClipFormat))
			return DROPEFFECT_COPY;

	return CTreeView::OnDragOver(pDataObject, dwKeyState, point);
}

BOOL CDaoTreeView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	if (!(m_pDB && pDataObject->IsDataAvailable(m_nIDClipFormat)))
		return CTreeView::OnDrop(pDataObject, dropEffect, point);

	HGLOBAL hGlob = pDataObject->GetGlobalData(m_nIDClipFormat);
	if (hGlob != NULL){
		CSharedFile globFile;
		globFile.SetHandle(hGlob,FALSE);
		CArchive ar(&globFile,CArchive::load);
		CDragItem dragItem;
		dragItem.Serialize(ar);
		ar.Close();
		dragItem.Transfer(this,m_pDB);
		return TRUE;
	}

	return FALSE;
}


void CDaoTreeView::OnSysColorChange() 
{
	CWnd::OnSysColorChange();

	// Reset the background color of our image list when notified
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
}

void CDaoTreeView::OnKeyDown(NMHDR *pNotifyStruct,LRESULT *result)
{
	TV_KEYDOWN* pKeyDown = (TV_KEYDOWN *) pNotifyStruct;

	if (pKeyDown->wVKey == VK_DELETE)
	{
		BOOL bShowWarnings = ((CDaoViewApp *)AfxGetApp())->m_bShowWarnings;
		int retCode = IDYES;
		if (bShowWarnings)
		{
			retCode = MessageBox(
				_T("Are you sure you want to delete this item ?"),
				_T("DaoView - Warning"),MB_YESNO);
		}
		if (retCode == IDYES)
			DeleteItem(m_ItemSel);
	}
	*result = 0;
}

void CDaoTreeView::OnNewQuery() 
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDlgSQL dlgSQL;

	if (dlgSQL.DoModal() != IDOK)
		return;

	try
	{
		CDaoQueryDef qdTarget(m_pDB);

		qdTarget.Create(dlgSQL.m_strName,dlgSQL.m_strSQL);
		qdTarget.Append();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
	AddItem(IID_QUERYDEF,dlgSQL.m_strName);
}


void CDaoTreeView::OnQueryEdit() 
{
	try
	{
		CDlgSQL dlgSQL;

		dlgSQL.m_bEditMode = TRUE;

		CDaoQueryDef qdEdit(m_pDB);
		qdEdit.Open(m_ItemSel.GetText());

		dlgSQL.m_strName = m_ItemSel.GetText();
		dlgSQL.m_strSQL = qdEdit.GetSQL();

		if (dlgSQL.DoModal() == IDOK)
			qdEdit.SetSQL(dlgSQL.m_strSQL);

		qdEdit.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
}
