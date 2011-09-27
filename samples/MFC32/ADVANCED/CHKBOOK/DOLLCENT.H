// dollcent.h: interface for DDX_DollarsCents
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#define INVALID_DOLLARS_CENTS 0xffffffff

void AFXAPI DDX_DollarsCents(CDataExchange* pDX, int nIDC, DWORD& dwCents);
BOOL GetDollarsCents(CWnd* pWnd, DWORD& dwCents);
BOOL GetDollarsCents(HWND hWnd, DWORD& dwCents);
void SetDollarsCents(CWnd* pWnd, DWORD dwCents);
CString GetDollarsCentsFormatted(DWORD dwCents);
CString GetDollarsCentsText(DWORD dwCents);
