// addform.h : interface of the CAddForm class
//
/////////////////////////////////////////////////////////////////////////////

class CAddForm : public CRecordView
{
protected:
	CAddForm(UINT nIDTemplate);
	DECLARE_DYNAMIC(CAddForm)

protected:
	BOOL m_bAddMode;

// Operations
public:
	virtual BOOL OnMove(UINT nIDMoveCommand);
	virtual BOOL RecordAdd();
	virtual BOOL RecordRefresh();
	virtual BOOL RecordDelete();

// Implementation
public:
	virtual ~CAddForm();
// Generated message map functions
protected:
	afx_msg void OnRecordAdd();
	afx_msg void OnRecordRefresh();
	afx_msg void OnRecordDelete();
	afx_msg void OnUpdateRecordFirst(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};
