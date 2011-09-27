// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright © 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  PROGRAM:  print.c
//
//  PURPOSE:  Routines used for printing.
//
//  PLATFORMS:  Chicago
//

#include <windows.h>
#include <string.h>
#define PRINTSS_LOCAL
#include "print.h"
#include "icmtest.h"
#include "resource.h"

//***************************************************************************
//                                                                          * 
//   Functions private to this module                                       *
//                                                                          *
//***************************************************************************

BOOL FAR PASCAL AbortProc (HDC, short);
BOOL FAR PASCAL PrintDlgProc (HWND, unsigned, WORD, DWORD);
void PrintDIBs(HDC hDC, int start, int stop);
void PrintImage(HDC hDC, int FirstPage, int LastPage);
void PrintEdgeGradients(HDC hDC);
void PrintDiagonalGradients(HDC hDC);
void PrintCalibrationGradients(HDC hDC, int Page);

//***************************************************************************
//                                                                          *
//  Global vars external to this module                                     *
//                                                                          *
//***************************************************************************
extern WORD                     gwBMPIndex;
extern BMPINFO                  gBMPInfo[MAX_BMP];
extern HCURSOR                  ghcurSave;
extern HINSTANCE                ghInst;
extern HCOLORSPACE              ghlcsCurrent;
extern COLORSPACECONTROLENTRY   galcsControl[MAX_COLORSPACES];
extern UINT                     guiColorSpacesUsed;
extern HCOLORSPACE              ghlcsDefault;
extern BOOL                     gfICMEnabled;

//***************************************************************************
//                                                                          *
//  Global vars internal to this module                                     *
//                                                                          *
//***************************************************************************

LOGCOLORSPACE ScannerCS = {0x50534F43, 0x400, sizeof(LOGCOLORSPACE),
			   LCS_CALIBRATED_RGB, 1,
			   0,0,0,0,0,0,0,0,0,
			   0,0,0,
			   "c:\\windows\\system\\color\\hpsjtw.icm"};
LOGCOLORSPACE ScreenCS	= {0x50534F43, 0x400, sizeof(LOGCOLORSPACE),
			   LCS_CALIBRATED_RGB, 1,
			   0,0,0,0,0,0,0,0,0,
			   0,0,0,
			   "c:\\windows\\system\\color\\mnb22g21.icm"};
HANDLE	 hDevMode	   = NULL;
DEVMODE *pDevMode	   = NULL;
HANDLE	 hDevNames	   = NULL;
FARPROC  glpfnAbortProc    = NULL;
FARPROC  glpfnPrintDlgProc = NULL;
HWND     ghWndParent       = NULL;
HWND     ghDlgPrint        = NULL;
BOOL     gbUserAbort;
LOGFONT  LogFont = { 0, 0, 0, 0, 400, 0, 0, 0, 0, 0 ,0 ,0 , 0, "Arial"};
WCHAR	 *DIBLabels[] = {L"Dummy",
			 L"3.1   1 bit",
			 L"3.1   4 bit",
			 L"3.1   8 bit",
			 L"     16 bit",
			 L"555  16 bit",
			 L"565  16 bit",
			 L"3.1  24 bit",
			 L"     32 bit",
			 L"888  32 bit",
			 L"down  8 bit",
			 L"icm   1 bit",
			 L"icm   4 bit",
			 L"icm   8 bit",
			 L"icm  16 bit",
			 L"icm 555 bit",
			 L"icm 565 bit",
			 L"icm  24 bit",
			 L"icm  32 bit",
			 L"icm 888 bit",
			 L"CMYK 32 bit"};

WCHAR	*ImageLabels[][2] = {{L"Faces 24 bits, ICM off.",
			      L"Faces 24 bits, ICM on."  },
			     {L"Sunset 24 bits, ICM off.",
			      L"Sunset 24 bits, ICM on."  },
			     {L"Church 24 bits, ICM off.",
			      L"Church 24 bits, ICM on."  },
			     {L"Redbush 24 bits, ICM off.",
			      L"Redbush 24 bits, ICM on."  }};

//***************************************************************************
//                                                                          *
// FunctionName: PrintICMImages 					    *
//                                                                          *
// Purpose:                                                                 *
//    To print the ICM images on the printer.                               *
//                                                                          *
// Parameters:                                                              *
//    HWND    Handle to window                                              *
//                                                                          *
//                                                                          *
// Return Value:                                                            *
//    BOOL  TRUE  - Success.                                                *
//      FALSE - Failed.                                                     *
//                                                                          *
// Comments:                                                                *
//                                                                          *
//***************************************************************************

BOOL PrintICMImages(HWND hWnd, int Page)
{
  HDC		hDC;
  HANDLE	hFont, oldhFont;
  char		szDocName[256];

  GetWindowText(hWnd, szDocName, sizeof(szDocName));

  // Get DC for DEFAULT printer
  if (!(hDC = GetPrinterDC()))
    return(FALSE);

  // Use a 14 point font for output.

  LogFont.lfHeight = 14 * ((GetDeviceCaps(hDC, LOGPIXELSY))/72);
  hFont = CreateFontIndirect(&LogFont);
  oldhFont = SelectObject(hDC, hFont);

  if (InitPrinting(hDC, hWnd, ghInst, szDocName)) 
  {
    switch (Page)
    {
      case IDM_FILE_PRINT_BI_UP:
	PrintDIBs(hDC, BMP_1, BMP_888);
	break;
      case IDM_FILE_PRINT_BI_DOWN:
	PrintDIBs(hDC, BMP_8_DOWN, BMP_8_DOWN);
	break;
      case IDM_FILE_PRINT_BV4_UP:
	PrintDIBs(hDC, BMP_1_ICM, BMP_888_ICM);
	break;
      case IDM_FILE_PRINT_BV4_DOWN:
	break;
      case IDM_FILE_PRINT_CMYK:
	PrintDIBs(hDC, BMP_CMYK_ICM, BMP_CMYK_ICM);
	break;
      case IDM_FILE_PRINT_TESTDIBS_ALL:
	PrintDIBs(hDC, BMP_1, BMP_888);
	EndPage(hDC);
	StartPage(hDC);
	PrintDIBs(hDC, BMP_8_DOWN, BMP_8_DOWN);
	EndPage(hDC);
	StartPage(hDC);
	PrintDIBs(hDC, BMP_1_ICM, BMP_888_ICM);
	EndPage(hDC);
	StartPage(hDC);
	PrintDIBs(hDC, BMP_CMYK_ICM, BMP_CMYK_ICM);
	break;
      case IDM_FILE_PRINT_FACES:
	PrintImage(hDC, BMP_FACES, BMP_FACES);
	break;
      case IDM_FILE_PRINT_SUNSET:
	PrintImage(hDC, BMP_SUNSET, BMP_SUNSET);
	break;
      case IDM_FILE_PRINT_CHURCH:
	PrintImage(hDC, BMP_CHURCH, BMP_CHURCH);
	break;
      case IDM_FILE_PRINT_REDBUSH:
	PrintImage(hDC, BMP_REDBUSH, BMP_REDBUSH);
	break;
      case IDM_FILE_PRINT_NAT_IMAGES_ALL:
	PrintImage(hDC, BMP_FACES, BMP_REDBUSH);
	break;
      case IDM_FILE_PRINT_TRC_1_GRADIENTS:
      case IDM_FILE_PRINT_TRC_2_GRADIENTS:
	PrintCalibrationGradients(hDC, Page);
	break;
      case IDM_FILE_PRINT_EDGE_GRADIENTS:
	PrintEdgeGradients(hDC);
	break;
      case IDM_FILE_PRINT_DIAG_GRADIENTS:
	PrintDiagonalGradients(hDC);
	break;
    }

    // Try different intents. 
    // Signal to the driver to begin translating the
    // drawing commands to printer output...
    TermPrinting(hDC);
  }
  SelectObject(hDC, oldhFont);
  DeleteObject(oldhFont);
  DeleteDC(hDC);
}

//***************************************************************************
//                                                                          *
//  FUNCTION: PrintDIBs(HDC hDC, int start, int stop)			    *
//                                                                          *
//  PURPOSE:  Set the DIBs bits to the printer DC.			    *
//                                                                          *
//***************************************************************************
void PrintDIBs(HDC hDC, int start, int stop)
{
  HANDLE	    doDIB;
  BITMAPINFOHEADER  bi;
  int		    xSize, ySize, xRes, yRes, dx, dy;
  int		    i, yoffset;
  HCOLORSPACE	    hcsWeird;

  doDIB = gBMPInfo[gwBMPIndex].hDib;
  bi = *(LPBITMAPINFOHEADER)doDIB;

  // Obtain info about printer resolution
  xSize = GetDeviceCaps(hDC, HORZRES);
  ySize = GetDeviceCaps(hDC, VERTRES);
  xRes  = GetDeviceCaps(hDC, LOGPIXELSX);
  yRes  = GetDeviceCaps(hDC, LOGPIXELSY);

  // Put 4 images across withs space for comments.
  // Maintain the same aspect ratio.
  dx = (xSize - (xRes))/5;
  dy = ((int)((long)dx * bi.biHeight/bi.biWidth));

  if (pDevMode)
    TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));
  for (i = start, yoffset = yRes/4; i <= stop; yoffset += (dy + yRes/16), i++)
  {
    // Override whatever the user has picked in the printer setup dialog box.
    SetICMMode(hDC, ICM_OFF);

    doDIB = gBMPInfo[i].hDib;

    // Label the rows.

    TextOutW(hDC, 0, yoffset + dy/2, DIBLabels[i], wcslen(DIBLabels[i]));

    // First print with no ICM.
    DebugOut("ICMTEST : PRINT.C : Printing DIB #%d first \r\n", i);
    PrintDIB(doDIB, hDC, dx + xRes/4, yoffset, dx, dy);

    if (gfICMEnabled)
    {
      // Turn on ICM and see what happens.
      if (!SetICMMode(hDC, ICM_ON))
	OutputDebugString("PRINT.C : PrintDIBs : SetICMMode(ICM_ON) FAILED \r\n");
      DebugOut("ICMTEST : PRINT.C : Printing DIB #%d second \r\n", i);
      PrintDIB(doDIB, hDC, (2 * dx) + xRes/2, yoffset, dx, dy);

      SetColorSpace(hDC, ghlcsCurrent);
      DebugOut("ICMTEST : PRINT.C : Printing DIB #%d third \r\n", i);
      PrintDIB(doDIB, hDC, (3 * dx) + (3 * xRes/4), yoffset, dx, dy);

      // Create and set the scanner color space.
      hcsWeird = CreateColorSpace(&ScannerCS);
      if (!hcsWeird)
	OutputDebugString("PRINT.C : PrintDIBs : Create scanner space FAILED \r\n");
      SetColorSpace(hDC, hcsWeird);
      DebugOut("ICMTEST : PRINT.C : Printing DIB #%d fourth \r\n", i);
      PrintDIB(doDIB, hDC, (4 * dx) + xRes, yoffset, dx, dy);
      SetColorSpace(hDC, ghlcsDefault);
      DeleteColorSpace(hcsWeird);
    }
  }
}

//***************************************************************************
//                                                                          *
//  FUNCTION:  PrintDIB(HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy) *
//                                                                          *
//  PURPOSE:  Set the DIB bits to the printer DC.                           *
//                                                                          *
//***************************************************************************
void PrintDIB (HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy)
{
  LPBITMAPINFOHEADER lpbi;

  StartWait();	// put up busy cursor

  lpbi = (LPBITMAPINFOHEADER)hDIB;

  StretchDIBits(hDC,
		x, y,
		dx, dy,
		0, 0,
		(int)lpbi->biWidth, (ABS((int)lpbi->biHeight)),
		(LPSTR)((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)),
		(LPBITMAPINFO)lpbi,
		DIB_RGB_COLORS,
		SRCCOPY);
   EndWait();	// restore cursor
}

//***************************************************************************
//                                                                          *
//  FUNCTION:  PrintDIBF(HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy)*
//                                                                          *
//  PURPOSE:  Set the first half of the DIB bits to the printer DC.	    *
//                                                                          *
//***************************************************************************
void PrintDIBF (HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy)
{
  LPBITMAPINFOHEADER lpbi;

  lpbi = (LPBITMAPINFOHEADER)hDIB;

  StretchDIBits(hDC,
		x, y,
		dx, dy,
		0, 0,
		(int)lpbi->biWidth/2, (ABS((int)lpbi->biHeight)),
		(LPSTR)((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)),
		(LPBITMAPINFO)lpbi,
		DIB_RGB_COLORS,
		SRCCOPY);
}

//***************************************************************************
//                                                                          *
//  FUNCTION:  PrintDIBB(HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy)*
//                                                                          *
//  PURPOSE:  Set the back half of the DIB bits to the printer DC.	    *
//                                                                          *
//***************************************************************************
void PrintDIBB (HANDLE hDIB, HDC hDC, int x, int y, int dx, int dy)
{
  LPBITMAPINFOHEADER lpbi;

  lpbi = (LPBITMAPINFOHEADER)hDIB;

  StretchDIBits(hDC,
		x, y,
		dx, dy,
		(int)lpbi->biWidth/2, 0,
		(int)lpbi->biWidth/2, (ABS((int)lpbi->biHeight)),
		(LPSTR)((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)),
		(LPBITMAPINFO)lpbi,
		DIB_RGB_COLORS,
		SRCCOPY);
}

//***************************************************************************
//                                                                          *
//  FUNCTION: PrintImage(hDC, Page, Page)				    *
//                                                                          *
//  PURPOSE:  Compares the images with and without ICM. 		    *
//                                                                          *
//***************************************************************************
void PrintImage(HDC hDC, int FirstPage, int LastPage)
{
  HANDLE	    doDIB;
  BITMAPINFOHEADER  bi;
  int		    xSize, ySize, xRes, yRes, dx, dy;
  int		    i;
  HCOLORSPACE	    hcsWeird;

  // Obtain info about printer resolution
  xSize = GetDeviceCaps(hDC, HORZRES);
  ySize = GetDeviceCaps(hDC, VERTRES);
  xRes  = GetDeviceCaps(hDC, LOGPIXELSX);
  yRes  = GetDeviceCaps(hDC, LOGPIXELSY);

  for (i = FirstPage; i <= LastPage ; i++)
  {
    doDIB = gBMPInfo[i].hDib;
    bi = *(LPBITMAPINFOHEADER)doDIB;

    // Use half inch margins on top and bottom.
    // Maintain the same aspect ratio.
    dy = (ySize - (yRes))/2;
    dx = ((int)((long)dy * bi.biWidth/ABS(bi.biHeight)));

    // Make sure the image still fits.
    if (dx > xSize)
    {
      dx = xSize - xRes/2;
      dy = ((int)((long)dx * ABS(bi.biHeight)/bi.biWidth));
    }
    if (!(i == FirstPage))
      StartPage(hDC);
    // Override whatever the user has picked in the printer setup dialog box.
    SetICMMode(hDC, ICM_OFF);

    doDIB = gBMPInfo[i].hDib;

    // Label the images.

    if (pDevMode)
      TextOut(hDC, 0, 0, pDevMode->dmDeviceName,strlen(pDevMode->dmDeviceName));
    //TextOutW(hDC, 0, ySize/2 - LogFont.lfHeight, ImageLabelsW, wcslen(ImageLabelsW));
    TextOutW(hDC, 0, ySize/2 - LogFont.lfHeight,
	     ImageLabels[i - BMP_FACES][0],
	     wcslen(ImageLabels[i - BMP_FACES][0]));

    // First print with no ICM.
    DebugOut("ICMTEST : PRINT.C : Printing DIB #%d no ICM \r\n", i);
    PrintDIB(doDIB, hDC, (xSize - dx)/2, (ySize/2 - dy)/2, dx, dy);

    // Turn on ICM and see what happens.

    if (SetICMMode(hDC, ICM_ON))
    {
      TextOutW(hDC, 0, ySize - LogFont.lfHeight,
	       ImageLabels[i - BMP_FACES][1],
	       wcslen(ImageLabels[i - BMP_FACES][1]));
      // Create and set the scanner color space.
      hcsWeird = CreateColorSpace(&ScannerCS);
      SetColorSpace(hDC, hcsWeird);
      DebugOut("ICMTEST : PRINT.C : Printing DIB #%d with ICM \r\n", i);
      PrintDIB(doDIB, hDC, (xSize - dx)/2, (3*ySize/2 - dy)/2, dx, dy);
      SetColorSpace(hDC, ghlcsDefault);
      DeleteColorSpace(hcsWeird);
    }
    else
      OutputDebugString("PRINT.C : PrintICMImage : SetICMMode(ICM_ON) FAILED \r\n");

    if (!(i == LastPage))
      EndPage(hDC);
  }
}

//***************************************************************************
//                                                                          *
//  FUNCTION: PrintCalibrationGradients(hDC, Page)			    *
//                                                                          *
//  PURPOSE:  Prints the edge gradients and grey bar in long strips for     *
//  calibration of TRC curves.						    *
//                                                                          *
//***************************************************************************
void PrintCalibrationGradients(HDC hDC, int Page)
{
  HANDLE rd, bd, gd, cd, md, yd, bwd, rcd, gmd, byd;
  int	 xSize, ySize, xRes, yRes, dx, dy;
  int	 yOffset, yDelta;
  int	 i, j;

  rd  = gBMPInfo[BMP_RED_GRAD].hDib;
  bd  = gBMPInfo[BMP_BLUE_GRAD].hDib;
  gd  = gBMPInfo[BMP_GREEN_GRAD].hDib;
  cd  = gBMPInfo[BMP_CYAN_GRAD].hDib;
  md  = gBMPInfo[BMP_MAGENTA_GRAD].hDib;
  yd  = gBMPInfo[BMP_YELLOW_GRAD].hDib;
  bwd = gBMPInfo[BMP_BW_GRAD].hDib;
  rcd = gBMPInfo[BMP_RECY_GRAD].hDib;
  gmd = gBMPInfo[BMP_GRMA_GRAD].hDib;
  byd = gBMPInfo[BMP_BLYE_GRAD].hDib;

  // Obtain info about printer resolution.
  xSize = GetDeviceCaps(hDC, HORZRES);
  ySize = GetDeviceCaps(hDC, VERTRES);
  xRes  = GetDeviceCaps(hDC, LOGPIXELSX);
  yRes  = GetDeviceCaps(hDC, LOGPIXELSY);

  // There is 1 swatch across.
  // There are 10 swatches down, set 1/4 inch apart, 1/4 inch at top.
  dx = (xSize - xRes/2);
  dy = (ySize - (9*4 + 4)*yRes/16)/10;
  yOffset = yRes/4;
  yDelta  = dy + yRes/4;

  // Override whatever the user has picked in the printer setup dialog box.
  SetICMMode(hDC, ICM_OFF);

  // Print what printer is printing, and other pertinent information from
  //  the DevMode. Also list what color spaces are being used.

  if (Page == IDM_FILE_PRINT_TRC_1_GRADIENTS)
  {
    if (pDevMode)
      TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));

    PrintDIB(rd,  hDC, xRes/4, yOffset, 	   dx, dy);
    PrintDIB(cd,  hDC, xRes/4, yOffset + yDelta,   dx, dy);
    PrintDIB(gd,  hDC, xRes/4, yOffset + 2*yDelta, dx, dy);
    PrintDIB(md,  hDC, xRes/4, yOffset + 3*yDelta, dx, dy);
    PrintDIB(bd,  hDC, xRes/4, yOffset + 4*yDelta, dx, dy);
    PrintDIB(yd,  hDC, xRes/4, yOffset + 5*yDelta, dx, dy);
    PrintDIB(bwd, hDC, xRes/4, yOffset + 6*yDelta, dx, dy);
    for (j = 0; j <=6; j++)
    {
      for (i = 0; i <= 17; i++)
      {
	MoveToEx(hDC,  xRes/4 + (i * dx)/16, yOffset +	 (j)*yDelta + dy, 0);
	LineTo	(hDC,  xRes/4 + (i * dx)/16, yOffset + (j+1)*yDelta);
      }
    }
  }
  else
  {
    if (pDevMode)
      TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));

    PrintDIBF(rd,  hDC, xRes/4, yOffset,	    dx, dy);
    PrintDIBF(cd,  hDC, xRes/4, yOffset + yDelta,   dx, dy);
    PrintDIBF(gd,  hDC, xRes/4, yOffset + 2*yDelta, dx, dy);
    PrintDIBF(md,  hDC, xRes/4, yOffset + 3*yDelta, dx, dy);
    PrintDIBF(bd,  hDC, xRes/4, yOffset + 4*yDelta, dx, dy);
    PrintDIBF(yd,  hDC, xRes/4, yOffset + 5*yDelta, dx, dy);
    PrintDIBF(bwd, hDC, xRes/4, yOffset + 6*yDelta, dx, dy);
    for (j = 0; j <=6; j++)
    {
      for (i = 0; i <= 17; i++)
      {
	MoveToEx(hDC,  xRes/4 + (i * dx)/16, yOffset +	 (j)*yDelta + dy, 0);
	LineTo	(hDC,  xRes/4 + (i * dx)/16, yOffset + (j+1)*yDelta);
      }
    }
    EndPage(hDC);
    StartPage(hDC);

    // Override whatever the user has picked in the printer setup dialog box.
    // StartPage reinits the DC to have ICM on!
    SetICMMode(hDC, ICM_OFF);

    if (pDevMode)
      TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));

    PrintDIBB(rd,  hDC, xRes/4, yOffset,	    dx, dy);
    PrintDIBB(cd,  hDC, xRes/4, yOffset + yDelta,   dx, dy);
    PrintDIBB(gd,  hDC, xRes/4, yOffset + 2*yDelta, dx, dy);
    PrintDIBB(md,  hDC, xRes/4, yOffset + 3*yDelta, dx, dy);
    PrintDIBB(bd,  hDC, xRes/4, yOffset + 4*yDelta, dx, dy);
    PrintDIBB(yd,  hDC, xRes/4, yOffset + 5*yDelta, dx, dy);
    PrintDIBB(bwd, hDC, xRes/4, yOffset + 6*yDelta, dx, dy);
    for (j = 0; j <=6; j++)
    {
      for (i = 0; i <= 17; i++)
      {
	MoveToEx(hDC,  xRes/4 + (i * dx)/16, yOffset +	 (j)*yDelta + dy, 0);
	LineTo	(hDC,  xRes/4 + (i * dx)/16, yOffset + (j+1)*yDelta);
      }
    }
  }
}

//***************************************************************************
//                                                                          *
//  FUNCTION: PrintEdgeGradients(hDC)					    *
//                                                                          *
//  PURPOSE:  Compares gradients along the edge of the color cube	    *
//  with and without ICM.						    *
//                                                                          *
//***************************************************************************
void PrintEdgeGradients(HDC hDC)
{
  HANDLE	rd, bd, gd, cd, md, yd;
  int		xSize, ySize, xRes, yRes, dx, dy;
  int		i, yOffset, yDelta;
  HCOLORSPACE	hcsScreen;
  HCOLORSPACE	hcsScanner;

  rd = gBMPInfo[BMP_RED_GRAD].hDib;
  bd = gBMPInfo[BMP_BLUE_GRAD].hDib;
  gd = gBMPInfo[BMP_GREEN_GRAD].hDib;
  cd = gBMPInfo[BMP_CYAN_GRAD].hDib;
  md = gBMPInfo[BMP_MAGENTA_GRAD].hDib;
  yd = gBMPInfo[BMP_YELLOW_GRAD].hDib;

  // Obtain info about printer resolution.
  xSize = GetDeviceCaps(hDC, HORZRES);
  ySize = GetDeviceCaps(hDC, VERTRES);
  xRes  = GetDeviceCaps(hDC, LOGPIXELSX);
  yRes  = GetDeviceCaps(hDC, LOGPIXELSY);

  // There are 2 swatches across, each separated by 1/4 inch.
  // There are 3 sets of 4 swatches down, set 1/2 inch apart, 1/4 inch at top.
  dx = (xSize - xRes/4)/2;
  dy = (ySize - (9*3 + 2*8 + 4)*yRes/16)/12;
  yOffset = yRes/4;
  yDelta  = 4*dy + 3*3*yRes/16 + yRes/2;

  // Create the screen color space.
  hcsScreen = CreateColorSpace(&ScreenCS);

  // Create the scanner color space.
  hcsScanner = CreateColorSpace(&ScannerCS);

  // Override whatever the user has picked in the printer setup dialog box.
  SetICMMode(hDC, ICM_OFF);

  // Print what printer is printing, and other pertinent information from
  //  the DevMode. Also list what color spaces are being used.

  if (pDevMode)
    TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));
  TextOut(hDC, 0, yOffset +    dy + 3*yRes/16  - LogFont.lfHeight,
		  "No ICM", strlen("No ICM"));
  TextOut(hDC, 0, yOffset + 2*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Default Color Space", strlen("Default Color Space"));
  TextOut(hDC, 0, yOffset + 3*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Screen Color Space", strlen("Screen Color Space"));
  TextOut(hDC, 0, yOffset + 4*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Scanner Color Space", strlen("Scanner Color Space"));

  PrintDIB(rd, hDC, 0,			yOffset,	    dx, dy);
  PrintDIB(cd, hDC, (xSize/2 + xRes/8), yOffset,	    dx, dy);
  PrintDIB(gd, hDC, 0,			yOffset + yDelta,   dx, dy);
  PrintDIB(md, hDC, (xSize/2 + xRes/8), yOffset + yDelta,   dx, dy);
  PrintDIB(bd, hDC, 0,			yOffset + 2*yDelta, dx, dy);
  PrintDIB(yd, hDC, (xSize/2 + xRes/8), yOffset + 2*yDelta, dx, dy);

  // Turn on ICM and see what happens.
  if (SetICMMode(hDC, ICM_ON))
  {
    SetColorSpace(hDC, ghlcsCurrent);
    for (i = 0, yOffset = dy + 7*yRes/16; i < 3; yOffset += dy + 3*yRes/16, i++)
    //for (i = 0, yOffset = dy + 7*yRes/16; i < 1; yOffset += dy + 3*yRes/16, i++)
    //for (i = 1, yOffset = (dy + 7*yRes/16) + (dy + 3*yRes/16); i < 3; yOffset += dy + 3*yRes/16, i++)
    {
      PrintDIB(rd, hDC, 0,		    yOffset,		dx, dy);
      PrintDIB(cd, hDC, (xSize/2 + xRes/8), yOffset,		dx, dy);
      PrintDIB(gd, hDC, 0,		    yOffset + yDelta,	dx, dy);
      PrintDIB(md, hDC, (xSize/2 + xRes/8), yOffset + yDelta,	dx, dy);
      PrintDIB(bd, hDC, 0,		    yOffset + 2*yDelta, dx, dy);
      PrintDIB(yd, hDC, (xSize/2 + xRes/8), yOffset + 2*yDelta, dx, dy);
      if (i == 0)
	SetColorSpace(hDC, hcsScreen);
      else if (i == 1)
	SetColorSpace(hDC, hcsScanner);
    }
  }
  else
  {
    OutputDebugString("PRINT.C : PrintICMImage : SetICMMode(ICM_ON) FAILED \r\n");
  }
  SetColorSpace(hDC, ghlcsDefault);
  DeleteColorSpace(hcsScreen);
  DeleteColorSpace(hcsScanner);
}


//***************************************************************************
//                                                                          *
//  FUNCTION: PrintDiagonalGradients(hDC)				    *
//                                                                          *
//  PURPOSE:  Compares gradients along the diagonals of the color	    *
//  cube with and without ICM.						    *
//                                                                          *
//***************************************************************************
void PrintDiagonalGradients(HDC hDC)
{
  HANDLE	bwd, recyd, grmad, blyed;
  int		xSize, ySize, xRes, yRes, dx, dy;
  int		i, yOffset, yDelta;
  HCOLORSPACE	hcsScreen;
  HCOLORSPACE	hcsScanner;

  bwd	= gBMPInfo[BMP_BW_GRAD].hDib;
  recyd = gBMPInfo[BMP_RECY_GRAD].hDib;
  grmad = gBMPInfo[BMP_GRMA_GRAD].hDib;
  blyed = gBMPInfo[BMP_BLYE_GRAD].hDib;

  // Obtain info about printer resolution.
  xSize = GetDeviceCaps(hDC, HORZRES);
  ySize = GetDeviceCaps(hDC, VERTRES);
  xRes  = GetDeviceCaps(hDC, LOGPIXELSX);
  yRes  = GetDeviceCaps(hDC, LOGPIXELSY);

  // Use same spacing as for edge gradients.
  // There are 2 swatches across, each separated by 1/4 inch.
  // There are 3 sets of 4 swatches down, set 1/2 inch apart, 1/4 inch at top.
  dx = (xSize - xRes/4)/2;
  dy = (ySize - (9*3 + 2*8 + 4)*yRes/16)/12;
  yOffset = yRes/4;
  yDelta  = 4*dy + 3*3*yRes/16 + yRes/2;

  // Create the screen color space.
  hcsScreen = CreateColorSpace(&ScreenCS);

  // Create the scanner color space.
  hcsScanner = CreateColorSpace(&ScannerCS);

  // Override whatever the user has picked in the printer setup dialog box.
  SetICMMode(hDC, ICM_OFF);

  // Print what printer is printing, and other pertinent information from
  //  the DevMode. Also list what color spaces are being used.

  if (pDevMode)
    TextOut(hDC, 0, 0, pDevMode->dmDeviceName, strlen(pDevMode->dmDeviceName));
  TextOut(hDC, 0, yOffset +    dy + 3*yRes/16  - LogFont.lfHeight,
		  "No ICM", strlen("No ICM"));
  TextOut(hDC, 0, yOffset + 2*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Default Color Space", strlen("Default Color Space"));
  TextOut(hDC, 0, yOffset + 3*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Screen Color Space", strlen("Screen Color Space"));
  TextOut(hDC, 0, yOffset + 4*(dy + 3*yRes/16) - LogFont.lfHeight,
		  "Scanner Color Space", strlen("Scanner Color Space"));

  PrintDIB(bwd,   hDC, 0,		   yOffset,	       dx, dy);
  PrintDIB(recyd, hDC, (xSize/2 + xRes/8), yOffset,	       dx, dy);
  PrintDIB(grmad, hDC, 0,		   yOffset + yDelta,   dx, dy);
  PrintDIB(blyed, hDC, (xSize/2 + xRes/8), yOffset + yDelta,   dx, dy);

  // Turn on ICM and see what happens.
  if (SetICMMode(hDC, ICM_ON))
  {
    SetColorSpace(hDC, ghlcsCurrent);
    for (i = 0, yOffset = dy + 7*yRes/16; i < 3; yOffset += dy + 3*yRes/16, i++)
    {
      PrintDIB(bwd,   hDC, 0,		       yOffset, 	   dx, dy);
      PrintDIB(recyd, hDC, (xSize/2 + xRes/8), yOffset, 	   dx, dy);
      PrintDIB(grmad, hDC, 0,		       yOffset + yDelta,   dx, dy);
      PrintDIB(blyed, hDC, (xSize/2 + xRes/8), yOffset + yDelta,   dx, dy);
      if (i == 0)
	SetColorSpace(hDC, hcsScreen);
      else if (i == 1)
	SetColorSpace(hDC, hcsScanner);
    }
  }
  else
  {
    OutputDebugString("PRINT.C : PrintICMImage : SetICMMode(ICM_ON) FAILED \r\n");
  }
  SetColorSpace(hDC, ghlcsDefault);
  DeleteColorSpace(hcsScreen);
  DeleteColorSpace(hcsScanner);
}


//***************************************************************************
//                                                                          *
//  FUNCTION:  GetPrinterDC()						    *
//                                                                          *
//  PURPOSE:  Read Registry.ini for default printer and create DC for it.   *
//                                                                          *
//  PARAMETERS: None                                                        *
//                                                                          *
//  RETURNS:  A handle to the DC if successful or NULL otherwise.           *
//                                                                          *
//***************************************************************************
HDC PASCAL GetPrinterDC()
{
  HDC    hDC;
  LPSTR   lpszPrinter;
  HANDLE  hString;

    PRINTDLG pd = {0};

    pd.lStructSize = sizeof(pd);
    pd.hwndOwner   = 0;
    pd.hDevMode    = 0;
    pd.hDevNames   = 0;
    pd.Flags = PD_RETURNDEFAULT;

    if (PrintDlg(&pd))
    {
	hDevMode = pd.hDevMode;
	pDevMode = GlobalLock(hDevMode);
        hDevNames = pd.hDevNames;
    }

  // Enumerating printers may take time--display wait cursor
  StartWait();

  // Init vars
  hDC = NULL;
  lpszPrinter = NULL;
  hString = NULL;
  
  // Get Name of default printer from registry
  lpszPrinter = GetDefaultPrinter(&hString);

  if(lpszPrinter != NULL)
  {
    // Now get a DC for the printer
    hDC = CreateDC (NULL, lpszPrinter, NULL, NULL);
    GlobalFree(hString);
  }
  
  // Restore cursor and return
  EndWait();
  return(hDC);
}

//***************************************************************************
//                                                                          *
//  FUNCTION:  InitPrinting                                                 *
//                                                                          *
//  PURPOSE:  Makes preliminary driver calls to set up print job.           *
//                                                                          *
//  PARAMETERS:                                                             *
//      HDC     Handle to printer DC                                        *
//      HWND    Handle to the window with the image                         *
//      HANDLE  Instance handle                                             *
//      LPSTR   Title of image                                              *
//                                                                          *
//                                                                          *
//  RETURNS:  TRUE  - if successful.                                        *
//    FALSE - otherwise.                                                    *
//                                                                          *
//***************************************************************************
BOOL PASCAL InitPrinting(HDC hDC, HWND hWnd, HANDLE hInst, LPSTR lpszFriendlyName)
{
  BOOL    bRetVal;
  DOCINFO diDocInfo;  // Document info for StartDoc call

  if (!gBMPInfo[gwBMPIndex].hDib)
    return(FALSE);

  //
  // Init vars
  //
  bRetVal     = TRUE;  // assume success
  gbUserAbort = FALSE;  // user hasn't aborted
  ghWndParent = hWnd;  // save for Enable at Term time

  //
  // Get addresses of dlg procs
  //
  glpfnPrintDlgProc = (FARPROC)&PrintDlgProc;
  glpfnAbortProc    = (FARPROC)&AbortProc;

  //
  // Create the printing dialog
  //
  ghDlgPrint = CreateDialog (hInst, "DLG_PRINT", ghWndParent, (DLGPROC)glpfnPrintDlgProc);

  if (!ghDlgPrint)
  {
    return(FALSE);
  }

  SetWindowText (ghDlgPrint, lpszFriendlyName);
  EnableWindow (ghWndParent, FALSE);        // disable parent   

  //
  // Set the abort procedure
  //
  if (SetAbortProc(hDC, (ABORTPROC)glpfnAbortProc) == SP_ERROR)
  {
    ErrMsg("InitPrinting:  SetAbortProc FAILED");
    bRetVal = FALSE;
    goto exit;
  }
  
  //
  // Fill in the DOCINFO structure
  //
  diDocInfo.cbSize = sizeof(DOCINFO);
  diDocInfo.lpszDocName = lpszFriendlyName;
  diDocInfo.lpszOutput = NULL;
  diDocInfo.lpszDatatype = NULL;
  diDocInfo.fwType = 0;

  //
  // Start the document
  //
  if (StartDoc(hDC, &diDocInfo) == SP_ERROR)
  {
    ErrMsg("InitPrinting:  StartDoc FAILED");
    bRetVal = FALSE;
    goto exit;
  }

  //
  // Start the page
  //
  if (StartPage(hDC) == SP_ERROR)
  {
    ErrMsg("InitPrinting:  StartPage FAILED");
    AbortDoc(hDC);
    bRetVal = FALSE;
  }

exit:
  if (bRetVal == FALSE)
  {
    EnableWindow(ghWndParent, TRUE);
    DestroyWindow(ghDlgPrint);
  }
  return(bRetVal);
}

//***************************************************************************
//                                                                          *
//  FUNCTION:  TermPrinting(HDC hDC)                                        *
//                                                                          *
//  PURPOSE:  Terminates print job.                                         *
//                                                                          *
//***************************************************************************
void PASCAL TermPrinting(HDC hDC)
{
  if (!gbUserAbort)
  {
    //
    // If the user didn't abort, end the doc
    //
    if (EndPage(hDC) == SP_ERROR)
    {
      ErrMsg("TermPrinting:  EndPage FAILED");
    }
    if (EndDoc(hDC) == SP_ERROR)
    {
      ErrMsg("TermPrinting:  EndDoc FAILED");
    }
    //
    // Destroy the DevMode and DevNames
    //
    if (hDevMode)
    {
       GlobalUnlock(hDevMode);
       GlobalFree(hDevMode);
       hDevMode = 0;
       pDevMode = 0;
    }
    if (hDevNames)
    {
       GlobalFree(hDevNames);
       hDevNames = 0;
    }
    //
    // Destroy the dialog
    //
    EnableWindow(ghWndParent, TRUE);
    DestroyWindow(ghDlgPrint);
  }
  else 
  {
    if (AbortDoc(hDC) == SP_ERROR)
    {
      ErrMsg("TermPrinting:  AbortDoc FAILED");
    }
  }
}

// **************************************************************************
//                                                                          *
//  FUNCTION:  PrintDlgProc (HWND, unsigned , WORD , DWORD )                *
//                                                                          *
//  PURPOSE:  Dialog function for the "Cancel Printing" dialog. It sets     *
//              the abort flag if the user presses <Cancel>.                *
//                                                                          *
// **************************************************************************
BOOL FAR PASCAL PrintDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, DWORD lParam)
{
  switch (iMessage) 
  {
    case WM_INITDIALOG:
      EnableMenuItem (GetSystemMenu (hDlg, FALSE), SC_CLOSE, MF_GRAYED);
      break;

    case WM_COMMAND:
      gbUserAbort = TRUE;
      EnableWindow (ghWndParent, TRUE);
      DestroyWindow (hDlg);
      ghDlgPrint = 0;
      break;

    default:
      return(FALSE);
  }
  return(TRUE);
}

// **************************************************************************
//                                                                          *
//  FUNCTION:  AbortProc (HDC hPrnDC, short nCode)                          *
//                                                                          *
//  PURPOSE:  Checks message queue for messages from the "Cancel Printing"  *
//              dialog. If it sees a message, (this will be from a print    *
//              cancel command), it terminates.                             *
//                                                                          *
//  RETURNS:  Inverse of Abort flag                                         *
//                                                                          *
// **************************************************************************
BOOL FAR PASCAL AbortProc (HDC hPrnDC, short nCode)
{
    MSG   msg;

    while (!gbUserAbort && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) 
    {
      if (!ghDlgPrint || !IsDialogMessage(ghDlgPrint, &msg)) 
      {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
      }
    }
    return(!gbUserAbort);
}

// *****************************************************************************
//
// FunctionName: GetDefaultPrinter()
// 
// Purpose:
//    The purpose of this function is to 
//
// Parameters:
//    None.
//
// Return Value:  
//    LPSTR    NULL - Failed.
//
// Comments:
//
// *****************************************************************************

LPSTR GetDefaultPrinter(LPHANDLE lphString)

{
  LPSTR              lpszDefaultPrinter;
  DWORD              dwPrinters     = 0;
  DWORD              dwIndex        = 0;
  HANDLE             hPrinterInfo   = NULL;
  LPPRINTER_INFO_2   lpPrinterInfo  = NULL;

  //
  // Enumerate Printers.
  //
  dwPrinters = EnumerateAllPrinters(PRINTER_ENUM_LOCAL, NULL, LEVEL2, &hPrinterInfo);
  if ((dwPrinters >= ENUM_ERROR) ||   (dwPrinters == 0))
    return(NULL);

  lpszDefaultPrinter = *lphString = (LPSTR)GlobalAlloc(GPTR, MAX_STRING);
  if (lpszDefaultPrinter == NULL)
    return(NULL);

  lpPrinterInfo = (LPPRINTER_INFO_2)hPrinterInfo;

  if (lpPrinterInfo != NULL)
  {
    //
    // Loop through printers and find default.
    //
    for (dwIndex = 0; dwIndex < dwPrinters; dwIndex++)
    {
      if (lpPrinterInfo[dwIndex].Attributes & PRINTER_ATTRIBUTE_DEFAULT)
      lstrcpy(lpszDefaultPrinter, lpPrinterInfo[dwIndex].pPrinterName);
    }
  }

  //
  // Free Memory.
  //
  GlobalFree(hPrinterInfo);

  return(lpszDefaultPrinter);
}


// *****************************************************************************
//
// FunctionName: EnumerateAllPrinters()
// 
// Purpose:
//    The purpose of this function is to Enumerate and return an array of the 
//    appropriate structure(s) given by dwLevel.
//
// Parameters:
//    DWORD    dwType         - type of printer object to Enumerate.
//    LPSTR    lpszName       - name of printer object.
//    DWORD    dwLevel        - structure level.
//    LPHANDLE lphPrinterInfo - handle which points to array of PRINTER_INFO_X.
//
// Return Value:  
//    DWORD    00000000 - 7FFFFFFF - Success.
//             7FFFFFFF - FFFFFFFF - Fail.
//
// Comments:
//
// *****************************************************************************

DWORD EnumerateAllPrinters(DWORD dwType, LPSTR lpszName, DWORD dwLevel, LPHANDLE lphPrinterInfo)

{
  DWORD        dwSize      = 0;
  DWORD        dwPrinters  = 0;
  DWORD        dwNeeded    = 0;
  DWORD        dwErrorCode = 0;
  BOOL         bReturnCode = 0;
  BOOL         bRC         = FALSE;
  LPBYTE       lpInfo      = NULL;
     
  //
  // Set handle to NULL.
  //
  *lphPrinterInfo = NULL;

  //
  // Enumerate Printers.
  //
  bReturnCode = EnumPrinters(dwType, lpszName, dwLevel, NULL, 0, &dwSize, &dwPrinters);

  //
  // If Return Code is TRUE the first time, we know there is 
  // nothing to enumerate.
  //
  if (bReturnCode)
  {
    dwPrinters = 0L;
    return(dwPrinters);
  }

  //
  // Since Return Code is FALSE, check LastError.
  // If LastError is any thing other than allocate size error, flag and exit.
  //
  dwErrorCode = GetLastError();
  if (dwErrorCode != ERROR_INSUFFICIENT_BUFFER)
    return(ERROR_ENUMPRINTERS);

  //
  // Loop until we have size right.
  //
  while (!bRC)
  {
    if (lpInfo = *lphPrinterInfo = GlobalAlloc(GPTR, dwSize))
    {
      //
      // Enumerate
      //
      bRC = EnumPrinters(dwType, lpszName, dwLevel, lpInfo, dwSize, &dwNeeded, &dwPrinters);
      
      if (!bRC)
      {
        dwErrorCode = GetLastError();

        //
        // If any thing other than allocate size error, flag and exit.
        //
        if (dwErrorCode != ERROR_INSUFFICIENT_BUFFER)
          return(ERROR_ENUMPRINTERS);
        else
        {
	  GlobalFree(*lphPrinterInfo);
          dwSize = dwNeeded; 
        }
      }
    }
    else
      return(ERROR_ENUMPRINTERS);
  }
  return(dwPrinters);
}
