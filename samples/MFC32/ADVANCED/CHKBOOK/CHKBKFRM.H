// chkbkfrm.h : interface of the CCheckBookFrame class
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

// We override CMDIChildWnd to customize the MDI child's title bar.
// By default the title bar shows the document name.  But we want
// it to instead show the text defined as the first string in
// the document template STRINGTABLE resource.  This string is
// "Book" in the case of the MDI child window that embeds the book
// view, and similarly "Check" for the other MDI child window.  If
// we didn't customize the title bar, the two MDI child windows would
// show MYCHECKS:1 and MYCHECKS:2 if the document were named MYCHECKS.

/////////////////////////////////////////////////////////////////////////////

class CCheckBookFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCheckBookFrame)
protected:
	BOOL PreCreateWindow(CREATESTRUCT& cs);
};
