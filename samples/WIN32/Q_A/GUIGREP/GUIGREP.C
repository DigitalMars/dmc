
/*************************************************************************/
/*                                                                       */
/*           NT-based multithreaded file browser                         */
/*           Ruediger R. Asche                                           */
/*           Copyright (c) 1992-1995 Microsoft Corporation               */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/*           Description:                                                */
/*           This file manager extension will perform the following:     */
/*           - user clicks "search". This will open the target window    */
/*             which has an edit control embedded in it and send a       */
/*             WM_LAUNCH message to that window.                         */
/*           - The window function then dispatches the "master thread"   */
/*             which will, in turn, dispatch a new thread for every      */
/*             file selected by the user. The master thread waits until  */
/*             all file threads have finished, then send a WM_FREEAGAIN  */
/*             message to the main window which will then display the    */
/*             results and clean up.                                     */
/*                                                                       */
/*************************************************************************/


/*************************************************************************/
/*                                                                       */
/*           Includes                                                    */
/*                                                                       */
/*************************************************************************/


extern "C"
{
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "wfext.h"
}

#include "guigrep.h"
/*************************************************************************/
/*                                                                       */
/*           Global variables                                            */
/*                                                                       */
/*************************************************************************/

memory *MasterBlock;
growmemory *ResultBlock;
HWND hWndGlobal;
int iHitCount=0;
char szGlobalSearchString[MAXLINELEN];
unsigned int iGlobalSStLen;
int iFinishedFiles;
BOOL bCaseInsens=FALSE;
HINSTANCE hOurselves;
WNDPROC lpfnOldEditProc;
char lpszCRLF[]="\r\n";
int ilpszCRLF = strlen("\r\n");
BOOL bObjectExists = FALSE;

/*************************************************************************/
/*                                                                       */
/*           Symbolic equals.                                            */
/*                                                                       */
/*************************************************************************/

#define WNDCLASSNAME "SearchWndClass"

/*************************************************************************/
/*                                                                       */
/*           Forward function declarations                               */
/*                                                                       */
/*************************************************************************/


BOOL APIENTRY About(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam);



long Poll_On_Threads(LONG lParam);


/*************************************************************************/
/*                                                                       */
/* A generic window class that carries an edit child in its entire       */
/* client area wherever it goes.                                                                                 */
/*                                                                       */
/*************************************************************************/


LONG APIENTRY ColorWndProc(
	HWND hWnd,
	UINT message,
	UINT wParam,
	LONG lParam)
{
    static char szBuf[13];
    static char szPathName[MAX_PATH];

    RECT   rcClient;
	static HWND hWndHitBox,hWndFileMan;
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);
    switch (message) {
	case WM_CREATE:
	    AppendMenu(GetSystemMenu(hWnd,FALSE),MF_STRING, IDM_ABOUT, "About...");
	    hWndHitBox = CreateWindow("EDIT","",WS_CHILD | WS_VISIBLE | ES_WANTRETURN | ES_AUTOHSCROLL |
					       DS_LOCALEDIT|ES_MULTILINE | WS_BORDER|WS_VSCROLL|WS_HSCROLL,
						 0,0,1,1,hWnd, NULL, hInst,
						 NULL);
	  if (!hWndHitBox)
	     {MessageBox(hWnd,"Unable to create Child Window!","Browse Extension",MB_OK);
	      return(-1);
	     };

		  CloseHandle((HANDLE)SendMessage(hWndHitBox,EM_GETHANDLE,0,0));
	  ShowWindow(hWndHitBox,SW_SHOWNORMAL);
		  break;
	case WM_SIZE:
	    GetClientRect(hWnd,&rcClient);
	    MoveWindow(hWndHitBox,rcClient.left,rcClient.top,
				   rcClient.right, rcClient.bottom, TRUE);
	    break;
	case WM_LAUNCH:
		hWndFileMan = (HWND)lParam;
			 { static THREADPBLOCK tpbParms;
			   int iPollThreadID;
			   tpbParms.hWndGlobal = hWnd;
			   tpbParms.hWndFileBox = hWndFileMan;
			   SendMessage(hWndHitBox,LB_RESETCONTENT,0,0);
			   ResultBlock = new growmemory(4*4096,4096);
			   bObjectExists=TRUE;
			   HANDLE hNewThread=CreateThread(NULL,0,
			     (LPTHREAD_START_ROUTINE)Poll_On_Threads,(LPVOID)&tpbParms,0,(LPDWORD)&iPollThreadID);
		 };
		   break;
	case WM_ADDITEM:
	  { char szBuf[40];
	    wsprintf(szBuf,"Finished %d File(s)",(int)lParam);
     SetWindowText(hWnd,szBuf);
	  };
		break;
    case WM_FREEAGAIN:
		SendMessage(hWndHitBox,EM_SETHANDLE,(WPARAM)ResultBlock->GetHandle(),0);
		InvalidateRect(hWndHitBox,NULL,FALSE);
		break;

	case WM_SYSCOMMAND:
	  if (wParam == IDM_ABOUT)
		   DialogBox(hOurselves,
			     "AboutBox",
			     hWnd,
			     (DLGPROC)About);
		   else
	    return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_DESTROY:
	 bObjectExists=FALSE;
	 delete(ResultBlock);
	 break;
	default:
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}


/*************************************************************************/
/*                                                                       */
/*           The inevitable about box...                                 */
/*                                                                       */
/*************************************************************************/


BOOL APIENTRY About(
	HWND hDlg,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
    switch (message) {
	case WM_INITDIALOG:
	    return (TRUE);

	case WM_COMMAND:
	    if (LOWORD(wParam) == IDOK
		|| LOWORD(wParam) == IDCANCEL) {
		EndDialog(hDlg, TRUE);
		return (TRUE);
	    }
	    break;
    }
    return (FALSE);
	UNREFERENCED_PARAMETER(lParam);
}

/*************************************************************************/
/*                                                                       */
/*           The DLL Entry and exit dispatcher.                          */
/*                                                                       */
/*************************************************************************/



extern "C"
{
BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
    WNDCLASS  wc;
    hOurselves=hDLL;

    wc.style = NULL;
    wc.lpfnWndProc = (WNDPROC)ColorWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = (HINSTANCE)GetCurrentProcess();
    wc.hIcon = LoadIcon(hDLL, "GrepIcon");
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  NULL;
    wc.lpszClassName = WNDCLASSNAME;

    if (!RegisterClass(&wc))
    {
    MessageBox(GetFocus(),"Unable to create class",
		 "Browse extension",MB_OK);
    return(NULL);
    }
    };
   };
   return( TRUE );
}
}

/*************************************************************************/
/*                                                                       */
/*           Dialog box function for the "Select string" dialog          */
/*                                                                       */
/*************************************************************************/



BOOL APIENTRY StringProc(
	HWND hDlg,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{   HWND hEditControl;
    switch (message)
	    {
	case WM_INITDIALOG:
	    hEditControl = GetDlgItem(hDlg, IDM_STRINGEDIT);
	    SetWindowText(hEditControl,szGlobalSearchString);
		SendMessage(GetDlgItem(hDlg,IDM_CASESENS),BM_SETCHECK,bCaseInsens,0);
	    return (TRUE);

	case WM_COMMAND:
	  switch (LOWORD(wParam))
	   {
		 case IDM_CANCEL:
		   EndDialog(hDlg,FALSE);
		   return(TRUE);
		   break;
	     case IDM_TERMBOX:
		  {int iLoopVar, iLen;
	       bCaseInsens = SendMessage(GetDlgItem(hDlg,IDM_CASESENS),BM_GETCHECK,0,0);
	       hEditControl = GetDlgItem(hDlg,IDM_STRINGEDIT);
	       GetWindowText(hEditControl,szGlobalSearchString,MAXLINELEN);
		   if (bCaseInsens)
		    { iLen = strlen(szGlobalSearchString);
		    for (iLoopVar = 0; iLoopVar < iLen;iLoopVar++)
			 if (IsCharUpper(szGlobalSearchString[iLoopVar]))
			 szGlobalSearchString[iLoopVar] = (char)_tolower((int)szGlobalSearchString[iLoopVar]);
			};
		   EndDialog(hDlg, TRUE);
		  }
	       break;

	    }
	    break;
    }
    return (FALSE);
	UNREFERENCED_PARAMETER(lParam);
}


long TraverseFileCaseInsens(HANDLE hFile, LPSTR lpFilNam, LPSTR lpFileView)
{
  unsigned int iGlobalSearchString, iCurrentBuffPt, iPrefixLength;
  BOOL bMatch;
  LPSTR lpLineBuff;
  unsigned int iLineCt = 0;
  DWORD dwFileSize, dwCurrentFilePt;

  dwFileSize = GetFileSize(hFile,NULL);
  lpLineBuff = MasterBlock->Alloc(MAXLINELEN);
  if (!lpLineBuff)
   {
   MessageBox(GetFocus(),lpFilNam,"Out of memory processing:",MB_OK);
   return (-1);
   };
/**************************************************************************/
/*    Initialize variables:                                               */
/*    lpLineBuff contains the buffer in which we store the whole line.    */
/*    If the line doesn't match, we "recycle" the buffer by using it      */
/*    for the next line; if it does, we send it off.                      */
/*    iCurrentBuff is the length of the line we currently work on,        */
/*    and iGlobalSearchString describes how far in the global search      */
/*    string we have progreesed so far matching.                          */
/**************************************************************************/

  strcpy(lpLineBuff,lpFilNam);
  strcat(lpLineBuff," <    >: ");
  iPrefixLength = strlen(lpLineBuff);
  iCurrentBuffPt = iPrefixLength-1;
  iGlobalSearchString = 0;
  bMatch = FALSE;
/**************************************************************************/
/*    Now go through the mapping character by character                   */
/**************************************************************************/

  for (dwCurrentFilePt=0;
       dwCurrentFilePt<dwFileSize;
       dwCurrentFilePt++)

    { if (iCurrentBuffPt >= MAXLINELEN)
		{MessageBox(GetFocus(),"File not readable!",lpFilNam,MB_OK);
		 return(-1);
		};

      iCurrentBuffPt++;
      lpLineBuff[iCurrentBuffPt]= lpFileView[dwCurrentFilePt];
     if (lpLineBuff[iCurrentBuffPt] == '\n')
	{

	 iLineCt++;
       if (bMatch)
	       {
/**************************************************************************/
/*    A match has occurred!                                               */
/*    At this point, lpLineBuff points to the string that contains the    */
/*    matching line; iCurrentBuffPt its length.                           */
/**************************************************************************/

		OemToChar(lpLineBuff,lpLineBuff);
		wsprintf(&lpLineBuff[strlen(lpFilNam)+2],"%4d",iLineCt);
		lpLineBuff[strlen(lpFilNam)+6] = '>';
		iHitCount++;
    		ResultBlock->add(lpLineBuff,iCurrentBuffPt-1);
		strcpy(lpLineBuff, lpFilNam);
		wsprintf(&lpLineBuff[strlen(lpFilNam)]," <    >: ");

	       };
/**************************************************************************/
/*    The FOR-logic will increment the pointer,                           */
/*    so we reduce it by one because we use it in the NEXT iteration.     */
/**************************************************************************/

	iCurrentBuffPt = iPrefixLength -1;
	iGlobalSearchString = 0;
	bMatch = FALSE;
	}
     else

	{ if (iGlobalSearchString < iGlobalSStLen)
	    if ( /* This is the only difference to case-sensitive... */
			(IsCharUpper(lpLineBuff[iCurrentBuffPt]) ? (char)_tolower(lpLineBuff[iCurrentBuffPt])
			  : lpLineBuff[iCurrentBuffPt]) == szGlobalSearchString[iGlobalSearchString])
	       { iGlobalSearchString++;
		 bMatch = TRUE;
	       }
	    else
	       { bMatch = FALSE;
		 iGlobalSearchString = 0;

	       };

	 };

	 };
return(TRUE);
}

/**************************************************************************/
/*    The case sensitive traversal function is basically the same         */
/*    as the case-insensitive one, just one line difference.              */
/*    The reason why we separate this is that at runtime it is much more  */
/*    efficient not to have to test for case-insensitivity on each and    */
/*    every character we test...                                          */
/**************************************************************************/


long TraverseFileCaseSens(HANDLE hFile, LPSTR lpFilNam, LPSTR lpFileView)
{
  unsigned int iGlobalSearchString, iCurrentBuffPt, iPrefixLength;
  BOOL bMatch;
  LPSTR lpLineBuff;
  unsigned int iLineCt = 0;
  DWORD dwFileSize, dwCurrentFilePt;

  dwFileSize = GetFileSize(hFile,NULL);
  lpLineBuff = MasterBlock->Alloc(MAXLINELEN);
  if (!lpLineBuff)
   {
   MessageBox(GetFocus(),lpFilNam,"Out of memory processing:",MB_OK);
   return (-1);
   };

  strcpy(lpLineBuff,lpFilNam);
  strcat(lpLineBuff," <    >: ");
  iPrefixLength = strlen(lpLineBuff);
  iCurrentBuffPt = iPrefixLength-1;
  iGlobalSearchString = 0;
  bMatch = FALSE;
  for (dwCurrentFilePt=0;
       dwCurrentFilePt<dwFileSize;
       dwCurrentFilePt++)

    { if (iCurrentBuffPt >= MAXLINELEN)
		{MessageBox(GetFocus(),"File not readable!",lpFilNam,MB_OK);
		 return(-1);
		};

      iCurrentBuffPt++;
      lpLineBuff[iCurrentBuffPt]= lpFileView[dwCurrentFilePt];
     if (lpLineBuff[iCurrentBuffPt] == '\n')
	{

	 iLineCt++;
       if (bMatch)
	       {
		OemToChar(lpLineBuff,lpLineBuff);
		wsprintf(&lpLineBuff[strlen(lpFilNam)+2],"%4d",iLineCt);
		lpLineBuff[strlen(lpFilNam)+6] = '>';
		iHitCount++;
		ResultBlock->add(lpLineBuff,iCurrentBuffPt-1);
		strcpy(lpLineBuff, lpFilNam);
		wsprintf(&lpLineBuff[strlen(lpFilNam)]," <    >: ");

	       };
	iCurrentBuffPt = iPrefixLength -1;
	iGlobalSearchString = 0;
	bMatch = FALSE;
	}
     else

	{ if (iGlobalSearchString < iGlobalSStLen)
	    if (lpLineBuff[iCurrentBuffPt] == szGlobalSearchString[iGlobalSearchString])
	       { iGlobalSearchString++;
		 bMatch = TRUE;
	       }
	    else
	       { bMatch = FALSE;
		 iGlobalSearchString = 0;

	       };

	 };

	 };

return(TRUE);
}




long ProcessFileCommonCode (LPTHREADPBLOCK tpArgs)
{ FMS_GETFILESEL gfsSelection;
  LPSTR lpFilNam;
  HANDLE hFile, hMappedFile;
  LPSTR lpFileView;
  unsigned int iLineCt = 0;
/**************************************************************************/
/*    Set up file mapping etc.                                            */
/**************************************************************************/
  SendMessage(tpArgs->hWndFileBox,FM_GETFILESEL,tpArgs->iLoop,(LPARAM)&gfsSelection);
  lpFilNam = MasterBlock->Alloc(1+strlen(gfsSelection.szName));
		strcpy(lpFilNam,gfsSelection.szName);

  if ((hFile = CreateFile(lpFilNam, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL))
      == INVALID_HANDLE_VALUE)
       return(FALSE);
  if (!(hMappedFile = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,0)))
       {MessageBox(GetFocus(),"Could not open file mapping!",lpFilNam,MB_OK);
	return(FALSE);
       };
  if (!(lpFileView = (LPSTR)MapViewOfFile(hMappedFile,FILE_MAP_READ,0,0,0)))
       {MessageBox(GetFocus(),"Could not map file view",lpFilNam,MB_OK);
	return(FALSE);
       };
/**************************************************************************/
/*    Now traverse file                                                   */
/**************************************************************************/
  if (bCaseInsens)
    TraverseFileCaseInsens(hFile,lpFilNam,lpFileView);
  else
    TraverseFileCaseSens(hFile,lpFilNam,lpFileView);

/**************************************************************************/
/*    Clean up.                                                   */
/**************************************************************************/

  if (!UnmapViewOfFile(lpFileView))
	 MessageBox(GetFocus(),"UnmapViewOfFile Failure",lpFilNam,MB_OK);
  if (!CloseHandle(hMappedFile))
   MessageBox(GetFocus(),"CloseHandle Failure...",lpFilNam,MB_OK);
  if (!CloseHandle(hFile))
   MessageBox(GetFocus(),"CloseHandle Failure...",lpFilNam,MB_OK);

iFinishedFiles++;
SendMessage(hWndGlobal,WM_ADDITEM,(WPARAM)iHitCount,iFinishedFiles);
  return(1);

}



/*************************************************************************/
/*                                                                       */
/*          This is the "master thread." Its only purpose is to          */
/*          dispatch one thread for each file selected, then wait for    */
/*          completition of all threads, and return to the main window.  */
/*          This has been made a separate thread due to possible         */
/*          deadlock conditions that might occurr if it was in the       */
/*          main window function.                                        */
/*                                                                       */
/*          Since there is no system-provided way to limit the number    */
/*          of threads AND it doesn't make sense to have zillions        */
/*          of threads in the system, there is a mechanism that          */
/*          limits the number of threads to a certain limit (currently   */
/*          30). Would make sense to devise some kind of a fancy         */
/*          algorithm to compute the number of threads dynamically.      */
/*                                                                       */
/*************************************************************************/



long Poll_On_Threads(LONG lParam)
{ int iLoop, iEndLoop,iThread,iArrayIndex;
  char tempFilBuf[80];
  static HANDLE aThreads[MAX_CONCURRENT_THREADS];
  HWND hWndFileBox;
  LPTHREADPBLOCK lpTB = (LPTHREADPBLOCK)lParam;
  iHitCount = 0;
  iFinishedFiles = 0;
  iGlobalSStLen = strlen(szGlobalSearchString);
  hWndFileBox = lpTB->hWndFileBox;
  hWndGlobal =  lpTB->hWndGlobal;
  MasterBlock = new memory(150000);
  iEndLoop = SendMessage(hWndFileBox,FM_GETSELCOUNT,0,0);
  if (iEndLoop == 0)
   { MessageBox(hWndFileBox,"No files selected","Browser extension",MB_OK);
     return(0);
   };
  for (iLoop = 0; iLoop < iEndLoop; iLoop++)
	{HANDLE hNewThread;

		if (iLoop < MAX_CONCURRENT_THREADS)
				     iArrayIndex = iLoop;
		else
			{
			 iArrayIndex = WaitForMultipleObjects(
				   MAX_CONCURRENT_THREADS,aThreads,FALSE,INFINITE);
			 if (iArrayIndex == -1)
				{MessageBeep(0);
					  return(-1);
				 };

			 CloseHandle(aThreads[iArrayIndex]);
	    };
		lpTB = (LPTHREADPBLOCK)MasterBlock->Alloc(sizeof(THREADPBLOCK));
		lpTB->hWndFileBox=hWndFileBox;
	lpTB->iLoop = iLoop;
	    hNewThread = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE) ProcessFileCommonCode,
		     (LPVOID)lpTB,0,(LPDWORD)&iThread);
	if (!hNewThread)
	   MessageBox(GetFocus(),"Failed","CreateThread",MB_OK);
		aThreads[iArrayIndex] = hNewThread;


	 };
/* We are now processing the last few threads. Clean up. */
	iEndLoop = min(iEndLoop,MAX_CONCURRENT_THREADS);
	WaitForMultipleObjects(iEndLoop, aThreads,TRUE,INFINITE);
	for (iLoop = 0; iLoop < iEndLoop ; iLoop++)
	  if (!CloseHandle(aThreads[iLoop]))
	    MessageBox(GetFocus(),"Can't delete thread!","",MB_OK);
	wsprintf(tempFilBuf,"Total Hits: %d",iHitCount);
	SetWindowText(hWndGlobal,tempFilBuf);
	delete (MasterBlock);
	SendMessage(hWndGlobal,WM_FREEAGAIN,0,0);
    return(0);
}

/********************************************************************/
/* Following is the relevant stuff for the file manager extensions. */
/********************************************************************/

LONG WINAPI FMExtensionProc(HWND hWndFileMan, WORD wMsg, LONG lParam)
{
  static HMENU hSubMenu;
  static UINT wDelta;
  static HWND hWnd;
 switch (wMsg)
 {

/* this message is sent to the extension proc to update its toolbar to
   add custom buttons as accelarators for the extension menu. */

  case FMEVENT_TOOLBARLOAD:
   {
     static EXT_BUTTON ExtButtons[1] =
     { { IDM_STARTGREP-100, 0 , 0} };   /* This must correspond to the wMsg parameter passed to the extension proc */

#define lpTB ((LPFMS_TOOLBARLOAD) lParam)

  lpTB->dwSize = sizeof(FMS_TOOLBARLOAD);
  lpTB->lpButtons = (LPEXT_BUTTON) &ExtButtons;
  lpTB->cButtons = 1;
  lpTB->cBitmaps = 1;
  lpTB->idBitmap = IDM_STARTGREP;       /* Must correspond to the bitmap identifier */
  return(TRUE);
   };

/* this message is sent to the extension proc to associate a help string with
   a menu item. */

  case FMEVENT_HELPSTRING:
   strcpy(((LPFMS_HELPSTRING)lParam)->szHelp,"Search for a string in multiple files");
   return (TRUE);

  case FMEVENT_LOAD:
   if (!hOurselves)
	  return(NULL);

	   hSubMenu = LoadMenu(hOurselves,"FileManMenu");
	   wDelta = ((LPFMS_LOAD)lParam)->wMenuDelta;
	   ((LPFMS_LOAD)lParam)->dwSize= sizeof(FMS_LOAD);
	   strcpy(((LPFMS_LOAD)lParam)->szMenuName,"S&earch");
	   ((LPFMS_LOAD)lParam)->hMenu = hSubMenu;


	   return (LONG) hSubMenu;
  default:
       switch (wMsg+100)
       {
	      case IDM_DEFGREP:
		   DialogBox(hOurselves,
			     "StringDialog",
			     hWndFileMan,
			     (DLGPROC)StringProc);
		   break;

		case IDM_STARTGREP:
		if (strlen(szGlobalSearchString) == 0)
		   if (!DialogBox(hOurselves,
			     "StringDialog",
			     hWndFileMan,
			     (DLGPROC)StringProc))
			  break;
	if (IsWindow(hWndGlobal))
	 {MessageBox(GetFocus(),"No more than one window supported","Close hit window",MB_OK);
	  break;
	 };
    hWnd = CreateWindow(
	WNDCLASSNAME,
	"Windows NT File Search Extension",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	(HINSTANCE)GetCurrentProcess(),
	NULL
    );
	SendMessage(hWnd,WM_LAUNCH,0,(LONG)hWndFileMan);
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
    break;
    default:
    break;
    };
  return(0);
 };
}


