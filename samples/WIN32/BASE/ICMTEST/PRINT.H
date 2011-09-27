//**********************************************************
// File Name:
//    PRINT.H
//
// Description:
//    Header file for PRINT.C
//
// Network Location:
//    *
//
// Test Machine Location:
//    *
//
// Internal Dependencies:
//    *
//
// Output Files:
//    *
//
// Functions:
//    *
//
// Developers:
//    *
//
// Written by Microsoft System Testing
// Copyright (c) 1993 Microsoft Corporation. All rights reserved.
//**********************************************************



#ifdef PRINTSS_LOCAL

//
// Globals
//
#define MAX_STRING   256

#define LEVEL0       0L
#define LEVEL1       1L
#define LEVEL2       2L
#define LEVEL3       3L
#define LEVEL4       4L

#define ENUM_ERROR         0x80000000
#define ERROR_ENUMPRINTERS 0x80000001

#endif



//*************************************************************************
//
//   Functions available to other modules
//
//*************************************************************************

BOOL  PASCAL InitPrinting (HDC hDC, HWND hWnd, HANDLE hInst, LPSTR msg);
void  PASCAL TermPrinting (HDC hDC);
void  PrintDIB (HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy);
LPSTR GetDefaultPrinter(LPHANDLE lphString);
DWORD EnumerateAllPrinters(DWORD dwType, LPSTR lpszName, DWORD dwLevel, 
                           LPHANDLE lphPrinterInfo);


  
