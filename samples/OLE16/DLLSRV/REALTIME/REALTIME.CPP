// realtime.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <ole2ver.h>
#define INITGUIDS

#include "utils.h"
#include "mainfrm.h"
#include "realtdoc.h"
#include "realtime.h"
#include "rtformvi.h"
#include "rtcntr.h"

#include <string.h>
#include "..\prgidmon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


extern WORD cfRealtimeItem;
/////////////////////////////////////////////////////////////////////////////
// CRealtimeApp

BEGIN_MESSAGE_MAP(CRealtimeApp, CWinApp)
	//{{AFX_MSG_MAP(CRealtimeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealtimeApp construction

CRealtimeApp::CRealtimeApp()
{
	m_dwRegCF = 0;
	m_pClassFactory = NULL;
	m_fInitialized = FALSE;
	m_pDoc = NULL;
}

CRealtimeApp::~CRealtimeApp(void)
{
	if (m_dwRegCF != 0)
		CoRevokeClassObject(m_dwRegCF);
	if (NULL != m_pClassFactory)
		m_pClassFactory->Release();
//  if (NULL != m_pDoc) delete m_pDoc;
	if (m_fInitialized)
		OleUninitialize();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRealtimeApp object

CRealtimeApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CRealtimeApp initialization

BOOL CRealtimeApp::InitApplication()
{
	//  OLE initialization section
	int cMsg = 96;   // recommend msg queue size for OLE
	while (cMsg && ! SetMessageQueue(cMsg))  // take largest size we can get.
		cMsg -= 8;
	if (! cMsg)
		return FALSE;  // ERROR: we got no message queue
	return TRUE;
}

BOOL CRealtimeApp::InitInstance()
{
	DWORD dwVer;
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	cfRealtimeItem = RegisterClipboardFormat( "RealtimeItem" );

	SetDialogBkColor();        // set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
			RUNTIME_CLASS(CRealtimeDoc),
			RUNTIME_CLASS(CMainFrame),     // main SDI frame window
			RUNTIME_CLASS(CRTFormView)));


	m_fUser = TRUE;
	if (m_lpCmdLine[0] != '\0')
	{
		if (_fstrstr(m_lpCmdLine, "Embedding"))
			m_fUser = FALSE;
	}
	if (!m_fUser) m_nCmdShow = SW_SHOWMINIMIZED;


	// create a new (empty) document
	OnFileNew();

	dwVer = CoBuildVersion();
	if (rmm != HIWORD(dwVer))
		return FALSE;
	if (FAILED(OleInitialize(NULL)))
		return FALSE;
	m_fInitialized = TRUE;
	m_pClassFactory = new CRTClassFactory;
	if (m_pClassFactory == NULL)
		return FALSE;
	m_pClassFactory->AddRef();
	if (FAILED(CoRegisterClassObject(CLSID_CRealtime,
		(LPUNKNOWN)m_pClassFactory,
		CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE,
		&m_dwRegCF)))
		return FALSE;
	return TRUE;
}


void CRealtimeApp::Dump(CDumpContext& dc) const
{
	dc << "CRealtimeApp";
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
void CRealtimeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRealtimeApp commands

CRTClassFactory::CRTClassFactory(void) :  m_parseDisplayName(this)
{
	m_refs = 1;
	m_pContainer = NULL;
}


STDMETHODIMP CRTClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
{
	*ppv = NULL;
	if (riid == IID_IUnknown || riid == IID_IClassFactory)
		*ppv = (LPVOID)this;
	else if (riid == IID_IParseDisplayName )
		*ppv = (LPVOID)&m_parseDisplayName;

	if (*ppv != NULL)
	{
		((LPUNKNOWN)(*ppv))->AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
};


STDMETHODIMP_(ULONG) CRTClassFactory::AddRef(void)
{
	return m_refs++;
}


STDMETHODIMP_(ULONG) CRTClassFactory::Release(void)
{
	m_refs--;
	if (m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}


STDMETHODIMP CRTClassFactory::CreateInstance ( LPUNKNOWN pUnkOuter,
		REFIID riid, LPVOID FAR * ppvObj)
{
	HRESULT hresult;

	*ppvObj = NULL;
	if (m_pContainer == NULL)
	{
		m_pContainer = new CRTContainer(pUnkOuter, theApp.m_pDoc );
		if (m_pContainer == NULL) return ResultFromScode(E_OUTOFMEMORY);
		hresult = m_pContainer->QueryInterface( riid, ppvObj );
		m_pContainer->Release();
		m_pContainer->Register();
	}
	else
	{
		hresult = m_pContainer->QueryInterface( riid, ppvObj );
	}
	if (theApp.m_fUser) CoLockObjectExternal(m_pContainer, TRUE, TRUE);
	return hresult;
}


STDMETHODIMP CRTClassFactory::LockServer (BOOL fLock)
{
	return NOERROR;
}


CRTClassFactory::CParseDisplayName::CParseDisplayName(CRTClassFactory FAR* pClassFactory)
	: m_pClassFactory(pClassFactory)
	{}

STDMETHODIMP CRTClassFactory::CParseDisplayName::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pClassFactory->QueryInterface(riid, ppvObj);
}


STDMETHODIMP_(ULONG) CRTClassFactory::CParseDisplayName::AddRef(void)
{
	return m_pClassFactory->AddRef();
}


STDMETHODIMP_(ULONG) CRTClassFactory::CParseDisplayName::Release(void)
{
	return m_pClassFactory->Release();
}

// *** IParseDisplayName method ***
STDMETHODIMP CRTClassFactory::CParseDisplayName::ParseDisplayName( LPBC pbc, LPSTR lpszDisplayName,
		  ULONG FAR* pchEaten, LPMONIKER FAR* ppmkOut)
{
	//  Assert the string starts out with "@Realtime\", return a progid
	//  moniker composed with an item moniker made with the rest of the string.
	LPSTR   szExpected = "Realtime";
	LPMONIKER pmkProgId;
	LPMONIKER pmkItem;
	LPSTR psz1 = lpszDisplayName;
	LPSTR psz2 = szExpected;
	HRESULT hresult;

	*ppmkOut = NULL;
	*pchEaten = 0;

	if (psz1 == NULL || *psz1 != '@')
		return ResultFromScode(MK_E_SYNTAX);

	psz1++;
	while (*psz2) {
		if (AnsiUpper((char FAR *)(*psz1))!=AnsiUpper((char FAR*)(*psz2)))
			return ResultFromScode(MK_E_SYNTAX);
		psz1 = AnsiNext(psz1);
		psz2 = AnsiNext(psz2);
	}

	hresult = CreateProgIdMoniker(szExpected, &pmkProgId);
	if (FAILED(hresult))
		return hresult;

	if (*psz1 == '\0') {
		*ppmkOut = pmkProgId;
		*pchEaten = lstrlen(lpszDisplayName);
		return NOERROR;
	}

	hresult = pmkProgId->ParseDisplayName(pbc, NULL, psz1, pchEaten,&pmkItem);
	if (FAILED(hresult)) {
		pmkProgId->Release();
		return hresult;
	}

	hresult = pmkProgId->ComposeWith(pmkItem, FALSE, ppmkOut);
	pmkItem->Release();
	pmkProgId->Release();
	if (hresult == NOERROR && pchEaten)
		*pchEaten = lstrlen(lpszDisplayName);
	return hresult;
}


void CRealtimeApp::OnUpdateAppExit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(Exitable());
}


BOOL CRealtimeApp::Exitable(void)
{
	if (m_fUser) return
		m_pClassFactory->m_pContainer != NULL ?
		m_pClassFactory->m_pContainer->Exitable() : TRUE;
	return FALSE;
}
