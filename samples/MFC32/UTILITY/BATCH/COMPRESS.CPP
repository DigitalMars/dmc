/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/
// compress.cpp : implementation file
//

#include "stdafx.h"
#include "batch.h"
#include "compress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompressThread

IMPLEMENT_DYNCREATE(CCompressThread, CWinThread)

CCompressThread::CCompressThread()
{
}

CCompressThread::~CCompressThread()
{
}


BEGIN_MESSAGE_MAP(CCompressThread, CCmdTarget)
	//{{AFX_MSG_MAP(CCompressThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompressThread message handlers
