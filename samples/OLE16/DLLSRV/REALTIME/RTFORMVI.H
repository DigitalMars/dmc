// rtformvi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRTFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CRTFormView : public CFormView
{
	DECLARE_DYNCREATE(CRTFormView)
protected:
	CRTFormView();          // protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CRTFormView)
	enum { IDD = IDD_FORMVIEW };
	CListBox    m_list;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Implementation
protected:
	virtual ~CRTFormView();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate( CView * pSender, LPARAM lHint, CObject * pHint );
	virtual BOOL UpdateData( BOOL bSaveAndValidate);
	// Generated message map functions
	//{{AFX_MSG(CRTFormView)
	afx_msg void OnDblclkList1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
