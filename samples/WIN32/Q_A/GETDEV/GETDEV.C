
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
*  getdev.c -- sample program demonstrating the getdev... APIs
*
*  In this sample the main window is a dialog box.  There is no need to
*   register a new window class or create a new window.  Instead just call
*   DialogBox() and use the template defined in the .RC file.  All of the
*   interesting code is thus in the window procedure for the dialog box.
*
*  The dialog template currently specifies a "monospaced font."  This makes
*   the dialog look somewhat odd, but makes text formatting much easier.  If
*   the specified font does not exist on the system running this sample, the
*   program will work fine, but the contents of the listbox will not look
*   very good.
*
\**************************************************************************/

#include <windows.h>
#include "getdev.h"


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
int ret;

    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow);

    ret = DialogBox (hInstance, "getdevDlg", NULL, (DLGPROC)MainDlgProc);
    return ret;
}



/**************************************************************************\
*
*  function:  MainDlgProc()
*
*  input parameters:  standard window procedure parameters.
*
* At initialization time, call GetDeviceCaps() repeatedly and place the
*  results in the list box.
*
\**************************************************************************/
LONG APIENTRY MainDlgProc(HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
  UNREFERENCED_PARAMETER(lParam);


  switch (message) {

    case WM_INITDIALOG: {
      int i;
      int value;
      HDC hdc;

      hdc = GetDC (hwnd);

      for (i = 0; i < NINDEX ; i++) {
        value = GetDeviceCaps (hdc , DevCaps[i].Value);
        wsprintf (buffer,"%s %8d", DevCaps[i].String, value);
        SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
        ExpandInfo (hwnd, DevCaps[i].Value, value);
      }

      ReleaseDC (hwnd,hdc);
    } break;


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


    default: return FALSE;
  } /* end switch(message) */
  return 0;
}





/**************************************************************************\
*
*  function:  ExpandInfo()
*
*  input parameters:
*   hwnd  - parent of the list box with the info.
*   index - the input value which was sent to GetDeviceCaps().
*   value - the return value from calling GetDeviceCaps().
*
*  global variables:
*   buffer - string to hold info sent to list box.
*   space  - format string to wsprintf
*
*  Some of the return values from GetDeviceCaps() are bit strings
*   where each bit has a constant defined for it in wingdi.h.  This
*   function simply translates the bits into the string constant and
*   then places that in the listbox.
*
\**************************************************************************/
VOID ExpandInfo (HWND hwnd, int index, int value)
{
  int i;

  switch (index) {

    case TECHNOLOGY   :
      for (i = 0 ; i< NDevice ; i++) {
        if (value == Device[i].Value) {
          wsprintf (buffer,space, Device[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case CURVECAPS    :
      for (i = 0 ; i< NCurveCaps ; i++) {
        if (value & CurveCaps[i].Value) {
          wsprintf (buffer,space, CurveCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case LINECAPS     :
      for (i = 0 ; i< NLineCaps ; i++) {
        if (value & LineCaps[i].Value) {
          wsprintf (buffer,space, LineCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case POLYGONALCAPS:
      for (i = 0 ; i< NPolygonalCaps ; i++) {
        if (value & PolygonalCaps[i].Value) {
          wsprintf (buffer,space, PolygonalCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case TEXTCAPS     :
      for (i = 0 ; i< NTextCaps ; i++) {
        if (value & TextCaps[i].Value) {
          wsprintf (buffer,space, TextCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case CLIPCAPS     :
      for (i = 0 ; i< NClipCaps ; i++) {
        if (value & ClipCaps[i].Value) {
          wsprintf (buffer,space, ClipCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;



    case RASTERCAPS   :
      for (i = 0 ; i< NRasterCaps ; i++) {
        if (value & RasterCaps[i].Value) {
          wsprintf (buffer,space, RasterCaps[i].String);
          SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG)buffer);
        }
      }
    break;


    default:
    break;

  } /* end switch */
}
