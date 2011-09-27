/*** 
*tdata.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1991-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  CPoly and CPoint type data descriptions.
*
*  These data descriptions are used to construct TypeInfos for these
*  objects at runtime.
*
*****************************************************************************/

#include "spoly.h"
#include "cpoint.h"
#include "cpoly.h"


//---------------------------------------------------------------------
//                      CPoint type data definitions 
//---------------------------------------------------------------------


static PARAMDATA NEAR rgpdataCPointSetX[] = 
{
    { OLESTR("X"), VT_I2 }
};

static PARAMDATA NEAR rgpdataCPointSetY[] = 
{
    { OLESTR("Y"), VT_I2 }
};

static METHODDATA NEAR rgmdataCPoint[] = 
{
    // CPoint::GetX()
    {
	OLESTR("GetX"),
	NULL,
	IDMEMBER_CPOINT_GETX,
	IMETH_CPOINT_GETX,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // CPoint::SetX()
    {
	OLESTR("SetX"),
	rgpdataCPointSetX,
	IDMEMBER_CPOINT_SETX,
	IMETH_CPOINT_SETX,
	CC_CALL,
	DIM(rgpdataCPointSetX),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // CPoint::GetY()
    {
	OLESTR("GetY"),
	NULL,
	IDMEMBER_CPOINT_GETY,
	IMETH_CPOINT_GETY,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // CPoint::SetY()
    {
	OLESTR("SetY"),
	rgpdataCPointSetY,
	IDMEMBER_CPOINT_SETY,
	IMETH_CPOINT_SETY,
	CC_CALL,
	DIM(rgpdataCPointSetY),
	DISPATCH_METHOD,
	VT_EMPTY
    }
};

INTERFACEDATA NEAR g_idataCPoint = 
{
    rgmdataCPoint, DIM(rgmdataCPoint)
};


//---------------------------------------------------------------------
//                   CPoly type data definitions
//---------------------------------------------------------------------


static PARAMDATA NEAR rgpdataCPolyAddPoint[] = 
{
    { OLESTR("x"),	VT_I2 },
    { OLESTR("y"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetXOrigin[] =
{
    { OLESTR("x"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetYOrigin[] = 
{
    { OLESTR("y"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetWidth[] = 
{
    { OLESTR("width"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetRed[] = 
{
    { OLESTR("red"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetGreen[] = 
{
    { OLESTR("green"),	VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetBlue[] = 
{
    { OLESTR("blue"),	VT_I2 }
};

static METHODDATA NEAR rgmdataCPoly[] = 
{
    // void CPoly::Draw(void)
    {
	OLESTR("Draw"),
	NULL,
	IDMEMBER_CPOLY_DRAW,
	IMETH_CPOLY_DRAW,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // void CPoly::Reset(void)
    {
	OLESTR("Reset"),
	NULL,
	IDMEMBER_CPOLY_RESET,
	IMETH_CPOLY_RESET,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // HRESULT CPoly::AddPoint(short x, short y)
    {
	OLESTR("AddPoint"),
	rgpdataCPolyAddPoint,
	IDMEMBER_CPOLY_ADDPOINT,
	IMETH_CPOLY_ADDPOINT,
	CC_CALL,
	DIM(rgpdataCPolyAddPoint),
	DISPATCH_METHOD,
	VT_ERROR
    },

    // IUnknown FAR* CPoly::EnumPoints(void)
    {
	OLESTR("EnumPoints"),
	NULL,
	IDMEMBER_CPOLY_ENUMPOINTS,
	IMETH_CPOLY_ENUMPOINTS,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_UNKNOWN
    },

    // short CPoly::GetXOrigin(void)
    {
	OLESTR("GetXOrigin"),
	NULL,
	IDMEMBER_CPOLY_GETXORIGIN,
	IMETH_CPOLY_GETXORIGIN,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::SetXOrigin(short x)
    {
	OLESTR("SetXOrigin"),
	rgpdataCPolySetXOrigin,
	IDMEMBER_CPOLY_SETXORIGIN,
	IMETH_CPOLY_SETXORIGIN,
	CC_CALL,
	DIM(rgpdataCPolySetXOrigin),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // short CPoly::GetYOrigin(void)
    {
	OLESTR("GetYOrigin"),
	NULL,
	IDMEMBER_CPOLY_GETYORIGIN,
	IMETH_CPOLY_GETYORIGIN,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::SetYOrigin(short y)
    {
	OLESTR("SetYOrigin"),
	rgpdataCPolySetYOrigin,
	IDMEMBER_CPOLY_SETYORIGIN,
	IMETH_CPOLY_SETYORIGIN,
	CC_CALL,
	DIM(rgpdataCPolySetYOrigin),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // short CPoly::GetWidth(void)
    {
	OLESTR("GetWidth"),
	NULL,
	IDMEMBER_CPOLY_GETWIDTH,
	IMETH_CPOLY_GETWIDTH,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::SetWidth(short width)
    {
	OLESTR("SetWidth"),
	rgpdataCPolySetWidth,
	IDMEMBER_CPOLY_SETWIDTH,
	IMETH_CPOLY_SETWIDTH,
	CC_CALL,
	DIM(rgpdataCPolySetWidth),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // short CPoly::get_red(void)
    {
	OLESTR("get_red"),
	NULL,
	IDMEMBER_CPOLY_GETRED,
	IMETH_CPOLY_GETRED,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::set_red(short red)
    {
	OLESTR("set_red"),
	rgpdataCPolySetRed,
	IDMEMBER_CPOLY_SETRED,
	IMETH_CPOLY_SETRED,
	CC_CALL,
	DIM(rgpdataCPolySetRed),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // short CPoly::get_green(void)
    {
	OLESTR("get_green"),
	NULL,
	IDMEMBER_CPOLY_GETGREEN,
	IMETH_CPOLY_GETGREEN,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::set_green(short green)
    {
	OLESTR("set_green"),
	rgpdataCPolySetGreen,
	IDMEMBER_CPOLY_SETGREEN,
	IMETH_CPOLY_SETGREEN,
	CC_CALL,
	DIM(rgpdataCPolySetGreen),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // short CPoly::get_blue(void)
    {
	OLESTR("get_blue"),
	NULL,
	IDMEMBER_CPOLY_GETBLUE,
	IMETH_CPOLY_GETBLUE,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_I2
    },

    // void CPoly::set_blue(short blue)
    {
	OLESTR("set_blue"),
	rgpdataCPolySetBlue,
	IDMEMBER_CPOLY_SETBLUE,
	IMETH_CPOLY_SETBLUE,
	CC_CALL,
	DIM(rgpdataCPolySetBlue),
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // void CPoly::Dump(void)
    {
	OLESTR("Dump"),
	NULL,
	IDMEMBER_CPOLY_DUMP,
	IMETH_CPOLY_DUMP,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_EMPTY
    },

    // void CPoly::Quit(void)
    {
	OLESTR("Quit"),
	NULL,
	IDMEMBER_CPOLY_QUIT,
	IMETH_CPOLY_QUIT,
	CC_CALL,
	0,
	DISPATCH_METHOD,
	VT_EMPTY
    },

};

INTERFACEDATA NEAR g_idataCPoly =
{
    rgmdataCPoly, DIM(rgmdataCPoly)
};
