/************************************************************************

  File: font.c

  Purpose:

    Contains all functions pertinent to CDTEST's Font dialog.

  Functions:

     DoFontDlg()          -- Creates CDTEST's font dialog.

     FontProc()           -- Callback for CDTEST's font dialog.

     InitFontStruct()     -- Fills a CHOOSEFONT structure with some defaults.

     FillFontDlg()        -- Fills CDTEST's font dialog with the contents
                             of a CHOOSEFONT structure.

     GetFontDlg()         -- Retrieves the contents of CDTEST's font dialog
                             and fills a CHOOSEFONT structure with them.

     GetHdc()             -- Creates and returns an HDC

     LogFontProc()        -- Callback function for the logfont dlg.

     InitLogFontStruct()  -- Fills a LOGFONT structure with some defaults.

     FillLogFontDlg()     -- Fills the LOGFONT dlg with the values in
                             a LOGFONT structure.

     GetLogFontDlg()      -- Retrieves the user's edits in the LogFont dlg
                             and puts them in a LOGFONT structure.

     FontHookProc()       -- Callback function for CHOOSEFONT->lpfnHook

     GetFontDlgHandle()   -- Creates a handle to the custom template and
                             returns a handle to it.

     EnumFontsProc()      -- Callback function for EnumFontFamilies()

     FillFontsBox()       -- Fills CDTEST's font list box with requested fonts

     ResetCheckBoxes()    -- handles the font choices check box manipulations

     HandleFontCheckBox() -- Handles the WM_COMMAND messages from the font
                             choices check boxes.

     DoChooseFontStuff()  -- Calls the ChooseFont() function.

     FontThreadProc1()    -- Starting address for the first thread.

     FontThreadProc2()    -- Starting address for the second thread.

     MultiThreadFontDlg() -- Creates two threads which each create a
                             ChooseFont() dialog.

     FontEnableButtons()  -- Enables or disables CDTEST's font dialog buttons.
                             Necessary for multithreading part of this app.

************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <winnls.h>
#include "cdtest.h"
#include "font.h"
#include "logfont.h"



/* some definitions that will help us tell the difference between
   screen and printer fonts */

#define FONT_TYPE_WYSIWYG 1
BOOL bScreen ;



/* function prototypes and general variables */

void InitFontStruct(HWND, LPCHOOSEFONT) ;
void FillFontDlg(HWND, LPCHOOSEFONT) ;
void GetFontDlg(HWND, LPCHOOSEFONT) ;
HDC  GetHdc(HWND, int) ;
void InitLogFontStruct(HWND, LPLOGFONT) ;
void FillLogFontDlg(HWND, LPLOGFONT) ;
void GetLogFontDlg(HWND, LPLOGFONT) ;
BOOL APIENTRY LogFontProc(HWND, UINT, UINT, LONG) ;
UINT APIENTRY FontHookProc(HWND, UINT, UINT, LONG) ;
int  CALLBACK EnumFontsProc(LPLOGFONT, LPTEXTMETRIC, DWORD, LONG) ;
void FillFontsBox(HWND, DWORD) ;
void ResetCheckBoxes(HWND) ;
void HandleFontCheckBox(HWND, int) ;
void DoChooseFontStuff(HWND, LPCHOOSEFONT) ;



/* Global variables and some external variables and functions */

DWORD dwFontFlag ;
PRINTDLG pfd ;
BOOL bLogFontParam ;
LOGFONT lfWM_CF_LF ;                 //for the WM_CHOOSEFONT_GETLOGFONT message
extern UINT uMode ;
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;
HANDLE hResFont, hDialogFont ;
HANDLE GetFontDlgHandle(void) ;
CHOOSEFONT cf ;
LOGFONT lf ;
HDC hdc ;
TCHAR szFaceName[50] ;
TCHAR szFontTemplate[50] ;
TCHAR szFontStyle[50] ;
#define HDCSCREEN 1
#define HDCPRINTER 2
#define HDCNULL 3
#define HDCINVALID 4
INT nHdcType ;



/* Multithreading stuff */

HANDLE hFontThread1 ;
HANDLE hFontThread2 ;
DWORD  dwFontThreadID1 ;
DWORD  dwFontThreadID2 ;
DWORD  dwFontThreadParm1 ;
DWORD  dwFontThreadParm2 ;
DWORD  FontThreadProc1(LPDWORD) ;
DWORD  FontThreadProc2(LPDWORD) ;
HANDLE hwndMainFont ;
int    nOpenFontDialogCount ;
CHOOSEFONT cfThread1 ;
CHOOSEFONT cfThread2 ;
void MultiThreadFontDlg(void) ;
void EnableFontButtons(HWND, BOOL) ;




/************************************************************************

  Function: DoFontDlg(HWND)

  Purpose:

    To create the CDTEST's font dialog

  Returns: Nothing.

  Comments:


************************************************************************/

void DoFontDialog(HWND hwnd)
{


  DialogBox(hInst, MAKEINTRESOURCE(ID_FONTDIALOG),
            hwnd, FontProc) ;

}







/************************************************************************

  Function: FontProc(HWND, UINT, UINT, LONG)

  Purpose:

    Callback function for CDTEST's font dialog.

  Returns: TRUE or FALSE depending on the situation.

  Comments:


************************************************************************/


BOOL APIENTRY FontProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  BOOL b ;
  TCHAR szNum[30] ;

  switch (msg)
  {
    case WM_INITDIALOG:

        InitLogFontStruct(hwnd, &lf) ;   //fill (struct lf) so the choosefont
        InitFontStruct(hwnd, &cf) ;      //struct can use it
        FillFontDlg(hwnd, &cf) ;

        CheckRadioButton(hwnd, ID_HDCSCREEN, ID_HDCPRINTER, ID_HDCSCREEN) ;
        nHdcType = HDCSCREEN ;

        dwFontFlag = cf.Flags ;

        FillFontsBox(hwnd, dwFontFlag) ;

        ResetCheckBoxes(hwnd) ;

        *(&cfThread1) = *(&cfThread2) = *(&cf) ;

        hwndMainFont = hwnd ;
        nOpenFontDialogCount = 0 ;

        SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEF)) ;

        break ;


    case UMSG_DECREMENTDLGCOUNT:  // user defined message.  This is send by
                                  // the functions that are executing when
                                  // a new thread is created.  When these
                                  // Thread functions end, they should send
                                  // this message.

      nOpenFontDialogCount-- ;

      if (nOpenFontDialogCount == 0)
        EnableFontButtons(hwnd, TRUE) ;

      break ;


    case WM_CF_LF:

       /* If this message comes in, we know that the user clicked the
          button that tells the ChooseFont() dialog to tell the parent
          to send the WM_CHOOSEFONT_GETLOGFONT message.  So what we will
          do is send the message and then show the user what was in the
          logfont by calling creating the Logfont dialog box */


       SendMessage((HWND) lParam, WM_CHOOSEFONT_GETLOGFONT,
                   0, (LPARAM) &lfWM_CF_LF) ;

       bLogFontParam = TRUE ; //tells us if we are doing normal logfont
                              //processing or answering the WM_CF_LF message.

       DialogBoxParam(hInst, MAKEINTRESOURCE(ID_LOGFONTDIALOG),
                      (HWND) lParam, LogFontProc, (LPARAM) &lf) ;

       bLogFontParam = FALSE ;

       break ;


    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            DoChooseFontStuff(hwnd, &cf) ;  //create the dialog.
            break ;

          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;
            break ;

          case ID_RESETFONT:

            InitLogFontStruct(hwnd, &lf) ;  //reset all the structures.
            InitFontStruct(hwnd, &cf) ;
            FillFontDlg(hwnd, &cf) ;        //refill the dialog.

            CheckRadioButton(hwnd, ID_HDCSCREEN, ID_HDCPRINTER, ID_HDCSCREEN) ;
            nHdcType = HDCSCREEN ;

            SendDlgItemMessage(hwnd, ID_NULLSTRUCTFONT, BM_SETCHECK, (WPARAM) 0, (LPARAM)0) ;
            SendDlgItemMessage(hwnd, ID_PRELOADEDFONT,  BM_SETCHECK, (WPARAM) 0, (LPARAM)0) ;

            dwFontFlag = cf.Flags ;
            FillFontsBox(hwnd, dwFontFlag) ;
            ResetCheckBoxes(hwnd) ;

            SetFocus(GetDlgItem(hwnd, ID_STRUCTSIZEF)) ;

            break ;


          case ID_HDCSCREEN:
            nHdcType = HDCSCREEN ;
            break ;

          case ID_HDCPRINTER:
            nHdcType = HDCPRINTER ;
            break ;

          case ID_HDCNULL:
            nHdcType = HDCNULL ;
            break ;

          case ID_HDCINVALID:
            nHdcType = HDCINVALID ;
            break ;

          case ID_EDITLOGFONT:

            /* Get the address of the logfont and then show the user what
               is in it with the Logfont dialog box */

            GetDlgItemText(hwnd, ID_LOGFONTF, szNum, 30) ;
            cf.lpLogFont = (LPLOGFONT) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

            DialogBox(hInst, MAKEINTRESOURCE(ID_LOGFONTDIALOG),
                      hwnd, LogFontProc) ;
            break ;


          case F_TTONLY:
          case F_ANSIONLY:
          case F_PRINTERFONTS:
          case F_SCREENFONTS:
          case F_FIXEDPITCHONLY:
          case F_NOOEMFONTS:
          case F_NOVECTORFONTS:
          case F_SCALABLEONLY:
          case F_WYSIWYG:
            HandleFontCheckBox(hwnd, LOWORD(wParam)) ;
            break ;


          case ID_MULTITHREADFONT:

            /* Set the dialog count to 2, disable the buttons in CDTEST's font
               dialog so the user can't do anything until last dialog has
               been terminated */

            nOpenFontDialogCount = 2 ;

            EnableFontButtons(hwndMainFont, FALSE) ;

            /* And then do the multithreading */

            MultiThreadFontDlg() ;

            break ;


          default: break ;
        }
    }

    default:

      /*
         If the help button is pressed in the ChooseFont()
         dialog, it will send a message Registered with RegisterWindowMessage()
         to the parent window.  The message nHelpMessage was registered
         at application startup

         It must be detected with an IF statement because the value
         returned by RegisterWindowMessage() is not a constant
      */

      if (msg == nHelpMessage)
        MessageBox(GetForegroundWindow(), TEXT("Hello from the help button"),
                   TEXT("Font Help Button"), MB_OK | MB_APPLMODAL) ;
      break ;
  }

  return FALSE ;
}








/************************************************************************

  Function: InitFontStruct(HWND, LPCHOOSEFONT)

  Purpose:

    Fill the CHOOSEFONT structure passed as the second parameter with
    some default values.

  Returns: Nothing.

  Comments:


************************************************************************/

void InitFontStruct(HWND hwnd, LPCHOOSEFONT pcf)
{
  pcf->lStructSize = sizeof(CHOOSEFONT) ;
  pcf->hwndOwner = hwnd ;
  pcf->hDC = (HDC) 0 ;
  pcf->lpLogFont = &lf ;
  pcf->iPointSize = 24 ;
  pcf->Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_SHOWHELP | CF_APPLY ;
  pcf->rgbColors = RGB(0, 255, 0) ;
  pcf->lCustData = 0L ;
  pcf->lpfnHook = FontHookProc ;

  lstrcpy(szFontTemplate, TEXT("fonttemp")) ;
  pcf->lpTemplateName = szFontTemplate ;

  pcf->hInstance = (HANDLE) hInst ;

  lstrcpy(szFontStyle, TEXT("Bold")) ;
  pcf->lpszStyle = szFontStyle ;

  pcf->nFontType = SCREEN_FONTTYPE ;

  pcf->nSizeMin = 8 ;
  pcf->nSizeMax = 50 ;
}










/************************************************************************

  Function: FillFontDlg(HWND, LPCHOOSEFONT)

  Purpose:

    Fill CDTEST's font dialog with the values in the CHOOSEFONT structure
    passed via the second parameter.

  Returns: Nothing.

  Comments:


************************************************************************/

void FillFontDlg(HWND hwnd, LPCHOOSEFONT pcf)
{
  wsprintf(szTemp, szLongFilter, pcf->lStructSize) ;
  SetDlgItemText(hwnd, ID_STRUCTSIZEF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->hwndOwner) ;
  SetDlgItemText(hwnd, ID_HWNDOWNERF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->hDC) ;
  SetDlgItemText(hwnd, ID_HDCF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->hDC) ;
  SetDlgItemText(hwnd, ID_HDCF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->lpLogFont) ;
  SetDlgItemText(hwnd, ID_LOGFONTF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->iPointSize) ;
  SetDlgItemText(hwnd, ID_POINTSIZEF, szTemp) ;

  wsprintf(szTemp, szLongFilter, pcf->Flags) ;
  SetDlgItemText(hwnd, ID_FLAGSF, szTemp) ;

  wsprintf(szTemp, szLongFilter, pcf->rgbColors) ;
  SetDlgItemText(hwnd, ID_RGBCOLORSF, szTemp) ;

  wsprintf(szTemp, szLongFilter, pcf->lCustData) ;
  SetDlgItemText(hwnd, ID_CUSTDATAF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->lpfnHook) ;
  SetDlgItemText(hwnd, ID_HOOKF, szTemp) ;

  SetDlgItemText(hwnd, ID_TEMPLATEF, pcf->lpTemplateName) ;

  wsprintf(szTemp, szLongFilter, (DWORD) pcf->hInstance) ;
  SetDlgItemText(hwnd, ID_HINSTANCEF, szTemp) ;

  SetDlgItemText(hwnd, ID_STYLEF, pcf->lpszStyle) ;

  wsprintf(szTemp, szLongFilter, (int) pcf->nFontType) ;
  SetDlgItemText(hwnd, ID_FONTTYPEF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (int) pcf->nSizeMin) ;
  SetDlgItemText(hwnd, ID_SIZEMINF, szTemp) ;

  wsprintf(szTemp, szLongFilter, (int) pcf->nSizeMax) ;
  SetDlgItemText(hwnd, ID_SIZEMAXF, szTemp) ;

}







/************************************************************************

  Function: GetFontDlg(HWND, LPCHOOSEFONT)

  Purpose:

    Retrieve the users edits from CDTEST's font dialog and put them
    in the CHOOSEFONT structure passed in as the second parameter

  Returns: Nothing.

  Comments:


************************************************************************/

void GetFontDlg(HWND hwnd, LPCHOOSEFONT pcf)
{
  BOOL b ;
  TCHAR szNum[30] ;

#define WSIZEFO 30

  GetDlgItemText(hwnd, ID_STRUCTSIZEF, szNum, WSIZEFO) ;
  pcf->lStructSize = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HWNDOWNERF, szNum, WSIZEFO) ;
  pcf->hwndOwner = (HWND) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LOGFONTF, szNum, WSIZEFO) ;
  pcf->lpLogFont = (LPLOGFONT) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_POINTSIZEF, szNum, WSIZEFO) ;
  pcf->iPointSize = (int) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_FLAGSF, szNum, WSIZEFO) ;
  pcf->Flags = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_RGBCOLORSF, szNum, WSIZEFO) ;
  pcf->rgbColors = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_CUSTDATAF, szNum, WSIZEFO) ;
  pcf->lCustData = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_HOOKF, szNum, WSIZEFO) ;
  pcf->lpfnHook = (LPCFHOOKPROC) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_TEMPLATEF, szFontTemplate, 50) ;

  GetDlgItemText(hwnd, ID_HINSTANCEF, szNum, WSIZEFO) ;
  pcf->hInstance = (HANDLE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_STYLEF, szFontStyle, 50) ;

  GetDlgItemText(hwnd, ID_FONTTYPEF, szNum, WSIZEFO) ;
  pcf->nFontType = (int) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_SIZEMINF, szNum, WSIZEFO) ;
  pcf->nSizeMin = (int) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_SIZEMAXF, szNum, WSIZEFO) ;
  pcf->nSizeMax = (int) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;
}







/************************************************************************

  Function: GetHdc(HWND, int)

  Purpose:

    Creates and returns an HDC of the type specified.


  Returns: The HDC that it creates.


  Comments:

    The NULL and INVALID HDCs are only useful to test if the common dialogs
    handle an HDC that is not useful, but this function is also necessary
    in that the HDC must be a printer HDC if ChooseFont() is called
    with the CF_PRINTERFONTS flag bit set.

    To tell ChooseFont() to list only printer fonts:

    1.  Set the HDC in CDTEST's font dialog to "Printer"

    2.  Set the "Flags" edit box CF_PRINTERFONTS


************************************************************************/



HDC GetHdc(HWND hwnd, int nType)
{
  switch (nType)
  {
    case HDCSCREEN:
        return GetDC(hwnd) ;
        break ;

    case HDCNULL:
        return (HDC) 0 ;
        break ;

    case HDCINVALID:
        return (HDC) 999 ;
        break ;

    case HDCPRINTER:

        /* To get the HDC of the current printer, fill out a PRINTDLG
           structure, and set it's flags to (PD_RETURNDC | PD_RETURNDEFAULT).
           This will tell PrintDlg() to create you an HDC but not show
           the Print dialog box. */

        pfd.lStructSize = sizeof(PRINTDLG) ;
        pfd.hwndOwner = hwnd ;
        pfd.hDevMode = (HANDLE) 0 ;
        pfd.hDevNames = (HANDLE) 0 ;
        pfd.Flags = PD_RETURNDC | PD_RETURNDEFAULT ;   //just get default printer
        pfd.nFromPage = 0 ;
        pfd.nToPage = 0 ;
        pfd.nMinPage = 0 ;
        pfd.nMaxPage = 0 ;
        pfd.nCopies = 0 ;
        pfd.hInstance = (HANDLE) hInst ;

        if (PrintDlg(&pfd) == 0)
            MessageBox(hwnd, TEXT("Error: Could not create a printer DC!"),
                       (LPTSTR) NULL, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;
        else
           return pfd.hDC ;

        break ;

    default:
        break ;
  }
  return (HDC) 0 ;
}









/************************************************************************

  Function: LogFontProc(HWND, int)

  Purpose:

    The callback proc for the LogFont dialog box


  Returns: BOOL -- depending on the situation.


  Comments:

************************************************************************/


BOOL APIENTRY LogFontProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  switch (msg)
  {
    case WM_INITDIALOG:
        FillLogFontDlg(hwnd, cf.lpLogFont) ;  //start off with the one that
                                              //is returned from ChooseFont()
        break ;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
          case IDOK:

            GetLogFontDlg(hwnd, &lf) ;

            cf.lpLogFont = &lf ;

            EndDialog(hwnd, TRUE) ;

            break ;



          case ID_RESETLF:

            SetFocus(GetDlgItem(hwnd, ID_LFHEIGHT)) ;

            InitLogFontStruct(hwnd, &lf) ;

            FillLogFontDlg(hwnd, &lf) ;

            break ;


          case IDCANCEL:

            EndDialog(hwnd, FALSE) ;

            break ;


          default:
            break ;
        }

    default: break ;
  }


  return FALSE ;
}







/************************************************************************

  Function: InitLogFontStruct(HWND, LPLOGFONT)

  Purpose:

    Fills the LOGFONT structure passed in as the second parameter with
    some default values.

  Returns: Nothing.

  Comments:

************************************************************************/

void InitLogFontStruct(HWND hwnd, LPLOGFONT plf)
{
  plf->lfHeight = 24 ;
  plf->lfWidth  = 20 ;
  plf->lfEscapement = 0 ;
  plf->lfOrientation = 10 ;
  plf->lfWeight = 400 ;
  plf->lfItalic = FALSE ;
  plf->lfUnderline = FALSE ;
  plf->lfStrikeOut = FALSE ;
  plf->lfCharSet = ANSI_CHARSET ;
  plf->lfOutPrecision = OUT_DEFAULT_PRECIS ;
  plf->lfClipPrecision = CLIP_DEFAULT_PRECIS ;
  plf->lfQuality = DEFAULT_QUALITY ;
  plf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE ;

  szFaceName[0] = (TCHAR) 0 ;
  lstrcpy(plf->lfFaceName, szFaceName) ;
}






/************************************************************************

  Function: FillLogFontDlg(HWND, LPLOGFONT)

  Purpose:

    Fills CDTEST's logfont dialog with the values in the logfont that
    is passed in as the second parameter.

  Returns: Nothing.

  Comments:

************************************************************************/

void FillLogFontDlg(HWND hwnd, LPLOGFONT plf)
{
  TCHAR szTemp[50] ;

  LPLOGFONT pl = plf ;

  if (bLogFontParam)    //Are creating this dialog in response to the
    pl = &lfWM_CF_LF ;  //WM_CHOOSEFONT_GETLOGFONT message ?

  if (!pl)              //Avert a ghastly exception error
    return ;

  wsprintf(szTemp, szLongFilter, pl->lfHeight) ;
  SetDlgItemText(hwnd, ID_LFHEIGHT, szTemp) ;

  wsprintf(szTemp, szLongFilter, pl->lfWidth) ;
  SetDlgItemText(hwnd, ID_LFWIDTH, szTemp) ;

  wsprintf(szTemp, szLongFilter, pl->lfEscapement) ;
  SetDlgItemText(hwnd, ID_LFESCAPEMENT, szTemp) ;

  wsprintf(szTemp, szLongFilter, pl->lfOrientation) ;
  SetDlgItemText(hwnd, ID_LFORIENTATION, szTemp) ;

  wsprintf(szTemp, szLongFilter, pl->lfWeight) ;
  SetDlgItemText(hwnd, ID_LFWEIGHT, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfItalic) ;
  SetDlgItemText(hwnd, ID_LFITALIC, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfUnderline) ;
  SetDlgItemText(hwnd, ID_LFUNDERLINE, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfStrikeOut) ;
  SetDlgItemText(hwnd, ID_LFSTRIKEOUT, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfCharSet) ;
  SetDlgItemText(hwnd, ID_LFCHARSET, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfOutPrecision) ;
  SetDlgItemText(hwnd, ID_LFOUTP, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfClipPrecision) ;
  SetDlgItemText(hwnd, ID_LFCLIPP, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfQuality) ;
  SetDlgItemText(hwnd, ID_LFQUALITY, szTemp) ;

  wsprintf(szTemp, szShortFilter, pl->lfPitchAndFamily) ;
  SetDlgItemText(hwnd, ID_LFPITCHANDFAM, szTemp) ;

  SetDlgItemText(hwnd, ID_LFFACENAME, (LPTSTR) pl->lfFaceName) ;
}






/************************************************************************

  Function: GetLogFontDlg(HWND, LPLOGFONT)

  Purpose:

    Retrieves the users edits in CDTEST's Logfont dialog and puts them
    into the LOGFONT struct that is passed as the second parameter.

  Returns: Nothing.

  Comments:

************************************************************************/

void GetLogFontDlg(HWND hwnd, LPLOGFONT plf)
{
  BOOL b ;
  TCHAR szNum[30] ;

  if (bLogFontParam)  //if we are just viewing the logfont because we sent a
    return ;          //WM_CHOOSEFONT_GETLOGFONT message, don't change the Logfont Struct
                      //that ChooseFont() gives us...

  #define LFSIZE 30

  GetDlgItemText(hwnd, ID_LFHEIGHT, szNum, LFSIZE) ;
  plf->lfHeight = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFWIDTH, szNum, LFSIZE) ;
  plf->lfWidth = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFESCAPEMENT, szNum, LFSIZE) ;
  plf->lfEscapement = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFORIENTATION, szNum, LFSIZE) ;
  plf->lfOrientation = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFWEIGHT, szNum, LFSIZE) ;
  plf->lfWeight = MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFITALIC, szNum, LFSIZE) ;
  plf->lfItalic = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFUNDERLINE, szNum, LFSIZE) ;
  plf->lfUnderline = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFSTRIKEOUT, szNum, LFSIZE) ;
  plf->lfStrikeOut = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFCHARSET, szNum, LFSIZE) ;
  plf->lfCharSet = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFOUTP, szNum, LFSIZE) ;
  plf->lfOutPrecision = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFCLIPP, szNum, LFSIZE) ;
  plf->lfClipPrecision = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFQUALITY, szNum, LFSIZE) ;
  plf->lfQuality = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFPITCHANDFAM, szNum, LFSIZE) ;
  plf->lfPitchAndFamily = (BYTE) MyAtol(szNum, uMode == IDM_HEXMODE, &b) ;

  GetDlgItemText(hwnd, ID_LFFACENAME, (LPTSTR) plf->lfFaceName, 32) ;
}








/************************************************************************

  Function: FontHookProc(HWND, UINT, UINT, LONG)

  Purpose: The callback function that acts as the hook proc for
           the ChooseFont() dialog.


  Returns: TRUE to discard the message.  FALSE to send the message on
           to the normal dialog processing done by the ChooseFont()
           function.

  Comments:

************************************************************************/

UINT APIENTRY FontHookProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  LPCHOOSEFONT pCf ;
  TCHAR szMsg[50] ;

  switch(msg)
  {
    case WM_INITDIALOG:

      pCf = (LPCHOOSEFONT) lParam ;

      if (pCf->lCustData != 0L)
      {
        wsprintf(szMsg, TEXT("CHOOSEFONT->lCustData is: %ld"), pCf->lCustData) ;
        MessageBox(hwnd, szMsg, TEXT("lCustData Sent!"), MB_OK) ;
      }

      SetWindowText(hwnd, TEXT("Font Hook Proc Dialog")) ;

      break ;

    case WM_COMMAND:

        /* NOTE: This button will only be available if the user creates the
           ChooseFont() dialog with the CF_ENABLETEMPLATE or CF_ENABLETEMPLATEHANDLE
           flag.

           This button will only work if the ChooseFont() dialog is created
           with the CF_ENABLEHOOK flag.  So, to get full functionality from
           this button:

           1.  Enter (CF_ENABLEHOOK | CF_ENABLETEMPLATE | CF_SCREENFONTS)
               in the "Flags" edit box of CDTEST'S Font dialog.

           2.  Click OK
        */



        /* The parent will send the WM_CHOOSEFONT_GETLOGFONT message and
           display it's findings in a LogFont dialog if it gets this message */

        if (LOWORD(wParam) == ID_SEND_WM_CF_LF_MSG)
          SendMessage(GetParent(hwnd), WM_CF_LF, 0, (LPARAM) hwnd) ;

        break ;


    default:
      break ;
  }

  return FALSE ;   //send msg to the common dialog code
}









/************************************************************************

  Function: GetFontDlgHandle(void)

  Purpose:  Finds and loads the custom template resource and returns a
            handle to it.


  Returns: A handle to the custom template resource.

  Comments:

************************************************************************/

HANDLE GetFontDlgHandle(void)
{

  hResFont = FindResource(hInst, TEXT("fonttemp"), RT_DIALOG) ;

  hDialogFont = LoadResource(hInst, hResFont) ;


  return hDialogFont ;
}








/************************************************************************

  Function: EnumFontsProc(LPLOGFONT, LPTEXTMETRIC, DWORD, LONG)

  Purpose: Acts as a callback function for the EnumFontFamilies()
           function.

  Returns: 0 if the font enumeration should stop.  1 to ask for the
           next font.

  Comments:

    EnumFontFamilies will find the fonts on the system and call this
    function each time it finds a font.  Pointers to the LOGFONT and
    TEXTMETRIC structures the describe the font are passed as the first
    and second arguments, so we can determine if the fonts we get
    meet the specs we want.  If they do, we can add them to the font
    list box in CDTEST's Font dialog.

    NOTE:

    None of this affects the performance of ChooseFont().  This serves
    only to illustrate how ChooseFont() finds the fonts you request based
    on the flags you pass to the ChooseFont() function.

    The fonts found with this function should be the same as the ones
    found by ChooseFont() for any flag combination.


************************************************************************/


int CALLBACK EnumFontsProc(LPLOGFONT lplf, LPTEXTMETRIC lptm,
                           DWORD dwStyle, LONG lParam)
{
  UINT i ;

  /* check to see if the font is already there right off the bat
     and just continue the enumeration if it is...        */


  i = SendDlgItemMessage((HWND) lParam, ID_FONTLIST, LB_FINDSTRINGEXACT, (WPARAM) 0,
                         (LONG) (LPTSTR) lplf->lfFaceName) ;

  if (i != LB_ERR)
  {
    if (bScreen)     //if we are adding it for the first time
      return 1 ;

    //else, we are adding printer fonts, so if we find one that is already there
    //it must be available for both the printer and the screen...

    else
    {
      SendDlgItemMessage((HWND) lParam, ID_FONTLIST, LB_SETITEMDATA, i,
                          FONT_TYPE_WYSIWYG) ;
      return 1 ;
    }
  }


  //Get rid of any fonts that we don't want...

  if ((dwFontFlag & CF_TTONLY) && (!(dwStyle & TRUETYPE_FONTTYPE)))
    return 1 ;

  if ((dwFontFlag & CF_SCALABLEONLY) && (dwStyle & RASTER_FONTTYPE))
    return 1 ;

  if ((dwFontFlag & CF_ANSIONLY) && (lplf->lfCharSet != ANSI_CHARSET))
    return 1 ;

  if ((dwFontFlag & CF_FIXEDPITCHONLY) && (lplf->lfPitchAndFamily & VARIABLE_PITCH))
    return 1 ;

  if ((dwFontFlag & CF_NOVECTORFONTS) && (lplf->lfCharSet == OEM_CHARSET))
    return 1 ;


  //if there is a font to be added, add it.

  if (*(lplf->lfFaceName))
    SendDlgItemMessage((HWND) lParam, ID_FONTLIST, LB_ADDSTRING,
                     (WPARAM) 0, (LONG) (LPTSTR) lplf->lfFaceName) ;

  return 1 ;
}








/************************************************************************

  Function: FillFontsBox(HWND, DWORD)

  Purpose: Enumerates all fonts on the system and sends the ones that
           meet the criteria of the dwFlags parameter (CF_SCREENFONTS,
           CF_SCALABLEONLY, etc).

  Returns: Nothing.

  Comments:


************************************************************************/

void FillFontsBox(HWND hwnd, DWORD dwFlags)
{
  HDC hdc ;
  HWND hwndControl ;
  DWORD dwData ;
  int nItemCount ;


  /* Empty the list box, and turn of the redraw because we may need
     to remove some fonts after they are added if the user calls this
     function with the CF_WYSIWYG flag bit set. */

  SendDlgItemMessage(hwnd, ID_FONTLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM) 0) ;

  hwndControl = GetDlgItem(hwnd, ID_FONTLIST) ;

  SendMessage(hwndControl, WM_SETREDRAW, FALSE, 0L) ;


  /* Now call EnumFontFamilies() for each type of HDC requested */

  if (dwFlags & CF_SCREENFONTS)
  {
    bScreen = TRUE ;
    hdc = GetHdc(hwnd, HDCSCREEN) ;
    EnumFontFamilies(hdc, (LPTSTR) NULL, (FONTENUMPROC) EnumFontsProc, (LONG) hwnd) ;
    ReleaseDC(hwnd, hdc) ;
  }

  if (dwFlags & CF_PRINTERFONTS)
  {
    bScreen = FALSE ;
    hdc = GetHdc(hwnd, HDCPRINTER) ;
    EnumFontFamilies(hdc, (LPTSTR) NULL, (FONTENUMPROC) EnumFontsProc, (LONG) hwnd) ;
    ReleaseDC(hwnd, hdc) ;
  }


  /* Special case:  If the CF_WYSIWYG flag is used, we have to enumerate
     the fonts in both HDCs and then remove all the ones that are not
     both printer fonts and screen fonts. */

  if (dwFontFlag & CF_WYSIWYG)
  {
    nItemCount = SendDlgItemMessage(hwnd, ID_FONTLIST, LB_GETCOUNT, 0, 0L) ;

    nItemCount-- ;   //the list is zero based

    while (nItemCount >= 0)
    {
      dwData = SendDlgItemMessage(hwnd, ID_FONTLIST, LB_GETITEMDATA, nItemCount, 0L) ;

      if (dwData != FONT_TYPE_WYSIWYG)
      {
        SendDlgItemMessage(hwnd, ID_FONTLIST, LB_DELETESTRING, nItemCount, 0L) ;
      }

      nItemCount-- ;
    }
  }


  /* Now redraw the font list */

  SendMessage(hwndControl, WM_SETREDRAW, TRUE, 0L) ;
  InvalidateRect(hwndControl, NULL, FALSE) ;

  return ;
}






/************************************************************************

  Function: ResetCheckBoxes(HWND)

  Purpose: Checks the checkboxes that control what fonts are listed in
           CDTEST's font list box.

  Returns: Nothing.

  Comments:


************************************************************************/

void ResetCheckBoxes(HWND hwnd)
{

  SendDlgItemMessage(hwnd, F_TTONLY,          BM_SETCHECK, dwFontFlag & CF_TTONLY         ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_ANSIONLY,        BM_SETCHECK, dwFontFlag & CF_ANSIONLY       ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_SCREENFONTS,     BM_SETCHECK, dwFontFlag & CF_SCREENFONTS    ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_PRINTERFONTS,    BM_SETCHECK, dwFontFlag & CF_PRINTERFONTS   ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_NOOEMFONTS,      BM_SETCHECK, dwFontFlag & CF_NOOEMFONTS     ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_NOVECTORFONTS,   BM_SETCHECK, dwFontFlag & CF_NOVECTORFONTS  ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_SCALABLEONLY,    BM_SETCHECK, dwFontFlag & CF_SCALABLEONLY   ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_WYSIWYG,         BM_SETCHECK, dwFontFlag & CF_WYSIWYG        ? 1 : 0, (LPARAM)0) ;
  SendDlgItemMessage(hwnd, F_FIXEDPITCHONLY,  BM_SETCHECK, dwFontFlag & CF_FIXEDPITCHONLY ? 1 : 0, (LPARAM)0) ;

}






/************************************************************************

  Function: HandleFontCheckBox(HWND)

  Purpose: If the user clicks one of the fonts in the CDTEST's list of
           valid font types, this function will check the correct boxes
           and set the correct values in the flag which keeps track of
           which fonts are to be enumerated and added to CDTEST's font
           list box.


  Returns: Nothing.

  Comments:

    CF_NOOEMFONTS and CF_NOVECTORFONTS are identical.


************************************************************************/

void HandleFontCheckBox(HWND hwnd, int nID)
{
  DWORD dwTemp ;

  dwTemp = dwFontFlag ;

  switch (nID)
  {
    case F_TTONLY:

      /* TTONLY is a special case.  If we only want Truetype fonts,
         turn off irrelevant flags */

      if (!(dwFontFlag & CF_TTONLY)) //if we're turning this flag on...
      {
        dwFontFlag = 0L ;  //turn everyone off but the following:

        dwFontFlag |= CF_TTONLY ;

        if (dwTemp & CF_ANSIONLY)
          dwFontFlag |= CF_ANSIONLY ;

        if (dwTemp & CF_FIXEDPITCHONLY)
          dwFontFlag |= CF_FIXEDPITCHONLY ;

        if (dwTemp & CF_SCREENFONTS)
          dwFontFlag |= CF_SCREENFONTS ;

        if (dwTemp & CF_PRINTERFONTS)
          dwFontFlag |= CF_PRINTERFONTS ;
      }
      else
      {
        dwFontFlag ^= CF_TTONLY ;
      }


      break ;

    case F_ANSIONLY:
      dwFontFlag ^= CF_ANSIONLY ;        //otherwise toggle the bit.
      dwFontFlag &= ~CF_WYSIWYG ;

      break ;

    case F_PRINTERFONTS:
      dwFontFlag ^= CF_PRINTERFONTS ;

      break ;

    case F_SCREENFONTS:
      dwFontFlag ^= CF_SCREENFONTS ;

      break ;

    case F_FIXEDPITCHONLY:
      dwFontFlag ^= CF_FIXEDPITCHONLY ;

      break ;

    case F_NOOEMFONTS:
      dwFontFlag ^= CF_NOOEMFONTS ;

      break ;

    case F_NOVECTORFONTS:
      dwFontFlag ^= CF_NOVECTORFONTS ;

      break ;

    case F_SCALABLEONLY:
      dwFontFlag ^= CF_SCALABLEONLY ;

      break ;

    case F_WYSIWYG:                        //We want fonts for both the
                                           //screen and the printer...
      dwFontFlag ^= CF_WYSIWYG ;

      if (dwFontFlag & CF_WYSIWYG)
        dwFontFlag |= (CF_SCREENFONTS | CF_PRINTERFONTS | CF_SCALABLEONLY) ;

      break ;

    default: break ;
  }

  ResetCheckBoxes(hwnd) ;
  FillFontsBox(hwnd, dwFontFlag) ;
}







/************************************************************************

  Function: DoChooseFontStuff(HWND, LPCHOOSEFONT)

  Purpose:

    Calls the ChooseFont() function.

  Returns: Nothing.

  Comments:


************************************************************************/


void DoChooseFontStuff(HWND hwnd, LPCHOOSEFONT pcf)
{
  BOOL bRet = FALSE ;

  if (IsDlgButtonChecked(hwnd, ID_PRELOADEDFONT) == 1)
  {
    pcf->hInstance = GetFontDlgHandle() ;
    wsprintf(szTemp, szLongFilter, pcf->hInstance) ;
    SetDlgItemText(hwnd, ID_HINSTANCEF, szTemp) ;
  }

  pcf->hDC = GetHdc(hwnd, nHdcType) ;

  wsprintf(szTemp, szLongFilter, pcf->hDC) ;
  SetDlgItemText(hwnd, ID_HDCF, szTemp) ;


  GetFontDlg(hwnd, pcf) ;


  if (IsDlgButtonChecked(hwnd, ID_NULLSTRUCTFONT) == 1)
    bRet = ChooseFont((LPCHOOSEFONT)NULL) ;
  else
    bRet = ChooseFont(pcf) ;

  wsprintf(szTemp, szLongFilter, CommDlgExtendedError()) ;
  SetDlgItemText(hwnd, ID_ERRORF, szTemp) ;

  if (pcf->hDC)
  {
    ReleaseDC(hwnd, pcf->hDC) ;   //free the HDC that we used if we called
    pcf->hDC = (HDC) 0 ;          //the function with a valid HDC.
  }

  if (hDialogFont)
  {
    FreeResource(hDialogFont) ;
    hDialogFont = (HANDLE) 0 ;
    hResFont = (HANDLE) 0 ;
  }

  wsprintf(szTemp, szShortFilter, bRet) ;     //fill results into the Font dlg
  SetDlgItemText(hwnd, ID_RETURNF, szTemp) ;

  FillFontDlg(hwnd, pcf) ;
}






/************************************************************************

  Function: FontThreadProc1(LPDWORD)

  Purpose:

    Acts as the starting address for thread 1

  Returns: Any DWORD value.

  Comments:


************************************************************************/

DWORD FontThreadProc1(LPDWORD pdw)
{
  DoChooseFontStuff(hwndMainFont, &cfThread1) ;

  PostMessage(hwndMainFont, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}







/************************************************************************

  Function: FontThreadProc2(LPDWORD)

  Purpose:

    Acts as the starting address for thread 2

  Returns: Any DWORD value.

  Comments:


************************************************************************/


DWORD FontThreadProc2(LPDWORD pdw)
{
  DoChooseFontStuff(hwndMainFont, &cfThread2) ;

  PostMessage(hwndMainFont, UMSG_DECREMENTDLGCOUNT, 0, 0L ) ;

  return 0L ;
}







/************************************************************************

  Function: MultiThreadFontDlg(void)

  Purpose:

    Create the two threads that will in turn create two ChooseFont()
    dialogs.

  Returns: Nothing.

  Comments:

    Multithreading note:

    This function will return before the common dialog functions return.
    Therefore, do not pass any parameters to this function that will be
    referenced by the common dialogs because as soon as this function
    ends those parameters will be gone.

************************************************************************/

void MultiThreadFontDlg(void)
{

  dwFontThreadParm1 = dwFontThreadParm2 = 0L ;

  if (!(hFontThread1 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) FontThreadProc1,
                                     &dwFontThreadParm1, CREATE_SUSPENDED, &dwFontThreadID1)))

  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 1"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenFontDialogCount = 0 ;

    EnableFontButtons(hwndMainFont, TRUE) ;

    return ;
  }


  if (!(hFontThread2 = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
                                     (LPTHREAD_START_ROUTINE) FontThreadProc2,
                                     &dwFontThreadParm2, CREATE_SUSPENDED, &dwFontThreadID2)))
  {
    MessageBox(GetForegroundWindow(), TEXT("Error creating thread 2"), NULL,
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL) ;

    nOpenFontDialogCount = 0 ;

    EnableFontButtons(hwndMainFont, TRUE) ;

    return ;
  }

  ResumeThread(hFontThread1) ;
  ResumeThread(hFontThread2) ;

  return ;
}






/************************************************************************

  Function: EnableFontButtons(HWND, BOOL)

  Purpose:

    Enables or disables CDTEST's font dialogs buttons based on the
    status of the second parameter.


  Returns: Nothing.

  Comments:

    This is necessary when multithreading in the case of this application.

************************************************************************/

void EnableFontButtons(HWND hwnd, BOOL bEnable)
{
  EnableWindow(GetDlgItem(hwnd, IDOK), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, IDCANCEL), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_RESETFONT), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_MULTITHREADFONT), bEnable) ;
  EnableWindow(GetDlgItem(hwnd, ID_EDITLOGFONT), bEnable) ;
}
