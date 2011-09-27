// stdreg.cpp : Defines the class behaviors for the application.
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
#include "stdreg.h"
#include "typeinfo.h"
#include "dialog.h"
#include "columdlg.h"
#include "coursset.h"
#include "stdset.h"
#include "instrset.h"
#include "sectset.h"
#include "dsectset.h"
#include "enrolset.h"
#include "initdata.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStdRegSetupApp

BEGIN_MESSAGE_MAP(CStdRegSetupApp, CWinApp)
	//{{AFX_MSG_MAP(CStdRegSetupApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStdRegSetupApp construction

CStdRegSetupApp::CStdRegSetupApp()
{
}

CStdRegSetupApp::~CStdRegSetupApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CStdRegSetupApp object

CStdRegSetupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CStdRegSetupApp initialization

BOOL CStdRegSetupApp::InitInstance()
{
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
	LoadStdProfileSettings();

	m_mapSQLTypeToSyntax[SQL_VARCHAR] = "varchar(50)";
	m_mapSQLTypeToSyntax[SQL_INTEGER] = "int";
	m_mapSQLTypeToSyntax[SQL_SMALLINT] = "smallint";

	CStdRegSetupDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}

void CStdRegSetupApp::AddDataSource()
{
	if (AfxMessageBox(IDS_CONFIRM_DB_ADDED_ALREADY, MB_YESNO) != IDYES)
		exit;

	CString strDSN;
	strDSN.LoadString(IDS_DEFAULT_DATA_SOURCE_NAME);

	#ifndef _MAC
	if (SQLCreateDataSource(m_pMainWnd->m_hWnd, strDSN))
	#else
	if (SQLCreateDataSource(GetWrapperWindow(m_pMainWnd->m_hWnd), strDSN))
	#endif
		AfxMessageBox(IDS_DATA_SOURCE_ADDED);
	else
		AfxMessageBox(IDS_DATA_SOURCE_NOT_ADDED);
}

BOOL CStdRegSetupApp::GetColumnSyntax()
{
	CColSyntaxDlg dlgColSyntax;
	dlgColSyntax.m_pMapSQLTypeToSyntax = &m_mapSQLTypeToSyntax;
	if (dlgColSyntax.DoModal() != IDOK)
		return FALSE;
	return TRUE;
}

void CStdRegSetupApp::InitializeData()
{
	CString strDSN;
	strDSN.LoadString(IDS_DEFAULT_DATA_SOURCE_NAME);

	if (!m_db.Open(strDSN))
	{
		AfxMessageBox(IDS_CANNOT_OPEN_DATA_SOURCE);
		return;
	}

	if (!GetColumnSyntax())
			return;

	BeginWaitCursor();

	if (AddCourseTable()
		&& AddStudentTable()
		&& AddInstructorTable()
		&& AddSectionTable()
		&& AddDynabindSectionTable()
		&& AddEnrollmentTable())
	{
		AfxMessageBox(IDS_INITIALIZATION_COMPLETE);
	}
	else
	{
		AfxMessageBox(IDS_DATA_SOURCE_NOT_INITIALIZED);
	}

	ShowProgress(0);

	m_db.Close();

	EndWaitCursor();
}

void CStdRegSetupApp::ShowProgress(int nTablesDone)
{
	CStdRegSetupDlg* pDlg = (CStdRegSetupDlg*)m_pMainWnd;

	CString strProgress;

	if (nTablesDone > 0)
	{
		CString strProgressFormat;
		strProgressFormat.LoadString(IDS_PROGRESS);
		strProgress.Format(strProgressFormat, nTablesDone, 6);
	}

	pDlg->m_ctlProgress.SetWindowText(strProgress);
}


BOOL CStdRegSetupApp::ExecuteSQLAndReportFailure(const CString& strSQL)
{
	TRY
	{
		m_db.ExecuteSQL(strSQL);
	}
	CATCH(CDBException, e)
	{
		CString strMsg;
		strMsg.LoadString(IDS_EXECUTE_SQL_FAILED);
		strMsg += strSQL;
		return FALSE;
	}
	END_CATCH
	return TRUE;
}

BOOL CStdRegSetupApp::DropThenAddTable(const CString& strTableName, const CString& strColumns)
{
	CString strSQL;
	TRY
	{
		strSQL = "DROP TABLE ";
		strSQL += strTableName;
		m_db.ExecuteSQL(strSQL);  // failure is ok, for example, if table doesn't already exist
	}
	CATCH(CDBException,e)
	{
		// It is ok if table does not already exist.
	}
	END_CATCH

	strSQL = "CREATE TABLE ";
	strSQL += strTableName;
	strSQL += '(';
	strSQL += strColumns;
	strSQL += ')';
	return ExecuteSQLAndReportFailure(strSQL);
}

void CStdRegSetupApp::AddColumn(CString& strColumns, LPCSTR lpszColumnName, SWORD fSqlType,
	LPCSTR lpszColLength)
{
	if (!strColumns.IsEmpty())
		strColumns += ',';
	strColumns += lpszColumnName;

	CString strDataType;
	VERIFY(m_mapSQLTypeToSyntax.Lookup(fSqlType,strDataType));

	if (fSqlType == SQL_VARCHAR)
	{
		// The Column Syntax dialog instructed the user to specify
		// the syntax for a varchar with a maximum length of 50.
		// Replace the "50" with the column-specific length.
		BOOL b50Found = FALSE;

		ASSERT(lpszColLength != NULL);
		CString strDataTypeWith50Replaced;
		for (int nPos = 0; nPos < strDataType.GetLength(); nPos++)
		{
			if (strDataType[nPos] == '5' && strDataType[nPos+1] == '0')
			{
				strDataTypeWith50Replaced += lpszColLength;
				nPos += 1;
				b50Found = TRUE;
			}
			else
			{
				strDataTypeWith50Replaced += strDataType[nPos];
			}
		}
		if (b50Found)
			strDataType = strDataTypeWith50Replaced;
	}

	strColumns += ' ';
	strColumns += strDataType;
}

BOOL CStdRegSetupApp::AddCourseTable()
{
	ShowProgress(1);

	CString strTableName = "COURSE";
	CString strColumns;

	AddColumn(strColumns, "CourseID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "CourseTitle", SQL_VARCHAR, "50");
	AddColumn(strColumns, "Hours", SQL_SMALLINT);

	if (!DropThenAddTable(strTableName, strColumns))
		return FALSE;

	CCourseSet setCourse(&m_db);
	setCourse.Open();

	for (int nIndex = 0; nIndex < sizeof(courseData)/sizeof(CCourseData); nIndex++)
	{
		setCourse.AddNew();

		setCourse.m_CourseID    = courseData[nIndex].m_CourseID;
		setCourse.m_CourseTitle = courseData[nIndex].m_CourseTitle;
		setCourse.m_Hours       = courseData[nIndex].m_Hours;

		setCourse.Update();
	}

	setCourse.Close();

	return TRUE;
}

BOOL CStdRegSetupApp::AddStudentTable()
{
	ShowProgress(2);

	CString strTableName = "STUDENT";
	CString strColumns;

	AddColumn(strColumns, "StudentID", SQL_INTEGER);
	AddColumn(strColumns, "Name", SQL_VARCHAR, "40");
	AddColumn(strColumns, "GradYear", SQL_SMALLINT);

	if (!DropThenAddTable(strTableName, strColumns))
		return FALSE;

	CStudentSet setStudent(&m_db);
	setStudent.Open();

	for (int nIndex = 0; nIndex < sizeof(studentData)/sizeof(CStudentData); nIndex++)
	{
		setStudent.AddNew();

		setStudent.m_StudentID   = studentData[nIndex].m_StudentID;
		setStudent.m_Name        = studentData[nIndex].m_Name;
		setStudent.m_GradYear    = studentData[nIndex].m_GradYear;

		setStudent.Update();
	}

	setStudent.Close();

	return TRUE;
}

BOOL CStdRegSetupApp::AddInstructorTable()
{
	ShowProgress(3);

	CString strTableName = "INSTRUCTOR";
	CString strColumns;

	AddColumn(strColumns, "InstructorID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "Name", SQL_VARCHAR, "40");
	AddColumn(strColumns, "RoomNo", SQL_VARCHAR, "10");

	if (!DropThenAddTable(strTableName, strColumns))
		return FALSE;

	CInstructorSet setInstructor(&m_db);
	setInstructor.Open();

	for (int nIndex = 0; nIndex < sizeof(instructorData)/sizeof(CInstructorData); nIndex++)
	{
		setInstructor.AddNew();

		setInstructor.m_InstructorID = instructorData[nIndex].m_InstructorID;
		setInstructor.m_Name         = instructorData[nIndex].m_Name;
		setInstructor.m_RoomNo       = instructorData[nIndex].m_RoomNo;

		setInstructor.Update();
	}

	setInstructor.Close();


	return TRUE;
}

BOOL CStdRegSetupApp::AddSectionTable()
{
	ShowProgress(4);

	CString strTableName = "SECTION";
	CString strColumns;

	AddColumn(strColumns, "CourseID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "SectionNo", SQL_VARCHAR, "4");
	AddColumn(strColumns, "InstructorID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "RoomNo", SQL_VARCHAR, "10");
	AddColumn(strColumns, "Schedule", SQL_VARCHAR, "24");
	AddColumn(strColumns, "Capacity", SQL_SMALLINT);

	if (!DropThenAddTable(strTableName, strColumns))
		return FALSE;

	CSectionSet setSection(&m_db);
	setSection.Open();

	for (int nIndex = 0; nIndex < sizeof(sectionData)/sizeof(CSectionData); nIndex++)
	{
		setSection.AddNew();

		setSection.m_CourseID       =sectionData[nIndex].m_CourseID;
		setSection.m_SectionNo      =sectionData[nIndex].m_SectionNo;
		setSection.m_InstructorID   =sectionData[nIndex].m_InstructorID;
		setSection.m_RoomNo         =sectionData[nIndex].m_RoomNo;
		setSection.m_Schedule       =sectionData[nIndex].m_Schedule;
		setSection.m_Capacity       =sectionData[nIndex].m_Capacity;

		setSection.Update();
	}

	setSection.Close();

	return TRUE;
}

BOOL CStdRegSetupApp::AddDynabindSectionTable()
{
	ShowProgress(5);

	CString strTableName = "DYNABIND_SECTION";
	CString strColumns;

	AddColumn(strColumns, "CourseID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "SectionNo", SQL_VARCHAR, "4");
	AddColumn(strColumns, "InstructorID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "RoomNo", SQL_VARCHAR, "10");
	AddColumn(strColumns, "Schedule", SQL_VARCHAR, "24");
	AddColumn(strColumns, "Capacity", SQL_SMALLINT);
	AddColumn(strColumns, "LabRoomNo", SQL_VARCHAR, "10");
	AddColumn(strColumns, "LabSchedule", SQL_VARCHAR, "24");

	if (!DropThenAddTable(strTableName,strColumns))
		return FALSE;

	CDynabindSectionSet setDynabindSection(&m_db);
	setDynabindSection.Open();

	for (int nIndex = 0; nIndex < sizeof(dynabindSectionData)/sizeof(CDynabindSectionData); nIndex++)
	{
		setDynabindSection.AddNew();

		setDynabindSection.m_CourseID     = dynabindSectionData[nIndex].m_CourseID;
		setDynabindSection.m_SectionNo    = dynabindSectionData[nIndex].m_SectionNo;
		setDynabindSection.m_InstructorID = dynabindSectionData[nIndex].m_InstructorID;
		setDynabindSection.m_RoomNo       = dynabindSectionData[nIndex].m_RoomNo;
		setDynabindSection.m_Schedule     = dynabindSectionData[nIndex].m_Schedule;
		setDynabindSection.m_Capacity     = dynabindSectionData[nIndex].m_Capacity;
		setDynabindSection.m_LabRoomNo    = dynabindSectionData[nIndex].m_LabRoomNo;
		setDynabindSection.m_LabSchedule  = dynabindSectionData[nIndex].m_LabSchedule;

		setDynabindSection.Update();
	}

	setDynabindSection.Close();

	return TRUE;
}

BOOL CStdRegSetupApp::AddEnrollmentTable()
{
	ShowProgress(6);

	CString strTableName = "ENROLLMENT";
	CString strColumns;

	AddColumn(strColumns, "StudentID", SQL_INTEGER);
	AddColumn(strColumns, "CourseID", SQL_VARCHAR, "8");
	AddColumn(strColumns, "SectionNo", SQL_VARCHAR, "4");
	AddColumn(strColumns, "Grade", SQL_VARCHAR, "1");

	if (!DropThenAddTable(strTableName,strColumns))
		return FALSE;

	CEnrollmentSet setEnrollment(&m_db);
	setEnrollment.Open();

	for (int nIndex = 0; nIndex < sizeof(enrollmentData)/sizeof(CEnrollmentData); nIndex++)
	{
		setEnrollment.AddNew();

		setEnrollment.m_StudentID      = enrollmentData[nIndex].m_StudentID;
		setEnrollment.m_CourseID       = enrollmentData[nIndex].m_CourseID;
		setEnrollment.m_SectionNo      = enrollmentData[nIndex].m_SectionNo;
		setEnrollment.m_Grade          = enrollmentData[nIndex].m_Grade;

		setEnrollment.Update();
	}

	setEnrollment.Close();

	return TRUE;
}
