// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright  1993, 1994, 1995  Microsoft Corporation.  All Rights Reserved.
//
//  PROGRAM:  icmtest.c
//
//  PURPOSE:  Image Color Matching Support
//
//  PLATFORMS:  Chicago
//
//

#include <windows.h>      // required for all Windows applications  
#include <string.h>
#include <stdlib.h>       // for _splitpath            
#include <time.h>         // for performance measurement  
#include "icmtest.h"      // specific to this program  
#include "dialogs.h"      // specific to this program  
#include "resource.h"   

//
// External vars from DIB.C
//
 extern HPALETTE ghpalCurrent;   // Handle to current palette       

// ************************************************************************
//
//   Functions private to this module
//
// ************************************************************************

int PASCAL          WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL                InitApplication(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT FAR PASCAL  MainWndProc(HWND, UINT, WPARAM, LPARAM );

VOID                PaintALot(HDC hdc, int xoffset);
VOID                LabelRows(HDC hdc, int iFirstRow, int iRowHeight);
VOID                SelectRGBs(HDC hdc);
VOID                SelectPalRGBs(HDC hdc);
VOID                DeleteObjects(HDC hdc);
VOID                CheckGamutStuff(HDC hdc, int xoffset);

BOOL                MenuCommand (HWND hWnd, WPARAM wParam);
void                InitICMTESTMenu(HMENU hMenu);

// ***********************************************************************
//
// Global variables
//
// ***********************************************************************
LOGCOLORSPACE GLCS_CS = {0};
COLORSPACECONTROLENTRY galcsControl[MAX_COLORSPACES];
BOOL          gfExistenceCheck=TRUE;

HCOLORSPACE   ghGCS_CS = (HCOLORSPACE)NULL;
HCOLORSPACE   ghlcsDefault = (HCOLORSPACE)NULL;
HCOLORSPACE   ghlcsCurrent = (HCOLORSPACE)NULL;

HINSTANCE     ghInst;             // current instance 
HANDLE        myhMem        = 0;
HANDLE        ICBM2         = 0;
HANDLE        ICBM3         = 0;
HANDLE        ICBM4         = 0;
WORD          ICBM2count    = 0;
WORD          ICBM3count    = 0;
WORD          ICBM4count    = 0;
DWORD         ICM_ICM2_ID   = 0;
DWORD         gdwGamutMethod  = 1;
DWORD         gdwCurGamutMethod = 1;
HDC           ghdcCompat    = (HDC)NULL;
int           giDCType;

char          GammaBuffer[768] = {0};
char          achDeviceColorFile[130];
NPLOGPALETTE  pLogPal;
char          gszColorSpace[MAX_CHAR];
char          gszICM_INIFile[MAX_CHAR];
UINT          guiColorSpacesUsed;


BOOL          gfCheckGamut  = FALSE;  // TRUE if gamut range check is done  
BOOL          gfMatchTarget = FALSE;  // TRUE if matching to target wanted  
BOOL          gfPalRelRGBs  = FALSE;

BOOL          gfPerfPixels  = FALSE;  // TRUE if perf of pixels wanted      
BOOL          gfPerfBitmap  = FALSE;  // TRUE if perf of bitmap wanted      

WORD          gwBMPIndex    = BMP_8;
BOOL          gfICMEnabled  = FALSE;
BOOL          gfDoICM       = FALSE;

HPALETTE ghpalCurrent  = NULL;        // Handle to current palette        

BMPINFO gBMPInfo[MAX_BMP];            // Holds handles for DIBs, BMPs, etc.   
                                      // MAX_BMP must be 1 greater than the   
                                      // number of BMP's used by program in   
                                      // order to use constants as array      
                                      // indices.  See ICMTEST.H.             

HANDLE   ghTempBits  = NULL;          // buffer for gamut range checking    

HCURSOR ghcurSave;

COLORREF  gTextColorOld  = 0;
COLORREF  gBkColorOld  = 0;
COLORREF  gPixelColor  = 0;
HPEN      ghPenOld  = NULL;
HBRUSH    ghBrushOld = NULL;

// ***********************************************************************
//
// FUNCTION: WinMain
//
// PURPOSE:
//    calls initialization function, processes message loop
//
// PARAMETERS:
//    HINSTANCE Instance handle
//    HINSTANCE Intance handle of any previous instance of this application.
//    LPSTR     Command-line
//    int       Initial display state
//
// RETURN VALUE:  
//    int       Return value from PostQuitMessage
//
// COMMENTS:
//    Windows recognizes this function by name as the initial entry point
//    for the program.  This function calls the application initialization
//    routine, if no other instance of the program is running, and always
//    calls the instance initialization routine.  It then executes a message
//    retrieval and dispatch loop that is the top-level control structure
//    for the remainder of execution.  The loop is terminated when a WM_QUIT
//    message is received, at which time this function exits the application
//    instance by returning the value passed by PostQuitMessage().
//
//    If this function must abort before entering the message loop, it
//    returns the conventional value NULL.
//
// ***********************************************************************

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  MSG msg;    // message   

  nCmdShow = SW_SHOWMAXIMIZED;

  if (!InitApplication(hInstance))    // Initialize shared things   
    return(FALSE);     // Exits if unable to initialize   

  // Perform initializations that apply to a specific instance   
  if (!InitInstance(hInstance, nCmdShow))
    return(FALSE);

  // Acquire and dispatch messages until a WM_QUIT message is received.   
  while (GetMessage(&msg,     // message structure   
    NULL,   // handle of window receiving the message   
    (UINT)NULL,   // lowest message to examine      
    (UINT)NULL))  // highest message to examine     
  {
    TranslateMessage(&msg);     // Translates virtual key codes         
    DispatchMessage(&msg);      // Dispatches message to window         
  }
  return(msg.wParam);        // Returns the value from PostQuitMessage   
}


// ***********************************************************************
//
// FUNCTION: InitApplication
//
// PURPOSE:
//    Initializes window data and registers window class
//
// PARAMETERS:
//    HINSTANCE   Instance handle
//
// RETURN VALUE:  
//    BOOL  Indicator of RegisterClass function
//
// COMMENTS:
//    This function is called at initialization time only if no other
//    instances of the application are running.  This function performs
//    initialization tasks that can be done once for any number of running
//    instances.
//
//    In this case, we initialize a window class by filling out a data
//    structure of type WNDCLASS and calling the Windows RegisterClass()
//    function. Since all instances of this application use the same window
//    class, we only need to do this when the first instance is initialized.
//
// ***********************************************************************

BOOL InitApplication(HINSTANCE hInstance)
{
  WNDCLASS  wc;

// Fill in window class structure with parameters that describe the main window.   

  wc.style=(UINT)NULL;              // Class style(s)                       
  wc.lpfnWndProc=MainWndProc;       // Function to retrieve messages for    
                                    // windows of this class                
  wc.cbClsExtra=0;                  // No per-class extra data              
  wc.cbWndExtra=0;                  // No per-window extra data             
  wc.hInstance=hInstance;           // Application that owns the class      
  wc.hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor=LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground=GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName= "ICMTest";       // Name of menu resource in .RC file.   
  wc.lpszClassName="ICMTestClass";  // Name used in call to CreateWindow.   

  // Register the window class and return success/failure code.   
  return(RegisterClass(&wc));
}


// ***********************************************************************
//
// FUNCTION: InitInstance
//
// PURPOSE:
//    Saves instance handle and creates main window
//
// PARAMETERS:
//    HINSTANCE   Instance handle.
//    int         Initial display mode.
//
// RETURN VALUE:  
//    TRUE        If application initialized successfully.
//    FALSE       Otherwise.
//
// COMMENTS:
//    This function is called at initialization time for every instance of
//    this application.  This function performs initialization tasks that
//    cannot be shared by multiple instances.
//
//    In this case, we save the instance handle in a static variable and
//    create and display the main program window.
//
// ***********************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  HWND    hWnd;                       // Main window handle   
  char    szModuleName[MAX_CHAR];
  char    szDrive[4], szPath[MAX_CHAR];
  HDC     hDC;                        // Handle to DC

  // Save the instance handle in static variable, which will be used in
  // many subsequence calls from this application to Windows. 
  ghInst= hInstance;

  // Create a main window for this application instance.  
  hWnd = CreateWindow(
    "ICMTestClass",         // See RegisterClass() call   
    "ICMTest",              // Text for window title bar   
    WS_OVERLAPPEDWINDOW,    // Window style   
    CW_USEDEFAULT,          // Default horizontal position   
    CW_USEDEFAULT,          // Default vertical position   
    CW_USEDEFAULT,          // Default width   
    CW_USEDEFAULT,          // Default height   
    NULL,                   // Overlapped windows have no parent   
    NULL,                   // Use the window class menu   
    hInstance,              // This instance owns this window   
    NULL                    // Pointer not needed   
  );

  // If window could not be created, return "failure"   
  if (!hWnd)
    return(FALSE);


  // Make the window visible; update its client area; and return "success"   
  ShowWindow(hWnd, nCmdShow);     // Show the window   
  UpdateWindow(hWnd);     // Sends WM_PAINT message   


  // Get directory from which app is running   
  GetModuleFileName( hInstance, (LPSTR)szModuleName, sizeof( szModuleName ));
  _splitpath (szModuleName, szDrive, szPath, NULL, NULL );
  wsprintf(gszICM_INIFile, "%s%sICMTEST.INI", (LPSTR)szDrive, (LPSTR)szPath );

  // Fill in the default colorspace information.
  lstrcpy(galcsControl[0].szName, "Default");
  hDC = GetDC(hWnd);
  if (!hDC)
  {
    OutputDebugString("ICMTEST.C : InitInstance :  GetDC FAILED\r\n");
    return(FALSE);
  }
  ghlcsDefault = GetColorSpace(hDC);
  if (ghlcsDefault)
  {
    ghlcsCurrent = ghlcsDefault;
    galcsControl[0].hlcs = ghlcsDefault;
    GetLogColorSpace(ghlcsDefault, (LPVOID)&galcsControl[0].lcs, sizeof(LOGCOLORSPACE));
  }
  else
  {
    DebugOut("ICMTEST.C : InitInstance : GetColorSpace FAILED\r\n");
  }

  // Read INI file and create arrays of colorspaces   
  ReadCSArrayFromINI();
  strcpy(gszColorSpace, "Default");

  return(TRUE);  // Returns the value from PostQuitMessage   
}



// **********************************************************************
//
// FUNCTION: MainWndProc
//
// PURPOSE:
//    Main window procedure.
//
// PARAMETERS:
//    HWND      Handle to the dialog.
//    unsigned  Message identifier.
//    WPARAM
//    LPARAM
//
// RETURN VALUE:  
//    TRUE  if message processed
//    FALSE otherwise
//
// COMMENTS:
//    !
//
// ***********************************************************************

LRESULT FAR PASCAL MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC         hDC;
    char        szBuffer[80];
    int         xoffset=FIRST_COL;
    UINT        uiRealized;
    HPALETTE    hOldPal, hCurPal;
    time_t      tStart, tEnd;
    time_t      tElapsed;
    WORD        wCount;

  switch (message)
  {

    case WM_DESTROY:        // message: window being destroyed   
      if (myhMem) 
        GlobalFree(myhMem);
      FreeDIBs();
      WriteLCSArrayToINI();
      PostQuitMessage(0);
      break;

    case WM_CREATE:
      // Allocate space for our logical palette   
      pLogPal = (NPLOGPALETTE) LocalAlloc( LMEM_FIXED,
                (sizeof(LOGPALETTE) +
                (sizeof(PALETTEENTRY)*(MAXPALETTE))));
      // If DIB initialization fails, quit the app
      if (!InitDIBs())
        PostQuitMessage (3) ;
      break;

    case WM_ACTIVATE:
      if (!wParam)  // app. is being de-activated   
        break;
      
      // If the app. is moving to the foreground, fall through and
      // redraw full client area with the newly realized palette,
      // if the palette has changed.

    case WM_QUERYNEWPALETTE:
      
      // If palette realization causes a palette change,
      // we need to do a full redraw.
      hDC = GetDC (hWnd);
      hOldPal = SelectPalette (hDC, ghpalCurrent, 0);

      uiRealized = RealizePalette(hDC);
      SelectPalette(hDC,hOldPal,0);
      ReleaseDC(hWnd,hDC);

      if (uiRealized) 
      {
        InvalidateRect (hWnd, (LPRECT)(NULL), TRUE);
        return(TRUE);
      }
      return(FALSE);
      break;

    case WM_PALETTECHANGED:

      // if ICMTEST was not responsible for palette change and if
      // palette realization causes a palette change, do a redraw.
      if ((HWND)wParam != hWnd)
      {
        hDC = GetDC (hWnd);
        hOldPal = SelectPalette (hDC, hCurPal, 0);
        uiRealized = RealizePalette (hDC);

        if (uiRealized) 
        {
          InvalidateRect (hWnd, (LPRECT) (NULL), 1);
        }
        SelectPalette (hDC, hOldPal, 0);
        ReleaseDC (hWnd, hDC);
      }
      break;

    case WM_PAINT:
      if (gfICMEnabled)
        SetWindowText(hWnd, "ICMTEST - ICM Enabled");
      else
        SetWindowText(hWnd, "ICMTEST - ICM Disabled");
      BeginPaint(hWnd, &ps);
      ghdcCompat = CreateCompatibleDC(ps.hdc);
      hOldPal = SelectPalette (ps.hdc, ghpalCurrent, 0);
      RealizePalette(ps.hdc);
      gfDoICM = FALSE;

      // First label the rows. 
      LabelRows(ps.hdc, FIRST_ROW, 35);

      // Do no ICM. This is to see the normal state of affairs. 
      wsprintf(szBuffer,"** No ICM **");
      TextOut(ps.hdc, FIRST_COL, 20, szBuffer, lstrlen(szBuffer));

      // Set color attributes
      if (!gfPalRelRGBs)
        SelectRGBs(ps.hdc);
      else
        SelectPalRGBs(ps.hdc);

      PaintALot(ps.hdc, xoffset); 

      xoffset = xoffset + 130;

      if (!gfICMEnabled)
      {
        DeleteObjects(ps.hdc);
      }
      else
      {
        if (gfPerfPixels)
        {
          DeleteObjects(ps.hdc);
          if(!SetICMMode(ps.hdc, ICM_ON))
            DebugOut("ICMTEST.C : MainWndProc : Pix : SetICMMode(ICM_ON) FAILED\r\n");
          SetColorSpace(ps.hdc, ghlcsCurrent);
          tStart = clock();
          for (wCount=0; wCount < 500; wCount++)
          {
            SetPixel(ps.hdc, xoffset, 100, gPixelColor);
          }
          tEnd = clock();
          tElapsed = (tEnd - tStart);
          SetColorSpace(ps.hdc, ghlcsDefault);
          if (!SetICMMode(ps.hdc, ICM_OFF))
            DebugOut("ICMTEST.C : MainWndProc : Pix : SetICMMode(ICM_OFF) FAILED");
          tStart = clock();
          for (wCount=0; wCount < 500; wCount++)
          {
            SetPixel(ps.hdc, xoffset, 100, gPixelColor);
          }
          tEnd = clock();
          tElapsed -= (tEnd - tStart);
          wsprintf(szBuffer," Elapsed time = %8li ms", tElapsed);
          TextOut(ps.hdc, xoffset, 120, szBuffer, lstrlen(szBuffer));
          wsprintf(szBuffer," Pixels/sec   = %8li ",  (tElapsed != 0 ? (long)500000/tElapsed : 0));
          TextOut(ps.hdc, xoffset, 140, szBuffer, lstrlen(szBuffer));
        }
        else if (gfPerfBitmap)
        {
          LPBITMAPINFOHEADER   lpbi;
          LPSTR     pBuf;

          DeleteObjects(ps.hdc);
          if (!SetICMMode(ps.hdc, ICM_ON))
            DebugOut("ICMTEST.C : MainWndProc : BMP : SetICMMode(ICM_ON) FAILED");
          SetColorSpace(ps.hdc, ghlcsCurrent);
          tStart = clock();

          lpbi = (VOID FAR *)gBMPInfo[BMP_24_BIG].hDib;

          pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize;
          for (wCount=0; wCount< 4; wCount++)
          {
            SetDIBitsToDevice (ps.hdc, xoffset, 160, 128, 128,
                                0, 0,
                                0,
                                128,
                                pBuf, (LPBITMAPINFO)lpbi,
                                DIB_RGB_COLORS );
          }
          tEnd = clock();
          tElapsed = (tEnd - tStart);
          SetColorSpace(ps.hdc, ghlcsDefault);
          if (!SetICMMode(ps.hdc, ICM_OFF))
            DebugOut("ICMTEST.C : MainWndProc : BMP : SetICMMode(ICM_OFF) FAILED");
          tStart = clock();
          for (wCount=0; wCount< 4; wCount++)
          {
            SetDIBitsToDevice (ps.hdc, xoffset, 300, 128, 128,
                                0, 0,
                                0,
                                128,
                                pBuf,  (LPBITMAPINFO)lpbi,
                                DIB_RGB_COLORS );
            GlobalFree(GlobalAlloc(GPTR, GlobalSize(gBMPInfo[BMP_24_BIG].hDib)));
          }
          tEnd = clock();
          tElapsed -= (tEnd - tStart);
          wsprintf(szBuffer," Elapsed time = %8li ms", tElapsed);
          TextOut(ps.hdc, xoffset, 450, szBuffer, lstrlen(szBuffer));
          wsprintf(szBuffer," Pixels/sec   = %8li ", (long)4*16384000/tElapsed );
          TextOut(ps.hdc, xoffset, 470, szBuffer, lstrlen(szBuffer));
        }
        else if (!gfCheckGamut)
        {
          int   iRetVal;
          
          wsprintf(szBuffer,"* Enable ICM *");
          TextOut(ps.hdc, xoffset,  20, szBuffer, lstrlen(szBuffer));
          gfDoICM = TRUE;

          //Enable ICM. All colors should be adjusted. 
          iRetVal = SetICMMode(ps.hdc, ICM_ON);
          if((iRetVal != ICM_ON) && (iRetVal != ICM_OFF))
            DebugOut("ICMTEST.C : MainWndProc : WM_PAINT: SetICMMode FAILED\r\n");
          iRetVal = SetColorSpace(ps.hdc, ghlcsCurrent);
          if(!iRetVal)
            DebugOut("ICMTEST.C : MainWndProc : WM_PAINT: SetColorSpace FAILED\r\n");
          PaintALot(ps.hdc, xoffset);
          xoffset  = xoffset + 130;

          // Select Objects. All colors should be matching. The
          // selecting forces the realization of pens and brushes. 
          wsprintf(szBuffer,"* Create again *");
          TextOut(ps.hdc, xoffset,  20, szBuffer, lstrlen(szBuffer));
          DeleteObjects(ps.hdc);
          if (!gfPalRelRGBs)
            SelectRGBs(ps.hdc);
          else
            SelectPalRGBs(ps.hdc);
          PaintALot(ps.hdc, xoffset);
          DeleteObjects(ps.hdc);
          xoffset = xoffset + 130;

          if (gfMatchTarget) // Test out color matching to a target.
          {
            HDC hDCPrinter;

            wsprintf(szBuffer,"* ClrMtchTTrgt *");
            TextOut(ps.hdc, xoffset,  20, szBuffer, lstrlen(szBuffer));
            if (hDCPrinter = GetPrinterDC())
            {
              ColorMatchToTarget(ps.hdc,hDCPrinter,CS_ENABLE);

              if (!gfPalRelRGBs)
                SelectRGBs(ps.hdc);
              else
                SelectPalRGBs(ps.hdc);
              PaintALot(ps.hdc, xoffset);
              DeleteObjects(ps.hdc);

              ColorMatchToTarget(ps.hdc,hDCPrinter,CS_DISABLE);
              xoffset = xoffset + 130;
              DeleteDC(hDCPrinter);
            }
          }
          if (!SetICMMode(ps.hdc, ICM_OFF))
            DebugOut("ICMTEST.C : MainWndProc : Match : SetICMMode(ICM_OFF) FAILED");
          gfDoICM = FALSE;
        }
        else  //Check Gamut Stuff
        {
          CheckGamutStuff(ps.hdc, xoffset);
          DeleteObjects(ps.hdc);
        }
      }
      SelectPalette (ps.hdc, hOldPal, 0);
      DeleteDC(ghdcCompat);
      EndPaint(hWnd,&ps);
      break;

    case WM_COMMAND:  
      return(MenuCommand (hWnd, wParam));
      break;


    case WM_INITMENU:
      InitICMTESTMenu((HMENU)wParam);
      break;

    default:    // Passes it on if unproccessed      
      return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return((LRESULT)NULL);
}

// ***********************************************************************
//
// FUNCTION: InitICMTESTMenu
//
// PURPOSE:
//    To initialize all of the menu check items, etc.
//
// PARAMETERS:
//    HMENU   Handle to the menu
//
// RETURN VALUE:  
//    None.
//
// COMMENTS:
//    !
//
// ***********************************************************************

void InitICMTESTMenu(HMENU hMenu)
{
  
  // check/uncheck menu items depending on state of related flags
  CheckMenuItem(hMenu, IDM_CHECKCOLORS, 
                (gfCheckGamut ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_MATCHTOTARGET, 
                (gfMatchTarget ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_OPTIONS_EXISTENCE, 
                (gfExistenceCheck ? MF_CHECKED : MF_UNCHECKED));

  CheckMenuItem(hMenu, IDM_ENABLEICM, 
                (gfICMEnabled ? MF_CHECKED : MF_UNCHECKED));

  CheckMenuItem(hMenu, IDM_RGBS, 
                (!(gfPalRelRGBs) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_PALETTERGBS, 
                (gfPalRelRGBs  ? MF_CHECKED : MF_UNCHECKED));

  CheckMenuItem(hMenu, IDM_1BITDIB,
    ((gwBMPIndex == BMP_1) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_4BITDIB,
    ((gwBMPIndex == BMP_4) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_8BITDIB, 
                ((gwBMPIndex == BMP_8) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_16BITDIB, 
                ((gwBMPIndex == BMP_16) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_555BITDIB,
    ((gwBMPIndex == BMP_555) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_565BITDIB,
    ((gwBMPIndex == BMP_565) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_24BITDIB, 
                ((gwBMPIndex == BMP_24) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_32BITDIB,
    ((gwBMPIndex == BMP_32) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_888BITDIB,
    ((gwBMPIndex == BMP_888) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_8BITDOWNDIB,
    ((gwBMPIndex == BMP_8_DOWN) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_1BITICMDIB,
    ((gwBMPIndex == BMP_1_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_4BITICMDIB,
    ((gwBMPIndex == BMP_4_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_8BITICMDIB,
    ((gwBMPIndex == BMP_8_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_16BITICMDIB,
    ((gwBMPIndex == BMP_16_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_555BITICMDIB,
    ((gwBMPIndex == BMP_555_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_565BITICMDIB,
    ((gwBMPIndex == BMP_565_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_24BITICMDIB,
    ((gwBMPIndex == BMP_24_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_32BITICMDIB,
    ((gwBMPIndex == BMP_32_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_888BITICMDIB,
    ((gwBMPIndex == BMP_888_ICM) ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_CMYKICMDIB,
    ((gwBMPIndex == BMP_CMYK_ICM) ? MF_CHECKED : MF_UNCHECKED));

  CheckMenuItem(hMenu, IDM_PERF_PIXELS, 
                (gfPerfPixels ? MF_CHECKED : MF_UNCHECKED));
  CheckMenuItem(hMenu, IDM_PERF_BITMAP, 
                (gfPerfBitmap ? MF_CHECKED : MF_UNCHECKED));
}

// ***********************************************************************
//
// FUNCTION: MenuCommand
//
// PURPOSE:
//    Processes menu commands. 
//
// PARAMETERS:
//    HWND    Handle to window.
//    WPARAM  Control identifier.
//
// RETURN VALUE:  
//    TRUE  - if command could be processed.      !
//    FALSE - otherwise                           
//
// COMMENTS:
//    !
//
// ***********************************************************************

BOOL MenuCommand (HWND hWnd, WPARAM id)

{
  PAINTSTRUCT       ps;
  FARPROC           lpDlgProc = (FARPROC)NULL; 

  switch (id)
  {

    case IDM_FILE_OPEN:
    case IDM_FILE_SAVE:
        break;

    case IDM_FILE_PRINT_BI_UP:
      PrintICMImages(hWnd,IDM_FILE_PRINT_BI_UP);
      break;

    case IDM_FILE_PRINT_BI_DOWN:
      PrintICMImages(hWnd,IDM_FILE_PRINT_BI_DOWN);
      break;

    case IDM_FILE_PRINT_BV4_UP:
      PrintICMImages(hWnd,IDM_FILE_PRINT_BV4_UP);
      break;

    case IDM_FILE_PRINT_BV4_DOWN:
      PrintICMImages(hWnd,IDM_FILE_PRINT_BV4_DOWN);
      break;

    case IDM_FILE_PRINT_CMYK:
      PrintICMImages(hWnd,IDM_FILE_PRINT_CMYK);
      break;

    case IDM_FILE_PRINT_TESTDIBS_ALL:
      PrintICMImages(hWnd,IDM_FILE_PRINT_TESTDIBS_ALL);
      break;

    case IDM_FILE_PRINT_FACES:
      PrintICMImages(hWnd,IDM_FILE_PRINT_FACES);
      break;

    case IDM_FILE_PRINT_SUNSET:
      PrintICMImages(hWnd,IDM_FILE_PRINT_SUNSET);
      break;

    case IDM_FILE_PRINT_CHURCH:
      PrintICMImages(hWnd,IDM_FILE_PRINT_CHURCH);
      break;

    case IDM_FILE_PRINT_REDBUSH:
      PrintICMImages(hWnd,IDM_FILE_PRINT_REDBUSH);
      break;

    case IDM_FILE_PRINT_NAT_IMAGES_ALL:
      PrintICMImages(hWnd,IDM_FILE_PRINT_NAT_IMAGES_ALL);
      break;

    case IDM_FILE_PRINT_TRC_1_GRADIENTS:
      PrintICMImages(hWnd,IDM_FILE_PRINT_TRC_1_GRADIENTS);
      break;

    case IDM_FILE_PRINT_TRC_2_GRADIENTS:
      PrintICMImages(hWnd,IDM_FILE_PRINT_TRC_2_GRADIENTS);
      break;

    case IDM_FILE_PRINT_EDGE_GRADIENTS:
      PrintICMImages(hWnd,IDM_FILE_PRINT_EDGE_GRADIENTS);
      break;

    case IDM_FILE_PRINT_DIAG_GRADIENTS:
      PrintICMImages(hWnd,IDM_FILE_PRINT_DIAG_GRADIENTS);
      break;

    case IDM_OPTIONS_EXISTENCE:
      gfExistenceCheck = !gfExistenceCheck;
      CheckMenuItem(GetMenu(hWnd), IDM_OPTIONS_EXISTENCE, (gfExistenceCheck ? MF_CHECKED : MF_UNCHECKED));
      break;

    case IDM_FILE_EXIT:
      PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0L);
      break;

    case IDM_ENABLEICM:
      gfICMEnabled = !gfICMEnabled;
      break;

    case IDM_GETCOLORSPACE:
      BeginPaint(hWnd, &ps);
      ghGCS_CS = GetColorSpace(ps.hdc);
      if(ghGCS_CS == NULL)
      {
        ErrMsg("MenuCommand:  GetColorSpace returned NULL");
      }
      EndPaint(hWnd,&ps);
      break;

    case IDM_GETLOGCOLORSPACE:
      ghGCS_CS = GetColorSpace(ps.hdc);
      if(GetLogColorSpace(ghGCS_CS,&GLCS_CS,sizeof(LOGCOLORSPACE)== FALSE))
      {
        ErrMsg("MenuCommand : GetLogColorSpace FAILED");
      }
      break;

    case IDM_APIS_DEFINECOLORSPACE:
      lpDlgProc = (FARPROC)&dlgprcDefineColorSpace;
      if (lpDlgProc)
        DialogBox( ghInst, "DLG_DEFINE_LOGICAL_COLORSPACE", hWnd, (DLGPROC)lpDlgProc);
      else
        ErrMsg("Couldn't get pointer to dialog box");
      break;

    case IDM_CHECKCOLORS:
      gfCheckGamut = !gfCheckGamut;
      break;

    case IDM_MATCHTOTARGET:
      gfMatchTarget = !gfMatchTarget;
      break;

    case IDM_GETGAMMARAMP:
      BeginPaint(hWnd, &ps);
      GetDeviceGammaRamp(ps.hdc,&GammaBuffer);
      EndPaint(hWnd,&ps);
      break;

    case IDM_SETGAMMARAMP:
      BeginPaint(hWnd, &ps);
      SetDeviceGammaRamp(ps.hdc,&GammaBuffer);
      EndPaint(hWnd,&ps);
      break;

    case IDM_TEST:
      InvalidateRect(hWnd,NULL,TRUE);
      break;

    case IDM_RGBS:
      gfPalRelRGBs = FALSE;
      break;

    case IDM_PALETTERGBS:
      gfPalRelRGBs = TRUE;
      break;

    case IDM_1BITDIB:
      gwBMPIndex = BMP_1;
      break;

    case IDM_4BITDIB:
      gwBMPIndex = BMP_4;
      break;

    case IDM_8BITDIB:
      gwBMPIndex = BMP_8;
      break;

    case IDM_16BITDIB:
      gwBMPIndex = BMP_16;
      break;
        
    case IDM_555BITDIB:
      gwBMPIndex = BMP_555;
      break;
        
    case IDM_565BITDIB:
      gwBMPIndex = BMP_565;
      break;
        
    case IDM_24BITDIB:
      gwBMPIndex = BMP_24;
      break;

    case IDM_32BITDIB:
      gwBMPIndex = BMP_32;
      break;

    case IDM_888BITDIB:
      gwBMPIndex = BMP_888;
      break;

    case IDM_8BITDOWNDIB:
      gwBMPIndex = BMP_8_DOWN;
      break;

    case IDM_1BITICMDIB:
      gwBMPIndex = BMP_1_ICM;
      break;

    case IDM_4BITICMDIB:
      gwBMPIndex = BMP_4_ICM;
      break;

    case IDM_8BITICMDIB:
      gwBMPIndex = BMP_8_ICM;
      break;

    case IDM_16BITICMDIB:
      gwBMPIndex = BMP_16_ICM;
      break;

    case IDM_555BITICMDIB:
      gwBMPIndex = BMP_555_ICM;
      break;
        
    case IDM_565BITICMDIB:
      gwBMPIndex = BMP_565_ICM;
      break;
        
    case IDM_24BITICMDIB:
      gwBMPIndex = BMP_24_ICM;
      break;

    case IDM_32BITICMDIB:
      gwBMPIndex = BMP_32_ICM;
      break;

    case IDM_888BITICMDIB:
      gwBMPIndex = BMP_888_ICM;
      break;

    case IDM_CMYKICMDIB:
      gwBMPIndex = BMP_CMYK_ICM;
      break;

    case IDM_PERF_PIXELS:
      gfPerfPixels = !gfPerfPixels;
      gfPerfBitmap = FALSE; // Timing tests are exclusive
      if (gfPerfPixels)
        InvalidateRect(hWnd,NULL,TRUE);
      break;

    case IDM_PERF_BITMAP:
      gfPerfBitmap = !gfPerfBitmap;
      gfPerfPixels = FALSE; // Timing tests are exclusive
      if (gfPerfBitmap)
        InvalidateRect(hWnd,NULL,TRUE);
      break;

    case IDM_ABOUT:
      lpDlgProc = (FARPROC)&dlgprcAbout;
      if(lpDlgProc)
        DialogBox(ghInst,  "DLG_ABOUT", hWnd, (DLGPROC)lpDlgProc);
      else
        ErrMsg("Couldn't get pointer to About dialog");
      break;

    case IDM_SETCOLORPROFILE_SCREEN:
      giDCType = SCREEN_DC;
      lpDlgProc = (FARPROC)&SetDeviceProfile_DlgProc;
      if(lpDlgProc)
        DialogBox(ghInst,  "SetDeviceProfile", hWnd, (DLGPROC)lpDlgProc);
      else
        ErrMsg("Couldn't get pointer to SetDeviceProfile dialog");
      break;
        
    case IDM_SETCOLORPROFILE_PRINTER:
      giDCType = PRINTER_DC;
      lpDlgProc = (FARPROC)&SetDeviceProfile_DlgProc;
      if(lpDlgProc)
        DialogBox(ghInst,  "SetDeviceProfile", hWnd, (DLGPROC)lpDlgProc);
      else
        ErrMsg("Couldn't get pointer to SetDeviceProfile dialog");
      break;
    
    default:
      break;
  }
  return(TRUE);
}

// ***********************************************************************
//
//  FUNCTION: PaintALot
//
//  PURPOSE:
//    !
//
//  PARAMETERS:
//    HDC   Handle to device context to use.
//    int   X offset value
//
//  RETURN VALUE:  
//    None.
//
//  COMMENTS:
//    !
//
// ***********************************************************************

VOID PaintALot(HDC hdc, int xoffset)
{
  int  i,j;
  int  yoffset = 50;
  char  szBuffer[80];
  COLORREF tempPixel;
  
  // Display busy cursor
  StartWait();
  
  wsprintf(szBuffer,"Text Color");
  TextOut(hdc, xoffset + 15, yoffset, szBuffer, lstrlen(szBuffer));
  yoffset = yoffset + 35;
  wsprintf(szBuffer,"Bkcolor");
  TextOut(hdc, xoffset + 20, yoffset, szBuffer, lstrlen(szBuffer));
  yoffset = yoffset + 35;
  for(i = 0; i < 10; i++)
  {
    for(j = 0; j < 35; j++)
    {
      SetPixel(hdc, xoffset + j + 15, yoffset + i, gPixelColor);
      tempPixel = GetPixel(hdc, xoffset + j + 15, yoffset + i);
      if (gPixelColor & 0x02000000) tempPixel |= 0x02000000;
      SetPixel(hdc, xoffset + j + 51, yoffset + i, tempPixel);
    }
  }
  yoffset = yoffset + 35;

  for(i = 0; i < 20; i++)
  {
    MoveToEx(hdc, xoffset, yoffset + i, NULL);
    LineTo(hdc, xoffset + 100, yoffset + i);
  }
  yoffset = yoffset + 35;

  Rectangle(hdc, xoffset, yoffset, xoffset + 100, yoffset + 50);
  yoffset = yoffset + 60;

  DIBPaint(hdc, xoffset + 8, yoffset);

  // Restore original text and background colors
//  SetTextColor(hdc, gTextColorOld);
//  SetBkColor(hdc,gBkColorOld );
  
  // Display normal cursor
  EndWait();
}

//************ PRIVATE ROUTINES TO READ/WRITE MORE THAN 64K **************  


// ***********************************************************************
//
//  FUNCTION: lread
//
//  PURPOSE:
//    Reads data in steps of 32k till all the data has been read.
//
//  PARAMETERS:
//    int         Handle to file to read.
//    VOID far *  
//    DWORD       !
//
//  RETURN VALUE:  
//    !
//
//  COMMENTS:
//    !
//
// ***********************************************************************

DWORD PASCAL lread (int fh, VOID far *pv, DWORD ul)
{
    BYTE HUGE *hp = pv;
    DWORD     ulT = ul;


    while (ul > (DWORD)MAXREAD) 
    {
      if (_lread(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
              return(0);
      ul -= MAXREAD;
      hp += MAXREAD;
    }
    if (_lread(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
      return(0);
    return(ulT);
}

// ***********************************************************************
//
//  FUNCTION: LabelRows
//
//  PURPOSE:
//    To label the rows of the display
//
//  PARAMETERS:
//    HDC     Handle to display DC
//    int     y-value of first row
//    int     height of each row
//
//  RETURN VALUE:  
//    none
//
//  COMMENTS:
//    Depends upon the global variable gfRel
//
// ***********************************************************************

void LabelRows(HDC  hdc, int iFirstRow, int iRowHeight)
{
  char szBuffer[MAX_CHAR];
  int  iCurRow;

  iCurRow = iFirstRow;
  if (!gfPalRelRGBs)
  {
    strcpy(szBuffer,"Text color:  00808080");
    TextOut(hdc, 0,  iCurRow, szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"BkColor:     00FF0000");
    TextOut(hdc, 0,  (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Pixels:      0000FF00");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Pen color:   00FF00FF");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Brush color: 000000FF");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
  }
  else
  {
    strcpy(szBuffer,"Text color:  0200BA00");
    TextOut(hdc, 0,  iCurRow, szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"BkColor:     02710000");
    TextOut(hdc, 0,  (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Pixels:      02E61448");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Pen color:   020810BE");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
    strcpy(szBuffer,"Brush color: 0261796D");
    TextOut(hdc, 0, (iCurRow+=iRowHeight), szBuffer, lstrlen(szBuffer));
  }
  strcpy(szBuffer,"BitBlt");
  TextOut(hdc, 0, (iCurRow+=55), szBuffer, lstrlen(szBuffer));
  strcpy(szBuffer,"CreateandBitBlt");
  TextOut(hdc, 0, (iCurRow+=55), szBuffer, lstrlen(szBuffer));
  strcpy(szBuffer,"SetDIBitsToDevice");
  TextOut(hdc, 0, (iCurRow+=55), szBuffer, lstrlen(szBuffer));
  strcpy(szBuffer,"StretchDIBits");
  TextOut(hdc, 0, (iCurRow+=55), szBuffer, lstrlen(szBuffer));
  strcpy(szBuffer,"MysteryBlt");
  TextOut(hdc, 0, (iCurRow+=55), szBuffer, lstrlen(szBuffer));


}

 
VOID SelectRGBs(HDC hdc)

{
  HPEN  hPen;
  HBRUSH  hBrush;

  gTextColorOld = SetTextColor(hdc, RGB(0x80,0x80,0x80));
  gBkColorOld   = SetBkColor  (hdc, RGB(0xFF,0x00,0x00));

  gPixelColor = RGB(0x00,0xFF,0x00);

  hPen = CreatePen(PS_SOLID, 1, RGB(0xFF,0x00,0xFF));
  ghPenOld = SelectObject(hdc, hPen);

  hBrush = CreateSolidBrush(RGB(0x00,0x00,0xFF));
  ghBrushOld = SelectObject(hdc, hBrush);
}


//**************************************************************************  

VOID SelectPalRGBs(HDC hdc)

{
  HPEN  hPen;
  HBRUSH  hBrush;

  gTextColorOld = SetTextColor(hdc, PALETTERGB(0,186,0));
  gBkColorOld   = SetBkColor(hdc, PALETTERGB(113,0,0));

  gPixelColor = PALETTERGB(230,20,72);

  hPen = CreatePen(PS_SOLID, 1, PALETTERGB(8,16,190));
  ghPenOld = SelectObject(hdc, hPen);

  hBrush = CreateSolidBrush(PALETTERGB(97,121,109));
  ghBrushOld = SelectObject(hdc, hBrush);
}


// ***********************************************************************
//
//  FUNCTION: DeleteObjects
//
//  PURPOSE:
//    To delete pens and brushes from the DC.
//
//  PARAMETERS:
//    HDC     Handle to display DC
//
//  RETURN VALUE:  
//    none
//
//  COMMENTS:
//
// ***********************************************************************

VOID DeleteObjects(HDC hdc)

{
  HPEN  hPen;
  HBRUSH  hBrush;

  SetTextColor(hdc, gTextColorOld);
  SetBkColor  (hdc, gBkColorOld);

  gPixelColor = RGB(0,0,0);

  hPen = SelectObject(hdc, ghPenOld);
  DeleteObject(hPen);

  hBrush = SelectObject(hdc, ghBrushOld);
  DeleteObject(hBrush);
}

VOID CheckGamutStuff(HDC hdc, int xoffset)
{
    HDC hDCPrinter;
    HANDLE hdib;
    HBITMAP hbm;
    LPBITMAPINFOHEADER lpbi0;
    LPBITMAPINFOHEADER lpbi;
    LPBITMAPINFOHEADER lpbitb;
    int yoffset = 20;
    DWORD FAR *lpb1;
    DWORD FAR *lpb2;
    BYTE FAR *lpb3;
    DWORD FAR *lpb4;
    DWORD FAR *lpb5;
    BYTE FAR *lpb6;
    DWORD i,j;
    char szBuffer[80];

    hdib = gBMPInfo[BMP_24_BIG].hDib;
    hbm  = gBMPInfo[BMP_24_BIG].hBmp;

    lpbi0 = (LPBITMAPINFOHEADER)hdib;

    lpb1 = (DWORD FAR *)gBMPInfo[BMP_24_BIG].hDib;
    lpb2 = (DWORD FAR *)ghTempBits;
   // lpbitb = (DWORD FAR *)ghTempBits;
   lpbitb = (LPBITMAPINFOHEADER)ghTempBits;

    lpbi = (LPBITMAPINFOHEADER)lpb1;
    j = (DWORD)lpbi->biWidth * ABS(lpbi->biHeight);

    for (i=0; i < (sizeof(BITMAPINFOHEADER))/4; i++)
    *lpb2++ = *lpb1++;

    lpb3 = (BYTE FAR *)((DWORD)lpb2 + (j*2));
    lpb4=lpb1;
    lpb5=lpb2;
    lpb6=lpb3;

    // Get a printer DC for gamut matching   
    hDCPrinter = GetPrinterDC();
    if (!hDCPrinter)
    {
      DebugOut(">>>ICMTEST.C : CheckGamutStuff : Couldn't get printer DC!\r\n");
      goto CGS_exit;
    }

    if (!SetICMMode(hDCPrinter, ICM_ON))
      DebugOut("ICMTEST.C : CheckGamutStuff : SetICMMode(ICM_ON) FAILED");
    SetColorSpace(hDCPrinter, ghlcsCurrent);
    CheckColorsInGamut(hDCPrinter, lpb1, lpb3, j);
    if (!SetICMMode(hDCPrinter, ICM_OFF))
      DebugOut("ICMTEST.C : CheckGamutStuff : SetICMMode(ICM_OFF) FAILED");
    DeleteDC(hDCPrinter);

    for (i=0; i < j; i++)
    {
      *lpb2++ = ((*lpb3++ == 1) ? *lpb1 : 0x00000000 );
      lpb1 = (DWORD FAR *)(((DWORD)lpb1) + 3);
      lpb2 = (DWORD FAR *)(((DWORD)lpb2) - 1);
    }
    wsprintf(szBuffer,"* Check Colors In Gamut *");
    TextOut(hdc, xoffset, yoffset, szBuffer, lstrlen(szBuffer));
    yoffset += 30;

    SetDIBitsToDevice(hdc,
          xoffset, yoffset,
          (int)lpbi0->biWidth, ABS((int)lpbi0->biHeight),
          0,0,
          0,
          ABS((int)lpbi0->biHeight),
          (LPSTR)(LPSTR)lpbi0 + (WORD)lpbi0->biSize + PaletteSize(lpbi0),
          (LPBITMAPINFO)lpbi0,
          DIB_RGB_COLORS);
    yoffset = yoffset + ABS((int)lpbi0->biHeight) + 20;

    wsprintf(szBuffer,"* 24Bit blackened *");
    TextOut(hdc, xoffset, yoffset, szBuffer, lstrlen(szBuffer));
    yoffset += 20;

    SetDIBitsToDevice(hdc,
          xoffset, yoffset,
          (int)lpbi0->biWidth, ABS((int)lpbi0->biHeight),
          0,0,
          0,
          ABS((int)lpbi0->biHeight),
          (LPSTR)(LPSTR)lpbitb + (WORD)lpbitb->biSize + PaletteSize(lpbitb),
          (LPBITMAPINFO)lpbitb,
          DIB_RGB_COLORS);
    yoffset = yoffset + ABS((int)lpbi0->biHeight) + 20;

    wsprintf(szBuffer,"* 24Bit bad bits *");
    TextOut(hdc, xoffset, yoffset, szBuffer, lstrlen(szBuffer));
    yoffset += 20;

    for (i=0; i < j; i++)
    {
      *lpb5++ = ((*lpb6++ == 1) ? 0x00FFFFFF : 0x00000000 );
      lpb5 = (DWORD FAR *)(((DWORD)lpb5) - 1);
    }

    SetDIBitsToDevice(hdc,
          xoffset, yoffset,
          (int)lpbi0->biWidth, ABS((int)lpbi0->biHeight),
          0,0,
          0,
          ABS((int)lpbi0->biHeight),
          (LPSTR)(LPSTR)lpbitb + (WORD)lpbitb->biSize + PaletteSize(lpbitb),
          (LPBITMAPINFO)lpbitb,
          DIB_RGB_COLORS);
    yoffset = yoffset + ABS((int)lpbi0->biHeight) + 20;

CGS_exit:
//    GlobalUnlock(gBMPInfo[BMP_24_BIG].hDib);
//    GlobalUnlock(ghTempBits);
  return;
}

//*************************************************************************
//                                                                          
//  FUNCTION   : DebugOut (LPSTR sz,...)                                      
//                                                                          
//  PURPOSE    : Sends a formatted message string out the debug port.
//                                                                          
//  RETURNS    : none.                   
//                                                                          
//*************************************************************************
void DebugOut (PSTR sz,...)
{
  char ach[128];

  wvsprintf (ach, sz, (LPSTR)(&sz+1));   // Format the string
  OutputDebugString(ach);
}
