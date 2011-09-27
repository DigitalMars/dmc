// Machine generated IDispatch driver class(es) created with ClassWizard.
//
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
#include "calctype.h"


/////////////////////////////////////////////////////////////////////////////
// CDispCalc2 properties

/////////////////////////////////////////////////////////////////////////////
// CDispCalc2 operations

void CDispCalc2::SetAccum(long p0)
{
	static BYTE BASED_CODE parms[] =
		VTS_I4;
	InvokeHelper(0x60010016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 p0);
}

long CDispCalc2::GetAccum()
{
	long result;
	InvokeHelper(0x60010016, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDispCalc2::SetOpnd(long p0)
{
	static BYTE BASED_CODE parms[] =
		VTS_I4;
	InvokeHelper(0x60010046, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 p0);
}

long CDispCalc2::GetOpnd()
{
	long result;
	InvokeHelper(0x60010046, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDispCalc2::SetOp(short p0)
{
	static BYTE BASED_CODE parms[] =
		VTS_I2;
	InvokeHelper(0x60010076, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 p0);
}

short CDispCalc2::GetOp()
{
	short result;
	InvokeHelper(0x60010076, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL CDispCalc2::Eval()
{
	BOOL result;
	InvokeHelper(0x600100ae, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CDispCalc2::Clear()
{
	InvokeHelper(0x600100c4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispCalc2::Display()
{
	InvokeHelper(0x600100da, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispCalc2::Quit()
{
	InvokeHelper(0x600100f0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CDispCalc2::Button(const char* Button)
{
	BOOL result;
	static BYTE BASED_CODE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60010106, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Button);
	return result;
}
