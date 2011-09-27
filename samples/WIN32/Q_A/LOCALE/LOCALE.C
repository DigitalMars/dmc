
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  locale.c -- sample program demonstrating the locale... APIs
*
*  In this sample the main window is a dialog box.  There is no need to
*   register a new window class or create a new window.  Instead just call
*   DialogBox() and use the template defined in the .RC file.  All of the
*   interesting code is thus in the window procedure for the dialog box.
*   In this case, simply respond to the combobox command messsages and fill
*   the list box with appropriate values.
*
\**************************************************************************/

#define UNICODE


#include <windows.h>
#include "locale.h"


/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
int ret;

    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow);

// check if running on Windows NT, if not, display notice and terminate
    if( GetVersion() & 0x80000000 )
    {
       MessageBoxA( NULL,
           "This sample application can only be run on Windows NT.\n"
           "This application will now terminate.",
           "Locale",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
    }

    ret = DialogBox (hInstance, TEXT("localeDlg"), NULL, (DLGPROC)MainDlgProc);
    return ret;
}



/**************************************************************************\
*
*  function:  MainDlgProc()
*
*  input parameters:  standard window procedure parameters.
*
\**************************************************************************/
LRESULT CALLBACK MainDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int i;
  TCHAR buffer[MAXTCHAR];

  UNREFERENCED_PARAMETER(lParam);


  switch (message) {

    /********************************************************************\
    * WM_INITDIALOG
    *
    * Fill the combobox with the language, sublanguage pairs.
    *
    \********************************************************************/
    case WM_INITDIALOG:

      /* fill up the combobox with the possible input values to
       *  GetLocaleInfoW().  The strings are in a lookup table
       *  in the file locale.h
       */
      for (i = 0; i < NLANG; i++) {

        SendDlgItemMessage (hwnd, DID_COMBOBOX, CB_ADDSTRING, 0,
            (LPARAM)langstrings[i].String);
      }

      /* Attempt to create a unicode font for the listbox with the
       *  same size as the default system font.  This will work well
       *  if the uclucida.ttf font was installed with the control panel.
       *  If not, then some of the characters in the strings returned
       *  by GetLocaleInfoW() will be displayed as the 'missing' font glyph.
       */
      { HFONT   hfont;
        LOGFONT  logfont;

        GetObject (GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &logfont);
        logfont.lfCharSet = UNICODE_CHARSET;
        wsprintf (logfont.lfFaceName, TEXT("Lucida Sans Unicode"));
        hfont = CreateFontIndirect (&logfont);
        SendDlgItemMessage (hwnd, DID_LISTBOX, WM_SETFONT, (WPARAM) hfont, 0);
      }
    return 0;


    /********************************************************************\
    * WM_SYSCOMMAND
    *
    * ignore all syscommand messages, except for SC_CLOSE.
    *  on this one, call EndDialog().
    \********************************************************************/
    case WM_SYSCOMMAND:
      if (wParam == SC_CLOSE) {
        EndDialog (hwnd, TRUE);
        return TRUE;
      } else
        return FALSE;
    break;



    /********************************************************************\
    * WM_COMMAND
    *
    * When the different languages are selected, clear the list box,
    *  then fill it up again with the new information.
    *
    \********************************************************************/
    case WM_COMMAND:
      if ((HIWORD(wParam) == CBN_SELCHANGE) && (LOWORD(wParam) == DID_COMBOBOX)) {
        int iSel;
        TCHAR linfo[MAXTCHAR];

        iSel = SendMessage ((HWND) lParam, CB_GETCURSEL, 0,0);
        SendDlgItemMessage (hwnd, DID_LISTBOX, LB_RESETCONTENT, 0, 0);

        for (i = 0; i < NLOCALEENTRIES; i++) {
          linfo[0] = 0;
          GetLocaleInfoW (MAKELCID( langstrings[iSel].Value, 0 ),
                  localeentry[i].Value, linfo, MAXTCHAR);

          wsprintf (buffer, TEXT("%s \t %s"), localeentry[i].String, linfo);

          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer);
        }

      }
    break; /* end WM_COMMAND */


    default: return FALSE;
  } /* end switch(message) */
}
