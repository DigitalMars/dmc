// mainfrm.cpp : implementation of the CMainFrame class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "vcterm.h"
#include "mainfrm.h"
#include "setdlg.h"
#include "inputdlg.h"

#include "io.h"
#include "candlg.h"
#include "winnls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_OPENLOG, ID_FILE_TRANSMIT, OnUpdateFile)
	ON_COMMAND(ID_FILE_OPENLOG, OnFileOpenlog)
	ON_COMMAND(ID_FILE_CLOSELOG, OnFileCloselog)
	ON_COMMAND(ID_FILE_TRANSMIT, OnFileTransmit)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COMM_OPEN, ID_COMM_HANGUP, OnUpdateCommPort)
	ON_COMMAND_RANGE(ID_COMM_OPEN, ID_COMM_HANGUP, OnCommPort)
	ON_UPDATE_COMMAND_UI(ID_PROP_DTRENABLE, OnUpdatePropDTREnable)
	ON_COMMAND_RANGE(ID_PROP_INPUTLEN, ID_PROP_DSRHOLDING, OnProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// add eventsink map to hook OnComm event
BEGIN_EVENTSINK_MAP(CMainFrame, CFrameWnd)
	ON_EVENT(CMainFrame, ID_COMMCTRL, 1 /* OnComm */, OnCommEvent, VTS_NONE)
END_EVENTSINK_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_strPhoneNum.Empty();
	m_strLogFile.Empty();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Create the MSCOMM32 OLE Control.
	if (!m_commctrl.Create(NULL,0,CRect(0,0,0,0),this,ID_COMMCTRL))
	{
		TRACE0("Failed to create OLE Communications Control\n");
		return -1;      // fail to create 
	}

	// Set RThreshold property to 1. The default RThreshold value (0)
	// causes the OnComm event to NOT fire when a character is detected
	// in the serial port. 
	m_commctrl.SetRThreshold(1);

	// Create the Edit control that sits in the mainframe window.
	if (!m_edit.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_WANTRETURN|
		ES_MULTILINE|ES_AUTOVSCROLL,rectDefault,this,ID_TERMEDIT))
	{
		TRACE0("Failed to create edit control\n");
		return -1;      // fail to create
	}

	return 0;
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	// Pass ID_TERMEDIT as last param to size the edit control to the
	// leftover client area in mainframe window after toolbar and 
	// statusbar are repositioned.
	RepositionBars(0,0xFFFF,ID_TERMEDIT);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// Set focus to edit control in main window.
	m_edit.SetFocus();
}

void CMainFrame::OnUpdateFile(CCmdUI* pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
		// Enable if there is no logfile currently open.
		case ID_FILE_OPENLOG:   
			pCmdUI->Enable(m_strLogFile.IsEmpty());
			break;
		// Enable if a log file is currently open. 
		case ID_FILE_CLOSELOG:
			pCmdUI->Enable(!m_strLogFile.IsEmpty());
			break;
		// Enable if comm port is open.
		case ID_FILE_TRANSMIT:
			pCmdUI->Enable(m_commctrl.GetPortOpen());
			break;
	}
}

void CMainFrame::OnFileOpenlog() 
{
	ASSERT(m_strLogFile.IsEmpty());
		
	CFileDialog dlg(TRUE,_T("log"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	_T("Log Files (*.log) | *.log | All Files (*.*) | *.* ||"),this);
	
	if (dlg.DoModal()==IDOK)
	{
		m_strLogFile = dlg.GetPathName();

		if (!m_file.Open(m_strLogFile,CFile::modeCreate|CFile::modeWrite,NULL))
		{
			AfxMessageBox(IDS_FILEOPENFAILED);
			m_strLogFile.Empty();
			return;
		}
		
		// Update mainframe window caption.
		CString str;
		str.Format(_T("VCTerm - %s"),m_strLogFile);
		SetWindowText(str);
	}
}

void CMainFrame::OnFileCloselog() 
{
	m_file.Flush();
	m_file.Close();
	m_strLogFile.Empty();
	SetWindowText(_T("VCTerm"));
}

void CMainFrame::OnFileTransmit() 
{
	CFileDialog dlg(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
	_T("Text Files (*.txt) | *.txt | All Files (*.*) | *.* ||"),this);

	if (dlg.DoModal()!=IDOK)
		return;

	CFile file(dlg.GetPathName(),CFile::modeRead);
	
	// Display cancel dialog.
	EnableWindow(FALSE);
	m_bCancel = FALSE;

	CCancelDlg* pDlg = new CCancelDlg(this);
	pDlg->Create(IDD_CANCELDLG,this);

	int nBlockSize = m_commctrl.GetOutBufferSize();
	int nFileSize = file.GetLength();
	LPSTR pchBuffer = new char[nBlockSize];
	LPWSTR pchData = new WCHAR[nBlockSize];

	VARIANT var;

	// Read/Transmit the file in blocks the size of the transmit buffer.
	for (int nRxTx = 0; nRxTx<(nFileSize/nBlockSize) && !m_bCancel; nRxTx++)
	{
		file.Read(pchBuffer,nBlockSize);

		// Convert the ASCII text string to a UNICODE string.
		MultiByteToWideChar(CP_ACP,0,pchBuffer,nBlockSize,pchData,nBlockSize);
		var.vt = VT_BSTR;
		var.bstrVal = ::SysAllocStringLen(pchData,nBlockSize);

		// Send data to comm port.
		m_commctrl.SetOutput(var);
  		SysFreeString(var.bstrVal);

		// Wait for the transmit buffer to empty.
		while ((m_commctrl.GetOutBufferCount() > 0) && !m_bCancel)
			DoEvents();
	}

	// Dump last bit of file to the transmit buffer.
	int nLeftOver = nFileSize % nBlockSize;

	// If file size is identical to Output Buffer size.
	if (!nLeftOver)
		nLeftOver = nBlockSize;

	// Read/Transmit the last chunk of the file.
	if (nLeftOver && !m_bCancel)
	{
		file.Read(pchBuffer,nLeftOver);
		
		// Convert the ASCII text string to a UNICODE string.
		MultiByteToWideChar(CP_ACP,0,pchBuffer,nLeftOver,pchData,nLeftOver);
		var.vt = VT_BSTR;
		var.bstrVal = ::SysAllocStringLen(pchData,nLeftOver);

		// Send data to comm port.
		m_commctrl.SetOutput(var);
		SysFreeString(var.bstrVal);

		// Wait for the transmit buffer to empty.
		while ((m_commctrl.GetOutBufferCount() > 0) && !m_bCancel)
			DoEvents();
	}

	// Close out the Cancel dialog.
	if (!m_bCancel)
		pDlg->OnCancel();

	delete pchBuffer;
	delete pchData;
	file.Close();
	EnableWindow(TRUE);
	m_edit.SetFocus();
}

void CMainFrame::OnClose() 
{
	if (m_commctrl.GetPortOpen())
	{
		// Set timeout period for 10 seconds from current time.
		BOOL bTimedOut = FALSE;
		CTime timeOut = CTime::GetCurrentTime() + CTimeSpan(0,0,0,10); 

		while (m_commctrl.GetOutBufferCount())
		{
			// Process all pending messages.
			m_bCancel=FALSE;
			DoEvents();

			if ( (CTime::GetCurrentTime() > timeOut) || bTimedOut)
			{
				int ret = AfxMessageBox(IDS_DATASENDFAILED,MB_ABORTRETRYIGNORE);

				switch (ret)
				{
					// Attempt to send data for another 10 seconds.
					case IDRETRY:
						timeOut = CTime::GetCurrentTime() + CTimeSpan(0,0,0,10);
						break;
					// Ignore timeout and exit. 
					case IDIGNORE:
						bTimedOut = TRUE;
						break;
					// Abort attempt to exit application.
					case IDABORT:
						return;
				}
			}
		}
		m_commctrl.SetPortOpen(FALSE);
	}

	// Close log file if it's currently open.
	if (!m_strLogFile.IsEmpty())
		OnFileCloselog();

	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateCommPort(CCmdUI* pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
	case ID_COMM_OPEN:
		if (m_commctrl.GetPortOpen())
			pCmdUI->SetText(_T("&Port Close"));
		else 
			pCmdUI->SetText(_T("&Port Open"));
		pCmdUI->Enable();
		break;

	case ID_COMM_SETTINGS:
		if (m_commctrl.GetPortOpen())
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable();
		break;

	case ID_COMM_DIAL:
	case ID_COMM_HANGUP:
		pCmdUI->Enable(m_commctrl.GetPortOpen());
		break;
	}
}

void CMainFrame::OnCommPort(UINT nID)
{
	switch(nID)
	{
	case ID_COMM_OPEN:
		if (m_commctrl.GetPortOpen())
			m_commctrl.SetPortOpen(FALSE);
		else
			m_commctrl.SetPortOpen(TRUE);
		break;

	case ID_COMM_SETTINGS:
		{
			// Invoke settings dialog. NOTE: see CSettingsDlg::OnOk()
			// for processing of the selected dialog settings.
			CSettingsDlg dlg(this);
			dlg.DoModal();
		}
		break;

	case ID_COMM_DIAL:
		{
			CInputDlg dlg(this);
			dlg.m_strPrompt.LoadString(IDS_PHONEPROMPT);
			dlg.m_strEdit = m_strPhoneNum;
			if (dlg.DoModal()==IDOK)
			{
				// NOTE: Modem command string will differ according to
				// modem type. Check documentation included with modem to
				// determine the correct command string to autodial.

				CString str;
				m_strPhoneNum = dlg.m_strEdit;
				str.Format(IDS_DIALCOMMAND,m_strPhoneNum);
		
				VARIANT var;
				var.vt = VT_BSTR;
				var.bstrVal = str.AllocSysString();
				m_commctrl.SetOutput(var);
				SysFreeString(var.bstrVal);
			}
		}
		break;

	case ID_COMM_HANGUP:

		// Toggle DTR line to signal modem to hang up. NOTE: Not all
		// modems are automatically configured to hang up when the DTR
		// line goes cold. A special modem command may be required to
		// force modem to monitor the DTR line.	Check documentation 
		// included with modem to determine the correct command string
		// to send to the modem.

		BOOL bEnabled = m_commctrl.GetDTREnable();
		m_commctrl.SetDTREnable(TRUE);
		m_commctrl.SetDTREnable(FALSE);
		m_commctrl.SetDTREnable(bEnabled);
		break;
	}
}

// Toggle check box on DTREnable menu item.
void CMainFrame::OnUpdatePropDTREnable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_commctrl.GetDTREnable());
	pCmdUI->Enable();
}

void CMainFrame::OnProperties(UINT nID)
{
	CInputDlg dlg(this);
	TCHAR buff[40];

	switch(nID)
	{
	case ID_PROP_INPUTLEN:
		dlg.m_strPrompt.LoadString(IDS_INPUTLENPROMPT);
		_itot(m_commctrl.GetInputLen(),buff,10);
		dlg.m_strEdit = buff;
		if (dlg.DoModal()==IDOK)
			m_commctrl.SetInputLen(_ttoi(dlg.m_strEdit));
		break;

	case ID_PROP_RTHRESHOLD:
		dlg.m_strPrompt.LoadString(IDS_RTHRESHOLDPROMPT);
		_itot(m_commctrl.GetRThreshold(),buff,10);
		dlg.m_strEdit = buff;
		if (dlg.DoModal()==IDOK)
			m_commctrl.SetRThreshold(_ttoi(dlg.m_strEdit));
		break;

	case ID_PROP_STHRESHOLD:
		dlg.m_strPrompt.LoadString(IDS_STHRESHOLDPROMPT);
		_itot(m_commctrl.GetSThreshold(),buff,10);
		dlg.m_strEdit = buff;
		if (dlg.DoModal()==IDOK)
			m_commctrl.SetSThreshold(_ttoi(dlg.m_strEdit));
		break;

	case ID_PROP_PARITYREPLACE:
		dlg.m_strPrompt.LoadString(IDS_PARITYREPLACEPROMPT);
		dlg.m_strEdit = m_commctrl.GetParityReplace();
		if (dlg.DoModal()==IDOK)
			m_commctrl.SetParityReplace(dlg.m_strEdit);
		break;

	case ID_PROP_DTRENABLE:
		m_commctrl.SetDTREnable(!m_commctrl.GetDTREnable());
		break;

	case ID_PROP_CDHOLDING:
		if (m_commctrl.GetCDHolding())
			AfxMessageBox(IDS_CDHOLDINGTRUE);
		else
			AfxMessageBox(IDS_CDHOLDINGFALSE);
		break;
		
	case ID_PROP_CTSHOLDING:
		if (m_commctrl.GetCTSHolding())
			AfxMessageBox(IDS_CTSHOLDINGTRUE);
		else
			AfxMessageBox(IDS_CTSHOLDINGFALSE);
		break;
		
	case ID_PROP_DSRHOLDING:
		if (m_commctrl.GetDSRHolding())
			AfxMessageBox(IDS_DSRHOLDINGTRUE);
		else
			AfxMessageBox(IDS_DSRHOLDINGFALSE);
		break;
	}
}

void CMainFrame::ShowData(LPCTSTR pchData)
{
	CString str;
	int nCharIndex;
	int nBackcount = 0;

	// Make sure existing text doesn't get too large.
	int len = m_edit.GetWindowTextLength();
	if (len >= 16384)
	{
		m_edit.GetWindowText(str);
		m_edit.SetWindowText(str.GetBuffer(len)+4097);
		len -= 4097;
	}

	// Handle backspace characters.
	str = pchData;
	while ((nCharIndex = str.Find(TCHAR(VK_BACK))) != -1)
	{
		if (nCharIndex == 0)
		{
			nBackcount++;
			str = str.Mid(nCharIndex+1);
		}
		else
			str = str.Left(nCharIndex-1) + str.Mid(nCharIndex+1);
	}

	// Set text insertion point according to number of backspaces
	// at the begining of strData.
	if (nBackcount>len)
		nBackcount = len;
	m_edit.SetSel(len-nBackcount,len);

	// Eliminate line feeds.
	while ((nCharIndex = str.Find(TCHAR('\n'))) != -1)
		str = str.Left(nCharIndex) + str.Mid(nCharIndex + 1);

	// Ensure all carriage returns have a line feed following.
	LPCTSTR pchStart = (LPCTSTR)str;
	LPTSTR pchIndex = _tcschr(pchStart,(TCHAR)'\r');

	while (pchIndex != NULL)
	{
		int nPos = pchIndex - pchStart; 

		str = str.Left(nPos+1) + CString('\n') + str.Mid(nPos+1);
		
		// Above operation probably allocated a new string buffer.
		pchStart = (LPCTSTR)str;			
		pchIndex = _tcschr(pchStart + nPos + 1,(TCHAR)'\r');

	}

	// Add filtered data to edit control.
	m_edit.ReplaceSel(str);

	// Log data to file if requested.
	if (!m_strLogFile.IsEmpty())
		m_file.Write(str,str.GetLength());
}

void CMainFrame::OnCommEvent()
{
	CString strEvent, strError;

	switch(m_commctrl.GetCommEvent())
	{
	    case 1: // vbMSCommEvSend:
			strEvent.LoadString(IDS_SENDING);
			break;
		case 2: // vbMSCommEvReceive:
			strEvent.LoadString(IDS_RECEIVING);
			ShowData(m_commctrl.GetInput());
			break;
		case 3: // vbMSCommEvCTS:
			strEvent.LoadString(IDS_CTSCHANGE);
			break;
		case 4: // vbMSCommEvDSR:
			strEvent.LoadString(IDS_DSRCHANGE);
			break;
		case 5: // vbMSCommEvCD:
			strEvent.LoadString(IDS_CDCHANGE);
			break;
		case 6: // vbMSCommEvRing:
			strEvent.LoadString(IDS_PHONERING);
			break;
		case 7: // vbMSCommEvEOF:
			strEvent.LoadString(IDS_EOFDETECTED);
			break;

		case 1001: //  vbMSCommErBreak:
			strError.LoadString(IDS_BREAKRECEIVED);
			break;
		case 1002: // vbMSCommErCTSTO:
			strError.LoadString(IDS_CTSTIMEOUT);
			break;
		case 1003: // vbMSCommErDSRTO:
			strError.LoadString(IDS_DSRTIMEOUT);
			break;
		case 1004: // vbMSCommErFrame:
			strError.LoadString(IDS_FRAMEERROR);
			break;
		case 1006: // vbMSCommErOverrun:
			strError.LoadString(IDS_OVERRUNERROR);
			break;
		case 1007: // vbMSCommErCDTO:
			strError.LoadString(IDS_CDTIMEOUT);
			break;
		case 1008: // vbMSCommErRxOver:
			strError.LoadString(IDS_RBUFFEROVERFLOW);
			break;
		case 1009: // vbMSCommErRxParity:
			strError.LoadString(IDS_PARITYERROR);
			break;
		case 1010: // vbMSCommErTxFull:
			strError.LoadString(IDS_TBUFFERFULL);
			break;
	}

	if (!strEvent.IsEmpty())
		m_wndStatusBar.SetPaneText(0,strEvent);
	else 
		if (!strError.IsEmpty())
		{
			MessageBeep(MB_ICONEXCLAMATION);
			strError += _T("\nOK to ignore, Cancel to quit"); 
			int ret = AfxMessageBox(strError,MB_OKCANCEL|MB_ICONEXCLAMATION);
			if (ret==IDCANCEL)
				m_commctrl.SetPortOpen(FALSE);
		}	
}

void CMainFrame::DoEvents()
{
	MSG msg;

	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) && !m_bCancel)
	{
		if (!AfxGetThread()->PumpMessage())
			return;
	}
}
	

