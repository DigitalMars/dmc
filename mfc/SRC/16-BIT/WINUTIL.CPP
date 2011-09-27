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
#include <malloc.h>

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Sub-Segment Allocation

#pragma optimize("qgel", off) // assembler cannot be globally optimized
//
// Helper function for using LocalAlloc on arbitrary segments
// returns either a long pointer to the handle (LHND flags used)
// or a long pointer to the buffer allocated (LPTR flags used)
//
LPVOID PASCAL _AfxLocalAlloc(UINT sgmnt, UINT wFlags, UINT wBytes)
{
	HANDLE  hMem;

	_asm {
		push    ds          ; /* Save DS */
		mov     ds, sgmnt   ; /* get the segment to allocate within */
	}

	// Cannot use any globals here

	hMem = LocalAlloc(wFlags, wBytes);

	_asm {
		pop     ds          ; /* Restore DS */
	}

	if (hMem != NULL)
		return (LPVOID)MAKELONG(hMem, sgmnt);
	else
		return NULL;
}

//
// Helper function for using LocalFree to free a memory block
// allocated with _AfxLocalAlloc.
//
void PASCAL _AfxLocalFree(LPVOID lhBlock)
{
	_asm {
		push    ds              ; /* Save DS */
		mov     ds, WORD PTR lhBlock[2] ; /* Get the segment from the long pointer */
	}

	::LocalFree((HLOCAL)_AFX_FP_OFF(lhBlock));

	_asm {
		pop     ds              ; /* Restore DS */
	}
}

//
// Helper function for using LocalLock to Lock a memory block
// allocated with _AfxLocalAlloc.
//
LPSTR PASCAL _AfxLocalLock(LPVOID lhBlock)
{
	_asm {
		push    ds              ; /* Save DS */
		mov     ds, WORD PTR lhBlock[2] ; /* Get the segment from the long pointer */
	}

	PSTR p = (PSTR)::LocalLock((HLOCAL)_AFX_FP_OFF(lhBlock));

	_asm {
		pop     ds              ; /* Restore DS */
	}
	return (LPSTR)MAKELONG(p, HIWORD(lhBlock));
}

//
// Helper function for using LocalUnlock to Unlock a memory block
// allocated with _AfxLocalAlloc.
//
BOOL PASCAL _AfxLocalUnlock(LPVOID lhBlock)
{
	_asm {
		push    ds              ; /* Save DS */
		mov     ds, WORD PTR lhBlock[2] ; /* Get the segment from the long pointer */
	}

	BOOL b = ::LocalUnlock((HLOCAL)_AFX_FP_OFF(lhBlock));

	_asm {
		pop     ds              ; /* Restore DS */
	}
	return b;
}
#pragma optimize("", on)    // return to default optimizations

/////////////////////////////////////////////////////////////////////////////
// Other helpers

// turn a file, relative path or other into an absolute path
BOOL PASCAL _AfxFullPath(LPSTR lpszPathOut, LPCSTR lpszFileIn)
	// lpszPathOut = buffer of _MAX_PATH
	// lpszFileIn = file, relative path or absolute path
	// (both in ANSI character set)
{
	ASSERT(AfxIsValidAddress(lpszPathOut, _MAX_PATH));

	OFSTRUCT of;
	if (OpenFile(lpszFileIn, &of, OF_PARSE) != HFILE_ERROR)
	{
		// of.szPathName is in the OEM character set
		OemToAnsi(of.szPathName, lpszPathOut);
		AnsiUpper(lpszPathOut); // paths in upper case just to be sure
		return TRUE;
	}
	else
	{
		TRACE1("Warning: could not parse the path %Fs\n", lpszFileIn);
		lstrcpy(lpszPathOut, lpszFileIn);  // take it literally
		AnsiUpper(lpszPathOut); // paths in upper case just to be sure
		return FALSE;
	}
}

// like strncpy/fstrncpy but always zero terminate and don't zero fill
void PASCAL _AfxStrCpy(LPSTR lpszDest, LPCSTR lpszSrc, size_t nSizeDest)
{
	ASSERT(AfxIsValidAddress(lpszDest, nSizeDest));
	size_t nLen = lstrlen(lpszSrc);
	if (nLen > nSizeDest-1)
	{
		nLen = nSizeDest-1;
		TRACE2("Warning: truncating string '%Fs' to %d characters\n",
				lpszSrc, nLen);
	}
	_fmemcpy(lpszDest, lpszSrc, nLen);
	lpszDest[nLen] = '\0';
}

BOOL PASCAL _AfxIsComboBoxControl(HWND hWnd, UINT nStyle)
{
	if (hWnd == NULL)
		return FALSE;
	// do cheap style compare first
	if ((UINT)(::GetWindowLong(hWnd, GWL_STYLE) & 0x0F) != nStyle)
		return FALSE;

	// do expensive classname compare next
	static char BASED_CODE szComboBox[] = "combobox";
	char szCompare[sizeof(szComboBox) + 1];
	::GetClassName(hWnd, szCompare, sizeof(szCompare));
	return (lstrcmpi(szCompare, szComboBox) == 0);
}


void PASCAL _AfxSmartSetWindowText(HWND hWndCtrl, LPCSTR lpszNew)
{
	int nNewLen = lstrlen(lpszNew);
	char szOld[64];
	// fast check to see if text really changes (reduces flash in controls)
	if (nNewLen > sizeof(szOld) ||
		::GetWindowText(hWndCtrl, szOld, sizeof(szOld)) != nNewLen ||
		lstrcmp(szOld, lpszNew) != 0)
	{
		// change it
		::SetWindowText(hWndCtrl, lpszNew);
	}
}

HWND PASCAL _AfxGetParentOwner(HWND hWnd)
{
	// check for permanent-owned window first
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
	if (pWnd != NULL)
		return pWnd->GetOwner()->GetSafeHwnd();

	// otherwise, return parent in the Windows sense
	return ::GetParent(hWnd);
}

void PASCAL _AfxCancelModes(HWND hWndRcvr)
{
	// if we receive a message destined for a window, cancel any combobox
	//  popups that could be in toolbars or dialog bars
	HWND hWndCancel = ::GetFocus();
	if (hWndCancel == NULL)
		return;     // nothing to cancel

	if (hWndCancel == hWndRcvr)
		return;     // let input go to window with focus

	// focus is in part of a combo-box
	if (!_AfxIsComboBoxControl(hWndCancel, (UINT)CBS_DROPDOWNLIST))
	{
		// try as a dropdown
		hWndCancel = ::GetParent(hWndCancel);   // parent of edit is combo
		if (hWndCancel == hWndRcvr)
			return;     // let input go to part of combo

		if (!_AfxIsComboBoxControl(hWndCancel, (UINT)CBS_DROPDOWN))
			return;     // not a combo-box that is active
	}

	// combo-box is active, but if receiver is a popup, do nothing
	if (hWndRcvr != NULL &&
	  (::GetWindowLong(hWndRcvr, GWL_STYLE) & WS_CHILD) != 0 &&
	  ::GetParent(hWndRcvr) == ::GetDesktopWindow())
		return;

	// finally, we should cancel the mode !
	::SendMessage(hWndCancel, CB_SHOWDROPDOWN, FALSE, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Special new handler for safety pool on temp maps

#ifdef _WINDOWS
#ifndef _PORTABLE

#define MIN_MALLOC_OVERHEAD 4           // LocalAlloc or other overhead

int cdecl AfxCriticalNewHandler(size_t nSize)       // nSize is already rounded
{
	// called during critical memory allocation
	//  free up part of the app's safety cache
	TRACE0("Warning: Critical memory allocation failed!\n");
	CWinApp* pApp = AfxGetApp();

	if (pApp != NULL && pApp->m_pSafetyPoolBuffer != NULL)
	{
		size_t nOldBufferSize = _msize(pApp->m_pSafetyPoolBuffer);
		if (nOldBufferSize <= nSize + MIN_MALLOC_OVERHEAD)
		{
			// give it all up
			TRACE0("Warning: Freeing application's memory safety pool!\n");
			free(pApp->m_pSafetyPoolBuffer);
			pApp->m_pSafetyPoolBuffer = NULL;
		}
		else
		{
			_expand(pApp->m_pSafetyPoolBuffer,
				nOldBufferSize - (nSize + MIN_MALLOC_OVERHEAD));
			TRACE3("Warning: Shrinking safety pool from %d to %d to "
				"satisfy request of %d bytes\n", nOldBufferSize,
				 _msize(pApp->m_pSafetyPoolBuffer), nSize);
		}
		return 1;       // retry it
	}
	TRACE0("ERROR: Critical memory allocation from safety pool failed!\n");
	AfxThrowMemoryException();      // oops
	return 0;
}
#endif // !_PORTABLE
#endif // _WINDOWS

/////////////////////////////////////////////////////////////////////////////
