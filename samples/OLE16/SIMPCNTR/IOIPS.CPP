//**********************************************************************
// File name: IOIPS.CPP
//
//      Implementation file for COleInPlaceSite
//
// Functions:
//
//      See IOIPS.H for class Definition
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
// CSimpleSite::COleInPlaceSite::QueryInterface
//
// Purpose:
//
//      Used for interface negotiation at the interface level.
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
//      CSimpleSite::QueryInterface SITE.CPP
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceSite::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	OutputDebugString("In IOIPS::QueryInterface\r\n");

	// delegate to the container Site
	return m_pSite->QueryInterface(riid, ppvObj);
}

//**********************************************************************
//
// CSimpleSite::COleInPlaceSite::AddRef
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

STDMETHODIMP_(ULONG) COleInPlaceSite::AddRef()
{
	OutputDebugString("In IOIPS::AddRef\r\n");

	// increment the interface reference count (for debugging only)
	++m_nCount;

	// delegate to the container Site
	return m_pSite->AddRef();
}

//**********************************************************************
//
// CSimpleSite::COleInPlaceSite::Release
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
//      CSimpleSite::Release        SITE.CPP
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) COleInPlaceSite::Release()
{
	OutputDebugString("In IOIPS::Release\r\n");
	// decrement the interface reference count (for debugging only)
	m_nCount--;

	// delegate to the container Site
	return m_pSite->Release();
}

//**********************************************************************
//
// COleInPlaceSite::GetWindow
//
// Purpose:
//
//      Returns the Window Handle of the client site
//
// Parameters:
//
//      HWND FAR* lphwnd    - place to return the handle
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

STDMETHODIMP COleInPlaceSite::GetWindow (HWND FAR* lphwnd)
{
	OutputDebugString("In IOIPS::GetWindow\r\n");

	// return the handle to our editing window.
	*lphwnd = m_pSite->m_lpDoc->m_hDocWnd;

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::ContextSensitiveHelp
//
// Purpose:
//
//
// Parameters:
//
//      BOOL fEnterMode - TRUE for entering Context Sensitive help mode
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
//      Be sure to read the technotes included with the OLE toolkit.
//
//********************************************************************

STDMETHODIMP COleInPlaceSite::ContextSensitiveHelp (BOOL fEnterMode)
{
	OutputDebugString("In IOIPS::ContextSensitiveHelp\r\n");

	if (m_pSite->m_lpDoc->m_lpApp->m_fCSHMode != fEnterMode)
		m_pSite->m_lpDoc->m_lpApp->m_fCSHMode = fEnterMode;

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::CanInPlaceActivate
//
// Purpose:
//
//      Object calls to find out if the container can InPlace activate
//
// Parameters:
//
//      None
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

STDMETHODIMP COleInPlaceSite::CanInPlaceActivate ()
{
	OutputDebugString("In IOIPS::CanInPlaceActivate\r\n");

	// return S_OK to indicate we can in-place activate
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::OnInPlaceActivate
//
// Purpose:
//
//      Called by the object on InPlace Activation
//
// Parameters:
//
//      None
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

STDMETHODIMP COleInPlaceSite::OnInPlaceActivate ()
{
	HRESULT hrErr;
	OutputDebugString("In IOIPS::OnInPlaceActivate\r\n");

	hrErr = m_pSite->m_lpOleObject->QueryInterface(
			IID_IOleInPlaceObject, (LPVOID FAR *)&m_pSite->m_lpInPlaceObject);
	if (hrErr != NOERROR)
		return ResultFromScode(E_FAIL);

	// return S_OK to indicate we can in-place activate.
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::OnUIActivate
//
// Purpose:
//
//      Object calls this method when it displays it's UI.
//
// Parameters:
//
//      None.
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

STDMETHODIMP COleInPlaceSite::OnUIActivate ()
{
	OutputDebugString("In IOIPS::OnUIActivate\r\n");

	m_pSite->m_lpDoc->m_fAddMyUI=FALSE;
	m_pSite->m_lpDoc->m_fInPlaceActive = TRUE;
	m_pSite->m_fInPlaceActive = TRUE;

	m_pSite->m_lpInPlaceObject->GetWindow((HWND FAR*)&m_pSite->m_hwndIPObj);

	// return S_OK to continue in-place activation
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::GetWindowContext
//
// Purpose:
//
//      Called by the object to get information for InPlace Negotiation.
//
// Parameters:
//
//      LPOLEINPLACEFRAME FAR* lplpFrame    - Location to return a pointer
//                                            to IOleInPlaceFrame.
//
//      LPOLEINPLACEUIWINDOW FAR* lplpDoc   - Location to return a pointer
//                                            to IOleInPlaceUIWindow.
//
//      LPRECT lprcPosRect                  - The rect that the object
//                                            occupies
//
//      LPRECT lprcClipRect                 - The clipping rect
//
//      LPOLEINPLACEFRAMEINFO lpFrameInfo   - Pointer to FRAMEINFO
//
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      COleInPlaceFrame::AddRef    IOIPF.CPP
//      CSimpleSite::GetObjRect     SITE.CPP
//      OutputDebugString           Windows API
//      SetMapMode                  Windows API
//      GetDC                       Windows API
//      ReleaseDC                   Windows API
//      CopyRect                    Windows API
//      GetClientRect               Windows API
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceSite::GetWindowContext (LPOLEINPLACEFRAME FAR* lplpFrame,
							   LPOLEINPLACEUIWINDOW FAR* lplpDoc,
							   LPRECT lprcPosRect,
							   LPRECT lprcClipRect,
							   LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	RECT rect;

	OutputDebugString("In IOIPS::GetWindowContext\r\n");

	// the frame is associated with the application object.
	// need to AddRef() it...
	m_pSite->m_lpDoc->m_lpApp->m_OleInPlaceFrame.AddRef();
	*lplpFrame = &m_pSite->m_lpDoc->m_lpApp->m_OleInPlaceFrame;
	*lplpDoc = NULL;  // must be NULL, cause we're SDI.

	// get the size of the object in pixels
	m_pSite->GetObjRect(&rect);

	// Copy this to the passed buffer
	CopyRect(lprcPosRect, &rect);

	// fill the clipping region
	GetClientRect(m_pSite->m_lpDoc->m_hDocWnd, &rect);
	CopyRect(lprcClipRect, &rect);

	// fill the FRAMEINFO
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = m_pSite->m_lpDoc->m_lpApp->m_hAppWnd;
	lpFrameInfo->haccel = NULL;
	lpFrameInfo->cAccelEntries = 0;

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::Scroll
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

STDMETHODIMP COleInPlaceSite::Scroll (SIZE scrollExtent)
{
	OutputDebugString("In IOIPS::Scroll\r\n");
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
// COleInPlaceSite::OnUIDeactivate
//
// Purpose:
//
//      Called by the object when its UI goes away
//
// Parameters:
//
//       BOOL fUndoable
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CSimpleAPP::AddFrameLevelUI APP.CPP
//      ResultFromScode             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceSite::OnUIDeactivate (BOOL fUndoable)
{
	OutputDebugString("In IOIPS::OnUIDeactivate\r\n");

	// need to clear this flag first
	m_pSite->m_lpDoc->m_fInPlaceActive = FALSE;
	m_pSite->m_fInPlaceActive = FALSE;

	m_pSite->m_lpDoc->m_lpApp->QueryNewPalette();
	m_pSite->m_lpDoc->m_lpApp->AddFrameLevelUI();
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::OnInPlaceDeactivate
//
// Purpose:
//
//      Called when the inplace session is over
//
// Parameters:
//
//      None
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

STDMETHODIMP COleInPlaceSite::OnInPlaceDeactivate ()
{
	OutputDebugString("In IOIPS::OnInPlaceDeactivate\r\n");

	if (m_pSite->m_lpInPlaceObject) {
		m_pSite->m_lpInPlaceObject->Release();
		m_pSite->m_lpInPlaceObject = NULL;
	}
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleInPlaceSite::DiscardUndoState
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

STDMETHODIMP COleInPlaceSite::DiscardUndoState ()
{
	OutputDebugString("In IOIPS::DiscardUndoState\r\n");
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
// COleInPlaceSite::DeactivateAndUndo
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

STDMETHODIMP COleInPlaceSite::DeactivateAndUndo ()
{
	OutputDebugString("In IOIPS::DeactivateAndUndo\r\n");
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
// COleInPlaceSite::OnPosRectChange
//
// Purpose:
//
//      The object calls this method when it's size changes during an
//      InPlace Session
//
// Parameters:
//
//      LPCRECT lprcPosRect -   The new object rect
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      GetClientRect                       Windows API
//      IOleObject::GetExtent               Object
//      IOleObject::QueryInterface          Object
//      IOleInPlaceObject::SetObjectRects   Object
//      IOleInPlaceObject::Release          Object
//      ResultFromScode                     OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleInPlaceSite::OnPosRectChange (LPCRECT lprcPosRect)
{
	OutputDebugString("In IOIPS::OnPosRectChange\r\n");

	// update the size in the document object
	// NOTE: here we must call IOleObject::GetExtent to get actual extents
	//       of the running object. IViewObject2::GetExtent returns the
	//       last cached extents.
	m_pSite->m_lpOleObject->GetExtent(DVASPECT_CONTENT, &m_pSite->m_sizel);
	RECT rect;
	GetClientRect(m_pSite->m_lpDoc->m_hDocWnd, &rect);

	// tell the object its new size
	m_pSite->m_lpInPlaceObject->SetObjectRects(lprcPosRect, &rect);

	return ResultFromScode(S_OK);
}
