// coursese.cpp : implementation file
//

#include "stdafx.h"
#include "DaoEnrol.h"
#include "coursese.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCourseSet

IMPLEMENT_DYNAMIC(CCourseSet, CDaoRecordset)

CCourseSet::CCourseSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCourseSet)
	m_CourseID = _T("");
	m_CourseTitle = _T("");
	m_Hours = 0;
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CCourseSet::GetDefaultSQL()
{
	return _T("[Course]");
}

void CCourseSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCourseSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[CourseID]"), m_CourseID);
	DFX_Text(pFX, _T("[CourseTitle]"), m_CourseTitle);
	DFX_Short(pFX, _T("[Hours]"), m_Hours);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCourseSet diagnostics

#ifdef _DEBUG
void CCourseSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CCourseSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
