// dbctl.h : Declaration of the CDbCtrl OLE control class.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CGenericSet recordset

class CGenericSet : public CRecordset
{
public:
	CGenericSet(CDatabase* pDatabase, CString* pstrColumnName);

// Field/Param Data
	//{{AFX_FIELD(CGenericSet, CRecordset)
	CString	m_strCurrentValue;
	//}}AFX_FIELD


// Implementation
protected:
	CString m_strColumnName;
	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange(CFieldExchange* pFX);
	DECLARE_DYNAMIC(CGenericSet)
};


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl : See dbctl.cpp for implementation.

class CDbCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDbCtrl)

// Constructor
public:
	CDbCtrl();

// Overrides

	// Drawing function
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

// Implementation
protected:
	~CDbCtrl();

	CDatabase 	m_cDatabase;
	CGenericSet* m_pRecordSet;
	
	BOOL	m_bConnected;

	DECLARE_OLECREATE_EX(CDbCtrl)	// Class factory and guid
	DECLARE_OLETYPELIB(CDbCtrl)		// GetTypeInfo
	DECLARE_OLECTLTYPE(CDbCtrl)
	DECLARE_PROPPAGEIDS(CDbCtrl)		// Property page IDs

	// Subclassed control support
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	WNDPROC* GetSuperWndProcAddr(void);
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// Message maps
	//{{AFX_MSG(CDbCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CDbCtrl)
	CString m_query;
	afx_msg void OnQueryChanged();
	CString m_dataSource;
	afx_msg void OnDataSourceChanged();
	CString m_tableName;
	afx_msg void OnTableNameChanged();
	CString m_columnName;
	afx_msg void OnColumnNameChanged();
	CString m_userName;
	afx_msg void OnUserNameChanged();
	CString m_password;
	afx_msg void OnPasswordChanged();
	afx_msg void ReQuery();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CDbCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CDbCtrl)
	dispidDataSource = 2L,
	dispidTableName = 3L,
	dispidColumnName = 4L,
	dispidUserName = 5L,
	dispidPassword = 6L,
	dispidQuery = 1L,
	dispidReQuery = 7L,
	//}}AFX_DISP_ID
	};
};
