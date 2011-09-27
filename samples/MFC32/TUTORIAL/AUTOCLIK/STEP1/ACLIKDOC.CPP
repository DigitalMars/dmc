// AClikDoc.cpp : implementation of the CAutoClickDoc class
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

#include "stdafx.h"
#include "AutoClik.h"

#include "AClikDoc.h"
#include "Dialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoClickDoc

IMPLEMENT_DYNCREATE(CAutoClickDoc, CDocument)

BEGIN_MESSAGE_MAP(CAutoClickDoc, CDocument)
	//{{AFX_MSG_MAP(CAutoClickDoc)
	ON_COMMAND(ID_EDIT_CHANGETEXT, OnEditChangetext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAutoClickDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CAutoClickDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAClick to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {47D53E05-CC33-11CE-8F35-00DD01109044}
static const IID IID_IAClick =
{ 0xfc866850, 0x9F96, 0x11ce, { 0xb0, 0xf2, 0x00, 0xaa, 0x00, 0x6c, 0x28, 0xb3 } };

BEGIN_INTERFACE_MAP(CAutoClickDoc, CDocument)
	INTERFACE_PART(CAutoClickDoc, IID_IAClick, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoClickDoc construction/destruction

CAutoClickDoc::CAutoClickDoc()
{
	EnableAutomation();
	m_pt = CPoint(10, 10);
	m_str = _T("Automation!");

	AfxOleLockApp();
}

CAutoClickDoc::~CAutoClickDoc()
{
	AfxOleUnlockApp();
}

BOOL CAutoClickDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CAutoClickDoc::Refresh()
{
	UpdateAllViews(NULL);
	SetModifiedFlag();
}

/////////////////////////////////////////////////////////////////////////////
// CAutoClickDoc serialization

void CAutoClickDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_pt << m_str;
	}
	else
	{
		ar >> m_pt >> m_str;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAutoClickDoc diagnostics

#ifdef _DEBUG
void CAutoClickDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAutoClickDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoClickDoc commands

void CAutoClickDoc::OnEditChangetext() 
{
	CChangeText dlg;
	dlg.m_str = m_str;
	if (dlg.DoModal())
	{
		m_str = dlg.m_str;
		Refresh();
	}
}
