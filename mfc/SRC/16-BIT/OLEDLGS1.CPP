// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <malloc.h>

#ifdef AFX_OLE2_SEG
#pragma code_seg(AFX_OLE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

extern UINT CALLBACK AFX_EXPORT
_AfxOleHookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

////////////////////////////////////////////////////////////////////////////
// InsertObject dialog wrapper

IMPLEMENT_DYNAMIC(COleInsertDialog, COleDialog)

COleInsertDialog::COleInsertDialog(DWORD dwFlags /* = 0 */,
	CWnd* pParentWnd /* = NULL */) : COleDialog(pParentWnd)
{
	memset(&m_io, 0, sizeof(m_io)); // initialize structure to 0/NULL

	// fill in common part
	m_io.cbStruct = sizeof(m_io);
	m_io.dwFlags = dwFlags;
	if (_AfxHelpEnabled())
		m_io.dwFlags |= IOF_SHOWHELP;
	m_io.lpfnHook = _AfxOleHookProc;
	m_nIDHelp = AFX_IDD_INSERTOBJECT;

	// specific to this dialog
	m_io.lpszFile = m_szFileName;
	m_io.cchFile = sizeof(m_szFileName);
	m_szFileName[0] = '\0';
}

COleInsertDialog::~COleInsertDialog()
{
	// cleanup metafile handle if it was created
	if (m_io.hMetaPict != NULL)
		OleUIMetafilePictIconFree(m_io.hMetaPict);
}

int COleInsertDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_io.lpfnHook != NULL);  // can still be a user hook

	m_io.hWndOwner = PreModal();
	int iResult = MapResult(::OleUIInsertObject(&m_io));
	PostModal();

	return iResult;
}

UINT COleInsertDialog::GetSelectionType() const
{
	ASSERT_VALID(this);

	if (m_io.dwFlags & IOF_SELECTCREATEFROMFILE)
	{
		if (m_io.dwFlags & IOF_CHECKLINK)
			return linkToFile;
		else
			return insertFromFile;
	}
	ASSERT(m_io.dwFlags & IOF_SELECTCREATENEW);
	return createNewItem;
}

// allocate an item first, then call this fuction to create it
BOOL COleInsertDialog::CreateItem(COleClientItem* pNewItem)
{
	ASSERT_VALID(pNewItem);

	// switch on selection type
	UINT selType = GetSelectionType();
	BOOL bResult;

	switch (selType)
	{
	case linkToFile:
		// link to file selected
		ASSERT(m_szFileName[0] != 0);
		bResult = pNewItem->CreateLinkFromFile(m_szFileName);
		break;
	case insertFromFile:
		// insert file selected
		ASSERT(m_szFileName[0] != 0);
		bResult = pNewItem->CreateFromFile(m_szFileName);
		break;
	default:
		// otherwise must be create new
		ASSERT(selType == createNewItem);
		bResult = pNewItem->CreateNewItem(m_io.clsid);
		break;
	}

	// deal with Display As Iconic option
	if (bResult && GetDrawAspect() == DVASPECT_ICON)
	{
		// setup iconic cache (it will draw iconic by default as well)
		if (!pNewItem->SetIconicMetafile(m_io.hMetaPict))
		{
			TRACE0("Warning: failed to set iconic aspect in "
				"COleInsertDialog::CreateItem.\n");
			return TRUE;
		}

		// since picture was set OK, draw as iconic as well...
		pNewItem->SetDrawAspect(DVASPECT_ICON);
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// COleInsertDialog diagnostics

#ifdef _DEBUG
void COleInsertDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);
	AFX_DUMP1(dc, "\nm_szFileName = ", m_szFileName);

	AFX_DUMP1(dc, "\nm_io.cbStruct = ", m_io.cbStruct);
	AFX_DUMP1(dc, "\nm_io.dwFlags = ", (LPVOID)m_io.dwFlags);
	AFX_DUMP1(dc, "\nm_io.hWndOwner = ", (UINT)m_io.hWndOwner);
	AFX_DUMP1(dc, "\nm_io.lpszCaption = ", m_io.lpszCaption);
	AFX_DUMP1(dc, "\nm_io.lCustData = ", (LPVOID)m_io.lCustData);
	AFX_DUMP1(dc, "\nm_io.hInstance = ", (UINT)m_io.hInstance);
	AFX_DUMP1(dc, "\nm_io.lpszTemplate = ", (LPVOID)m_io.lpszTemplate);
	AFX_DUMP1(dc, "\nm_io.hResource = ", (UINT)m_io.hResource);
	if (m_io.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");

	AFX_DUMP1(dc, "\nm_io.hMetaPict = ", (UINT)m_io.hMetaPict);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COleConvertDialog

IMPLEMENT_DYNAMIC(COleConvertDialog, COleDialog)

COleConvertDialog::COleConvertDialog(COleClientItem* pItem, DWORD dwFlags,
	CLSID FAR* pClassID, CWnd* pParentWnd) : COleDialog(pParentWnd)
{
	if (pItem != NULL)
		ASSERT_VALID(pItem);
	ASSERT(pClassID == NULL || AfxIsValidAddress(pClassID, sizeof(CLSID), FALSE));

	memset(&m_cv, 0, sizeof(m_cv)); // initialize structure to 0/NULL
	if (pClassID != NULL)
		m_cv.clsid = *pClassID;

	// fill in common part
	m_cv.cbStruct = sizeof(m_cv);
	m_cv.dwFlags = dwFlags;
	if (_AfxHelpEnabled())
		m_cv.dwFlags |= CF_SHOWHELPBUTTON;
	m_cv.lpfnHook = _AfxOleHookProc;
	m_nIDHelp = AFX_IDD_CONVERT;

	// specific to this dialog
	m_cv.fIsLinkedObject = pItem->GetType() == OT_LINK;
	m_cv.dvAspect = pItem->GetDrawAspect();
	if (pClassID == NULL && !m_cv.fIsLinkedObject)
	{
		// for embeddings, attempt to get class ID from the storage
		if (ReadClassStg(pItem->m_lpStorage, &m_cv.clsid) == NOERROR)
			pClassID = &m_cv.clsid;

		// attempt to get user type from storage
		CLIPFORMAT cf;
		ReadFmtUserTypeStg(pItem->m_lpStorage, &cf, &m_cv.lpszUserType);
		m_cv.wFormat = (WORD)cf;
	}
	// get class id if neded
	if (pClassID == NULL)
	{
		// no class ID in the storage, use class ID of the object
		pItem->GetClassID(&m_cv.clsid);
	}

	// get user type if needed
	if (m_cv.lpszUserType == NULL)
	{
		char szUserType[256];

		// no user type in storge, get user type from class ID
		if (!OleStdGetUserTypeOfClass(m_cv.clsid, szUserType,
			sizeof(szUserType), NULL))
		{
			szUserType[0] = '?';
			szUserType[1] = 0;
			_AfxLoadString(IDS_PSUNKNOWNTYPE, szUserType);
		}
		// copy it to OLE string (allocated by task allocator)
		m_cv.lpszUserType = OleStdCopyString(szUserType, NULL);
	}
	m_cv.hMetaPict = pItem->GetIconicMetafile();
}

COleConvertDialog::~COleConvertDialog()
{
	// cleanup metafile handle if it was created
	if (m_cv.hMetaPict != NULL)
		OleUIMetafilePictIconFree(m_cv.hMetaPict);
}

int COleConvertDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_cv.lpfnHook != NULL);  // can still be a user hook

	m_cv.hWndOwner = PreModal();
	int iResult = MapResult(::OleUIConvert(&m_cv));
	PostModal();

	return iResult;
}

BOOL COleConvertDialog::DoConvert(COleClientItem* pItem)
{
	ASSERT_VALID(pItem);

	BeginWaitCursor();

	UINT selType = GetSelectionType();
	BOOL bResult = TRUE;

	switch (selType)
	{
	case convertItem:
		bResult = pItem->ConvertTo(m_cv.clsidNew);
		break;
	case activateAs:
		bResult = pItem->ActivateAs(m_cv.lpszUserType, m_cv.clsid, m_cv.clsidNew);
		break;
	default:
		ASSERT(selType == noConversion);
		break;
	}

	if (!bResult)
	{
		// if unable to convert the object show message box
		EndWaitCursor();
		AfxMessageBox(AFX_IDP_FAILED_TO_CONVERT);
		return FALSE;
	}

	// change to iconic/content view if changed
	if ((DVASPECT)m_cv.dvAspect != pItem->GetDrawAspect())
	{
		pItem->OnChange(OLE_CHANGED_ASPECT, (DWORD)m_cv.dvAspect);
		pItem->SetDrawAspect((DVASPECT)m_cv.dvAspect);
	}

	// change the actual icon as well
	if (m_cv.fObjectsIconChanged)
	{
		pItem->SetIconicMetafile(m_cv.hMetaPict);
		if (pItem->GetDrawAspect() == DVASPECT_ICON)
			pItem->OnChange(OLE_CHANGED, (DWORD)DVASPECT_ICON);
	}

	EndWaitCursor();
	return TRUE;
}

UINT COleConvertDialog::GetSelectionType() const
{
	ASSERT_VALID(this);

	if (m_cv.clsid != m_cv.clsidNew)
	{
		if (m_cv.dwFlags & CF_SELECTCONVERTTO)
			return convertItem;
		else if (m_cv.dwFlags & CF_SELECTACTIVATEAS)
			return activateAs;
	}
	return noConversion;
}

/////////////////////////////////////////////////////////////////////////////
// COleConvertDialog diagnostics

#ifdef _DEBUG
void COleConvertDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_cv.cbStruct = ", m_cv.cbStruct);
	AFX_DUMP1(dc, "\nm_cv.dwFlags = ", (LPVOID)m_cv.dwFlags);
	AFX_DUMP1(dc, "\nm_cv.hWndOwner = ", (UINT)m_cv.hWndOwner);
	AFX_DUMP1(dc, "\nm_cv.lpszCaption = ", m_cv.lpszCaption);
	AFX_DUMP1(dc, "\nm_cv.lCustData = ", (LPVOID)m_cv.lCustData);
	AFX_DUMP1(dc, "\nm_cv.hInstance = ", (UINT)m_cv.hInstance);
	AFX_DUMP1(dc, "\nm_cv.lpszTemplate = ", (LPVOID)m_cv.lpszTemplate);
	AFX_DUMP1(dc, "\nm_cv.hResource = ", (UINT)m_cv.hResource);
	if (m_cv.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");

	AFX_DUMP1(dc, "\nm_cv.dvAspect = ", (UINT)m_cv.dvAspect);
	AFX_DUMP1(dc, "\nm_cv.wFormat = ", (UINT)m_cv.wFormat);
	AFX_DUMP1(dc, "\nm_cv.fIsLinkedObject = ", m_cv.fIsLinkedObject);
	AFX_DUMP1(dc, "\nm_cv.hMetaPict = ", (UINT)m_cv.hMetaPict);
	AFX_DUMP1(dc, "\nm_cv.lpszUserType = ", m_cv.lpszUserType);
	AFX_DUMP1(dc, "\nm_cv.fObjectsIconChanged = ", m_cv.fObjectsIconChanged);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// COleChangeIconDialog

IMPLEMENT_DYNAMIC(COleChangeIconDialog, COleDialog)

COleChangeIconDialog::COleChangeIconDialog(COleClientItem* pItem,
	DWORD dwFlags, CWnd* pParentWnd) : COleDialog(pParentWnd)
{
	if (pItem != NULL)
		ASSERT_VALID(pItem);

	memset(&m_ci, 0, sizeof(m_ci)); // initialize structure to 0/NULL

	// fill in common part
	m_ci.cbStruct = sizeof(m_ci);
	m_ci.dwFlags = dwFlags;
	if (_AfxHelpEnabled())
		m_ci.dwFlags |= CIF_SHOWHELP;
	m_ci.lpfnHook = _AfxOleHookProc;
	m_nIDHelp = AFX_IDD_CHANGEICON;

	// specific to this dialog
	if (pItem != NULL)
	{
		pItem->GetClassID(&m_ci.clsid);
		m_ci.hMetaPict = pItem->GetIconicMetafile();
	}
}

COleChangeIconDialog::~COleChangeIconDialog()
{
	// cleanup metafile handle if it was created
	if (m_ci.hMetaPict != NULL)
		OleUIMetafilePictIconFree(m_ci.hMetaPict);
}

int COleChangeIconDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_ci.lpfnHook != NULL);  // can still be a user hook

	m_ci.hWndOwner = PreModal();
	int iResult = MapResult(::OleUIChangeIcon(&m_ci));
	PostModal();

	return iResult;
}

BOOL COleChangeIconDialog::DoChangeIcon(COleClientItem* pItem)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pItem);

	// set the picture
	if (!pItem->SetIconicMetafile(GetIconicMetafile()))
		return FALSE;

	// notify the item of the change if the current draw aspect is ICON
	if (pItem->GetDrawAspect() == DVASPECT_ICON)
		pItem->OnChange(OLE_CHANGED, (DWORD)DVASPECT_ICON);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleChangeIconDialog diagnostics

#ifdef _DEBUG
void COleChangeIconDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_ci.cbStruct = ", m_ci.cbStruct);
	AFX_DUMP1(dc, "\nm_ci.dwFlags = ", (LPVOID)m_ci.dwFlags);
	AFX_DUMP1(dc, "\nm_ci.hWndOwner = ", (UINT)m_ci.hWndOwner);
	AFX_DUMP1(dc, "\nm_ci.lpszCaption = ", m_ci.lpszCaption);
	AFX_DUMP1(dc, "\nm_ci.lCustData = ", (LPVOID)m_ci.lCustData);
	AFX_DUMP1(dc, "\nm_ci.hInstance = ", (UINT)m_ci.hInstance);
	AFX_DUMP1(dc, "\nm_ci.lpszTemplate = ", (LPVOID)m_ci.lpszTemplate);
	AFX_DUMP1(dc, "\nm_ci.hResource = ", (UINT)m_ci.hResource);
	if (m_ci.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");

	AFX_DUMP1(dc, "\nm_ci.hMetaPict = ", (UINT)m_ci.hMetaPict);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// COleLinksDialog

IMPLEMENT_DYNAMIC(COleLinksDialog, COleDialog)

COleLinksDialog::COleLinksDialog(
	COleDocument* pDoc, CView* pView,
	DWORD dwFlags, CWnd* pParentWnd) : COleDialog(pParentWnd)
{
	ASSERT_VALID(pDoc);
	if (pView != NULL)
		ASSERT_VALID(pView);

	memset(&m_el, 0, sizeof(m_el)); // initialize structure to 0/NULL

	// fill in common part
	m_el.cbStruct = sizeof(m_el);
	m_el.dwFlags = dwFlags;
	if (_AfxHelpEnabled())
		m_el.dwFlags |= ELF_SHOWHELP;
	m_el.lpfnHook = _AfxOleHookProc;
	m_nIDHelp = AFX_IDD_EDITLINKS;

	// specific to this dialog
	m_pDocument = pDoc;
	if (pView != NULL)
		m_pSelectedItem = m_pDocument->GetPrimarySelectedItem(pView);
	else
		m_pSelectedItem = NULL;
	m_el.lpOleUILinkContainer = &m_xOleUILinkContainer;
}

COleLinksDialog::~COleLinksDialog()
{
}

int COleLinksDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_el.lpfnHook != NULL);  // can still be a user hook

	// this function is always used for updating links
	m_bUpdateEmbeddings = FALSE;
	m_bUpdateLinks = TRUE;

	m_el.hWndOwner = PreModal();
	int iResult = MapResult(::OleUIEditLinks(&m_el));
	PostModal();

	return iResult;
}

BEGIN_INTERFACE_MAP(COleLinksDialog, COleDialog)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleLinksDialog::XOleUILinkContainer

STDMETHODIMP_(ULONG) COleLinksDialog::XOleUILinkContainer::AddRef()
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleLinksDialog::XOleUILinkContainer::Release()
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(DWORD) COleLinksDialog::XOleUILinkContainer::GetNextLink(
	DWORD dwLink)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	if (dwLink == 0)
	{
		// start enumerating from the beginning
		pThis->m_pos = pThis->m_pDocument->GetStartPosition();
	}
	COleDocument* pDoc = pThis->m_pDocument;
	COleClientItem* pItem;
	while ((pItem = pDoc->GetNextClientItem(pThis->m_pos)) != NULL)
	{
		// check for links
		OLE_OBJTYPE objType = pItem->GetType();
		if (pThis->m_bUpdateLinks && objType == OT_LINK)
		{
			// link found -- return it
			return (DWORD)(void FAR*)pItem;
		}
		// check for embeddings
		if (pThis->m_bUpdateEmbeddings && objType == OT_EMBEDDED)
		{
			// embedding w/mismatched target device
			return (DWORD)(void FAR*)pItem;
		}
	}
	return 0;   // link not found
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::SetLinkUpdateOptions(
	DWORD dwLink, DWORD dwUpdateOpt)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));
	ASSERT(pItem->GetType() == OT_LINK);

	SCODE sc;
	TRY
	{
		// item is a link -- get it's link options
		pItem->SetLinkUpdateOptions((OLEUPDATE)dwUpdateOpt);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::GetLinkUpdateOptions(
	DWORD dwLink, DWORD FAR* lpdwUpdateOpt)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));

	SCODE sc;
	TRY
	{
		if (pItem->GetType() == OT_LINK)
			*lpdwUpdateOpt = pItem->GetLinkUpdateOptions();
		else
			*lpdwUpdateOpt = OLEUPDATE_ALWAYS;  // make believe it is auto-link
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::SetLinkSource(
	DWORD       dwLink,
	LPSTR       lpszDisplayName,
	ULONG       lenFileName,
	ULONG FAR*  pchEaten,
	BOOL        fValidateSource)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));
	ASSERT(pItem->GetType() == OT_LINK);

	HRESULT hr;
	LPMONIKER lpMoniker;
	CLSID clsid = CLSID_NULL;

	// assume link is not available
	pItem->m_bLinkUnavail = FALSE;

	if (fValidateSource)
	{
		// validate the source by parsing it into a moniker
		LPBC lpbc;
		hr = ::CreateBindCtx(0, (LPBC FAR*)&lpbc);
		if (hr != NOERROR)
			return hr;

		// parse the moniker
		hr = ::MkParseDisplayName(lpbc, lpszDisplayName, pchEaten, &lpMoniker);
		if (hr != NOERROR)
		{
			lpbc->Release();
			return hr;
		}

		// attempt to bind to the moniker that was successfully parse
		//      (this rarely fails -- but might)
		LPOLEOBJECT lpOleObject = NULL;
		hr = lpMoniker->BindToObject(lpbc, NULL,
			IID_IOleObject, (LPLP)&lpOleObject);
		if (hr != NOERROR)
		{
			pItem->m_bLinkUnavail = TRUE;
			lpbc->Release();
			_AfxRelease((LPUNKNOWN*)&lpOleObject);
			return hr;
		}
		ASSERT(lpOleObject != NULL);

		// have to update the class id in the default handler
		//      (or GetUserType will no longer work)
		// this is done by calling GetUserClassID while running
		lpOleObject->GetUserClassID(&clsid);
		pItem->m_bLinkUnavail = FALSE;

		// cleanup
		lpOleObject->Release();
		lpbc->Release();
	}
	else
	{
		LPMONIKER       lpmkFile = NULL;
		LPMONIKER       lpmkItem = NULL;
		char            szName[OLEUI_CCHPATHMAX];

		lstrcpyn(szName, lpszDisplayName, (int)lenFileName + 1);
		hr = ::CreateFileMoniker(szName, &lpmkFile);
		if (lpmkFile == NULL)
			return hr;

		if (lstrlen(lpszDisplayName) > (int)lenFileName)
		{
			// have item name
			lstrcpy(szName, lpszDisplayName + lenFileName + 1);
			hr = ::CreateItemMoniker(OLESTDDELIM, szName, &lpmkItem);
			if (lpmkItem == NULL)
			{
				lpmkFile->Release();
				return hr;
			}

			// create composite
			hr = ::CreateGenericComposite(lpmkFile, lpmkItem, &lpMoniker);

			// release component monikers
			_AfxRelease((LPUNKNOWN*)&lpmkFile);
			_AfxRelease((LPUNKNOWN*)&lpmkItem);

			// creating composite moniker may fail...
			if (hr != NOERROR)
				return hr;
		}
		else
		{
			// file moniker is full moniker
			lpMoniker = lpmkFile;
		}
	}

	// get IOleLink interface
	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(pItem->m_lpObject, IOleLink);
	ASSERT(lpOleLink != NULL);

	if (lpMoniker != NULL)
	{
		// link source validated -- go ahead and set it
		hr = lpOleLink->SetSourceMoniker(lpMoniker, clsid);
		lpMoniker->Release();
		if (hr != NOERROR)
		{
			lpOleLink->Release();
			return hr;
		}
	}
	else
	{
		// link source not validated -- just set the display name
		//      (this link will not bind properly)
		hr = lpOleLink->SetSourceDisplayName(lpszDisplayName);
		if (hr != NOERROR)
		{
			lpOleLink->Release();
			return hr;
		}
	}

	lpOleLink->Release();
	return NOERROR;
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::GetLinkSource(
	DWORD       dwLink,
	LPSTR FAR*  lplpszDisplayName,
	ULONG FAR*  lplenFileName,
	LPSTR FAR*  lplpszFullLinkType,
	LPSTR FAR*  lplpszShortLinkType,
	BOOL FAR*   lpfSourceAvailable,
	BOOL FAR*   lpfIsSelected)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));
	ASSERT(pItem->GetType() == OT_LINK);

	// set OUT params to NULL
	*lplpszDisplayName  = NULL;
	*lplpszFullLinkType = NULL;
	*lplpszShortLinkType= NULL;
	*lplenFileName      = NULL;
	*lpfSourceAvailable = !pItem->m_bLinkUnavail;

	// get IOleLink interface
	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(pItem->m_lpObject, IOleLink);
	ASSERT(lpOleLink != NULL);

	// get moniker & object information
	LPMONIKER lpMoniker;
	if (lpOleLink->GetSourceMoniker(&lpMoniker) == NOERROR)
	{
		pItem->m_lpObject->GetUserType(USERCLASSTYPE_FULL,
			lplpszFullLinkType);
		pItem->m_lpObject->GetUserType(USERCLASSTYPE_SHORT,
			lplpszShortLinkType);
		*lplenFileName = ::OleStdGetLenFilePrefixOfMoniker(lpMoniker);
		lpMoniker->Release();
	}

	// get source display name for moniker
	HRESULT hr = lpOleLink->GetSourceDisplayName(lplpszDisplayName);
	lpOleLink->Release();
	if (hr != NOERROR)
		return hr;

	// see if item is selected if specified
	if (lpfIsSelected)
		*lpfIsSelected = (pThis->m_pSelectedItem == pItem);

	return NOERROR;
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::OpenLinkSource(DWORD dwLink)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));
	ASSERT(pItem->GetType() == OT_LINK);

	SCODE sc;
	TRY
	{
		// Note: no need for valid CView* since links don't activate inplace
		pItem->DoVerb(OLEIVERB_SHOW, NULL);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::UpdateLink(
	DWORD dwLink,
	BOOL fErrorMessage,
	BOOL fErrorAction)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));

	SCODE sc;
	TRY
	{
		if (pItem->m_bLinkUnavail || !pItem->IsLinkUpToDate())
		{
			// link not up-to-date, attempt to update it
			if (!pItem->UpdateLink())
				AfxThrowOleException(pItem->GetLastStatus());
			pItem->m_bLinkUnavail = FALSE;
		}
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		pItem->m_bLinkUnavail = TRUE;
		sc = COleException::Process(e);
		pItem->ReportError(sc);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinksDialog::XOleUILinkContainer::CancelLink(DWORD dwLink)
{
	METHOD_PROLOGUE(COleLinksDialog, OleUILinkContainer)
	ASSERT_VALID(pThis);

	COleClientItem* pItem = (COleClientItem*)dwLink;
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleClientItem)));
	ASSERT(pItem->GetType() == OT_LINK);

	SCODE sc = E_FAIL;
	TRY
	{
		if (pItem->FreezeLink())
			sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	// report error
	if (sc != S_OK)
		pItem->ReportError(sc);

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// COleLinksDialog diagnostics

#ifdef _DEBUG
void COleLinksDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);
	AFX_DUMP1(dc, "\nwith document ", m_pDocument);
	if (m_pSelectedItem != NULL)
		AFX_DUMP1(dc, "\nwith selected item ", m_pSelectedItem);
	else
		AFX_DUMP0(dc, "\nwith no selected item");
	AFX_DUMP1(dc, "\nm_pos = ", m_pos);
	AFX_DUMP1(dc, "\nm_bUpdateLinks = ", m_bUpdateLinks);
	AFX_DUMP1(dc, "\nm_bUpdateEmbeddings = ", m_bUpdateEmbeddings);

	AFX_DUMP1(dc, "\nm_el.cbStruct = ", m_el.cbStruct);
	AFX_DUMP1(dc, "\nm_el.dwFlags = ", (LPVOID)m_el.dwFlags);
	AFX_DUMP1(dc, "\nm_el.hWndOwner = ", (UINT)m_el.hWndOwner);
	AFX_DUMP1(dc, "\nm_el.lpszCaption = ", m_el.lpszCaption);
	AFX_DUMP1(dc, "\nm_el.lCustData = ", (LPVOID)m_el.lCustData);
	AFX_DUMP1(dc, "\nm_el.hInstance = ", (UINT)m_el.hInstance);
	AFX_DUMP1(dc, "\nm_el.lpszTemplate = ", (LPVOID)m_el.lpszTemplate);
	AFX_DUMP1(dc, "\nm_el.hResource = ", (UINT)m_el.hResource);
	if (m_el.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");
}

void COleLinksDialog::AssertValid() const
{
	COleDialog::AssertValid();
	m_pDocument->AssertValid();
	if (m_pSelectedItem != NULL)
		m_pSelectedItem->AssertValid();
}
#endif

/////////////////////////////////////////////////////////////////////////////
// COleUpdateDialog

IMPLEMENT_DYNAMIC(COleUpdateDialog, COleLinksDialog)

COleUpdateDialog::COleUpdateDialog(COleDocument* pDoc,
	BOOL bUpdateLinks, BOOL bUpdateEmbeddings, CWnd* pParentWnd)
		: COleLinksDialog(pDoc, NULL, 0, pParentWnd)
{
	ASSERT_VALID(pDoc);
	ASSERT(bUpdateLinks || bUpdateEmbeddings);

	// base class COleLinksDialog should have set these up
	ASSERT(m_pDocument == pDoc);
	ASSERT(m_pSelectedItem == NULL);

	// non-base class parameters
	m_bUpdateLinks = bUpdateLinks;
	m_bUpdateEmbeddings = bUpdateEmbeddings;
	m_strCaption.LoadString(AFX_IDS_UPDATING_ITEMS);
}

COleUpdateDialog::~COleUpdateDialog()
{
}

int COleUpdateDialog::DoModal()
{
	ASSERT_VALID(this);

	// first count number of links/embeddings to be updated
	DWORD dwLink = 0;
	int cLinks = 0;
	while ((dwLink = m_el.lpOleUILinkContainer->GetNextLink(dwLink)) != 0)
		++cLinks;
	// when no links are out-of-date, don't bother
	if (cLinks == 0)
		return IDCANCEL;

	// bring up the dialog that processes all the links
	HWND hWndParent = PreModal();
	BOOL bResult = OleUIUpdateLinks(m_el.lpOleUILinkContainer,
		hWndParent, (LPSTR)(LPCSTR)m_strCaption, cLinks);
	PostModal();

	return bResult ? IDOK : IDABORT;
}

/////////////////////////////////////////////////////////////////////////////
// COleUpdateDialog diagnostics

#ifdef _DEBUG
void COleUpdateDialog::Dump(CDumpContext& dc) const
{
	COleLinksDialog::Dump(dc);
	AFX_DUMP1(dc, "\nm_strCaption = ", m_strCaption);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// COlePasteSpecialDialog

IMPLEMENT_DYNAMIC(COlePasteSpecialDialog, COleDialog)

COlePasteSpecialDialog::COlePasteSpecialDialog(DWORD dwFlags,
	COleDataObject* pDataObject, CWnd* pParentWnd) : COleDialog(pParentWnd)
{
	memset(&m_ps, 0, sizeof(m_ps)); // initialize structure to 0/NULL

	// fill in common part
	m_ps.cbStruct = sizeof(m_ps);
	m_ps.dwFlags = dwFlags;
	if (_AfxHelpEnabled())
		m_ps.dwFlags |= PSF_SHOWHELP;
	m_ps.lpfnHook = _AfxOleHookProc;
	m_nIDHelp = AFX_IDD_PASTESPECIAL;

	// specific to this dialog
	COleDataObject dataObject;
	if (pDataObject == NULL)
	{
		dataObject.AttachClipboard();
		pDataObject = &dataObject;
	}
	ASSERT(pDataObject != NULL);
	m_ps.lpSrcDataObj = pDataObject->m_lpDataObject;
	m_ps.lpSrcDataObj->AddRef();

	m_ps.arrPasteEntries = NULL;
	m_ps.cPasteEntries = 0;
	m_ps.arrLinkTypes = m_arrLinkTypes;
	m_ps.cLinkTypes = 0;
}

COlePasteSpecialDialog::~COlePasteSpecialDialog()
{
	int i;
	// cleanup metafile handle if it was created
	if (m_ps.hMetaPict != NULL)
		OleUIMetafilePictIconFree(m_ps.hMetaPict);
	ASSERT(m_ps.lpSrcDataObj != NULL);

	for (i = 0; i < m_ps.cPasteEntries; i++)
	{
		_ffree((void FAR*)m_ps.arrPasteEntries[i].lpstrFormatName);
		_ffree((void FAR*)m_ps.arrPasteEntries[i].lpstrResultText);
	}
	_ffree(m_ps.arrPasteEntries);
	m_ps.lpSrcDataObj->Release();
}

int COlePasteSpecialDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_ps.lpfnHook != NULL);  // can still be a user hook

	m_ps.hWndOwner = PreModal();
	int iResult = MapResult(::OleUIPasteSpecial(&m_ps));
	PostModal();

	return iResult;
}

UINT COlePasteSpecialDialog::GetSelectionType() const
{
	ASSERT_VALID(this);
	ASSERT(m_ps.dwFlags & (PSF_SELECTPASTE|PSF_SELECTPASTELINK));

	UINT cf = m_ps.arrPasteEntries[m_ps.nSelectedIndex].fmtetc.cfFormat;
	Selection selType = pasteOther;
	if (m_ps.dwFlags & PSF_SELECTPASTELINK)
		selType = pasteLink;
	else if (cf == _oleData.cfEmbedSource || cf == _oleData.cfEmbeddedObject ||
		cf == _oleData.cfLinkSource)
		selType = pasteNormal;
	else if (cf == CF_METAFILEPICT || cf == CF_DIB || cf == CF_BITMAP)
		selType = pasteStatic;
	return selType;
}

/////////////////////////////////////////////////////////////////////////////
// COlePasteSpecialDialog diagnostics

#ifdef _DEBUG
void COlePasteSpecialDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_ps.cbStruct = ", m_ps.cbStruct);
	AFX_DUMP1(dc, "\nm_ps.dwFlags = ", (LPVOID)m_ps.dwFlags);
	AFX_DUMP1(dc, "\nm_ps.hWndOwner = ", (UINT)m_ps.hWndOwner);
	AFX_DUMP1(dc, "\nm_ps.lpszCaption = ", m_ps.lpszCaption);
	AFX_DUMP1(dc, "\nm_ps.lCustData = ", (LPVOID)m_ps.lCustData);
	AFX_DUMP1(dc, "\nm_ps.hInstance = ", (UINT)m_ps.hInstance);
	AFX_DUMP1(dc, "\nm_ps.lpszTemplate = ", (LPVOID)m_ps.lpszTemplate);
	AFX_DUMP1(dc, "\nm_ps.hResource = ", (UINT)m_ps.hResource);
	if (m_ps.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");

	AFX_DUMP1(dc, "\nm_ps.lpSrcDataObj = ", (LPVOID)m_ps.lpSrcDataObj);
	AFX_DUMP1(dc, "\nm_ps.cPasteEntries = ", m_ps.cPasteEntries);
	AFX_DUMP1(dc, "\nm_ps.cLinkTypes = ", m_ps.cLinkTypes);
	AFX_DUMP1(dc, "\nm_ps.nSelectedIndex = ", m_ps.nSelectedIndex);
	AFX_DUMP1(dc, "\nm_ps.fLink = ", m_ps.fLink);
}

void COlePasteSpecialDialog::AssertValid() const
{
	COleDialog::AssertValid();
	ASSERT(m_ps.cPasteEntries == 0 || m_ps.arrPasteEntries != NULL);
	ASSERT(m_ps.arrLinkTypes != NULL);
	ASSERT(m_ps.cLinkTypes <= 8);
}
#endif

////////////////////////////////////////////////////////////////////////////

OLEUIPASTEFLAG COlePasteSpecialDialog::AddLinkEntry(UINT cf)
{
	ASSERT_VALID(this);
	ASSERT(m_ps.cLinkTypes <= 8);
	for (int i = 0; i < m_ps.cLinkTypes; i++)
	{
		if (m_ps.arrLinkTypes[i] == cf)
			break;
	}
	if (i == 8)
		return (OLEUIPASTEFLAG)0;
	m_ps.arrLinkTypes[i] = cf;
	if (i == m_ps.cLinkTypes)
		m_ps.cLinkTypes++;
	return (OLEUIPASTEFLAG) (OLEUIPASTE_LINKTYPE1 << i);
}

void COlePasteSpecialDialog::AddFormat(UINT cf, DWORD tymed, UINT nFormatID,
	BOOL bEnableIcon, BOOL bLink)
{
	char szFormat[256];
	if (!_AfxLoadString(nFormatID, szFormat))
	{
		AfxThrowResourceException();
		ASSERT(FALSE);
	}

	// the format and result strings are delimited by a newline
	LPSTR pszResult = _AfxStrChr(szFormat, '\n');
	ASSERT(pszResult != NULL);  // must contain a newline
	*pszResult = '\0';
	++pszResult;    // one char past newline

	// add it to the array of acceptable formats
	m_ps.arrPasteEntries = (OLEUIPASTEENTRY FAR *)_frealloc(m_ps.arrPasteEntries,
		sizeof(OLEUIPASTEENTRY) * (m_ps.cPasteEntries +1));
	OLEUIPASTEENTRY FAR* pEntry = &m_ps.arrPasteEntries[m_ps.cPasteEntries];
	SETDEFAULTFORMATETC(pEntry->fmtetc, cf, tymed);
	pEntry->lpstrFormatName = _fstrdup(szFormat);
	pEntry->lpstrResultText = _fstrdup(pszResult);
	pEntry->dwFlags = OLEUIPASTE_PASTE;
	if (bEnableIcon)
		pEntry->dwFlags |= OLEUIPASTE_ENABLEICON;
	if (bLink)
		pEntry->dwFlags |= AddLinkEntry(cf);
	if (pEntry->dwFlags == OLEUIPASTE_PASTE)
		pEntry->dwFlags = OLEUIPASTE_PASTEONLY;
	pEntry->dwScratchSpace = NULL;
	m_ps.cPasteEntries++;
}

// if the flags parameter includes a LINKTYPE# flag, it should be obtained
// from AddLinkEntry
void COlePasteSpecialDialog::AddFormat(const FORMATETC& formatEtc,
	LPSTR lpstrFormat, LPSTR lpstrResult, DWORD dwFlags)
{
	ASSERT_VALID(this);

	m_ps.arrPasteEntries = (OLEUIPASTEENTRY FAR *)_frealloc(
		m_ps.arrPasteEntries, sizeof(OLEUIPASTEENTRY) * (m_ps.cPasteEntries +1));
	OLEUIPASTEENTRY FAR* pEntry = &m_ps.arrPasteEntries[m_ps.cPasteEntries];
	pEntry->fmtetc = formatEtc;
	pEntry->lpstrFormatName = _fstrdup(lpstrFormat);
	pEntry->lpstrResultText = _fstrdup(lpstrResult);
	pEntry->dwFlags = dwFlags;
	pEntry->dwScratchSpace = NULL;
	m_ps.cPasteEntries++;
}

void COlePasteSpecialDialog::AddStandardFormats(BOOL bEnableLink)
{
	// Note: only need to add Embedded Object because Embed Source is
	//  automatically recognized by the paste special dialog implementation.
	ASSERT(_oleData.cfEmbeddedObject != NULL);
	AddFormat(_oleData.cfEmbeddedObject, TYMED_ISTORAGE, AFX_IDS_EMBED_FORMAT,
		TRUE, FALSE);

	// add link source if requested
	if (bEnableLink)
	{
		ASSERT(_oleData.cfLinkSource != NULL);
		AddFormat(_oleData.cfLinkSource, TYMED_ISTREAM, AFX_IDS_LINKSOURCE_FORMAT,
			TRUE, TRUE);
	}

	// add formats that can be used for 'static' items
	AddFormat(CF_METAFILEPICT, TYMED_MFPICT, AFX_IDS_METAFILE_FORMAT,
		FALSE, FALSE);
	AddFormat(CF_DIB, TYMED_HGLOBAL, AFX_IDS_DIB_FORMAT, FALSE, FALSE);
	AddFormat(CF_BITMAP, TYMED_GDI, AFX_IDS_BITMAP_FORMAT, FALSE, FALSE);
}

BOOL COlePasteSpecialDialog::CreateItem(COleClientItem *pNewItem)
{
	ASSERT_VALID(this);
	ASSERT(pNewItem != NULL);

	BeginWaitCursor();

	COleDataObject dataObject;
	dataObject.Attach(m_ps.lpSrcDataObj, FALSE);

	UINT selType = GetSelectionType();
	BOOL bResult = TRUE;

	switch (selType)
	{
	case pasteLink:
		// paste link
		if (!pNewItem->CreateLinkFromData(&dataObject))
		{
			TRACE0("Warning: CreateLinkFromData in paste special dialog failed.\n");
			bResult = FALSE;
		}
		break;
	case pasteStatic:
		if (!pNewItem->CreateStaticFromData(&dataObject))
		{
			TRACE0("Warning: CreateStaticFromData in paste special dialog failed.");
			bResult = FALSE;
		}
		break;
	default:
		ASSERT(selType == pasteNormal);
		if (!pNewItem->CreateFromData(&dataObject))
		{
			TRACE0("Warning: CreateFromData in paste special dialog failed.");
			bResult = FALSE;
		}
		break;
	}

	// deal with Display As Iconic option
	if (bResult && GetDrawAspect() == DVASPECT_ICON)
	{
		// setup iconic cache (it will draw iconic by default as well)
		if (!pNewItem->SetIconicMetafile(m_ps.hMetaPict))
		{
			TRACE0("Warning: failed to set iconic aspect in "
				"COlePasteSpecialDialog::CreateItem.\n");
			bResult = FALSE;
		}
		else
		{
			// since picture was set OK, draw as iconic as well...
			pNewItem->SetDrawAspect(DVASPECT_ICON);
		}
	}

	EndWaitCursor();

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE dialog APIs
static char BASED_CODE _szAfxOleInl[] = "afxole.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxOleInl
#define _AFXODLGS_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
