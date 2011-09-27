// cntritem.h : interface of the CCntrItem class
//

class CContainDoc;
class CContainView;

class CCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CCntrItem)

// Constructors
public:
	CCntrItem(CContainDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CRect m_rect;   // position within the document
	CContainDoc* GetDocument()
		{ return (CContainDoc*)COleClientItem::GetDocument(); }
	CContainView* GetActiveView()
		{ return (CContainView*)COleClientItem::GetActiveView(); }
// Operations
	void InvalidateItem();
	void UpdateFromServerExtent();

// Implementation
public:
	~CCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);

protected:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
};

/////////////////////////////////////////////////////////////////////////////
