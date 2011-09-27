/************************************************************************

  File: print.c

  Purpose:  To manage CDTEST's print dialog box.


  Functions:

    DoPrintDlg()            -- Creates CDTEST's print dialog box.

    PrintProc()             -- Callback function for CDTEST's print dialog box.

    DevnamesProc()          -- Callback function for CDTESTS Devnames dlg box.

    DevmodeProc()           -- Callback function for CDTESTS Devmode dlg box.

    InitPrintStruct()       -- Fills a PRINTDLG struct with some default values.

    FillPrintDlg()          -- Fills CDTEST's print dialog with contents of a
                               PRINTDLG structure.

    GetPrintDlg()           -- Retrieves the users edits from CDTEST's print dlg.

    FillDevnamesDlg()       -- Fills CDTEST's Devnames dialog with the contents
                               of a DEVNAMES structure.

    GetDevnamesDlg()        -- Retrieves the user's edits from CDTEST's devnames dlg.

    FillDevmodeDlg()        -- Fills CDTEST's devmode dialog with the contents of
                               a DEVMODE structure.

    GetDevmodeDlg()         -- Retrieves the user's edits from CDTEST's devmode dlg.

    GetPrintDlgHandle()     -- Loads and returns a handle to a Print dlg custom template.

    GetSetupDlgHandle()     -- Loads and returns a handle to a Setup dlg custom template.

    PrintHookProc()         -- Callback function for PRINTDLG->lpfnPrintHook and
                               for PRINTDLG->lpfnSetupHook

    PrintMultiProc1()       -- Starting address for multithread option thread 1

    PrintMultiProc2()       -- Starting address for multithread option thread 2

    MultiThreadPrintDlg()   -- Creates two threads which each call PrintDlg()

    EnablePrintControls()   -- Enables or disables CDTEST's print dlg controls.

    DoPrintDlgStuff()       -- Calls PrintDlg()

************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <winnls.h>
#include "cdtest.h"
#include "print.h"
#include "devnames.h"
#include "devmode.h"
#include "dlgs.h"



/* Externs, prototypes, variable declarations for print.c */

extern UINT uMode ;
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;
UINT APIENTRY PrintHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam) ;
void InitPrintStruct(HWND, LPPRINTDLG) ;
void FillPrintDlg(HWND, LPPRINTDLG) ;
void GetPrintDlg(HWND, LPPRINTDLG) ;
void FillDevnamesDlg(HWND, LPPRINTDLG) ;
void GetDevnamesDlg(HWND, LPPRINTDLG) ;
void FillDevmodeDlg(HWND, LPPRINTDLG) ;
void GetDevmodeDlg(HWND, LPPRINTDLG) ;
HANDLE GetPrintDlgHandle(void) ;
HANDLE GetSetupDlgHandle(void) ;
void DoPrintDlgStuff(HWND, LPPRINTDLG) ;

HANDLE hResPrint ;
HANDLE hDialogPrint ;
PRINTDLG pd ;
HANDLE hDevNames ;
HANDLE hDevMode ;

TCHAR szDriverOffset[100] ;
TCHAR szDeviceOffset[100] ;
TCHAR szOutputOffset[100] ;
TCHAR szPrintTempName[40] ;
TCHAR szSetupTempName[40] ;


/* Multithreading function declarations, variables */

DWORD PrintMultiProc1(LPDWORD) ;
DWORD PrintMultiProc2(LPDWORD) ;
void MultiThreadPrintDlg(void) ;
void EnablePrintControls(HWND, BOOL) ;

HANDLE hPrintThread1 ;
HANDLE hPrintThread2 ;
DWORD dwPrintThreadID1 ;
DWORD dwPrintThreadID2 ;
DWORD dwPrintThreadParm1 ;
DWORD dwPrintThreadParm2 ;
PRINTDLG pdThread1 ;
PRINTDLG pdThread2 ;
HWND hwndMainPrint ;
int nOpenDialogCount ;

HBRUSH hBrushDlg ;
HBRUSH hBrushEdit ;    //brush handles for new colors done with hook proc
HBRUSH hBrushButton ;





/************************************************************************

  Function: DoPrintDialog(HWND)

  Purpose:  Creates CDTEST's print dialog box.

  Returns:  Nothing.

  Comments: There is only one dialog for both Print Setup and Print Print.
            The Print Print dialog is the default.  To create the Print
            Setup dialog, enter the value for PD_PRINTSETUP into the
            "Flags" edit box and click OK.

************************************************************************/

void DoPrintDialog(HWND hwnd)
{


  DialogBox(hInst, MAKEINTRESOURCE(ID_PRINTDIALOG),
            hwnd, PrintProc) ;

}







/************************************************************************

  Function: PrintProc(HWND, UINT, UINT, LONG)

  Purpose:  Callback function for CDTEST's print dialog box.

  Returns:  TRUE or FALSE depending on the situation/message.

  Comments:

************************************************************************/

BOOL APIENTRY PrintProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {
    case WM_INITDIALOG:

        InitPrintStruct(hwnd, &pd) ;
        FillPrintDlg(hwnd, &pd) ;

        *(&pdThread1) = *(&pdThread2) = *(&pd) ;

        hwndMainPrint = hwnd ;

        nOpenDialogCount = 0 ;

        SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEP)) ;

        break ;


    case UMSG_DECREMENTDLGCOUNT: //user defined message that is send when
                                 //each thread created when multithreading ends
        nOpenDialogCount-- ;

        if (nOpenDialogCount == 0)               //once both multitheading dlgs
            EnablePrintControls(hwnd, TRUE) ;   //are done, enable the controls again

        break ;


    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            GetPrintDlg(hwnd, &pd) ;
            DoPrintDlgStuff(hwnd, &pd) ;
            break ;

          case IDCANCEL:

            if (pd.hDC)
              ReleaseDC(hwnd, pd.hDC) ;

            EndDialog(hwnd, FALSE) ;
            break ;

          case ID_RESETPRINT:
            SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEP)) ;
            InitPrintStruct(hwnd, &pd) ;
            FillPrintDlg(hwnd, &pd) ;
            SendDlgItemMessage(hwnd, ID_PRESETUPP,   BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            SendDlgItemMessage(hwnd, ID_PREPRINTP,   BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            SendDlgItemMessage(hwnd, ID_NULLSTRUCTP, BM_SETCHECK, (WPARAM)0, (LPARAM)0) ;
            break ;

          case ID_EDITDEVNAMES:
            DialogBox(hInst, MAKEINTRESOURCE(ID_DEVNAMESDIALOG),
                      hwnd, DevnamesProc) ;
            break ;

          case ID_EDITDEVMODE:
            DialogBox(hInst, MAKEINTRESOURCE(ID_DEVMODEDIALOG),
                      hwnd, DevmodeProc) ;
            break ;

          case ID_MULTITHREADPRINT:
            nOpenDialogCount = 2 ;
            EnablePrintControls(hwnd, FALSE) ;
            MultiThreadPrintDlg() ;
            break ;

          default: break ;
        }
    }

    default:

      /* If the help button is pressed in the PrintDlg() dialog box,
         it will send a message Registered with RegisterWindowMessage()
         to the parent window.  The message nHelpMessage was registered
         at application startup */

      if (msg == nHelpMessage)
        MessageBox(GetForegroundWindow(), TEXT("Hello from the help button"),
                   TEXT("Print Help Button"), MB_OK | MB_APPLMODAL) ;
      break ;
  }

  return FALSE ;
}








/************************************************************************

  Function: DevnamesProc(HWND, UINT, UINT, LONG)

  Purpose:  Callback function for the Devnames dialog box

  Returns:  TRUE or FALSE depending on situation / message

  Comments:

************************************************************************/

BOOL APIENTRY DevnamesProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {
    case WM_INITDIALOG:
        FillDevnamesDlg(hwnd, &pd) ;
        break ;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            GetDevnamesDlg(hwnd, &pd) ;
            EndDialog(hwnd, TRUE) ;
            break ;

          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;
            break ;

          default: break ;
        }
    }

    default: break ;
  }

  return FALSE ;
}






/************************************************************************

  Function: DevmodeProc(HWND, UINT, UINT, LONG)

  Purpose:  Callback function for the Devmode dialog box

  Returns:  TRUE or FALSE depending on situation / message

  Comments:

************************************************************************/

BOOL APIENTRY DevmodeProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {
    case WM_INITDIALOG:
        FillDevmodeDlg(hwnd, &pd) ;
        break ;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            GetDevmodeDlg(hwnd, &pd) ;
            EndDialog(hwnd, TRUE) ;
            break ;

          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;
            break ;

          default: break ;
        }
    }

    default: break ;
  }

  return FALSE ;
}








/************************************************************************

  Function: InitPrintStruct(HWND, LPPRINTDLG)

  Purpose:  Fills a PRINTDLG structure with some default values.

  Returns:  Nothing.

  Comments:

    In order to fill in the hDevMode and hDevNames PRINTDLG structure
    elements, a call to PrintDlg() must be made with the
    (PD_RETURNDEFAULT | PD_RETURNDC) flag bits set.

************************************************************************/

void InitPrintStruct(HWND hwnd, LPPRINTDLG ppd)
{
  DWORD dwTempFlags = 0 ;

  ppd->lStructSize = sizeof(PRINTDLG) ;
  ppd->hwndOwner = hwnd ;
  ppd->hDevMode = (HANDLE) 0 ;
  ppd->hDevNames = (HANDLE) 0 ;
  ppd->hDC = (HDC) 0 ;
  ppd->Flags = PD_RETURNDC | PD_COLLATE | PD_SHOWHELP | PD_PAGENUMS ;
  ppd->nFromPage = 1 ;
  ppd->nToPage = 10 ;
  ppd->nMinPage = 0 ;
  ppd->nMaxPage = 9 ;
  ppd->nCopies = 1000 ;
  ppd->hInstance = (HANDLE) hInst ;
  ppd->lCustData = 0L ;

  ppd->lpfnPrintHook = PrintHookProc ;
  ppd->lpfnSetupHook = PrintHookProc ;

  lstrcpy(szPrintTempName, TEXT("prtemp1")) ;
  ppd->lpPrintTemplateName = szPrintTempName ;

  lstrcpy(szSetupTempName, TEXT("prtemp2")) ;
  ppd->lpSetupTemplateName = szSetupTempName ;

  ppd->hPrintTemplate = (HANDLE) 0 ;   //these are unknown right now
  ppd->hSetupTemplate = (HANDLE) 0 ;


  /* Let's fill the hDevMode and hDevNames structures. */
  /* This call should just return handles to structures of this */
  /* type filled with the info for the default printer */

  dwTempFlags = ppd->Flags ;
  ppd->Flags = PD_RETURNDEFAULT | PD_RETURNDC ;
  PrintDlg(ppd) ;

  ppd->Flags = dwTempFlags ;
}







/************************************************************************

  Function: FillPrintDlg(HWND, LPPRINTDLG)

  Purpose:  Fills CDTEST's print dialog with the contents of a
            PRINTDLG structure.

  Returns:  Nothing.

  Comments:

************************************************************************/


void FillPrintDlg(HWND hwnd, LPPRINTDLG ppd)
{
  wsprintf(szTemp, szLongFilter, ppd->lStructSize) ;
  SetDlgItemText(hwnd, ID_STRUCTSIZEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hwndOwner) ;
  SetDlgItemText(hwnd, ID_HWNDOWNERP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hDevMode) ;
  SetDlgItemText(hwnd, ID_HDEVMODEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hDevNames) ;
  SetDlgItemText(hwnd, ID_HDEVNAMESP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hDC) ;
  SetDlgItemText(hwnd, ID_HDCP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->Flags) ;
  SetDlgItemText(hwnd, ID_FLAGSP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->nFromPage) ;
  SetDlgItemText(hwnd, ID_FROMPAGEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->nToPage) ;
  SetDlgItemText(hwnd, ID_TOPAGEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->nMinPage) ;
  SetDlgItemText(hwnd, ID_MINPAGEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->nMaxPage) ;
  SetDlgItemText(hwnd, ID_MAXPAGEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->nCopies) ;
  SetDlgItemText(hwnd, ID_COPIESP, szTemp) ;

  wsprintf(szTemp, szLongFilter, ppd->hInstance) ;
  SetDlgItemText(hwnd, ID_HINSTANCEP, szTemp) ;

  wsprintf(szTemp, szLongFilter, ppd->lCustData) ;
  SetDlgItemText(hwnd, ID_CUSTDATAP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->lpfnPrintHook) ;
  SetDlgItemText(hwnd, ID_PRINTHOOKP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->lpfnSetupHook) ;
  SetDlgItemText(hwnd, ID_SETUPHOOKP, szTemp) ;

  SetDlgItemText(hwnd, ID_LPPRINTTEMPP, ppd->lpPrintTemplateName) ;

  SetDlgItemText(hwnd, ID_LPSETUPTEMPP, ppd->lpSetupTemplateName) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hPrintTemplate) ;
  SetDlgItemText(hwnd, ID_HPRINTTEMPP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hSetupTemplate) ;
  SetDlgItemText(hwnd, ID_HSETUPTEMPP, szTemp) ;
}









/************************************************************************

  Function: GetPrintDlg(HWND, LPPRINTDLG)

  Purpose:  Fills a PRINTDLG structure with the users choices in CDTEST's
            print dialog box.

  Returns:  Nothing.

  Comments:

************************************************************************/

void GetPrintDlg(HWND hwnd, LPPRINTDLG ppd)
{
  BOOL b ;
  TCHAR szNum[20] ;

  #define WMAX 20

  GetDlgItemText(hwnd, ID_STRUCTSIZEP, szNum, WMAX) ;
  ppd->lStructSize = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HWNDOWNERP, szNum, WMAX) ;
  ppd->hwndOwner = (HWND) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HDEVMODEP, szNum, WMAX) ;
  ppd->hDevMode = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HDEVNAMESP, szNum, WMAX) ;
  ppd->hDevNames = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HDCP, szNum, WMAX) ;
  ppd->hDC = (HDC) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_FLAGSP, szNum, WMAX) ;
  ppd->Flags = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_FROMPAGEP, szNum, WMAX) ;
  ppd->nFromPage = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_TOPAGEP, szNum, WMAX) ;
  ppd->nToPage = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_MINPAGEP, szNum, WMAX) ;
  ppd->nMinPage = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_MAXPAGEP, szNum, WMAX) ;
  ppd->nMaxPage = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_COPIESP, szNum, WMAX) ;
  ppd->nCopies = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HINSTANCEP, szNum, WMAX) ;
  ppd->hInstance = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_CUSTDATAP, szNum, WMAX) ;
  ppd->lCustData = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_PRINTHOOKP, szNum, WMAX) ;
  ppd->lpfnPrintHook = (LPPRINTHOOKPROC) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_SETUPHOOKP, szNum, WMAX) ;
  ppd->lpfnSetupHook = (LPSETUPHOOKPROC) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LPPRINTTEMPP, szPrintTempName, 100) ;

  GetDlgItemText(hwnd, ID_LPSETUPTEMPP, szSetupTempName, 100) ;

  GetDlgItemText(hwnd, ID_HPRINTTEMPP, szNum, WMAX) ;
  ppd->hPrintTemplate = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HSETUPTEMPP, szNum, WMAX) ;
  ppd->hSetupTemplate = (HANDLE) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

}








/************************************************************************

  Function: FillDevnamesDlg(HWND, LPPRINTDLG)

  Purpose:  Fills CDTEST's devnames dialog box with the values in
            The hDevNames structure member of a PRINTDLG structure.

  Returns:  Nothing.

  Comments:

************************************************************************/


void FillDevnamesDlg(HWND hwnd, LPPRINTDLG ppd)
{
  LPDEVNAMES pn = (LPDEVNAMES) 0 ;

  if (ppd->hDevNames == (HANDLE) 0)
    return ;

  pn = (LPDEVNAMES) GlobalLock(ppd->hDevNames) ;
  if (pn == (LPDEVNAMES) NULL) return ;


  /* To find the strings in this block of memory, add the correct
     offset to the original pointer returned by GlobalLock() */

  SetDlgItemText(hwnd, ID_WDRIVEROFF, (LPTSTR) pn + pn->wDriverOffset) ;

  SetDlgItemText(hwnd, ID_WDEVICEOFF, (LPTSTR) pn + pn->wDeviceOffset) ;

  SetDlgItemText(hwnd, ID_WOUTPUTOFF, (LPTSTR) pn + pn->wOutputOffset) ;


  wsprintf(szTemp, szShortFilter, (int) pn->wDefault) ;
  SetDlgItemText(hwnd, ID_WDEFAULT, szTemp) ;


  GlobalUnlock(ppd->hDevNames) ;
}







/************************************************************************

  Function: GetDevnamesDlg(HWND, LPPRINTDLG)

  Purpose:  Retrieves the user's choices in CDTEST's devnames dialog box,
            Allocates some memory to hold a new DEVNAMES structure, fills
            the users choices into the new DEVNAMES structure, and sets
            the hDevNames member of a PRINTDLG structure to this new
            memory handle.


  Returns:  Nothing.

  Comments:

************************************************************************/

void GetDevnamesDlg(HWND hwnd, LPPRINTDLG ppd)
{
  BOOL b ;
  TCHAR szNum[30] ;
  LPDEVNAMES pn ;
  TCHAR sz1[100] ;
  TCHAR sz2[100] ;
  TCHAR sz3[100] ;
  DWORD dwSize ;
  int n1, n2, n3 ;

  GetDlgItemText(hwnd, ID_WDRIVEROFF, sz1, 100) ;
  GetDlgItemText(hwnd, ID_WDEVICEOFF, sz2, 100) ;
  GetDlgItemText(hwnd, ID_WOUTPUTOFF, sz3, 100) ;

  n1 = lstrlen(sz1) ;
  n2 = lstrlen(sz2) ;
  n3 = lstrlen(sz3) ;

  if (hDevNames)
  {
    GlobalFree(hDevNames) ;
    hDevNames = (HANDLE) 0 ;
  }


  /* allocate size of everything + 5 extra bytes for zeroes */

  dwSize = sizeof(DEVNAMES) + ((n1+n2+n3+5) * sizeof(TCHAR)) ;


  /* Allocate the memory and lock it down */

  if (!(hDevNames = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize)))
  {
    MessageBox(hwnd, TEXT("Can't Alloc memory for hDevNames!"),
               TEXT("Fatal Error!"), MB_OK | MB_APPLMODAL) ;
    return ;
  }

  if (!(pn = (LPDEVNAMES) GlobalLock(hDevNames)))
  {
    MessageBox(hwnd, TEXT("Can't Lock New hDevNames memory!"),
               TEXT("Fatal Error"), MB_OK | MB_APPLMODAL) ;

    GlobalFree(hDevNames) ;

    hDevNames = (HANDLE) 0 ;

    return ;
  }


  /* skip over devnames and copy string 1 there.  Make sure to
     divide by sizeof(TCHAR) or the compiler will create a pointer
     that points too far if you compile as a UNICODE program */

  pn->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCHAR) ;
  lstrcpy((LPTSTR) pn + pn->wDriverOffset, (LPTSTR) sz1) ;



  /* skip over string 1 and add string 1 with a zero between them */

  pn->wDeviceOffset = pn->wDriverOffset + n1 + 1 ;
  lstrcpy((LPTSTR) pn + pn->wDeviceOffset, (LPTSTR) sz2) ;



  /* skip over string 2 and put string 3 there with a zero between them */

  pn->wOutputOffset = pn->wDeviceOffset + n2 + 1 ;
  lstrcpy((LPTSTR) pn + pn->wOutputOffset, (LPTSTR) sz3) ;


  GetDlgItemText(hwnd, ID_WDEFAULT, szNum, 30) ;
  pn->wDefault = (WORD) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GlobalUnlock(hDevNames) ;

  ppd->hDevNames = hDevNames ;

  wsprintf(szTemp, szLongFilter, ppd->hDevNames) ;
  SetDlgItemText(GetParent(hwnd), ID_HDEVNAMESP, szTemp) ;

}






/************************************************************************

  Function: FillDevmodeDlg(HWND, LPPRINTDLG)

  Purpose: Fills CDTEST's devmode dialog with the contents of the DEVMODE
           structure handle of a PRINTDLG structure.


  Returns:  Nothing.

  Comments:

************************************************************************/

void FillDevmodeDlg(HWND hwnd, LPPRINTDLG ppd)
{
  LPDEVMODE p ;

  if (ppd->hDevMode == (HANDLE) 0)
    return ;

  p = (LPDEVMODE) GlobalLock(ppd->hDevMode) ;
  if (p == (LPDEVMODE) NULL) return ;

  SetDlgItemText(hwnd, ID_DMDEVNAME, (LPTSTR) p->dmDeviceName) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmSpecVersion) ;
  SetDlgItemText(hwnd, ID_DMSPECVER, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmDriverVersion) ;
  SetDlgItemText(hwnd, ID_DMDRIVERVER, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmSize) ;
  SetDlgItemText(hwnd, ID_DMSIZEDEV, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmDriverExtra) ;
  SetDlgItemText(hwnd, ID_DMDRIVEREXTRA, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmFields) ;
  SetDlgItemText(hwnd, ID_DMFIELDS, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmOrientation) ;
  SetDlgItemText(hwnd, ID_DMORIENTATION, szTemp) ;

  wsprintf(szTemp, szShortFilter, (int) p->dmPaperSize) ;
  SetDlgItemText(hwnd, ID_DMPAPERSIZE, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmPaperLength) ;
  SetDlgItemText(hwnd, ID_DMPAPERLENGTH, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmPaperWidth) ;
  SetDlgItemText(hwnd, ID_DMPAPERWIDTH, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmScale) ;
  SetDlgItemText(hwnd, ID_DMSCALE, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmCopies) ;
  SetDlgItemText(hwnd, ID_DMCOPIES, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmDefaultSource) ;
  SetDlgItemText(hwnd, ID_DMDEFAULTSOURCE, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmPrintQuality) ;
  SetDlgItemText(hwnd, ID_DMPRINTQUALITY, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmColor) ;
  SetDlgItemText(hwnd, ID_DMCOLOR, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmDuplex) ;
  SetDlgItemText(hwnd, ID_DMDUPLEX, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmCollate) ;
  SetDlgItemText(hwnd, ID_DMCOLLATE, szTemp) ;

  SetDlgItemText(hwnd, ID_DMFORMNAME, (LPTSTR) p->dmFormName) ;

  wsprintf(szTemp, szLongFilter, p->dmBitsPerPel) ;
  SetDlgItemText(hwnd, ID_DMBITSPERPEL, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmPelsWidth) ;
  SetDlgItemText(hwnd, ID_DMPELSWIDTH, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmPelsHeight) ;
  SetDlgItemText(hwnd, ID_DMPELSHEIGHT, szTemp) ;

  wsprintf(szTemp, szLongFilter, p->dmDisplayFrequency) ;
  SetDlgItemText(hwnd, ID_DMDISPLAYFREQ, szTemp) ;

  GlobalUnlock(ppd->hDevMode) ;
}









/************************************************************************

  Function: GetDevmodeDlg(HWND, LPPRINTDLG)

  Purpose:  Retrieves the user's edits in CDTEST's DEVMODE dialog box,
            allocates memory for a new DEVMODE structure, fills that
            memory with the user's edits, and puts a handle to that
            memory in the hDevMode member of a PRINTDLG structure.

  Returns:  Nothing.

  Comments:

************************************************************************/

void GetDevmodeDlg(HWND hwnd, LPPRINTDLG ppd)
{
  LPDEVMODE p ;
  BOOL b ;
  TCHAR szNum[30] ;

  #define WMAXDEV 30

  if (hDevMode)
  {
    GlobalFree(hDevMode) ;
    hDevMode = (HANDLE) 0 ;
  }

  if (!(hDevMode = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, sizeof(DEVMODE))))
  {
    MessageBox(hwnd, TEXT("Can't allocate memory for hDevMode"),
               TEXT("Fatal Error"), MB_OK | MB_APPLMODAL) ;
    return ;
  }

  if (!(p = (LPDEVMODE) GlobalLock(hDevMode)))
  {
    MessageBox(hwnd, TEXT("Can't lock memory for hDevMode"),
               TEXT("Fatal Error"), MB_OK | MB_APPLMODAL) ;

    GlobalFree(hDevMode) ;
    hDevMode = (HANDLE) 0 ;

    return ;
  }

  GetDlgItemText(hwnd, ID_DMDEVNAME, p->dmDeviceName, 32) ;

  GetDlgItemText(hwnd, ID_DMSPECVER, szNum, WMAXDEV) ;
  p->dmSpecVersion = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMDRIVERVER, szNum, WMAXDEV) ;
  p->dmDriverVersion = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMSIZEDEV, szNum, WMAXDEV) ;
  p->dmSize = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMDRIVEREXTRA, szNum, WMAXDEV) ;
  p->dmDriverExtra = (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMFIELDS, szNum, WMAXDEV) ;
  p->dmFields = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMORIENTATION, szNum, WMAXDEV) ;
  p->dmOrientation = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPAPERSIZE, szNum, WMAXDEV) ;
  p->dmPaperSize = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPAPERLENGTH, szNum, WMAXDEV) ;
  p->dmPaperLength = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPAPERWIDTH, szNum, WMAXDEV) ;
  p->dmPaperWidth = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMSCALE, szNum, WMAXDEV) ;
  p->dmScale = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMCOPIES, szNum, WMAXDEV) ;
  p->dmCopies = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMDEFAULTSOURCE, szNum, WMAXDEV) ;
  p->dmDefaultSource = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPRINTQUALITY, szNum, WMAXDEV) ;
  p->dmPrintQuality = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMCOLOR, szNum, WMAXDEV) ;
  p->dmColor = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMDUPLEX, szNum, WMAXDEV) ;
  p->dmDuplex = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMCOLLATE, szNum, WMAXDEV) ;
  p->dmCollate = (short) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMFORMNAME, p->dmFormName, 32) ;

  GetDlgItemText(hwnd, ID_DMBITSPERPEL, szNum, WMAXDEV) ;
  p->dmBitsPerPel = (USHORT) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPELSWIDTH, szNum, WMAXDEV) ;
  p->dmPelsWidth = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMPELSHEIGHT, szNum, WMAXDEV) ;
  p->dmPelsHeight = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_DMDISPLAYFREQ, szNum, WMAXDEV) ;
  p->dmDisplayFrequency = MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

  GlobalUnlock(hDevMode) ;
  ppd->hDevMode = hDevMode ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hDevMode) ;
  SetDlgItemText(GetParent(hwnd), ID_HDEVMODEP, szTemp) ;
}






/************************************************************************

  Function: GetPrintDlgHandle(void)

  Purpose:  Finds the custom Print dialog template in the EXE, loads it
            into a handle, and returns the handle.

  Returns:  Nothing.

  Comments:

************************************************************************/

HANDLE GetPrintDlgHandle(void)
{
  hResPrint = FindResource(hInst, TEXT("prtemp1"), RT_DIALOG) ;

  hDialogPrint = LoadResource(hInst, hResPrint) ;


  return hDialogPrint ;
}








/************************************************************************

  Function: GetSetupDlgHandle(void)

  Purpose:  Finds the custom Setup dialog template in the EXE, loads it
            into a handle, and returns the handle.

  Returns:  Nothing.

  Comments:

************************************************************************/

HANDLE GetSetupDlgHandle(void)
{
  hResPrint = FindResource(hInst, TEXT("prtemp2"), RT_DIALOG) ;

  hDialogPrint = LoadResource(hInst, hResPrint) ;

  return hDialogPrint ;
}








/************************************************************************

  Function: PrintHookProc(HWND, UINT, UINT, LONG)

  Purpose:  A callback function that will receive messages intended for
            the PrintDlg() dialog boxes before the normal common dialog
            routine receives them.


  Returns:  FALSE to allow the common dialogs to process the message
            with its normal logic.  TRUE to discard the message.

  Comments:

     To enable this function for the Print dialog in CDTEST, enter the
     value for PD_ENABLEPRINTHOOK in the "Flags" edit box.

     To enable this function for the Setup dialog in CDTEST, enter the
     value for (PD_ENABLESETUPHOOK | PD_PRINTSETUP) in the "Flags" edit box.

************************************************************************/

UINT APIENTRY PrintHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  LPPRINTDLG pPr ;
  TCHAR szMsg[50] ;

  switch(msg)
  {
    case WM_INITDIALOG:

      pPr = (LPPRINTDLG) lParam ;

      if (pPr->lCustData != 0)
      {
        wsprintf(szMsg, TEXT("PRINTDLG->lCustData is: %ld"), pPr->lCustData) ;

        MessageBox(hwnd, szMsg, TEXT("lCustData Sent!"), MB_OK | MB_APPLMODAL) ;
      }

      SetWindowText(hwnd, TEXT("Print Hook Proc Dialog")) ;

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

  Function: PrintMultiProc1(LPDWORD)

  Purpose:  Is the starting address for the first new thread when
            multithreading.

  Returns:  Any DWORD value.

  Comments:
            The new thread will start executing here and will end when
            the PrintDlg() function returns and it has posted a message
            to the CDTEST's print dialog window saying that one of the
            two multithreading dialogs been terminated.

************************************************************************/

DWORD PrintMultiProc1(LPDWORD pdw)
{
  GetPrintDlg(hwndMainPrint, &pdThread1) ;

  DoPrintDlgStuff(hwndMainPrint, &pdThread1) ;

  PostMessage(hwndMainPrint, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}









/************************************************************************

  Function: PrintMultiProc2(LPDWORD)

  Purpose:  Is the starting address for the second new thread when
            multithreading.

  Returns:  Any DWORD value.

  Comments:
            The new thread will start executing here and will end when
            the PrintDlg() function returns and it has posted a message
            to the CDTEST's print dialog window saying that one of the
            two multithreading dialogs been terminated.

************************************************************************/

DWORD PrintMultiProc2(LPDWORD pdw)
{
  GetPrintDlg(hwndMainPrint, &pdThread2) ;

  DoPrintDlgStuff(hwndMainPrint, &pdThread2) ;

  PostMessage(hwndMainPrint, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}








/************************************************************************

  Function: MultiThreadPrintDlg(void)

  Purpose:  Is the starting address for the first new thread when
            multithreading.

  Returns:  Any DWORD value.

  Comments:
            The new thread will start executing here and will end when
            the PrintDlg() function returns and it has posted a message
            to the CDTEST's print dialog window saying that one of the
            two multithreading dialogs been terminated.

************************************************************************/

void MultiThreadPrintDlg(void)
{

  dwPrintThreadParm1 = dwPrintThreadParm2 = 0L ;

  if (!(hPrintThread1 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) PrintMultiProc1,
                                     &dwPrintThreadParm1, CREATE_SUSPENDED, &dwPrintThreadID1)))

  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 1"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenDialogCount = 0 ;

    EnablePrintControls(hwndMainPrint, TRUE) ;

    return ;
  }


  if (!(hPrintThread2 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) PrintMultiProc2,
                                     &dwPrintThreadParm2, CREATE_SUSPENDED, &dwPrintThreadID2)))
  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 2"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenDialogCount = 0 ;

    EnablePrintControls(hwndMainPrint, TRUE) ;

    return ;
  }

  ResumeThread(hPrintThread1) ;

  ResumeThread(hPrintThread2) ;

  return ;
}







/************************************************************************

  Function: EnablePrintControls(HWND, BOOL)

  Purpose:  Enable or disable CDTEST's print dialog's controls.

  Returns:  Nothing.

  Comments: This is necessary when CDTEST is multithreading.

************************************************************************/

void EnablePrintControls(HWND hwnd, BOOL bEnable)
{
  EnableWindow(GetDlgItem(hwnd, IDOK), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, IDCANCEL), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_RESETPRINT), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_MULTITHREADPRINT), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_EDITDEVMODE), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_EDITDEVNAMES), bEnable) ;
}






/************************************************************************

  Function: DoPrintDlgStuff(HWND, LPPRINTDLG)

  Purpose:  Calls PrintDlg() with the correct parameters.

  Returns:  Nothing.

  Comments:

************************************************************************/


void DoPrintDlgStuff(HWND hwnd, LPPRINTDLG ppd)
{
  BOOL bRet = FALSE ;


  /* Figure out how the user want's to call PrintDlg() */

  if (IsDlgButtonChecked(hwnd, ID_PREPRINTP) == 1)
    ppd->hPrintTemplate = GetPrintDlgHandle() ;

  if (IsDlgButtonChecked(hwnd, ID_PRESETUPP) == 1)
    ppd->hSetupTemplate = GetSetupDlgHandle() ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hPrintTemplate) ;
  SetDlgItemText(hwnd, ID_HPRINTTEMPP, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) ppd->hSetupTemplate) ;
  SetDlgItemText(hwnd, ID_HSETUPTEMPP, szTemp) ;



  /* Call the function */

  if (IsDlgButtonChecked(hwnd, ID_NULLSTRUCTP) == 1)
    bRet = PrintDlg((LPPRINTDLG) NULL) ;
  else
    bRet = PrintDlg(ppd) ;



  /* Clean up and show results */

  wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
  SetDlgItemText(hwnd, ID_ERRORP, szTemp) ;

  SetDlgItemInt(hwnd, ID_RETURNP, bRet, TRUE) ;

  if (hDialogPrint)
  {
    FreeResource(hDialogPrint) ; //obsolete call, but
    hDialogPrint = (HANDLE) 0 ;  //it's possible that this
    hResPrint = (HANDLE) 0 ;    //might be recompiled for win 3.x
  }

  pd.hDevMode = ppd->hDevMode ;    //need to fix this so that FillDevnamesDlg()
  pd.hDevNames = ppd->hDevNames ;  //and FillDevmodeDlg() work...

  FillPrintDlg(hwnd, ppd) ;

  if (ppd->hDC)  //release the HDC after its value is
                 //returned to the test app
  {
    ReleaseDC(ppd->hwndOwner, ppd->hDC) ;
    ppd->hDC = (HDC) 0 ;
  }
}
