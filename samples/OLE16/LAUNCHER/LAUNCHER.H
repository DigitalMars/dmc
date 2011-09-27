//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  classfct.h
//  date:
//  author:  tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING LAUNCHER.H from " __FILE__)
#endif  /* RC_INVOKED */

/////////////////////////////////////////////////////////////////////////////

#define SZCLASSID   "{A11CC4E0-C0E1-1068-B87C-00DD0107D0BD}"
//#define SZCLASSID "{A11CC4E0-C0E1-1068-B87C-00DD0107D0BE}"

class CMainWindow : public CFrameWnd
{
private:
  DWORD m_dwRegister;
  CClassFactory *m_pClassFactory;

public:
	CMainWindow();

  HRESULT CreateClassObject(REFCLSID rclsid);

	afx_msg void OnClose();
  afx_msg LONG OnUpdateContainer(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

}; /* CMainWindow */


/////////////////////////////////////////////////////////////////////////////


class CTheApp : public CWinApp
{
public:
	BOOL InitInstance();
	int  ExitInstance();

}; /* CTheApp */


/////////////////////////////////////////////////////////////////////////////

#endif // __LAUNCHER_H__
