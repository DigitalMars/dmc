//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module: utils.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"


STDAPI_(LPVOID)
  OleStdAlloc(ULONG cb)
//----------------------------------------------------------------------------
// OleStdAlloc:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  LPVOID pMem;
  ULONG dwRefs;
  HRESULT hRes;

  hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
  if (hRes != NOERROR) {
	return NULL;
  }

  pMem = pMalloc->Alloc(cb);
  dwRefs = pMalloc->Release();

  return pMem;
} /* OleStdAlloc()
   */


STDAPI_(LPVOID)
  OleStdRealloc(void FAR* pv, ULONG cb)
//----------------------------------------------------------------------------
// OleStdRealloc:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  LPVOID pMem;
  ULONG dwRefs;
  HRESULT hRes;

  hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
  if (hRes != NOERROR) {
	return NULL;
  }

  pMem = pMalloc->Realloc(pv, cb);
  dwRefs = pMalloc->Release();

  return pMem;

} /* OleStdRealloc()
   */


STDAPI_(VOID)
  OleStdFree(void FAR* pv)
//----------------------------------------------------------------------------
// OleStdFree:
//----------------------------------------------------------------------------
{
  LPMALLOC pMalloc;
  ULONG dwRefs;
  HRESULT hRes;

  if (pv != NULL) {
	hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc);
	if (hRes == NOERROR) {
	  pMalloc->Free(pv);
	  dwRefs = pMalloc->Release();
	}
  }

} /* OleStdFree()
   */
