// DAOCnDlg.cpp : implementation file
//

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
#include "DAOCont.h"
#include "DAOCnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// variant handling
#ifndef _UNICODE
	#define V_BSTRT(b)	(LPSTR)V_BSTR(b)
#else
	#define V_BSTRT(b)	V_BSTR(b)
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
// CDAOContDlg dialog

CDAOContDlg::CDAOContDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDAOContDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDAOContDlg)
	m_bAllowEdits = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDB = NULL;
	m_pRS = NULL;
	m_bAllowEdits = TRUE;
}

CDAOContDlg::~CDAOContDlg()
{
	if (m_pRS)
		if (m_pRS->IsOpen())
			m_pRS->Close();
	delete m_pRS;

	if (m_pDB)
		if (m_pDB->IsOpen())
			m_pDB->Close();
	
	delete m_pDB;
}

void CDAOContDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDAOContDlg)
	DDX_Control(pDX, IDC_RECNUM, m_ctlRecNum);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin);
	DDX_Control(pDX, IDC_TREE1, m_ctlTree);
	DDX_Control(pDX, IDC_ACCESSPICTCTRL1, m_ctlPicture);
	DDX_Check(pDX, IDC_ALLOW_EDITS, m_bAllowEdits);
	//}}AFX_DATA_MAP
	for (int i=0; i<4; i++)
		DDX_Control(pDX, IDC_COLNAME1 + i, m_ctlColName[i]);
	for (i=0; i<4; i++)
		DDX_Control(pDX, IDC_DAOEDITCTRL1 + i, m_ctlEdit[i]);
}

BEGIN_MESSAGE_MAP(CDAOContDlg, CDialog)
	//{{AFX_MSG_MAP(CDAOContDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_DAO, OnConnectDao)
	ON_BN_CLICKED(IDC_CONNECT_ODBC, OnConnectOdbc)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDAOContDlg message handlers

BOOL CDAOContDlg::OnInitDialog()
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
	
	// Create the Image List
	m_ctlImage.Create(IDB_TREEIMAGE,16,0,RGB(255,0,255));
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	m_ctlTree.SetImageList(&m_ctlImage);

	m_pDB = new CDaoDatabase;
	m_pRS = new CDaoRecordset(m_pDB);
	
	m_ctlTree.ModifyStyle(0,TVS_HASLINES | TVS_HASBUTTONS,0);
	m_ctlTree.GetRootItem().AddTail(_T("No Database loaded!"));
	
	m_ctlSpin.SetRange(0,2);
	m_ctlSpin.SetPos(1);
	m_bUpdating = FALSE;

	Clear();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDAOContDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDAOContDlg::OnPaint() 
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

// The system calls this to obtain the m_cursor to display while the user drags
//  the minimized window.
HCURSOR CDAOContDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDAOContDlg::OnConnectDao() 
{
	m_bODBC = FALSE;
	
	CFileDialog dlgFile(
		TRUE,
		_T(".mdb"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Access Files (*.mdb)|*.mdb|All Files (*.*)|*.*||"));
	if (dlgFile.DoModal() != IDOK)
		return;
	
	m_ctlTree.DeleteAllItems();
	m_ctlTree.GetRootItem().AddTail(_T("No Database loaded!"));

	CWaitCursor wait;
	UpdateWindow();

	if (m_pDB->IsOpen())
		m_pDB->Close();
	
	try
	{
		m_pDB->Open(dlgFile.GetFileName());
	}
	catch (CDaoException* e)
	{
		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
		e->Delete();
	}
	if (m_pDB->IsOpen())
		PopulateTree();
}


void CDAOContDlg::OnConnectOdbc() 
{
	m_bODBC = TRUE;

	m_ctlTree.DeleteAllItems();
	m_ctlTree.GetRootItem().AddTail(_T("No Database loaded!"));

	if (m_pDB->IsOpen())
		m_pDB->Close();
	
	CWaitCursor wait;
	UpdateWindow();
	
	try
	{
		m_pDB->Open(_T(""),FALSE,FALSE,_T("ODBC;"));
	}
	catch (CDaoException* e)
	{
		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
		e->Delete();
	}
	if (m_pDB->IsOpen())
		PopulateTree();
}

void CDAOContDlg::PopulateTree() 
{
	m_ctlTree.DeleteAllItems();

	ASSERT(m_pDB);
	ASSERT(m_pDB->IsOpen());

	// Insert root node by call to tree object this time
	m_cursDB = m_ctlTree.GetRootItem().AddTail(m_pDB->GetName(),IID_DATABASE);

	// From now on call via iterators
	m_cursTDS = m_cursDB.AddTail(_T("Tables"),IID_TABLES);

	CDaoTableDefInfo tabInfo;
	CDaoFieldInfo fieldInfo;
	int nTableDefCount = m_pDB->GetTableDefCount();
	for (int i = 0; i < nTableDefCount; i++)
	{
		m_pDB->GetTableDefInfo(i,tabInfo);
		if (tabInfo.m_lAttributes & dbSystemObject)
			continue;
		AddItem(IID_TABLE,tabInfo.m_strName);
	}

	m_cursDB.Expand();
}

void CDAOContDlg::AddItem(WORD nItemType, LPCTSTR lpszName)
{
	switch (nItemType)
	{
		case IID_TABLE:
		{
			m_cursTD = m_cursTDS.AddTail(lpszName,IID_TABLE);
			CDaoTableDef td(m_pDB);
			try
			{
				td.Open(lpszName);
				CDaoFieldInfo fieldInfo;
				int nFieldCount = td.GetFieldCount();
				for (int j=0; j < nFieldCount; j++)
				{
					td.GetFieldInfo(j,fieldInfo);
					int nImageID;
					switch (fieldInfo.m_nType){
						case dbLongBinary:
							nImageID = IID_PICTURE; break;
						case dbText:
						case dbMemo:
							nImageID = IID_GOODFIELD; break;
						default:
							nImageID = IID_BADFIELD;
					}
					m_cursTD.AddTail(fieldInfo.m_strName, nImageID);
				}
			}
			catch(CDaoException* e)
			{
				e->Delete();
			}
			td.Close();
			break;
		}
	}
}

void CDAOContDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTreeCursor cursSel = m_ctlTree.GetSelectedItem();
	UINT nImageID = cursSel.GetImageID();
	
	CString strTableName;
	strTableName = cursSel.GetParent().GetText();
	switch (nImageID)
	{
		case IID_GOODFIELD:
			SetRecordset(strTableName);
			m_ctlEdit[m_nEditIndex].SetFieldName(cursSel.GetText());
			m_ctlColName[m_nEditIndex].SetWindowText(cursSel.GetText());
			m_nEditIndex = (m_nEditIndex + 1) % 4;
			UpdateFields();
			break;
		case IID_PICTURE:
			SetRecordset(strTableName);
			m_strPictCol = cursSel.GetText();
			UpdateFields();
			break;
		case IID_BADFIELD:
			AfxMessageBox(_T("Field type not supported"));
			break;
	}
	*pResult = 0;
}

void CDAOContDlg::SetRecordset(LPCTSTR strTableName)
{
	if (strTableName == m_strTableName)
		return;
	
	Clear();
	
	try
	{
		CString strQuery = _T("Select * from ");
		strQuery += strTableName;
		m_pRS->Open(dbOpenDynaset,strQuery);
		m_pRS->m_bCheckCacheForDirtyFields = FALSE;
	}
	catch(CDaoException* e)
	{
		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
		e->Delete();
	}
	
	m_strTableName = strTableName;
}

void CDAOContDlg::Clear() 
{
	if (m_pRS->IsOpen())
		m_pRS->Close();
	for (int i=0; i<4; i++){
		m_ctlEdit[i].SetText(_T(""));
		m_ctlEdit[i].SetFieldName(_T(""));
		m_ctlColName[i].SetWindowText(_T(""));
	}
	m_strPictCol = _T("");
	m_ctlPicture.SetData(NULL);
	m_nEditIndex = 0;
	m_strTableName = _T("");
	m_ctlRecNum.SetWindowText(_T(""));
	m_nRecNum = 1;
}

void CDAOContDlg::UpdateFields()
{
	m_bUpdating = TRUE; // Don't want data binding notifications during updates
	CString strRecNum;
	strRecNum.Format(_T("(%d)"),m_nRecNum);
	m_ctlRecNum.SetWindowText(strRecNum);
	for (int i=0; i<4; i++)
	{
		CString strFieldName = _T("[") + m_ctlEdit[i].GetFieldName() + _T("]");
		if (strFieldName != _T("[]"))
		{
			COleVariant var = m_pRS->GetFieldValue(strFieldName);
			m_ctlEdit[i].SetText(CString(V_BSTRT(&var)));
		}
	}
	if (m_strPictCol != _T(""))
	{
			CString strFieldName = _T("[") + m_strPictCol + _T("]");
			COleVariant var = m_pRS->GetFieldValue(strFieldName);
			try
			{
				m_ctlPicture.SetData(&var);
			}
			catch (COleException* e)
			{
				// This is used to catch exceptions thrown
				// by the picture control when it can't display
				// the data we gave it for some reason
				e->Delete();
			}
	}
	m_bUpdating = FALSE;
}


void CDAOContDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Force the current control to receive a WM_KILLFOCUS
	CWnd* wndFocus = GetFocus();
	m_ctlSpin.SetFocus();
	wndFocus->SetFocus();

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (m_pRS->IsOpen())
	{
		if (pNMUpDown->iDelta == 1)
		{
			if (!m_pRS->IsEOF())
			{
				m_pRS->MoveNext();
				m_nRecNum++;
			}
			if (m_pRS->IsEOF())
			{
				m_pRS->MovePrev();
				m_nRecNum--;
			}
		}
		if (pNMUpDown->iDelta == -1)
		{
			if (!m_pRS->IsBOF())
			{
				m_pRS->MovePrev();
				m_nRecNum--;
			}
			if (m_pRS->IsBOF())
			{
				m_pRS->MoveNext();
				m_nRecNum++;
			}
		}
		UpdateFields();
	}
	m_ctlSpin.SetPos(1);
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CDAOContDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDAOContDlg)
	//}}AFX_EVENTSINK_MAP
	ON_PROPNOTIFY_RANGE(CDAOContDlg, IDC_DAOEDITCTRL1, IDC_DAOEDITCTRL4, DISPID_TEXT, OnRequestEdit, OnChanged)
//  ON_PROPNOTIFY_RANGE(theClass, idFirst, idLast, dispid, pfnRequest, pfnChanged)
END_EVENTSINK_MAP()

BOOL CDAOContDlg::OnRequestEdit(UINT nID, BOOL* pBool) 
{
	UpdateData(TRUE);
	if (m_bUpdating)
		*pBool = TRUE;
	else
		*pBool = m_bAllowEdits;

	return TRUE;
}

BOOL CDAOContDlg::OnChanged(UINT nID) 
{
	// Filter out changes due to refreshing the control
	if (m_bUpdating)
		return TRUE;

	if (m_pRS->IsOpen())
	{
		UINT nCtl = nID - IDC_DAOEDITCTRL1;
		CString strFieldName = _T("[") + m_ctlEdit[nCtl].GetFieldName() + _T("]");
		m_pRS->Edit();
		COleVariant var(m_ctlEdit[nCtl].GetText(), VT_BSTRT);
		m_pRS->SetFieldValue(strFieldName, var);
		m_pRS->Update();
	}
	return TRUE;
}
