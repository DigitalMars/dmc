// realtdoc.h : interface of the CRealtimeDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CRealtimeDoc : public CDocument
{
static   void CALLBACK EXPORT  TimerProc(  HWND, UINT, UINT, DWORD );

protected: // create from serialization only
	CRealtimeDoc();
	DECLARE_DYNCREATE(CRealtimeDoc)

// Attributes
private:
	CWnd m_wndTimer;
	UINT m_uTimer;

// Operations
public:
	void Iterate(void);
	CMapStringToPtr m_mapList;
// Implementation
public:
	virtual ~CRealtimeDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual BOOL    OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CRealtimeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
