
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/***************************************************************************
 *                                                                         *
 *  PROGRAM     : MyPal.c                                                  *
 *                                                                         *
 *  PURPOSE     : Sets up a bar representation of the current physical     *
 *                palette and displays useful information regarding        *
 *                pixel colors and palette indices.                        *
 *                                                                         *
 *  FUNCTIONS   : WinMain() - calls initialization function,               *
 *                            processes message loop                       *
 *                                                                         *
 *                WndProc() - Window function for app. Processes           *
 *                            window messages.                             *
 *                                                                         *
 *              ShowColor() - Displays a little box on each side of the    *
 *                            caption bar displaying the pixel color at the*
 *                            mouse position.                              *
 ***************************************************************************/

#include <windows.h>
#include "mypal.h"

HANDLE          hPal;          /* Handle to the application's logical palette */
static INT      nSizeX;        /* Width of the application window             */
static INT      nSizeY;        /* Height of the application window            */
NPLOGPALETTE    pLogPal;       /* Pointer to program's logical palette        */
INT             nXBorder;      /* Width of window border                      */
INT             nXTitle;       /* Width of title bar                          */
INT             nYTitle;       /* Height of title bar                         */
BOOL            bCaptureOn;    /* Indicates if mouse capture is on            */
INT             iIndex;        /* Last index selected in palette              */
CHAR            szTitlebuf[90];/* Buffer for pixel and palette info. text     */
HDC             hDCGlobal;     /* The Screen DC                               */
INT             iNumColors;    /* Number of colors supported by device        */
INT             iRasterCaps;   /* Raster capabilities                         */
RECT            rClientRect;   /* Client rectangle coordinates                */
DWORD    dwPal[PALETTESIZE];   /* Stores palette entries for later lookup     */
INT             iGlobalXOffset;
INT             iGlobalYOffset;
INT             iYMiddle;

LONG APIENTRY WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam);

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : void ShowColor(HWND hWnd, HDC hDC)                         *
 *                                                                          *
 *  PURPOSE    : Displays a little box on each side of the caption bar      *
 *               displaying the pixel color at the mouse position.          *
 *                                                                          *
 ****************************************************************************/
VOID ShowColor (
        HWND  hWnd,
        HDC   hDC)
{
     HBRUSH  hBrush, hOldBrush;

     hBrush    = CreateSolidBrush ( PALETTEINDEX(iIndex) );
     hOldBrush = SelectObject (hDC,hBrush) ;

     GetWindowRect (hWnd, (LPRECT)&rClientRect);

     PatBlt ( hDC,
              rClientRect.left + nXTitle + nXBorder + 1,
              rClientRect.top + nXBorder,
              nXTitle,
              nYTitle,
              PATCOPY);

     PatBlt(hDC,
            rClientRect.right - ( 3 * nXTitle + nXBorder + 2),
            rClientRect.top + nXBorder,
            nXTitle,
            nYTitle,
            PATCOPY);
     SelectObject (hDC, hOldBrush);
     DeleteObject (hBrush) ;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : WinMain(HANDLE, HANDLE, LPSTR, int)                        *
 *                                                                          *
 *  PURPOSE    : Creates the app. window and processes the message loop.    *
 *                                                                          *
 ****************************************************************************/
int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{
     static CHAR szAppName [] = "MyPal";
     HWND        hWnd;
     WNDCLASS    wndclass;
     MSG         msg ;
     INT         xScreen;
     INT         yScreen;

     UNREFERENCED_PARAMETER( lpCmdLine );

     if (!hPrevInstance){
         wndclass.style         = CS_HREDRAW | CS_VREDRAW;
         wndclass.lpfnWndProc   = (WNDPROC) WndProc;
         wndclass.cbClsExtra    = 0;
         wndclass.cbWndExtra    = 0;
         wndclass.hInstance     = hInstance;
         wndclass.hIcon         = LoadIcon(hInstance, szAppName);
         wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
         wndclass.hbrBackground = GetStockObject (BLACK_BRUSH);
         wndclass.lpszMenuName  = szAppName;
         wndclass.lpszClassName = szAppName;

         if (!RegisterClass (&wndclass))
             return FALSE ;
     }

     /* Do some global initializations */
     xScreen     = GetSystemMetrics (SM_CXSCREEN);
     yScreen     = GetSystemMetrics (SM_CYSCREEN);
     nXBorder    = (INT)GetSystemMetrics (SM_CXFRAME);
     nXTitle     = (INT)GetSystemMetrics (SM_CXSIZE);
     nYTitle     = (INT)GetSystemMetrics (SM_CYSIZE);
     iIndex      = 0;
     bCaptureOn  = FALSE;

     hDCGlobal   = GetDC (NULL);
     iRasterCaps = GetDeviceCaps(hDCGlobal, RASTERCAPS);
     iRasterCaps = (iRasterCaps & RC_PALETTE) ? TRUE : FALSE;

     if (iRasterCaps)
         iNumColors = GetDeviceCaps(hDCGlobal, SIZEPALETTE);
     else
         iNumColors = GetDeviceCaps( hDCGlobal, NUMCOLORS);
     ReleaseDC (NULL,hDCGlobal);

     nSizeX = ((xScreen - 2*nXBorder) / PALETTESIZE) * PALETTESIZE;

     /* create the app. window */
     hWnd = CreateWindow (szAppName,
                          "My Physical Palette ",
                          WS_OVERLAPPEDWINDOW,
                          (xScreen-nSizeX) / 2 - nXBorder,
                          yScreen - ( 4 * GetSystemMetrics (SM_CYCAPTION)),
                          nSizeX + 2 * nXBorder,
                          4 * GetSystemMetrics (SM_CYCAPTION),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
     ShowWindow (hWnd, nCmdShow);
     UpdateWindow (hWnd);

     while (GetMessage (&msg, NULL, 0, 0)){
           TranslateMessage (&msg) ;
           DispatchMessage (&msg) ;
     }

     return msg.wParam ;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)                             *
 *                                                                            *
 *  PURPOSE:  Processes window messages and sets up a 256 bar representation  *
 *            of the current physical palette. Specifically, in response to:  *
 *                                                                            *
 *               WM_CREATE  -Allocates for and sets up a LOGPALETTE           *
 *                           structure, creates a logical palette the same    *
 *                           size as the physical palette and obtains a       *
 *                           handle to the logical palette.                   *
 *                                                                            *
 *               WM_DESTROY -Destroys the logical palette and shuts down app. *
 *                                                                            *
 *               WM_PAINT   -Resizes client area to hold as many vertical     *
 *                           color bars as there are physical palette entries.*
 *                           Also realises the current logical palette and    *
 *                           draws one color bar corresponding to each        *
 *                           palette entry                                    *
 *                                                                            *
 *           WM_RBUTTONDOWN -Captures the mouse and initiates the below       *
 *                           process:                                         *
 *                                                                            *
 *           WM_MOUSEMOVE   -Following a WM_RBUTTONDOWN, if the right mouse   *
 *                           key is depressed, displays info about the        *
 *                           pixel RGB value and palette index of the mouse   *
 *                           coordinates.                                     *
 *                                                                            *
 *           WM_RBUTTONUP   -Release mouse capture and terminates the above   *
 *                           process                                          *
 *                                                                            *
 *           WM_LBUTTONDOWN -Determines and displays the palette index and    *
 *                           RGB value of the bar under the mouse.            *
 *                                                                            *
 *           WM_KEYDOWN     -Allows use of the arrow keys in stepping thro'   *
 *                           palette entries.                                 *
 *                                                                            *
 *****************************************************************************/
LONG APIENTRY WndProc (
    HWND         hWnd,
    UINT     iMessage,
    UINT         wParam,
    LONG         lParam)
{
    HDC           hDC;
    PAINTSTRUCT   ps;
    INT           iLoop;
    INT           nStart;
    HBRUSH        hBrush;
    HBRUSH        hOldBrush;

    MPOINT        pt;
    static INT    nIncr;
    static DWORD  dwColor;
    static DWORD  dwLastColor;
    static INT    i, x;

    switch (iMessage) {
         case WM_DESTROY:
              /* delete the handle to the logical palette if it has any
               * color entries and quit.
               */
              if (pLogPal->palNumEntries)
                  DeleteObject (hPal);
              PostQuitMessage (0) ;
              break ;

         case WM_CREATE:
              /* Allocate enough memory for a logical palette with
               * PALETTESIZE entries and set the size and version fields
               * of the logical palette structure.
               */
              pLogPal = (NPLOGPALETTE) LocalAlloc (LMEM_FIXED,
                                                  (sizeof (LOGPALETTE) +
                                                  (sizeof (PALETTEENTRY) * (PALETTESIZE))));

          if(!pLogPal){
            MessageBox(hWnd, "<WM_CREATE> Not enough memory for palette.", NULL, MB_OK | MB_ICONHAND);
                PostQuitMessage (0) ;
            break;
          }

              pLogPal->palVersion    = 0x300;
              pLogPal->palNumEntries = PALETTESIZE;

              /* fill in intensities for all palette entry colors */
              for (iLoop = 0; iLoop < PALETTESIZE; iLoop++) {
                  *((WORD *) (&pLogPal->palPalEntry[iLoop].peRed)) = (WORD)iLoop;
                  pLogPal->palPalEntry[iLoop].peBlue  = 0;
                  pLogPal->palPalEntry[iLoop].peFlags = PC_EXPLICIT;
              }

              /*  create a logical color palette according the information
               *  in the LOGPALETTE structure.
               */
              hPal = CreatePalette ((LPLOGPALETTE) pLogPal) ;
              break;

         case WM_GETMINMAXINFO:

              ((LPRGPT)lParam)->iInfo[6] = nXBorder * 2 + PALETTESIZE;
              ((LPRGPT)lParam)->iInfo[7] = nXBorder * 2 + nYTitle*3;

              return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
              break;

         case WM_PAINT:

              /* Divide client width into equal-sized parts, one per palette
               * entry, and re-calculate client width so that it will display
               * exactly as many vertical bars as there are palette entries.
               */
               GetClientRect(hWnd,(LPRECT) &rClientRect);
               nSizeX = (rClientRect.right - rClientRect.left);
               nSizeX = (nSizeX/iNumColors) * iNumColors;

               nSizeY = rClientRect.bottom - rClientRect.top;
               GetWindowRect(hWnd,(LPRECT) &rClientRect);

              /* Adjust window width so that it can display exactly
               * as many vertical bars( of equal width) as there are palette
               * colors.
               */

              SetWindowPos( hWnd,
                            (HWND)NULL,
                            0,
                            0,
                            nSizeX + 2*nXBorder,
                            rClientRect.bottom - rClientRect.top,
                            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

              hDC = BeginPaint(hWnd, &ps);

              /* Select the palette into the window device context and
               * make the Palette Manager map the logical palette to the
               * system palette (realize it).
               */
              SelectPalette (hDC, hPal, 1);
              RealizePalette (hDC);

              /* Calculate width of each color bar to be displayed */
              nIncr = nSizeX / iNumColors;

              /* Paint the individual bars separately on the app. window */
              for (nStart = iLoop = 0; iLoop < iNumColors; iLoop++){

                  /* Since this app. uses a logical palette, use the
                   * PALETTEINDEX macro to specify the palette entry
                   * index instead of using an explicit RGB value.
                   */
                  hBrush       = CreateSolidBrush (PALETTEINDEX (iLoop));
                  dwPal[iLoop] = GetNearestColor (hDC, PALETTEINDEX (iLoop) );
                  hOldBrush    = SelectObject (hDC,hBrush) ;
                  PatBlt (hDC, nStart, 0, nIncr, nSizeY, PATCOPY);
                  nStart       += nIncr;
                  SelectObject (hDC, hOldBrush);
                  DeleteObject (hBrush) ;
              }
              wsprintf (szTitlebuf, "MyPal Colors= %d", iNumColors);
              SetWindowText (hWnd, (LPSTR)szTitlebuf);

              EndPaint(hWnd,&ps);

              break ;

         case WM_MOUSEMOVE:

              if (wParam & MK_RBUTTON) {

          POINT pt;
                  
#ifdef WIN16
                  /* Convert mouse position to screen coordinates */
                  pt.x = LOWORD(lParam);
                  pt.y = HIWORD(lParam);
#else
                  LONG2POINT(lParam, pt);       
#endif
                  ClientToScreen(hWnd, &pt);

                  /* Get RGB value (color) of pixel under mouse coordinate */
                  dwColor = GetPixel(hDCGlobal, pt.x, pt.y);

                  /* If color value already exists in palette lookup table,
                   * obtain it's index.
                   */
                  for (i=0 ; i < iNumColors ; i++)
                      if ( dwColor == dwPal[i] )
                          break;
                  iIndex = i;

                  /* If previous color value was not identical to current one,
                   * display color boxes on either side of title bar,
                   * the R, G, B values and palette index of current color.
                   */
                  if (dwColor != dwLastColor) {
                      wsprintf ( szTitlebuf,
                                 "MyPal Colors=%d  Index=%d  R=%3u G=%3u B=%3u",
                                 iNumColors,
                                 iIndex,
                                 (WORD)(BYTE) GetRValue (dwColor),
                                 (WORD)(BYTE) GetGValue (dwColor),
                                 (WORD)(BYTE) GetBValue (dwColor));
                      SetWindowText (hWnd, (LPSTR)szTitlebuf);
                      ShowColor (hWnd, hDCGlobal);
                      dwLastColor = dwColor;
                  }
              }
              break;

         case WM_RBUTTONDOWN:

              /* Determine number of color bar under mouse, thus the index
               * of color in palette.
               */
              x = LOWORD(lParam);
              iIndex = (x / nIncr );

              wsprintf ( szTitlebuf,
                         "MyPal Colors=%d  Index=%d  PalSize=%d RasterCaps:%d",
                         iNumColors,
                         iIndex,
                         iNumColors,
                         iRasterCaps );

              SetWindowText (hWnd, (LPSTR)szTitlebuf);

              /* Set mouse capture so that subsequent WM_MOUSEMOVEs
               * (with right mouse button depressed) will allow MyPal
               * to display RGB info anywhere on the screen without losing
               * the focus.
               */
              SetCapture (hWnd);
              bCaptureOn = TRUE;
              hDCGlobal = GetDC(NULL);
              if (hPal) {
                  SelectPalette (hDCGlobal, hPal, FALSE);
                  RealizePalette (hDCGlobal);
              }
              break;

         case WM_RBUTTONUP:
              /* Stops displaying RGB and palette info and releases mouse
               * capture
               */
              ReleaseDC (NULL, hDCGlobal);
              bCaptureOn = FALSE;
              ReleaseCapture ();
              break;

         case WM_MOVE:
              /* If you have a wide column, this adds 1/2 so X is centered */
              iGlobalXOffset  = LOWORD (lParam);
              iGlobalYOffset  = HIWORD (lParam) + nXBorder;
              break;

         case WM_SIZE:
              iYMiddle = (HIWORD (lParam)/2);
              break;

         case WM_LBUTTONDOWN:
         case WM_KEYDOWN:

             if (iMessage == WM_LBUTTONDOWN){
                 /* determine which column was hit by the mouse */
                 x = LOWORD(lParam);
                 iIndex = (x / nIncr );
             }
             else{
                 /* Use arrow keys to step thro' the palette entries */
                 switch (wParam) {
                     case VK_RIGHT:
                     case VK_UP:
                                  /* go to next (higher) palette entry */
                                  iIndex++;
                                  break;
                     case VK_LEFT:
                     case VK_DOWN:
                                  /* go to previous (lower) palette entry */
                                  iIndex--;
                                  break;
                     case VK_NEXT:
                                  iIndex += 10;
                                  break;
                     case VK_PRIOR:
                                  iIndex -= 10;
                                  break;
                     case VK_HOME:
                                  /* go to first palette entry */
                                  iIndex = 0;
                                  break;
                     case VK_END:
                                  /* go to last palette entry */
                                  iIndex = iNumColors-1;
                                  break;
                     default:
                                  return 0L;
                                  break;
                 }
                 /* Make sure the palette index is within range else
                  * set it to the limiting values and give a warning beep.
                  */
                 if (iIndex < 0) {
                     iIndex = 0;
                     MessageBeep(1);
                 }
                 else{
                     if (iIndex > iNumColors-1) {
                         iIndex = iNumColors-1;
                         MessageBeep(1);
                      }
                 }

                 pt.x = (SHORT)((iIndex * nIncr) +
                        iGlobalXOffset   +
                        ((nIncr > 1) ? (nIncr / 2) : 1));
                 pt.y = (SHORT)(iYMiddle + iGlobalYOffset);

                 SetCursorPos (pt.x, pt.y);
             }

             if (TRUE == bCaptureOn) {
                 MessageBeep(1);
                 break;
             }

             /* Select & realize the palette or the colors > 0x7
              * will not match up.
              */
             hDC = GetDC(NULL);
             SelectPalette  (hDC, hPal, 1);
             RealizePalette (hDC) ;

             dwColor = GetNearestColor (hDC, PALETTEINDEX (iIndex));

             wsprintf ( szTitlebuf,
                        "MyPal Colors=%d  Index=%d  R=%3u G=%3u B=%3u",
                        iNumColors,
                        iIndex,
                        (WORD)(BYTE)GetRValue (dwColor),
                        (WORD)(BYTE)GetGValue (dwColor),
                        (WORD)(BYTE)GetBValue (dwColor)
                     );

             SetWindowText (hWnd, (LPSTR)szTitlebuf);
             ShowColor (hWnd,hDC);
             ReleaseDC(NULL, hDC);
             break;

         default:
              return DefWindowProc (hWnd, iMessage, wParam, lParam) ;

    }
    return 0L ;
}
