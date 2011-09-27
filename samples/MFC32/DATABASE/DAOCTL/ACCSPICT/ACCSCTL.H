// AccsCtl.h : Declaration of the CAccessPictCtrl OLE control class.


/////////////////////////////////////////////////////////////////////////////
// struct OLEOBJECTHEADER : Access Object Header Structure

struct OLEOBJECTHEADER
{
	WORD typ;		// Type signature (0x1C15)
	WORD cbHdr;		// sizeof(struct OLEOBJECTHEADER) + cchName + cchClass
	LONG lobjType;	// OLE Object Type Code (OT_STATIC, OT_LINKED, OT_EMBEDDED)
	WORD cchName;	// Count of characters in object Name (CchSz(szName) + 1))
	WORD cchClass;	// Count of characters in class Name (CchSz(szClss) + 1))
	WORD ibName;	// Offset of object name in structure (sizeof(OLEOBJECTHEADER)
	WORD ibClass;	// Offset of class name in structure (ibName + cchName)
	POINTS ptSize;	// Original size of Object (MM_HIMETRIC)
};


/////////////////////////////////////////////////////////////////////////////
// struct OLE1STREAM : OLESTREAM struct w/ knowledge of the Access data

typedef struct 
{
	LPOLESTREAMVTBL pvt;		// OLESTREAMVTBL pointer
	LPBYTE			lpData;		// Link to Data in .MDB file
	DWORD			dwSize;		// OLESTREAM length (relative to position)
} OLE1STREAM, FAR* LPOLE1STREAM;	


/////////////////////////////////////////////////////////////////////////////
// OLESTREAMVTBL Get and Put callback functions

DWORD FAR PASCAL Get(LPOLESTREAM pstm, void FAR* pb, DWORD cb);
DWORD FAR PASCAL Put(LPOLESTREAM pstm, const void FAR* pb, DWORD cb);

/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl : See AccsCtl.cpp for implementation.

class CAccessPictCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAccessPictCtrl)

// Constructor
public:
	CAccessPictCtrl();

// Overrides

	// Drawing function
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

// Attributes
protected:
	CRect CalcRectSize();
	// Attributes of Access Object
	BOOL			m_bIsObject;
	CStreamDib		m_StreamDib;
	CSize			m_size;
	
	CPictureHolder	m_Pict;
	BOOL			m_bPictLoaded;
	
// Implementation
protected:
	// State Cleanup Function
	void ResetState();

	// Drawing Helper
	CRect CalcRectSize(const CRect& rect);

	~CAccessPictCtrl();

	DECLARE_OLECREATE_EX(CAccessPictCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAccessPictCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAccessPictCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAccessPictCtrl)		 // Type name and misc status

	// Data Read Functions
	BOOL ReadObject(SAFEARRAY FAR* psaObject);
	LPSTORAGE ReadObjectHeader(SAFEARRAY FAR* psaObject);	
	BOOL ReadObjectData(LPSTREAM lpStream);		
	
	// OLESTREAM Function Helpers
	LPOLE1STREAM	CreateOleStream(LPBYTE lpData, DWORD dwSize);
	void			DeleteOleStream(LPOLE1STREAM pStream);
	DWORD			GetStreamSize(SAFEARRAY FAR* psaObject, DWORD dwOffset);	
	

// Message maps
	//{{AFX_MSG(CAccessPictCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CAccessPictCtrl)
	BOOL m_bStretch;
	BOOL m_bPreserveRatio;
	afx_msg BOOL GetIsAccessObject();
	afx_msg LPPICTUREDISP GetPicture();
	afx_msg BOOL SetData(VARIANT FAR* pvarData);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CAccessPictCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CAccessPictCtrl)
	dispidIsAccessObject = 3L,
	dispidStretchToFit = 1L,
	dispidPreserveRatio = 2L,
	dispidPicture = 4L,
	dispidSetData = 5L,
	//}}AFX_DISP_ID
	};
};
