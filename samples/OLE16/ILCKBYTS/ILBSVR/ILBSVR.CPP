// ilbsvr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ole2.h"
#include "storage.h"

#include "ilbsvr.h"

#include "mainfrm.h"
#include "ilbsvdoc.h"
#include "ilbsvvw.h"

#include "initguid.h"
#include "test.h"

/* 5CEA3D01-DF79-1068-AA3D-08002B2DED80 */
DEFINE_GUID(CLSID_ILBSVR, 0x5CEA3D01, 0xDF79, 0x1068, 0xAA, 0x3D, 0x08, 0x00, 0x2B, 0x2D, 0xED, 0x80);


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrApp

BEGIN_MESSAGE_MAP(CIlbsvrApp, CWinApp)
	//{{AFX_MSG_MAP(CIlbsvrApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrApp construction

CIlbsvrApp::CIlbsvrApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CIlbsvrApp::~CIlbsvrApp()
{
	if (m_fInit)
		{
		CoRevokeClassObject(m_dw);
		OleUninitialize();
		}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIlbsvrApp object

CIlbsvrApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrApp initialization

BOOL CIlbsvrApp::InitInstance()
{
	static CClassFactory FAR * pCF = new CClassFactory;

	if (OleInitialize(NULL) == NOERROR)
		{
		m_fInit = TRUE;
		pCF->AddRef();
		CoRegisterClassObject ( CLSID_ILBSVR, pCF ,CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &m_dw);
		pCF->Release();
		}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();        // set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
			RUNTIME_CLASS(CIlbsvrDoc),
			RUNTIME_CLASS(CMainFrame),     // main SDI frame window
			RUNTIME_CLASS(CIlbsvrView)));


	// create a new (empty) document
	OnFileNew();


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
void CIlbsvrApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrApp commands
