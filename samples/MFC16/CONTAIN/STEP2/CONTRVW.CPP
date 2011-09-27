// contrvw.cpp : implementation of the CContainView class
//

#include "stdafx.h"
#include "contain.h"

#include "contrdoc.h"
#include "cntritem.h"
#include "contrvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContainView

IMPLEMENT_DYNCREATE(CContainView, CView)

BEGIN_MESSAGE_MAP(CContainView, CView)
	//{{AFX_MSG_MAP(CContainView)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT, OnCancelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContainView construction/destruction

CContainView::CContainView()
{
	// TODO: add construction code here
}

CContainView::~CContainView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CContainView drawing

void CContainView::OnDraw(CDC* pDC)
{
	CContainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// draw the OLE items from the list
	POSITION pos = pDoc->GetStartPosition();
	while (pos != NULL)
	{
		// draw the item
		CCntrItem* pItem = (CCntrItem*)pDoc->GetNextItem(pos);
		pItem->Draw(pDC, pItem->m_rect);

		// draw the tracker over the item
		CRectTracker tracker;
		SetupTracker(pItem, &tracker);
		tracker.Draw(pDC);
	}
}

void CContainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: remove this code when final selection model code is written
	m_pSelection = NULL;    // initialize selection

}

void CContainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // invalidate entire window
		Invalidate();
		break;
	case HINT_UPDATE_ITEM:      // invalidate single item
		{
			CRectTracker tracker;
			SetupTracker((CCntrItem*)pHint, &tracker);
			CRect rect;
			tracker.GetTrueRect(rect);
			InvalidateRect(rect);
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CContainView printing

BOOL CContainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CContainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CContainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CContainView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CContainView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CCntrItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		CContainDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data.
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);

		pItem->UpdateLink();
		pItem->UpdateFromServerExtent();

		// If item created from class list (not from file) then launch
		//  the server to edit the item.
		if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted.

		// TODO: reimplement selection as appropriate for your application

		SetSelection(pItem);
		pItem->InvalidateItem();
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.
void CContainView::OnCancelEdit()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CContainView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CContainView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

CCntrItem* CContainView::HitTestItems(CPoint point)
{
	CContainDoc* pDoc = GetDocument();
	CCntrItem* pItemHit = NULL;
	POSITION pos = pDoc->GetStartPosition();
	while (pos != NULL)
	{
		CCntrItem* pItem = (CCntrItem*)pDoc->GetNextItem(pos);
		if (pItem->m_rect.PtInRect(point))
			pItemHit = pItem;
	}
	return pItemHit;    // return top item at point
}

void CContainView::SetSelection(CCntrItem* pItem)
{
	// close in-place active item
	if (pItem == NULL || m_pSelection != pItem)
	{
		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
		if (pActiveItem != NULL && pActiveItem != pItem)
			pActiveItem->Close();
	}
	// update view to new selection
	if (m_pSelection != pItem)
	{
		if (m_pSelection != NULL)
			OnUpdate(NULL, HINT_UPDATE_ITEM, m_pSelection);

		m_pSelection = pItem;
		if (m_pSelection != NULL)
			OnUpdate(NULL, HINT_UPDATE_ITEM, m_pSelection);
	}
}

void CContainView::SetupTracker(CCntrItem* pItem, CRectTracker* pTracker)
{
	pTracker->m_rect = pItem->m_rect;

	if (pItem == m_pSelection)
		pTracker->m_nStyle |= CRectTracker::resizeInside;

	if (pItem->GetType() == OT_LINK)
		pTracker->m_nStyle |= CRectTracker::dottedLine;
	else
		pTracker->m_nStyle |= CRectTracker::solidLine;

	if (pItem->GetItemState() == COleClientItem::openState ||
		pItem->GetItemState() == COleClientItem::activeUIState)
	{
		pTracker->m_nStyle |= CRectTracker::hatchInside;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CContainView diagnostics

#ifdef _DEBUG
void CContainView::AssertValid() const
{
	CView::AssertValid();
}

void CContainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CContainDoc* CContainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CContainDoc)));
	return (CContainDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CContainView message handlers

void CContainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCntrItem* pItemHit = HitTestItems(point);
	SetSelection(pItemHit);

	if (pItemHit != NULL)
	{
		CRectTracker tracker;
		SetupTracker(pItemHit, &tracker);

		UpdateWindow();
		if (tracker.Track(this, point))
		{
			pItemHit->InvalidateItem();
			pItemHit->m_rect = tracker.m_rect;
			pItemHit->InvalidateItem();
			GetDocument()->SetModifiedFlag();
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CContainView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);

	if (m_pSelection != NULL)
	{
		m_pSelection->DoVerb(GetKeyState(VK_CONTROL) < 0 ?
			OLEIVERB_OPEN : OLEIVERB_PRIMARY, this);
	}

	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CContainView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && m_pSelection != NULL)
	{
		// give the tracker for the selection a chance
		CRectTracker tracker;
		SetupTracker(m_pSelection, &tracker);
		if (tracker.SetCursor(this, nHitTest))
			return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CContainView::OnEditClear()
{
	if (m_pSelection != NULL)
	{
		m_pSelection->Delete();
		m_pSelection = NULL;
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CContainView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pSelection != NULL);

}
void CContainView::OnEditCopy()
{
	if (m_pSelection != NULL)
		m_pSelection->CopyToClipboard();
}

void CContainView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pSelection != NULL);
}

void CContainView::OnEditPaste()
{
	CCntrItem* pItem = NULL;

	TRY
	{
		// Create new item connected to this document.
		CContainDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from clipboard data
		if (!pItem->CreateFromClipboard())
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);

		// update the size before displaying
		pItem->UpdateFromServerExtent();

		// set selection to newly pasted item
		SetSelection(pItem);
		pItem->InvalidateItem();
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH
}
