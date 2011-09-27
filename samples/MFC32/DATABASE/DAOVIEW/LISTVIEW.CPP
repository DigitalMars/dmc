// samplvw.cpp : implementation of the CDaoListView class
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
#include "DlgParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDaoListView

IMPLEMENT_DYNCREATE(CDaoListView, CListView)

BEGIN_MESSAGE_MAP(CDaoListView, CListView)
	//{{AFX_MSG_MAP(CDaoListView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaoListView construction/destruction

CDaoListView::CDaoListView()
{
}

CDaoListView::~CDaoListView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDaoListView drawing

void CDaoListView::OnDraw(CDC* pDC)
{
	CDaoViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDaoListView diagnostics

#ifdef _DEBUG
void CDaoListView::AssertValid() const
{
	CListView::AssertValid();
}

void CDaoListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

#endif //_DEBUG
CDaoViewDoc* CDaoListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDaoViewDoc)));
	return (CDaoViewDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CDaoListView message handlers

int CDaoListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= LVS_REPORT;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Give the document a pointer to this view
	GetDocument()->m_pListView = this;

	return 0;
}

void CDaoListView::EraseList()
{
	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();
	ctlList.DeleteAllItems();
	while(ctlList.DeleteColumn(0));
	UpdateWindow();
}

void CDaoListView::ShowDatabase()
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoDatabaseInfo dbInfo;

	m_bVertical = TRUE;
	DisplayColumnHeadings(IDS_COL_DATABASE);

	try
	{
		m_pDB->m_pWorkspace->GetDatabaseInfo(m_pDB->GetName(),dbInfo,AFX_DAO_ALL_INFO);
		ShowDatabaseInfo(0,dbInfo);
	}
	catch(CDaoException* e)
	{
		e->Delete();
	}
	AdjustColumnWidths();
}

void CDaoListView::ShowTableData(LPCTSTR strTableName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());
	
	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();

	CDaoFieldInfo fieldInfo;
	int nFields;
	// Attempt to open the table (which may fail with a security violation)

	EraseList();

	CDaoTableDef td(m_pDB);
	try
	{
		td.Open(strTableName);
		nFields = td.GetFieldCount();
		for (int j=0; j < nFields; j++)
		{
			td.GetFieldInfo(j,fieldInfo);
			ctlList.AddColumn(fieldInfo.m_strName,j);
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
	td.Close();
	CDaoRecordset rs(m_pDB);
	int nItem = 0;
	int nLoaded = 0;
	BOOL MAXRECORDS = ((CDaoViewApp *)AfxGetApp())->m_nMaxRecords;
	try
	{
		CString strSelect(_T("Select * From ["));
		strSelect += strTableName;
		strSelect += _T("]");
		rs.Open(dbOpenDynaset,strSelect);
		while (!rs.IsEOF()) {
			if (nItem < MAXRECORDS)\
			{
				nLoaded++;
				COleVariant var;
				for (int i=0; i < nFields; i++)
				{
					var = rs.GetFieldValue(i);
					ctlList.AddItem(nItem,i,CCrack::strVARIANT(var));
				}
			}
			nItem++;
			rs.MoveNext();
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
	CString strRecCount;
	strRecCount.Format(_T("Loaded %d of %d total records"),nLoaded,nItem);
	UpdateWindow();
	if (nItem>=MAXRECORDS)
		MessageBox(strRecCount);
	((CFrameWnd *) AfxGetMainWnd())->SetMessageText(strRecCount);
	rs.Close();
}

void CDaoListView::ShowTableSchema(LPCTSTR strTableDefName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoTableDefInfo tabInfo;

	m_bVertical = strTableDefName != NULL;
	DisplayColumnHeadings(IDS_COL_TABLE);

	int nItem = 0;
	int nTableDefCount = m_pDB->GetTableDefCount();
	BOOL bShowSystemObjects = ((CDaoViewApp *)AfxGetApp())->m_bShowSystemObjects;
	for (int j=0; j < nTableDefCount; j++)
	{
		try
		{
			m_pDB->GetTableDefInfo(j,tabInfo,AFX_DAO_ALL_INFO);
			if (!bShowSystemObjects)
				if (tabInfo.m_lAttributes & dbSystemObject)
					continue;
			if (strTableDefName == NULL || tabInfo.m_strName == strTableDefName)
				ShowTableDefInfo(nItem++,tabInfo);
		}
		catch(CDaoException* e)
		{
			e->Delete();
		}
	}
	AdjustColumnWidths();
}

void CDaoListView::ShowRelations(LPCTSTR strRelationName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoRelationInfo Info;

	m_bVertical = strRelationName != NULL;
	DisplayColumnHeadings(IDS_COL_RELATION);

	int nItem = 0;
	int nTableDefCount = m_pDB->GetTableDefCount();
	BOOL bShowSystemObjects = ((CDaoViewApp *)AfxGetApp())->m_bShowSystemObjects;
	for (int j=0; j < nTableDefCount; j++)
	{
		try
		{
			m_pDB->GetRelationInfo(j,Info,AFX_DAO_ALL_INFO);
			if (!bShowSystemObjects)
				if (Info.m_lAttributes & dbSystemObject)
					continue;
			if (strRelationName == NULL || Info.m_strName == strRelationName)
				ShowRelationInfo(nItem++,Info);
		}
		catch(CDaoException* e)
		{
			e->Delete();
		}
	}
	AdjustColumnWidths();
}

void CDaoListView::ShowIndexes(LPCTSTR strTableName,LPCTSTR strIndexName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoTableDef td(m_pDB);
	CDaoIndexInfo Info;

	m_bVertical = strIndexName != NULL;
	DisplayColumnHeadings(IDS_COL_INDEX);

	try
	{
		td.Open(strTableName);
		int nItem = 0;
		int nIndexCount = td.GetIndexCount();
		for (int j=0; j < nIndexCount; j++)
		{
			try
			{
				td.GetIndexInfo(j,Info,AFX_DAO_ALL_INFO);
				if (strIndexName == NULL || Info.m_strName == strIndexName)
					ShowIndexInfo(nItem++,Info);
			}
			catch(CDaoException* e)
			{
				e->Delete();
			}
		}
		AdjustColumnWidths();
	}
	catch(CDaoException* e)
	{
		e->Delete();
	}
	td.Close();
	AdjustColumnWidths();
}

void CDaoListView::ShowQuerySchema(LPCTSTR strQueryDefName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoQueryDefInfo qdInfo;

	m_bVertical = strQueryDefName != NULL;
	DisplayColumnHeadings(IDS_COL_QUERYDEF);

	int nItem = 0;
	int nQueryDefCount = m_pDB->GetQueryDefCount();
	BOOL bShowSystemObjects = ((CDaoViewApp *)AfxGetApp())->m_bShowSystemObjects;
	for (int j=0; j < nQueryDefCount; j++)
	{
		try
		{
			m_pDB->GetQueryDefInfo(j,qdInfo,AFX_DAO_ALL_INFO);
			if (!bShowSystemObjects)
				if (qdInfo.m_nType == 5)
					continue;
			if (strQueryDefName == NULL || qdInfo.m_strName == strQueryDefName)
				ShowQueryDefInfo(nItem++,qdInfo);
		}
		catch (CDaoException* e)
		{
			e->Delete();
		}
	}
	AdjustColumnWidths();
}

void CDaoListView::RunQueryDef(LPCTSTR strQueryDefName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();

	CDaoFieldInfo fieldInfo;
	int nFields;

	CDaoQueryDef qd(m_pDB);

	EraseList();

	try
	{
		qd.Open(strQueryDefName);
		if (qd.GetParameterCount() > 0)
		{
			CDlgParams dlgParams;
			dlgParams.SetInfo(m_pDB,strQueryDefName);
			dlgParams.DoModal();
		}
		nFields = qd.GetFieldCount();
		for (int j=0; j < nFields; j++)
		{
			qd.GetFieldInfo(j,fieldInfo);
			ctlList.AddColumn(fieldInfo.m_strName,j);
		}
	}
	catch(CDaoException* e)
	{
		e->Delete();
	}
	//qd.Close();
	CDaoRecordset rs(m_pDB);
	int nItem = 0;
	int nLoaded = 0;
	BOOL MAXRECORDS = ((CDaoViewApp *)AfxGetApp())->m_nMaxRecords;
	try
	{
		rs.Open(&qd,dbOpenSnapshot,dbReadOnly);
		while (!rs.IsEOF()) {
			if (nItem<MAXRECORDS)
			{
				nLoaded++;
				COleVariant var;
				for (int i=0; i < nFields; i++)
				{
					var = rs.GetFieldValue(i);
					ctlList.AddItem(nItem,i,CCrack::strVARIANT(var));
				}
			}
			nItem++;
			rs.MoveNext();
		}
	}
	catch(CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
	CString strRecCount;
	strRecCount.Format(_T("Loaded %d of %d total records"),nLoaded,nItem);
	UpdateWindow();
	if (nItem>=MAXRECORDS)
		MessageBox(strRecCount);
	((CFrameWnd *) AfxGetMainWnd())->SetMessageText(strRecCount);
	if (rs.IsOpen())
		rs.Close();
}

void CDaoListView::ShowFields(LPCTSTR strTableName,LPCTSTR strFieldName)
{
	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	CDaoFieldInfo fieldInfo;

	m_bVertical = strFieldName != NULL;
	DisplayColumnHeadings(IDS_COL_FIELD);

	// Attempt to open the table (which may fail with a security violation)
	CDaoTableDef td(m_pDB);
	try
	{
		td.Open(strTableName);
		int nItem = 0;
		int nFieldCount = td.GetFieldCount();
		for (int j=0; j < nFieldCount; j++)
		{
			td.GetFieldInfo(j,fieldInfo,AFX_DAO_ALL_INFO);
			if (strFieldName == NULL || fieldInfo.m_strName == strFieldName)
			{
				ShowFieldInfo(nItem++,fieldInfo);
			}
		}
		AdjustColumnWidths();
	}
	catch(CDaoException* e)
	{
		e->Delete();
	}
	td.Close();
}

void CDaoListView::ShowFieldInfo(int nItem,CDaoFieldInfo& Info)
{
	CString strSize;
	CString strCollatingOrder;
	strSize.Format(_T("%ld"),Info.m_lSize);
	strCollatingOrder.Format(_T("%ld"),Info.m_lCollatingOrder);

	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,CCrack::strFieldType(Info.m_nType));
	AddItem(nItem,2,strSize);
	AddItem(nItem,3,CCrack::strBOOL(Info.m_bRequired));
	AddItem(nItem,4,CCrack::strBOOL(Info.m_bAllowZeroLength));
	AddItem(nItem,5,strCollatingOrder);
	AddItem(nItem,6,Info.m_strForeignName);
	AddItem(nItem,7,Info.m_strValidationRule);
	AddItem(nItem,8,Info.m_strValidationText);
	AddItem(nItem,9,Info.m_strDefaultValue);
}

void CDaoListView::ShowDatabaseInfo(int nItem,CDaoDatabaseInfo& Info)
{
	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,CCrack::strBOOL(Info.m_bUpdatable));
	AddItem(nItem,2,CCrack::strBOOL(Info.m_bTransactions));
	AddItem(nItem,3,Info.m_strVersion);
	AddItem(nItem,4,CCrack::strVARIANT(COleVariant(Info.m_lCollatingOrder)));
	AddItem(nItem,5,CCrack::strVARIANT(COleVariant(Info.m_nQueryTimeout)));
	AddItem(nItem,6,Info.m_strConnect);
}

void CDaoListView::ShowQueryDefInfo(int nItem,CDaoQueryDefInfo& Info)
{
	CString strODBCTimeout;
	strODBCTimeout.Format(_T("%hd"),Info.m_nODBCTimeout);
	
	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,CCrack::strQueryDefType(Info.m_nType));
	AddItem(nItem,2,Info.m_dateCreated.Format());
	AddItem(nItem,3,Info.m_dateLastUpdated.Format());
	AddItem(nItem,4,CCrack::strBOOL(Info.m_bUpdatable));
	AddItem(nItem,5,CCrack::strBOOL(Info.m_bReturnsRecords));
	AddItem(nItem,6,Info.m_strSQL);
	AddItem(nItem,7,Info.m_strConnect);
	AddItem(nItem,8,strODBCTimeout);
}

void CDaoListView::ShowTableDefInfo(int nItem,CDaoTableDefInfo& Info)
{
	CString strRecordCount;
	strRecordCount.Format(_T("%ld"),Info.m_lRecordCount);

	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,CCrack::strBOOL(Info.m_bUpdatable));
	AddItem(nItem,2,Info.m_dateCreated.Format());
	AddItem(nItem,3,Info.m_dateLastUpdated.Format());
	AddItem(nItem,4,Info.m_strSrcTableName);
	AddItem(nItem,5,Info.m_strConnect);
	AddItem(nItem,6,Info.m_strValidationRule);
	AddItem(nItem,7,Info.m_strValidationText);
	AddItem(nItem,8,strRecordCount);
}

void CDaoListView::ShowRelationInfo(int nItem,CDaoRelationInfo& Info)
{
	CString strAttributes;
	strAttributes.Format(_T("%ld"),Info.m_lAttributes);

	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,Info.m_strTable);
	AddItem(nItem,2,Info.m_strForeignTable);
	AddItem(nItem,3,strAttributes);
}

void CDaoListView::ShowIndexInfo(int nItem,CDaoIndexInfo& Info)
{
	CString strFieldInfo;
	for (int nIndex = 0; nIndex < Info.m_nFields; nIndex++)
	{
		strFieldInfo += Info.m_pFieldInfos[nIndex].m_bDescending ? _T("-") : _T("+");
		strFieldInfo += Info.m_pFieldInfos[nIndex].m_strName;
		if (nIndex < Info.m_nFields - 1)
			strFieldInfo += _T("; ");
	}
	CString strDistinctCount;
	strDistinctCount.Format(_T("%ld"),Info.m_lDistinctCount);

	AddItem(nItem,0,Info.m_strName);
	AddItem(nItem,1,strFieldInfo);
	AddItem(nItem,2,CCrack::strBOOL(Info.m_bPrimary));
	AddItem(nItem,3,CCrack::strBOOL(Info.m_bUnique));
	AddItem(nItem,4,CCrack::strBOOL(Info.m_bClustered));
	AddItem(nItem,5,CCrack::strBOOL(Info.m_bIgnoreNulls));
	AddItem(nItem,6,CCrack::strBOOL(Info.m_bRequired));
	AddItem(nItem,7,CCrack::strBOOL(Info.m_bForeign));
	AddItem(nItem,8,strDistinctCount);
}

void CDaoListView::DisplayColumnHeadings(UINT nStringID)
{
	CString strHeadings;
	strHeadings.LoadString(nStringID);

	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();

	int nPos;
	int nCount = 0;

	EraseList();

	if (m_bVertical)
	{
		ctlList.AddColumn(_T("Property     "),0);
		ctlList.AddColumn(_T("Value     "),1);
		m_nColumns = 2;
	}
	while ((nPos = strHeadings.Find(_T(","))) != -1){
		CString strItem;
		strItem = strHeadings.Left(nPos);
		if (m_bVertical)
			ctlList.AddItem(nCount++,0,strItem);
		else
			ctlList.AddColumn(strItem,nCount++);
		strItem = strHeadings.Mid(nPos + 1);
		strHeadings = strItem;
	}
	if (m_bVertical)
		ctlList.AddItem(nCount,0,strHeadings);
	else
		ctlList.AddColumn(strHeadings,nCount);
	m_nColumns = nCount;
}

void CDaoListView::AdjustColumnWidths()
{
	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();

	ctlList.SetColumnWidth(-1,-3);
}

void CDaoListView::AddItem(int nItem,int nSubItem,LPCTSTR strItem)
{
	CListCtrlEx& ctlList = (CListCtrlEx&) GetListCtrl();

	if (m_bVertical)
		ctlList.AddItem(nSubItem,1,strItem);
	else
		ctlList.AddItem(nItem,nSubItem,strItem);
}
