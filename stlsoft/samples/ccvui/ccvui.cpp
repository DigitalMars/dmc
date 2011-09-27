// ccvui.cpp : Defines the class behaviors for the application.
//
// Updated: 30th May 2004
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ccvui.h"
#include "ccvuiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCcvUiApp

BEGIN_MESSAGE_MAP(CCcvUiApp, CWinApp)
	//{{AFX_MSG_MAP(CCcvUiApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcvUiApp construction

CCcvUiApp::CCcvUiApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCcvUiApp object

CCcvUiApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCcvUiApp initialization

BOOL CCcvUiApp::InitInstance()
{
	com_initialiser	coinit;

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCcvDlg dlg;
	m_pMainWnd = &dlg;

    int nResponse = dlg.DoModal();

    if (nResponse == IDOK)
	{
        // Dialog exited with OK status
    }
	else if (nResponse == IDCANCEL)
	{
		// Dialog exited with CANCEL status
	}

	return FALSE;
