/************************************************************************

  File: cdtest.c

  Purpose:  Contains the functions which handle the main window's functionality --
            creating the main window, handling menu commands, exiting the program.

  Functions:

           WinMain()           - Program's entry point

           InitApplication()   - Registers class and some user defined messages

           InitInstance()      - Creates main window

           MainWndProc()       - Window procedure for main window

           HandleTheCommand()  - Processes all WM_COMMAND messages

           InitGlobals()       - Initializes all global variables that need to
                                 be initialized at startup

           MyAtol()            - Converts an ASCII string in either hexi-
                                 decimal or decimal notation to a LONG.

           AboutProc()         - Callback function for CDTEST's about box.

************************************************************************/



#include <windows.h>
#include <commdlg.h>
#include <winnls.h>
#include "cdtest.h"
#include "colors.h"
#include "save.h"
#include "print.h"
#include "title.h"
#include "replace.h"
#include "open.h"
#include "font.h"
#include "find.h"





/* Some defines, global variables, and function declarations */

#define szClass TEXT("cdtestclass")
#define szIcon  TEXT("theicon")
#define szMenu  TEXT("themenu")

#ifdef UNICODE
  #define szTitle TEXT("Common Dialog Test App - Unicode Version")
#else
  #define szTitle TEXT("Common Dialog Test App - ANSI Version")
#endif


void InitGlobals(void) ;

UINT uMode = IDM_HEXMODE ;







/************************************************************************


  Function: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

  Purpose:

    - Contains standard windows entry point

    - Initializes the application

    - Contains the main message loop

  Returns: Final msg.wParam

  Comments:

************************************************************************/


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

  MSG msg;


  UNREFERENCED_PARAMETER( lpCmdLine );


  if (!hPrevInstance)
    if (!InitApplication(hInstance))
        return (FALSE);

  if (!InitInstance(hInstance, nCmdShow))
    return (FALSE);

  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }


  return (msg.wParam);
}







/************************************************************************


  Function: InitApplication(HANDLE)


  Purpose:

    - Fills in the WNDCLASS structure

    - Registers messages needed to communicate with the common dialogs

    - Registers the window class

  Returns: The return value of RegisterClass().  If this fails then the
           whole program fails.

  Comments:


************************************************************************/


BOOL InitApplication(HANDLE hInstance)     
{
  WNDCLASS  wc;

  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, szIcon) ;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = szMenu ;
  wc.lpszClassName = szClass ;

  
  InitGlobals() ;

  
  /* Register any messages that the common dialogs will need to 
     communicate with this app. */

  nFindMsg       = RegisterWindowMessage((LPTSTR) FINDMSGSTRING) ;
  nOpenShareVMsg = RegisterWindowMessage((LPTSTR) SHAREVISTRING) ;
  nHelpMessage   = RegisterWindowMessage((LPTSTR) HELPMSGSTRING) ;


  return (RegisterClass(&wc));
}








/************************************************************************


 Function: InitInstance(HANDLE, int)


 Purpose:

   - Creates the main window

   - Shows the main window

 Returns: FALSE if it cannot create the window, TRUE if it can.


 Comments:


************************************************************************/


BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND hWnd;
    DWORD dwFlags = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU ;

    
    /* save the instance handle in a global variable */

    hInst = hInstance;


    /* Create the main window */

    hWnd = CreateWindow(szClass, szTitle, dwFlags, 
                        10, 10, 400, 200, NULL, NULL, hInstance, NULL) ;

    if (!hWnd)
        return (FALSE);

    ShowWindow(hWnd, SW_SHOWNORMAL) ;
    UpdateWindow(hWnd);

    return (TRUE);
}








/************************************************************************


  Function: MainWndProc(HWND, UINT, UINT, LONG)

  Purpose:

    - Is the callback function that handles all messages for the main window

  Returns:

    - Returns DefWindowProc() for any message it does not explicitly
      respond to.

  Comments:


************************************************************************/


LONG APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
    {
      case WM_CREATE:
        CheckMenuItem(GetMenu(hWnd), IDM_HEXMODE, MF_CHECKED) ;
        break ;

      case WM_COMMAND:
        HandleTheCommand(hWnd, wParam, lParam) ;
        break ;

      case WM_DESTROY:
          PostQuitMessage(0);
          break;

      default:
          return (DefWindowProc(hWnd, message, wParam, lParam));
          break ;
    }
    return (0L);
}






/************************************************************************


  Function: HandleTheCommand(HWND, UINT, UINT)

  Purpose:

    - Handles all WM_COMMAND messages passed to the MainWndProc().  The
      menu or control ID value for Win32 WM_COMMAND messages is contained
      in the low word of the wParam parameter.

  Returns: Nothing.

  Comments:


************************************************************************/


void HandleTheCommand(HWND hWnd, UINT wParam, LONG lParam)
{

  switch (LOWORD(wParam))
  {
    case IDM_COLOR:                //For any of the dialog creation
      DoColorsDialog(hWnd) ;       //commands, call the appropriate
      break ;                      //function.  The function will
                                   //create the dialog...
    case IDM_FONT:
      DoFontDialog(hWnd) ;
      break ;

    case IDM_TITLE:
      DoTitleDialog(hWnd) ;
      break ;

    case IDM_FIND:
      DoFindDialog(hWnd) ;
      break ;

    case IDM_OPEN:
      DoOpenDialog(hWnd) ;
      break ;

    case IDM_SAVE:
      DoSaveDialog(hWnd) ;
      break ;

    case IDM_PRINT:
      DoPrintDialog(hWnd) ;
      break ;

    case IDM_REPLACE:
      DoReplaceDialog(hWnd) ;
      break ;

    case IDM_EXIT:
      PostQuitMessage(0) ;
      break ;

    case IDM_HEXMODE:
    case IDM_DECIMALMODE:

      /* We need to maintain a global variable that will indicate what
         kind of number processing we have to do.  First, check the
         menu item corresponding to the new mode... */

      CheckMenuItem(GetMenu(hWnd), uMode, MF_UNCHECKED) ;
      uMode = wParam ;
      CheckMenuItem(GetMenu(hWnd), uMode, MF_CHECKED) ;


      /* and then create an appropriate filter for wsprintf() type
         functions */

      if (uMode == IDM_HEXMODE)
      {
        lstrcpy(szShortFilter, TEXT("%x")) ;
        lstrcpy(szLongFilter, TEXT("%lx")) ;
      }
      if (uMode == IDM_DECIMALMODE)
      {
        lstrcpy(szShortFilter, TEXT("%d")) ;
        lstrcpy(szLongFilter, TEXT("%ld")) ;
      }

      break ;


    case IDM_ABOUT:
       DialogBox(hInst, TEXT("about"), hWnd, AboutProc) ;
       break ;


    default: break ;
  }

  return ;
}








/************************************************************************


  Function: InitGlobals(void)

  Purpose:

    - Any global variables can be initialized here since this function is
      called on app startup.

  Returns: Nothing

  Comments:


************************************************************************/


void InitGlobals(void)
{

  /* not really too much to do here.  Create a hex wsprintf() filter since
     the app starts off in Hex mode. */

  lstrcpy(szShortFilter, TEXT("%x")) ;
  lstrcpy(szLongFilter, TEXT("%lx")) ;

}









/************************************************************************


  Function: MyAtol(LPTSTR, BOOL, LPBOOL)

  Purpose:

    - This function will convert an ascii string to a LONG.

  Returns:

    - If it receives an invalid ascii character, it will return 0 and
      set the LPBOOL variable to false...

  Comments:

      Since the function may need to deal with either a hex number or a decimal
      number, it should use a variable as a multiplier.


************************************************************************/


LONG MyAtol(LPTSTR szString, BOOL bHex, LPBOOL bSuccess)
{
  LPTSTR p ;
  LONG l ;
  LONG lMultiplier ;
  BOOL bDigit ;

  if (bHex)
    lMultiplier = 16 ;
  else
    lMultiplier = 10 ;

  p = szString ;
  l = 0 ;

  while (*p)      //while chars
  {
     bDigit = FALSE ;  //set to false for each char that we look at

     if (*p >= (TCHAR) '0' && *p <= (TCHAR) '9')  //is it an ascii char ?
     {
       bDigit = TRUE ;
       l+=(*p - (TCHAR) '0') ;
     }

     if (bHex)
     {
       if (*p >= (TCHAR) 'A' && *p <= (TCHAR) 'F')  //or hex?
       {
         l+=(*p - (TCHAR) 'A' + 10) ;
         bDigit = TRUE ;
       }

       if (*p >= (TCHAR) 'a' && *p <= (TCHAR) 'f') 
       {
         l+=(*p - (TCHAR) 'a' + 10) ;
         bDigit = TRUE ;
       }

     }

     if (bDigit == FALSE)
     {
       *bSuccess = FALSE ;
       return 0 ;
     }

     p++ ;               //get next char

     if (*p)             //if there is going to be at least one more char
       l*=lMultiplier ;  //then multiply what we have by the multiplier...
  }

  *bSuccess = TRUE ;

  return l ;             //success! return the value.
}









/************************************************************************


  Function: AboutProc(HWND, UINT, UINT, LONG)

  Purpose:  Callback function for the about dialog box.


  Returns:  BOOL - FALSE ...


  Comments:

************************************************************************/


BOOL APIENTRY AboutProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{

  if (
       (msg == WM_COMMAND) &&
       (LOWORD(wParam) == IDCANCEL)
     )

     EndDialog(hwnd, FALSE) ;

  else
    return FALSE ;
}
