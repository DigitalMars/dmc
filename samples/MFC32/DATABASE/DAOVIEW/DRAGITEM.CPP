// DragItem.cpp : implementation file
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
#include "DragItem.h"
#include "renamedl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragItem

CDragItem::CDragItem()
{
	m_nItemType = itemNull;
}

CDragItem::CDragItem(WORD nItemType, LPCTSTR lpszDBName, LPCTSTR lpszDBConnect, LPCTSTR lpszItemName )
{
	m_nItemType = nItemType;
	m_strDBName = lpszDBName;
	m_strDBConnect = lpszDBConnect;
	m_strItemName = lpszItemName;
}

CDragItem::~CDragItem()
{
}

void CDragItem::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_nItemType;
		ar << m_strDBName;
		ar << m_strDBConnect;
		ar << m_strItemName;
	}
	else
	{
		ar >> m_nItemType;
		ar >> m_strDBName;
		ar >> m_strDBConnect;
		ar >> m_strItemName;
	}
}

BOOL CDragItem::Transfer(CDaoTreeView* pTreeView,CDaoDatabase* pDBTarget)
{
	CWaitCursor curWait;

	CDaoDatabase dbSource;

	try
	{
		dbSource.Open(m_strDBName,FALSE,TRUE,m_strDBConnect);
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return FALSE;
	}

	switch (m_nItemType)
	{
		case CDaoTreeView::IID_TABLE:
			if (CopyTable(&dbSource,pDBTarget))
				pTreeView->AddItem(m_nItemType,m_strItemName);
			break;
		case CDaoTreeView::IID_QUERYDEF:
			if (CopyQueryDef(&dbSource,pDBTarget))
				pTreeView->AddItem(m_nItemType,m_strItemName);
			break;
	}

	if (dbSource.IsOpen())
		dbSource.Close();

	return TRUE;
}

BOOL CDragItem::CopyTable(CDaoDatabase* pDBSource,CDaoDatabase* pDBTarget)
{
	CRenameDlg dlgRename;
	dlgRename.m_strName = m_strItemName;

	// Find out if the table exists already
	CDaoTableDefInfo tabInfo;
	int nTableCount = pDBTarget->GetTableDefCount();
	for (int i=0; i < nTableCount; i++)
	{
		pDBTarget->GetTableDefInfo(i,tabInfo);
		if (tabInfo.m_strName == dlgRename.m_strName)
		{
			if (dlgRename.DoModal() == IDCANCEL)
				return FALSE;
			i--;
		}
	}

	CDaoTableDef tdSource(pDBSource);
	CDaoTableDef tdTarget(pDBTarget);

	tdSource.Open(m_strItemName);
	pDBSource->GetTableDefInfo(m_strItemName,tabInfo,AFX_DAO_SECONDARY_INFO);

	tdTarget.Create(
		dlgRename.m_strName,
		tabInfo.m_lAttributes,
		tabInfo.m_strSrcTableName,
		tabInfo.m_strConnect);

	// Enumerate fields and append to new table
	CDaoFieldInfo fieldInfo;
	int nFieldCount = tdSource.GetFieldCount();
	for (i=0; i < nFieldCount; i++)
	{
		try{
			tdSource.GetFieldInfo(i,fieldInfo);
			tdTarget.CreateField(
				fieldInfo.m_strName,
				fieldInfo.m_nType,
				fieldInfo.m_lSize);
		}
		catch (CDaoException* e)
		{
			DisplayDaoException(e);
			e->Delete();
		}
	}

	tdTarget.Append();

	// Now attempt to copy the data
	CDaoRecordset rsSource(pDBSource);
	CDaoRecordset rsTarget(pDBTarget);
	try
	{
		CString strSel1,strSel2;
		strSel1 = strSel2 = (_T("Select * From ["));
		strSel1 += m_strItemName;
		strSel1 += _T("]");
		strSel2 += dlgRename.m_strName;
		strSel2 += _T("]");
		
		rsSource.Open(dbOpenDynaset,strSel1);
		rsTarget.Open(dbOpenDynaset,strSel2);
		while (!rsSource.IsEOF()) {
			COleVariant var;
			rsTarget.AddNew();
			for (int i=0; i < nFieldCount; i++)
			{
				var = rsSource.GetFieldValue(i);
				rsTarget.SetFieldValue(i,var);
			}

			rsTarget.Update();

			rsSource.MoveNext();
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}

	rsSource.Close();
	rsTarget.Close();

	tdTarget.Close();
	tdSource.Close();

	m_strItemName = dlgRename.m_strName;
	return TRUE;
}

BOOL CDragItem::CopyQueryDef(CDaoDatabase* pDBSource,CDaoDatabase* pDBTarget)
{
	CRenameDlg dlgRename;
	dlgRename.m_strName = m_strItemName;

	// Find out if the querydef exists already
	CDaoQueryDefInfo queryInfo;
	int nQueryDefCount = pDBTarget->GetQueryDefCount();
	for (int i=0; i < nQueryDefCount; i++)
	{
		pDBTarget->GetQueryDefInfo(i,queryInfo);
		if (queryInfo.m_strName == dlgRename.m_strName)
		{
			if (dlgRename.DoModal() == IDCANCEL)
				return FALSE;
			i--;
		}
	}

	CDaoQueryDef qdTarget(pDBTarget);

	try
	{
		pDBSource->GetQueryDefInfo(m_strItemName,queryInfo,AFX_DAO_ALL_INFO);

		qdTarget.Create(dlgRename.m_strName,queryInfo.m_strSQL);

		qdTarget.Append();
		qdTarget.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}

	m_strItemName = dlgRename.m_strName;
	return TRUE;
}

