// ilbcntr.h : main header file for the ILBCNTR application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Storage.h"

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrApp:
// See ilbcntr.cpp for the implementation of this class
//

class CIlbcntrApp : public CWinApp
{
public:
	CIlbcntrApp();
	~CIlbcntrApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CIlbcntrApp)
	afx_msg void OnAppAbout();
	afx_msg void OnTestNocustommarshalling();
	afx_msg void OnTestUsecustommarshalling();
	afx_msg void OnBytes1();
	afx_msg void OnBytes10();
	afx_msg void OnBytes100();
	afx_msg void OnBytes1000();
	afx_msg void OnBytes10000();
	afx_msg void OnBytes2();
	afx_msg void OnBytes5();
	afx_msg void OnUpdateBytes1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes100(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes1000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes10000(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBytes5(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void TestLockBytes(LPLOCKBYTES pLockBytes);

private:
	int m_nCount;
	BOOL m_fInitialized;
	BOOL m_fPassed;

};


/////////////////////////////////////////////////////////////////////////////
