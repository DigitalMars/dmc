
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
*  getsys.c -- sample program demonstrating the GetSys... APIs
*
*  In this sample the main window is a dialog box.  There is no need to
*   register a new window class or create a new window.  Instead just call
*   DialogBox() and use the template defined in the .RC file.  All of the
*   interesting code is thus in the window procedure for the dialog box.
*   In this case, simply respond to the button command messsages and fill
*   the list box with appropriate values.
*
*  The dialog template currently specifies a "monospaced font."  This makes
*   the dialog look somewhat odd, but makes text formatting much easier.  If
*   the specified font does not exist on the system running this sample, the
*   program will work fine, but the contents of the listbox will not look
*   very good.
*
\**************************************************************************/

#include <windows.h>
#include "getsys.h"


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

    ret = DialogBox (hInstance, "getsysDlg", NULL, (DLGPROC)MainDlgProc);
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
  UNREFERENCED_PARAMETER(lParam);


  switch (message) {

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
    * When the different buttons are hit, clear the list box, disable
    *  updating to it, call the function which will fill it, reenable
    *  updating, and then force a repaint.
    *
    \********************************************************************/
    case WM_COMMAND:

      /* if the list box sends back messages, return.  Otherwise we will
       *  clear it out, and that is not what we want to do at this point.
       */
      if (LOWORD(wParam)==DID_LISTBOX) return TRUE;

      SendDlgItemMessage (hwnd, DID_LISTBOX, WM_SETREDRAW,    FALSE, 0);
      SendDlgItemMessage (hwnd, DID_LISTBOX, LB_RESETCONTENT, 0,     0);

      /* switch on the control ID of the button that is pressed. */
      switch (LOWORD(wParam)) {
        case DID_SYSCOLORS : doSysColors (hwnd); break;
        case DID_DIRECTORY : doDirectory (hwnd); break;
        case DID_INFO      : doInfo      (hwnd); break;
        case DID_METRICS   : doMetrics   (hwnd); break;
        case DID_PALETTE   : doPalette   (hwnd); break;
        case DID_LOCALTIME : doLocalTime (hwnd); break;
        case DID_TIME      : doTime      (hwnd); break;
        case DID_VERSIONEX : doVersionEx (hwnd); break;
      } /* end switch (LOWORD()) */

      SendDlgItemMessage (hwnd, DID_LISTBOX, WM_SETREDRAW, TRUE, 0);
      InvalidateRect (GetDlgItem (hwnd, DID_LISTBOX), NULL, TRUE);
      return TRUE;
    break; /* end WM_COMMAND */


    default: return FALSE;
  } /* end switch(message) */
}






/**************************************************************************\
*
*  functions:  do...()
*
* In all of the functions that follow, first set the text int the static
*  text field to label the contents of the list box.  Then, query the
*  desired system information, format it into strings, and add those strings
*  to the listbox.
*
*  input parameters:  HWND - window handle for the dialog box.
*  global variables:  buffer - array of char's to hold string w/ info.
*
\**************************************************************************/

VOID doSysColors (HWND hwnd)
{
int i;
DWORD answer;

  SetDlgItemText (hwnd, DID_TEXT, "System Colors");

  for (i = 0; i<NSYSCOLORS; i++) {

    /*******************************************************/
    /*******************************************************/
    answer = GetSysColor (SysColors[i].Value);
    /*******************************************************/
    /*******************************************************/

    wsprintf (buffer, SysColors[i].String, (int)answer);
    SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  }
  return;
}



VOID doDirectory (HWND hwnd)
{
char buffer[MAX_PATH];

  SetDlgItemText (hwnd, DID_TEXT, "System Directory");

  /*******************************************************/
  /*******************************************************/
  GetSystemDirectory (buffer, MAX_PATH);
  /*******************************************************/
  /*******************************************************/

  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);

  return;
}


VOID doInfo(HWND hwnd)
{
SYSTEM_INFO  si;

  SetDlgItemText (hwnd, DID_TEXT, "SYSTEM_INFO");

  /*******************************************************/
  /*******************************************************/
  GetSystemInfo (&si);
  /*******************************************************/
  /*******************************************************/

  wsprintf (buffer, "dwOemId             \t%d",  (int) si.dwOemId                    );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwPageSize          \t%d",  (int) si.dwPageSize                 );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "lpMinimumApplicationAddress \t%08lx", (LONG)si.lpMinimumApplicationAddress );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "lpMaximumApplicationAddress \t%08lx", (LONG)si.lpMaximumApplicationAddress );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwActiveProcessorMask \t%d",  (int) si.dwActiveProcessorMask      );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwNumberOfProcessors  \t%d",  (int) si.dwNumberOfProcessors       );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwProcessorType       \t%d",  (int) si.dwProcessorType            );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);

#if     defined(STILL_IN_SYSINFO)
  wsprintf (buffer, "dwAllocationGranularity \t%d",  (int) si.dwAllocationGranularity                );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwReserved           \t%d",  (int) si.dwReserved                );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
#endif

  return;
}



VOID doMetrics   (HWND hwnd)
{
int  i;
int  answer;

  SetDlgItemText (hwnd, DID_TEXT, "System Metrics");

  for (i = 0; i<NSYSMETRICS; i++) {

    /*******************************************************/
    /*******************************************************/
    answer = GetSystemMetrics (SystemMetrics[i].Value);
    /*******************************************************/
    /*******************************************************/

    wsprintf (buffer, SystemMetrics[i].String, (int)answer);
    SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  }

  return;
}



VOID doPalette(HWND hwnd)
{
int nEntries;
HDC hdc;
int i;
LPPALETTEENTRY  lpp;

  SetDlgItemText (hwnd, DID_TEXT, "System Palette");

  /* this function is slightly different because the amount of information
   * is not known until run time.  First find the number of entries in the
   * palette (16 for VGA, but 256 for 8514, ...), then allocate enough
   * space to hold all of them, query the information, put it in the list
   * box, and then free up the memory allocated.
   */
  hdc = GetDC (hwnd);
  nEntries = GetSystemPaletteEntries (hdc, 0,0, NULL);
  lpp = (LPPALETTEENTRY)LocalAlloc (LPTR,
                           (DWORD) (nEntries* sizeof (PALETTEENTRY)));

  if (lpp == NULL) {
    MessageBox (hwnd, "Memory allocation failed.", "Warning", MB_ICONSTOP | MB_OK);
    return;
  }

  /*******************************************************/
  /*******************************************************/
  nEntries = GetSystemPaletteEntries (hdc, 0,nEntries, lpp);
  /*******************************************************/
  /*******************************************************/

  ReleaseDC (hwnd, hdc);

  for (i = 0; i<nEntries; i++) {
    wsprintf (buffer, "%d) \t%x \t%x \t%x \t%x", i,
              lpp[i].peRed, lpp[i].peGreen, lpp[i].peBlue, lpp[i].peFlags);
    SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  }

  LocalFree (LocalHandle ((LPSTR)lpp));
  return;
}




VOID doLocalTime (HWND hwnd)
{
SYSTEMTIME  st;

  SetDlgItemText (hwnd, DID_TEXT, "SYSTEMTIME");

  /*******************************************************/
  /*******************************************************/
  GetLocalTime (&st);
  /*******************************************************/
  /*******************************************************/

  wsprintf (buffer, "wYear         \t%d",  (int)st.wYear         );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMonth        \t%d",  (int)st.wMonth        );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wDayOfWeek    \t%d",  (int)st.wDayOfWeek    );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wDay          \t%d",  (int)st.wDay          );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wHour         \t%d",  (int)st.wHour         );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMinute       \t%d",  (int)st.wMinute       );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wSecond       \t%d",  (int)st.wSecond       );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMilliseconds \t%d",  (int)st.wMilliseconds );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  return;
}



VOID doTime (HWND hwnd)
{
SYSTEMTIME  st;

  SetDlgItemText (hwnd, DID_TEXT, "SYSTEMTIME");

  /*******************************************************/
  /*******************************************************/
  GetSystemTime (&st);
  /*******************************************************/
  /*******************************************************/

  wsprintf (buffer, "wYear         \t%d",  (int)st.wYear         );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMonth        \t%d",  (int)st.wMonth        );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wDayOfWeek    \t%d",  (int)st.wDayOfWeek    );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wDay          \t%d",  (int)st.wDay          );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wHour         \t%d",  (int)st.wHour         );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMinute       \t%d",  (int)st.wMinute       );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wSecond       \t%d",  (int)st.wSecond       );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "wMilliseconds \t%d",  (int)st.wMilliseconds );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  return;
}





VOID doVersionEx (HWND hwnd)
{
OSVERSIONINFO osvi;

  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  SetDlgItemText (hwnd, DID_TEXT, "OSVERSIONINFO");

  /*******************************************************/
  /*******************************************************/
  GetVersionEx (&osvi);
  /*******************************************************/
  /*******************************************************/


  wsprintf (buffer, "dwOSVersionInfoSize \t%d",  (int)osvi.dwOSVersionInfoSize );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwMajorVersion \t%d",  (int)osvi.dwMajorVersion );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwMinorVersion \t%d",  (int)osvi.dwMinorVersion );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwBuildNumber \t%d",  (int)osvi.dwBuildNumber );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "dwPlatformId \t%d",  (int)osvi.dwPlatformId );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
  wsprintf (buffer, "szCSDVersion: \t%s",  (LPTSTR) osvi.szCSDVersion );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);


#ifdef GETVERSION
  wsprintf (buffer, "GetVersion(): 0x%08x",  GetVersion () );
  SendDlgItemMessage (hwnd, DID_LISTBOX, LB_ADDSTRING, 0, (LONG) buffer);
#endif

  return;
}
