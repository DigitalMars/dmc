// DAOTDlg.cpp : implementation file
//
// Main dialog of DAOTable--most other dialogs are entered
// from the dialog implemented here.

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DAOTable.h"
#include "DAOTDlg.h"

#include "database.h"
#include "tabledef.h"
#include "querydef.h"

#include "addtbdlg.h"
#include "listctrl.h"
#include "addixdlg.h"
#include "AddQyDlg.h"
#include "AddDbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDAOTableDlg dialog

CDAOTableDlg::CDAOTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDAOTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDAOTableDlg)
	m_strDatabaseName = _T("");
	m_strTableName = _T("");
	m_strQueryName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// initialize the database pointer
	m_pDatabase = NULL;
}

void CDAOTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDAOTableDlg)
	DDX_Control(pDX, IDC_COMBO_QUERY_NAME, m_QueryNameComboControl);
	DDX_Control(pDX, IDC_COMBO_TABLE_NAME, m_TableNameComboControl);
	DDX_Text(pDX, IDC_EDIT_DATABASE_NAME, m_strDatabaseName);
	DDX_Text(pDX, IDC_COMBO_TABLE_NAME, m_strTableName);
	DDX_Text(pDX, IDC_COMBO_QUERY_NAME, m_strQueryName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDAOTableDlg, CDialog)
	//{{AFX_MSG_MAP(CDAOTableDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_TABLE_FIELDS, OnButtonTableFields)
	ON_BN_CLICKED(IDC_BUTTON_TABLE_INDEXES, OnButtonTableIndexes)
	ON_BN_CLICKED(IDC_BUTTON_QUERY_DEFINITION, OnButtonQueryDefinition)
	ON_CBN_DROPDOWN(IDC_COMBO_TABLE_NAME, OnDropdownComboTableName)
	ON_CBN_DROPDOWN(IDC_COMBO_QUERY_NAME, OnDropdownComboQueryName)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TABLE, OnButtonDeleteTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDAOTableDlg message handlers

BOOL CDAOTableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// set focus to name-of-database edit box
	CEdit *pEdit;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DATABASE_NAME);
	if (pEdit != NULL)
		pEdit->SetFocus();

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CDAOTableDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CDAOTableDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDAOTableDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDAOTableDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// user selected the "Connect" button to
// a) connect to a specified database file
// b) create a new database file 
// c) browse for a database file to open
void CDAOTableDlg::OnButtonConnect() 
{
	// close any open database
	closeDatabase(&m_pDatabase);

	// get the database name (if any) from the dialog control
	UpdateData(TRUE);
	
	// if user specified a name, then open it directly,
	if (m_strDatabaseName != _T(""))
	{
		// now open the database if possible
		int retCode = openDatabase(&m_pDatabase, m_strDatabaseName, FALSE);
		if (SUCCESS == retCode)    	
		{
			// once a database is opened, other controls are enabled
			enableControlsForOpenDatabase(TRUE);              	
		}                                                     	
		else if (FAILURE == retCode) // user has specified a new database to create
		{
			// just to keep this function short, farm rest of 
			// process out to helper function--create a new database
			createNewDatabase();
		}
		else // FATAL -- nothing can be done
			// if database not open, other controls are disabled
			enableControlsForOpenDatabase(FALSE);              	
	}
	else
	{
		// allow user to select database to open
		CFileDialog	dlg (TRUE, _T("mdb"), NULL, OFN_HIDEREADONLY,
						 _T("Access Database Files (*.mdb)|*.mdb||"),
						 this);

		// if user selected a file then open it as a database
		if (IDOK == dlg.DoModal())
		{
			// get full path to database file
			m_strDatabaseName = dlg.GetPathName();

			// display the name of the database on the dialog
			UpdateData(FALSE);

			// now open the database if possible
			int retCode = openDatabase(&m_pDatabase, m_strDatabaseName);
			if (SUCCESS == retCode)
			{
				// once a database is opened, other controls are enabled
				enableControlsForOpenDatabase(TRUE);
			}
			else	// can't interact with an unopened database
			{
				// if database not open, other controls are disabled
				enableControlsForOpenDatabase(FALSE);
			}
		}
	}
}

// helper function --  create a new database
void CDAOTableDlg::createNewDatabase()
{
	// see if user wants to create a new database                 		
	if (IDYES == AfxMessageBox(_T("Specified database doesn't exist.  Do you want to create a new database?"), MB_YESNO))
	{                                                             		
		// display database creation dialog                       		
		CAddDatabaseDlg dlg(&m_pDatabase, m_strDatabaseName);     		
		dlg.DoModal();

		// if the database was opened, then it won't be NULL
		if (m_pDatabase != NULL)  		  	
		{                                                     			
			// once a database is opened, other controls are enabled
			enableControlsForOpenDatabase(TRUE);              			
		}                                                     		
		else  // only other option is to let user try again           		
		{                                                             		
			// if database not open, other controls are disabled
			enableControlsForOpenDatabase(FALSE);             	          		
		}                                                             		
	}
}

// can only interact with tables and queries if you have an open database
// so enable/disable table and query controls conditionally
void CDAOTableDlg::enableControlsForOpenDatabase(BOOL bEnable /*= TRUE*/)
{
	// handle controls subclassed via DDX_Control
	m_QueryNameComboControl.EnableWindow(bEnable);
	m_TableNameComboControl.EnableWindow(bEnable);

	// must enable/disable buttons directly
	CButton *pButton;
	pButton = (CButton *) GetDlgItem(IDC_BUTTON_TABLE_FIELDS);
	if (pButton != NULL)
		pButton->EnableWindow(bEnable);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON_TABLE_INDEXES);
	if (pButton != NULL)
		pButton->EnableWindow(bEnable);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON_DELETE_TABLE);
	if (pButton != NULL)
		pButton->EnableWindow(bEnable);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON_QUERY_DEFINITION);
	if (pButton != NULL)
		pButton->EnableWindow(bEnable);
}

// user selected to close the window without pressing "Done"
// button
void CDAOTableDlg::OnCancel() 
{
	// close the database in case it is open
	closeDatabase(&m_pDatabase);
	
	CDialog::OnCancel();
}

// user selected the "done" button
void CDAOTableDlg::OnOK() 
{
	// close the database in case it is open
	closeDatabase(&m_pDatabase);
	
	CDialog::OnOK();
}

// user selected the "Fields" button to add or view the
// fields that make up the table definition for the specified
// table
void CDAOTableDlg::OnButtonTableFields() 
{
	UpdateData(TRUE);
	
	// table specified                            	
	CDlgAddTable dlg(m_pDatabase, m_strTableName);	
	dlg.DoModal();                                	
}

// user selected the "Indexes" button to add, delete, or view
// indexes for the table specified
void CDAOTableDlg::OnButtonTableIndexes() 
{
	UpdateData(TRUE);
	
	// can only continue if this is an existing table                         	
	// new tables must have fields added first                                	
	if (IsExistentTable(m_pDatabase, m_strTableName))                         	
	{                                                                         	
		// existing table specified, so allow interaction                     	
		CAddIndexDlg dlg(m_pDatabase, m_strTableName);                        	
		dlg.DoModal();                                                        	
	}                                                                         	
	else                                                                      	
		AfxMessageBox(_T("You can only define indexes on existing tables.")); 	
}

// user selected "Definition" of query button to view or delete an
// existing querydef or add a new one 
void CDAOTableDlg::OnButtonQueryDefinition() 
{
	UpdateData(TRUE);
	
	// query specified, so view or create as appropriate      	                           	
	CAddQueryDlg dlg(m_pDatabase, m_strQueryName);     	      	
	dlg.DoModal();
	
	// clear the query name combobox since user may have
	// changed the name
	m_strQueryName = "";
	UpdateData(FALSE);
}

// user wants to browse existing tables so fill in the 
// combobox
void CDAOTableDlg::OnDropdownComboTableName() 
{
	int index = 0;
	CDaoTableDefInfo TableInfo;

	// clear out any items in control
	m_TableNameComboControl.ResetContent();

	// while there are tables to get information on, continue
	// this is a get info by index, not name and no error
	// reporting is done (specified by FALSE)
	while (getTableInfo(m_pDatabase, &TableInfo, index, FALSE))
	{
		// add to list box if not a system object (i.e. system table)
		// or hidden object
		if (!(TableInfo.m_lAttributes & dbSystemObject)
			&& !(TableInfo.m_lAttributes & dbHiddenObject))
			m_TableNameComboControl.AddString(TableInfo.m_strName);

		// continue loop 
		index += 1;
	}
}

// user wants to browse existing queries so fill in the 
// combobox
void CDAOTableDlg::OnDropdownComboQueryName() 
{
	int index = 0;
	CDaoQueryDefInfo QueryInfo;

	// clear out any items in control
	m_QueryNameComboControl.ResetContent();

	// while there are tables to get information on, continue
	// this is a get info by index, not name and no error
	// reporting is done (specified by FALSE)
	while (getQueryInfo(m_pDatabase, &QueryInfo, index, FALSE))
	{
		// add to list box
		m_QueryNameComboControl.AddString(QueryInfo.m_strName);

		// continue loop 
		index += 1;
	}
}

// user selected to delete the current tabledef--prompt
// for acceptance before proceeding
void CDAOTableDlg::OnButtonDeleteTable() 
{
	UpdateData(TRUE);
	
	// can only delete existing tables
	if (IsExistentTable(m_pDatabase, m_strTableName))
	{
		// is user sure?
		if (IDYES == AfxMessageBox(_T("Delete current table?  This will delete all fields and indexes for this table."), MB_YESNO))
		{
			// delete the table
			deleteTable(m_pDatabase, m_strTableName);
			
			// clear the table name
			m_strTableName = _T("");

			// update the dialog controls to erase deleted field
			UpdateData(FALSE);
		}
	}
}

