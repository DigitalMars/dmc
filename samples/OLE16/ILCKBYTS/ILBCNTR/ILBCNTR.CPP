// ilbcntr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ole2.h"
#include "storage.h"
#include "..\memstm.h"

#include "ilbcntr.h"

#include "mainfrm.h"
#include "ilbcndoc.h"
#include "ilbcnvw.h"

#include "initguid.h"

/* 5CEA3D01-DF79-1068-AA3D-08002B2DED80 */
DEFINE_GUID(CLSID_ILBSVR, 0x5CEA3D01, 0xDF79, 0x1068, 0xAA, 0x3D, 0x08, 0x00, 0x2B, 0x2D, 0xED, 0x80);



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrApp

BEGIN_MESSAGE_MAP(CIlbcntrApp, CWinApp)
	//{{AFX_MSG_MAP(CIlbcntrApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TEST_NOCUSTOMMARSHALLING, OnTestNocustommarshalling)
	ON_COMMAND(ID_TEST_USECUSTOMMARSHALLING, OnTestUsecustommarshalling)
	ON_COMMAND(ID_BYTES_1, OnBytes1)
	ON_COMMAND(ID_BYTES_10, OnBytes10)
	ON_COMMAND(ID_BYTES_100, OnBytes100)
	ON_COMMAND(ID_BYTES_1000, OnBytes1000)
	ON_COMMAND(ID_BYTES_10000, OnBytes10000)
	ON_COMMAND(ID_BYTES_2, OnBytes2)
	ON_COMMAND(ID_BYTES_5, OnBytes5)
	ON_UPDATE_COMMAND_UI(ID_BYTES_1, OnUpdateBytes1)
	ON_UPDATE_COMMAND_UI(ID_BYTES_10, OnUpdateBytes10)
	ON_UPDATE_COMMAND_UI(ID_BYTES_100, OnUpdateBytes100)
	ON_UPDATE_COMMAND_UI(ID_BYTES_1000, OnUpdateBytes1000)
	ON_UPDATE_COMMAND_UI(ID_BYTES_10000, OnUpdateBytes10000)
	ON_UPDATE_COMMAND_UI(ID_BYTES_2, OnUpdateBytes2)
	ON_UPDATE_COMMAND_UI(ID_BYTES_5, OnUpdateBytes5)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrApp construction

CIlbcntrApp::CIlbcntrApp()
{
	m_nCount = 1;
}

CIlbcntrApp::~CIlbcntrApp()
{
	if (m_fInitialized)
		OleUninitialize();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIlbcntrApp object

CIlbcntrApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrApp initialization

BOOL CIlbcntrApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();        // set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
			RUNTIME_CLASS(CIlbcntrDoc),
			RUNTIME_CLASS(CMainFrame),     // main SDI frame window
			RUNTIME_CLASS(CIlbcntrView)));


	// create a new (empty) document
	OnFileNew();

	if (m_lpCmdLine[0] != '\0')
	{
		// TODO: add command line processing here
	}

	if (OleInitialize(NULL) == NOERROR)
		m_fInitialized = TRUE;


	return TRUE;
}

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

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
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

// App command to run the dialog
void CIlbcntrApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrApp commands

void CIlbcntrApp::OnTestNocustommarshalling()
{
	LPLOCKBYTES pLockBytes;

	DWORD dwStart = GetTickCount();
	CreateMyILockBytes(FALSE, &pLockBytes);
	TestLockBytes(pLockBytes);
	pLockBytes->Release();
	DWORD dwFinish = GetTickCount();

	char szBuffer[100];

	wsprintf(szBuffer,"The test took %10lu milliseconds", dwFinish - dwStart);

	AfxMessageBox(szBuffer);

	if (m_fPassed)
		AfxMessageBox("Test Successful");
	else
		AfxMessageBox("Test Failed");
}

void CIlbcntrApp::OnTestUsecustommarshalling()
{
	LPLOCKBYTES pLockBytes;
	DWORD dwStart, dwFinish;

	dwStart = GetTickCount();
	CreateMyILockBytes(TRUE, &pLockBytes);
   // CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);
	TestLockBytes(pLockBytes);
	pLockBytes->Release();
	dwFinish = GetTickCount();

	char szBuffer[100];

	wsprintf(szBuffer,"The test took %10lu milliseconds", dwFinish - dwStart);

	AfxMessageBox(szBuffer);

	if (m_fPassed)
		AfxMessageBox("Test Successful");
	else
		AfxMessageBox("Test Failed");
}


void CIlbcntrApp::TestLockBytes(LPLOCKBYTES pLockBytes)
{
	LPSTREAM lpStream;
	LPSTORAGE lpStorage;
	LPPERSISTSTORAGE lpPS;

	m_fPassed = FALSE;

	StgCreateDocfileOnILockBytes( pLockBytes,
								  STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
								  0,
								  &lpStorage);

	if (lpStorage)
		{
		lpStorage->CreateStream ( "Number of Bytes",
								  STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
								  0,
								  0,
								  &lpStream );

		if (lpStream)
			{
			lpStream->Write ( &m_nCount, sizeof (m_nCount), NULL);
			lpStream->Release();
			}

		CoCreateInstance( CLSID_ILBSVR, NULL, CLSCTX_LOCAL_SERVER, IID_IPersistStorage, (LPVOID FAR *) &lpPS);

		if (lpPS)
			{
			lpPS->Save(lpStorage, FALSE);
			lpPS->Release();

			char ch;
			lpStorage->OpenStream("Test",
								  0,
								  STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
								  0,
								  &lpStream);

			if (lpStream)
				{
				for (int i = 0; i < m_nCount; i++)
					{
					HRESULT hErr = lpStream->Read(&ch, 1, NULL);
					if ( (ch != 'A') ||
						 (hErr != NOERROR) )
						goto failed;
					}
				m_fPassed = TRUE;
failed:
				lpStream->Release();
				}

			}

		lpStorage->Release();
		}

}


void CIlbcntrApp::OnBytes1()
{
	m_nCount = 1;
}

void CIlbcntrApp::OnBytes10()
{
	m_nCount = 10;
}

void CIlbcntrApp::OnBytes100()
{
	m_nCount = 100;
}

void CIlbcntrApp::OnBytes1000()
{
	m_nCount = 1000;
}

void CIlbcntrApp::OnBytes10000()
{
	m_nCount = 10000;
}

void CIlbcntrApp::OnBytes2()
{
	m_nCount = 2;
}

void CIlbcntrApp::OnBytes5()
{
	m_nCount = 5;
}

void CIlbcntrApp::OnUpdateBytes1(CCmdUI* pCmdUI)
{
	if (m_nCount ==1)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes10(CCmdUI* pCmdUI)
{
	if (m_nCount == 10)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes100(CCmdUI* pCmdUI)
{
	if (m_nCount == 100)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes1000(CCmdUI* pCmdUI)
{
	if (m_nCount == 1000)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes10000(CCmdUI* pCmdUI)
{
	if (m_nCount == 10000)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes2(CCmdUI* pCmdUI)
{
	if (m_nCount == 2)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CIlbcntrApp::OnUpdateBytes5(CCmdUI* pCmdUI)
{
	if (m_nCount == 5)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}
