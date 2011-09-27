// AccsCtl.cpp : Implementation of the CAccessPictCtrl OLE control class.

#include "stdafx.h"
#include "AccsPict.h"
#include "StrmDib.h"
#include "AccsCtl.h"
#include "AccsPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAccessPictCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAccessPictCtrl, COleControl)
	//{{AFX_MSG_MAP(CAccessPictCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CAccessPictCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CAccessPictCtrl)
	DISP_PROPERTY(CAccessPictCtrl, "StretchToFit", m_bStretch, VT_BOOL)
	DISP_PROPERTY(CAccessPictCtrl, "PreserveRatio", m_bPreserveRatio, VT_BOOL)
	DISP_PROPERTY_EX(CAccessPictCtrl, "IsAccessObject", GetIsAccessObject, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CAccessPictCtrl, "Picture", GetPicture, SetNotSupported, VT_PICTURE)
	DISP_FUNCTION(CAccessPictCtrl, "SetData", SetData, VT_BOOL, VTS_PVARIANT)
	DISP_STOCKPROP_BORDERSTYLE()
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_FONT()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAccessPictCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CAccessPictCtrl, COleControl)
	//{{AFX_EVENT_MAP(CAccessPictCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAccessPictCtrl, 2)
	PROPPAGEID(CAccessPictPropPage::guid)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CAccessPictCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAccessPictCtrl, "ACCESSPICT.AccessPictCtrl.1",
	0x1f4e7c23, 0xc38f, 0x11ce, 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CAccessPictCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DAccessPict =
		{ 0x1f4e7c21, 0xc38f, 0x11ce, { 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf } };
const IID BASED_CODE IID_DAccessPictEvents =
		{ 0x1f4e7c22, 0xc38f, 0x11ce, { 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwAccessPictOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAccessPictCtrl, IDS_ACCESSPICT, _dwAccessPictOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::CAccessPictCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAccessPictCtrl

BOOL CAccessPictCtrl::CAccessPictCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACCESSPICT,
			IDB_ACCESSPICT,
			FALSE,                      //  Not insertable
			_dwAccessPictOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::CAccessPictCtrl - Constructor

CAccessPictCtrl::CAccessPictCtrl()
{
	InitializeIIDs(&IID_DAccessPict, &IID_DAccessPictEvents);

	// Access Object Information
	m_bIsObject = FALSE;

	// Picture information
	m_bPictLoaded = FALSE;
	m_bStretch = FALSE;
	m_bPreserveRatio = FALSE;
	m_size = CSize(0,0);
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::~CAccessPictCtrl - Destructor

CAccessPictCtrl::~CAccessPictCtrl()
{
	// Make sure we clear out the data before exiting
	ResetState();
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::OnDraw - Drawing function

void CAccessPictCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CRect		rc;
	TEXTMETRIC  tm;

	// Paint the background with the AmbientBackColor.
	CBrush bkBrush(TranslateColor(AmbientBackColor()));
	pdc->FillRect(rcBounds, &bkBrush);
	
	// Draw the picture if available otherwise draw a simple text line.
	if (m_bPictLoaded)
	{
		// Resize the rendering rectangle to that of the bitmap so 
		// the aspect ratio can be preserved.
		rc = CalcRectSize(rcBounds);
		m_Pict.Render(pdc, rc, rcBounds);
	}

	// Render the caption property
	CString strText = InternalGetText();
	if (!strText.IsEmpty())
	{
		rc = rcBounds;

		// Select the stock font
		CFont* pOldFont = SelectStockFont(pdc);
		pdc->SetBkMode(TRANSPARENT);
		GetStockTextMetrics(&tm);

		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut((rc.left + rc.right) / 2, 
			(3*(rc.top + rc.bottom - tm.tmHeight) / 4), ETO_CLIPPED,
			rc, strText, strText.GetLength(), NULL);

//	CBrush* pOldBrush = pdc->SelectObject(&bkBrush);
//	pdc->SelectObject(pOldBrush);
		pdc->SelectObject(pOldFont);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::DoPropExchange - Persistence support

void CAccessPictCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::OnResetState - Reset control to default state

void CAccessPictCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
	m_Pict.CreateEmpty();		  // Create empty picture on startup
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl::AboutBox - Display an "About" box to the user

void CAccessPictCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ACCESSPICT);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictCtrl automation handlers

BOOL CAccessPictCtrl::GetIsAccessObject() 
{
	return m_bIsObject;
}

LPPICTUREDISP CAccessPictCtrl::GetPicture() 
{
	// Only return a valid Picture dispatch if the SetData method has
	// been called AND a valid picture object was received. 
	if (m_bPictLoaded)
		return m_Pict.GetPictureDispatch();
	else
		return NULL;
}

BOOL CAccessPictCtrl::SetData(VARIANT FAR* pvarData) 
{
	// This function is called by the container with a VARIANT representing
	// the OLE data from Access.  We take it and immediately try to Parse out
	// the information.  The VARIANT should contain a SAFEARRAY which in turn 
	// contains all of the data for the Access OLE Object.

	SAFEARRAY FAR*	psaObjectData = NULL;

	TRY
	{
		// First, Reset state to abandon any picture.
		ResetState();

		// If NULL, user doesn't want a picture (or wishes to clear pict)
		if (pvarData == NULL)
			return TRUE;		

		// Check that we have an actual SAFEARRAY of VT_UI1 (BYTES).  
		if (pvarData->vt != (VT_ARRAY | VT_UI1))
			AfxThrowOleException(E_INVALIDARG);	// Any exception will do
		
		// Attempt to copy the data so we can use it.
		if (FAILED(::SafeArrayCopy(pvarData->parray, &psaObjectData)))
			AfxThrowOleException(E_INVALIDARG);

		// Attempt to Read the Object	
		if (!ReadObject(psaObjectData))
			AfxThrowOleException(E_INVALIDARG);
	
		// Render the new object
		InvalidateControl();	
		
		// Cleanup
		::SafeArrayDestroy(psaObjectData);
		psaObjectData = NULL;

		// Operation has succeeded
		return TRUE;
	}
	CATCH(CMemoryException, e)
	{
		// Cleanup our copy of the object data
		if (psaObjectData != NULL)
		{
			::SafeArrayDestroy(psaObjectData);
			psaObjectData = NULL;
		}
		
		// Data couldn't be copied so reset data.  
		ResetState();
		return FALSE;
	}
	END_CATCH			
}


///////////////////////////////////////////////////////////////////////////
// Implementation

BOOL CAccessPictCtrl::ReadObject(SAFEARRAY FAR* psaObject)
{
	LPSTORAGE		lpStorage = NULL;	// An OLE 2.0 IStorage object
	LPSTREAM		lpStream = NULL;	// An OLE 2.0 Data Stream
	LARGE_INTEGER	liOffset;

	// Reading the object takes two forms.  First the header structure
	// needs to be examined to see if the SAFEARRAY contains an OLE
	// object.  It doesn't determine if the object is a picture or not.
	if ((lpStorage = ReadObjectHeader(psaObject)) == NULL)
		return FALSE;

	// Open a stream on the storage.  We want the OLE 1.0 Native
	// Data stream.
	if (FAILED(lpStorage->OpenStream(L"\1Ole10Native", NULL,
		STGM_DIRECT|STGM_READ|STGM_SHARE_EXCLUSIVE, 0, &lpStream)))
	{
		lpStorage->Release();
		lpStorage = NULL;
		return FALSE;
	}

	// Access keeps its picture data 4 bytes from the beginning of the
	// Stream.  Seek the stream 4 bytes to the beginning of the 
	// BITMAPFILEHEADER.
	liOffset.LowPart = 4;
	liOffset.HighPart = 0;
	if (FAILED(lpStream->Seek(liOffset, STREAM_SEEK_SET, NULL)))
	{
		lpStream->Release();
		lpStorage->Release();
		return FALSE;
	}

	// Attempt to read the picture data in.  If the object is not a
	// picture, it will return FALSE.
	BOOL bRet = ReadObjectData(lpStream);

	// Shutdown
	lpStream->Release();
	lpStream = NULL;

	lpStorage->Release();
	lpStorage = NULL;

	return bRet;
}

LPSTORAGE CAccessPictCtrl::ReadObjectHeader(SAFEARRAY FAR* psaObject)
{
	void HUGEP*				pArray = NULL;
	struct OLEOBJECTHEADER* pHeader = NULL;
	LPBYTE					lpData = NULL;
	BOOL					bAccessed = FALSE;	
	LPOLE1STREAM			lpStream = NULL;
	LPSTORAGE				lpStorage = NULL;

	TRY
	{
		if (psaObject == NULL)
			AfxThrowOleException(E_INVALIDARG);
	
		// Attempt to read the object header from the safearray.  Check the 
		// type signature of the header.  If it is correct, then read in 
		// the rest of the information.
	
		// Access the data in the array.
		if (FAILED(::SafeArrayAccessData(psaObject, &pArray)))
			AfxThrowOleException(E_INVALIDARG);
		bAccessed = TRUE;	
		
		pHeader = (OLEOBJECTHEADER*)pArray;
		if (pHeader->typ != 0x1C15)	// 0x1C15 is Access cookie for OLE Object
			AfxThrowOleException(E_INVALIDARG);

		// Object is an Access OLE object set the 'IsObject' flag to TRUE
		m_bIsObject = TRUE;
	
		// Call OleConvertOLESTREAMToIStorage to convert the Access object
		// (in OLE 1.0 format) to an OLE 2.0 format.
		lpData = (LPBYTE)pArray + pHeader->cbHdr;
	
		// Create the OLESTREAM
		lpStream = CreateOleStream(lpData, 
			GetStreamSize(psaObject, pHeader->cbHdr));

		// Create an OLE Storage (use a temporary file)
		if (FAILED(::StgCreateDocfile(NULL, 
			STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DIRECT|
			STGM_DELETEONRELEASE,0, &lpStorage)))
			AfxThrowOleException(E_INVALIDARG);
	
		if (FAILED(::OleConvertOLESTREAMToIStorage((LPOLESTREAM)lpStream,
			lpStorage, NULL)))
			AfxThrowOleException(E_INVALIDARG);
	
		DeleteOleStream(lpStream);
		::SafeArrayUnaccessData(psaObject);
		bAccessed = FALSE;

		return lpStorage;
	}
	CATCH(COleException, e)
	{
		if (bAccessed)
			::SafeArrayUnaccessData(psaObject);
		
		if (lpStream != NULL)
		{
			DeleteOleStream(lpStream);
			lpStream = NULL;
		}

		if (lpStorage != NULL)
		{
			lpStorage->Release();
			return NULL;
		}
		ResetState();
		
		return NULL;
	}
	END_CATCH
}

BOOL CAccessPictCtrl::ReadObjectData(LPSTREAM lpStream)
{
	BOOL bRet;

	TRY
	{
		if (lpStream == NULL)
			AfxThrowOleException(E_INVALIDARG);

		// If the seek was successful, load the bitmap from the stream
		bRet = m_StreamDib.LoadBitmap(lpStream);
		if (bRet);
		{
			// Create the new picture from the bitmap and palette
			if (m_Pict.CreateFromBitmap(m_StreamDib.GetBitmap(), 
					m_StreamDib.GetPalette()))
			{
				m_bPictLoaded = TRUE;
				m_size = m_StreamDib.GetSize();
				bRet = TRUE;
			}
			else
			{
				m_bPictLoaded = FALSE;
				ResetState();
				bRet = FALSE;
			}
		}
	}
	CATCH(COleException, e)
	{
		ResetState();
		return FALSE;
	}
	END_CATCH
	
	return bRet;		
}

void CAccessPictCtrl::ResetState()
{
	m_bIsObject = FALSE;
	m_bPictLoaded = FALSE;
}


///////////////////////////////////////////////////////////////////////////
// LPOLE1STREAM creation function

LPOLE1STREAM CAccessPictCtrl::CreateOleStream(LPBYTE lpData, DWORD dwSize)
{
	ASSERT(lpData != NULL);
	
	// Create a new OLESTREAMVTBL to handle callbacks
	LPOLESTREAMVTBL pvt = new OLESTREAMVTBL;
	
	// Initialize callbacks w/ our global functions
	pvt->Get = &Get;	
	pvt->Put = &Put;
	
	// Create an OLE1STREAM
	LPOLE1STREAM pStream = new OLE1STREAM;
	
	// Initialize it with the OLESTREAMVTBL and data
	pStream->pvt = pvt;
	pStream->dwSize = dwSize;
	pStream->lpData = lpData;
	
	return pStream;
}

void CAccessPictCtrl::DeleteOleStream(LPOLE1STREAM pStream)
{
	// Delete all the data members of the OLE1STREAM and 
	// NULL the OLE1STREAM::lpData member (it will be 
	// destroyed during ::SafeArrayDestroy().
	
	ASSERT(pStream != NULL);
	ASSERT(pStream->pvt != NULL);
	
	// Delete or NULL member data
	delete pStream->pvt;
	pStream->lpData = NULL;
	
	// Delete the stream itself
	delete pStream;
	pStream = NULL;
}


DWORD CAccessPictCtrl::GetStreamSize(SAFEARRAY FAR* psaObject, DWORD dwOffset)
{
	ASSERT(psaObject != NULL);
	LONG lBoundLower, lBoundUpper;

	::SafeArrayGetLBound(psaObject, 0, &lBoundLower);
	::SafeArrayGetUBound(psaObject, 0, &lBoundUpper);

	DWORD dwSize = ((DWORD)(lBoundUpper - lBoundLower)) - dwOffset;
	if (dwSize < 0)
		dwSize = 0;

	return dwSize;
}

///////////////////////////////////////////////////////////////////////////
// OLESTREAMVTBL Callback Functions

DWORD FAR PASCAL Get(LPOLESTREAM pstm, void FAR* pb, DWORD cb)
{
	// Check the validity of the stream and that we are not reading more
	// bytes than are in the stream.
	LPOLE1STREAM pStream = (LPOLE1STREAM)pstm;
	if ((pStream == NULL) || (pStream->lpData == NULL) || 
		(pStream->dwSize < cb))
		return 0L;
	
	// ASSUME that buffers do not overlap.
	memcpy(pb, pStream->lpData, cb);
	pStream->lpData += cb;		// Update pointer
	pStream->dwSize -= cb;
	
	return cb;
}

DWORD FAR PASCAL Put(LPOLESTREAM pstm, const void FAR* pb, DWORD cb)
{
	// Check the validity of the stream and that we are not writing
	// more data than we have space for.
	LPOLE1STREAM pStream = (LPOLE1STREAM)pstm;
	if ((pStream == NULL) || (pStream->lpData == NULL) || (pb == NULL) ||
		(pStream->dwSize < cb))
		return 0L;
	
	// ASSUME that buffers don't overlap.
	memcpy(pStream->lpData, pb, cb);
	pStream->lpData += cb;		// Update pointer
	pStream->dwSize -= cb;
	
	return cb;
}

CRect CAccessPictCtrl::CalcRectSize(const CRect& rect)
{
	CRect sizedRect;
	ASSERT(m_bPictLoaded);

	if (m_bStretch)
	{
		if (m_bPreserveRatio)
		{
			// Stretch to fit and preserve ratio
			sizedRect = rect;

			// Calculate ratio of both size & rect (x/y).
			double dfSizeRatio = (double)m_size.cx / (double)m_size.cy;
			double dfRectRatio = (double)sizedRect.Width() / 
				(double)sizedRect.Height();

			int xOff = 0;
			int yOff = 0;

			if (dfSizeRatio > dfRectRatio)
			{
				// Use the X axis for the boundary axis.  Calculate the
				// offset into the Y axis.

				yOff = (int)((m_size.cy * sizedRect.Width())/m_size.cx);
				yOff = (int)((sizedRect.Height() - yOff)/-2);
			}
			else
			{
				// Use the Y axis for the boundary axis.  Calculate the
				// offset into the X axis.

				xOff = (int)((m_size.cx * sizedRect.Height())/m_size.cy);
				xOff = (int)((sizedRect.Width() - xOff)/-2);
			}
			
			// Change the rectangle
			sizedRect.InflateRect(xOff, yOff);

		}
		else
		{
			// Stretch to fit, don't preserve ratio
			// Use rcBounds as rectangle
			sizedRect = rect;
		}
	}
	else
	{
		// Don't stretch, use a centering algorithm
		sizedRect = rect;
		int xOff = (m_size.cx - sizedRect.Width()) / 2;
		int yOff = (m_size.cy - sizedRect.Height()) / 2;
		sizedRect.InflateRect(xOff, yOff);	
	}

	return sizedRect;
}
