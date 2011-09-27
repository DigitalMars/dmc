//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module: utils.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING UTILS.H from " __FILE__)
#endif  /* RC_INVOKED */

STDAPI_(LPVOID) OleStdAlloc(ULONG cb);
STDAPI_(LPVOID) OleStdRealloc(void FAR* pv, ULONG cb);
STDAPI_(VOID) OleStdFree(void FAR* pv);

#endif // __UTILS_H__
