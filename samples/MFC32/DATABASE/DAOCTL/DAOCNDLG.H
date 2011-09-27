// DAOCnDlg.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CDAOContDlg dialog
//{{AFX_INCLUDES()
#include "daoedit.h"
#include "accpict.h"
//}}AFX_INCLUDES

class CDAOContDlg : public CDialog
{
// Construction
public:
	CDAOContDlg(CWnd* pParent = NULL);	// standard constructor
	~CDAOContDlg();

// Dialog Data
	//{{AFX_DATA(CDAOContDlg)
	enum { IDD = IDD_DAOCONT_DIALOG };
	CStatic	m_ctlRecNum;
	CSpinButtonCtrl	m_ctlSpin;
	CTreeCtrlEx	m_ctlTree;
	CAccessPict	m_ctlPicture;
	BOOL	m_bAllowEdits;
	//}}AFX_DATA
	CStatic		m_ctlColName[4];
	CDaoEdit	m_ctlEdit[4];

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDAOContDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	enum ImageIDS{
		IID_DATABASE = 0,IID_DATABASES,
		IID_TABLE,IID_TABLES,
		IID_FIELD,IID_FIELDS,
		IID_INDEX,IID_INDEXES,
		IID_RELATION,IID_RELATIONS,
		IID_QUERYDEF,IID_QUERYDEFS,
		IID_COLLECT,IID_FOLDER,
		IID_GOODFIELD,IID_BADFIELD,
		IID_PICTURE
	};

	HICON m_hIcon;
	CImageList m_ctlImage;
	CDaoDatabase* m_pDB;
	CDaoRecordset* m_pRS;
	CTreeCursor m_cursDB,m_cursTDS,m_cursQDS;
	CTreeCursor m_cursTD,m_cursQD;
	BOOL m_bODBC;
	UINT m_nEditIndex;
	UINT m_nRecNum;
	CString m_strTableName;
	BOOL m_bUpdating;
	CString m_strPictCol;
	
	// Generated message map functions
	//{{AFX_MSG(CDAOContDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnectDao();
	afx_msg void OnConnectOdbc();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnRequestEdit(UINT nID, BOOL* pBool);
	afx_msg BOOL OnChanged(UINT nID);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	
	void PopulateTree();
	void AddItem(WORD nItemType, LPCTSTR lpszName);
	void SetRecordset(LPCTSTR strTableName);
	void UpdateFields();
	void Clear();
	
	DECLARE_MESSAGE_MAP()
};
