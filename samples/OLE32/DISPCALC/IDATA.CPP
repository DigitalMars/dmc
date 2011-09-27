/*** 
*idata.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  This file contains the INTERFACEDATA definitions for the methods
*  and properties on the CCalc object that are exposed for external
*  programmability via IDispatch.
*
*
*Implementation Notes:
*
*****************************************************************************/

#include "dispcalc.h"


//---------------------------------------------------------------------
//                   INTERFACEDATA definitions 
//---------------------------------------------------------------------

/* The INTERFACEDATA table describes the methods and properties that
 * are being exposed for external programmability via IDispatch.
 * This table is used to construct a CDispTypeInfo for this interface,
 * and that TypeInfo is in turn used to initialize and drive the
 * default implementation of IDispatch.
 */


// PDATA1() declares the PARAMDATA for a methods that takes a single param
//
#define PDATA1(NAME, TYPE) \
    static PARAMDATA NEARDATA rgpdata ## NAME = {OLESTR(#NAME), TYPE}

// MDATA() declares a single METHODDATA entry
//
#define MDATA(NAME, PDATA, IDMEMBER, IMETH, CARGS, KIND, TYPE) \
    { OLESTR(#NAME), PDATA, IDMEMBER, IMETH, CC_CDECL, CARGS, KIND, TYPE }

// The following macro defines the METHODDATA entries for a 
// property Put/Get method pair.
//
// Note: this macro *assumes* that the Put/Get pair are adjacent
// in the vtable, and that the Put method comes first.
//
#define PROPERTY(NAME, IMETH, ID, TYPE) \
    MDATA(NAME, &rgpdata ## NAME, ID, IMETH, 1,DISPATCH_PROPERTYPUT,VT_EMPTY), \
    MDATA(NAME, NULL, ID, IMETH+1, 0, DISPATCH_PROPERTYGET, TYPE)

// The following macro is used to define a METHODDATA entry for
// a method that takes zero parameters.
//
#define METHOD0(NAME, IMETH, ID, TYPE) \
    MDATA(NAME, NULL, ID, IMETH, 0, DISPATCH_METHOD, TYPE)

// and for one param
#define METHOD1(NAME, IMETH, ID, TYPE) \
    MDATA(NAME, &rgpdata ## NAME, ID, IMETH, 1, DISPATCH_METHOD, TYPE)


PDATA1(VALUE,  VT_I4);
PDATA1(ACCUM,  VT_I4);
PDATA1(OPND,   VT_I4);
PDATA1(OP,     VT_I2);
PDATA1(BUTTON, VT_BSTR);

static METHODDATA NEARDATA rgmdataCCalc[] = 
{
      PROPERTY(VALUE,   IMETH_ACCUM,    IDMEMBER_ACCUM,    VT_I4) 
    , PROPERTY(ACCUM,   IMETH_ACCUM,    IDMEMBER_ACCUM,    VT_I4) 
    , PROPERTY(OPND,    IMETH_OPERAND,  IDMEMBER_OPERAND,  VT_I4) 
    , PROPERTY(OP,      IMETH_OPERATOR, IDMEMBER_OPERATOR, VT_I2) 
    ,  METHOD0(EVAL,    IMETH_EVAL,     IDMEMBER_EVAL,     VT_BOOL) 
    ,  METHOD0(CLEAR,   IMETH_CLEAR,    IDMEMBER_CLEAR,    VT_EMPTY)
    ,  METHOD0(DISPLAY, IMETH_DISPLAY,  IDMEMBER_DISPLAY,  VT_EMPTY)
    ,  METHOD0(QUIT,    IMETH_QUIT,     IDMEMBER_QUIT,     VT_EMPTY)
    ,  METHOD1(BUTTON,  IMETH_BUTTON,   IDMEMBER_BUTTON,   VT_BOOL)
};

INTERFACEDATA NEARDATA g_idataCCalc = 
{
    rgmdataCCalc, DIM(rgmdataCCalc)
};
