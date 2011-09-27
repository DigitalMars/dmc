/*  Symantec C++ Demonstration Program
 *  Copyright (C) 1993 Symantec, Corp.
 *
 */

#include <windows.h>
#include "wclock.h"

//-------------- Function prototypes------------------------------
extern "C" {
  void FAR PASCAL CenterDlg(HWND hDlg);
  BOOL FAR PASCAL AboutDlgProc(HWND	hDlg   ,
			     unsigned	msg    ,
			     WPARAM	wParam ,
			     LPARAM	lParam );
  void FAR PASCAL _export AboutClock(HWND hWnd, HINSTANCE clkHInst);
  int  FAR PASCAL LibMain( HANDLE  hModule,
			WORD    wDataSeg,
			WORD    cbHeapSize,
			LPSTR   lpszCmdLine);
  int  FAR PASCAL _WEP(int bSystemExit);
};
//----------------------------------------------------------------


#define CLKABOUTNAME	"CLKABOUT"

static HANDLE hInstDLL;

void FAR PASCAL CenterDlg(HWND hDlg)
{
    int  width, height;
    RECT rect;
    HDC  hDC;

    GetWindowRect(hDlg, &rect);

    width 	= rect.right  - rect.left;
    height	= rect.bottom - rect.top ;

    hDC		= GetDC(NULL);
    rect.left	= (GetDeviceCaps(hDC, HORZRES) - width ) / 2;
    rect.top	= (GetDeviceCaps(hDC, VERTRES) - height) / 2;
    rect.right	= rect.left + width  - 1;
    rect.bottom	= rect.top  + height - 1;

    ReleaseDC(NULL, hDC);

    MoveWindow(hDlg		      ,
	       rect.left	      ,
	       rect.top		      ,
	       rect.right-rect.left   ,
	       rect.bottom - rect.top ,
	       FALSE		      );
}


BOOL FAR PASCAL AboutDlgProc(HWND	hDlg   ,
			     unsigned	msg    ,
			     WPARAM	wParam ,
			     LPARAM	lParam )
{
    switch (msg) {

    case WM_INITDIALOG :
        CenterDlg(hDlg);
        return TRUE;

    case WM_COMMAND:
        EndDialog(hDlg, TRUE);
        return TRUE;
    }
    return FALSE;
}


void FAR PASCAL _export AboutClock(HWND hWnd, HINSTANCE clkHInst)
{
    DialogBox(clkHInst, CLKABOUTNAME, hWnd,
   	      MakeProcInstance((FARPROC)AboutDlgProc, hInstDLL));
}


int FAR PASCAL LibMain( HANDLE  hModule, 
                        WORD    wDataSeg, 
                        WORD    cbHeapSize, 
                        LPSTR   lpszCmdLine)
{
    hInstDLL = hModule;

    return 1;
}


int FAR PASCAL _WEP(int bSystemExit)
{
    return 1;
}


