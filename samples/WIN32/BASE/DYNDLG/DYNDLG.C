/**************************************************************************\
*  Simple sample to show how to dynamically create Win32 dialogs.
*
*         Steve Firebaugh
*         Microsoft Developer Support
*         Copyright (c) 1992, 1993 Microsoft Corporation
*
*
\**************************************************************************/

#define UNICODE
#include <windows.h>
#include "dyndlg.h"

LRESULT APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
LRESULT APIENTRY About(HWND, UINT, WPARAM, LPARAM );

int Create1(HWND);
int Create2(HWND);

HINSTANCE ghInst;

/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    HWND   hwnd;
    MSG    msg;
    HANDLE hLibrary;

    UNREFERENCED_PARAMETER( lpCmdLine );

    ghInst = hInstance;

    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {
        WNDCLASS  wc;

        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)MainWndProc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, TEXT("dyndlgIcon"));
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszMenuName = TEXT("dyndlgMenu");
        wc.lpszClassName = TEXT("dyndlg");

        if (!RegisterClass(&wc)) return (FALSE);
    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hwnd = CreateWindow(
        TEXT("dyndlg"),
        TEXT("dyndlg"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd) return (FALSE);


    /***** CUSTOM CONTROL
    * Load the DLL containing the custom control.
    *****/
    hLibrary = LoadLibrary (TEXT("..\\spincube\\SPINCUBE.DLL"));
    if (hLibrary == NULL)
      MessageBox (hwnd, TEXT("LoadLibrary (..\\spincube\\SPINCUBE.DLL) failed"),
                  TEXT("Error, this app requires spincube."), MB_OK | MB_ICONEXCLAMATION);
    /***** CUSTOM CONTROL *****/


    /* Demo: Just for fun, start out with one of the dialogs created. */
    PostMessage (hwnd, WM_COMMAND, IDM_DIALOG2, 0);

    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (hLibrary != NULL) FreeLibrary (hLibrary);

    return (msg.wParam);
}










/***************************************************************************\
*    FUNCTION: MainWndProc
\***************************************************************************/
LRESULT APIENTRY MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {

    /**********************************************************************\
    *  Menu item support.
    *
    \**********************************************************************/
    case WM_COMMAND:
      switch (LOWORD(wParam)) {

        case IDM_DIALOG1:
          Create1 (hwnd);
        break;

        case IDM_DIALOG2:
          Create2 (hwnd);
        break;

        case IDM_HELP:
          WinHelp( hwnd, TEXT("dyndlg.hlp"), HELP_INDEX, (DWORD) NULL );
        break;

        case IDM_ABOUT:
          DialogBox (GetModuleHandle(NULL), TEXT("aboutBox"), hwnd, (DLGPROC)About);
        return 0;

      }  /* end switch */
    break;  /* end wm_command */



    case WM_DESTROY:
      WinHelp( hwnd,  TEXT("dyndlg.hlp"), (UINT) HELP_QUIT, (DWORD) NULL );
      PostQuitMessage(0);
    break;

    } /* end switch */
    return (DefWindowProc(hwnd, message, wParam, lParam));
}


/****************************************************************************
    FUNCTION: About
****************************************************************************/
LRESULT CALLBACK About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if ((message == WM_COMMAND) && (LOWORD(wParam) == IDOK)) {
    EndDialog (hwnd, TRUE);
    return TRUE;
  }
  if ((message == WM_SYSCOMMAND) && (wParam == SC_CLOSE)) {
    EndDialog (hwnd, TRUE);
    return TRUE;
  }
  return FALSE;
}








/*+++


    Create the first dialog dynamically.  Notice that we are NOT using
    structures here because too many of the fields are of variable length.
    Instead, just allocate some memory to play with, and start filling in
    the data at that pointer.

    p - pointer which is moved down through the DLGTEMPLATE information.
    pdlgtemplate - pointer to the TOP of the DLGTEMPLATE information.


    Notice that UNICODE is defined to be on in this module.  That means:
      1.  All strings included in TEXT() macro will be made unicode strings
        by the compiler.
      2.  wsprintf() will accept a unicode string as input, and will fill
        its lpOut buffer with a unicode string.  Notice that in any case,
        the return value is the number of *characters*  not the number of
        bytes.
      3.  Any system call which may be dependent upon unicode will be mapped
        to its wide character version (*W not *A) by the header files.
        Notice that this does not matter for the CreateDialogIndirect() call.
        Both the A and W versions expect the dialog template to contain wide
        character strings.


    Here we create a simple dialog with one item.  The dialog has a title,
    the item has text, and the item class is specified by ordinal.  There
    is no font information.

---*/

Create1(HWND hwnd)
{
WORD *p, *pdlgtemplate;
int nchar;

  /* declare variables purely for ease of reading the names provide. */
  DWORD   lStyle;
  DWORD   lExtendedStyle;
  WORD    NumberOfItems;
  WORD    x;
  WORD    y;
  WORD    cx;
  WORD    cy;

  WORD    wId;

  /* allocate some memory to play with  */
  pdlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

  lStyle              = DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
  lExtendedStyle      = 0;
  NumberOfItems       = 1;
  x                   = 10;
  y                   = 10;
  cx                  = 100;
  cy                  = 100;

  /* start to fill in the dlgtemplate information.  addressing by WORDs */
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = LOWORD (lExtendedStyle);
  *p++ = HIWORD (lExtendedStyle);
  *p++ = NumberOfItems;
  *p++ = x ;
  *p++ = y ;
  *p++ = cx;
  *p++ = cy;
  *p++ = 0;     // Menu
  *p++ = 0;     // Class

  /* copy the title of the dialog, null terminate the string. */
  nchar = wsprintf (p, TEXT("Title 1"));
  p += nchar;
  *p++ = 0;

  /* add in the wPointSize and szFontName here iff the DS_SETFONT bit on */

  /* make sure the first item starts on a DWORD boundary */
  { ULONG l;

  l = (ULONG) p;
  l +=3;
  l >>=2;
  l <<=2;
  p = (PWORD) l;
  }


  /* now start with the first item */
  lStyle              = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD;
  x                   = 10;
  y                   = 70;
  cx                  = 80;
  cy                  = 20;
  wId                 = IDOK;


  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = LOWORD (lExtendedStyle);
  *p++ = HIWORD (lExtendedStyle);
  *p++ = x ;
  *p++ = y ;
  *p++ = cx;
  *p++ = cy;
  *p++ = wId;

  /* fill in class i.d. Button in this case */
  *p++ = (WORD)0xffff;
  *p++ = (WORD)0x0080;

  /* copy the text of the first item, null terminate the string. */
  nchar = wsprintf (p, TEXT("OK"));
  p += nchar;
  *p++ = 0;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  CreateDialogIndirect (ghInst, (LPDLGTEMPLATE) pdlgtemplate, hwnd, (DLGPROC) About);

  LocalFree (LocalHandle (pdlgtemplate));

  return 0;
}







/*+++


    Create the second dialog dynamically.

    Here we create a dialog which has font information (DS_SETFONT),
    and which has two items with the item class specified by name.


---*/
Create2(HWND hwnd)
{
WORD *p, *pdlgtemplate;
int nchar;

  /* declare variables purely for ease of reading the names provide. */
  DWORD   lStyle;
  DWORD   lExtendedStyle;
  WORD    NumberOfItems;
  WORD    x;
  WORD    y;
  WORD    cx;
  WORD    cy;

  WORD    wId;

  /* allocate some memory to play with  */
  pdlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

  lStyle              = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
  lExtendedStyle      = 0;
  NumberOfItems       = 2;
  x                   = 210;
  y                   = 10;
  cx                  = 100;
  cy                  = 100;

  /* start to fill in the dlgtemplate information.  addressing by WORDs */
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = LOWORD (lExtendedStyle);
  *p++ = HIWORD (lExtendedStyle);
  *p++ = NumberOfItems;
  *p++ = x ;
  *p++ = y ;
  *p++ = cx;
  *p++ = cy;
  *p++ = 0;     // Menu
  *p++ = 0;     // Class

  /* copy the title of the dialog, null terminate the string. */
  nchar = wsprintf (p, TEXT("Title 2"));
  p += nchar;
  *p++ = 0;

  /* Font information because of DS_SETFONT */
  *p++ = 18;     // point size
  nchar = wsprintf (p, TEXT("Times New Roman"));  // Face name
  p += nchar;
  *p++ = 0;


  /* make sure the first item starts on a DWORD boundary */
  { ULONG l;

  l = (ULONG) p;
  l +=3;
  l >>=2;
  l <<=2;
  p = (PWORD) l;
  }


  /* now start with the first item */
  lStyle              = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP;
  x                   = 10;
  y                   = 60;
  cx                  = 80;
  cy                  = 20;
  wId                 = IDOK;


  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = LOWORD (lExtendedStyle);
  *p++ = HIWORD (lExtendedStyle);
  *p++ = x ;
  *p++ = y ;
  *p++ = cx;
  *p++ = cy;
  *p++ = wId;


  /* fill in class i.d., this time by name */
  nchar = wsprintf (p, TEXT("BUTTON"));
  p += nchar;
  *p++ = 0;


  /* copy the text of the first item, null terminate the string. */
  nchar = wsprintf (p, TEXT("OK"));
  p += nchar;
  *p++ = 0;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  /* make sure the second item starts on a DWORD boundary */
  { ULONG l;

  l = (ULONG) p;
  l +=3;
  l >>=2;
  l <<=2;
  p = (PWORD) l;
  }

#define SS_INMOTION 0x0002  /* from spincube.h */
  lStyle              = WS_VISIBLE | WS_CHILD | SS_INMOTION;
  x                   = 20;
  y                   = 5;
  cx                  = 65;
  cy                  = 45;
  wId                 = 57;


  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = LOWORD (lExtendedStyle);
  *p++ = HIWORD (lExtendedStyle);
  *p++ = x ;
  *p++ = y ;
  *p++ = cx;
  *p++ = cy;
  *p++ = wId;


  /* fill in class i.d., this time by name */

  /***** CUSTOM CONTROL
  * Fill in the class name that is specified in the DLL
  *  See the \q_a\samples\spincube sample for the source to this.
  *****/
  nchar = wsprintf (p, TEXT("Spincube"));
  p += nchar;
  *p++ = 0;

  /* copy the text of the second item, null terminate the string. */
  nchar = wsprintf (p, TEXT(""));
  p += nchar;
  *p++ = 0;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  CreateDialogIndirect (ghInst, (LPDLGTEMPLATE) pdlgtemplate, hwnd, (DLGPROC) About);

  LocalFree (LocalHandle (pdlgtemplate));

  return 0;
}
