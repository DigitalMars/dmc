// cntritem.cpp : implementation of the CCntrItem class
//

#include "stdafx.h"
#include "contain.h"

#include "contrdoc.h"
#include "cntritem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCntrItem implementation

IMPLEMENT_SERIAL(CCntrItem, COleClientItem, 0)

CCntrItem::CCntrItem(CContainDoc* pContainer)
	: COleClientItem(pContainer)
{
	m_rect.SetRect(10, 10, 50, 50);
}

CCntrItem::~CCntrItem()
{
	// TODO: add cleanup code here
}


void CCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	// TODO: invalidate the item by calling UpdateAllViews
	//  (with hints appropriate to your application)

	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

BOOL CCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CCntrItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleClientItem::SetItemRects to move the item
	//  to the new position.

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	GetDocument()->UpdateAllViews(NULL);
	m_rect = rectPos;

	// mark document as dirty
	GetDocument()->SetModifiedFlag();

	return TRUE;
}

void CCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// return rect relative to client area of view
	rPosition = m_rect;
}

void CCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

	// Close an in-place active item whenever it removes the user
	//  interface.  The action here should match as closely as possible
	//  to the handling of the escape key in the view.

	Deactivate();   // nothing fancy here -- just deactivate the object
}

void CCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleClientItem data.
	// Since this sets up the m_pDocument pointer returned from
	//  CCntrItem::GetDocument, it is a good idea to call
	//  the base class Serialize first.
	COleClientItem::Serialize(ar);

	// now store/retrieve data specific to CCntrItem
	if (ar.IsStoring())
	{
		ar << m_rect;
	}
	else
	{
		ar >> m_rect;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCntrItem diagnostics

#ifdef _DEBUG
void CCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
