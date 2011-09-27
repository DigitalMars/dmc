// dbppg.cpp : Implementation of the CDbPropPage property page class.
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
#include "db.h"
#include "dbppg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDbPropPage, COlePropertyPage)

#define STR_LEN 128+1
 


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDbPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CDbPropPage)
	ON_CBN_SELCHANGE(IDC_DATASOURCE, OnSelchangeDatasource)
	ON_CBN_SELCHANGE(IDC_COLUMNS, OnSelchangeColumns)
	ON_CBN_SELCHANGE(IDC_TABLES, OnSelchangeTables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDbPropPage, "DB.DbPropPage.1",
	0xeff01749, 0x7825, 0x101b, 0x83, 0x75, 0x0, 0xaa, 0x0, 0x37, 0x3f, 0xb9)


/////////////////////////////////////////////////////////////////////////////
// CDbPropPage::CDbPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CDbPropPage

BOOL CDbPropPage::CDbPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DB_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CDbPropPage::CDbPropPage - Constructor

CDbPropPage::CDbPropPage() :
	COlePropertyPage(IDD, IDS_DB_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CDbPropPage)
	m_dataSource = _T("");
	m_columnName = _T("");
	m_tableName = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CDbPropPage::DoDataExchange - Moves data between page and properties

void CDbPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CDbPropPage)
	DDP_CBString(pDX, IDC_DATASOURCE, m_dataSource, _T("DataSource") );
	DDX_CBString(pDX, IDC_DATASOURCE, m_dataSource);
	DDP_CBString(pDX, IDC_TABLES, m_tableName, _T("TableName") );
	DDX_CBString(pDX, IDC_TABLES, m_tableName);
	DDP_CBString(pDX, IDC_COLUMNS, m_columnName, _T("ColumnName") );
	DDX_CBString(pDX, IDC_COLUMNS, m_columnName);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CDbPropPage message handlers

BOOL CDbPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();

	UCHAR 		szSourceName[SQL_MAX_DSN_LENGTH+1];
	UCHAR 		szDescription[255];
	SWORD		swDLength;
	SWORD		swSLength;
	RETCODE 	rCode;
	HENV 		hEnv;
	CComboBox	*pDSNCombo = (CComboBox *) GetDlgItem(IDC_DATASOURCE);
	CComboBox	*pTablesCombo = (CComboBox *) GetDlgItem(IDC_TABLES);
	CComboBox	*pColumnsCombo = (CComboBox *) GetDlgItem(IDC_COLUMNS);

	pDSNCombo->EnableWindow(FALSE);
	pTablesCombo->EnableWindow(FALSE);
	pColumnsCombo->EnableWindow(FALSE);

	if (SQLAllocEnv(&hEnv) != SQL_SUCCESS)
	{
		MessageBox("Couldn't allocate HENV!");
		return FALSE;
	}

	rCode = SQLDataSources(hEnv, SQL_FETCH_FIRST, szSourceName,
	    	SQL_MAX_DSN_LENGTH, &swSLength, szDescription,
			sizeof(szDescription), &swDLength);

	if (rCode != SQL_ERROR && rCode != SQL_NO_DATA_FOUND)
   	{
    	do {
    		pDSNCombo->AddString((char *) szSourceName);

	    	rCode = SQLDataSources(hEnv, SQL_FETCH_NEXT, szSourceName,
    			SQL_MAX_DSN_LENGTH, &swSLength, szDescription, sizeof(szDescription), &swDLength);
   		}
    	while (rCode != SQL_ERROR && rCode != SQL_NO_DATA_FOUND);
   	}
     
	SQLFreeEnv(hEnv);
	
	if (pDSNCombo->GetCount() > 0)
	{
		pDSNCombo->EnableWindow();

		if (!m_dataSource.IsEmpty())
			if (pDSNCombo->SelectString(-1, m_dataSource) == CB_ERR)
				m_dataSource.Empty();
			else
				OnSelchangeDatasource();
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void ODBCErrorBox(CWnd* pParent, HENV hEnv, HDBC hDBC)
{
	UCHAR	szState[SQL_MAX_MESSAGE_LENGTH];
	UCHAR	szError[SQL_MAX_MESSAGE_LENGTH];
	SDWORD	dwError;
	SWORD	wErrorLen;
	CString	strMessage;
				
	SQLError(hEnv, hDBC, SQL_NULL_HSTMT, szState, &dwError, szError,
		SQL_MAX_MESSAGE_LENGTH-1, &wErrorLen);
	strMessage = (LPSTR) szState;
	strMessage += ": ";
	strMessage += (LPSTR) szError;
		
	if (pParent == NULL)
		::MessageBox(NULL, (LPSTR) szError, "ODBC Error!", MB_OK);
	else
		pParent->MessageBox((LPSTR) szError, "ODBC Error!");
	
	return;
}

static int GetDriverLevel(HDBC hdbc)
{
	WORD	wLevel;
	
	SQLGetInfo(hdbc,
		SQL_ODBC_API_CONFORMANCE,
        (PTR)&wLevel,
        sizeof(wLevel),
        NULL);
    
    return (int) wLevel;
}


void CDbPropPage::OnSelchangeDatasource() 
{
	HENV		henv;
	HDBC		hdbc;
	HSTMT		hstmt;
	RETCODE		retcode;
	CComboBox	*psColumns = (CComboBox *) GetDlgItem(IDC_COLUMNS);
	CComboBox	*psBox = (CComboBox *) GetDlgItem(IDC_DATASOURCE);
	CComboBox	*psTables = (CComboBox *) GetDlgItem(IDC_TABLES);
	int			nSelected;
	char        szOwner[STR_LEN];
	char		szTableName[STR_LEN];
	UCHAR		szSource[STR_LEN];
	UCHAR		szBrowseResult[STR_LEN];
	SDWORD		cbOwner, cbName;
	
	nSelected = psBox->GetCurSel();
	if (nSelected == CB_ERR)
		return;
		
	psBox->GetLBText(nSelected, m_dataSource);
	strcpy((LPSTR) szSource, m_dataSource);

	retcode = SQLAllocEnv(&henv);              /* Environment handle */
	
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLAllocConnect(henv, &hdbc); /* Connection handle */
		if (retcode == SQL_SUCCESS)
		{
			BOOL bNeedID = FALSE;
			
			if (GetDriverLevel(hdbc) == 2)
			{
				SWORD	cbBrowse;
				UCHAR	szSourceInfo[256];
				
				cbBrowse = STR_LEN;
				strcpy((LPSTR) szSourceInfo, "DSN=");
				strcat((LPSTR) szSourceInfo, m_dataSource);
				
				SQLBrowseConnect(hdbc, szSourceInfo, SQL_NTS,
		                 szBrowseResult, STR_LEN, &cbBrowse); 

				MessageBox((LPSTR) szBrowseResult);
				if (strstr((LPCSTR) szBrowseResult, "PWD") != NULL)
				{
					bNeedID = TRUE;
				}
			}
			
			GetDlgItem(ID_USERNAME)->EnableWindow(bNeedID);
			GetDlgItem(ID_PASSWORD)->EnableWindow(bNeedID);

 	// Set login timeout to 15 seconds

			SQLSetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, 15);

	// Connect to data source
                                       
			retcode = SQLConnect(hdbc, szSource, SQL_NTS,
									NULL, SQL_NTS, NULL, SQL_NTS);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{

	// try to get a list of tables

				retcode = SQLAllocStmt(hdbc, &hstmt); /* Statement handle */
	 			if (retcode == SQL_SUCCESS)
				{
					retcode = SQLTables(hstmt,
								NULL, 0,	// all qualifiers
								NULL, 0,	// all owners
								NULL, 0,	// all tables
								NULL, 0);	// all types
								
					if (retcode == SQL_SUCCESS)
					{
						SQLBindCol(hstmt, 2, SQL_C_CHAR, szOwner, STR_LEN, &cbOwner);
						SQLBindCol(hstmt, 3, SQL_C_CHAR, szTableName, STR_LEN, &cbName);
                                    
						psTables->ResetContent();
						psColumns->ResetContent();
						psColumns->EnableWindow(FALSE);
						BOOL bAddedOne = FALSE;
						
						while (TRUE)
						{
							retcode = SQLFetch(hstmt);
							
							if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
							{
								CString strAdd;
								
								if (lstrlen(szOwner) != 0)
								{
									strAdd = szOwner;
									strAdd += ".";
								}
								strAdd += szTableName;
								
								psTables->AddString(strAdd);
								bAddedOne = TRUE;
							}
							else
							{
								break;
							}
						}
						
						psTables->EnableWindow(bAddedOne);
					}
					else
					{
						ODBCErrorBox(this, henv, hdbc);
					}

					SQLFreeStmt(hstmt, SQL_DROP);
				}
				
				SQLDisconnect(hdbc);
			}
			else
			{
				ODBCErrorBox(this, henv, hdbc);
			}

			SQLFreeConnect(hdbc);
		}
		SQLFreeEnv(henv);
	}
	
	return;
}

void CDbPropPage::OnSelchangeColumns() 
{
	return;
}

void CDbPropPage::OnSelchangeTables() 
{
	CComboBox	*psTables = (CComboBox *) GetDlgItem(IDC_TABLES);
	CComboBox	*psColumns = (CComboBox *) GetDlgItem(IDC_COLUMNS);
	int 		nSelected;
	HENV		henv;
	HDBC		hdbc;
	HSTMT		hstmt;
	RETCODE		retcode;
	BOOL		bAddedOne;
	UCHAR		szSource[256];
	UCHAR		szSelectedTable[256];
	UCHAR		szSelectedOwner[256];
	char		szColumnName[STR_LEN];
	SDWORD		cbColumnName;
	LPSTR		pstrTrim;
	BOOL		bHasOwner;


	nSelected = psTables->GetCurSel();
	if (nSelected == CB_ERR)
		return;

	psTables->GetLBText(nSelected, (char *) szSelectedTable);
	m_tableName = (char*) szSelectedTable;
	
	pstrTrim = strchr((LPSTR) szSelectedTable, '.');
	if (pstrTrim != NULL)
	{
		*pstrTrim = '\0';
		strcpy((LPSTR) szSelectedOwner, (LPSTR) szSelectedTable);
		strcpy((LPSTR) szSelectedTable, pstrTrim+1);
		bHasOwner = TRUE;
	}
	else
	{
		bHasOwner = FALSE;
	}
	strcpy((LPSTR) szSource, m_dataSource);

	retcode = SQLAllocEnv(&henv);              // Environment handle
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLAllocConnect(henv, &hdbc); // Connection handle
		if (retcode == SQL_SUCCESS)
		{

 	// Set login timeout to 15 seconds

			SQLSetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, 15);

	// Connect to data source
                                       
			retcode = SQLConnect(hdbc, szSource, SQL_NTS,
									NULL, SQL_NTS, NULL, SQL_NTS);
			
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				// try to get a list of columns for the table

				retcode = SQLAllocStmt(hdbc, &hstmt);		// Statement handle
	 			if (retcode == SQL_SUCCESS)
				{
					if (bHasOwner == TRUE)
					{
						retcode = SQLColumns(hstmt,
							NULL, 0,	// no table qualifier
							szSelectedOwner, SQL_NTS,	// specified owner
							szSelectedTable, SQL_NTS,	// selected table 
							NULL, 0		// no pattern
							);
					}
					else
					{
						retcode = SQLColumns(hstmt,
							NULL, 0,	// no table qualifier
							NULL, 0,	// no table owner
							szSelectedTable, SQL_NTS,	// selected table 
							NULL, 0		// no pattern
							);
					}
				
					if (retcode == SQL_SUCCESS)
					{
						SQLBindCol(hstmt, 4, SQL_C_CHAR, szColumnName, STR_LEN, &cbColumnName);
						
						psColumns->ResetContent();
						bAddedOne = FALSE;
						
						while (TRUE)
						{
							retcode = SQLFetch(hstmt);
							
							if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
							{
								psColumns->AddString(szColumnName);
								bAddedOne = TRUE;
							}
							else
							{
								break;
							}
						}
						
						psColumns->EnableWindow(bAddedOne);
					}
					else
					{
						ODBCErrorBox(this, henv, hdbc);
					}

					SQLFreeStmt(hstmt, SQL_DROP);
				}
				else
				{
					ODBCErrorBox(this, henv, hdbc);
				}

				SQLDisconnect(hdbc);
			}
			else
			{
				ODBCErrorBox(this, henv, hdbc);
			}

			SQLFreeConnect(hdbc);
		}
		SQLFreeEnv(henv);
	}

	if (psTables->GetCurSel() != CB_ERR)
	{
		psColumns->EnableWindow(TRUE);
	}
	else
	{
		psColumns->EnableWindow(TRUE);
	}	
}
