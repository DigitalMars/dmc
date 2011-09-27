/******************************Module*Header*******************************\
* Module Name: fractal.c
*
* Fractal generation Screen saver
*
* Created: 19-Dec-1992 20:43:00
* Author: Petrus Wong
*
* Copyright (c) 1992-1995 Microsoft Corporation
*
* The screen saver generates the Mandelbrot picture...
*
* Dependencies:
*
*   (#defines)
*   (#includes)
*
\**************************************************************************/
#include            <windows.h>
#include            <scrnsave.h>
#include            <stdio.h>
#include            <math.h>
#include            <commdlg.h>
#include            "fractal.h"

LONG APIENTRY ScreenSaverProc(HWND, UINT, WPARAM, LPARAM);
BOOL APIENTRY ScreenSaverConfigureDialog(HWND, UINT, WPARAM, LPARAM);
BOOL WINAPI RegisterDialogClasses(HANDLE);
BOOL StartMandelbrot (PINFO);
INT  iCreatePenFrPal(HDC, PVOID *, INT, HPALETTE *);
BOOL bInitInfo(PINFO);
void vLoadStrings(VOID);
BOOL SaveBitmapFile(HDC, HBITMAP, PSTR);
void ErrorOut(char errstring[30]);
BOOL bGetPPStr(char szXFr[MAXDOUBLELEN], char szYFr[MAXDOUBLELEN],
               char szX2[MAXDOUBLELEN],  char szY2[MAXDOUBLELEN],
               char szSavef[MAXFILELEN], char szIter[MAXDOUBLELEN]);
BOOL bInitDlgItems(HWND);
BOOL bColorCycle(HDC, PINFO);
/******************************Public*Routine******************************\
*
* ScreenSaverProc
*
* Effects:  Creates a drawing thread.
*
* Warnings:
*
* History:
*  18-Dec-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

LONG APIENTRY
ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBkBr;

    switch(message)
    {
        case WM_CREATE:
            hBkBr = GetStockObject(BLACK_BRUSH);
            hpnRed     = CreatePen(PS_SOLID, 0, RGB(0xFF, 0,    0));
            hpnGreen   = CreatePen(PS_SOLID, 0, RGB(0,    0xFF, 0));
            hpnBlack   = CreatePen(PS_SOLID, 0, RGB(0,    0,    0));

            vLoadStrings();
            bInitInfo(&gInfo);
            break;

        case WM_ERASEBKGND: {
            RECT    rc;

            GetClientRect(hWnd, &rc);
            FillRect((HDC)wParam, &rc, hBkBr);
            gInfo.bMandel = TRUE;
            gInfo.hwnd = hWnd;
            gInfo.hHTPal = CreateHalftonePalette((HDC)wParam);
            gInfo.hThrd = CreateThread(NULL, 0,
                                 (LPTHREAD_START_ROUTINE)StartMandelbrot,
                                 &gInfo,
                                 STANDARD_RIGHTS_REQUIRED,
                                 &gInfo.dwThreadId );

            if (gInfo.hThrd) {
                SetThreadPriority(gInfo.hThrd, THREAD_PRIORITY_BELOW_NORMAL);
            }


            return 0L;
        }
        case WM_DESTROY:
            DeleteObject(hBkBr);
            DeleteObject(hpnRed);
            DeleteObject(hpnGreen);
            DeleteObject(hpnBlack);
            TerminateThread(gInfo.hThrd, (DWORD)0L);
            break;
    }
    return (DefScreenSaverProc(hWnd,message,wParam,lParam));
}


/******************************Public*Routine******************************\
*
* ScreenSaverConfigureDialog
*
* Effects:
*
* Warnings:
*
* History:
*  19-Dec-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL APIENTRY
ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG: {
            vLoadStrings();
            bInitDlgItems(hDlg);
            return TRUE;
        }
        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
                static char         szFile[MAXPATHLEN];
                static BOOL         bSave2DiffFile = FALSE;

                case ID_SAVEAS: {
                    OPENFILENAME ofn;
                    char         szDirName[MAXPATHLEN];
                    char         szFileTitle[256];
                    static char  *szFilter;

                    szFilter = "DIB files (*.bmp)\0\0";

                    GetSystemDirectory((LPSTR) szDirName, MAXPATHLEN);
                    strcpy(szFile, "*.bmp\0");
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hDlg;
                    ofn.lpstrFilter = szFilter;
                    ofn.lpstrCustomFilter = (LPSTR) NULL;
                    ofn.nMaxCustFilter = 0L;
                    ofn.nFilterIndex = 0L;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFileTitle = szFileTitle;
                    ofn.nMaxFileTitle = sizeof(szFileTitle);
                    ofn.lpstrInitialDir = szDirName;
                    ofn.lpstrTitle = "Save Bitmap As";
                    ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
                    ofn.nFileOffset = 0;
                    ofn.nFileExtension = 0;
                    ofn.lpstrDefExt = (LPSTR)NULL;

                    if (GetSaveFileName(&ofn))
                        bSave2DiffFile = TRUE;

                    break;
                }
                case IDOK: {
                    char    szXFr[MAXDOUBLELEN];
                    char    szYFr[MAXDOUBLELEN];
                    char    szX2[MAXDOUBLELEN];
                    char    szY2[MAXDOUBLELEN];
                    char    szIter[MAXDOUBLELEN];

                    if (bSave2DiffFile)
                        WritePrivateProfileString(szAppName, szSaveFile, szFile, szIniFile);

                    //
                    // Saves new configuartion...
                    //
                    GetDlgItemText(hDlg, ID_XFROM, szXFr, MAXDOUBLELEN);
                    GetDlgItemText(hDlg, ID_YFROM, szYFr, MAXDOUBLELEN);
                    GetDlgItemText(hDlg, ID_XTO,   szX2,  MAXDOUBLELEN);
                    GetDlgItemText(hDlg, ID_YTO,   szY2,  MAXDOUBLELEN);
                    GetDlgItemText(hDlg, ID_ITERATION, szIter, MAXDOUBLELEN);
                    WritePrivateProfileString(szAppName, szXFrom, szXFr, szIniFile);
                    WritePrivateProfileString(szAppName, szYFrom, szYFr, szIniFile);
                    WritePrivateProfileString(szAppName, szXTo,    szX2,  szIniFile);
                    WritePrivateProfileString(szAppName, szYTo,    szY2,  szIniFile);
                    WritePrivateProfileString(szAppName, szIteration, szIter,  szIniFile);
                }
                case IDCANCEL:
                    bSave2DiffFile = FALSE;
                    EndDialog(hDlg,LOWORD(wParam) == IDOK);
                    return TRUE;
                default:
                    break;
            }
        default:
            break;
    }
    return FALSE;
}

/******************************Public*Routine******************************\
*
* RegisterDialogClasses
*
* Effects: Register window classes used by the configuration dialog
*
* Warnings: Return TRUE even if no class needed to be registered.
*
* History:
*  18-Dec-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
    return TRUE;
}

/**************************************************************************\
*
* StartMandelbrot
*					     2
* Effects: Draw'g the Mandelbrot Set for Q (z) = z  + c, where z, c complex
*
* Warnings:
*
* History:
*  15-Jan-1993      Petrus Wong     Added color cycle and removed the red &
*                                   black for saving the screen
*  19-Dec-1992      Petrus Wong     Adapted from the Mandel SDK Sample
*  14-Dec-1992 -by- Petrus Wong     Enable shadow bitmap
*  14-Jun-1992 -by- Petrus Wong     Modified to use the pen array and LineTo
*  20-Nov-1991 -by- Petrus Wong
* Wrote it.
\**************************************************************************/
BOOL StartMandelbrot(PINFO pInfo)
{
   DWORD       dwTick1;
   HDC         hDC;
   RECT        rc;
   int	       m, n, i, iPrev;
   int         xCurr, yCurr;
   int         iPen;

#ifndef THRDONE
   INT      iNumClr;
   char     text[256];
#endif

   double c1, c2;
   double x1, y1, x, y, r;

   HDC          hDCMem;
   HBITMAP      hBitmap, hOldBitmap;

   pInfo->bMandel = TRUE;
   pInfo->bDrawing = TRUE;
   hDC = GetDC(pInfo->hwnd);

#ifndef THRDONE
    if ((iNumClr = iCreatePenFrPal(hDC, NULL, 0, &(pInfo->hPal))) != 0) {
        sprintf( gtext,"iNumClr = %d\n", iNumClr);
        OutputDebugString( gtext);

        if ((pInfo->prghPen = (PVOID*) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(HPEN)*iNumClr)) == NULL) {
            MessageBox(GetFocus(), "Failed in Memory Allocation for pInfo->prghPen!", "Error", MB_OK);
        } else {
            if ((pInfo->iPen = iCreatePenFrPal(hDC, pInfo->prghPen, 1, &(pInfo->hPal))) == 0)
                MessageBox(GetFocus(), "Failed in creating pen!", "Error", MB_OK);
        }
    }

   hDCMem = CreateCompatibleDC(hDC);

   if (pInfo->hPal) {
    SelectPalette(hDCMem, pInfo->hPal, FALSE);
    RealizePalette(hDCMem);
   }
   iPen = pInfo->iPen + 1;
   wsprintf(text, "iPen = %d\n", iPen);
   //MessageBox(GetFocus(), text, "Error", MB_OK);
#endif

   GetClientRect(pInfo->hwnd, &rc);

   hBitmap = CreateCompatibleBitmap(hDC, (int)rc.right, (int)rc.bottom);
   hOldBitmap = SelectObject(hDCMem, hBitmap);

//#if 0
 {
   int iWidth, i, j;

   iWidth = (((iWidth = rc.right/iNumClr) == 0) ? 1 : iWidth);
   sprintf( gtext,"iWidth = %d\n", iWidth);
   OutputDebugString( gtext);

   for (i = 0; i < iNumClr; i++) {
   //for (i = 0; i < 256; i++) {
       //sprintf( gtext,"i mod iNumClr = %d\n", i % iNumClr);
       //OutputDebugString( gtext);
       if ((SelectObject(hDC, (HPEN)(pInfo->prghPen)[i % iNumClr])) == NULL) {
            sprintf( gtext,"Select Pen failed\n");
            OutputDebugString( gtext);
       }
       SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[i % iNumClr]);
       for (j = 0; j < iWidth; j++) {
            MoveToEx(hDC, i*iWidth+j, 0, NULL);
            LineTo(hDC, i*iWidth+j, rc.bottom);
            MoveToEx(hDCMem, i*iWidth+j, 0, NULL);
            LineTo(hDCMem, i*iWidth+j, rc.bottom);
       }

   }
 }
//#endif

   dwTick1 = GetTickCount();

   sprintf( gtext,"Start Drawing\n");
   OutputDebugString( gtext);
   yCurr = rc.top;
   for (n=rc.top; n<=rc.bottom; n+=pInfo->iStep, yCurr+=pInfo->iStep, iPrev = FIRST_PIXEL) {
       xCurr = rc.left;                   // since LineTo excludes last point
       MoveToEx(hDC, 0, yCurr, NULL);
       MoveToEx(hDCMem, 0, yCurr, NULL);
       c2 = Xform((double) n, 0.0, (double) rc.bottom, pInfo->yFrom, pInfo->yTo);

       for (m=rc.left; m<=rc.right; m++, xCurr++) {
           c1 = Xform((double) m, 0.0, (double) rc.right, pInfo->xFrom, pInfo->xTo);
	   x = c1;
	   y = c2;

	   for (i=1; i<=pInfo->iIteration; i++) {
               x1 = (x - y) * (x + y) + c1;
               y1 = 2 * x * y + c2;
	       r = x1 * x1 + y1 * y1;
	       x = x1;
	       y = y1;
	       if (r > 4.0) {
                   break;
               }
           }

           if (i != iPrev) {
               if (iPrev != FIRST_PIXEL) {
                 switch(iPrev) {
                   case 1: SelectObject(hDC, (HPEN)(pInfo->prghPen)[0]);
                           SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[0]);
                    break;
                   default:
                       if (iPrev >= pInfo->iIteration) {
                            SelectObject(hDC, (HPEN)(pInfo->prghPen)[iPen/3]);
                            SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[iPen/3]);
                            break;
                       }
                       SelectObject(hDC, (HPEN)(pInfo->prghPen)[iPrev % iPen]);
                       SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[iPrev % iPen]);
                       //SelectObject(hDC, (HPEN)(pInfo->prghPen)[iPrev % iNumClr]);
                       break;
                 }
                 iPrev = i;
                 LineTo(hDC, xCurr, yCurr);
                 LineTo(hDCMem, xCurr, yCurr);
               }
               else
		  iPrev = i;	// remember the color for the first pixel
           }
       }

            switch(i)
		{
                   case 1: SelectObject(hDC, (HPEN)(pInfo->prghPen)[0]);
                           SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[0]);
                    break;
                   default:
                       if (i >= pInfo->iIteration) {
                            SelectObject(hDC, (HPEN)(pInfo->prghPen)[iPen/3]);
                            SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[iPen/3]);
                            break;
                       }
                       SelectObject(hDC, (HPEN)(pInfo->prghPen)[i % iPen]);
                       SelectObject(hDCMem, (HPEN)(pInfo->prghPen)[i % iPen]);
                       //SelectObject(hDC, (HPEN)(pInfo->prghPen)[i % iNumClr]);
                       break;
		}

		 LineTo(hDC,xCurr,yCurr);
                 LineTo(hDCMem,xCurr,yCurr);

   }



   pInfo->dwElapsed = GetTickCount() - dwTick1;

   if (pInfo->hBmpSaved)
       DeleteObject(pInfo->hBmpSaved);

   pInfo->hBmpSaved = SelectObject(hDCMem, hOldBitmap);
   pInfo->bDrawing = FALSE;
   SaveBitmapFile(hDC, pInfo->hBmpSaved, pInfo->szSavef);
   pInfo->bClrCycle = TRUE;
   if (pInfo->hPal)
      bColorCycle(hDC, pInfo);
   ReleaseDC(pInfo->hwnd, hDC);

#ifndef THRDONE
   if (pInfo->prghPen != NULL) {
       for (i = 0; i <= pInfo->iPen; i++) {
           DeleteObject((HPEN) (pInfo->prghPen)[i]);
       }
       GlobalFree(pInfo->prghPen);
   }
#endif

   DeleteDC(hDCMem);

   ExitThread(0);
   if (!CloseHandle(pInfo->hThrd))
       MessageBox(GetFocus(), "Failed in CloseHandle!", "Error", MB_OK);

   return TRUE;
}


/******************************Public*Routine******************************\
*
* iCreatePenFrPal
*
* Effects: Create an array of pens from palette.
*          If device supports palette, then first creates a logical palette
*          with a good spread of color. Then select the logical palette into
*          the DC. Create pen that corresponds to each palette entry.
*          If system does not support palette, then use the system palette.
*
*          prghPen pointer to an array of hPen
*                  If this is NULL, the required size of the array is
*                  returned. If this is not NULL, the array will be filled
*                  with hPens.
*
*          returns the number of hPens created.
*
* Warnings:
*
* History:
*  13-Jun-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

INT iCreatePenFrPal(HDC hDC, PVOID *prghPen, INT iWidth, HPALETTE *phPal)
{
    INT             iNumClr, iResult, i, iEntry;
    PLOGPALETTE     plogPat;
    ULONG           ulSize;

    iResult = 0;

    if (!((GetDeviceCaps(hDC, RASTERCAPS)) & RC_PALETTE)) {

        if (iNumClr = GetSystemPaletteEntries(hDC, 0, 0, NULL)) {
            if (prghPen == NULL) {
                return (iNumClr);
            }

            if (iWidth < 0) {
                MessageBox(GetFocus(), "Invalid pen width!", "Error", MB_OK);
                return 0L;
            }

            for (i = 0; i < iNumClr; i++) {
                prghPen[i] =
                     (PVOID) CreatePen(PS_SOLID, iWidth, PALETTEINDEX(i));
                iResult = i;
#if 0
                SelectObject(hDC, (HPEN)prghPen[i]);
                MoveToEx(hDC, i*iWidth, 0, NULL);
                LineTo(hDC, i*iWidth, 300);
#endif
            }
            return iResult;
        }

        // GetPaletteEntries returns 0
        iNumClr = 1 << GetDeviceCaps(hDC, BITSPIXEL);

        if (iNumClr <= 16) {
            sprintf( gtext,"Assertion failed: iCreatePenFrPal() \n");
            OutputDebugString( gtext);
            sprintf( gtext,"iNumClr <= 16! iNumClr = %d\n", iNumClr);
            OutputDebugString( gtext);
            return (iNumClr);
        }

        // Assuming at least 16 bit device
        //
        iNumClr = 768;      // COLORREF format: 0x00bbggrr r,g,b=[0,255]

        if (prghPen == NULL) {
            return (iNumClr);
        }

        if (iWidth < 0) {
            MessageBox(GetFocus(), "Invalid pen width!", "Error", MB_OK);
            return 0L;
        }

        for (i = 0; i < iNumClr; i++) {
            if (i < 256) {
                prghPen[i] =
                    (PVOID) CreatePen(PS_SOLID, iWidth, RGB(i,0,255-i));
                iResult = i;
#if 0
                SelectObject(hDC, (HPEN)prghPen[i]);
                MoveToEx(hDC, i*iWidth, 0, NULL);
                LineTo(hDC, i*iWidth, 300);
#endif
                continue;
            }

            if (i < 512) {
                prghPen[i] =
                    (PVOID) CreatePen(PS_SOLID, iWidth, RGB(511-i,i-256,0));
                iResult = i;
#if 0
                SelectObject(hDC, (HPEN)prghPen[i]);
                MoveToEx(hDC, i*iWidth, 0, NULL);
                LineTo(hDC, i*iWidth, 300);
#endif
                continue;
            }


            prghPen[i] =
                (PVOID) CreatePen(PS_SOLID, iWidth, RGB(0,767-i,i-512));
            iResult = i;
#if 0
            SelectObject(hDC, (HPEN)prghPen[i]);
            MoveToEx(hDC, i*iWidth, 0, NULL);
            LineTo(hDC, i*iWidth, 300);
#endif
        }

        return iResult;
    }

    //
    // Palette managed device
    //
    iEntry = iNumClr = 1 << GetDeviceCaps(hDC, BITSPIXEL);

    if (iNumClr > 256) {
        sprintf( gtext,"Assertion failed: iCreatePenFrPal() \n");
        OutputDebugString( gtext);
        sprintf( gtext,"iNumClr > 256! iNumClr = %d\n", iNumClr);
        OutputDebugString( gtext);
        iEntry=iNumClr = 256;
    }

    ulSize = sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*iNumClr;

    iNumClr -= GetDeviceCaps(hDC, NUMRESERVED);

    if (prghPen == NULL)
        return iNumClr;

    if ((plogPat = (PLOGPALETTE) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, ulSize)) == NULL) {
        MessageBox(GetFocus(), "Failed in Memory Allocation for plogPat!", "Error", MB_OK);
        goto ErrExit1;
    }

    plogPat->palVersion = 0x300;
    plogPat->palNumEntries = (WORD) iEntry;

    {
    int     iFirst, iSecond, iInc;

        iFirst = iNumClr/3;
        iSecond = iFirst * 2;
        iInc = (iNumClr <= 256) ? 3 : 1;


        for (i = 0; i < iNumClr; i++) {
            if (i < iFirst) {
                plogPat->palPalEntry[i+10].peBlue   = 255-(i+1)*iInc;
                plogPat->palPalEntry[i+10].peGreen  = 0;
                plogPat->palPalEntry[i+10].peRed    = (i+1)*iInc;
                plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
                continue;
            }
            if (i < iSecond) {
                plogPat->palPalEntry[i+10].peBlue   = 0;
                plogPat->palPalEntry[i+10].peGreen  = (i+1-iFirst)*iInc;
                plogPat->palPalEntry[i+10].peRed    = 255-(i+1-iFirst)*iInc;
                plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
                continue;
            }
            plogPat->palPalEntry[i+10].peBlue   = (i+1-iSecond)*iInc;
            plogPat->palPalEntry[i+10].peGreen  = 255-(i+1-iSecond)*iInc;
            plogPat->palPalEntry[i+10].peRed    = 0;
            plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
        }
    }

    DeleteObject(*phPal);

    if ((*phPal = CreatePalette(plogPat)) == (HPALETTE) NULL) {
        MessageBox(GetFocus(), "Failed in creating palette!", "Error", MB_OK);
        goto ErrExit2;
    }
    if (*phPal) {
       SelectPalette(hDC, *phPal, FALSE);
       RealizePalette(hDC);
    }

    for (i = 0; i < iNumClr; i++) {
        HPEN    hPen;

        hPen = CreatePen(PS_SOLID, iWidth, PALETTEINDEX(i+10));
        prghPen[i] = (PVOID) hPen;
        iResult = i;
#if 0
        SelectObject(hDC, hPen);
        MoveToEx(hDC, i*iWidth, 0, NULL);
        LineTo(hDC, i*iWidth, 300);
#endif
    }

ErrExit2:
    GlobalFree(plogPat);
ErrExit1:
    return iResult;

}

/******************************Public*Routine******************************\
*
* bInitInfo
*
* Effects: Initialize the Info data structure
*
* Warnings:
*
* History:
*  19-Dec-1992      Petrus Wong         Retrieve setup info fr control.ini
*  14-Dec-1992 -by- Petrus Wong         Added Rle and pens stuff
*  28-Jan-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL bInitInfo(PINFO pInfo)
{
    char    szXFr[MAXDOUBLELEN], szYFr[MAXDOUBLELEN], szX2[MAXDOUBLELEN],
            szY2[MAXDOUBLELEN], szSavef[MAXPATHLEN], szIter[MAXDOUBLELEN];

    bGetPPStr(szXFr, szYFr, szX2, szY2, szSavef, szIter);
    pInfo->xFrom        = atof(szXFr);
    pInfo->xTo          = atof(szX2);
    pInfo->yFrom        = atof(szYFr);
    pInfo->yTo          = atof(szY2);
    pInfo->lxFrom       = -4096;                // 20.11 fix point
    pInfo->lxTo         = 4096;                 // representation of
    pInfo->lyFrom       = 4096;                 // -2, 2, 2, and -2
    pInfo->lyTo         = -4096;                //
    pInfo->iIteration   = (INT) atof(szIter);
    pInfo->iStep        = gStep;
    pInfo->bStretch     = gbStretch;
    pInfo->iStretchMode = giStretchMode;
    pInfo->hwnd         = NULL;
    pInfo->hThrd        = NULL;
    pInfo->bDrawing     = FALSE;
    pInfo->dwThreadId   = 0;
    pInfo->dwElapsed    = 0L;
    pInfo->c1           = 0.0;
    pInfo->c2           = 0.0;
    pInfo->lc1          = 0L;
    pInfo->lc2          = 0L;
    pInfo->hBmpSaved    = NULL;
    pInfo->bMandel      = TRUE;
    pInfo->bSetDIBsToDevice = FALSE;
    pInfo->hPal         = NULL;
    pInfo->prghPen      = NULL;
    pInfo->iPen         = 0;
    pInfo->bClrCycle    = FALSE;
    wsprintf((LPSTR) &(pInfo->szSavef), szSavef);
    return TRUE;
}




/******************************Public*Routine******************************\
*
* vLoadStrings
*
* Effects: Load strings from resource
*
* Warnings:
*
* History:
*  19-Dec-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

void vLoadStrings(VOID)
{
    LoadString(hMainInstance, idsAppName, szAppName, APPNAMEBUFFERLEN);
    LoadString(hMainInstance, idsName, szName, TITLEBARNAMELEN);
    LoadString(hMainInstance, idsIniFile, szIniFile, MAXFILELEN);
    LoadString(hMainInstance, idsScreenSaver, szScreenSaver, 22);
    LoadString(hMainInstance, idsHelpFile, szHelpFile, MAXFILELEN);
    LoadString(hMainInstance, idsNoHelpMemory, szNoHelpMemory, BUFFLEN);
    LoadString(hMainInstance, idsXFrom, szXFrom, MAXDOUBLELEN);
    LoadString(hMainInstance, idsYFrom, szYFrom, MAXDOUBLELEN);
    LoadString(hMainInstance, idsXTo, szXTo, MAXDOUBLELEN);
    LoadString(hMainInstance, idsYTo, szYTo, MAXDOUBLELEN);
    LoadString(hMainInstance, idsSaveAs, szSaveFile, MAXPATHLEN);
    LoadString(hMainInstance, idsIteration, szIteration, MAXDOUBLELEN);
}



/******************************Public*Routine******************************\
* SaveBitmapFile
*
*
* Effects: Save pInfo->hBmpSaved into disk specified by pszFileName
*
* Warnings: assumes hBmpSaved is not selected into window's DC other than
*           pInfo->hwnd's DC
*
* History:
*  14-Jan-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL SaveBitmapFile(HDC hDC, HBITMAP hBmp, PSTR pszFileName)
{
    int         hFile;
    OFSTRUCT    ofReOpenBuff;
    HBITMAP     hTmpBmp, hBmpOld;
    BOOL        bSuccess;
    BITMAPFILEHEADER    bfh;
    PBITMAPINFO pbmi;
    PBYTE       pBits;
    BITMAPINFO  bmi;
    PBYTE pjTmp, pjTmpBmi;
    ULONG sizBMI;


    bSuccess = TRUE;
    if (!hBmp) {
        MessageBox(GetFocus(), "There's no Bitmap to save!", "Error", MB_OK);
        return FALSE;
    }

    //
    // Let the graphics engine to retrieve the dimension of the bitmap for us
    // GetDIBits uses the size to determine if it's BITMAPCOREINFO or BITMAPINFO
    // if BitCount != 0, color table will be retrieved
    //
    bmi.bmiHeader.biSize = 0x28;              // GDI need this to work
    bmi.bmiHeader.biBitCount = 0;             // don't get the color table
    if ((GetDIBits(hDC, hBmp, 0, 0, (LPSTR)NULL, &bmi, DIB_RGB_COLORS)) == 0) {
        MessageBox(GetFocus(), "GetDIBits failed!", "Error", MB_OK);
        return FALSE;
    }

    //
    // Now that we know the size of the image, alloc enough memory to retrieve
    // the actual bits
    //
    if ((pBits = (PBYTE)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
                bmi.bmiHeader.biSizeImage)) == NULL) {
        MessageBox(GetFocus(), "Failed in Memory Allocation for pBits!", "Error", MB_OK);
        return FALSE;
    }

    //
    // Note: 24 bits per pixel has no color table.  So, we don't have to
    // allocate memory for retrieving that.  Otherwise, we do.
    //
    pbmi = &bmi;                                      // assume no color table
    if (bmi.bmiHeader.biBitCount != 24) {             // has color table
        sizBMI = sizeof(BITMAPINFO)+sizeof(RGBQUAD)*(1<<bmi.bmiHeader.biBitCount);
        //
        // I need more memory for the color table
        //
        if ((pbmi = (PBITMAPINFO)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizBMI )) == NULL) {
            MessageBox(GetFocus(), "Failed in Memory Allocation for pbmi!", "Error", MB_OK);
            bSuccess = FALSE;
            goto ErrExit1;
        }
        //
        // Now that we've a bigger chunk of memory, let's copy the Bitmap
        // info header data over
        //
        pjTmp = (PBYTE)pbmi;
        pjTmpBmi = (PBYTE)&bmi;
        sizBMI = sizeof(BITMAPINFOHEADER);

        while(sizBMI--)
        {
            *(((PBYTE)pjTmp)++) = *((pjTmpBmi)++);
        }

    }

    //
    // Let's open the file and get ready for writing
    //
    if ((hFile = OpenFile(pszFileName, (LPOFSTRUCT)&ofReOpenBuff,
                 OF_CREATE | OF_WRITE)) == -1) {
        MessageBox(GetFocus(), "Failed in OpenFile!", "Error", MB_OK);
        ErrorOut("OpenFile");
        goto ErrExit2;
    }

    //
    // But first, fill in the info for the BitmapFileHeader
    //
    bfh.bfType = 0x4D42;                            // 'BM'
    bfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*
        ((pbmi->bmiHeader.biBitCount == 24) ? 0 : (1<<pbmi->bmiHeader.biBitCount))+
        pbmi->bmiHeader.biSizeImage;
    bfh.bfReserved1 =
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
        sizeof(RGBQUAD)*((pbmi->bmiHeader.biBitCount == 24) ? 0 : (1<<pbmi->bmiHeader.biBitCount));

    //
    // Write out the file header now
    //
    if (_lwrite(hFile, (LPSTR)&bfh, sizeof(BITMAPFILEHEADER)) == -1) {
        MessageBox(GetFocus(), "Failed in WriteFile!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit3;
    }

    //
    // Bitmap can't be selected into a DC when calling GetDIBits
    // Assume that the hDC is the DC where the bitmap would have been selected
    // if indeed it has been selected
    //
    if (hTmpBmp = CreateCompatibleBitmap(hDC, pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight)) {
        hBmpOld = SelectObject(hDC, hTmpBmp);
        if ((GetDIBits(hDC, hBmp, 0, pbmi->bmiHeader.biHeight, (LPSTR)pBits, pbmi, DIB_RGB_COLORS))==0){
            MessageBox(GetFocus(), "Failed in GetDIBits!", "Error", MB_OK);
            bSuccess = FALSE;
            goto ErrExit4;
        }
    } else {
        MessageBox(GetFocus(), "Failed in creating bitmap!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit3;
    }

    //
    // Now write out the BitmapInfoHeader and color table, if any
    //
    if (_lwrite(hFile, (LPSTR)pbmi, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) *
        ((pbmi->bmiHeader.biBitCount == 24) ? 0 : (1 << pbmi->bmiHeader.biBitCount))) == -1) {
        MessageBox(GetFocus(), "Failed in WriteFile!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit4;
    }

    //
    // write the bits also
    //
    if (_lwrite(hFile, (LPSTR)pBits, pbmi->bmiHeader.biSizeImage) == -1) {
        MessageBox(GetFocus(), "Failed in WriteFile!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit4;
    }


ErrExit4:
    SelectObject(hDC, hBmpOld);
    DeleteObject(hTmpBmp);
ErrExit3:
    _lclose(hFile);
ErrExit2:
    GlobalFree(pbmi);
ErrExit1:
    GlobalFree(pBits);
    return bSuccess;
}

/************************************************************************
 * void ErrorOut(char errstring[30])
 *
 * Purpose: Print out an meainful error code by means of
 *	  GetLastError and printf
 *
 * Inputs:  errstring - the action that failed, passed by the
 *		      calling proc.
 *
 * Returns: none
 *
 * Calls:   GetLastError
 *
 * History:
 *
\************************************************************************/


void ErrorOut(char errstring[30])
{
DWORD Error;
CHAR  str[80];

Error= GetLastError();
wsprintf((LPSTR) str, "Error on %s = %d\n", errstring, Error);
MessageBox(GetFocus(), (LPSTR)str, "Error", MB_OK);
}

BOOL bInitDlgItems(HWND hDlg)
{
    char    szXFr[MAXDOUBLELEN], szYFr[MAXDOUBLELEN], szX2[MAXDOUBLELEN],
            szY2[MAXDOUBLELEN], szSavef[MAXPATHLEN], szIter[MAXDOUBLELEN];
    BOOL    bSuccess;

    bSuccess = bGetPPStr(szXFr, szYFr, szX2, szY2, szSavef, szIter);
    SetDlgItemText(hDlg, ID_XFROM, szXFr);
    SetDlgItemText(hDlg, ID_YFROM, szYFr);
    SetDlgItemText(hDlg, ID_XTO, szX2);
    SetDlgItemText(hDlg, ID_YTO, szY2);
    SetDlgItemText(hDlg, ID_ITERATION, szIter);

    return bSuccess;
}


/******************************Public*Routine******************************\
*
* bGetPPStr
*
* Effects: GetPrivateProfileString from control.ini
*
* Warnings:
*
* History:
*  19-Dec-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL bGetPPStr(char szXFr[MAXDOUBLELEN], char szYFr[MAXDOUBLELEN],
               char szX2[MAXDOUBLELEN],  char szY2[MAXDOUBLELEN],
               char szSavef[MAXPATHLEN], char szIter[MAXDOUBLELEN])
{
    char    szDefXFr[MAXDOUBLELEN];
    char    szDefYFr[MAXDOUBLELEN];
    char    szDefX2[MAXDOUBLELEN];
    char    szDefY2[MAXDOUBLELEN];
    char    szDefSavef[MAXPATHLEN];
    char    szDefIter[MAXDOUBLELEN];

    sprintf(szDefXFr, "-2.00");
    sprintf(szDefYFr, "1.00");
    sprintf(szDefX2,  "1.00");
    sprintf(szDefY2,  "-1.00");
    sprintf(szDefSavef, "c:\\fractal.bmp");
    sprintf(szDefIter, "500");
    GetPrivateProfileString(szAppName, szXFrom, szDefXFr, szXFr, MAXDOUBLELEN, szIniFile);
    GetPrivateProfileString(szAppName, szYFrom, szDefYFr, szYFr, MAXDOUBLELEN, szIniFile);
    GetPrivateProfileString(szAppName, szXTo, szDefX2, szX2, MAXDOUBLELEN, szIniFile);
    GetPrivateProfileString(szAppName, szYTo, szDefY2, szY2, MAXDOUBLELEN, szIniFile);
    GetPrivateProfileString(szAppName, szSaveFile, szDefSavef, szSavef, MAXPATHLEN, szIniFile);
    GetPrivateProfileString(szAppName, szIteration, szDefIter, szIter, MAXDOUBLELEN, szIniFile);
    return TRUE;
}



/******************************Public*Routine******************************\
*
* bColorCycle
*
* Effects:  Create a logical palette with a good spread of color
*           Animate the palette, shift the palette entries and animate
*           again for 256 times altogether.
*           This creates the color cycling effect.
*
* Warnings: Only works in device that support palette.
*
* History:
*  15-Jan-1993      Petrus Wong     Adaped from the SDK Mandel sample
*  24-Apr-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL bColorCycle(HDC hDC, PINFO pInfo)
{
    INT             iEntry, i, iNumClr;
    PLOGPALETTE     plogPat;
    ULONG           ulSize;
    BOOL            bSuccess;
    HPALETTE        hPal, hPalOld;
    PALETTEENTRY    peTemp;
    BOOL            bQuit;


UINT uRC;

    bSuccess = TRUE;
    iEntry = 1<<GetDeviceCaps(hDC, BITSPIXEL);
    iNumClr = 236;          // 256 minus 20 reserved colors
    ulSize = sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*256;

    if ((plogPat = (PLOGPALETTE) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, ulSize)) == NULL) {
        MessageBox(hMainWindow, "Failed in Memory Allocation for plogPat!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit1;
    }

    plogPat->palVersion = 0x300;
    plogPat->palNumEntries = (WORD) 256;
    {
    int     iFirst, iSecond, iInc;

        iFirst = iNumClr/3;
        iSecond = iFirst * 2;
        iInc = (iNumClr <= 256) ? 3 : 1;

        for (i = 0; i < iNumClr; i++) {
            if (i < iFirst) {
                plogPat->palPalEntry[i+10].peBlue   = 255-(i+1)*iInc;
                plogPat->palPalEntry[i+10].peGreen  = 0;
                plogPat->palPalEntry[i+10].peRed    = (i+1)*iInc;
                plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
                continue;
            }
            if (i < iSecond) {
                plogPat->palPalEntry[i+10].peBlue   = 0;
                plogPat->palPalEntry[i+10].peGreen  = (i+1-iFirst)*iInc;
                plogPat->palPalEntry[i+10].peRed    = 255-(i+1-iFirst)*iInc;
                plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
                continue;
            }
            plogPat->palPalEntry[i+10].peBlue   = (i+1-iSecond)*iInc;
            plogPat->palPalEntry[i+10].peGreen  = 255-(i+1-iSecond)*iInc;
            plogPat->palPalEntry[i+10].peRed    = 0;
            plogPat->palPalEntry[i+10].peFlags  = PC_RESERVED;
        }
    }

    if ((hPal = CreatePalette(plogPat)) == (HPALETTE) NULL) {
        MessageBox(hMainWindow, "Failed in creating palette!", "Error", MB_OK);
        bSuccess = FALSE;
        goto ErrExit2;
    }

    hPalOld = SelectPalette(hDC, hPal, FALSE);
    RealizePalette(hDC);
    //SelectPalette(hDC, hPalOld, FALSE);

    bQuit = FALSE;
    while (TRUE && !bQuit) {
        if (!pInfo->bClrCycle) {
            //sprintf( gtext,"NOT to animate palette\n");
            //OutputDebugString( gtext );
            bQuit = TRUE;
        }

        if (!bQuit) {
            //sprintf( gtext,"About to animate palette\n");
            //OutputDebugString( gtext );


            peTemp = plogPat->palPalEntry[10];
            for (i = 0; i < iNumClr; i++) {
                plogPat->palPalEntry[i+10] = plogPat->palPalEntry[i+10+1];
            }
            plogPat->palPalEntry[i+10] = peTemp;

            Sleep(25);

            if (!AnimatePalette(hPal, 0, 256, plogPat->palPalEntry)) {
                sprintf( gtext,"Anmiate palette failed\n");
                OutputDebugString( gtext );
            }

            if ((uRC = RealizePalette(hDC)) == -1) {
                sprintf( gtext,"Realize palette failed\n");
                OutputDebugString( gtext );
            }

            if (!UpdateColors(hDC)) {
                sprintf( gtext,"Update Colors failed\n");
                OutputDebugString( gtext );
            }

        }

    }

    SelectPalette(hDC, hPalOld, 0);
    DeleteObject(hPal);
ErrExit2:
    GlobalFree(plogPat);
ErrExit1:
    return bSuccess;

}
