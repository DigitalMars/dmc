// ilbsvr.h : main header file for the ILBSVR application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrApp:
// See ilbsvr.cpp for the implementation of this class
//

class CIlbsvrApp : public CWinApp
{
private:
	BOOL m_fInit;
	DWORD m_dw;

public:
	CIlbsvrApp();
	~CIlbsvrApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CIlbsvrApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
