// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <dlgs.h>       // for standard control IDs for commdlg

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern const CString NEAR afxEmptyString;  // for creating empty key strings

/////////////////////////////////////////////////////////////////////////////
// Print/Print Setup dialog

IMPLEMENT_DYNAMIC(CPrintDialog, CDialog)

BEGIN_MESSAGE_MAP(CPrintDialog, CDialog)
	//{{AFX_MSG_MAP(CPrintDialog)
	ON_COMMAND(psh1, OnPrintSetup) // print setup button when print is displayed
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPrintDialog::CPrintDialog(BOOL bPrintSetupOnly,
	DWORD dwFlags /* = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS
		| PD_HIDEPRINTTOFILE | PD_NOSELECTION */,
	CWnd* pParentWnd /* = NULL */)
		: m_pd(m_pdActual), CDialog((UINT)0, pParentWnd)
{
	memset(&m_pdActual, 0, sizeof(m_pdActual));

	m_pd.lStructSize = sizeof(m_pdActual);
	m_pd.Flags = (dwFlags | PD_ENABLEPRINTHOOK | PD_ENABLESETUPHOOK);
	if (_AfxHelpEnabled())
		m_pd.Flags |= PD_SHOWHELP;
	m_pd.lpfnPrintHook = (COMMDLGPROC)_AfxCommDlgProc;
	m_pd.lpfnSetupHook = (COMMDLGPROC)_AfxCommDlgProc;

	if (bPrintSetupOnly)
	{
		m_nIDHelp = AFX_IDD_PRINTSETUP;
		m_pd.Flags |= PD_PRINTSETUP;
	}
	else
	{
		m_nIDHelp = AFX_IDD_PRINT;
		m_pd.Flags |= PD_RETURNDC;
	}

	m_pd.Flags &= ~PD_RETURNIC; // do not support information context
}

// Helper ctor for AttachOnSetup
#ifdef AFX_CLASS_MODEL
CPrintDialog::CPrintDialog(PRINTDLG FAR& pdInit)
#else
CPrintDialog::CPrintDialog(PRINTDLG& pdInit)
#endif
		: m_pd(pdInit), CDialog((UINT)0, NULL)
{
}

// Function to keep m_pd in sync after user invokes Setup from
// the print dialog (via the Setup button)
// If you decide to handle any messages/notifications and wish to
// handle them differently between Print/PrintSetup then override
// this function and create an object of a derived class
CPrintDialog* CPrintDialog::AttachOnSetup()
{
	ASSERT_VALID(this);

	CPrintDialog* pDlgSetup;

	pDlgSetup = new CPrintDialog(m_pd);
	pDlgSetup->m_hWnd = NULL;
	pDlgSetup->m_pParentWnd = m_pParentWnd;
	pDlgSetup->m_nIDHelp = AFX_IDD_PRINTSETUP;
	return pDlgSetup;
}

void CPrintDialog::OnPrintSetup()
{
	ASSERT_VALID(this);

	CPrintDialog* pDlgSetup = AttachOnSetup();
	ASSERT(pDlgSetup != NULL);

	_AfxHookWindowCreate(pDlgSetup);
	Default();
	_AfxUnhookWindowCreate();

	delete pDlgSetup;
}

int CPrintDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_pd.Flags & PD_ENABLEPRINTHOOK);
	ASSERT(m_pd.Flags & PD_ENABLESETUPHOOK);
	ASSERT(m_pd.lpfnPrintHook != NULL); // can still be a user hook
	ASSERT(m_pd.lpfnSetupHook != NULL); // can still be a user hook

	m_pd.hwndOwner = PreModal();
	BOOL bResult = ::PrintDlg(&m_pd);
	PostModal();

	return bResult ? IDOK : IDCANCEL;
}

// Create an HDC without calling DoModal.
HDC CPrintDialog::CreatePrinterDC()
{
	ASSERT_VALID(this);
	ASSERT(m_pd.hDevNames != NULL);

	LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(m_pd.hDevNames);
	LPDEVMODE  lpDevMode = (m_pd.hDevMode != NULL) ?
						(LPDEVMODE)::GlobalLock(m_pd.hDevMode) : NULL;

	if (lpDevNames == NULL)
		return NULL;

	m_pd.hDC = ::CreateDC((LPCSTR)lpDevNames + lpDevNames->wDriverOffset,
					  (LPCSTR)lpDevNames + lpDevNames->wDeviceOffset,
					  (LPCSTR)lpDevNames + lpDevNames->wOutputOffset,
					  lpDevMode);

	// Unnecessary global unlocks (not needed in protect mode)
	// ::GlobalUnlock(m_pd.hDevNames);
	// ::GlobalUnlock(m_pd.hDevMode);
	return m_pd.hDC;
}


// Return an HDC.  We don't return a CDC* so the user can decide
// where to attach this HDC: either to a newly allocated object
// (use operator delete to clean up) or to an embedded/frame
// object (destructor will clean up when leaving scope)
HDC CPrintDialog::GetPrinterDC() const
{
	ASSERT_VALID(this);
	ASSERT(m_pd.Flags & PD_RETURNDC);

	return m_pd.hDC;
}

int CPrintDialog::GetCopies() const
{
	ASSERT_VALID(this);

	if (m_pd.Flags & PD_USEDEVMODECOPIES)
		return GetDevMode()->dmCopies;
	else
		return m_pd.nCopies;
}

LPDEVMODE CPrintDialog::GetDevMode() const
{
	if (m_pd.hDevMode == NULL)
		return NULL;

	return (LPDEVMODE)::GlobalLock(m_pd.hDevMode);
}

CString CPrintDialog::GetDriverName() const
{
	if (m_pd.hDevNames == NULL)
		return afxEmptyString;

	LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);
	return (LPSTR)(lpDev) + (UINT)(lpDev->wDriverOffset);
}

CString CPrintDialog::GetDeviceName() const
{
	if (m_pd.hDevNames == NULL)
		return afxEmptyString;

	LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);
	return (LPSTR)(lpDev) + (UINT)(lpDev->wDeviceOffset);
}

CString CPrintDialog::GetPortName() const
{
	if (m_pd.hDevNames == NULL)
		return afxEmptyString;

	LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);
	return (LPSTR)(lpDev) + (UINT)(lpDev->wOutputOffset);
}

void CPrintDialog::OnOK()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

void CPrintDialog::OnCancel()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

////////////////////////////////////////////////////////////////////////////
// CPrintDialog diagnostics

#ifdef _DEBUG
void CPrintDialog::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);
	CDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_pd.hwndOwner = ", (UINT)m_pd.hwndOwner);
	if (m_pd.hDC != NULL)
		AFX_DUMP1(dc, "\nm_pd.hDC = ", CDC::FromHandle(m_pd.hDC));
	AFX_DUMP1(dc, "\nm_pd.Flags = ", (LPVOID)m_pd.Flags);
	AFX_DUMP1(dc, "\nm_pd.nFromPage = ", m_pd.nFromPage);
	AFX_DUMP1(dc, "\nm_pd.nToPage = ", m_pd.nToPage);
	AFX_DUMP1(dc, "\nm_pd.nMinPage = ", m_pd.nMinPage);
	AFX_DUMP1(dc, "\nm_pd.nMaxPage = ", m_pd.nMaxPage);
	AFX_DUMP1(dc, "\nm_pd.nCopies = ", m_pd.nCopies);
	if (m_pd.lpfnSetupHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nsetup hook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nsetup hook function set to non-standard hook function");
	if (m_pd.lpfnPrintHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nprint hook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nprint hook function set to non-standard hook function");
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////
