/***
*tdata.cpp
*
*  Copyright (C) 1991, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
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
	{ "X",      VT_I2 }
};

static PARAMDATA NEAR rgpdataCPointSetY[] =
{
	{ "Y",      VT_I2 }
};

static METHODDATA NEAR rgmdataCPoint[] =
{
	// CPoint::GetX()
	{
		"GetX",
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
		"SetX",
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
		"GetY",
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
		"SetY",
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
	{ "x",      VT_I2 },
	{ "y",      VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetXOrigin[] =
{
	{ "x",      VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetYOrigin[] =
{
	{ "y",      VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetWidth[] =
{
	{ "width",  VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetRed[] =
{
	{ "red",    VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetGreen[] =
{
	{ "green",  VT_I2 }
};

static PARAMDATA NEAR rgpdataCPolySetBlue[] =
{
	{ "blue",   VT_I2 }
};

static METHODDATA NEAR rgmdataCPoly[] =
{
	// void CPoly::Draw(void)
	{
		"Draw",
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
		"Reset",
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
		"AddPoint",
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
		"EnumPoints",
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
		"GetXOrigin",
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
		"SetXOrigin",
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
		"GetYOrigin",
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
		"SetYOrigin",
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
		"GetWidth",
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
		"SetWidth",
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
		"get_red",
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
		"set_red",
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
		"get_green",
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
		"set_green",
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
		"get_blue",
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
		"set_blue",
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
		"Dump",
		NULL,
		IDMEMBER_CPOLY_DUMP,
		IMETH_CPOLY_DUMP,
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
