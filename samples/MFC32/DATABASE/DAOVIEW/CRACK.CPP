// crack.cpp

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "crack.h"

LPCTSTR CCrack::strFieldType(short sType)
{
	switch(sType){
		case (dbBoolean):
			return _T("Bool");
		case (dbByte):
			return _T("Byte");
		case (dbInteger):
			return _T("Integer");
		case (dbLong):
			return _T("Long");
		case (dbCurrency):
			return _T("Currency");
		case (dbSingle):
			return _T("Single");
		case (dbDouble):
			return _T("Double");
		case (dbDate):
			return _T("Date");
		case (dbText):
			return _T("Text");
		case (dbLongBinary):
			return _T("Long Binary");
		case (dbMemo):
			return _T("Memo");
		case (dbGUID):
			return _T("GUID");
	}
	return _T("Unknown");
}

LPCTSTR CCrack::strQueryDefType(short sType)
{
	switch(sType){
		case (dbQSelect):
			return _T("Select");
		case (dbQAction):
			return _T("Action");
		case (dbQCrosstab):
			return _T("Crosstab");
		case (dbQDelete):
			return _T("Delete");
		case (dbQUpdate):
			return _T("Update");
		case (dbQAppend):
			return _T("Append");
		case (dbQMakeTable):
			return _T("MakeTable");
		case (dbQDDL):
			return _T("DDL");
		case (dbQSQLPassThrough):
			return _T("SQLPassThrough");
		case (dbQSetOperation):
			return _T("Set Operation");
		case (dbQSPTBulk):
			return _T("SPTBulk");
	}
	return _T("Unknown");
}

LPCTSTR CCrack::strBOOL(BOOL bFlag)
{
	return bFlag ? _T("TRUE") : _T("FALSE");
}

CString CCrack::strVARIANT(const COleVariant& var)
{
	CString strRet;
	strRet = _T("Fish");
	switch(var.vt){
		case VT_EMPTY:
		case VT_NULL:
			strRet = _T("NULL");
			break;
		case VT_I2:
			strRet.Format(_T("%hd"),V_I2(&var));
			break;
		case VT_I4:
			strRet.Format(_T("%d"),V_I4(&var));
			break;
		case VT_R4:
			strRet.Format(_T("%e"),(double)V_R4(&var));
			break;
		case VT_R8:
			strRet.Format(_T("%e"),V_R8(&var));
			break;
		case VT_CY:
			strRet = COleCurrency(var).Format();
			break;
		case VT_DATE:
			strRet = COleDateTime(var).Format(_T("%m %d %y"));
			break;
		case VT_BSTR:
			strRet = V_BSTRT(&var);
			break;
		case VT_DISPATCH:
			strRet = _T("VT_DISPATCH");
			break;
		case VT_ERROR:
			strRet = _T("VT_ERROR");
			break;
		case VT_BOOL:
			return strBOOL(V_BOOL(&var));
		case VT_VARIANT:
			strRet = _T("VT_VARIANT");
			break;
		case VT_UNKNOWN:
			strRet = _T("VT_UNKNOWN");
			break;
		case VT_I1:
			strRet = _T("VT_I1");
			break;
		case VT_UI1:
			strRet.Format(_T("0x%02hX"),(unsigned short)V_UI1(&var));
			break;
		case VT_UI2:
			strRet = _T("VT_UI2");
			break;
		case VT_UI4:
			strRet = _T("VT_UI4");
			break;
		case VT_I8:
			strRet = _T("VT_I8");
			break;
		case VT_UI8:
			strRet = _T("VT_UI8");
			break;
		case VT_INT:
			strRet = _T("VT_INT");
			break;
		case VT_UINT:
			strRet = _T("VT_UINT");
			break;
		case VT_VOID:
			strRet = _T("VT_VOID");
			break;
		case VT_HRESULT:
			strRet = _T("VT_HRESULT");
			break;
		case VT_PTR:
			strRet = _T("VT_PTR");
			break;
		case VT_SAFEARRAY:
			strRet = _T("VT_SAFEARRAY");
			break;
		case VT_CARRAY:
			strRet = _T("VT_CARRAY");
			break;
		case VT_USERDEFINED:
			strRet = _T("VT_USERDEFINED");
			break;
		case VT_LPSTR:
			strRet = _T("VT_LPSTR");
			break;
		case VT_LPWSTR:
			strRet = _T("VT_LPWSTR");
			break;
		case VT_FILETIME:
			strRet = _T("VT_FILETIME");
			break;
		case VT_BLOB:
			strRet = _T("VT_BLOB");
			break;
		case VT_STREAM:
			strRet = _T("VT_STREAM");
			break;
		case VT_STORAGE:
			strRet = _T("VT_STORAGE");
			break;
		case VT_STREAMED_OBJECT:
			strRet = _T("VT_STREAMED_OBJECT");
			break;
		case VT_STORED_OBJECT:
			strRet = _T("VT_STORED_OBJECT");
			break;
		case VT_BLOB_OBJECT:
			strRet = _T("VT_BLOB_OBJECT");
			break;
		case VT_CF:
			strRet = _T("VT_CF");
			break;
		case VT_CLSID:
			strRet = _T("VT_CLSID");
			break;
	}
	WORD vt = var.vt;
	if(vt & VT_ARRAY){
		vt = vt & ~VT_ARRAY;
		strRet = _T("Array of ");
	}
	if(vt & VT_BYREF){
		vt = vt & ~VT_BYREF;
		strRet += _T("Pointer to ");
	}
	if(vt != var.vt){
		switch(vt){
			case VT_EMPTY:
				strRet += _T("VT_EMPTY");
				break;
			case VT_NULL:
				strRet += _T("VT_NULL");
				break;
			case VT_I2:
				strRet += _T("VT_I2");
				break;
			case VT_I4:
				strRet += _T("VT_I4");
				break;
			case VT_R4:
				strRet += _T("VT_R4");
				break;
			case VT_R8:
				strRet += _T("VT_R8");
				break;
			case VT_CY:
				strRet += _T("VT_CY");
				break;
			case VT_DATE:
				strRet += _T("VT_DATE");
				break;
			case VT_BSTR:
				strRet += _T("VT_BSTR");
				break;
			case VT_DISPATCH:
				strRet += _T("VT_DISPATCH");
				break;
			case VT_ERROR:
				strRet += _T("VT_ERROR");
				break;
			case VT_BOOL:
				strRet += _T("VT_BOOL");
				break;
			case VT_VARIANT:
				strRet += _T("VT_VARIANT");
				break;
			case VT_UNKNOWN:
				strRet += _T("VT_UNKNOWN");
				break;
			case VT_I1:
				strRet += _T("VT_I1");
				break;
			case VT_UI1:
				strRet += _T("VT_UI1");
				break;
			case VT_UI2:
				strRet += _T("VT_UI2");
				break;
			case VT_UI4:
				strRet += _T("VT_UI4");
				break;
			case VT_I8:
				strRet += _T("VT_I8");
				break;
			case VT_UI8:
				strRet += _T("VT_UI8");
				break;
			case VT_INT:
				strRet += _T("VT_INT");
				break;
			case VT_UINT:
				strRet += _T("VT_UINT");
				break;
			case VT_VOID:
				strRet += _T("VT_VOID");
				break;
			case VT_HRESULT:
				strRet += _T("VT_HRESULT");
				break;
			case VT_PTR:
				strRet += _T("VT_PTR");
				break;
			case VT_SAFEARRAY:
				strRet += _T("VT_SAFEARRAY");
				break;
			case VT_CARRAY:
				strRet += _T("VT_CARRAY");
				break;
			case VT_USERDEFINED:
				strRet += _T("VT_USERDEFINED");
				break;
			case VT_LPSTR:
				strRet += _T("VT_LPSTR");
				break;
			case VT_LPWSTR:
				strRet += _T("VT_LPWSTR");
				break;
			case VT_FILETIME:
				strRet += _T("VT_FILETIME");
				break;
			case VT_BLOB:
				strRet += _T("VT_BLOB");
				break;
			case VT_STREAM:
				strRet += _T("VT_STREAM");
				break;
			case VT_STORAGE:
				strRet += _T("VT_STORAGE");
				break;
			case VT_STREAMED_OBJECT:
				strRet += _T("VT_STREAMED_OBJECT");
				break;
			case VT_STORED_OBJECT:
				strRet += _T("VT_STORED_OBJECT");
				break;
			case VT_BLOB_OBJECT:
				strRet += _T("VT_BLOB_OBJECT");
				break;
			case VT_CF:
				strRet += _T("VT_CF");
				break;
			case VT_CLSID:
				strRet += _T("VT_CLSID");
				break;
		}
	}
	return strRet;
}
