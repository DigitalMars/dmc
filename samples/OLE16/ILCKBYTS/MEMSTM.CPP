//**********************************************************************
// File name: memstm.cpp
//
//    Implementation file for a custom ILockBytes
//
// Functions:
//
//    See memstm.h for a list of member functions for each of the classes
//
//    DllGetClassObject - Used by OLE to get a ptr to the ClassFactory
//    CreateMyILockBytes - C wrapper used to create the custom ILockBytes
//
// Copyright (c) 1992 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include <windows.h>
#include <ole2.h>
#include <storage.h>
#include "memstm.h"

static const UINT grfMem = GMEM_SHARE | GMEM_MOVEABLE;

#include "initguid.h"

 /* B1A3AF80-D96F-1068-9917-00AA0030DCD7 */
DEFINE_GUID(CLSID_CustomMemBytes, 0xB1A3AF80, 0xD96F, 0x1068, 0x99, 0x17, 0x00, 0xAA, 0x00, 0x30, 0xDC, 0xD7);


//**********************************************************************
//
// CMemBytes::QueryInterface
//
// Purpose:
//
//      Returns a pointer to a requested interface.
//
// Parameters:
//
//      REFIID riid         - The requested interface
//
//      LPVOID FAR* ppvObj  - Place to return the interface
//
// Return Value:
//
//      NOERROR         -   The function was successful
//      E_OUTOFMEMORY   -   Couldn't instantiate custom marshalling
//                          Code.
//      E_NOINTERFACE   -   Asked for an unsupported interface
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CMarshalMemBytes::Create    MEMSTM.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMemBytes::QueryInterface(REFIID iidInterface,
									   void FAR* FAR* ppvObj)
{
	OutputDebugString("In CMemBytes::QueryInterface\r\n");

	HRESULT error = ResultFromScode (E_NOINTERFACE);
	*ppvObj = NULL;


	// Two interfaces supported: IUnknown, ILockBytes

	if (m_pData != NULL &&
			(iidInterface == IID_ILockBytes || iidInterface == IID_IUnknown)) {

		m_refs++;   // A pointer to this object is returned
		*ppvObj = this;
		error = NOERROR;
	}

	// Looking for the custom marshaller

	if (m_fCustomMarshal)   {
		if (iidInterface == IID_IMarshal) {
			*ppvObj = (LPVOID) CMarshalMemBytes::Create(this);
			if (*ppvObj != NULL)
				error = NOERROR;
			else
				error = ResultFromScode( E_OUTOFMEMORY );
		}
		else {                 // Not accessible or unsupported interface
			*ppvObj = NULL;
			error = ResultFromScode( E_NOINTERFACE );
		}
	}

	return error;
}


//**********************************************************************
//
// CMemBytes::AddRef
//
// Purpose:
//
//      Increments the reference count on this object
//
// Parameters:
//
//      None
//
// Return Value:
//
//      The current reference count on this Object
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CMemBytes::AddRef(void)
{
	OutputDebugString("In CMemBytes::AddRef\r\n");
	return ++m_refs;
}

//**********************************************************************
//
// CMemBytes::Release
//
// Purpose:
//
//      Decrements the reference count on this object
//
// Parameters:
//
//      None
//
// Return Value:
//
//      The current reference count on this object.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CMemBytes::Release(void)
{
	OutputDebugString("In CMemBytes::Release\r\n");

	if (--m_refs != 0) // Still used by others
		return m_refs;

	// Releases the structure that represents this ILockBytes
	ReleaseMemStm(&m_hMem);

	delete this; // Free storage
	return 0;
}


//**********************************************************************
//
// CMemBytes::ReadAt
//
// Purpose:
//
//      Reads a specified number of bytes from a specific location in
//      the byte array.
//
// Parameters:
//
//      ULARGE_INTEGER ulOffset - Location in the byte array in which
//                                to start reading.
//
//      void HUGEP* pb          - Pointer to the buffer in which the bytes
//                                should be copied.
//
//      ULONG cb                - The number of bytes to try to read from
//                                the byte array.
//
//      ULONG FAR* pcbRead      - Out ptr to return the actual number of
//                                bytes read.
//
// Return Value:
//
//      NOERROR         - No error
//      STG_E_READFAULT - Couldn't read the buffer.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GlobalLock                  Windows API
//      GlobalUnlock                Windows API
//      _fmemcpy                    C Run-time
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMemBytes::ReadAt(ULARGE_INTEGER ulOffset, void HUGEP* pb,
							   ULONG cb, ULONG FAR* pcbRead)
{
	OutputDebugString("In CMemBytes::ReadAt\r\n");

	HRESULT error = NOERROR;
	ULONG cbRead = cb;

	// pcbRead is NULL if the caller is not interested in the value.
	// If the caller is interested, Initialize the out ptr to zero.
	if (pcbRead)
		*pcbRead = 0L;

	// if we are going to try to read passed the end of the global
	// memory.
	if (cbRead + ulOffset.LowPart > m_pData->cb) {

		// if the offset is past the end of the global memory, then
		// set the number of bytes read to zero.  Otherwise, the
		// number of bytes to be read is the total number of bytes
		// in the global memory minus the offset in which to start
		// reading.
		if (ulOffset.LowPart > m_pData->cb)
			cbRead = 0;
		else
			cbRead = m_pData->cb - ulOffset.LowPart;
	}


	// if the number of bytes to be read is > 0.
	if (cbRead > 0)
	{
		// lock the buffer
		char HUGEP* pGlobal = (char HUGEP*)GlobalLock (m_pData->hGlobal);

		// if it couldn't be locked, return an error.
		if (NULL==pGlobal)
		{
			return ResultFromScode (STG_E_READFAULT);
		}

		// copy the appropriate section of the global memory into the passed
		// buffer.
		_fmemcpy (pb, pGlobal + ulOffset.LowPart, (size_t)cbRead);

		GlobalUnlock (m_pData->hGlobal);
	}

	// fill the out ptr if needed.
	if (pcbRead != NULL)
		*pcbRead = cbRead;

	return error;
}


//**********************************************************************
//
// CMemBytes::WriteAt
//
// Purpose:
//
//      Writes a specified number of bytes to a specific location in
//      the byte array.
//
// Parameters:
//
//      ULARGE_INTEGER ulOffset - Offset into the byte array in which to
//                                start writing the data.
//
//      void const HUGEP* pb    - Pointer to a buffer containing the data
//                                that needs to be written.
//
//      ULONG cb                - The number of bytes to attempt to write to
//                                the byte array.
//
//      ULONG FAR* pcbWritten   - An out ptr in which to return the actual
//                                number of bytes written.
//
// Return Value:
//
//      NOERROR             - No error occured
//      STG_E_WRITEFAULT    - Error locking the memory
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GlobalLock                  Windows API
//      GlobalUnlock                Windows API
//      ULISet32                    OLE API
//      CMemBytes::SetSize          MEMSTM.CPP
//      _fmemcpy                    C Run-time
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMemBytes::WriteAt(ULARGE_INTEGER ulOffset, void const HUGEP* pb,
								ULONG cb, ULONG FAR* pcbWritten)
{
	OutputDebugString("In CMemBytes::WriteAt\r\n");

	HRESULT error = NOERROR;
	ULONG cbWritten = cb;
	char HUGEP* pGlobal;

	// pcbWritten will be NULL if the caller is not interested in the
	// number of bytes written.  If pcbWritten exists, initialize it
	// to zero.
	if (pcbWritten) {
		*pcbWritten = 0;
	}

	// if writing past the end of the global buffer, then expand the
	// buffer so that the data can be written.
	if (cbWritten + ulOffset.LowPart > m_pData->cb) {
		ULARGE_INTEGER ularge_integer;
		ULISet32( ularge_integer, ulOffset.LowPart + cbWritten);
		error = SetSize( ularge_integer );

		// bomb out if there is an error expanding the global buffer.
		if (error != NOERROR)
			goto Exit;
	}

	// get a ptr to the global buffer
	pGlobal = (char HUGEP*)GlobalLock (m_pData->hGlobal);

	if (NULL==pGlobal)
	{
		return ResultFromScode (STG_E_WRITEFAULT);
	}

	// copy the appropriate data from the passed buffer into the
	// global buffer.
	_fmemcpy (pGlobal + ulOffset.LowPart, pb, (size_t)cbWritten);
	GlobalUnlock (m_pData->hGlobal);

	// if the caller was interested in the actual bytes written,
	// fill in the out ptr.
	if (pcbWritten != NULL)
		*pcbWritten = cbWritten;

Exit:
	return error;
}

//**********************************************************************
//
// CMemBytes::Flush
//
// Purpose:
//
//      Flushes out any buffers.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Since this implementation is not buffering read and writes,
//      implementation of this method is trivial.
//
//********************************************************************

STDMETHODIMP CMemBytes::Flush(void)
{
	OutputDebugString("In CMemBytes::Flush\r\n");
	return NOERROR;
}


//**********************************************************************
//
// CMemBytes::SetSize
//
// Purpose:
//
//      Changes the size of the byte array.
//
// Parameters:
//
//      ULARGE_INTEGER cb   - The new size of the array.
//
// Return Value:
//
//      NOERROR         - Worked ok.
//      E_OUTOFMEMORY   - Couldn't reallocate the global buffer.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GlobalReAlloc               Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMemBytes::SetSize(ULARGE_INTEGER cb)
{
	OutputDebugString("In CMemBytes::SetSize\r\n");

	HANDLE hMemNew;

	// if the current size of the byte array is the same as the
	// new size, then return success.
	if (m_pData->cb == cb.LowPart)
		return NOERROR;

	// re allocate the byte array.
	hMemNew = GlobalReAlloc(m_pData->hGlobal,
							max (cb.LowPart, 1),
							grfMem);

	// if the allocation failed, then return error.
	if (hMemNew == NULL)
	   return ResultFromScode( E_OUTOFMEMORY );

	// adjust the tracking structure to reflect the new values.
	m_pData->hGlobal = hMemNew;
	m_pData->cb = cb.LowPart;

	return NOERROR;
}


//**********************************************************************
//
// CMemBytes::LockRegion
//
// Purpose:
//
//      Instructs a section of the byte array to be locked.
//
// Parameters:
//
//      ULARGE_INTEGER libOffset    - Beginning of the region to lock
//
//      ULARGE_INTEGER cb           - Number of bytes to lock
//
//      DWORD dwLockType            - Type of lock.
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      This implementation doesn't support region locking.
//
//********************************************************************

STDMETHODIMP CMemBytes::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	OutputDebugString("In CMemBytes::LockRegion\r\n");

	return NOERROR;
}

//**********************************************************************
//
// CMemBytes::UnlockRegion
//
// Purpose:
//
//      Unlocks a region of the byte array.
//
// Parameters:
//
//      ULARGE_INTEGER libOffset    - Offset into the array to unlock.
//
//      ULARGE_INTEGER cb           - Number of bytes to unlock
//
//      DWORD dwLockType            - Type of Access
//
// Return Value:
//
//
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      This implementation doesn't support region locking.
//
//********************************************************************

STDMETHODIMP CMemBytes::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb,
									 DWORD dwLockType)
{
	OutputDebugString("In CMemBytes::UnlockRegion\r\n");
	return NOERROR;
}


//**********************************************************************
//
// CMemBytes::Stat
//
// Purpose:
//
//      Returns information about the byte array.
//
// Parameters:
//
//      STATSTG FAR *pstatstg   - Out ptr to fill in the info.
//
//      DWORD statflag          - Flag determining if the name is to
//                                be returned.
//
// Return Value:
//
//
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMemBytes::Stat(STATSTG FAR *pstatstg, DWORD statflag)
{
	OutputDebugString("In CMemBytes::Stat\r\n");

	pstatstg->pwcsName = NULL;              //  No name descibing this ILB
	pstatstg->type = STGTY_LOCKBYTES;       //  Type of stat
	pstatstg->cbSize.HighPart = 0;          //  Count of bytes
	pstatstg->cbSize.LowPart = m_pData->cb; //    "       "
	pstatstg->mtime.dwLowDateTime = 0;      //  Modifification Date and Time
	pstatstg->mtime.dwHighDateTime = 0;     //    "       "     "    "    "
	pstatstg->ctime.dwLowDateTime = 0;      //    "       "     "    "    "
	pstatstg->ctime.dwHighDateTime = 0;     //    "       "     "    "    "
	pstatstg->atime.dwLowDateTime = 0;      //    "       "     "    "    "
	pstatstg->atime.dwHighDateTime = 0;     //    "       "     "    "    "
	pstatstg->grfMode = 0;                  //  Mode
	pstatstg->grfLocksSupported = 0;        //  Locking not supported
	pstatstg->clsid = CLSID_NULL;           //  Only for storages
	pstatstg->grfStateBits = 0;             //   "    "    "   "
	pstatstg->reserved = 0;                 //  reserved.

	return NOERROR;
}

//**********************************************************************
//
// CreateMyILockBytes
//
// Purpose:
//
//      Creates the custom ILockBytes.
//
// Parameters:
//
//      HANDLE hMem                 - Handle to the initializer
//
//      LPLOCKBYTES FAR* lplpBytes  - Out ptr to return the ILockBytes
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CMemBytes::Create           MEMSTM.CPP
//
// Comments:
//
//
//********************************************************************

STDAPI CreateMyILockBytes (BOOL fCustomMarshal, LPLOCKBYTES FAR* lplpBytes)
{
	OutputDebugString("In CreateMyILockBytes \r\n");

	HGLOBAL hMem = GlobalAlloc( GHND | GMEM_SHARE, sizeof(MEMSTM) );

	MEMSTM FAR *pData = (MEMSTM FAR*)GlobalLock(hMem);

	pData->hGlobal = NULL;

	GlobalUnlock(hMem);

	*lplpBytes =  CMemBytes::Create(hMem, fCustomMarshal);

	return NOERROR;
}

//**********************************************************************
//
// CMemBytes::Create
//
// Purpose:
//
//      Creates the actual ILockBytes object.
//
// Parameters:
//
//      HANDLE hMem     - Handle to a MEMSTM structure.
//
// Return Value:
//
//      Pointer to the ILockBytes implementation.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GlobalLock                  Windows API
//      GlobalAlloc                 Windows API
//      CMemBytes::CMemBytes        MEMSTM.CPP
//
// Comments:
//
//
//********************************************************************

CMemBytes FAR* CMemBytes::Create(HANDLE hMem, BOOL fCustomMarshal)
{
	OutputDebugString("In CMemBytes::Create\r\n");
	CMemBytes FAR* pCMemBytes;
	struct MEMSTM FAR* pData;

	// lock the buffer
	pData = (MEMSTM FAR*)GlobalLock(hMem);

	// Allocate the byte array
	if (!pData->hGlobal)
		pData->hGlobal = GlobalAlloc(grfMem,0);

	// If the allocation fails return error.
	if (pData == NULL)
		return NULL;

	// instantiate object.
	pCMemBytes = new CMemBytes;

	if (pCMemBytes == NULL)
		return NULL;

	// Initialize CMemBytes
	//
	pCMemBytes->m_hMem = hMem;
	(pCMemBytes->m_pData = pData)->cRef++;  // AddRefMemStm
	pCMemBytes->m_refs = 1;
	pCMemBytes->m_fCustomMarshal = fCustomMarshal;

	return pCMemBytes;
}




// CMemBytes object's IMarshal implementation
//

//**********************************************************************
//
// CMarshalMemBytes::QueryInterface
//
// Purpose:
//
//      Returns a pointer to a requested interface.
//
// Parameters:
//
//      REFIID riid         - The requested interface
//
//      LPVOID FAR* ppvObj  - Place to return the interface
//
// Return Value:
//
//      NOERROR         -   The function was successful
//      E_NOINTERFACE   -   Asked for an unsupported interface
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************
STDMETHODIMP CMarshalMemBytes::QueryInterface(REFIID iidInterface,
													void FAR* FAR* ppvObj)
{
	OutputDebugString("In CMarshalMemBytes::QueryInterface \r\n");

	HRESULT error;

	*ppvObj = NULL;

	// Two interfaces supported: IUnknown, IMarshal

	if (iidInterface == IID_IMarshal || iidInterface == IID_IUnknown) {
		m_refs++;           // A pointer to this object is returned
		*ppvObj = this;
		error = NOERROR;
	}
	else {                  // Not accessible or unsupported interface
		*ppvObj = NULL;
		error = ResultFromScode( E_NOINTERFACE );
	}

	return error;
}


//**********************************************************************
//
// CMarshalMemBytes::AddRef
//
// Purpose:
//
//      Increments the reference count on this object
//
// Parameters:
//
//      None
//
// Return Value:
//
//      The current reference count on this Object
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CMarshalMemBytes::AddRef(void)
{
	OutputDebugString("In CMarshalMemBytes::AddRef \r\n");
	return ++m_refs;
}

//**********************************************************************
//
// CMarshalMemBytes::Release
//
// Purpose:
//
//      Decrements the reference count on this object
//
// Parameters:
//
//      None
//
// Return Value:
//
//      The current reference count on this object.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CMarshalMemBytes::Release(void)
{
	OutputDebugString("In CMarshalMemBytes::Release \r\n");
	if (--m_refs != 0) // Still used by others
		return m_refs;

	if (m_pMemBytes != NULL)
		m_pMemBytes->Release();

	delete this; // Free storage
	return 0;
}


//**********************************************************************
//
// CMarshalMemBytes::GetUnmarshalClass
//
// Purpose:
//
//      Gets the CLSID of the object that will do the Unmarshalling
//
// Parameters:
//
//      REFIID riid             - The IID of the interface to be marshalled
//
//      LPVOID pv               - Pointer to the interface to be marshalled
//
//      DWORD dwDestContext     - Destination Context relative to the
//                                current context.
//
//      LPVOID pvDestContext    - Used for some dwDestContext
//
//      DWORD mshlflags         - Type of marshalling.
//
//      CLSID FAR* pCid         - Out ptr in which to return the CLSID
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::GetUnmarshalClass(REFIID riid, LPVOID pv,
												 DWORD dwDestContext, LPVOID pvDestContext,
												 DWORD mshlflags, CLSID FAR* pCid)
{
	OutputDebugString("In CMarshalMemBytes::GetUnmarshalClass \r\n");

	// the marshalling context isn't understood.  Give it to the standard
	// marshaller.

	if ( dwDestContext != MSHCTX_LOCAL )
		{
		HRESULT hErr;
		LPMARSHAL lpMarshal;

		if ( (hErr = CoGetStandardMarshal ( riid,
											(LPUNKNOWN)m_pMemBytes,
											dwDestContext,
											pvDestContext,
											mshlflags,
											&lpMarshal) ) == NOERROR)
			{
			HRESULT hErr = lpMarshal->GetUnmarshalClass(riid, pv, dwDestContext, pvDestContext, mshlflags, pCid);
			lpMarshal->Release();
			}

		return hErr;
		}

	// set the out ptr to be our clsid

	*pCid = m_clsid;
	return NOERROR;
}


//**********************************************************************
//
// CMarshalMemBytes::GetMarshalSizeMax
//
// Purpose:
//
//      Returns an upper bound on the amount of data that would be written
//      into the marshalling stream.
//
// Parameters:
//
//      REFIID riid             - IID of the interface to be marshalled
//
//      LPVOID pv               - Pointer to the interface to be marshalled
//
//      DWORD dwDestContext     - The destination context realtive to the
//                                current context.
//
//      LPVOID pvDestContext    - Used for some dwDestContext
//
//      DWORD mshlflags         - Type of marshalling
//
//      DWORD FAR* pSize        - Out ptr to return the max size.
//
// Return Value:
//
//
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::GetMarshalSizeMax(REFIID riid, LPVOID pv,
												 DWORD dwDestContext, LPVOID pvDestContext,
												 DWORD mshlflags, DWORD FAR* pSize)
{
	OutputDebugString("In CMarshalMemBytes::GetMarshalSizeMax \r\n");

	// the marshalling context isn't understood.  Give it to the standard
	// marshaller.
	if ( dwDestContext != MSHCTX_LOCAL )
		{
		HRESULT hErr;
		LPMARSHAL lpMarshal;

		if ( (hErr = CoGetStandardMarshal ( riid,
											(LPUNKNOWN)m_pMemBytes,
											dwDestContext,
											pvDestContext,
											mshlflags,
											&lpMarshal) ) == NOERROR)
			{
			HRESULT hErr = lpMarshal->GetMarshalSizeMax(riid, pv, dwDestContext, pvDestContext, mshlflags, pSize);
			lpMarshal->Release();
			}

		return hErr;
		}


   // this implementation simply stuffs the memory handle into the
   // marshalling stream.
   *pSize = sizeof(m_pMemBytes->m_hMem);

	return NOERROR;
}


//**********************************************************************
//
// CMarshalMemBytes::MarshalInterface
//
// Purpose:
//
//      Marshals the interface into the stream.
//
// Parameters:
//
//      IStream FAR* pStm       - The stream onto which the object should
//                                be marshalled.
//
//      REFIID riid             - The interface of this object in which to
//                                be marshalled.
//
//      void FAR* pv            - The actual ptr to be marshalled.
//
//      DWORD dwDestContext     - The destination context realtive to the
//                                current context.
//
//      LPVOID pvDestContext    - Used for some dwDestContext
//
//      DWORD mshlflags         - Type of marshalling
//
// Return Value:
//
//      E_FAIL          - We don't have a ptr to the ILockBytes
//      E_INVALIDARG    - Bad parameter was passed.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::MarshalInterface(IStream FAR* pStm,
												REFIID riid, void FAR* pv,
												DWORD dwDestContext,
												LPVOID pvDestContext, DWORD mshlflags)
{
	OutputDebugString("In CMarshalMemBytes::MarshalInterface \r\n");

	// the marshalling context isn't understood.  Give it to the standard
	// marshaller.
	if ( dwDestContext != MSHCTX_LOCAL )
		{
		HRESULT hErr;
		LPMARSHAL lpMarshal;

		if ( (hErr = CoGetStandardMarshal ( riid,
											(LPUNKNOWN)m_pMemBytes,
											dwDestContext,
											pvDestContext,
											mshlflags,
											&lpMarshal) ) == NOERROR)
			{
			HRESULT hErr = lpMarshal->MarshalInterface(pStm, riid, pv, dwDestContext, pvDestContext, mshlflags);
			lpMarshal->Release();
			}

		return hErr;
		}


	// fail if we don't have a ptr to the ILockBytes Already.
	if (m_pMemBytes == NULL)
		return ResultFromScode( E_FAIL );

	// if the IID is not valid or the passed interface ptr is not the same
	// as our interface ptr, then fail.
	if ((riid != IID_ILockBytes && riid != IID_IUnknown) || pv != m_pMemBytes)
		return ResultFromScode( E_INVALIDARG );

	// increase ref count on hglobal (ReleaseMarshalData has -- to match)
	HRESULT error;
	// write the memory handle to the stream.
	if ((error = pStm->Write(&m_pMemBytes->m_hMem, sizeof(m_pMemBytes->m_hMem),
				NULL)) == NOERROR)
		m_pMemBytes->m_pData->cRef++;   // AddRefMemStm

	return error;
}


//**********************************************************************
//
// CMarshalMemBytes::UnmarshalInterface
//
// Purpose:
//
//      Takes the memory handle from the stream and instantiates an
//      ILockBytes.
//
// Parameters:
//
//      IStream FAR* pStm       - The stream from which the interface
//                                should be unmarshalled.
//
//      REFIID riid             - IID of the interface to unmarshal.
//
//      void FAR* FAR* ppv      - Out ptr to fill with the ptr to the
//                                interface.
//
// Return Value:
//
//      E_INVALIDARG    - Wrong IID was passed
//      E_FAIL          - Returned if the global handle is different
//                        than the stub already has.
//      E_OUTOFMEMORY   - Couldn't create our ILockBytes
//      NOERROR         - function successful
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      IStream::Read               STORAGE.DLL
//      CMemBytes::Create           MEMSTM.CPP
//
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::UnmarshalInterface(IStream FAR* pStm,
												  REFIID riid, void FAR* FAR* ppv)
{
	OutputDebugString("In CMarshalMemBytes::UnmarshalInterface \r\n");
	HRESULT error;
	HANDLE hMem;

	// NULL the Out ptr in case of failure.
	*ppv = NULL;

	// If not an interface that we support, then return error.
	if (riid != IID_ILockBytes && riid != IID_IUnknown)
		return ResultFromScode( E_INVALIDARG );

	// read the memory handle from the stream.
	error = pStm->Read((void HUGEP*)&hMem,sizeof(hMem),NULL);

	// if the read failed, then return the error.
	if (error != NOERROR)
		return error;

	// if there is already an ILockBytes instantiated, then make sure that
	// the handle to the global memory from the stream is the same as the
	// handle that we already have.
	// if there is not an ILockBytes instantiated, then instantiate one.
	if (m_pMemBytes != NULL) {
		if (hMem != m_pMemBytes->m_hMem)
			return ResultFromScode( E_FAIL );
	}
	else {
		m_pMemBytes = CMemBytes::Create(hMem); // Create the lockbytes

		if (m_pMemBytes == NULL)
			return ResultFromScode( E_OUTOFMEMORY );
	}

	// AddRef the ILockBytes
	m_pMemBytes->AddRef();

	// return the ptr.
	*ppv = (LPVOID) m_pMemBytes;

	return NOERROR;
}


//**********************************************************************
//
// CMarshalMemBytes::ReleaseMarshalData
//
// Purpose:
//
//      Called to release the marshalled data packet.
//
// Parameters:
//
//      IStream FAR* pStm   - Ptr to a stream that contains the data
//                            packe which is to be destroyed.
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      IStream::Read               STORAGE.DLL
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::ReleaseMarshalData(IStream FAR* pStm)
{
	OutputDebugString("In CMarshalMemBytes::ReleaseMarshalData \r\n");
	// reduce ref count on hglobal (matches that done in MarshalInterface)
	HRESULT error;
	HANDLE hMem;

	// read the memory handle from the stream.
	error = pStm->Read((void HUGEP*)&hMem,sizeof(hMem),NULL);

	// if the memory handle was successfully read, then call the
	// release function.
	if (error == NOERROR)
		ReleaseMemStm(&hMem);

	return error;
}


//**********************************************************************
//
// CMarshalMemBytes::DisconnectObject
//
// Purpose:
//
//      Used to notify us to release any outstanding ptrs to the object.
//
// Parameters:
//
//      DWORD dwReserved    - Reserved
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CMarshalMemBytes::DisconnectObject(DWORD dwReserved)
{
	OutputDebugString("In CMarshalMemBytes::DisconnectObject \r\n");
	return NOERROR;
}


//**********************************************************************
//
// CMarshalMemBytes::Create
//
// Purpose:
//
//      Static member used to create the CMarshalMemBytes object.
//
// Parameters:
//
//      CMemBytes FAR* pMemBytes    - ptr to the ILockBytes to be marshalled
//
// Return Value:
//
//      A ptr to the marshal object.
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      CMarshalMemBytes::CMarshalMembytes  MEMSTM.CPP
//
// Comments:
//
//
//********************************************************************

CMarshalMemBytes FAR* CMarshalMemBytes::Create(CMemBytes FAR* pMemBytes)
{
	OutputDebugString("In CMarshalMemBytes::Create \r\n");
	CMarshalMemBytes FAR* pMMB;

	// instantiate the CMarshalMemBytes object.
	pMMB = new CMarshalMemBytes;

	// if the instantiation failed, return error.
	if (pMMB == NULL)
		return NULL;

	// if passed a ptr to an ILockBytes, then save the ptr and AddRef it.
	if (pMemBytes != NULL) {
		pMMB->m_pMemBytes = pMemBytes;
		pMMB->m_pMemBytes->AddRef();
	}

	// set the CLSID
	pMMB->m_clsid = CLSID_CustomMemBytes;

	// set the refcount.
	pMMB->m_refs = 1;

	return pMMB;
}

//**********************************************************************
//
// ReleaseMemStm
//
// Purpose:
//
//      Removes ref count from global memory, and releases when it goes
//      to zero.
//
// Parameters:
//
//      LPHANDLE phMem      - memory handle to be released.
//
//      BOOL fInternalOnly  - Specifies to free the memory.
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      GlobalLock                          Windows API
//      GlobalUnlock                        Windows API
//      GlobalFree                          Windows API
//
// Comments:
//
//
//********************************************************************

STDAPI_(void) ReleaseMemStm (LPHANDLE phMem, BOOL fInternalOnly)
{
	struct MEMSTM FAR* pData;

	OutputDebugString("In ReleaseMemStm \r\n");

	// lock the memory
	pData = (MEMSTM FAR*)GlobalLock(*phMem);

	// check for NULL pointer in case handle got freed already
	// decrement ref count and free if no refs left
	if (pData != NULL && --pData->cRef == 0)
	{
		OutputDebugString("*** Releasing Memory *** \r\n");
		if (pData->fDeleteOnRelease)
			GlobalFree (pData->hGlobal);

		if (!fInternalOnly)
			{
			GlobalUnlock(*phMem);
			GlobalFree(*phMem);
			}
	}

	*phMem = NULL;
}

//**********************************************************************
//
// DllGetClassObject
//
// Purpose:
//
//      Allow COMPOBJ.DLL to get a ptr to the ClassFactory for this object
//
// Parameters:
//
//      REFCLSID clsid      - CLSID of the object
//
//      REFIID iid          - IID of interface
//
//      LPVOID FAR *ppv     - Out ptr to return ptr to iid
//
// Return Value:
//
//      E_FAIL          - Wrong CLSID
//      NOERROR         - Worked OK
//      E_NOINTERFACE   - IID not supported
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//
// Comments:
//
//
//********************************************************************

STDAPI DllGetClassObject ( REFCLSID clsid, REFIID iid, LPVOID FAR *ppv )
{
	OutputDebugString("In DllGetClassObject\r\n");

	static CClassFactory FAR* pCF = NULL;

	// if no ClassFactory has been instantiated, create one.
	if (!pCF)
		pCF = new CClassFactory;

	// NULL the out ptr
	*ppv = NULL;

	// Not the right CLSID
	if (clsid != CLSID_CustomMemBytes)
		return ResultFromScode(E_FAIL);

	// return the appropriate interface
	if ( (iid == IID_IClassFactory) ||
		 (iid == IID_IUnknown) )
		{
		*ppv = (LPVOID) pCF;
		pCF->AddRef();
		return NOERROR;
		}
	else
		return ResultFromScode (E_NOINTERFACE);
}

//**********************************************************************
//
// CClassFactory::QueryInterface
//
// Purpose:
//
//      Returns ptrs to the supported interfaces.
//
// Parameters:
//
//      REFIID riid         - Requested interface
//
//      LPVOID FAR* ppvObj  - Out ptr to return the interface
//
// Return Value:
//
//      NOERROR
//      E_NOINTERFACE   - Interface not supported
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	OutputDebugString("In ILBDLL CClassFactory::QueryInterface\r\n");
	*ppvObj = NULL;

	// return the supported interfaces
	if ( (riid == IID_IUnknown) ||
		 (riid == IID_IClassFactory) )
		{
		*ppvObj = this;
		AddRef();
		return NOERROR;
		}
	else
		return ResultFromScode(E_NOINTERFACE);
}

//**********************************************************************
//
// CClassFactory::AddRef
//
// Purpose:
//
//      Increments the reference count on CClassFactory
//
// Parameters:
//
//      None
//
// Return Value:
//
//      New ref. count
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CClassFactory::AddRef ()
{
	OutputDebugString("In ILBDLL CClassFactory::AddRef\r\n");
	return ++m_refs;
}

//**********************************************************************
//
// CClassFactory::Release
//
// Purpose:
//
//      Decrements the reference count on CClassFactory
//
// Parameters:
//
//      None
//
// Return Value:
//
//      The new reference count
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CClassFactory::Release ()
{
   OutputDebugString("In ILBDLL CClassFactory::Release\r\n");

   // if last reference, then delete the object.
   if (--m_refs == 0)
	   delete this;

   return m_refs;
}

//**********************************************************************
//
// CClassFactory::CreateInstance
//
// Purpose:
//
//      Creates an instance of the custom marshaller.  Needed to
//      Unmarshal the data in the other process.
//
// Parameters:
//
//      LPUNKNOWN pUnkOuter     - ptr to Outer IUnknown.  Used for
//                                aggragation.
//
//      REFIID riid             - requested IID.
//
//      LPVOID FAR* ppvObject   - Out ptr to return the object.
//
// Return Value:
//
//      NOERROR
//      E_NOINTERFACE   - Interface not supported.
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      CMarshalMemBytes::Create            MEMSTM.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::CreateInstance  ( LPUNKNOWN pUnkOuter,
											  REFIID riid,
											  LPVOID FAR* ppvObject)
{
	OutputDebugString("In ILBDLL CClassFactory::CreateInstance\r\n");

	// NULL the out ptr
	*ppvObject = NULL;

	// if asking for one of the supported interfaces, then create
	// the marshal object, otherwise fail.
	if ( (riid == IID_IMarshal) ||
		 (riid == IID_IUnknown) )
		{
		*ppvObject = (LPVOID) CMarshalMemBytes::Create(NULL);
		return NOERROR;
		}
	else
		return ResultFromScode (E_NOINTERFACE);
}

//**********************************************************************
//
// CClassFactory::LockServer
//
// Purpose:
//
//      Holds the server in memory.  Not strong enough to over-ride
//      a user.
//
// Parameters:
//
//      BOOL fLock  - TRUE if locking, FALSE if unlocking.
//
// Return Value:
//
//      NOERROR
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::LockServer (BOOL fLock)
{
	OutputDebugString("In ILBDLL CClassFactory::CreateInstance\r\n");
	return NOERROR;
}
