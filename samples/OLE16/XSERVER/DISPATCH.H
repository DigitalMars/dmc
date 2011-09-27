//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: dispatch.h
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __DISPATCH_H__
#define __DISPATCH_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING DISPATCH.H from " __FILE__)
#endif  /* RC_INVOKED */

/////////////////////////////////////////////////////////////////////////////

#define DIM(X) (sizeof(X) / sizeof(X[0]))

// DISPIDs for the members and properties available via IDispatch.
//
enum IDMEMBER {
	IDMEMBER_FASTDRAW = 1,
	IDMEMBER_SETEXTENT,
	IDMEMBER_GETEXTENT,
	IDMEMBER_SHOWOBJECT,
	IDMEMBER_MAX
};

// CXObject method indices
//
enum IMETH_CXOBJECT_CDISPATCH {
	IMETH_CXOBJECT_CDISPATCH_QUERYINTERFACE = 0,
	IMETH_CXOBJECT_CDISPATCH_ADDREF,
	IMETH_CXOBJECT_CDISPATCH_RELEASE,
	IMETH_CXOBJECT_CDISPATCH_GETTYPEINFOCOUNT,
	IMETH_CXOBJECT_CDISPATCH_GETTYPEINFO,
	IMETH_CXOBJECT_CDISPATCH_GETIDSOFNAMES,
	IMETH_CXOBJECT_CDISPATCH_INVOKE,
	IMETH_CXOBJECT_CDISPATCH_FASTDRAW,
	IMETH_CXOBJECT_CDISPATCH_SETEXTENT,
	IMETH_CXOBJECT_CDISPATCH_GETEXTENT,
	IMETH_CXOBJECT_CDISPATCH_SHOWOBJECT,
	IMETH_CXOBJECT_MAX
};

/////////////////////////////////////////////////////////////////////////////

#endif // __DISPATCH_H__
