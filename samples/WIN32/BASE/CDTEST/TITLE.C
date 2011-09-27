/************************************************************************

  File: title.c

  Purpose:

     Contains the functions that control CDTEST's GetFileTitle()
     dialog box.


  Functions:

    - DoTitleDialog()    -- Creates CDTEST's GetFileTitle() dialog.

    - TitleProc()        -- The callback function for CDTEST's
                            GetFileTitle() dialog box.

************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include "cdtest.h"
#include "title.h"

extern UINT uMode ;                               //see cdtest.c
extern LONG MyAtol(LPTSTR, BOOL, LPBOOL) ;


#define MAXTITLE 256

TCHAR szTitle[MAXTITLE] ;
TCHAR szPath[MAXTITLE] ;
short nReturn ;
WORD wBufSize ;




/************************************************************************

  Function: DoTitleDialog(HWND)

  Purpose: Creates CDTEST's GetFileTitle() dialog box.

  Returns: Nothing.

  Comments:

************************************************************************/

void DoTitleDialog(HWND hwnd)
{
  DialogBox(hInst, MAKEINTRESOURCE(ID_TITLEDIALOG), hwnd, TitleProc) ;

}






/************************************************************************

  Function: TitleProc(HWND, UINT, UINT, LONG)

  Purpose: Is the callback function for CDTEST's GetFileTitle() dialog.

  Returns: TRUE or FALSE depending on the situation.

  Comments:

************************************************************************/


BOOL APIENTRY TitleProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  TCHAR szNum[30] ;
  BOOL b ;

  switch (msg)
  {
    case WM_INITDIALOG:
      SetDlgItemText(hwnd, ID_BUFFSIZETI, TEXT("256")) ;
      break ;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
          case IDOK:
            GetDlgItemText(hwnd, ID_PATHTI, szPath, MAXTITLE) ;
            GetDlgItemText(hwnd, ID_BUFFSIZETI, szNum, 30) ;

            wBufSize =  (WORD) MyAtol(szNum, uMode==IDM_HEXMODE, &b) ;

            if (wBufSize > 256)
            {
                wBufSize = 256 ;
                SetDlgItemText(hwnd, ID_BUFFSIZETI, TEXT("256")) ;
            }

            nReturn = GetFileTitle(szPath, szTitle, wBufSize) ;
            wsprintf(szTemp, szShortFilter, nReturn) ;
            SetDlgItemText(hwnd, ID_RETURNTI, szTemp) ;
            SetDlgItemText(hwnd, ID_TITLETI, szTitle) ;

            break ;

          case IDCANCEL:
            EndDialog(hwnd, FALSE) ;
            break ;

          case ID_RESETTITLE:
            szPath[0] = 0 ;
            szNum[0] = 0 ;
            szTemp[0] = 0 ;
            szTitle[0] = 0 ;
            SetDlgItemText(hwnd, ID_PATHTI, szPath) ;
            SetDlgItemText(hwnd, ID_RETURNTI, szTemp) ;
            SetDlgItemText(hwnd, ID_TITLETI, szTitle) ;
            SetDlgItemText(hwnd, ID_BUFFSIZETI, TEXT("256")) ;
            break ;


          default: break ;
        }
    }

    default: break ;
  }

  return FALSE ;
}
