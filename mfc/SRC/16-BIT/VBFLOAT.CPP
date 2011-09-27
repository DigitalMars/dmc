// This is a part of the Microsoft Classes C++ Class Library.
// Copyright (C) 1992-1993 Microsoft Corporation,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "vbctrl.h"

#ifdef AFX_VBX_SEG
#pragma code_seg(AFX_VBX_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////

static int PASCAL _ConvertFontSizeToTwips(LONG lFontSize)
{
	float value = *(float*)&lFontSize;
	return (int)((value * 20.0) + 0.5);
}

static long PASCAL _ConvertTwipsToFontSize(int nTwips)
{
	float temp = ((float)nTwips) / 20;
	return *(LONG*)&temp;
}

void CVBControl::EnableVBXFloat()
{
	_afxConvertFontSizeToTwipsProc = _ConvertFontSizeToTwips;
	_afxConvertTwipsToFontSizeProc = _ConvertTwipsToFontSize;
}


float CVBControl::GetFloatProperty(int nPropIndex, int index)
{
	if (_afxConvertFontSizeToTwipsProc == NULL)
		EnableVBXFloat();

	LONG temp;
	temp = GetNumPropertyWithType(nPropIndex, TYPE_REAL, index);
	return *(float*)&temp;      // Do not do straight cast (float)
								// as this would change the data format
}

BOOL CVBControl::SetFloatProperty(int nPropIndex, float value, int index)
{
	if (_afxConvertFontSizeToTwipsProc == NULL)
		EnableVBXFloat();

	return SetPropertyWithType(nPropIndex, TYPE_REAL, *(LONG*)&value, index);
}
