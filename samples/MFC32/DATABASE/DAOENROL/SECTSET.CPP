// sectset.cpp : implementation of the CsectionSet class
//

#include "stdafx.h"
#include "DaoEnrol.h"
#include "sectset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CsectionSet implementation

IMPLEMENT_DYNAMIC(CsectionSet, CDaoRecordset)

CsectionSet::CsectionSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CsectionSet)
	m_CourseID = _T("");
	m_SectionNo = _T("");
	m_InstructorID = _T("");
	m_RoomNo = _T("");
	m_Schedule = _T("");
	m_Capacity = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
	m_nParams = 1;
	m_strCourseIDParam = _T("");

}


CString CsectionSet::GetDefaultSQL()
{
	return _T("[Section]");
}

void CsectionSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CsectionSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[CourseID]"), m_CourseID);
	DFX_Text(pFX, _T("[SectionNo]"), m_SectionNo);
	DFX_Text(pFX, _T("[InstructorID]"), m_InstructorID);
	DFX_Text(pFX, _T("[RoomNo]"), m_RoomNo);
	DFX_Text(pFX, _T("[Schedule]"), m_Schedule);
	DFX_Short(pFX, _T("[Capacity]"), m_Capacity);
	//}}AFX_FIELD_MAP
	pFX->SetFieldType(CDaoFieldExchange::param);
 	DFX_Text(pFX, _T("CourseIDParam"), m_strCourseIDParam);

}

/////////////////////////////////////////////////////////////////////////////
// CsectionSet diagnostics

#ifdef _DEBUG
void CsectionSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CsectionSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
