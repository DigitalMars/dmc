// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// vbctrl.h - interface between CVBControl and private VB Runtime implementation

/////////////////////////////////////////////////////////////////////////////

#define VBM__BASE           ( WM_USER + 0x0C00 )
#define VBM_CREATED         ( VBM__BASE + 0x00 )
#define VBM_INITIALIZE      ( VBM__BASE + 0x02 )

#define VBM_METHOD          ( VBM__BASE + 0x11 )

#define METH_ADDITEM    0x0001  // args are: cArgs, hszItem, [index]
#define METH_REMOVEITEM 0x0002  // args are: cArgs, index
#define METH_REFRESH    0x0003  // no args (lp == NULL)
#define METH_MOVE       0x0004  // args are: cArgs, left, [top, [width, [height]]]

#define MODEL_fInitMsg      0x00000010L
#define MODEL_fLoadMsg      0x00000020L

/////////////////////////////////////////////////////////////////////////////
// Shared globals

class CVBXFile;

extern CVBControl* NEAR _afxNewVBControl; // Set before CreateWindow
extern CVBXFile* NEAR _afxVBXFileCur;   // for use of VBRegisterModel
extern CFile* NEAR _afxFormFileCur; // used in vbcore2.cpp

// Address of routines for EnableVBXFloat
extern int (PASCAL* NEAR _afxConvertFontSizeToTwipsProc)(LONG lFontSize);
extern long (PASCAL* NEAR _afxConvertTwipsToFontSizeProc)(int nTwips);
extern char BASED_CODE _afxVBPrefix[];

/////////////////////////////////////////////////////////////////////////////
