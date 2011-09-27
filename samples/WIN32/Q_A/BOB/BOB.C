/****************************************************************************\
**                                                                         **
**               Microsoft Developer Support                               **
**               Copyright (c) 1992-1995 Microsoft Corporation            **
**                                                                         **
**   MODULE:  BOB                                                          **
**                                                                         **
**                                                                         **
**   PURPOSE: Demonstrates the steps needed to programmatically            **
**            log off/reboot the machine. Note these features should       **
**            only be used after you've notified the user what you         **
**            are about to do. Windows itself will not provide any         **
**            other warnings to the user.                                  **
**                                                                         **
**                                                                         **
**                                                                         **
**                                                                         **
**   COMMENTS: Reads a file asynchronously writes to another synchronously **
**                                                                         **
**                                                                         **
\***************************************************************************/



/*  To enable NT's Reboot feature enable the below
 *  manifest constants. By default the program will
 *  shut down the machine.
 */

// #define REBOOT

#define WIN32S  0x80000000l

#define STRICT

#include <stddef.h>
#include <stdlib.h>


#define  OEMRESOURCE
#include "windows.h"            /* required for all Windows applications */

#include "BOB.H"                /* specific to this program */

HINSTANCE ghInst;               /* current instance */
HWND      ghWnd;
WNDPROC   OldButtonProc;

/****************************************************************************

    FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

    PURPOSE: calls initialization function, processes message loop

    COMMENTS:

        Windows recognizes this function by name as the initial entry point
        for the program.  This function calls the application initialization
        routine, if no other instance of the program is running, and always
        calls the instance initialization routine.  It then executes a message
        retrieval and dispatch loop that is the top-level control structure
        for the remainder of execution.  The loop is terminated when a WM_QUIT
        message is received, at which time this function exits the application
        instance by returning the value passed by PostQuitMessage().

        If this function must abort before entering the message loop, it
        returns the conventional value NULL.

****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, /* current instance  */
           HINSTANCE hPrevInstance,     /* previous instance */
           LPSTR     lpCmdLine,         /* command line      */
           int       nCmdShow)          /* show-window type (open/icon) */
{
    MSG msg;                            /* message              */

    if (!hPrevInstance)                 /* Other instances of app running? */
    if (!InitApplication(hInstance))    /* Initialize shared things */
        return (FALSE);                 /* Exits if unable to initialize */

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,    /* message structure                      */
        NULL,                  /* handle of window receiving the message */
        0,                     /* lowest message to examine              */
        0))                    /* highest message to examine             */
    {
        TranslateMessage(&msg);    /* Translates virtual key codes       */
        DispatchMessage(&msg);     /* Dispatches message to window       */

    }
    return (msg.wParam);       /* Returns the value from PostQuitMessage */
UNREFERENCED_PARAMETER(lpCmdLine);
}


/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

    COMMENTS:

        This function is called at initialization time only if no other
        instances of the application are running.  This function performs
        initialization tasks that can be done once for any number of running
        instances.

        In this case, we initialize a window class by filling out a data
        structure of type WNDCLASS and calling the Windows RegisterClass()
        function.  Since all instances of this application use the same window
        class, we only need to do this when the first instance is initialized.


****************************************************************************/

BOOL InitApplication(HANDLE hInstance)    /* current instance     */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = 0;                       /* Class style(s).            */
    wc.lpfnWndProc = (WNDPROC)MainWndProc;   /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "BOBMenu";       /* Name of menu resource in .RC file. */
    wc.lpszClassName = "BOBWClass";     /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */


    if ( !RegisterClass(&wc) )
        return( FALSE );


}


/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

    COMMENTS:

        This function is called at initialization time for every instance of
        this application.  This function performs initialization tasks that
        cannot be shared by multiple instances.

        In this case, we save the instance handle in a static variable and
        create and display the main program window.

****************************************************************************/

BOOL InitInstance(HANDLE hInstance, /* Current instance identifier.       */
          int nCmdShow )            /* Param for first ShowWindow() call. */
{
    HWND      hWnd;                 /* Main window handle.                */
    HWND      hWndDeskTop;
    RECT      rcDeskTop;


    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    ghInst = hInstance;

    /* Create a main window for this application instance.  */ 

    hWndDeskTop = GetDesktopWindow();
    GetWindowRect( hWndDeskTop, &rcDeskTop );

    hWnd = CreateWindow(
        "BOBWClass",                    /* See RegisterClass() call.          */
        "BOB",                          /* Text for window title bar.         */
        WS_OVERLAPPED,                  /* Window style.                      */
        rcDeskTop.left+50,              /* horizontal position.               */
        rcDeskTop.bottom-200,           /* vertical position.                 */
        150,                            /* width.                             */
        100,                            /* height.                            */
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer to Createion Parameters    */
    );

    /* If window could not be created, return "failure" */

    if (!hWnd)
        return (FALSE);

    ShowWindow(hWnd, nCmdShow);  /* Show the window                */
    UpdateWindow(hWnd);          /* Sends WM_PAINT message         */
    ghWnd = hWnd;

}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)

    PURPOSE:  Processes messages

    MESSAGES:

    WM_COMMAND    - application menu (About dialog box)
    WM_DESTROY    - destroy window

    COMMENTS:

    To process the IDM_ABOUT message, call MakeProcInstance() to get the
    current instance address of the About() function.  Then call Dialog
    box which will create the box according to the information in your
    BOB.RC file and turn control over to the About() function.  When
    it returns, free the intance address.

****************************************************************************/

LRESULT CALLBACK MainWndProc( HWND hWnd,      /* window handle           */
                              UINT message,   /* type of message         */
                              WPARAM wParam,  /* additional information  */
                              LPARAM lParam ) /* additional information  */
{
    DLGPROC lpProcAbout;
    CHAR buffer[80];

    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    static CONTROLTEXT CtlText;
    static TCHAR szTitle[10];
    static RECT  FocusRect, SelectRect;

    switch (message) {

        case WM_CREATE: {

            HWND hWndButton;
            CREATESTRUCT createStruct;
            RECT rcClient;

            int   cbTitle;
            SIZE  size;
            HDC    hDC;
            int    nSpaceWidth, nSpaceHeight;

            createStruct = *(LPCREATESTRUCT)lParam;
            GetClientRect( hWnd,&rcClient );

            hWndButton = CreateWindow(
                "BUTTON",                  /* See RegisterClass() call.       */
                TEXT("B. O. B."),          /* Text for window title bar.      */
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
                rcClient.left,
                rcClient.top,
                rcClient.right,
                rcClient.bottom,
                hWnd,                      /* Parent                          */
                (HMENU)BOB,                /* Child ID                        */
                createStruct.hInstance,    /* This instance owns this window. */
                NULL                       /* Pointer to Createion Parameters */
            );

            wsprintf( buffer, "hWndButton is %#x and hWnd is %#x\r\n",
                      hWndButton, hWnd );
            OutputDebugString ( buffer );

            if ( !hWndButton ) {
                MessageBox( hWnd, "Error creating Window", NULL, MB_OK );
                SendMessage( hWnd, WM_DESTROY, 0, 0 );
            }

            SetFocus( hWndButton );

            OldButtonProc = SubclassWindow( hWndButton, ButtonProc );

            // define size of dashed rectangles around text in both bitmaps
            hDC = GetDC( hWndButton );
            cbTitle = SendMessage( hWndButton,
                                   WM_GETTEXT,
                                   (WPARAM) sizeof(szTitle),
                                   (LPARAM)(LPTSTR)szTitle );
            GetTextExtentPoint( hDC,
                                szTitle,
                                cbTitle,
                                &size );
            ReleaseDC( hWndButton, hDC);

            CtlText.ptPosition.x = (rcClient.right - size.cx) >> 1;
            CtlText.ptPosition.y = (rcClient.bottom - size.cy) >> 1;
            CtlText.szTitle = szTitle;
            CtlText.cbTitle = cbTitle;

            // load bitmap

            CtlText.hDefaultButton = LoadBitmap(
                createStruct.hInstance,    /* This instance owns this window. */
                MAKEINTRESOURCE( DEFAULTBUTTON )
            );

            if ( !CtlText.hDefaultButton ) {
                wsprintf( buffer, "Error loading BUTTONBITMAP %d",
                          GetLastError() );
                MessageBox( hWnd, "Error creating Window", NULL, MB_OK );
                SendMessage( hWnd, WM_DESTROY, 0, 0 );
            }

            CtlText.hSelectButton = LoadBitmap(
                createStruct.hInstance,    /* This instance owns this window. */
                MAKEINTRESOURCE(SELECTEDBUTTON)    /* pushed state */
            );

            if ( !CtlText.hSelectButton ) {
                wsprintf( buffer, "Error loading BUTTONBITMAP %d",
                          GetLastError() );
                MessageBox( hWnd, "Error creating Window", NULL, MB_OK );
                SendMessage( hWnd, WM_DESTROY, 0, 0 );
            }
            
            nSpaceWidth   = GetSystemMetrics(SM_CXBORDER) << 1;
            nSpaceHeight  = GetSystemMetrics(SM_CYBORDER) << 1;

            FocusRect.top    = CtlText.ptPosition.y - nSpaceHeight;
            FocusRect.bottom = FocusRect.top + size.cy + (nSpaceHeight << 1);
            FocusRect.left   = CtlText.ptPosition.x - nSpaceWidth;
            FocusRect.right  = FocusRect.left + size.cx + (nSpaceWidth << 1);

            SelectRect.top    = 2 + FocusRect.top;
            SelectRect.bottom = 2 + FocusRect.bottom;
            SelectRect.left   = 2 + FocusRect.left;
            SelectRect.right  = 2 + FocusRect.right;

            return ( TRUE );
        }

    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT  lpdisCtrl;

        if ( (int)wParam == BOB ) {
            lpdisCtrl = (LPDRAWITEMSTRUCT)lParam;
            switch (lpdisCtrl->itemAction)
            {
                // handle normal drawing of button, but check if its
                // selected or focus

                case ODA_DRAWENTIRE:
                    OutputDebugString(
                        "WM_DRAWITEM: Action is ODA_DRAWENTIRE\r\n");
                        
                    // this handles both button down and button up
                    HandleSelectedState(lpdisCtrl, &CtlText);

                    // this handles focus rectangle around Button text
                    HandleFocusState(lpdisCtrl, &FocusRect, &SelectRect);
                    return TRUE;

                    // handle drawing selection if needed
                    //   selected is button down, not selected is button up
                    //   focus will occur when button is pressed.

                    case ODA_SELECT:
                        OutputDebugString(
                            "WM_DRAWITEM: Action is ODA_SELECT\r\n");
                        HandleSelectedState(lpdisCtrl, &CtlText);
                        HandleFocusState(lpdisCtrl, &FocusRect, &SelectRect);
                        return TRUE;

                    // handle focus drawing if needed
                    case ODA_FOCUS:
                        OutputDebugString(
                            "WM_DRAWITEM: Action is ODA_FOCUS\r\n");
                        HandleFocusState(lpdisCtrl, &FocusRect, &SelectRect);
                        return TRUE;
                }
            }
            break;
        }

        case WM_COMMAND:       /* message: command from application menu */

            if ( HIWORD(wParam) == BN_CLICKED &&
                 LOWORD(wParam) == BOB ) {
                /*
                 * Below are the steps needed to reboot the machine.
                 */
                lpProcAbout = (DLGPROC)MakeProcInstance((FARPROC)About, ghInst );
#if defined(REBOOT)
                DialogBox (ghInst, TEXT("AboutBox"), hWnd, lpProcAbout);
#else
                DialogBox (ghInst, TEXT("AboutBox"), hWnd, lpProcAbout);
#endif

                /* Get a token for this process. */

                if ( !(GetVersion() & WIN32S) )
                {
                    OutputDebugString("Setting token");
                    // Running on NT so need to change privileges

                    if (!OpenProcessToken(GetCurrentProcess(),
                                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, 
                                          &hToken)) 
                    {
                        wsprintf ( buffer, "OpenProcessToken Error #%d", 
                                   GetLastError ());
                        MessageBox(hWnd, buffer, NULL, MB_OK);
                    }
                        
                    /* Get the LUID for shutdown privilege */

                    LookupPrivilegeValue(NULL, TEXT("SeShutdownPrivilege"),
                                         &tkp.Privileges[0].Luid);

                    tkp.PrivilegeCount = 1;  /* one privilege to set    */
                    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

                    /* Get shutdown privilege for this process. */
                    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                                               (PTOKEN_PRIVILEGES)NULL, 0)) {
                        wsprintf ( buffer, "AdjustTokenPrivileges Error #%d", 
                                   GetLastError ());
                        MessageBox(hWnd, buffer, NULL, MB_OK);
                    }
                }

#if defined(REBOOT)
                /* Shut down the system, and reboot the system. */

                if ( !ExitWindowsEx( EWX_REBOOT, 0 )) {
                    wsprintf ( buffer, "Error ExitWindows Error #%d", 
                               GetLastError ());
                    MessageBox(hWnd, buffer, NULL, MB_OK);
                }
#else
                /* Shut down the system, and force all applications closed. */

                if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) {
                    wsprintf ( buffer, "Error ExitWindows Error #%d", 
                               GetLastError ());
                    MessageBox(hWnd, buffer, NULL, MB_OK);
                }
#endif
                break;
        }
        else                /* Lets Windows process it       */
            return (DefWindowProc(hWnd, message, wParam, lParam));

    case WM_DESTROY:          /* message: window being destroyed */
        DeleteObject(CtlText.hDefaultButton);
        DeleteObject(CtlText.hSelectButton);
        PostQuitMessage(0);
        break;

    default:              /* Passes it on if unproccessed    */
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return ( 0 );
}


/****************************************************************************

    FUNCTION: About(HWND, UINT, WPARAM, LPARAM)

    PURPOSE:  Processes messages for "About" dialog box

    MESSAGES:

    WM_INITDIALOG - initialize dialog box
    WM_COMMAND    - Input received

    COMMENTS:

    No initialization is needed for this particular dialog box, but TRUE
    must be returned to Windows.

    Wait for user to click on "Ok" button, then close the dialog box.

****************************************************************************/

BOOL CALLBACK About( HWND hDlg,       /* window handle of the dialog box */
                     UINT message,    /* type of message                 */
                     WPARAM wParam,   /* message-specific information    */
                     LPARAM lParam )  /* message-specific information    */
{
    switch (message) {
        case WM_INITDIALOG:           /* message: initialize dialog box */
            return (TRUE);

        case WM_COMMAND:                      /* message: received a command */
            if (wParam == IDOK                /* "OK" box selected?          */
                || wParam == IDCANCEL) {      /* System menu close command?  */
                EndDialog(hDlg, TRUE);        /* Exits the dialog box        */
                return (TRUE);
            }
            break;
    }
    return (FALSE);                           /* Didn't process a message    */
UNREFERENCED_PARAMETER(lParam);
}



/*****************************************************************************

    FUNCTION: HandleSelectedState

    PURPOSE:  Draw button bitmaps and text:
                  if button selected, draw "down"/pressed button bitmap
                      with text shifted right and down 2 pixels
                  else draw "up"/unpressed button bitmap
                      with text centered in button

*****************************************************************************/
void HandleSelectedState(LPDRAWITEMSTRUCT lpdisCtrl, LPCONTROLTEXT lpCtlText)
{

    int nOldBkMode;

    // handle button pressed down select state -- button down bitmap
    //   text is right & down 2 pixels
    if (lpdisCtrl->itemState & ODS_SELECTED)
    {
        DrawBitmap(lpdisCtrl->hDC,
           lpdisCtrl->rcItem,
           lpCtlText->hSelectButton,
                   SRCCOPY);

        // put text 2 pixels right and down of center of bitmap
        nOldBkMode = SetBkMode(lpdisCtrl->hDC,TRANSPARENT);

        TextOut( lpdisCtrl->hDC,
                 lpCtlText->ptPosition.x+2,
                 lpCtlText->ptPosition.y+2,
                 lpCtlText->szTitle,
                 lpCtlText->cbTitle );

        SetBkMode(lpdisCtrl->hDC,nOldBkMode);
    }
    // not selected -- button up; text is in normal position
    else
    {
        DrawBitmap(lpdisCtrl->hDC,
           lpdisCtrl->rcItem,
           lpCtlText->hDefaultButton,
                   SRCCOPY);

        // center text in bitmap
        nOldBkMode = SetBkMode(lpdisCtrl->hDC,TRANSPARENT);
        TextOut( lpdisCtrl->hDC,
                 lpCtlText->ptPosition.x,
                 lpCtlText->ptPosition.y,
                 lpCtlText->szTitle,
                 lpCtlText->cbTitle );

        SetBkMode(lpdisCtrl->hDC,nOldBkMode);
    }
}

/*****************************************************************************

    FUNCTION: HandleFocusState

    PURPOSE:  If button has focus, draw dashed rectangle around text in button

*****************************************************************************/
void HandleFocusState(LPDRAWITEMSTRUCT lpdisCtrl, LPRECT lpFocusRect, LPRECT lpSelectRect )
{
    // if focus state, draw a dashed rect around text
    if (lpdisCtrl->itemState & ODS_FOCUS)
    {
        // if selected, shift focus rect right and down 2 pixels around text
        if (lpdisCtrl->itemState & ODS_SELECTED) {
        OutputDebugString("HandleFocusState: State is ODS_SELECTED\r\n");
            DrawFocusRect(lpdisCtrl->hDC, lpSelectRect);
        }
        // else text centered, so focus rect will be too
        else {
            DrawFocusRect(lpdisCtrl->hDC, lpFocusRect);
            OutputDebugString("HandleFocusState: State is ODS_FOCUS\r\n");
        }

    }
    else
        if (lpdisCtrl->itemState & ODS_SELECTED)
            OutputDebugString(
                "Error HandleFocusState: State is ODS_SELECTED\r\n");
        else {
            char buffer[80];
            wsprintf(buffer, 
                "HandleFocusState:  lpdisCtrl->itemState is %#x\r\n",
                lpdisCtrl->itemState );
            OutputDebugString(buffer);
        }
}

/****************************************************************************

    FUNCTION: DrawBitmap

    PURPOSE:  Draw default or pushed button bitmap

****************************************************************************/

void DrawBitmap(HDC hDC, RECT rect, HBITMAP hBitmap, DWORD rop)
{
    HDC           hMemDC;
    BITMAP        bm;
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBitmap);
//    BitBlt(hDC, rect.left, rect.top, rect.right, rect.bottom, hMemDC, 0, 0, rop);
    GetObject(hBitmap, sizeof (BITMAP), (LPVOID) &bm);
    StretchBlt( hDC, rect.left, rect.top, rect.right, rect.bottom,
                hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, rop);
    DeleteDC(hMemDC);
}

/***************************************************************************

    FUNCTION: ButtonProc

***************************************************************************/

LRESULT CALLBACK ButtonProc(HWND hWnd,      /* window handle        */
                UINT message,   /* type of message       */
                WPARAM wParam,  /* additional information    */
                LPARAM lParam ) /* additional information    */
{

   switch ( message )
   {
       case WM_CHAR:
          OutputDebugString("ButtonProc: WM_CHAR received\r\n");
          SendMessage( ghWnd, WM_COMMAND, MAKEWPARAM(BOB,BN_CLICKED), 
                       (LPARAM)(hWnd));

       default:
          return CallWindowProc( OldButtonProc, hWnd, message, wParam, lParam );

   }

   return ( 0 );
}
