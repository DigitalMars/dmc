// addform.cpp : implementation of the CAddForm class
//

#include "stdafx.h"
#include "daoenrol.h"
#include "addform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAddForm, CDaoRecordView)

BEGIN_MESSAGE_MAP(CAddForm, CDaoRecordView)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
END_MESSAGE_MAP()

CAddForm::CAddForm(UINT nIDTemplate)
	: CDaoRecordView(nIDTemplate)
{
	m_bAddMode = FALSE;
}

CAddForm::~CAddForm()
{
}

BOOL CAddForm::OnMove(UINT nIDMoveCommand)
{
	CDaoRecordset* pRecordset = OnGetRecordset();
	if (m_bAddMode)
	{
		if (!UpdateData())
			return FALSE;
		try
		{
			pRecordset->Update();
		}
		catch (CDaoException* e)
		{
			AfxMessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
			return FALSE;
		}

		pRecordset->Requery();
		UpdateData(FALSE);
		m_bAddMode = FALSE;
		return TRUE;
	}
	else
	{
		return CDaoRecordView::OnMove(nIDMoveCommand);
	}
}

BOOL CAddForm::RecordAdd()
{
	// If already in add mode, then complete previous new record
	if (m_bAddMode)
		OnMove(ID_RECORD_FIRST);
	OnGetRecordset()->AddNew();
	m_bAddMode = TRUE;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CAddForm::RecordDelete()
{
	CDaoRecordset* pRecordset = OnGetRecordset();
	try
	{
		pRecordset->Delete();
	}
	catch (CDaoException* e)
	{
		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
		e->Delete();
		return FALSE;
	}

	// Move to the next record after the one just deleted
		pRecordset->MoveNext();

	// If we moved off the end of file, then move back to last record
	if (pRecordset->IsEOF())
		pRecordset->MoveLast();

	// If the recordset is now empty, then clear the fields
	// left over from the deleted record
	if (pRecordset->IsBOF())
		pRecordset->SetFieldNull(NULL);
	UpdateData(FALSE);
	return TRUE;
}


BOOL CAddForm::RecordRefresh()
{

	if (m_bAddMode == TRUE)
	{
		OnGetRecordset()->Move(0);
		m_bAddMode = FALSE;
	}
	// Copy fields from recordset to form, thus
	// overwriting any changes user may have made
	// on the form
	UpdateData(FALSE);

	return TRUE;
}

void CAddForm::OnRecordAdd()
{
	RecordAdd();
}

void CAddForm::OnUpdateRecordFirst(CCmdUI* pCmdUI)
{
	if (m_bAddMode)
		pCmdUI->Enable(TRUE);
	else
		CDaoRecordView::OnUpdateRecordFirst(pCmdUI);
}

void CAddForm::OnRecordRefresh()
{
	RecordRefresh();
}

void CAddForm::OnRecordDelete()
{
	RecordDelete();
}
