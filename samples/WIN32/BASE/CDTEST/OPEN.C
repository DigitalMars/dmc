/************************************************************************

  File: open.c

  Purpose:

    This file contains the routines to control the CDTEST.EXE "Open" dialog
    box.  The "Open" dialog box allows the user to enter values into an
    OPENFILENAME structure and then create GetOpenFileName() dialog boxes
    on the fly.

  Functions:

    DoOpenDialog()                   -- starts off the main dialog for "open"

    OpenFunc()                       -- Callback function for main dialog

    InitOpenStruct()                 -- Fills initial OPENFILENAME structure.

    FillOpenDlg()                    -- Fills the dialog with the values from
                                        the OPENFILENAME structure.

    GetOpenDlg()                     -- Retrieves the users entries from the
                                        main dialog and puts them in the
                                        OPENFILENAME structure.

    InitFilterString()               -- Creates filter string

    InitCustFiltString()             -- Creates custom filter string

    InterpretCustomFilterString()    -- Parses custom filter string returned
                                        from GetOpenFileName()

    GetCorrectResourceHandle()       -- Loads custom templates from file
                                        as resource handles

    OpenSaveHookProc()               -- The hook function that will be
                                        called if GetOpen/Save is called
                                        with the OFN_ENABLEHOOK flag set.

    MultiThreadOpenSave()            -- Creates two Open/Save dialogs that
                                        the user can simultaneously access

    OpenSaveThread1Proc()            -- The starting address of thread 1

    OpenSaveThread2Proc()            -- The starting address of thread 2

    DoOpenSaveStuff()                -- Does the actuall calling of
                                        GetOpen/SaveFileName()

    OpenMultiThreadEnableButtons()   -- Enables and disables buttons in
                                        main dialog.  Needed when multi-
                                        threading.

************************************************************************/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <winnls.h>
#include "cdtest.h"
#include "open.h"
#include "save.h"
#include "dlgs.h"     //include file that contains all #defines for the
                      //commdlg dialog templates.


/* All functions defined in this file + 1 external function
   and one external variable */

extern UINT uMode ;                               //see cdtest.c
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;
void InterpretCustomFilterString(void) ;
HANDLE GetCorrectResourceHandle(void) ;
void DoOpenSaveStuff(LPOPENFILENAME) ;
UINT APIENTRY OpenSaveHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam) ;
DWORD OpenSaveThread1Proc(LPDWORD) ;
DWORD OpenSaveThread2Proc(LPDWORD) ;
void MultiThreadOpenSave(void) ;
void OpenMultiThreadEnableButtons(BOOL, HWND) ;


/* All global variables defined in this file */

HWND hwndMainDialog ;   //global handle for open dialog.

HANDLE hRes ;           //handles to the resource and dialog for
HANDLE hDialog ;        //ofn_enabletemplatehandle

HBRUSH hBrushDlg ;
HBRUSH hBrushEdit ;     //brush handles for new colors done with hook proc
HBRUSH hBrushButton ;

HANDLE hOpenSaveThread1, hOpenSaveThread2 ;   //variables for the
DWORD dwThreadID1, dwThreadID2 ;              //multithreading part
DWORD dwThreadParm1, dwThreadParm2 ;
OPENFILENAME ofnThread1, ofnThread2 ;
int nOpenDialogCount ;





/************************************************************************

  Function: DoOpenDialog(HWND)

  Purpose: To create the GetOpenFileName() and GetSaveFileName()
           creation dialog.

  Returns: Nothing.

  Comments:

    GetOpenFileName() and GetSaveFileName() are similiar enough so that
    the same dialog can be used to edit their creation structure elements,
    so a global variable "bDoOpenDlg" keeps track of which one to create
    when the user clicks the OK or Multithread buttons...

************************************************************************/


void DoOpenDialog(HWND hwnd)
{

  bDoOpenDlg = TRUE ;


  DialogBox(hInst, MAKEINTRESOURCE(ID_OPENDIALOG), hwnd, OpenFunc) ;

}






/************************************************************************


  Function: OpenFunc(HWND, UINT, UINT, LONG)

  Purpose:

    This is the callback function for the dialog box containing the
    GetOpenFileName() and the GetSaveFileName() creation options.

    This function will handle the messages for this dialog and create
    either a GetOpenFileName() dialog or a GetSaveFileName() dialog
    depending on the state of the bDoOpenDlg variable.

  Returns: TRUE or FALSE depending on the situation.

  Comments:


************************************************************************/


BOOL APIENTRY OpenFunc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {

    case WM_INITDIALOG:

      if (bDoOpenDlg)
        SetWindowText(hwnd, TEXT("GetOpenFileName()")) ;
      else
        SetWindowText(hwnd, TEXT("GetSaveFileName()")) ;


      /* initialize the OPENFILENAME structure members */

      InitOpenStruct(hwnd, &ofn) ;


      /* Fill these values into the creation dialog */

      FillOpenDlg(hwnd, &ofn) ;


      /* There are three separate OPENFILENAME structures.  One for
         the main Open/Save dialog and one for each multithreaded dialog.
         Set them equal to begin with */

      *(&ofnThread1) = *(&ofnThread2) = *(&ofn) ;

      hwndMainDialog = hwnd ;

      SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEO)) ;

      break ;



    case UMSG_DECREMENTDLGCOUNT:  //user defined message indicating
                                  //the closure of a multithreaded dialog

      /* When we are multithreading, there is nothing to prevent the
         user from interacting with the creation dialog once the first
         GetOpen(Save) file name dialog has returned.  So, in order
         to prevent the Multithread" button from being pressed again
         before the previous two multithreaded dialogs have been canceled,
         disable the controls until we get a message from each thread
         that the dialog has ended */


      nOpenDialogCount-- ;

      if (nOpenDialogCount == 0)
        OpenMultiThreadEnableButtons(TRUE, hwnd) ;

      break ;


    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {

        case IDOK:
          GetOpenDlg(hwnd, &ofn) ;          //get the user's input
          DoOpenSaveStuff(&ofn) ;           //do the dialog
          break ;


        case IDCANCEL:
          EndDialog(hwnd, FALSE) ;
          break ;


        case ID_RESETOPEN:

          SendDlgItemMessage(hwnd, ID_FILTERO, CB_RESETCONTENT,
            (WPARAM) 0, (LPARAM) 0) ;

          InitOpenStruct(hwnd, &ofn) ;

          FillOpenDlg(hwnd, &ofn) ;

          SendDlgItemMessage(hwnd, ID_NULLSTRUCTO, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
          SendDlgItemMessage(hwnd, ID_USEHINSTO, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;

          *(&ofnThread1) = *(&ofnThread2) = *(&ofn) ;

          SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEO)) ;

          break ;


        case ID_ADD1O:

          GetDlgItemText(hwnd, ID_FILTERO, szTemp, 100) ;

          if (*szTemp)
          {
            SendDlgItemMessage(hwnd, ID_FILTERO, CB_ADDSTRING, (WPARAM) 0,
                              (LPARAM) (LPTSTR) szTemp) ;
            SetWindowText(GetDlgItem(hwnd, ID_FILTERO), TEXT("")) ;
          }
          break ;


        case ID_ADD2O:

          GetDlgItemText(hwnd, ID_CUSTFILTO, szTemp, 100) ;

          if (*szTemp)
          {
            SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_ADDSTRING, (WPARAM) 0,
                              (LPARAM) (LPTSTR) szTemp) ;
            SetWindowText(GetDlgItem(hwnd, ID_CUSTFILTO), TEXT("")) ;
          }
          break ;


        case ID_CLEAR1O:
          SendDlgItemMessage(hwnd, ID_FILTERO, CB_RESETCONTENT,
                             (WPARAM) 0, (LPARAM) 0) ;
          break ;

        case ID_CLEAR2O:
          SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_RESETCONTENT,
                             (WPARAM) 0, (LPARAM) 0) ;
          break ;

        case ID_MULTIOPEN:

        /* First, disable the OK, Cancel, and MultiThread buttons */

          OpenMultiThreadEnableButtons(FALSE, hwnd) ;


        /* Then multithread the dialogs */

          nOpenDialogCount = 2 ;

          MultiThreadOpenSave() ;

          break ;


        default:   //end WM_COMMAND case
          break ;
      }
    }

    default:

      /* If the help button is pressed in the GetOpen/SaveFileName()
         dialogs, it will send a message Registered with RegisterWindowMessage()
         to the parent window.  The message nHelpMessage was registered
         at application startup */

      if (msg == nHelpMessage)
        MessageBox(GetForegroundWindow(),
                   TEXT("Hello from the help button"),
                   TEXT("Open Help Button"), MB_OK | MB_APPLMODAL) ;

      break ;

  }

  return FALSE ;
}







/************************************************************************


  Function: InitOpenStruct(HWND, LPOPENFILENAME)

  Purpose:

    Initializes the OPENFILENAME structure.  The structure is referenced
    via a pointer passed in as the second parameter so that we can pass
    any of the three OPENFILENAME structures into this function and
    Initialize them.

  Returns: Nothing.

  Comments:

    The szFilterInits and szCustFiltInits arrays are initialized to
    contain some default strings.  Eventually the strings in
    these arrays must be arranged one after the other with a null
    character between them and two null characters at the end:

    "Text files\0*.txt\0All files\0*.*\0\0"

************************************************************************/


void InitOpenStruct(HWND hwnd, LPOPENFILENAME po)
{
   int i = 0 ;
   szFileName[0] = 0 ;
   szFileTitle[0] = 0 ;

   dwFlags = OFN_READONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_SHOWHELP  ;

   if (bDoOpenDlg)
     lstrcpy(szDlgTitle, TEXT("Open Dialog Title")) ;
   else
     lstrcpy(szDlgTitle, TEXT("Save Dialog Title")) ;

   lstrcpy(szDefExt, TEXT("rat")) ;
   lstrcpy(szInitialDir, TEXT("c:\\")) ;
   lstrcpy(szTempName, TEXT("opentemp1")) ;

   lstrcpy(&szFilterInits[0][0], TEXT("All Files (*.*)")) ;
   lstrcpy(&szFilterInits[1][0], TEXT("*.*")) ;
   lstrcpy(&szFilterInits[2][0], TEXT("Fat Files (*.fat)")) ;
   lstrcpy(&szFilterInits[3][0], TEXT("*.fat")) ;
   szFilterInits[4][0] = (TCHAR) 0 ;

   lstrcpy(&szCustFiltInits[0][0], TEXT("Last Filter Used")) ;
   lstrcpy(&szCustFiltInits[1][0], TEXT("*.lst")) ;
   szCustFiltInits[2][0] = (TCHAR) 0 ;


   /*

      These two functions will create "strings" in the applications
      data area that are in the form

      "Filter Description"\0
      "Filter"\0
      "Filter Description"\0
      "Filter"\0
      ..
      ..
      \0\0

      The filters must be in this form in order that the common dialogs
      interpret it correctly...
   */

   InitFilterString() ;

   InitCustFilterString() ;

   po->lStructSize          = sizeof(OPENFILENAME) ;
   po->hwndOwner            = hwnd ;
   po->hInstance            = hInst ;
   (LPTSTR) po->lpstrFilter = lpszFilterString ;
   po->lpstrCustomFilter    = lpszCustFilterString ;
   po->nMaxCustFilter       = MAXCUSTFILTER ;
   po->nFilterIndex         = 1L ;
   po->lpstrFile            = szFileName ;
   po->nMaxFile             = FILENAMESIZE ;
   po->lpstrFileTitle       = szFileTitle ;
   po->nMaxFileTitle        = FILETITLESIZE ;
   po->lpstrInitialDir      = szInitialDir ;
   (LPTSTR) po->lpstrTitle  = szDlgTitle ;
   po->Flags                = dwFlags ;
   po->nFileOffset          = 0 ;
   po->nFileExtension       = 0 ;
   (LPTSTR) po->lpstrDefExt = szDefExt;
   po->lCustData            = 0L ;
   po->lpfnHook             = OpenSaveHookProc ;
   (LPTSTR) po->lpTemplateName  = szTempName ;

   return ;
}







/************************************************************************


  Function: FillOpenDlg(HWND, LPOPENFILENAME)

  Purpose:

    - This function will fill in the edit boxes that correspond to each
      of the fields in the OPENFILENAME structure.  The user can accept
      these values to create the common dialog or edit them.

  Returns: Nothing.

  Comments:

      The contents of the strings "szShortFilter" and "szLongFilter"
      determine how a WORD or a DWORD value is represented in the edit boxes

************************************************************************/

void FillOpenDlg(HWND hwnd, LPOPENFILENAME po)
{
   int i = 0 ;

   wsprintf(szTemp, szShortFilter, (int) po->lStructSize) ;
   SetDlgItemText(hwnd, ID_STRUCTSIZEO, szTemp) ;

   wsprintf(szTemp, szLongFilter, (LONG) po->hwndOwner) ;
   SetDlgItemText(hwnd, ID_HWNDOWNERO, szTemp) ;

   wsprintf(szTemp, szLongFilter, (LONG) po->hInstance) ;
   SetDlgItemText(hwnd, ID_HINSTANCEO, szTemp) ;


   SendDlgItemMessage(hwnd, ID_FILTERO, CB_RESETCONTENT,
                      (WPARAM) 0, (LPARAM) 0) ;

   while (szFilterInits[i][0] != (TCHAR) 0)
   {
      SendDlgItemMessage(hwnd, ID_FILTERO, CB_ADDSTRING, (WPARAM) 0,
                        (LPARAM) (LPTSTR) &szFilterInits[i][0]) ;
      i++ ;
   }

   SendDlgItemMessage(hwnd, ID_FILTERO, CB_SETCURSEL,
                     (WPARAM) 0, (LPARAM) 0 ) ;



   SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_RESETCONTENT,
                      (WPARAM) 0, (LPARAM) 0) ;

   for (i=0; i<2; i++)
     SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_ADDSTRING, (WPARAM) 0,
                        (LPARAM) (LPTSTR) &szCustFiltInits[i][0]) ;

   SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_SETCURSEL,
                     (WPARAM) 0, (LPARAM) 0) ;


   wsprintf(szTemp, szShortFilter, (int) po->nMaxCustFilter) ;
   SetDlgItemText(hwnd, ID_MAXCUSTFILTO, szTemp) ;

   wsprintf(szTemp, szShortFilter, (int) po->nFilterIndex) ;
   SetDlgItemText(hwnd, ID_FILTINDEXO, szTemp) ;

   SetDlgItemText(hwnd, ID_SZFILEO, po->lpstrFile) ;

   wsprintf(szTemp, szShortFilter, (int) po->nMaxFile) ;
   SetDlgItemText(hwnd, ID_MAXSZFILEO, szTemp) ;

   SetDlgItemText(hwnd, ID_SZFILETITLEO, po->lpstrFileTitle) ;

   wsprintf(szTemp, szShortFilter, (int) po->nMaxFileTitle) ;
   SetDlgItemText(hwnd, ID_MAXSZFILETITLEO, szTemp) ;

   SetDlgItemText(hwnd, ID_SZINITDIRO, po->lpstrInitialDir) ;

   SetDlgItemText(hwnd, ID_SZTITLEO, po->lpstrTitle) ;

   wsprintf(szTemp, szLongFilter, po->Flags) ;
   SetDlgItemText(hwnd, ID_FLAGSO, szTemp) ;

   wsprintf(szTemp, szShortFilter, po->nFileOffset) ;
   SetDlgItemText(hwnd, ID_FILEOFFO, szTemp) ;

   wsprintf(szTemp, szShortFilter, po->nFileExtension) ;
   SetDlgItemText(hwnd, ID_FILEEXTO, szTemp) ;

   SetDlgItemText(hwnd, ID_SZDEFEXTO, po->lpstrDefExt) ;

   wsprintf(szTemp, szLongFilter, po->lCustData) ;
   SetDlgItemText(hwnd, ID_CUSTDATAO, szTemp) ;

   wsprintf(szTemp, szLongFilter, po->lpfnHook) ;
   SetDlgItemText(hwnd, ID_HOOKO, szTemp) ;

   SetDlgItemText(hwnd, ID_TEMPLATEO, po->lpTemplateName) ;

   return ;
}








/************************************************************************


  Function: GetOpenDlg(HWND, LPOPENFILENAME)


  Purpose:

      This function will retrieve the contents of each edit box corresponding
      with each field in the OPENFILENAME structure, and fill in the
      OPENFILENAME structure with these values.

  Returns: Nothing.

  Comments:

    if (uMode == IDM_HEXMODE), then the numbers should be interpreted as
    hexidecimal and the MyAtol() function is called with its "bHex"
    parameter set to true.

************************************************************************/

void GetOpenDlg(HWND hwnd, LPOPENFILENAME po)
{
   int i ;
   BOOL b ;
   TCHAR szNum[20] ;

   GetDlgItemText(hwnd, ID_STRUCTSIZEO, szNum, 20) ;
   po->lStructSize = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_HWNDOWNERO, szNum, 20) ;
   po->hwndOwner = (HWND) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_HINSTANCEO, szNum, 20) ;
   po->hInstance = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;


   /* these are just strings, no conversion necessary */

   i = 0 ;
   while (SendDlgItemMessage(hwnd, ID_FILTERO, CB_GETLBTEXT, (WPARAM) i,
                          (LPARAM) (LPTSTR) szFilterInits[i]) != CB_ERR)
   { i++ ; }


   /* create the filter string */

   InitFilterString() ;


   i = 0 ;

   while (SendDlgItemMessage(hwnd, ID_CUSTFILTO, CB_GETLBTEXT, (WPARAM) i,
                        (LPARAM) (LPTSTR) szCustFiltInits[i]) != CB_ERR)
   { i++ ; }


   InitCustFilterString() ;


   GetDlgItemText(hwnd, ID_MAXCUSTFILTO, szNum, 20) ;
   po->nMaxCustFilter = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_FILTINDEXO, szNum, 20) ;
   po->nFilterIndex = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_SZFILEO, po->lpstrFile, MAXBUF) ;

   GetDlgItemText(hwnd, ID_MAXSZFILEO, szNum, 20) ;
   po->nMaxFile = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_SZFILETITLEO, po->lpstrFileTitle, MAXBUF) ;

   GetDlgItemText(hwnd, ID_MAXSZFILETITLEO, szNum, MAXBUF) ;
   po->nMaxFileTitle = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_SZINITDIRO, (LPTSTR) po->lpstrInitialDir, MAXBUF) ;

   GetDlgItemText(hwnd, ID_SZTITLEO, (LPTSTR) po->lpstrTitle, MAXBUF) ;

   GetDlgItemText(hwnd, ID_FLAGSO, szNum, 20) ;
   po->Flags = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_FILEOFFO, szNum, 20) ;
   po->nFileOffset = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_FILEEXTO, szNum, 20) ;
   po->nFileExtension = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_SZDEFEXTO, (LPTSTR) po->lpstrDefExt, DEFEXTSIZE) ;

   GetDlgItemText(hwnd, ID_CUSTDATAO, szNum, 20) ;
   po->lCustData = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_HOOKO, szNum, 20) ;
   po->lpfnHook = (LPOFNHOOKPROC) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

   GetDlgItemText(hwnd, ID_TEMPLATEO, (LPTSTR) po->lpTemplateName, TEMPNAMESIZE) ;


   /*  if we are supposed to use a preloaded resource handle, load it and put it in
       OPENFILENAME.hInstance... */

   if (IsDlgButtonChecked(hwnd, ID_USEHINSTO) == 1)
     po->hInstance = GetCorrectResourceHandle() ;

   return ;
}







/************************************************************************


  Function: InitFilterString(void)


  Purpose:

    This function will create a "string" in memory in the form that the
    GetOpenFileName() function will expect for the filters it fills into
    the "List Files of Type" combo box.

  Returns: Nothing.

  Comments:

    The szFilterInits and szCustFiltInits arrays are initialized to
    contain some default strings.  Eventually the strings in
    these arrays must be arranged one after the other with a null
    character between them and two null characters at the end:

    "Text files\0*.txt\0All files\0*.*\0\0"

************************************************************************/


void InitFilterString(void)
{
  int i ;
  int nInc = 0 ;
  LPTSTR lpStr = szFilterString ;


  /* First, zero out this memory just for the sake of sanity */

  for (i=0; i<MAXBUF; i++)
    szFilterString[i] = 0 ;


  /* Now, for each string in the szFilterInits array, concatenate it to
     the last one right after the last one's null terminator */

  i = 0 ;

  while (szFilterInits[i][0] != (TCHAR) 0)
  {
    lstrcpy(lpStr, &szFilterInits[i][0]) ;
    nInc+=lstrlen(&szFilterInits[i][0]) + 1 ;   //1 past null term...
    lpStr = &szFilterString[nInc] ;
    i++ ;
  }

  szFilterString[nInc] = (TCHAR) 0 ;  //double terminator


  /* Set the lpszFilterString to point to the memory we just filled in
     with the filters because lpszFilterString is what is in
     OPENFILENAME->lpstrFilter */

  lpszFilterString = szFilterString ;

  return ;
}









/************************************************************************


  Function: InitCustFiltString(void)


  Purpose:

    This function will create a "string" in memory in the form that the
    GetOpenFileName() function will expect for a custom filter.

  Returns: Nothing.

  Comments:

    The szFilterInits and szCustFiltInits arrays are initialized to
    contain some default strings.  Eventually the strings in
    these arrays must be arranged one after the other with a null
    character between them and two null characters at the end:

    "Text files\0*.txt\0All files\0*.*\0\0"

    This program initializes these strings, but they do not need to be
    initialized.  The GetOpenFileName() functiion will write a filter
    into this memory area if the user types a new filter into the
    "FileName" box and returns by clicking the OK button (indicating that
    a file matching that filter was found).

************************************************************************/


void InitCustFilterString(void)
{
  int i ;
  LPTSTR lpStr = szCustFilterString ;
  int nInc = 0 ;

  for (i=0; i<MAXBUF; i++)
    szCustFilterString[i] = 0 ;

  i = 0 ;

  for(i=0; i<2; i++)  //only two for the custom filter
  {
    lstrcpy(lpStr, &szCustFiltInits[i][0]) ;
    nInc+=lstrlen(&szCustFiltInits[i][0]) + 1 ;
    lpStr = &szCustFilterString[nInc] ;
  }

  szCustFilterString[nInc] = (TCHAR) 0 ;

  lpszCustFilterString = szCustFilterString ;

  return ;
}









/************************************************************************

  Function: InterpretCustomFilterString(void)


  Purpose:

    This function will parse the memory that is being used for the
    custom filter string.  If the user returned TRUE after entering
    their own filter, the new filter will be here and so we need to
    add it to our custom filter init array...

  Returns: Nothing.

  Comments:

************************************************************************/

void InterpretCustomFilterString(void)
{
  LPTSTR pNext ;

  int nCount = lstrlen(ofn.lpstrCustomFilter) ;

  pNext = ofn.lpstrCustomFilter + nCount + 1 ;  //one past the NULL


  /* add it to the filter inits array */

  lstrcpy(&szCustFiltInits[0][0], ofn.lpstrCustomFilter) ;
  lstrcpy(&szCustFiltInits[1][0], pNext) ;

}









/************************************************************************

  Function: GetCorrectResourceHandle(void)

  Purpose:

    This function will use FindResource() to find the correct custom
    template resource, use LoadResource() to get a handle to it.

  Returns: A handle to a custom template resource.

  Comments:

    The names for the custom template for GetOpenFileName() are

      "opentemp1" -- normal
      "opentemp2" -- contains a multi-select list box for the file names

    If the user has marked the "Preloaded Template" box and specified
    OFN_ENABLETEMPLATEHANDLE in the "Flags" edit box, this handle
    will be used to create the GetOpenFileName() dialog box.

************************************************************************/


HANDLE GetCorrectResourceHandle(void)
{
  if (ofn.Flags & OFN_ALLOWMULTISELECT)
  {
     hRes = FindResource(hInst, TEXT("opentemp2"), RT_DIALOG) ;
     hDialog = LoadResource(hInst, hRes) ;
  }

  else
  {
     hRes = FindResource(hInst, TEXT("opentemp1"), RT_DIALOG) ;
     hDialog = LoadResource(hInst, hRes) ;
  }

  return hDialog ;
}







/************************************************************************

  Function: OpenSaveHookProc(HWND, UINT, UINT, LONG) ;

  Purpose:

    This function is the hook function for the GetOpenFileName() function.
    If GetOpenFileName() is called with the OFN_ENABLEHOOK flag, this
    function will be called before the normal GetOpenFileName() dialog
    function is called.

  Returns: FALSE to pass the message on to the normal GetOpenFileName()
           logic, TRUE to discard the message.

  Comments:

    To enable this function in this program, enter the value for
    OFN_ENABLEHOOK in the "Flags" edit box.

************************************************************************/


UINT APIENTRY OpenSaveHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  LPOPENFILENAME pOfn ;
  TCHAR szMsg[50] ;

  switch(msg)
  {
    case WM_INITDIALOG:

      pOfn = (LPOPENFILENAME) lParam ;


      /* During initialization, if there is a hook proc, the getopen()
         code will send pointer to the OPENFILENAME strucure in the
         lParam.  To demonstrate this, pop up a message box if this
         structure has a non zero value in the lCustData structure member */


      if (pOfn->lCustData != 0L)
      {
        wsprintf(szMsg, TEXT("OPENFILENAME->lCustData is: %ld"), pOfn->lCustData) ;

        MessageBox(hwnd, szMsg, TEXT("lCustData Sent!"), MB_OK) ;
      }

      SetWindowText(hwnd, TEXT("Open Hook Proc Dialog")) ;

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


    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
          case chx1:
            MessageBox(hwnd, TEXT("Read-Only button clicked..."),
                       TEXT("Open"), MB_OK | MB_APPLMODAL) ;
            break ;

          case ID_FILEPREVIEW:
            MessageBox(hwnd, TEXT("File Preview Button Clicked"),
                       TEXT("Open"), MB_OK | MB_APPLMODAL) ;
            break ;

          default: break ;
        }
        break ;

    default:

      if (msg == nOpenShareVMsg)
      {
        MessageBox(hwnd, TEXT("The SHAREVSTRING message is here!"),
                         TEXT("Open"),
                         MB_ICONEXCLAMATION | MB_OK | MB_APPLMODAL) ;

        return OFN_SHAREWARN ;
      }
      break ;
  }

  return FALSE ;   //send msg to the common dialog code
}









/************************************************************************

  Function: MultiThreadOpenSave(void)

  Purpose:

    This function will start two threads and then return.  The two
    threads will create GetOpenFileName() dialogs, and the user
    can interact with two Open dialogs at once.

  Returns: Nothing.

  Comments:

    This will create two Open dialogs, but they will be created in the
    same location on the screen.  The only way to change that would be
    to call the function with a hookproc and move one of the windows.

    But that would mean that if the user did not enter OFN_ENABLEHOOK
    in the "Flags" edit box, we would be no better off than before.  So,
    in order to see both dialogs, you just have to move the top one out
    of the way a little.

    Multithreading note:

    This function will return before the common dialog functions return.
    Therefore, do not pass any parameters to this function that will be
    referenced by the common dialogs because as soon as this function
    ends those parameters will be gone.

************************************************************************/

void MultiThreadOpenSave(void)
{

  if (!(hOpenSaveThread1 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                        (LPTHREAD_START_ROUTINE) OpenSaveThread1Proc,
                                        &dwThreadParm1, CREATE_SUSPENDED, &dwThreadID1)))

  {
    MessageBox(GetForegroundWindow(), TEXT("Cannot create thread 1"), NULL, MB_OK | MB_ICONEXCLAMATION) ;
    OpenMultiThreadEnableButtons(TRUE, hwndMainDialog) ;
    nOpenDialogCount = 0 ;
    return ;
  }



  if (!(hOpenSaveThread2 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                        (LPTHREAD_START_ROUTINE) OpenSaveThread2Proc,
                                        &dwThreadParm2, CREATE_SUSPENDED, &dwThreadID2)))
  {
    MessageBox(GetForegroundWindow(), TEXT("Cannot create thread 2"), NULL, MB_OK | MB_ICONEXCLAMATION) ;
    OpenMultiThreadEnableButtons(TRUE, hwndMainDialog) ;
    nOpenDialogCount = 0 ;
    return ;
  }


  ResumeThread(hOpenSaveThread1) ;
  ResumeThread(hOpenSaveThread2) ;

}







/************************************************************************

  Function: OpenSaveThreadProc1(LPDWORD)

  Purpose:

    This is the address where the first thread starts executing.

  Returns: Any DWORD value.

  Comments:

************************************************************************/


DWORD OpenSaveThread1Proc(LPDWORD pDw)
{

  /* Get the user's input */

  GetOpenDlg(hwndMainDialog, &ofnThread1) ;


  /* Do the dialog */

  DoOpenSaveStuff(&ofnThread1) ;


  /* send a message to the parent telling it to decrement the dialog count.
     When the dialog count reaches zero, all the buttons are enabled again. */

  SendMessage(hwndMainDialog, UMSG_DECREMENTDLGCOUNT, 0, 0L) ;

  return 0L ;
}







/************************************************************************

  Function: OpenSaveThreadProc2(LPDWORD)

  Purpose:

    This is the address where the second thread starts executing.

  Returns: Any DWORD value.

  Comments:

************************************************************************/

DWORD OpenSaveThread2Proc(LPDWORD pDw)
{
  GetOpenDlg(hwndMainDialog, &ofnThread2) ;

  DoOpenSaveStuff(&ofnThread2) ;

  SendMessage(hwndMainDialog, UMSG_DECREMENTDLGCOUNT, 0, 0L) ;

  return 0L ;
}







/************************************************************************

  Function: DoOpenSaveStuff(LPOPENFILENAME)

  Purpose:

    This is the function that actually calls GetOpenFileName() or
    GetSaveFileName().  It receives a pointer to the structure to
    use as its only parameter.

  Returns: Nothing.

  Comments:

************************************************************************/

void DoOpenSaveStuff(LPOPENFILENAME po)
{
  BOOL bRet = TRUE ;


  /* If bOpenDlg is TRUE, do GetOpenFileName(), else do GetSave().
     If the "NULL Structure" button is checked, send in a NULL pointer
     to the function */

  if (bDoOpenDlg)
  {
    if (IsDlgButtonChecked(hwndMainDialog, ID_NULLSTRUCTO) == 1)
      bRet = GetOpenFileName((LPOPENFILENAME) NULL) ;
    else
      bRet = GetOpenFileName(po) ;
  }

  else
  {
    if (IsDlgButtonChecked(hwndMainDialog, ID_NULLSTRUCTO))
      bRet = GetSaveFileName((LPOPENFILENAME) NULL) ;
    else
      bRet = GetSaveFileName(po) ;
  }


  /* Record the results */

  wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
  SetDlgItemText(hwndMainDialog, ID_ERRORO, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) bRet) ;
  SetDlgItemText(hwndMainDialog, ID_RETURNO, szTemp) ;

  InterpretCustomFilterString() ;

  FillOpenDlg(hwndMainDialog, po) ;

  if (hDialog)
  {
    FreeResource(hDialog) ;    //free ofn.hInstance
    hDialog = (HANDLE) 0 ;
    hRes = (HANDLE) 0 ;
  }
}







/************************************************************************

  Function: OpenMultiThreadEnableButtons(BOOL, HWND)

  Purpose:

    Enable or disable the buttons that might affect the state of the
    OPENFILENAME structures.  This is necessary because during a
    multithreading session, these buttons may be accessible even though
    there are Open dialogs still working and we don't want to change
    the OPENFILENAME structure out from under them.


  Returns: Nothing.

  Comments:

************************************************************************/

void OpenMultiThreadEnableButtons(BOOL bEnable, HWND hwnd)
{
  EnableWindow(GetDlgItem(hwnd, IDOK), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, IDCANCEL), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_RESETOPEN), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_MULTIOPEN), bEnable) ;
}
