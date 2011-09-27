/************************************************************************

  File: find.c

  Purpose:

     Manages CDTEST's find/replace dialog box.

  Functions:

    - lpfnFilterProc()      -- A callback function for a filter that must be
                               installed if a modeless dialog is created with
                               another dialog as its parent.

    - DoFindDialog()        -- Creates CDTEST's Open/Save dialog.

    - FindProc()            -- Callback function for CDTEST's Find/Replace dlg.

    - InitFindStruct()      -- Fills a FINDREPLACE structure with some defaults.

    - FillFindDlg()         -- Fills CDTESTs Find/Replace dialog with the contents
                               of a FINDREPLACE structure.

    - GetFindDlg()          -- Retrieves the users edit's from CDTEST's find/
                               replace dialog and puts them in a FINDREPLACE
                               structure.

    - FindReplaceHookProc() -- Callback function for FindText() or ReplaceText()
                               which will be called if either of these dialogs
                               is created with the FR_ENABLEHOOK flag.

    - GetFindDlgHandle()    -- Returns a handle to a preloaded FindText() template.

    - GetReplaceDlgHandle() -- Returns a handle to a preloaded ReplaceText() template.

    - DoFindRepStuff()      -- Calls FindText() or ReplaceText().


  NOTE: CDTEST does not multithread the FindText() or the ReplaceText()
        common dialogs.  The reason for this is that since these dialogs
        are modeless, their creation functions return immediately after the
        dialogs are created as opposed to other dialog functions that
        don't return until after the dialog has been destroyed by the user.

        As a result, any threads that create modeless dialogs will end
        immediately unless the threads themselves have separate message
        loops.  For the sake of clarity, this functionality has not been
        added to CDTEST.

************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <winnls.h>
#include "cdtest.h"
#include "find.h"
#include "replace.h"


/* All functions used in this module + some exported ones */

void InitFindStruct(HWND, LPFINDREPLACE) ;
void FillFindDlg(HWND, LPFINDREPLACE) ;
void GetFindDlg(HWND, LPFINDREPLACE) ;
extern UINT uMode ;
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;
UINT APIENTRY FindReplaceHookProc(HWND, UINT, UINT, LONG) ;
void DoFindRepStuff(LPFINDREPLACE) ;



/* All global variables used in this module */

HWND hwndFind ;
HWND hwndMainDialog ;

FINDREPLACE fr ;
LPFINDREPLACE lpFr ;

TCHAR szFindWhat[100] ;
TCHAR szReplaceWith[100] ;
TCHAR szTemplate[40] ;

HANDLE hResFind, hDialogFind ;
HANDLE GetFindDlgHandle(void) ;
HANDLE GetReplaceDlgHandle(void) ;

HBRUSH hBrushDlg ;
HBRUSH hBrushEdit ;    //brush handles for new colors done with hook proc
HBRUSH hBrushButton ;





/************************************************************************

  Function: lpfnFilterProc(int, WPARAM, LAPRAM)

  Purpose: This is needed if a modeless dialog is created with its parent
           as another dialog box.


  Returns: TRUE if the message was handled and FALSE if not.

  Comments:

    The reason for this is that the DialogBox() procedure does not call
    the IsDialogMessage() function before it processes messages, so we
    need to install a hook function to do it for us.

************************************************************************/


LRESULT CALLBACK lpfnFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  static bFirstTime = TRUE ;

  if (nCode < 0)
    return CallNextHookEx(hHook, nCode, wParam, lParam) ;

  if (nCode == MSGF_DIALOGBOX && bFirstTime)
  {
    bFirstTime = FALSE ;

    if (hwndFind && IsDialogMessage(hwndFind, (LPMSG) lParam))
    {
      bFirstTime = TRUE ;
      return 1L ;
    }

    else
    {
      bFirstTime = TRUE ;
      return 0L ;
    }
  }
  else return 0L ;
}







/************************************************************************

  Function: DoFindDialog(HWND)

  Purpose: This function installs the Hook function, creates the Find/
           Replace dialog, and un-installs the Hook.

  Returns: Nothing.

  Comments:

************************************************************************/

void DoFindDialog(HWND hwnd)
{
  bDoFindDlg = TRUE ;

  /* this is a little different than the others.  If the dialog is just
     created normally, it will make no IsDlgMessage() checks and the
     find/replace dialogs will have no keyboard input (i.e. tabbing and
     alt+key-ing from control to control.  To fix this, a message hook
     and message filter have to be installed

     It must be set to only look at the input for the current thread, or other
     programs will be interrupted by this hook also.
  */

  hHook = SetWindowsHookEx(WH_MSGFILTER, lpfnFilterProc,
                           hInst, GetCurrentThreadId()) ;

  if (!hHook) return ;

  DialogBox(hInst, MAKEINTRESOURCE(ID_FINDDIALOG), hwnd, FindProc) ;

  UnhookWindowsHookEx(hHook) ;

}







/************************************************************************

  Function: FindProc(HWND, UINT, UINT, LONG)

  Purpose: This is the callback function for the CDTEST's Find/Replace
           Dialog.

  Returns: TRUE or FALSE depending on the situation.

  Comments:

************************************************************************/

BOOL APIENTRY FindProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {
    case WM_INITDIALOG:

        if (bDoFindDlg)
          SetWindowText(hwnd, TEXT("FindText()")) ;
        else
          SetWindowText(hwnd, TEXT("ReplaceText()")) ;

        InitFindStruct(hwnd, &fr) ;
        FillFindDlg(hwnd, &fr) ;

        hwndMainDialog = hwnd ;


        /* The find and replace dialogs are a lot harder to multithread because they
           are modeless.  Modeless dialog creation functions return right after the
           dialog is created.  Since ExitThread will be called at this point, it is
           probably not possible to multithread these dialogs without a separate
           GetMessage() loop.
        */


        EnableWindow(GetDlgItem(hwnd, ID_MULTITHREADFINDREP), FALSE) ;

        SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEFT)) ;

        break ;


    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            GetFindDlg(hwnd, &fr) ;
            DoFindRepStuff(&fr) ;
            break ;

          case ID_RESETFIND:
            SendDlgItemMessage(hwnd, ID_FRNULLSTRUCT, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            SendDlgItemMessage(hwnd, ID_PRELOADEDFIND, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            InitFindStruct(hwnd, &fr) ;
            FillFindDlg(hwnd, &fr) ;
            SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEFT)) ;
            break ;

          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;

            break ;


          default: break ;
        }

    }

    default:


    /* nFindMsg is registered at program startup (see CDTEST.c).  The
       FindText() and ReplaceText() dialogs will communicate with the
       calling application via this message. */

    if (msg == nFindMsg)
    {
        lpFr = (LPFINDREPLACE) lParam ;

        if (lpFr->Flags & FR_DIALOGTERM)
        {
          PostMessage(hwnd, UMSG_DECREMENTDLGCOUNT, 0, 0L) ;

          if (hDialogFind)
          {
            FreeResource(hDialogFind) ;
            hDialogFind = (HANDLE) 0 ;
            hResFind = (HANDLE) 0 ;
          }

          hwndFind = (HWND) 0 ;
        }

        FillFindDlg(hwnd, &fr) ;

        wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
        SetDlgItemText(hwnd, ID_ERRORFT, szTemp) ;

        wsprintf(szTemp, szLongFilter, hwndFind) ;
        SetDlgItemText(hwnd, ID_RETURNFT, szTemp) ;
    }


    /* If the help button is pressed in the FindText() or ReplaceText()
       dialogs, it will send a message Registered with RegisterWindowMessage()
       to the parent window.  The message nHelpMessage was registered
       at application startup */

    if (msg == nHelpMessage)
      MessageBox(GetForegroundWindow(), TEXT("Hello from the help button"),
                 TEXT("Find Help Button"), MB_OK | MB_APPLMODAL) ;

    break ;
  }

  return FALSE ;
}






/************************************************************************

  Function: InitFindStruct(HWND, LPFINDREPLACE)

  Purpose: Fills a FINDREPLACE structure with some defaults.


  Returns: Nothing.

  Comments:

************************************************************************/

void InitFindStruct(HWND hwnd, LPFINDREPLACE pfr)
{
  pfr->lStructSize = (DWORD) sizeof(FINDREPLACE) ;
  pfr->hwndOwner = hwnd ;
  pfr->hInstance = (HANDLE) hInst ;
  pfr->Flags = FR_DOWN | FR_SHOWHELP  ;

  lstrcpy(szFindWhat, TEXT("Word to find")) ;
  pfr->lpstrFindWhat = szFindWhat ;
  pfr->wFindWhatLen = 100 ;

  lstrcpy(szReplaceWith, TEXT("Replace with word")) ;
  pfr->lpstrReplaceWith = szReplaceWith ;
  pfr->wReplaceWithLen = 100 ;

  pfr->lCustData = (DWORD) 0 ;

  pfr->lpfnHook = FindReplaceHookProc ;

  lstrcpy(szTemplate, TEXT("fttemp1")) ;

  pfr->lpTemplateName = szTemplate ;
}






/************************************************************************

  Function: FillFindDlg(HWND, LPFINDREPLACE)

  Purpose:  Fills CDTEST's Find/Replace dialog with the contents of a
            FINDREPLACE structure.

  Returns:  Nothing.

  Comments:

************************************************************************/

void FillFindDlg(HWND hwnd, LPFINDREPLACE pfr)
{
  wsprintf(szTemp, szLongFilter, pfr->lStructSize) ;
  SetDlgItemText(hwnd, ID_STRUCTSIZEFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pfr->hwndOwner) ;
  SetDlgItemText(hwnd, ID_HWNDOWNERFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pfr->hInstance) ;
  SetDlgItemText(hwnd, ID_HINSTANCEFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, pfr->Flags) ;
  SetDlgItemText(hwnd, ID_FLAGSFT, szTemp) ;

  SetDlgItemText(hwnd, ID_FINDWHATFT, pfr->lpstrFindWhat) ;

  wsprintf(szTemp, szLongFilter, pfr->wFindWhatLen) ;
  SetDlgItemText(hwnd, ID_FINDWHATLENFT, szTemp) ;

  SetDlgItemText(hwnd, ID_REPLACEWITHFT, pfr->lpstrReplaceWith) ;

  wsprintf(szTemp, szLongFilter, pfr->wReplaceWithLen) ;
  SetDlgItemText(hwnd, ID_REPLACEWITHLENFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, pfr->lCustData) ;
  SetDlgItemText(hwnd, ID_CUSTDATAFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pfr->lpfnHook) ;
  SetDlgItemText(hwnd, ID_HOOKFT, szTemp) ;

  SetDlgItemText(hwnd, ID_TEMPLATEFT, pfr->lpTemplateName) ;
}






/************************************************************************

  Function: GetFindDlg(HWND, LPFINDREPLACE)

  Purpose:  Fills a FINDREPLACE structure with the user's edits in CDTEST's
            Find/Replace dialog.

  Returns:  Nothing.

  Comments:

************************************************************************/

void GetFindDlg(HWND hwnd, LPFINDREPLACE pfr)
{
  TCHAR szNum[30] ;
  BOOL b ;

  #define WSIZEFR 30

  GetDlgItemText(hwnd, ID_STRUCTSIZEFT, szNum, WSIZEFR) ;
  pfr->lStructSize = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HWNDOWNERFT, szNum, WSIZEFR) ;
  pfr->hwndOwner = (HWND) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HINSTANCEFT, szNum, WSIZEFR) ;
  pfr->hInstance = (HANDLE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_FLAGSFT, szNum, WSIZEFR) ;
  pfr->Flags = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;


  GetDlgItemText(hwnd, ID_FINDWHATFT, szFindWhat, 100) ;

  GetDlgItemText(hwnd, ID_REPLACEWITHFT, szReplaceWith, 100) ;


  GetDlgItemText(hwnd, ID_FINDWHATLENFT, szNum, WSIZEFR) ;
  pfr->wFindWhatLen = (WORD) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_REPLACEWITHLENFT, szNum, WSIZEFR) ;
  pfr->wReplaceWithLen = (WORD) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_CUSTDATAFT, szNum, WSIZEFR) ;
  pfr->lCustData = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HOOKFT, szNum, WSIZEFR) ;
  pfr->lpfnHook = (LPFRHOOKPROC) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_TEMPLATEFT, szTemplate, 40) ;
}






/************************************************************************

  Function: FindReplaceHookProc(HWND, UINT, UINT, LONG)

  Purpose:  Is the callback function that will be called by FindText()
            or ReplaceText() if the function is called with the
            FR_ENABLEHOOK flag.


  Returns:  TRUE to discard the message, and FALSE to instruct the common
            dialogs to process the message with the default logic.

  Comments:

     NOTE!

     If the application returns FALSE in response to the WM_INITDIALOG
     message, it is then responsible for displaying the dialog by
     calling ShowWindow() and UpdateWindow().

***********************************************************************/


UINT APIENTRY FindReplaceHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  LPFINDREPLACE pFr ;
  TCHAR szMsg[75] ;
  TCHAR szTmp[20] ;
  int i ;
  TCHAR szDefString[] = TEXT("Default String ") ;

  switch(msg)
  {
    case WM_INITDIALOG:

      pFr = (LPFINDREPLACE) lParam ;

      if (pFr->lCustData != 0L)
      {
        wsprintf(szMsg, TEXT("FINDREPLACE->lCustData is: %ld"), pFr->lCustData) ;

        MessageBox(hwnd, szMsg, TEXT("lCustData Sent!"), MB_OK | MB_APPLMODAL) ;
      }

      SetWindowText(hwnd, TEXT("Find Hook Proc Dialog")) ;

      if (GetDlgItem(hwnd, ID_DEFSTRINGS))
      {
        for (i=0; i<5; i++)
        {
          lstrcpy(szMsg, szDefString) ;
          wsprintf(szTmp, TEXT("Number %d"), i+1) ;
          lstrcat(szMsg, szTmp) ;
          SendDlgItemMessage(hwnd, ID_DEFSTRINGS, LB_ADDSTRING, (WPARAM) 0,
                             (LONG) (LPTSTR) szMsg) ;
        }
      }

      return TRUE ;

      break ;



    /* use the WM_CTLCOLOR* messages to change the color of the Open
       dialog */

    case WM_CTLCOLORDLG:

        if (!hBrushDlg)
            hBrushDlg = GetStockObject(LTGRAY_BRUSH) ;

        return (UINT) hBrushDlg ;

        break ;


    case WM_CTLCOLORBTN:

        SetBkMode((HDC) wParam, TRANSPARENT) ;   //sets background color
                                                 //for push and check box
                                                 //buttons...

        if (!hBrushButton)
            hBrushButton = GetStockObject(LTGRAY_BRUSH) ;

        return (UINT) hBrushButton ;

        break ;


    case WM_CTLCOLORSTATIC:

        SetTextColor((HDC) wParam, RGB(0x00, 0xff, 0x00)) ;  //green
        SetBkMode((HDC) wParam, TRANSPARENT) ;               //transparent text

        if (!hBrushDlg)
            hBrushDlg = GetStockObject(LTGRAY_BRUSH) ;

        return (UINT) hBrushDlg ;

        break ;


    default:
      break ;
  }

  return FALSE ;   //send msg to the common dialog code
}









/************************************************************************

  Function: GetFindDlgHandle(void)

  Purpose:  Finds, loads, and returns a handle to the custom template
            for FindText() in CDTEST.EXE.

  Returns:  HANDLE to the dialog resource.

  Comments:

************************************************************************/

HANDLE GetFindDlgHandle(void)
{
  hResFind = FindResource(hInst, TEXT("fttemp1"), RT_DIALOG) ;

  hDialogFind = LoadResource(hInst, hResFind) ;


  return hDialogFind ;
}





/************************************************************************

  Function: GetReplaceDlgHandle(void)

  Purpose:  Finds, loads, and returns a handle to the custom template
            for ReplaceText() in CDTEST.EXE.

  Returns:  HANDLE to the dialog resource.

  Comments:

************************************************************************/

HANDLE GetReplaceDlgHandle(void)
{
  hResFind = FindResource(hInst, TEXT("fttemp2"), RT_DIALOG) ;

  hDialogFind = LoadResource(hInst, hResFind) ;

  return hDialogFind ;
}






/************************************************************************

  Function: DoFindReplaceStuff(LPFINDREPLACE)

  Purpose:  Calls FindText() or ReplaceText().

  Returns:  Nothing:

  Comments:

************************************************************************/

void DoFindRepStuff(LPFINDREPLACE pfr)
{
  if (IsDlgButtonChecked(hwndMainDialog, ID_PRELOADEDFIND) == 1)
  {
    if (bDoFindDlg)
      pfr->hInstance = GetFindDlgHandle() ;
    else
      pfr->hInstance = GetReplaceDlgHandle() ;
  }

  if (bDoFindDlg)
  {
    if (IsDlgButtonChecked(hwndMainDialog, ID_FRNULLSTRUCT) == 1)
    {
        hwndFind = FindText((LPFINDREPLACE) NULL) ;
    }
    else
    {
        hwndFind = FindText(pfr) ;
    }
  }
  else
  {
    if (IsDlgButtonChecked(hwndMainDialog, ID_FRNULLSTRUCT) == 1)
    {
        hwndFind = ReplaceText((LPFINDREPLACE) NULL) ;
    }
    else
    {
        hwndFind = ReplaceText(pfr) ;
    }

  }


  wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
  SetDlgItemText(hwndMainDialog, ID_ERRORFT, szTemp) ;

  wsprintf(szTemp, szLongFilter, hwndFind) ;
  SetDlgItemText(hwndMainDialog, ID_RETURNFT, szTemp) ;
}
