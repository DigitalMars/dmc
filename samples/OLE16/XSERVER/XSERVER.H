//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: xserver.h
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __XSERVER_H__
#define __XSERVER_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING XSERVER.H from " __FILE__)
#endif  /* RC_INVOKED */

#pragma warning(disable:4355)

#include <windows.h>
#include <ole2.h>
#include <olenls.h>
#include <dispatch.h>

#define NC(a,b) a##::##b
#define DECLARE_NC(a,b) friend b;

typedef LPVOID FAR* LPLPVOID;

#include "resource.h"
#include "dispatch.h"
#include "utils.h"
#include "enumstat.h"
#include "cdebug.h"
#include "xobject.h"
#include "classfct.h"

extern HINSTANCE g_hInstance;

int CALLBACK
  LibMain(HINSTANCE hInst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine);

/////////////////////////////////////////////////////////////////////////////

#endif /* __XSERVER_H__ */
