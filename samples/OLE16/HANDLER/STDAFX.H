//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: stdafx.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __STDAFX_H__
#define __STDAFX_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING STDAFX.H from " __FILE__)
#endif  /* RC_INVOKED */

#include <afxwin.h>         // MFC core and standard components
#include <ole2.h>
#include "resource.h"       // Resource ID definitions

class CHandlerWindow;
class CHandlerObject;
class CClassFactory;

#include "utils.h"
#include "dbghndlr.h"       // Debug Handler window object
#include "object.h"         // object handler
#include "classfct.h"       // IClassFactory implementation
#include "cdebug.h"         // Debugging class

/////////////////////////////////////////////////////////////////////////////

#endif // __STDAFX_H__
