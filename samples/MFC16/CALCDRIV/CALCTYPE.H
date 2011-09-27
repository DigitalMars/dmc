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


/////////////////////////////////////////////////////////////////////////////
// CDispCalc2 wrapper class

class CDispCalc2 : public COleDispatchDriver
{
// Attributes
public:

// Operations
public:
	void SetAccum(long p0);
	long GetAccum();
	void SetOpnd(long p0);
	long GetOpnd();
	void SetOp(short p0);
	short GetOp();
	BOOL Eval();
	void Clear();
	void Display();
	void Quit();
	BOOL Button(const char* Button);
};
