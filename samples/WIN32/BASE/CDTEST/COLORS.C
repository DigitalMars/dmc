/************************************************************************

  File: colors.c

  Purpose:

    This file contains all the code necessary for the colors dialog.


  Functions:

    DoColorsDialog()          -- Starts off the creation of the main colors dlg

    ColorsProc()              -- Callback function for the colors dialog

    InitColorStruct()         -- Initializes default CHOOSECOLOR strucure

    FillColorDlg()            -- Fills the colors dialog with the current
                                 CHOOSECOLOR structure values.

    GetColorDlg()             -- Retrieves users edits and puts them in
                                 the appropriate CHOOSECOLOR structure.

    ColorsHookProc()          -- Callback hook function if the ChooseColor()
                                 function is called with CC_ENABLEHOOK

    GetColorsResHandle()      -- Loads a custom template from the EXE
                                 and returns a handle to it

    DoChooseColorStuff()      -- Calls the ChooseColor() function.

    ColorThreadProc1()        -- Starting address for the first thread

    ColorThreadProc2()        -- Starting address for the second thread

    MultiThreadColorDlg()     -- Creates two threads in this process
                                 which each create ChooseColor() dialogs.

    EnableColorButtons()      -- Enables or disables buttons in the colors
                                 dialog.  Necessary for the multithreading.


************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <winnls.h>
#include "cdtest.h"
#include "colors.h"


/* Function prototypes */
void InitColorStruct(HWND hwnd, LPCHOOSECOLOR) ;
void FillColorDlg(HWND hwnd, LPCHOOSECOLOR) ;
void GetColorDlg(HWND hwnd, LPCHOOSECOLOR) ;
void DoChooseColorStuff(HWND, LPCHOOSECOLOR) ;
UINT APIENTRY ColorsHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam) ;


/* Variables and external declarations */

CHOOSECOLOR cc ;
DWORD dwCustColors[16] ;
DWORD dwColor ;
HANDLE hDialogColors, hResColors ;
HANDLE GetColorsResHandle(void) ;
TCHAR szTemplateNameC[50] ;
extern UINT uMode ;
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;


/* Multithreading stuff */

DWORD dwColorThreadID1 ;
DWORD dwColorThreadID2 ;
DWORD dwColorThreadParm1 ;
DWORD dwColorThreadParm2 ;
DWORD ColorThreadProc1(LPDWORD) ;
DWORD ColorThreadProc2(LPDWORD) ;
void MultiThreadColorDlg(void) ;
void EnableColorButtons(HWND, BOOL) ;
HANDLE hColorThread1 ;
HANDLE hColorThread2 ;
HWND hwndMainColor ;
int nOpenColorDlgCount ;
CHOOSECOLOR ccThread1 ;
CHOOSECOLOR ccThread2 ;




/************************************************************************

  Function: DoColorsDialog(HWND)

  Purpose: To create the ChooseColor() common dialog creation dialog.

  Returns: Nothing.

  Comments:

************************************************************************/

void DoColorsDialog(HWND hwnd)
{


  DialogBox(hInst, MAKEINTRESOURCE(ID_COLORSDIALOG),
            hwnd, ColorsProc) ;

}







/************************************************************************

  Function: ColorsProc(HWND, UINT, UINT, LONG)

  Purpose: The callback function for the main colors dialog.

  Returns: TRUE or FALSE depending on the situation.

  Comments:

************************************************************************/


BOOL APIENTRY ColorsProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  BOOL bRet = FALSE ;

  switch (msg)
  {

    case WM_INITDIALOG:

       InitColorStruct(hwnd, &cc) ;  //initialize the CHOOSECOLOR structure
       FillColorDlg(hwnd, &cc) ;  //and fill the colors dialog.

       nOpenColorDlgCount = 0 ;

       hwndMainColor = hwnd ;

       *(&ccThread1) = *(&ccThread2) = *(&cc) ;

       SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEC)) ;

       break ;


     case UMSG_DECREMENTDLGCOUNT:  //user defined message indicating
                                   //the closure of a multithreaded dialog

       /* Keep the main colors dialog buttons disabled until the last
          multithreaded dialog function has returned. */

       nOpenColorDlgCount-- ;

       if (nOpenColorDlgCount == 0)
         EnableColorButtons(hwnd, TRUE) ;

       break ;


     case WM_COMMAND:

        switch (LOWORD(wParam))
        {
          case IDOK:

            GetColorDlg(hwnd, &cc) ;            //get user's choices

            DoChooseColorStuff(hwnd, &cc) ;     //do the dialog

            break ;


          case ID_RESETC:

            SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_RESETCONTENT,
              (WPARAM) 0, (LPARAM) 0) ;

            InitColorStruct(hwnd, &cc) ;
            FillColorDlg(hwnd, &cc) ;

            SendDlgItemMessage(hwnd, ID_NULLSTRUCTCOLOR, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            SendDlgItemMessage(hwnd, ID_PRELOADEDCOLORS, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;

            SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEC)) ;

            break ;


          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;
            break ;


          case ID_ADD1C:
            GetDlgItemText(hwnd, ID_CUSTCOLORSC, szTemp, 100) ;

            if (*szTemp)
              SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_ADDSTRING,
                                 (WPARAM) 0, (LPARAM) (LPTSTR) szTemp) ;
            break ;

          case ID_CLEAR1C:
            SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_RESETCONTENT,
                               (WPARAM) 0, (LPARAM) 0) ;
            break ;

          case ID_MULTITHREADCOLORS:

            /* Start the dialog count off at 2, disable the main dialog's
               buttons, and multithread */

            nOpenColorDlgCount = 2 ;

            EnableColorButtons(hwnd, FALSE) ;

            MultiThreadColorDlg() ;

            break ;


          default: break ;
        }

    default:


      /* If the help button is pressed in the ChooseColor()
         dialog, it will send a message Registered with RegisterWindowMessage()
         to the parent window.  The message nHelpMessage was registered
         at application startup */

      if (msg == nHelpMessage)
        MessageBox(GetForegroundWindow(), TEXT("Hello from the help button"),
                   TEXT("Colors Help Button"), MB_OK | MB_APPLMODAL) ;

      break ;
  }

  return FALSE ;
}







/************************************************************************

  Function: InitColorStruct(HWND, LPCHOOSECOLOR)

  Purpose: Fills the default CHOOSECOLOR structure with some default
           values.

  Returns: Nothing.

  Comments:

************************************************************************/


void InitColorStruct(HWND hwnd, LPCHOOSECOLOR pcc)
{
  int i ;

  pcc->lStructSize  = sizeof(CHOOSECOLOR) ;
  pcc->hwndOwner    = hwnd ;
  pcc->hInstance    = (HANDLE) hInst ;

  dwColor = RGB(0, 0, 0) ;
  pcc->rgbResult    = dwColor ;

  for (i=0; i<16; i++)
    dwCustColors[i] = (RGB(255-i*10, i, i*10)) ;

  pcc->lpCustColors = (LPDWORD) dwCustColors ;

  pcc->Flags     = CC_FULLOPEN | CC_SHOWHELP | CC_RGBINIT ;

  pcc->lCustData = 0 ;

  pcc->lpfnHook = ColorsHookProc ;

  lstrcpy(szTemplateNameC, TEXT("clrtemp")) ;         //name of custom template

  pcc->lpTemplateName = (LPTSTR) szTemplateNameC ;

  return ;
}








/************************************************************************

  Function: FillColorDlg(HWND, LPCHOOSECOLOR)

  Purpose: Fills the colors dialog with the current contents of the
           CHOOSECOLOR structure passed in as the second parameter.

  Returns: Nothing.

  Comments:

************************************************************************/

void FillColorDlg(HWND hwnd, LPCHOOSECOLOR pcc)
{
  int i ;

  wsprintf(szTemp, szLongFilter, pcc->lStructSize) ;
  SetDlgItemText(hwnd, ID_STRUCTSIZEC, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcc->hwndOwner) ;
  SetDlgItemText(hwnd, ID_HWNDOWNERC, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcc->hInstance) ;
  SetDlgItemText(hwnd, ID_HINSTANCEC, szTemp) ;

  wsprintf(szTemp, szLongFilter, pcc->rgbResult) ;
  SetDlgItemText(hwnd, ID_RGBRESULTC, szTemp) ;

  SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_RESETCONTENT,
                     (WPARAM) 0, (LPARAM) 0) ;

  for (i=0; i<16; i++)
  {
    wsprintf(szTemp, szLongFilter, dwCustColors[i]) ;
    SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_ADDSTRING,
                       (WPARAM) 0, (LPARAM) (LPTSTR) szTemp) ;
  }

  SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_SETCURSEL,
                     (WPARAM) 0 , (LPARAM) 0) ;

  wsprintf(szTemp, szLongFilter, pcc->Flags) ;
  SetDlgItemText(hwnd, ID_FLAGSC, szTemp) ;

  wsprintf(szTemp, szLongFilter, pcc->lCustData) ;
  SetDlgItemText(hwnd, ID_CUSTDATAC, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcc->lpfnHook) ;
  SetDlgItemText(hwnd, ID_HOOKC, szTemp) ;

  SetDlgItemText(hwnd, ID_TEMPLATEC, (LPTSTR) pcc->lpTemplateName) ;
}







/************************************************************************

  Function: GetColorDlg(HWND, LPCHOOSECOLOR)

  Purpose: Retrieves the users entries in the colors dialog edit boxes
           and puts them in the appropriate CHOOSECOLOR structure
           members.

  Returns: Nothing.

  Comments:

************************************************************************/


void GetColorDlg(HWND hwnd, LPCHOOSECOLOR pcc)
{
  int i ;
  BOOL b ;
  TCHAR szNum[30] ;

  #define WSIZECC 30

  GetDlgItemText(hwnd, ID_STRUCTSIZEC, szNum, WSIZECC) ;
  pcc->lStructSize = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HWNDOWNERC, szNum, WSIZECC) ;
  pcc->hwndOwner = (HWND) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HINSTANCEC, szNum, WSIZECC) ;
  pcc->hInstance = (HANDLE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_RGBRESULTC, szNum, WSIZECC) ;
  pcc->rgbResult = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  for (i=0; i<16; i++)
  {
    if (SendDlgItemMessage(hwnd, ID_CUSTCOLORSC, CB_GETLBTEXT,
                          (WPARAM) i, (LPARAM) (LPTSTR) szNum) != CB_ERR)

      dwCustColors[i] = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

    else
      dwCustColors[i] = (DWORD) 0 ;

    *(pcc->lpCustColors + i) = dwCustColors[i] ;
  }

  GetDlgItemText(hwnd, ID_FLAGSC, szNum, WSIZECC) ;
  pcc->Flags = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_CUSTDATAC, szNum, WSIZECC) ;
  pcc->lCustData = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HOOKC, szNum, WSIZECC) ;
  pcc->lpfnHook = (LPCCHOOKPROC) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_TEMPLATEC, (LPTSTR) pcc->lpTemplateName, 50) ;

  return ;
}







/************************************************************************

  Function: ColorsHookProc(HWND, UINT, UINT, LONG) ;

  Purpose:  This function will be called before the normal ChooseColor()
            logic is envoked.

  Returns: FALSE to instruct ChooseColor() to proceed with it's normal
           processing of the message.  TRUE to discard normal processing.

  Comments:

    To enable the hook, type the value for "CC_ENABLEHOOK" in the
    "Flags" edit box.

************************************************************************/

UINT APIENTRY ColorsHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  LPCHOOSECOLOR pCc ;
  TCHAR szMsg[50] ;

  switch(msg)
  {
    case WM_INITDIALOG:

      /* During initialization of the ChooseColor() dialog, if there is a
         hook proc the ChooseColor() function will send a pointer to the
         current CHOOSECOLOR structure in the wParam parameter */

      pCc = (LPCHOOSECOLOR) lParam ;

      if (pCc->lCustData != 0L)
      {
        wsprintf(szMsg, TEXT("CHOOSECOLOR->lCustData is: %ld"), pCc->lCustData) ;
        MessageBox(hwnd, szMsg, TEXT("lCustData Sent!"), MB_OK | MB_APPLMODAL) ;
      }

      SetWindowText(hwnd, TEXT("Color Hook Proc Dialog")) ;

      break ;

    default:
      break ;
  }

  return FALSE ;   //send msg to the common dialog code
}







/************************************************************************

  Function: GetColorsResHandle(void)

  Purpose: Creates a handle to the custom template and returns it to the
           caller.

  Returns: HANDLE to a preloaded custom template

  Comments:

    To use this preloaded template, mark the "Preloaded Template" checkbox
    and enter CC_ENABLETEMPLATEHANDLE in the "Flags" edit box.

************************************************************************/

HANDLE GetColorsResHandle(void)
{

  hResColors = FindResource(hInst, TEXT("clrtemp"), RT_DIALOG) ;

  hDialogColors = LoadResource(hInst, hResColors) ;


  return hDialogColors ;
}







/************************************************************************

  Function:DoChooseColorStuff(HWND, LPCHOOSECOLOR)

  Purpose: Does the actual calling of the ChooseColor() function.


  Returns: Nothing.

  Comments:

************************************************************************/

void DoChooseColorStuff(HWND hwnd, LPCHOOSECOLOR pcc)
{
  BOOL bRet ;

  /* If the "Preloaded template button is checked, load a handle for
     the custom template and put it in the hInstance member of the
     CHOOSECOLOR structure */

  if (IsDlgButtonChecked(hwnd, ID_PRELOADEDCOLORS) == 1)
    pcc->hInstance = GetColorsResHandle() ;


   /* If the "NULL Structure" box is checked, call ChooseColor() will
      a NULL pointer.  Otherwise call with the current CHOOSECOLOR pointer */

  if (IsDlgButtonChecked(hwnd, ID_NULLSTRUCTCOLOR) == 1)
    bRet = ChooseColor((LPCHOOSECOLOR) NULL) ;
  else
    bRet = ChooseColor(pcc) ;

  if (pcc->hInstance)
  {
    FreeResource(hDialogColors) ;
    hDialogColors = (HANDLE) 0 ;
    hResColors = (HANDLE) 0 ;
  }


  /* Fill the results into the main colors dialog */

  FillColorDlg(hwnd, pcc) ;

  wsprintf(szTemp, szShortFilter, bRet) ;
  SetDlgItemText(hwnd, ID_RETURNC, szTemp) ;

  wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
  SetDlgItemText(hwnd, ID_ERRORC, szTemp) ;

}






/************************************************************************

  Function:ColorThreadProc1(LPDWORD)

  Purpose: Starting address for first thread


  Returns: Any DWORD value.

  Comments:

************************************************************************/


DWORD ColorThreadProc1(LPDWORD pdw)
{
  GetColorDlg(hwndMainColor, &ccThread1) ;
  DoChooseColorStuff(hwndMainColor, &ccThread1) ;

  PostMessage(hwndMainColor, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}







/************************************************************************

  Function:ColorThreadProc2(LPDWORD)

  Purpose: Starting address for second thread


  Returns: Any DWORD value.

  Comments:

************************************************************************/

DWORD ColorThreadProc2(LPDWORD pdw)
{
  GetColorDlg(hwndMainColor, &ccThread2) ;
  DoChooseColorStuff(hwndMainColor, &ccThread2) ;

  PostMessage(hwndMainColor, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}







/************************************************************************

  Function:MultiThreadColorDlg(void)

  Purpose: Creates two threads which will each create a new ChooseColor()
           dialog.


  Returns: Nothing.

  Comments:

    Do not pass any parameters to this function that may be used by the
    ChooseColor dialog routines because this function will return before
    the new threads are finished.

************************************************************************/


void MultiThreadColorDlg(void)
{

  dwColorThreadParm1 = dwColorThreadParm2 = 0L ;

  if (!(hColorThread1 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) ColorThreadProc1,
                                     &dwColorThreadParm1, CREATE_SUSPENDED, &dwColorThreadID1)))

  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 1"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenColorDlgCount = 0 ;

    EnableColorButtons(hwndMainColor, TRUE) ;

    return ;
  }


  if (!(hColorThread2 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) ColorThreadProc2,
                                     &dwColorThreadParm2, CREATE_SUSPENDED, &dwColorThreadID2)))
  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 2"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenColorDlgCount = 0 ;

    EnableColorButtons(hwndMainColor, TRUE) ;

    return ;
  }

  ResumeThread(hColorThread1) ;

  ResumeThread(hColorThread2) ;

  return ;
}





/************************************************************************

  Function:EnableColorButtons(HWND, BOOL)

  Purpose: Enables or disables the buttons on the main colors dialog.
           Necessary when multithreading.

  Returns: Nothing.

  Comments:

************************************************************************/


void EnableColorButtons(HWND hwnd, BOOL bEnable)
{
  EnableWindow(GetDlgItem(hwnd, IDOK), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, IDCANCEL), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_RESETC), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_MULTITHREADCOLORS), bEnable) ;
}
