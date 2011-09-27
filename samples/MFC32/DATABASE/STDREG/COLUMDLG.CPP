// columdlg.cpp : implementation file
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
#include "columdlg.h"
#include "resource.h"
#include "typeinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColSyntaxDlg dialog


CColSyntaxDlg::CColSyntaxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColSyntaxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColSyntaxDlg)
	m_strSyntax = "";
	m_strListBoxInstruction = "";
	m_strColTypeInstruction = "";
	//}}AFX_DATA_INIT
}

BOOL CColSyntaxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_posMapSQLTypeToSyntax = m_pMapSQLTypeToSyntax->GetStartPosition();
	GetNextSQLTypeAndUpdateControls();

	return TRUE;
}


void CColSyntaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColSyntaxDlg)
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Control(pDX, IDC_TYPE_INFO_LIST, m_ctlTypeInfoList);
	DDX_Text(pDX, IDC_SYNTAX, m_strSyntax);
	DDX_Text(pDX, IDC_LIST_BOX_INSTRUCTION, m_strListBoxInstruction);
	DDX_Text(pDX, IDC_COL_TYPE_INSTRUCTION, m_strColTypeInstruction);
	//}}AFX_DATA_MAP
}

void CColSyntaxDlg::GetNextSQLTypeAndUpdateControls()
{
	ASSERT(m_pMapSQLTypeToSyntax != NULL);
	m_pMapSQLTypeToSyntax->GetNextAssoc(m_posMapSQLTypeToSyntax,
		m_swCurrentSQLType, m_strSyntax);

	CString strWindowText;
	strWindowText.LoadString(m_posMapSQLTypeToSyntax == NULL? IDS_OK : IDS_NEXT);
	m_ctlOK.SetWindowText(strWindowText);

	int nInstructionIDS, nExampleIDS, nSQLTypeIDS;
	switch (m_swCurrentSQLType)
	{
		case SQL_VARCHAR:
			nInstructionIDS = IDS_VARCHAR_INSTRUCTION;
			nExampleIDS =IDS_VARCHAR_EXAMPLE;
			nSQLTypeIDS = IDS_VARCHAR;
			break;

		case SQL_INTEGER:
			nInstructionIDS = IDS_INTEGER_INSTRUCTION;
			nExampleIDS =IDS_INTEGER_EXAMPLE;
			nSQLTypeIDS = IDS_INTEGER;
			break;

		case SQL_SMALLINT:
			nInstructionIDS = IDS_SMALLINT_INSTRUCTION;
			nExampleIDS =IDS_SMALLINT_EXAMPLE;
			nSQLTypeIDS = IDS_SMALLINT;
			break;
	}

	m_strColTypeInstruction.LoadString(IDS_COL_TYPE_INSTRUCTION);
	m_strColTypeInstruction += "\n\n";

	CString strInstructionPart2;
	strInstructionPart2.LoadString(nInstructionIDS);
	m_strColTypeInstruction += strInstructionPart2;
	m_strColTypeInstruction += "\n\n";

	strInstructionPart2.LoadString(nExampleIDS);
	m_strColTypeInstruction += strInstructionPart2;

	CString strListBoxInstruction;
	strListBoxInstruction.LoadString(IDS_TYPE_INFO_LISTBOX_INSTRUCTION);
	CString strSQLType;
	strSQLType.LoadString(nSQLTypeIDS);
	m_strListBoxInstruction.Format(strListBoxInstruction,strSQLType);

	// Update the m_strSyntax field as well as the instructions in the static controls.
	UpdateData(FALSE);

	FillTypeInfoListbox();
}

void CColSyntaxDlg::FillTypeInfoListbox()
{
	CTypeInfo typeInfo(&((CStdRegSetupApp*)AfxGetApp())->m_db);

	typeInfo.m_fSqlTypeParam = m_swCurrentSQLType;
	if (!typeInfo.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly))
		return;

	m_ctlTypeInfoList.ResetContent();
	CString strTypeInfo;
	while (!typeInfo.IsEOF())
	{
		strTypeInfo = typeInfo.m_strTypeName;
		if (!typeInfo.m_strCreateParams.IsEmpty())
		{
			strTypeInfo += '(';
			strTypeInfo += typeInfo.m_strCreateParams;
			strTypeInfo += ')';
		}
		m_ctlTypeInfoList.AddString(strTypeInfo);
		typeInfo.MoveNext();
	}

	typeInfo.Close();
}


BEGIN_MESSAGE_MAP(CColSyntaxDlg, CDialog)
	//{{AFX_MSG_MAP(CColSyntaxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColSyntaxDlg message handlers

void CColSyntaxDlg::OnOK()
{
	if (!UpdateData())
		return;

	m_pMapSQLTypeToSyntax->SetAt(m_swCurrentSQLType, m_strSyntax);

	if (m_posMapSQLTypeToSyntax == NULL)
	{
		EndDialog(IDOK);
		return;
	}

	GetNextSQLTypeAndUpdateControls();
}
