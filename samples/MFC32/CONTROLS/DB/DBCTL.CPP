// dbctl.cpp : Implementation of the CDbCtrl OLE control class.
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
#include "db.h"
#include "dbctl.h"
#include "dbppg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDbCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDbCtrl, COleControl)
	//{{AFX_MSG_MAP(CDbCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	ON_OLEVERB(IDS_PROPERTIESVERB, OnProperties)
	ON_MESSAGE(OCM_COMMAND, OnOcmCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CDbCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CDbCtrl)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "Query", m_query, OnQueryChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "DataSource", m_dataSource, OnDataSourceChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "TableName", m_tableName, OnTableNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "ColumnName", m_columnName, OnColumnNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "UserName", m_userName, OnUserNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CDbCtrl, "Password", m_password, OnPasswordChanged, VT_BSTR)
	DISP_FUNCTION(CDbCtrl, "ReQuery", ReQuery, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CDbCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CDbCtrl, COleControl)
	//{{AFX_EVENT_MAP(CDbCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_DYNAMIC(CGenericSet, CRecordset)

CString CGenericSet::GetDefaultSQL()
{
	return GetSQL();
}

CGenericSet::CGenericSet(CDatabase* pDB, CString* pstrColumnName)
	: CRecordset(pDB)
{
	ASSERT(pDB != NULL);
	m_strColumnName = *pstrColumnName;
	m_nFields = 1;
}

void CGenericSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
 	RFX_Text(pFX, m_strColumnName, m_strCurrentValue);
}


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CDbCtrl, 1)
	PROPPAGEID(CDbPropPage::guid)
END_PROPPAGEIDS(CDbCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDbCtrl, "DB.DbCtrl.1",
	0xeff01745, 0x7825, 0x101b, 0x83, 0x75, 0x0, 0xaa, 0x0, 0x37, 0x3f, 0xb9)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CDbCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DDb =
		{ 0xeff01746, 0x7825, 0x101b, { 0x83, 0x75, 0x0, 0xaa, 0x0, 0x37, 0x3f, 0xb9 } };
const IID BASED_CODE IID_DDbEvents =
		{ 0xeff01747, 0x7825, 0x101b, { 0x83, 0x75, 0x0, 0xaa, 0x0, 0x37, 0x3f, 0xb9 } };





/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwDbOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDbCtrl, IDS_DB, _dwDbOleMisc);


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::CDbCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CDbCtrl

BOOL CDbCtrl::CDbCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DB,
			IDB_DB,
			TRUE,						//  Insertable
			_dwDbOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::CDbCtrl - Constructor

CDbCtrl::CDbCtrl()
{
	InitializeIIDs(&IID_DDb, &IID_DDbEvents);

	m_bConnected = FALSE;
	m_pRecordSet = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::~CDbCtrl - Destructor

CDbCtrl::~CDbCtrl()
{
	if (m_pRecordSet != NULL)
	{
		m_pRecordSet->Close();
		delete m_pRecordSet;
		m_pRecordSet = NULL;
	}

	if (m_bConnected == TRUE)
	{
		m_cDatabase.Close();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::OnDraw - Drawing function

void CDbCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	DoSuperclassPaint(pdc, rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::DoPropExchange - Persistence support

void CDbCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::OnResetState - Reset control to default state

void CDbCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::AboutBox - Display an "About" box to the user

void CDbCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_DB);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL CDbCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("LISTBOX");
	cs.style |= LBS_NOINTEGRALHEIGHT;
	return COleControl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::GetSuperWndProcAddr - Provide storage for window proc

WNDPROC* CDbCtrl::GetSuperWndProcAddr(void)
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl::OnOcmCommand - Handle command messages

LRESULT CDbCtrl::OnOcmCommand(WPARAM  wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	lParam;

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CDbCtrl message handlers

void CDbCtrl::OnDataSourceChanged() 
{
	SetModifiedFlag();
}

void CDbCtrl::OnTableNameChanged() 
{
	SetModifiedFlag();
}

void CDbCtrl::OnColumnNameChanged() 
{
	SetModifiedFlag();
    	
}

void CDbCtrl::OnUserNameChanged() 
{
	SetModifiedFlag();
}

void CDbCtrl::OnPasswordChanged() 
{
	SetModifiedFlag();
}

void CDbCtrl::OnQueryChanged() 
{
	SetModifiedFlag();
	return;
}


void CDbCtrl::ReQuery() 
{
	if (m_pRecordSet != NULL)
	{
		m_pRecordSet->Close();
		delete m_pRecordSet;
		m_pRecordSet = NULL;
	}
		
	if (m_bConnected == TRUE)
	{
		m_cDatabase.Close();
		m_bConnected = FALSE;
	}

	TRY                                                                             
	{
		m_bConnected = m_cDatabase.Open(m_dataSource,
			FALSE, TRUE, "ODBC;", FALSE);
	}           
	CATCH_ALL(e)
	{
		MessageBox("Threw an exception!", "CDatabase::Open()");
		return;
	}
	END_CATCH_ALL

	m_pRecordSet = new CGenericSet(&m_cDatabase, &m_columnName);

	TRY
	{
		m_query = "SELECT " + m_columnName + " FROM " + m_tableName;

		m_pRecordSet->Open(CRecordset::snapshot,
				m_query, CRecordset::readOnly);

		if (m_pRecordSet->IsBOF())
		{
			MessageBox("The requested table has no records.");
		}
		else
		{
			BOOL bMemoryFlag = FALSE;

			while (!m_pRecordSet->IsEOF())
			{
			if (SendMessage(LB_ADDSTRING, 0,
				(LPARAM) (LPCSTR) m_pRecordSet->m_strCurrentValue) == LB_ERR)
				{
				bMemoryFlag = TRUE;
				break;
				}
			m_pRecordSet->MoveNext();
			}

			if (bMemoryFlag == TRUE)
			{
				MessageBox("Warning:  Not all returned records are shown in the listbox.",
						"Out Of Memory");
			}
		}
	}           
	CATCH(CDBException, e)
	{
		if (strlen(e->m_strError) == 0 &&
			strlen(e->m_strStateNativeOrigin) == 0)
		{
			char szErrorMsg[_MAX_PATH];
			wsprintf(szErrorMsg, "ODBC Error %d\nError text was not availalbe from driver",
				e->m_nRetCode);
			MessageBox(szErrorMsg, "CRecordset::Open()", MB_ICONEXCLAMATION);
		}
		else
		{
			MessageBox(e->m_strError + "\n" +
				e->m_strStateNativeOrigin, "CRecordset::Open()", MB_ICONEXCLAMATION);
		}				    

		delete m_pRecordSet;
		m_pRecordSet = NULL;
	}
	END_CATCH
}
