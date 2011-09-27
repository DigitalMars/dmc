// bookvw.h : interface of the CBookView class
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


// Class CRowView implements a generic row-based scroll view.
// This derived class, CBookView, implements the details specific
// to the check book application.
/////////////////////////////////////////////////////////////////////////////

// Format of Book View line 1:
#define CHECKNO_COL 0
#define CHECKNO_LEN 4
#define DATE_COL    (CHECKNO_COL + CHECKNO_LEN + 1)
#define DATE_LEN    8
#define PAYTO_COL   (DATE_COL + DATE_LEN + 1)
#define PAYTO_LEN   40
#define AMOUNT_COL  (PAYTO_COL + PAYTO_LEN + 1)
#define AMOUNT_LEN  11  // ########.##
#define ROW_WIDTH   (AMOUNT_COL + AMOUNT_LEN)
// Format of Book View line 2:
#define MEMO_COL    PAYTO_COL
#define MEMO_LEN    40


class CBookView : public CRowView
{
	DECLARE_DYNCREATE(CBookView)
public:
	CBookView();

// Attributes
public:
	CChkBookDoc* GetDocument()
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChkBookDoc)));
			return (CChkBookDoc*) m_pDocument;
		}

// Overrides of CView
	void OnUpdate(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);

// Overrides of CRowView
	void GetRowWidthHeight(CDC* pDC, int& nRowWidth, int& nRowHeight);
	int GetActiveRow();
	int GetRowCount();
	void OnDrawRow(CDC* pDC, int nRowNo, int y, BOOL bSelected);
	void ChangeSelectionNextRow(BOOL bNext);
	void ChangeSelectionToRow(int nRow);

// Implementation
protected:
	virtual ~CBookView() {}
};
