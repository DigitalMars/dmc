
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*************************************************************************\
*  PROGRAM: setinfo.c
*
*  PURPOSE:
*
*    To demonstrate queries and setting file information much as the
*    OS/2 API's DosQFileInfo() and DosSetFileInfo().
*
*  GLOBAL VARIABLES:
*
*    HANDLE hInst;  - Instance handle.
*
*    HWND   hWnd;   - Client window handle.
*
*    HWND   hWndDlg;- Window handle to upper dialog box.
*
*    HWND   hWndFileDlg;
*                   - Window handle to lower dialog box.
*
*    DWORD  StepTime;
*                   - Time in ms., used to control the speed of reporting
*                     return codes in the upper dialog box.
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - To handle the messages to the main window.
*    StatusDlgProc()- To handle messages to the Status Dialog (upper) box.
*    FileDlgProc()  - To handle messages to the File Dialog (lower) box.
*    RepStat()      - To calculate return codes and to send results as
*                     messages to the StatusDlgProc()
*
*  COMMENTS:
*
*    Overview:
*      This sample captures and sets a files date/time, size, and
*      attributes information (note it does not *set* file size
*      information).  It also reports API return code status
*      information.
*
*    To Use:
*      Enter a file name into the appropriate edit field, and click on
*      the Get Info. button.  File information will be retrieved and
*      placed in the various edit fields and check buttons.  To set
*      file information, modify the values in the Time and Date edit
*      fields, and click the Set Info. button.  To set file attributes,
*      set the approprate check boxes, and click on the Set Attr. button.
*      Note this application does not check for rediculous Date and Time
*      information, and does not understand i.e. values before 1980 or after
*      2099.  The API are left to deal with such values as best they
*      can.
*
*      As the buttons are clicked the code sends return code status
*      information to be reported in the upper dialog box.  The user
*      can leave this reporting to be done at default time ( 0ms. Sleep
*      between each API), or can enter a time in the appropriate box
*      and clicking the Set Time button.  This will put a Sleep between
*      the API call so that the user can more easily read the return
*      codes.
*
*    Time Conversion:
*      Note that GetFileTime() and SetFileTime() use 64 bit FILETIME
*      structures.  These structure consist of two DWORD fields: which
*      represent file time in hundreds of nano-seconds.  This file time
*      can be converted into DosDate time (or back again) with the
*      FileTimeToDosDateTime() and DosDateTimeToFileTime() calls.  These
*      calls use WORD values representing DosDate and DosTime.  The
*      online Windows .hlp files will tell you how these WORDs break
*      down into days, months, hours, seconds, etc.  This sample uses
*      a combination of masking and shifting to extract the values
*      from the file (see the FileDlgProc() and the symbolic constants
*      mask defined in the header file).  Other points of interest are
*      that seconds are stored in 2 second increments (1-29), and that
*      years are represented from 1980 (meaning i.e. 1992 will be
*      be represented by 12).
*
*
\*************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include "setinfo.h"


HANDLE hInst;
HWND   hWnd, hWndDlg, hWndFileDlg;
DWORD  StepTime;



/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE:  Calls initialization function, processes message loop.
*
*  COMMENTS: A fairly standard WinMain, with the exception that it
*            creates two modeless dialog boxes which fit over the window.
*            Note the use of IsDialogMessage in the message loop.  This
*            call not only returns a boolean value, but also processes
*            the message if it is a dialog box message; otherwise the
*            message falls through to the body of the loop to be translated
*            and dispatched.
*
\*************************************************************************/

int APIENTRY WinMain (HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR  lpCmdLine,
                      int    nCmdShow)


{

  MSG  msg;
  WNDCLASS wc;
  RECT rect;

  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  hInst = hInstance;

  wc.style = 0;                          // Replaces CS_SIZEREDRAW.
  wc.lpfnWndProc = (WNDPROC)MainWndProc; // The client window procedure.
  wc.cbClsExtra = 0;                     // No room reserved for extra data.
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "";
  wc.lpszClassName = "SetInfoWClass";

  RegisterClass(&wc);

  hWnd = CreateWindow ("SetInfoWClass",
                       "Set File Info. Sample",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

  hWndDlg     = CreateDialog (hInst, "StatusDlg", hWnd, (DLGPROC)StatusDlgProc);
  hWndFileDlg = CreateDialog (hInst, "FileDlg",   hWnd, (DLGPROC)FileDlgProc);

  GetClientRect (hWnd, &rect);
  SendMessage (hWnd, WM_SIZE, 0, (rect.right - rect.left));
  ShowWindow  (hWndDlg,  SW_SHOW);
  ShowWindow  (hWndFileDlg, SW_SHOW);
  ShowWindow  (hWnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0, 0))
    if (!IsDialogMessage (hWndDlg, &msg)
        && !IsDialogMessage (hWndFileDlg, &msg))
      {
       DispatchMessage (&msg);   // Dispatch message to window.
      }

  return (msg.wParam);           // Returns value from PostQuitMessage.

}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, UINT, LONG)
*
*  PURPOSE:   To process messages.  To launch client and server threads
*             as appropriate.
*
*  VARIABLES USED:
*
*    - hWndDlg:
*             Global window handle for the upper dialog box.
*
*    - hWndFileDlg:
*             Global window handle for the lower dialog box.
*
*  MESSAGES:
*
*    WM_DESTROY      - Standard, destroys the window.
*    WM_SIZE         - Sends messages to the two dialog boxes so that they
*                      maintain their relative size to the client window.
*
*  CALLED BY:
*
*    WinMain();
*
\*************************************************************************/

LONG APIENTRY MainWndProc (HWND hwnd,
                           UINT message,
                           UINT wParam,
                           LONG lParam)
{
  switch (message)
      {

        case WM_SIZE :
          SetWindowPos (hWndDlg, NULL, 0,0, LOWORD(lParam), DIALOGHEIGHT, 0);
          SetWindowPos (hWndFileDlg, NULL, 0,DIALOGHEIGHT, LOWORD(lParam), HIWORD(lParam), 0);
          return (0);

        case WM_DESTROY :
            PostQuitMessage (0);
            return (0);

       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

/*************************************************************************\
*
*  FUNCTION:  StatusDlgProc (HWND, UINT, UINT, LONG)
*
*  PURPOSE:   To process messages for the upper dialog box.
*
*  VARIABLES USED:
*
*    - StepTime:
*             A global DWORD holding the value of the sleep time between
*             API calls.
*
*    - bTranslated:
*             A local BOOL needed for GetDlgItemInt().
*
*  MESSAGES:
*
*    WM_INITDIALOG:  - Sets a "0" in the time edit field IDE_TIME.  This
*                      is to indicate to the user that the default time
*                      set to sleep between reporting the results of
*                      API calls is set to 0.
*
*    WM_REPSTAT:     - A user defined message.  This message uses wParam
*                      as a pointer to a string holding an API i.e.
*                      "CreateFile()".  It uses lParam to hold a string
*                      reporting the value of returned by the API i.e.
*                      "ERROR_ACCESS_DENIED", or a number if the return
*                      value isn't identified in RepStat().  The code
*                      then places these strings in the IDE_API and
*                      IDE_REPSTAT edit fields.
*
*    WM_COMMAND
*
*      IDM_STEP:     - Captures the time value in the IDE_TIME edit
*                      field, and then sets that value into the
*                      global DWORD StepTime.  This value sets
*                      a sleep time inbetween reporting the results of
*                      each API (making it easier for the user to read).
*
*  COMMENTS:
*
*    This procedure controls the upper dialog box of the application.
*    The purpose of this box is to report on the API being used by
*    the lower box, and it's return code.  It also allows you to control
*    the speed that the application steps through these calls.  This is
*    basically a error checking/debugging feature, but it also lends an
*    intuitive look at the API being used.
*
\*************************************************************************/

LONG APIENTRY StatusDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
  BOOL bTranslated;

  UNREFERENCED_PARAMETER (wParam);
  UNREFERENCED_PARAMETER (lParam);

  switch (message)
    {
    case WM_INITDIALOG:
      StepTime = 0;
      SetDlgItemInt (hDlg, IDE_TIME, StepTime, FALSE);
      return (0);

    case WM_REPSTAT:
      SetDlgItemText (hDlg, IDE_API,(LPTSTR) wParam);
      SetDlgItemText (hDlg, IDE_RETSTAT, (LPTSTR)lParam);
      return (0);

    case WM_COMMAND:
      switch (LOWORD(wParam))
       {
       case IDB_STEP:
           StepTime = GetDlgItemInt (hDlg, IDE_TIME, &bTranslated, FALSE);
       }
    }
  return (0);
}

/*************************************************************************\
*
*  FUNCTION:  FileDlgProc (HWND, UINT, UINT, LONG)
*
*  PURPOSE:   To process the messages to the lower dialog box.
*
*  VARIABLES USED:
*
*    - fileName:
*             A local CHAR array used to capture the text from the
*             IDE_FILENAME edit field.
*
*    - hFile: Local file handle.
*
*    - retCode:
*             Local DWORD used to trap API return codes.
*
*    - ftCreation,
*    - ftAccessed,
*    - ftWrittenTo:
*             Local FILETIME structures.
*
*    - wDosDate,
*    - wDosTime:
*             Local WORDs, used to hold the values converted from the
*             FILETIME structures converted by FileTimeToDosDateTime()
*             and DosDateTimeToFileTime().
*
*    - DD[],
*    - MM[],
*    - YY[],
*    - Mn[],
*    - SS[],
*    - HH[]:  Local CHAR arrays used to trap the values in various
*             edit fields representing date and time values.
*
*    - dwFileSize:
*             Local DWORD holding file size.
*
*    - lpsFileSize[]:
*             Local CHAR array used to trap the string from the IDE_SIZE
*             edit field.
*
*    - dwFileAttr:
*             Local DWORD holding the file attributes.
*
*    - hDlg:  Input parameter, handle to the dialog box.
*
*  MESSAGES:
*
*    WM_COMMAND
*
*      IDB_ATTR:
*             It initializes the variable to hold the file attributes,
*             dwFileAttr, and then traps the checked states of the
*             dialog boxes checked buttons.  If a box is found to be
*             checked, then the appropriate file attribute flag is
*             OR'd into dwFileAttr.  When all of the attribute values
*             have been collected, these attributes are set to the file
*             listed in IDE_FILENAME.
*
*      IDB_SET:
*             This collects the values from the various time and date
*             edit fields, converts them into system time, and sets
*             them to the file listed in IDE_FILENAME.  Basically the
*             technique is to 1) get the values from the edit fields;
*             2) shift them to their correct location in the WORD; and
*             3) OR them together into a single WORD.
*
*      IDB_OPENFILE:
*             This gets the date, time, size, and attribute information
*             from the file listed in IDE_FILENAME.  It converts the
*             values appropriately and puts them into the various
*             edit fields and check buttons.  Again, to do time and date
*             conversions, you must AND the values returned from
*             GetFileTime() with certain masks to let the appropriate bits
*             fall through from the WORD (see the mask and shift values
*             in the setinfo.h file), and shift them to the low order
*             bit to get the new values.  This is done in the wsprintf()
*             call's parameter list.
*
*  CALLS TO:
*
*    RepStat();
*
*  COMMENTS:
*    The basic function of this box is to capture and set file information,
*    and to send messages to the upper dialog box via RepStat() to report
*    return code information.  Special characteristics of this function
*    is how to get and set file information and attributes, and how to
*    convert Time and Date information.
*
\*************************************************************************/

LONG APIENTRY FileDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
  CHAR     fileName[100];
  HANDLE   hFile;
  DWORD    retCode = 0;
  FILETIME ftCreation, ftAccessed, ftWrittenTo;
  WORD     wDosDate, wDosTime;
  CHAR     DD[3], MM[3], YY[3], HH[3], Mn[3], SS[3];
  DWORD    dwFileSize;
  CHAR     lpsFileSize[10];
  DWORD    dwFileAttr;

  UNREFERENCED_PARAMETER (wParam);
  UNREFERENCED_PARAMETER (lParam);

  switch (message)
    {

    case WM_COMMAND :
      switch (LOWORD(wParam))
        {
        case IDB_ATTR:

          dwFileAttr = 0;

          if (IsDlgButtonChecked (hDlg, IDC_HIDE))
             dwFileAttr |= FILE_ATTRIBUTE_HIDDEN;

          if (IsDlgButtonChecked (hDlg, IDC_NORMAL))
             dwFileAttr |= FILE_ATTRIBUTE_NORMAL;

          if (IsDlgButtonChecked (hDlg, IDC_ARC))
             dwFileAttr |= FILE_ATTRIBUTE_ARCHIVE;

          if (IsDlgButtonChecked (hDlg, IDC_SYSTEM))
             dwFileAttr |= FILE_ATTRIBUTE_SYSTEM;

          if (IsDlgButtonChecked (hDlg, IDC_READ))
             dwFileAttr |= FILE_ATTRIBUTE_READONLY;

          GetDlgItemText (hDlg, IDE_FILENAME, fileName, 100);

          retCode = SetFileAttributes (fileName, dwFileAttr);
            RepStat ("SetFileAttributes()", retCode);

          return (0);

        case IDB_SET:

          GetDlgItemText (hDlg, IDE_YEAR,  YY, 3);
          GetDlgItemText (hDlg, IDE_DAY,   DD, 3);
          GetDlgItemText (hDlg, IDE_MONTH, MM, 3);
          wDosDate = (WORD)((atoi(YY) - 80)<<YRSHIFT |
                             atoi(MM)<<MONSHIFT |
                             atoi(DD)<<DAYSHIFT);

          GetDlgItemText (hDlg, IDE_HOURS,   HH, 3);
          GetDlgItemText (hDlg, IDE_SECONDS, SS, 3);
          GetDlgItemText (hDlg, IDE_MINUTES, Mn, 3);
          wDosTime = (WORD)((atoi(SS) / 2)<< SECSHIFT |
                             atoi(Mn)<<MINSHIFT |
                             atoi(HH)<<HRSHIFT);

          retCode = DosDateTimeToFileTime (wDosDate, wDosTime, &ftWrittenTo);
            RepStat("DosDateTimeToFileTime()", retCode);

          GetDlgItemText (hDlg, IDE_FILENAME, fileName, 100);
          hFile = CreateFile (fileName,
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
          if (RepStat("CreateFile()", (DWORD)hFile))
            return (0);


          retCode = SetFileTime (hFile, NULL, NULL, &ftWrittenTo);
            RepStat("SetFileTime()", retCode);

          CloseHandle (hFile);
          return (0);


        case IDB_OPENFILE :
          GetDlgItemText (hDlg, IDE_FILENAME, fileName, 100);
          hFile = CreateFile (fileName,
                              GENERIC_READ,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
          if (RepStat("CreateFile()", (DWORD)hFile))
            return (0);

          (BOOL)retCode = GetFileTime (hFile,
                                       &ftCreation,
                                       &ftAccessed,
                                       &ftWrittenTo);
          RepStat( "GetFileTime()", retCode);

          (BOOL)retCode = FileTimeToDosDateTime( &ftWrittenTo,
                                                 &wDosDate,
                                                 &wDosTime);
          RepStat("FileTimeToDosDateTime()", retCode);

          dwFileSize = GetFileSize (hFile, NULL);
          RepStat("GetFileSize()", dwFileSize);


          wsprintf(YY, "%02d", ((wDosDate & YRMASK) >> YRSHIFT ) + 80);
          SetDlgItemText (hDlg, IDE_YEAR, YY);

          wsprintf(MM, "%02d", ((wDosDate & MONMASK) >> MONSHIFT));
          SetDlgItemText (hDlg, IDE_MONTH, MM);

          wsprintf(DD, "%02d", ((wDosDate & DAYMASK) >> DAYSHIFT) );
          SetDlgItemText (hDlg, IDE_DAY, DD);

          wsprintf(SS, "%02d", ((wDosTime & SECMASK) >> SECSHIFT ) * 2);
          SetDlgItemText (hDlg, IDE_SECONDS, SS);

          wsprintf(Mn, "%02d", ((wDosTime & MINMASK) >> MINSHIFT));
          SetDlgItemText (hDlg, IDE_MINUTES, Mn);

          wsprintf(HH, "%02d", (DWORD)(wDosTime & HRMASK) >> HRSHIFT );
          SetDlgItemText (hDlg, IDE_HOURS, HH);

          wsprintf(lpsFileSize, "%d", dwFileSize);
          SetDlgItemText (hDlg, IDE_SIZE, lpsFileSize);

          dwFileAttr = GetFileAttributes (fileName);
            RepStat("GetFileAttributes()", dwFileAttr);

          if (dwFileAttr & FILE_ATTRIBUTE_NORMAL)
            CheckDlgButton (hDlg, IDC_NORMAL, TRUE);
          else
            CheckDlgButton (hDlg, IDC_NORMAL, FALSE);

          if (dwFileAttr & FILE_ATTRIBUTE_READONLY)
            CheckDlgButton (hDlg, IDC_READ, TRUE);
          else
            CheckDlgButton (hDlg, IDC_READ, FALSE);

          if (dwFileAttr & FILE_ATTRIBUTE_HIDDEN)
            CheckDlgButton (hDlg, IDC_HIDE, TRUE);
          else
            CheckDlgButton (hDlg, IDC_HIDE, FALSE);

          if (dwFileAttr & FILE_ATTRIBUTE_SYSTEM)
            CheckDlgButton (hDlg, IDC_SYSTEM, TRUE);
          else
            CheckDlgButton (hDlg, IDC_SYSTEM, FALSE);

          if (dwFileAttr & FILE_ATTRIBUTE_ARCHIVE)
            CheckDlgButton (hDlg, IDC_ARC, TRUE);
          else
            CheckDlgButton (hDlg, IDC_ARC, FALSE);

          CloseHandle (hFile);

          return (0);
        }
    }
  return (0);
}

/*************************************************************************\
*
*  FUNCTION:  RepStat (CHAR *, DWORD)
*
*  PURPOSE:   To report return code information to the upper dialog box.
*
*  VARIABLES USED:
*
*    - TempBufW[],
*    - TempBufL[]:
*             Local CHAR arrays used to set the lParam and wParam of
*             the WM_REPSTAT message.
*
*    - StepTime:
*             Global DWORD representing the amount of sleep time chosen
*             by the user.
*
*  CALLED BY:
*
*    FileDlgProc();
*
*  COMMENTS:
*
*    This function receives a string representing an API, and a DWORD
*    representing it's return code value.  It checks the return code to
*    see if it indicates an error.  If so, GetLastError is called to find
*    the extended error information; if not, then retCode is set to 0.
*    The case statement is used to exchange the retCode value for
*    it's more intuitive string counterpart.  If no counterpart
*    is listed, the function just sends the number value in string
*    form.  Note that retCode 0 is just sent as a string number.
*    Once the function drops from the switch statment, it sends the
*    user define WM_REPSTAT message to the upper dialog box with the
*    API and retCode information.  It will then enter a Sleep of a
*    duration determined by the global StepTime value (set by the
*    user through and edit field.  The function returns a boolean
*    value so that the calling procedure can determine if it wants
*    to continue processing or break.
*
\*************************************************************************/


BOOL RepStat (CHAR *API, DWORD retCode)
{
  CHAR TempBufW[40];
  CHAR TempBufL[40];

  wsprintf(TempBufW, API);

  if ((int)retCode <= 0 )
    retCode = GetLastError();
  else
    retCode = 0;

  if (retCode)
    MessageBeep(0);

  switch (retCode)
    {
    case ERROR_INVALID_FUNCTION:   // 1L
      wsprintf(TempBufL, "ERROR_INVALID_FUNCTION");
      break;

    case ERROR_FILE_NOT_FOUND:     // 2L
      wsprintf(TempBufL, "ERROR_FILE_NOT_FOUND");
      break;

    case ERROR_ACCESS_DENIED:      // 5L
      wsprintf(TempBufL, "ERROR_ACCESS_DENIED");
      break;

    case ERROR_INVALID_HANDLE:     // 6L
      wsprintf(TempBufL, "ERROR_INVALID_HANDLE");
      break;

    case ERROR_INVALID_PARAMETER:  // 87L
      wsprintf(TempBufL, "ERROR_INVALID_HANDLE");
      break;

    case ERROR_INVALID_NAME:       // 123L
      wsprintf(TempBufL, "ERROR_INVALID_NAME");
      break;

    default:
      wsprintf(TempBufL, "%d", retCode);
    }

  SendMessage (hWndDlg, WM_REPSTAT, (DWORD)TempBufW, (LONG)TempBufL);
  Sleep( StepTime );

  return (retCode != 0);
}
