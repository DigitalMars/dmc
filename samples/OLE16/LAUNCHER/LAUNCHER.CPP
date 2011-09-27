//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: launcher.cpp
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include "classfct.h"
#include "launcher.h"


/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
  ON_WM_CLOSE()
  ON_MESSAGE(WM_UPDATE_CONTAINER, OnUpdateContainer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


CTheApp NEAR theApp;


CMainWindow::CMainWindow()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
	LoadAccelTable( "MainAccelTable" );
	Create( NULL, "OLE 2.0 InProc Object Launcher",
		WS_OVERLAPPEDWINDOW, rectDefault, NULL, NULL );

  CMenu* pMenu = GetSystemMenu(FALSE);
  pMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

} /* CMainWindow()
   */


HRESULT CMainWindow::
  CreateClassObject(REFCLSID rclsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;

  m_pClassFactory = new CClassFactory();

  hRes = m_pClassFactory->CreateClassObject(GetSafeHwnd(), rclsid);

  if (hRes == NOERROR) {
	hRes = CoRegisterClassObject(rclsid, (LPCLASSFACTORY)m_pClassFactory,
								 CLSCTX_LOCAL_SERVER,
								 REGCLS_SINGLEUSE, &m_dwRegister);
  }

  return hRes;

} /* CreateClassObject()
   */


void CMainWindow::OnClose()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  while (m_pClassFactory->CanRevokeNow() == FALSE) {
	::Yield();
  }

  m_pClassFactory->Release(); // so it can go away by itself later.
  m_pClassFactory = NULL;

  CoRevokeClassObject(m_dwRegister);

  DestroyWindow();

} /* OnClose()
   */


LONG
  CMainWindow::OnUpdateContainer(UINT /*wParam*/, LONG /*lParam*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;

  if (m_pClassFactory) {
	hRes = m_pClassFactory->UpdateContainer();
  }

  return 0L;
} /* OnUpdateContainer()
   */


BOOL CTheApp::InitInstance()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
	SetDialogBkColor();     // hook gray dialogs (was default in MFC V1)

  HRESULT hRes = OleInitialize(NULL);


  CLSID clsid;
  if (CLSIDFromString(SZCLASSID, &clsid) != NOERROR) {
	return FALSE;
  }

  m_pMainWnd = new CMainWindow();

  ((CMainWindow *)m_pMainWnd)->CreateClassObject(clsid);
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();

	return TRUE;

} /* InitInstance()
   */


int CTheApp::ExitInstance()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  OleUninitialize();
  return CWinApp::ExitInstance();

} /* ExitInstance()
   */
