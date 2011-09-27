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

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Command Targets

IMPLEMENT_DYNAMIC(CCmdTarget, CObject)

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget construction/destruction

CCmdTarget::CCmdTarget()
{
	// initialize state
	m_dwRef = 1;
	m_pOuterUnknown = NULL;
	m_xInnerUnknown = 0;
	m_xDispatch = 0;
}

CCmdTarget::~CCmdTarget()
{
	if (m_xDispatch != 0)
		((COleDispatchImpl FAR*)&m_xDispatch)->Disconnect();

	ASSERT(m_dwRef <= 1);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget windows message dispatching

union MessageMapFunctions
{
	AFX_PMSG pfn;   // generic member function pointer

	// specific type safe variants
	void    (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND)();
	BOOL    (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND_EX)(UINT);
	void    (AFX_MSG_CALL CCmdTarget::*pfn_VBXEVENT)(UINT, int, CWnd*, LPVOID);

	void    (AFX_MSG_CALL CCmdTarget::*pfn_UPDATE_COMMAND_UI)(CCmdUI*);
	void    (AFX_MSG_CALL CCmdTarget::*pfn_OTHER)(void*);
	BOOL    (AFX_MSG_CALL CCmdTarget::*pfn_OTHER_EX)(void*);
};


static BOOL DispatchCmdMsg(CCmdTarget* pTarget, UINT nID, int nCode,
	AFX_PMSG pfn, void* pExtra, UINT nSig, AFX_CMDHANDLERINFO* pHandlerInfo)
		// return TRUE to stop routing
{
	ASSERT_VALID(pTarget);

	union MessageMapFunctions mmf;
	mmf.pfn = pfn;
	BOOL bOK = TRUE; // default is ok

	if (pHandlerInfo != NULL)
	{
		// just fill in the information, don't do it
		pHandlerInfo->pTarget = pTarget;
		pHandlerInfo->pmf = mmf.pfn;
		return TRUE;
	}

	switch (nSig)
	{
	case AfxSig_vv:
		// normal command or control notification
		ASSERT(CN_COMMAND == 0);        // CN_COMMAND same as BN_CLICKED
		ASSERT(pExtra == NULL);
		(pTarget->*mmf.pfn_COMMAND)();
		break;

	case AfxSig_bw:
		// extended command (passed ID, returns bContinue)
		ASSERT(pExtra == NULL);
		bOK = (pTarget->*mmf.pfn_COMMAND_EX)(nID);
		break;

	case AfxSig_cmdui:
		{
			// ON_UPDATE_COMMAND_UI case
			ASSERT(nCode == CN_UPDATE_COMMAND_UI);
			ASSERT(pExtra != NULL);
			CCmdUI* pCmdUI = (CCmdUI*)pExtra;
			ASSERT(pCmdUI->m_nID == nID);           // sanity assert
			ASSERT(!pCmdUI->m_bContinueRouting);    // idle - not set
			(pTarget->*mmf.pfn_UPDATE_COMMAND_UI)(pCmdUI);
			bOK = !pCmdUI->m_bContinueRouting;
			pCmdUI->m_bContinueRouting = FALSE;     // go back to idle
		}
		break;

	case AfxSig_vbx:
		{
			// ON_VBX_EVENT case
			ASSERT(((WORD)nCode) >= 0xC000); // must be a registered VB event
			AFX_VBXEVENTPARAMS FAR* lpEvent = *(AFX_VBXEVENTPARAMS FAR**)pExtra;
			(pTarget->*mmf.pfn_VBXEVENT)(lpEvent->nNotifyCode,
				lpEvent->nEventIndex, lpEvent->pControl,
				lpEvent->lpUserParams);
		}
		break;

	// general extensibility hooks
	case AfxSig_vpv:
		(pTarget->*mmf.pfn_OTHER)(pExtra);
		break;
	case AfxSig_bpv:
		bOK = (pTarget->*mmf.pfn_OTHER_EX)(pExtra);
		break;

	default:    // illegal
		ASSERT(FALSE);
		return 0;
	}
	return bOK;
}

BOOL CCmdTarget::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	ASSERT(nID != 0);   // 0 command IDs are not allowed !

	// Now look through message map to see if it applies to us
	AFX_MSGMAP* pMessageMap;
	AFX_MSGMAP_ENTRY FAR* lpEntry;
	for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
	  pMessageMap = pMessageMap->pBaseMessageMap)
	{
		ASSERT(pMessageMap != pMessageMap->pBaseMessageMap);
			//NOTE: catches BEGIN_MESSAGE_MAP(CMyClass, CMyClass)!

		// Constant code...
		if ((UINT)nCode < 0xC000 || (UINT)nCode > 0xFF00)
		{
			lpEntry = _AfxFindMessageEntry(pMessageMap->lpEntries, nCode, nID);
		}
		else
		{
			// registered notification code:  E.g. VBX_EVENTs
			lpEntry = pMessageMap->lpEntries;

			while ((lpEntry = _AfxFindMessageEntry(lpEntry, 0xC001, nID))
			  != NULL)
			{
#ifndef _WINDLL
				int NEAR* pnCode = (int NEAR*)(lpEntry->nSig);
#else
				// if SS!=DS, then REGISTERED message must be
				//   in same data segment as the message map
				int FAR* pnCode = (int FAR*)MAKELONG(lpEntry->nSig,
						_AFX_FP_SEG(pMessageMap));
#endif
				ASSERT(((UINT)*pnCode) >= 0xC000);
						// must be successfully registered

				if (*pnCode == nCode)
					break;      // found it

				lpEntry++;      // keep looking past this one
			}
		}

		if (lpEntry != NULL)
		{
			// found it
#ifdef _DEBUG
			if (afxTraceFlags & 8)  // if command reporting
			{
				if (nCode == 0)
				{
					TRACE2("SENDING command id 0x%04X to %Fs target\n", nID,
						GetRuntimeClass()->m_lpszClassName);
				}
				else if (nCode > 0)
				{
					if (afxTraceFlags & 4)
					{
						TRACE3("SENDING control notification %d from "
						"control id 0x%04X to %Fs window\n",
							nCode, nID, GetRuntimeClass()->m_lpszClassName);
					}
				}
			}
#endif //_DEBUG
			if ((UINT)nCode < 0xC000 || (UINT)nCode > 0xFF00)
			{
				return DispatchCmdMsg(this, nID, nCode,
					lpEntry->pfn, pExtra, lpEntry->nSig, pHandlerInfo);
			}
			else
			{
				// Force VBX signature
				return DispatchCmdMsg(this, nID, nCode,
					lpEntry->pfn, pExtra, AfxSig_vbx, pHandlerInfo);
			}
		}
	}

	return FALSE;   // not handled
}

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget routines that delegate to the WinApp

void CCmdTarget::BeginWaitCursor()
	{ AfxGetApp()->DoWaitCursor(1); }
void CCmdTarget::EndWaitCursor()
	{ AfxGetApp()->DoWaitCursor(-1); }
void CCmdTarget::RestoreWaitCursor()
	{ AfxGetApp()->DoWaitCursor(0); }

/////////////////////////////////////////////////////////////////////////////
// Root of message maps

AFX_MSGMAP AFXAPI_DATA CCmdTarget::messageMap =
{
	NULL,   // end of chain of message maps
	(AFX_MSGMAP_ENTRY FAR*) &CCmdTarget::_messageEntries
};

AFX_MSGMAP* CCmdTarget::GetMessageMap() const
{
	return &CCmdTarget::messageMap;
}

AFX_MSGMAP_ENTRY BASED_CODE CCmdTarget::_messageEntries[] =
{
	{ 0, 0, AfxSig_end, 0 }     // nothing here
};

/////////////////////////////////////////////////////////////////////////////
// Root of dispatch maps

AFX_DISPMAP BASED_CODE CCmdTarget::dispatchMap =
{
	 NULL,  // end of chain of dispatch maps
	 (AFX_DISPMAP_ENTRY FAR*)&CCmdTarget::_dispatchEntries
};

AFX_DISPMAP FAR* CCmdTarget::GetDispatchMap() const
{
	return &CCmdTarget::dispatchMap;
}

AFX_DISPMAP_ENTRY BASED_CODE CCmdTarget::_dispatchEntries[] =
{
	{ "", -1, "", 0, (AFX_PMSG)NULL, (AFX_PMSG)NULL, (size_t)-1 }
	// nothing here
};

/////////////////////////////////////////////////////////////////////////////
// Root of interface maps

AFX_INTERFACEMAP FAR* CCmdTarget::GetInterfaceMap() const
{
	return &CCmdTarget::interfaceMap;
}

AFX_INTERFACEMAP BASED_CODE CCmdTarget::interfaceMap =
{
	NULL,
	(AFX_INTERFACEMAP_ENTRY FAR*)CCmdTarget::_interfaceEntries,
};


// this is the GUID for IID IDispatch
//  (we define this ourselves to avoid references to OLE2.DLL in non-OLE apps)
static const IID BASED_CODE IID_IMyDispatch =
	{ 0x00020400, 0, 0, { 0xC0, 0, 0, 0, 0, 0, 0, 0x46 } };

AFX_INTERFACEMAP_ENTRY BASED_CODE CCmdTarget::_interfaceEntries[] =
{
	INTERFACE_PART(CCmdTarget, IID_IMyDispatch, Dispatch)
	{ NULL, (size_t)-1 }    // end of entries
};

void CCmdTarget::OnFinalRelease()
{
	delete this;
}

BOOL CCmdTarget::OnCreateAggregates()
{
	return TRUE;
}

LPUNKNOWN CCmdTarget::GetInterfaceHook(const void FAR*)
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Special access to view routing info

CView* CCmdTarget::pRoutingView = NULL;
CView* CCmdTarget::GetRoutingView()
{
	ASSERT(CCmdTarget::pRoutingView != NULL);
	return CCmdTarget::pRoutingView;
}

/////////////////////////////////////////////////////////////////////////////
// CCmdUI - User Interface for a command

// CCmdUI is a protocol class for all command handler variants
//      CCmdUI is an implementation class for menus and general dialog
//        controls (usually buttons)

CCmdUI::CCmdUI()
{
	// zero out everything
	m_nID = m_nIndex = m_nIndexMax = 0;
	m_pMenu = m_pSubMenu = m_pParentMenu = NULL;
	m_pOther = NULL;
	m_bEnableChanged = m_bContinueRouting = FALSE;
}

// default CCmdUI implementation only works for Menu Items
void CCmdUI::Enable(BOOL bOn)
{
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		ASSERT(m_nIndex < m_nIndexMax);
		m_pMenu->EnableMenuItem(m_nIndex, MF_BYPOSITION |
			(bOn ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
	}
	else
	{
		// enable/disable a control (i.e. child window)
		ASSERT(m_pOther != NULL);

		// if control has the focus, move the focus before disabling
		if (!bOn && (::GetFocus() == m_pOther->m_hWnd))
			m_pOther->GetParent()->GetNextDlgTabItem(m_pOther)->SetFocus();
		m_pOther->EnableWindow(bOn);
	}
	m_bEnableChanged = TRUE;
}

void CCmdUI::SetCheck(int nCheck)
{
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		// place checkmark next to menu item
		ASSERT(m_nIndex < m_nIndexMax);
		m_pMenu->CheckMenuItem(m_nIndex, MF_BYPOSITION |
			(nCheck ? MF_CHECKED : MF_UNCHECKED));
	}
	else
	{
		// we can only check buttons or controls acting like buttons
		ASSERT(m_pOther != NULL);
		if (m_pOther->SendMessage(WM_GETDLGCODE) & DLGC_BUTTON)
			m_pOther->SendMessage(BM_SETCHECK, nCheck);
		// otherwise ignore it
	}
}

static void PASCAL _AfxLoadDotBitmap(); // for swap tuning

void CCmdUI::SetRadio(BOOL bOn)
{
	SetCheck(bOn ? 1 : 0); // this default works for most things as well
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		// for menu item - use dot instead of checkmark
		ASSERT(m_nIndex < m_nIndexMax);

		if (afxData.hbmMenuDot == NULL)
			_AfxLoadDotBitmap();    // in INIT segment

		if (afxData.hbmMenuDot != NULL)
			SetMenuItemBitmaps(m_pMenu->m_hMenu, m_nIndex, MF_BYPOSITION,
				NULL, afxData.hbmMenuDot);
	}
}

void CCmdUI::SetText(LPCSTR lpszText)
{
	ASSERT(lpszText != NULL);
	ASSERT(AfxIsValidString(lpszText));

	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		ASSERT(m_nIndex < m_nIndexMax);
		VERIFY(m_pMenu->ModifyMenu(m_nIndex, MF_BYPOSITION |
			MF_STRING, m_nID, lpszText));
	}
	else
	{
		ASSERT(m_pOther != NULL);
		_AfxSmartSetWindowText(m_pOther->m_hWnd, lpszText);
	}
}

void CCmdUI::DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID(pTarget);

	if (m_nID == 0 || LOWORD(m_nID) == 0xFFFF)
		return;     // ignore invalid IDs

	m_bEnableChanged = FALSE;
	if (!pTarget->OnCmdMsg(m_nID, CN_UPDATE_COMMAND_UI, this, NULL))
		ASSERT(!m_bEnableChanged); // not routed

	if (bDisableIfNoHndler && !m_bEnableChanged)
	{
		AFX_CMDHANDLERINFO info;
		info.pTarget = NULL;

		BOOL bHndler = pTarget->OnCmdMsg(m_nID, CN_COMMAND, this, &info);

#ifdef _DEBUG
		if ((afxTraceFlags & 8) && !bHndler)
			TRACE1("No handler for command ID 0x%04X, "
				"disabling it\n", m_nID);
#endif
		// Enable or Disable based on whether there is a handler there
		Enable(bHndler);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Special init

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

static BYTE BASED_CODE rgbDot[] =
	{ 0x6, 0xF, 0xF, 0xF, 0x6 }; // simple byte bitmap, 1=> bit on
#define DOT_WIDTH   4
#define DOT_HEIGHT  5

static void PASCAL _AfxLoadDotBitmap()
{
	ASSERT(afxData.hbmMenuDot == NULL);
	// try to load special bitmap, else default to arrow
	CSize size = ::GetMenuCheckMarkDimensions();
	ASSERT(size.cx > 4 && size.cy > 5); // not too small please
	if (size.cx > 32)
		size.cx = 32;
	int iwRow = (size.cx + 15) >> 4;    // # of WORDs per raster line
	int nShift = (size.cx - DOT_WIDTH) / 2;     // # of bits to shift over
	if (nShift > 16 - DOT_WIDTH)
		nShift = 16 - DOT_WIDTH;    // maximum shift for 1 word

	if (size.cy > 32)
		size.cy = 32;

	// bitmap 2/4/4/4/2 pixels wide - centered (0 => black)
	BYTE rgbBitmap[32 * 2 * sizeof(WORD)];
	memset(rgbBitmap, 0xff, sizeof(rgbBitmap));

	BYTE* pbOut = &rgbBitmap[iwRow * sizeof(WORD) *
							((size.cy - (DOT_HEIGHT+1)) >> 1)];
	BYTE FAR* pbIn = rgbDot;
	for (int y = 0; y < DOT_HEIGHT; y++)
	{
		WORD w = ~(((WORD)*pbIn++) << nShift);
		// bitmaps are always hi-lo
		pbOut[0] = HIBYTE(w);
		pbOut[1] = LOBYTE(w);
		pbOut += iwRow * sizeof(WORD);
	}

	afxData.hbmMenuDot = ::CreateBitmap(size.cx, size.cy, 1, 1,
			(LPCSTR)&rgbBitmap);
	if (afxData.hbmMenuDot == NULL)
	{
		TRACE0("Warning: using system arrow bitmap instead of dot\n");
#define OBM_MNARROW         32739
		afxData.hbmMenuDot = ::LoadBitmap(NULL, MAKEINTRESOURCE(OBM_MNARROW));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget diagnostics

#ifdef _DEBUG
void CCmdTarget::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void CCmdTarget::AssertValid() const
{
	CObject::AssertValid();
}
#endif

/////////////////////////////////////////////////////////////////////////////
