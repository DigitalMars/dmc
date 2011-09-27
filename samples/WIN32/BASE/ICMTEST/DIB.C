// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright © 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  PROGRAM:  dib.c
//
//  PURPOSE:  Processes the creation and manipulation of the DIBs in
//            ICMTEST.
//
//  PLATFORMS:  Chicago
//
//

#include <windows.h>  // required for all Windows applications
#include <string.h>
#include "dib.h"      // specific to this file
#include "icmtest.h"  // specific to this program

//**********************************************************************
//
//  Global vars external to this module
//
//**********************************************************************

extern HPALETTE     ghpalCurrent;
extern WORD         gwBMPIndex;
extern BMPINFO      gBMPInfo[MAX_BMP];      

extern HANDLE       ghTempBits;
extern HCURSOR      ghcurSave;
extern BOOL         gfDoICM;  
extern HCOLORSPACE  ghlcsCurrent;
extern HCOLORSPACE  ghlcsDefault;
extern DWORD        gdwGamutMethod;
extern HDC          ghdcCompat;

//**********************************************************************
//
//  FUNCTION: InitDIBs
//
//  PURPOSE:
//    Fills in the names of the bitmaps in the gBMPInfo structure,
//    initializes handle values to NULL, and calls InitIndividalDib
//    for each file.
//
//  PARAMETERS:
//    None.
//
//  RETURN VALUE:  
//    TRUE if bitmaps were initialized properly, FALSE otherwise.
//
//  COMMENTS:
//    !
//
//**********************************************************************

BOOL  InitDIBs()
{
  UINT    uiCount;
  
  // Initialize file names
  strcpy(gBMPInfo[0].szBMP,"BADFILE.BMP");
  strcpy(gBMPInfo[BMP_1].szBMP, "1bcolor.bmp");
  strcpy(gBMPInfo[BMP_4].szBMP, "4bcolor.bmp");
  strcpy(gBMPInfo[BMP_8].szBMP, "256color.bmp");  
  strcpy(gBMPInfo[BMP_16].szBMP, "16bcolor.bmp");
  strcpy(gBMPInfo[BMP_24].szBMP, "24bcolor.bmp");
  strcpy(gBMPInfo[BMP_555].szBMP, "555color.bmp");
  strcpy(gBMPInfo[BMP_565].szBMP, "565color.bmp");
  strcpy(gBMPInfo[BMP_32].szBMP, "32bcolor.bmp");
  strcpy(gBMPInfo[BMP_888].szBMP, "rgbcolor.bmp");
  strcpy(gBMPInfo[BMP_8_DOWN].szBMP, "8bdcolor.bmp");
  strcpy(gBMPInfo[BMP_1_ICM].szBMP, "1bcolor.ibm");
  strcpy(gBMPInfo[BMP_4_ICM].szBMP, "4bcolor.ibm");
  strcpy(gBMPInfo[BMP_8_ICM].szBMP, "8bcolor.ibm");
  strcpy(gBMPInfo[BMP_16_ICM].szBMP, "16bcolor.ibm");
  strcpy(gBMPInfo[BMP_24_ICM].szBMP, "24bcolor.ibm");
  strcpy(gBMPInfo[BMP_555_ICM].szBMP, "555color.ibm");
  strcpy(gBMPInfo[BMP_565_ICM].szBMP, "565color.ibm");
  strcpy(gBMPInfo[BMP_32_ICM].szBMP, "32bcolor.ibm");
  strcpy(gBMPInfo[BMP_888_ICM].szBMP, "rgbcolor.ibm");
  strcpy(gBMPInfo[BMP_CMYK_ICM].szBMP, "cmycolor.ibm");
  strcpy(gBMPInfo[BMP_FACES].szBMP, "faces.bmp");
  strcpy(gBMPInfo[BMP_SUNSET].szBMP, "sunset.bmp");
  strcpy(gBMPInfo[BMP_CHURCH].szBMP, "church.bmp");
  strcpy(gBMPInfo[BMP_REDBUSH].szBMP, "redbush.bmp");
  strcpy(gBMPInfo[BMP_RED_GRAD].szBMP, "redgrad.bmp");
  strcpy(gBMPInfo[BMP_GREEN_GRAD].szBMP, "grengrad.bmp");
  strcpy(gBMPInfo[BMP_BLUE_GRAD].szBMP, "bluegrad.bmp");
  strcpy(gBMPInfo[BMP_CYAN_GRAD].szBMP, "cyangrad.bmp");
  strcpy(gBMPInfo[BMP_MAGENTA_GRAD].szBMP, "magegrad.bmp");
  strcpy(gBMPInfo[BMP_YELLOW_GRAD].szBMP, "yellgrad.bmp");
  strcpy(gBMPInfo[BMP_BW_GRAD].szBMP, "bwgrad.bmp");
  strcpy(gBMPInfo[BMP_RECY_GRAD].szBMP, "recygrad.bmp");
  strcpy(gBMPInfo[BMP_GRMA_GRAD].szBMP, "grmagrad.bmp");
  strcpy(gBMPInfo[BMP_BLYE_GRAD].szBMP, "blyegrad.bmp");
  strcpy(gBMPInfo[BMP_24_BIG].szBMP, "24bbig.bmp");

  // Set all handles to NULL
  for (uiCount=0;uiCount < MAX_BMP; uiCount++)
  {
    gBMPInfo[uiCount].hDib = NULL;
    gBMPInfo[uiCount].hBmp = NULL;
  }

  // Initialize each individual dib file
  for (uiCount=1; uiCount < MAX_BMP; uiCount++)
  {
    InitIndividualDIB(uiCount);
  }

  // At the very least we want the standard 8-bit DIB to be around.
  if (gBMPInfo[BMP_8].hDib == NULL)
  {
    FreeDIBs();
    return(FALSE);
  }
  return(TRUE);
}


//*************************************************************************
//
//  FUNCTION: InitIndividualDIB
//
//  PURPOSE:
//    Reads a DIB from a file, obtains a handle to it's          
//    BITMAPINFO struct., sets up the palette and loads the DIB. 
//
//  PARAMETERS:
//    UINT  Index referring to which BMP to initialize.
//
//  RETURN VALUE:  
//    TRUE if DIB initialized successfully
//    FALSE otherwise
//
//  COMMENTS:
//
//*************************************************************************

BOOL InitIndividualDIB(UINT uiBMPIndex)
{
  LPBITMAPINFOHEADER  lpbi;
  BYTE FAR        *lpv1;
  BYTE FAR        *lpv2;

  DWORD         i;

  // Open the DIB, copy it into memory, and get a handle to BITMAPINFO
  lpbi = (LPBITMAPINFOHEADER)OpenDIB((LPSTR)gBMPInfo[uiBMPIndex].szBMP);
  gBMPInfo[uiBMPIndex].hDib = (HANDLE)lpbi;

  if (lpbi == NULL)
  {
    ErrMsg("InitIndividualDIB:  %s is not a Legitimate DIB File!", 
        (LPSTR)gBMPInfo[uiBMPIndex].szBMP);
    return(FALSE);
  }
  else
  {
    DebugOut("ICMTEST : DIB.C : InitIndividualDIB : %s initialized\r\n",
      gBMPInfo[uiBMPIndex].szBMP);
      gBMPInfo[uiBMPIndex].hDib = (HANDLE)lpbi;
  }

  //  If the DIB is RLE, exit gracefully NOW
  if (((lpbi->biCompression != BI_RGB)
       &&(lpbi->biCompression != BI_BITFIELDS)))
  {
    ErrMsg("InitIndividualDIB:  %s biCompression = %lu",
      (LPSTR)gBMPInfo[uiBMPIndex].szBMP, lpbi->biCompression);
    return(FALSE);
  }

  if(uiBMPIndex == BMP_8)
  {
    // Set up the palette
    ghpalCurrent = CreateDibPalette(lpbi);
    if (ghpalCurrent == NULL)
    {
      ErrMsg("InitDIB:  CreateDibPalette() Failed");
      return(FALSE);
    }
  }

  // Get DDBs from the DIBs
  gBMPInfo[uiBMPIndex].hBmp  = BitmapFromDib(lpbi,ghpalCurrent);
  if(!gBMPInfo[uiBMPIndex].hBmp)
  {
    ErrMsg("Could not create DDB for %s", (LPSTR)gBMPInfo[uiBMPIndex].szBMP);
    return(FALSE);
  }

  // Get a temp buffer for out of gamut checking
  if(uiBMPIndex == BMP_24_BIG)
  {
    ghTempBits = GlobalAlloc(GPTR, GlobalSize(lpbi));
    lpv1=(BYTE FAR *)lpbi;
    lpv2=(BYTE FAR *)ghTempBits;
    for (i=0; i < GlobalSize(lpbi); i++)
      *lpv2++ = *lpv1++;
  }
  return(TRUE);
}

//*************************************************************************
//
//  FUNCTION: ReadDibBitmapInfo
//
//  PURPOSE:
//    Will read a file in DIB format and return a global HANDLE  
//    to it's BITMAPINFO.  This function will work with both     
//    "old" (BITMAPCOREHEADER) and "new" (BITMAPINFOHEADER)      
//    bitmap formats, but will always return a "new" BITMAPINFO  
//
//  PARAMETERS:
//    int   File Handle
//
//  RETURN VALUE:  
//    A handle to the BITMAPINFO of the DIB in the file.
//
//  COMMENTS:
//    !
//
//  HISTORY:
//
//*************************************************************************

HANDLE ReadDibBitmapInfo (int fh)
{
  DWORD     off;
  HANDLE    hbi = NULL;
  int       size;
  int       i;
  WORD      nNumColors;

  RGBQUAD FAR       *pRgb;
  BITMAPINFOHEADER   bi;
  BITMAPCOREHEADER   bc;
  BITMAPFILEHEADER   bf;
  LPBITMAPINFOHEADER lpbi;
  DWORD              dwWidth = 0;
  DWORD              dwHeight = 0;
  WORD               wPlanes, wBitCount;

  // Reset file pointer and read file header   
  off = _llseek(fh, 0L, SEEK_CUR);
  if (sizeof (bf) != _lread (fh, (LPSTR)&bf, sizeof (bf)))
      return(NULL);

  // Do we have a RC HEADER?   
  if (!ISDIB (bf.bfType))
  {
    bf.bfOffBits = 0L;
    _llseek (fh, off, SEEK_SET);
  }
  if (sizeof (bi) != _lread (fh, (LPSTR)&bi, sizeof(bi)))
    return(NULL);

  nNumColors = DibNumColors(&bi);

  // Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
  // and extract the field information accordingly. If a BITMAPCOREHEADER,
  // transfer its field information to a BITMAPINFOHEADER-style block
  switch (size = (int)bi.biSize)
  {
    case sizeof(BITMAPINFOHEADER):
      break;

    case sizeof(BITMAPV4HEADER):
      break;

    case sizeof(BITMAPCOREHEADER):
  
      bc = *(BITMAPCOREHEADER*)&bi;

      dwWidth   = (DWORD)bc.bcWidth;
      dwHeight  = (DWORD)bc.bcHeight;
      wPlanes   = bc.bcPlanes;
      wBitCount = bc.bcBitCount;

      bi.biSize               = sizeof(BITMAPINFOHEADER);
      bi.biWidth              = dwWidth;
      bi.biHeight             = dwHeight;
      bi.biPlanes             = wPlanes;
      bi.biBitCount           = wBitCount;

      bi.biCompression        = BI_RGB;
      bi.biSizeImage          = 0;
      bi.biXPelsPerMeter      = 0;
      bi.biYPelsPerMeter      = 0;
      bi.biClrUsed            = nNumColors;
      bi.biClrImportant       = nNumColors;

      _llseek (fh, (LONG)sizeof(BITMAPCOREHEADER) - sizeof(BITMAPINFOHEADER), SEEK_CUR);
      break;

    default:
      // Not a DIB! 
      return(NULL);
  }

  // Fill in some default values if they are zero   
  if (bi.biSizeImage == 0)
  {
    bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount) * ABS(bi.biHeight);
  }
  if (bi.biClrUsed == 0)
    bi.biClrUsed = nNumColors;

  // Allocate for the BITMAPINFO structure and the color table.   
  hbi = GlobalAlloc(GPTR, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD)
            + 3 * sizeof(DWORD));
  if (!hbi)
    return(NULL);
  lpbi = (VOID FAR *)hbi;
  *lpbi = bi;

  // Get a pointer to the end of the infoheader
  pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + sizeof(BITMAPINFOHEADER));

  //If 4.0 header then get the whole thing.
  if (size == sizeof(BITMAPV4HEADER))
  {
    _lread (fh, (LPSTR)pRgb, sizeof(BITMAPV4HEADER) - sizeof(BITMAPINFOHEADER));
    (DWORD)pRgb += (sizeof(BITMAPV4HEADER) - sizeof(BITMAPINFOHEADER));
  }
  else  // copy over the mask bits as well
  {
    if (bi.biCompression == BI_BITFIELDS)
    {
      _lread (fh, (LPSTR)pRgb, 3 * sizeof(DWORD));
      (DWORD)pRgb += 3 * sizeof(DWORD);
    }
  }

  //pRgb should now be pointing at the color table
  if (nNumColors)
  {
    if (size == sizeof(BITMAPCOREHEADER))
    {
      // Convert a old color table (3 byte RGBTRIPLEs) to a new
      // color table (4 byte RGBQUADs)
      _lread (fh, (LPSTR)pRgb, nNumColors * sizeof(RGBTRIPLE));

      for (i = nNumColors - 1; i >= 0; i--)
      {
        RGBQUAD rgb;

        rgb.rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
        rgb.rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
        rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
        rgb.rgbReserved = (BYTE)0;

        pRgb[i] = rgb;
      }
    }
    else
    {
      _lread(fh,(LPSTR)pRgb,nNumColors * sizeof(RGBQUAD));
    }
  }

  return(hbi);
}

//*************************************************************************
//                                                                          
//  FUNCTION   : ErrMsg (LPSTR sz,...)                                      
//                                                                          
//  PURPOSE    : Opens a Message box with a error message in it.The user can
//               select the OK button to continue                           
//                                                                          
//  RETURNS    : FALSE to indicate an error has occured.                    
//                                                                          
//*************************************************************************

int ErrMsg (PSTR sz,...)
{
  char ach[128];

  wvsprintf (ach, sz, (LPSTR)(&sz+1));   // Format the string
  MessageBox (NULL, ach, "DOH!", MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL);
  return(FALSE);
}

//*************************************************************************
//                                                                          
//  FUNCTION   : CreateDibPalette(LPBITMAPINFOHEADER lpbi)
//                                                                          
//  PURPOSE    : Given a Pointer to a BITMAPINFO struct will create a       
//               a GDI palette object from the color table.                 
//                                                                          
//  RETURNS    : A handle to the palette.                                   
//                                                                          
//*************************************************************************

HPALETTE CreateDibPalette(LPBITMAPINFOHEADER lpbi)
{
  LOGPALETTE   *pPal;
  HPALETTE  hpal = NULL;
  WORD    nNumColors;
  BYTE    red;
  BYTE    green;
  BYTE    blue;
  WORD    i;
  RGBQUAD  FAR *pRgb;

  // Get a pointer to the color table and the number of colors in it
  pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
  nNumColors = DibNumColors(lpbi);

  if (nNumColors)
  {
    // Allocate for the logical palette structure
    pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
    if (!pPal)
  return(NULL);

    pPal->palNumEntries = nNumColors;
    pPal->palVersion  = PALVERSION;

    // Fill in the palette entries from the DIB color table and
    // create a logical color palette.
    for (i = 0; i < nNumColors; i++)
    {
      pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
      pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
      pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
      pPal->palPalEntry[i].peFlags = (BYTE)0;
    }
    hpal = CreatePalette(pPal);
    LocalFree((HANDLE)pPal);
  }
  else
  {
    if (lpbi->biBitCount > 8)
    {
      // A > 8 bitcount DIB has no color table entries so, set the number of
      // colors to the maximum value (256).

      nNumColors = MAXPALETTE;
      pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
      if (!pPal)
    return(NULL);

      pPal->palNumEntries = nNumColors;
      pPal->palVersion    = PALVERSION;

      red = green = blue = 0;

      // Generate 256 (=8*8*4) RGB combinations to fill
      // the palette entries.

      for (i = 0; i < pPal->palNumEntries; i++){
    pPal->palPalEntry[i].peRed   = red;
    pPal->palPalEntry[i].peGreen = green;
    pPal->palPalEntry[i].peBlue  = blue;
    pPal->palPalEntry[i].peFlags = (BYTE)0;

    if (!(red += 32))
        if (!(green += 32))
      blue += 64;
      }
      hpal = CreatePalette(pPal);
      LocalFree((HANDLE)pPal);
    }
  }
  return(hpal);
}

//*************************************************************************
//                                                                          
//  FUNCTION   :OpenDIB(LPSTR szFile)                                       
//                                                                          
//  PURPOSE    :Open a DIB file and create a MEMORY DIB, a memory handle    
//              containing BITMAPINFO, palette data and the bits.           
//                                                                          
//  RETURNS    :A handle to the DIB.                                        
//
//  COMMENTS   : 
//    08/18/94  TDM
//      Changed "lread" to "_lread"
//                                                                          
//*************************************************************************

HANDLE OpenDIB (LPSTR szFile)
{
  unsigned        fh;
  LPBITMAPINFOHEADER  lpbi;
  DWORD         dwLen = 0;
  DWORD         dwBits;
  HANDLE        hdib;
  HANDLE        h;
  OFSTRUCT        of;

  DebugOut("ICMTEST : DIB.C : OpenDIB : Opening %s\r\n", (LPSTR)szFile);
  // Open the file and read the DIB header information
  fh = OpenFile(szFile, &of, OF_READ);
  if (fh == -1)
  {
    ErrMsg("Can't open BMP file '%s'", szFile);
    return(NULL);
  }

  hdib = ReadDibBitmapInfo(fh);
  if (!hdib)
  {
    ErrMsg("OpenDIB:  %s is not a Legitimate DIB File!", szFile);
    return(NULL);
  }

  // Calculate the memory needed to hold the DIB
  lpbi = (VOID FAR *)hdib;
  dwBits = lpbi->biSizeImage;
  dwLen  = lpbi->biSize + (DWORD)PaletteSize(lpbi) + dwBits;

  // Try to increase the size of the bitmap info. buffer to hold the DIB
  h = GlobalReAlloc(hdib, dwLen, GHND);
  if (!h)
  {
    GlobalFree(hdib);
    hdib = NULL;
  }
  else
    hdib = h;

  // Read in the bits
  if (hdib)
  {
    lpbi = (VOID FAR *)hdib;
    lread(fh, (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi), dwBits); 
  }
  _lclose(fh);

  return(hdib);
}

//*************************************************************************
//                                                                          
//  FUNCTION   : BitmapFromDib(HANDLE hdib, HPALETTE hpal)                  
//                                                                          
//  PURPOSE    : Will create a DDB (Device Dependent Bitmap) given a global 
//               handle to a memory block in CF_DIB format                  
//                                                                          
//  RETURNS    : A handle to the DDB.                                       
//                                                                          
//  COMMENTS   : 
//
//*************************************************************************

HBITMAP BitmapFromDib (HANDLE hdib, HPALETTE hpal)
{
  LPBITMAPINFOHEADER  lpbi;
  HPALETTE            hpalT;
  HDC                 hdc;
  HBITMAP             hbm;
  DWORD               dwLastError;

  if(!hdib)
  {
    ErrMsg("DIB.C:  BitmapFromDib called with NULL hdib");
    return(NULL);
  }

  lpbi = (VOID FAR *)hdib;
  {
    WORD  wPaletteSize;
    
    hdc = GetDC(NULL);
    if (hpal)
    {
      hpalT = SelectPalette(hdc,hpal,FALSE);
      RealizePalette(hdc);   
    }

    wPaletteSize = PaletteSize(lpbi);
    
    SetLastError(0);
    hbm = CreateDIBitmap(hdc,
                        (LPBITMAPINFOHEADER)lpbi,
                        (DWORD)CBM_INIT,
                        (LPSTR)lpbi + lpbi->biSize + wPaletteSize,
                        (LPBITMAPINFO)lpbi,
                         DIB_RGB_COLORS );
    if (!hbm)  // Couldn't CreateDIBitmap
    {
      dwLastError = GetLastError();
      ErrMsg("ICMTEST : DIB.C : BitmapFromDib : CreateDIBitMap error = %ld", dwLastError);
    }
    if (hpal)
    {
      SelectPalette(hdc,hpalT,FALSE);
    }
    ReleaseDC(NULL,hdc);
  }
  return(hbm);
}
                          
//*************************************************************************
//                                                                          
//  FUNCTION   :  DIBPaint(HDC hDC, int x, int y)                           
//                                                                          
//  PURPOSE    :  Sets the DIB/bitmap bits on the screen or the given device
//                                                                          
//*************************************************************************

void DIBPaint (HDC hDC, int xoffset, int yoffset)
{
  HPALETTE hpalOld;
  LPBITMAPINFOHEADER lpbi;
  LPBITMAPINFOHEADER lpbi1;
  HANDLE hdib;
  BITMAP bm;
  HBITMAP hbm;
  HBITMAP hbmGetSet;
  BYTE FAR *lpb3;
  BYTE FAR *lpb4;
  DWORD i,j;
  int   iRetVal;        // Return from SetICMMode

  StartWait();  //Put up busy cursor
  
  hdib = gBMPInfo[gwBMPIndex].hDib;
  if (hdib)
    lpbi = (LPBITMAPINFOHEADER)hdib;
  hbm = gBMPInfo[gwBMPIndex].hBmp;
  
  if (hbm)
    DrawBitmap (hDC, xoffset, yoffset, hbm, SRCCOPY);
  yoffset += 55;

  if (hdib)
    CreateandDrawBitmap (hDC, xoffset, yoffset, hdib, SRCCOPY);
  yoffset += 55;

  if (lpbi)
    SetDIBitsToDevice (hDC,
           xoffset, yoffset,
	   (int)lpbi->biWidth, ABS((int)lpbi->biHeight),
           0,0,
           0,
	   ABS((int)lpbi->biHeight),
           (LPSTR)((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)),
           (LPBITMAPINFO)lpbi,
           DIB_RGB_COLORS );
  yoffset += 55;

  if (lpbi)
    StretchDIBits(hDC,
      xoffset, yoffset,
      (int)lpbi->biWidth, ABS((int)lpbi->biHeight),
      0, 0 + ABS((int)lpbi->biHeight)/2,
      (int)lpbi->biWidth,
      ABS((int)lpbi->biHeight) / 2,
      (LPSTR)((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)),
      (LPBITMAPINFO)lpbi,
      DIB_RGB_COLORS,
      SRCCOPY);
  yoffset += 55;

  // Test to make sure that Get/Set pairs make sense, that is, that the
  // image colors do not change.   

  if (hbm)
  {
    GetObject(hbm,sizeof(BITMAP),(LPSTR)&bm);
    hbmGetSet = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
    hbmGetSet = SelectObject(ghdcCompat,hbmGetSet);
    hpalOld = SelectPalette(ghdcCompat,ghpalCurrent,TRUE);
    RealizePalette(ghdcCompat);
    BitBlt(ghdcCompat,0,0,bm.bmWidth,bm.bmHeight,hDC,xoffset,yoffset-100,SRCCOPY);
    hbmGetSet = SelectObject(ghdcCompat,hbmGetSet);
    lpb3 = (BYTE FAR *)GlobalLock(hdib);
    lpb4 = (BYTE FAR *)GlobalLock(ghTempBits);
    lpbi1 = (LPBITMAPINFOHEADER)lpb4;
    j = ((LPBITMAPINFOHEADER)lpb3)->biSize;
    for(i=0; i < j; i++)
    {
      *lpb4++ = *lpb3++;
    }

    j = PaletteSize(hdib);
    for (i=0; i < j; i++)
    {
      *lpb4++ = *lpb3++;
    }

    if (gfDoICM)
    {
      if((iRetVal = SetICMMode(ghdcCompat, ICM_ON)) == 0)
        DebugOut("ICMTEST : DIB.C : DibPaint : SetICMMode On FAILED\r\n");
      SetColorSpace(ghdcCompat, ghlcsCurrent);
    }
    GetDIBits(ghdcCompat, hbmGetSet, 0, ABS((int)lpbi->biHeight), lpb4,
        (LPBITMAPINFO)lpbi1, DIB_RGB_COLORS);
    if (gfDoICM)
    {
      if((iRetVal = SetICMMode(ghdcCompat, ICM_ON)) == 0)
        DebugOut("ICMTEST : DIB.C : DibPaint : SetICMMode On FAILED\r\n");
      SetColorSpace(ghdcCompat, ghlcsDefault);
    }
    SelectPalette(ghdcCompat,hpalOld,TRUE);
    SetDIBitsToDevice (hDC,
           xoffset, yoffset,
	   (int)lpbi->biWidth, ABS((int)lpbi->biHeight),
           0,0,
           0,
	   ABS((int)lpbi->biHeight),
           (LPSTR)((LPSTR)lpbi1 + (WORD)lpbi1->biSize + PaletteSize(lpbi1)),
           (BITMAPINFO *)lpbi1,
           DIB_RGB_COLORS);

    DeleteObject(hbmGetSet);
  }
  EndWait(); // Return to normal cursor
}

//*************************************************************************
//                                                                          
//  FUNCTION   : DrawBitmap(HDC hdc, int x, int y, HBITMAP hbm, DWORD rop)  
//                                                                          
//  PURPOSE    : Draws bitmap <hbm> at the specifed position in DC <hdc>    
//                                                                          
//  RETURNS    : Return value of BitBlt()                                   
//                                                                          
//*************************************************************************

BOOL DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop)
{
  HDC      hdcBits;
  BITMAP    bm;
  HBITMAP   hbmpOldBmp;
  BOOL      bBitBlt;
  int      iBytes;

  hdcBits = CreateCompatibleDC(hdc);
  iBytes = GetObject(hbm,sizeof(BITMAP),(LPSTR)&bm);
  if(iBytes == 0)
  {
    DebugOut("ICMTEST : DIB.C : DrawBitmap : GetObject failed");
  }
  hbmpOldBmp = SelectObject(hdcBits,hbm);
  bBitBlt = BitBlt(hdc,x,y,bm.bmWidth,bm.bmHeight,hdcBits,0,0,rop);
  hbm = SelectObject(hdcBits,hbmpOldBmp);
  DeleteDC(hdcBits);
  return(bBitBlt);
}

//*************************************************************************
//                                                                          
//  FUNCTION   : CreateandDrawBitmap(HDC hdc,                               
//             int x,                                                       
//             int y,                                                       
//             HBITMAP hbm,                                                 
//             DWORD rop)                                                   
//                                                                          
//  PURPOSE    : Draws bitmap <hbm> at the specifed position in DC <hdc>    
//                                                                          
//  RETURNS    : Return value of BitBlt()                                   
//
//  COMMENTS   : 
//
//*************************************************************************

BOOL CreateandDrawBitmap (HDC hdc, int x, int y, HANDLE hdib, DWORD rop)
{
  LPBITMAPINFOHEADER  lpbi;
  HDC      hdcBits;
  HBITMAP   hbm;
  BITMAP    bm;
  BOOL      bBitBlt;

  lpbi = (VOID FAR *)hdib;

  //  Get a DDB from the DIB

  hbm = CreateDIBitmap(hdc,
        (LPBITMAPINFOHEADER)lpbi,
        (LONG)CBM_INIT,
        (LPSTR)lpbi + lpbi->biSize + PaletteSize(lpbi),
        (LPBITMAPINFO)lpbi,
        DIB_RGB_COLORS );

  if (!hbm)
    return(FALSE);

  hdcBits = CreateCompatibleDC(hdc);
  GetObject(hbm,sizeof(BITMAP),(LPSTR)&bm);
  hbm = SelectObject(hdcBits,hbm);
  bBitBlt = BitBlt(hdc,x,y,bm.bmWidth,bm.bmHeight,hdcBits,0,0,rop);
  hbm = SelectObject(hdcBits,hbm);
  DeleteDC(hdcBits);
  DeleteObject(hbm);

  return(bBitBlt);
}

//*************************************************************************
//                                                                          
//  FUNCTION   : DibNumColors(VOID FAR * pv)                                
//                                                                          
//  PURPOSE    : Determines the number of colors in the DIB by looking at   
//               the BitCount filed in the info block.                      
//                                                                          
//  RETURNS    : The number of colors in the DIB.                           
//                                                                          
//*************************************************************************

WORD DibNumColors(LPBITMAPINFOHEADER lpbi)
{
  //  With the BITMAPINFO format headers, the size of the palette
  //  is in biClrUsed, if biClrUsed is 0, it is dependent on the
  //  bits per pixel ( = 2 raised to the power of bits/pixel).
     
  if (lpbi->biClrUsed != 0)
  {
    return(WORD)lpbi->biClrUsed;
  }

  switch (lpbi->biBitCount)
  {
    case 1:
      return 2;
    case 4:
      return 16;
    case 8:
      return 256;
    default:
      // A > 8 bitcount DIB has no color table
      return 0;
  }
}

//*************************************************************************
//                                                                          
//  FUNCTION   :  PaletteSize(VOID FAR * pv)                                
//                                                                          
//  PURPOSE    :  Calculates the palette size in bytes.
//                                                                          
//  RETURNS    :  Palette size in number of bytes.                          
//                                                                          
//*************************************************************************

WORD PaletteSize (LPBITMAPINFOHEADER lpbi)
{
  if ( (lpbi->biSize == sizeof(BITMAPINFOHEADER))
       && ((lpbi->biBitCount == 16) || (lpbi->biBitCount == 32))
       && lpbi->biCompression == BI_BITFIELDS)
  {
    return (3 * sizeof(DWORD));
  }
  else
    return (DibNumColors(lpbi) * sizeof(RGBQUAD));
}

//*************************************************************************
//                                                                          
//  FUNCTION   : FreeDIBs(void)                                             
//                                                                          
//  PURPOSE    : Frees all currently active bitmap, DIB and palette objects 
//               and initializes their handles.                             
//                                                                          
//*************************************************************************

void FreeDIBs(void)
{
  UINT    uiCount;
  HGLOBAL hFreedHandle;
  
  if (ghpalCurrent)
    DeleteObject(ghpalCurrent);

  for(uiCount = 1; uiCount < MAX_BMP; uiCount++)
  {
    if (gBMPInfo[uiCount].hBmp)
    {
      DeleteObject(gBMPInfo[uiCount].hBmp);
      gBMPInfo[uiCount].hBmp = NULL;
    }
    if (gBMPInfo[uiCount].hDib)
    {
      hFreedHandle = GlobalFree(gBMPInfo[uiCount].hDib);
      if (hFreedHandle)
      {
        ErrMsg("FreeDIBs : failed to free gBMPInfo[%u].hDib", uiCount);
      }
      else
      {
  gBMPInfo[uiCount].hDib = NULL;
      }
      gBMPInfo[uiCount].hDib = NULL;
    }
  }
  ghpalCurrent = NULL;
}
