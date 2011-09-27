//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: xserver.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"

HINSTANCE g_hInstance = NULL;


void FAR* operator new(size_t size)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
	IMalloc FAR* pMalloc;
	LPVOID lpv;

	if (CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR) {
	lpv = pMalloc->Alloc(size);
	pMalloc->Release();
	return lpv;
  }
  return NULL;

} /* new()
   */


void operator delete (void FAR* lpv)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
	IMalloc FAR* pMalloc;

	if (lpv == NULL) return;

  if (CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR) {
	if (pMalloc->DidAlloc(lpv)) {
	  pMalloc->Free(lpv);
	}
	pMalloc->Release();
  }

} /* delete()
   */


int CALLBACK
  LibMain(HINSTANCE hInst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
//----------------------------------------------------------------------------
// InitInstance:
//----------------------------------------------------------------------------
{
  HDC hDC;

  if (g_hInstance == NULL) {
	g_hInstance = hInst;

	hDC = GetDC(NULL);
	giXppli = GetDeviceCaps(hDC, LOGPIXELSX);
	giYppli = GetDeviceCaps(hDC, LOGPIXELSY);

	ReleaseDC(NULL, hDC);

	CDebug::StartDebugging();
  }

  return TRUE;

} /* LibMain()
   */
