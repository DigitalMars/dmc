//**********************************************************************
// File name: IOIPF.CPP
//
//      Implementation file for COleInPlaceFrame
//
// Functions:
//
//      See IOIPF.H for class definition
//
// Copyright (c) 1992 - 1993 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "pre.h"
#include "iocs.h"
#include "ias.h"
#include "ioipf.h"
#include "ioips.h"
#include "app.h"
#include "site.h"
#include "doc.h"


//**********************************************************************
//
// CSimpleApp::QueryInterface
//
// Purpose:
//
//      Used for interface negotiation at the Interface level.
//
// Parameters:
//
//      REFIID riid         -   A reference to the interface that is
//                              being queried.
//
//      LPVOID FAR* ppvObj  -   An out parameter to return a pointer to
//                              the interface.
//
// Return Value:
//
//      S_OK    -   The interface is supported.
//      S_FALSE -   The interface is not supported
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CSimpleApp::QueryInterface  APP.CPP
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	OutputDebugString("In IOIPF::QueryInterface\r\n");

// delegate to the document Object
	return m_pApp->QueryInterface(riid, ppvObj);
}

//**********************************************************************
//
// CSimpleApp::AddRef
//
// Purpose:
//
//      Adds to the reference count at the interface level.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the interface.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) COleInPlaceFrame::AddRef()
{
	OutputDebugString("In IOIPF::AddRef\r\n");

// delegate to the document Object
	m_pApp->AddRef();

// increment the interface reference count
	return ++m_nCount;
}

//**********************************************************************
//
// CSimpleApp::Release
//
// Purpose:
//
//      Decrements the reference count at this level
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the interface.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CSimpleApp::Release         APP.CPP
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) COleInPlaceFrame::Release()
{
	OutputDebugString("In IOIPF::Release\r\n");

// delegate to the document object
	m_pApp->Release();

// decrement the interface reference count
	return --m_nCount;
}

//**********************************************************************
//
// COleInPlaceFrame::GetWindow
//
// Purpose:
//
//      Returns the frame window handle
//
// Parameters:
//
//      HWND FAR* lphwnd    - Location to return the window handle
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::GetWindow (HWND FAR* lphwnd)
{
	OutputDebugString("In IOIPF::GetWindow\r\n");
	*lphwnd = m_pApp->m_hAppWnd;
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::ContextSensitiveHelp
//
// Purpose:
//
//      Used in implementing Context sensitive help
//
// Parameters:
//
//      BOOL fEnterMode -   TRUE if starting Context Sensitive help mode
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//      Be sure to read the technotes in the OLE toolkit.
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::ContextSensitiveHelp (BOOL fEnterMode)
{
	OutputDebugString("In IOIPF::ContextSensitiveHelp\r\n");

	m_pApp->m_fMenuMode = fEnterMode;

	return ResultFromScode(S_OK);
}
//**********************************************************************
//
// COleInPlaceFrame::GetBorder
//
// Purpose:
//
//      Returns the outermost border that frame adornments can be attached
//      during InPlace Activation.
//
// Parameters:
//
//      LPRECT lprectBorder - return parameter to contain the outermost
//                            rect for frame adornments
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GetClientRect               Windows API
//      CopyRect                    Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::GetBorder (LPRECT lprectBorder)
{
	RECT rect;

	OutputDebugString("In IOIPF::GetBorder\r\n");

	// get the rect for the entire frame.
	GetClientRect(m_pApp->m_hAppWnd, &rect);

	CopyRect(lprectBorder, &rect);

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::RequestBorderSpace
//
// Purpose:
//
//      Approves/Denies requests for border space during InPlace
//      negotiation.
//
// Parameters:
//
//      LPCBORDERWIDTHS lpborderwidths  - The width in pixels needed on
//                                        each side of the frame.
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//      This implementation doesn't care about how much border space
//      is used.  It always returns S_OK.
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::RequestBorderSpace (LPCBORDERWIDTHS lpborderwidths)
{
	OutputDebugString("In IOIPF::RequestBorderSpace\r\n");

	// always approve the request
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::SetBorderSpace
//
// Purpose:
//
//      The object calls this method when it is actually going to
//      start using the border space.
//
// Parameters:
//
//      LPCBORDERWIDTHS lpborderwidths  - Border space actually being used
//                                        by the object
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                        Location
//
//      CSimpleApp::AddFrameLevelTools  APP.CPP
//      OutputDebugString               Windows API
//      GetClientRect                   Windows API
//      MoveWindow                      Windows API
//      ResultFromScode                 Windows API
//
// Comments:
//
//      This routine could be a little smarter and check to see if
//      the object is requesting the entire client area of the
//      window.
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::SetBorderSpace (LPCBORDERWIDTHS lpborderwidths)
{

	OutputDebugString("In IOIPF::SetBorderSpace\r\n");

	if (lpborderwidths == NULL)
		m_pApp->AddFrameLevelTools();
	else
		{
		RECT rect;

		GetClientRect(m_pApp->m_hAppWnd, &rect);

		MoveWindow( m_pApp->m_lpDoc->m_hDocWnd,
					rect.left + lpborderwidths->left,
					rect.top + lpborderwidths->top,
					rect.right - lpborderwidths->right - lpborderwidths->left,
					rect.bottom - lpborderwidths->bottom - lpborderwidths->top,
					TRUE);
		}
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::SetActiveObject
//
// Purpose:
//
//
// Parameters:
//
//      LPOLEINPLACEACTIVEOBJECT lpActiveObject     -   Pointer to the
//                                                      objects
//                                                      IOleInPlaceActiveObject
//                                                      interface
//
//      LPCSTR lpszObjName                          -   Name of the object
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      IOleInPlaceActiveObject::AddRef     Object
//      IOleInPlaceActiveObject::Release    Object
//      ResultFromScode                     OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::SetActiveObject (LPOLEINPLACEACTIVEOBJECT lpActiveObject,LPCSTR lpszObjName)
{

	OutputDebugString("In IOIPF::SetActiveObject\r\n");

	// AddRef() it and save it...
	if (lpActiveObject)
		{
		lpActiveObject->AddRef();

		lpActiveObject->GetWindow(&m_pApp->m_hwndUIActiveObj);
		if (m_pApp->m_hwndUIActiveObj)
			SendMessage(m_pApp->m_hwndUIActiveObj, WM_QUERYNEWPALETTE, 0, 0L);
		}
	else
		{
		if (m_pApp->m_lpDoc->m_lpActiveObject)
			m_pApp->m_lpDoc->m_lpActiveObject->Release();
		m_pApp->m_hwndUIActiveObj = NULL;
		}

	// in an MDI app, this method really shouldn't be called,
	// this method associated with the doc is called instead.

	m_pApp->m_lpDoc->m_lpActiveObject = lpActiveObject;
	// should set window title here

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::InsertMenus
//
// Purpose:
//
//      Inserts the container menu into the combined menu
//
// Parameters:
//
//      HMENU hmenuShared                   -   Menu Handle to be set.
//      LPOLEMENUGROUPWIDTHS lpMenuWidths   -   Width of menus
//
// Return Value:
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      AppendMenu                  Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::InsertMenus (HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	OutputDebugString("In IOIPF::InsertMenus\r\n");

	AppendMenu(hmenuShared, MF_BYPOSITION | MF_POPUP, m_pApp->m_hFileMenu, "&File");
	AppendMenu(hmenuShared, MF_BYPOSITION | MF_POPUP, m_pApp->m_hHelpMenu, "&Other");

	lpMenuWidths->width[0] = 1;
	lpMenuWidths->width[2] = 0;
	lpMenuWidths->width[4] = 1;

	return ResultFromScode(S_OK);
}


//**********************************************************************
//
// COleInPlaceFrame::SetMenu
//
// Purpose:
//
//      Sets the application menu to the combined menu
//
// Parameters:
//
//      HMENU hmenuShared       - The combined menu
//
//      HOLEMENU holemenu       - Used by OLE
//
//      HWND hwndActiveObject   - Used by OLE
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      SetMenu                     Windows API
//      OleSetMenuDescriptor        OLE API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::SetMenu (HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{

	OutputDebugString("In IOIPF::SetMenu\r\n");

	HMENU hMenu = m_pApp->m_hMainMenu;

	if (holemenu)
		hMenu = hmenuShared;

	// call the windows api, not this method
	::SetMenu (m_pApp->m_hAppWnd, hMenu);

	OleSetMenuDescriptor(holemenu, m_pApp->m_hAppWnd, hwndActiveObject, this, m_pApp->m_lpDoc->m_lpActiveObject);

	return ResultFromScode(S_OK);
}


//**********************************************************************
//
// COleInPlaceFrame::RemoveMenus
//
// Purpose:
//
//      Removes the container menus from the combined menu
//
// Parameters:
//
//      HMENU hmenuShared   - Handle to the combined menu.
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GetMenuItemCount            Windows API
//      RemoveMenu                  Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::RemoveMenus (HMENU hmenuShared)
{
	int retval;

	OutputDebugString("In IOIPF::RemoveMenus\r\n");

	while ((retval = GetMenuItemCount(hmenuShared)) && (retval != -1))
		RemoveMenu(hmenuShared, 0, MF_BYPOSITION);

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::SetStatusText
//
// Purpose:
//
//      Not Implemented
//
// Parameters:
//
//      Not Implemented
//
// Return Value:
//
//      Not Implemented
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      This function is not implemented due to the fact
//      that this application does not have a status bar.
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::SetStatusText (LPCSTR lpszStatusText)
{
	OutputDebugString("In IOIPF::SetStatusText\r\n");
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
// COleInPlaceFrame::EnableModeless
//
// Purpose:
//
//      Enables/Disables container modeless dialogs
//
// Parameters:
//
//      BOOL fEnable    - Enable/Disable
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      There are no modeless dialogs in this application, so the
//      implementation of this method is trivial.
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::EnableModeless (BOOL fEnable)
{
	OutputDebugString("In IOIPF::EnableModeless\r\n");
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceFrame::TranslateAccelerator
//
// Purpose:
//
//      Not Implemented
//
// Parameters:
//
//      Not Implemented
//
// Return Value:
//
//      Not Implemented
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Not Implemented
//
//********************************************************************

STDMETHODIMP COleInPlaceFrame::TranslateAccelerator (LPMSG lpmsg, WORD wID)
{
	OutputDebugString("In IOIPF::TranslateAccelerator\r\n");
	return ResultFromScode(S_FALSE);
}
