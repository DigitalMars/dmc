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

#ifdef AFX_PRINT_SEG
#pragma code_seg(AFX_PRINT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Printing Dialog

#ifndef _AFXDLL
static BOOL NEAR _afxUserAbort = FALSE;
#else
#define _afxUserAbort _AfxGetAppData()->appUserAbort
#endif

class CPrintingDialog : public CDialog
{
public:
	//{{AFX_DATA(CPrintingDialog)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA
	CPrintingDialog::CPrintingDialog(CWnd* pParent)
		{
			Create(CPrintingDialog::IDD, pParent);      // modeless !
			_afxUserAbort = FALSE;
		}

	virtual BOOL OnInitDialog();
	virtual void OnCancel();

protected:

#if 0 // for ClassWizard's use only
	//{{AFX_MSG(CPrintingDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif
};

#if 0 // for ClassWizard's use only
BEGIN_MESSAGE_MAP(CPrintingDialog, CDialog)
	//{{AFX_MSG_MAP(CPrintingDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif

BOOL CALLBACK AFX_EXPORT _AfxAbortProc(HDC, int)
{
	MSG msg;
	while (!_afxUserAbort &&
			::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
			return FALSE;               // Bag out if WM_QUIT received
	}
	return !_afxUserAbort;
}


BOOL CPrintingDialog::OnInitDialog()
{
	SetWindowText(AfxGetAppName());
	CenterWindow();
	return CDialog::OnInitDialog();
}

void CPrintingDialog::OnCancel()
{
	_afxUserAbort = TRUE;       // flag that user aborted print
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CView printing commands

BOOL CView::DoPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_bPreview)
	{
		// if preview, get default printer DC and create DC without calling
		//   print dialog.
		if (!AfxGetApp()->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
		{
			// bring up dialog to alert the user they need to install a printer.
			if (AfxGetApp()->DoPrintDialog(pInfo->m_pPD) != IDOK)
				return FALSE;
		}

		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// call CreatePrinterDC if DC was not created by above
			if (pInfo->m_pPD->CreatePrinterDC() == NULL)
				return FALSE;
		}

		// set up From and To page range from Min and Max
		pInfo->m_pPD->m_pd.nFromPage = pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = pInfo->GetMaxPage();

	}
	else
	{
		// otherwise, bring up the print dialog and allow user to change things

		// preset From-To range same as Min-Max range
		pInfo->m_pPD->m_pd.nFromPage = pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = pInfo->GetMaxPage();

		if (AfxGetApp()->DoPrintDialog(pInfo->m_pPD) != IDOK)
			return FALSE;       // do not print
	}

	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);

	pInfo->m_nNumPreviewPages = AfxGetApp()->m_nNumPreviewPages;
	VERIFY(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	return TRUE;
}

void CView::OnFilePrint()
{
	CPrintInfo printInfo;           // Get Default print info
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (OnPreparePrinting(&printInfo))
	{       // print if OK

		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);
				// must be set (did you remember to call DoPreparePrinting?)

		CDC dcPrint;
		dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);          // attach printer dc
		dcPrint.m_bPrinting = TRUE;

		OnBeginPrinting(&dcPrint, &printInfo);

		CPrintingDialog dlgPrintStatus(this);

		CString docTitle = GetDocument()->GetTitle();

		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, docTitle);
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
			printInfo.m_pPD->GetDeviceName());

		CString strPort;
		AfxFormatString1(strPort, AFX_IDS_PRINTONPORT,
			printInfo.m_pPD->GetPortName());
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strPort);
		dlgPrintStatus.ShowWindow(SW_SHOW);

		dcPrint.SetAbortProc(_AfxAbortProc);

		if (docTitle.GetLength() > 31)
			docTitle.ReleaseBuffer(31);

		DOCINFO docInfo;
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = docTitle;
		docInfo.lpszOutput = NULL;

		if (dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		AfxGetMainWnd()->EnableWindow(FALSE);   // Disable main window

		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		// Guarantee values are in the valid range
		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		BOOL bError = FALSE;
		for (printInfo.m_nCurPage = nStartPage;
			!bError && printInfo.m_nCurPage != nEndPage;
			printInfo.m_nCurPage += nStep)
		{
			OnPrepareDC(&dcPrint, &printInfo);

			if (!printInfo.m_bContinuePrinting)
				break;          // reached end of print

			// Set up drawing rect to entire page (in logical coordinates)
			printInfo.m_rectDraw.SetRect(0, 0, dcPrint.GetDeviceCaps(HORZRES),
											   dcPrint.GetDeviceCaps(VERTRES));
			dcPrint.DPtoLP(&printInfo.m_rectDraw);

			CString strFmt;
			VERIFY(strFmt.LoadString(AFX_IDS_PRINTPAGENUM));
			char szBuf[80];
			wsprintf(szBuf, strFmt, printInfo.m_nCurPage);
			dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

			VERIFY(dcPrint.StartPage());
			OnPrint(&dcPrint, &printInfo);
			if (dcPrint.EndPage() < 0)
				bError = TRUE;
		}

		if (!bError)
			dcPrint.EndDoc();

		AfxGetMainWnd()->EnableWindow(TRUE);    // Enable main window

		OnEndPrinting(&dcPrint, &printInfo);        // Clean up after printing

		dlgPrintStatus.DestroyWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintInfo helper structure

CPrintInfo::CPrintInfo()
{
	m_pPD = new CPrintDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES |
										PD_HIDEPRINTTOFILE | PD_NOSELECTION);

	SetMinPage(1);              // one based page numbers
	SetMaxPage(0xffff);         // unknown how many pages

	m_nCurPage = 1;

	m_lpUserData = NULL;        // Initialize to no user data
	m_bPreview = FALSE;         // initialize to not preview
	m_bContinuePrinting = TRUE; // Assume it is OK to print
}

CPrintInfo::~CPrintInfo()
{
	delete m_pPD;
}

/////////////////////////////////////////////////////////////////////////////
