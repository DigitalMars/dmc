
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************

    PROGRAM: Platform.c

    PURPOSE: Platform template for Windows applications

    FUNCTIONS:

        WinMain() - calls initialization function, processes message loop
        InitApplication() - initializes window data and registers window
        InitInstance() - saves instance handle and creates main window
        MainWndProc() - processes messages
        About() - processes messages for "About" dialog box

    COMMENTS:

        Windows can have several copies of your application running at the
        same time.  The variable hInst keeps track of which instance this
        application is so that processing will be to the correct window.

****************************************************************************/

#define STRICT


#include <stddef.h>
#include "windows.h"                /* required for all Windows applications */

#if defined(_WIN32)
#define WIN32S  0x80000000l
#else
#define WINAPI   FAR PASCAL
#define TCHAR char
#define LPTSTR LPSTR
#define UNREFERENCED_PARAMETER(x) x;
#define TEXT(x) x

#define WF1_WINNT 0x40        /* A special version of GetWinFlags() will
                               * be provide in the WOW Layer which returns
                               * a MANEFEST CONSTANT signifing to Win16
                               * apps that it's running in WOW instead of
                               * DOS/Windows. Unfortunately this value
                               * was not specified at the time of this
                               * sample. As *soon* as Development has set
                               * on a manifest contanst PSS will upload a
                               * KnowledgeBase article. Describing this flag
                               * and any other relevent details need to get
                               * this sample correctly identifing what platform
                               * it's running on.
                               */

#endif


#include "Platform.h"               /* specific to this program              */


HINSTANCE hInst;                       /* current instance                      */


/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

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

int PASCAL WinMain(HINSTANCE hInstance,      /* current instance  */
                   HINSTANCE hPrevInstance,  /* previous instance */
                   LPSTR     lpCmdLine,      /* command line      */
                   int       nCmdShow)       /* show-window type (open/icon) */
{
    MSG msg;                                 /* message                      */

    if (!hPrevInstance)                  /* Other instances of app running? */
        if (!InitApplication(hInstance)) /* Initialize shared things */
            return (FALSE);              /* Exits if unable to initialize     */

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,        /* message structure                      */
            NULL,                  /* handle of window receiving the message */
            0,                     /* lowest message to examine              */
            0))                    /* highest message to examine             */
    {
            TranslateMessage(&msg);    /* Translates virtual key codes           */
            DispatchMessage(&msg);     /* Dispatches message to window           */
    }

    return (msg.wParam);           /* Returns the value from PostQuitMessage */
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

    wc.style = 0;                        /* Class style(s).                    */
    wc.lpfnWndProc = (WNDPROC)MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "PlatformMenu";   /* Name of menu resource in .RC file. */
    wc.lpszClassName = "PlatformWClass"; /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return (RegisterClass(&wc));

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

BOOL InitInstance(HANDLE hInstance,     /* Current instance identifier.     */
                  int nCmdShow )        /* Param for first ShowWindow() call. */
{
    HWND            hWnd;               /* Main window handle.                */

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    hInst = hInstance;

    /* Create a main window for this application instance.  */

    hWnd = CreateWindow(
        "PlatformWClass",                /* See RegisterClass() call.          */
        "Platform Sample Application",   /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        10,                  /* Default horizontal position.       */
        10,                  /* Default vertical position.         */
        600,                  /* Default width.                     */
        400,                  /* Default height.                    */
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer not needed.                */
    );

    /* If window could not be created, return "failure" */

    if (!hWnd)
        return (FALSE);

    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hWnd, nCmdShow);  /* Show the window                        */
    UpdateWindow(hWnd);          /* Sends WM_PAINT message                 */
    return (TRUE);               /* Returns the value from PostQuitMessage */

}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, WORD, WPARAM, LPARAM)

    PURPOSE:  Processes messages

    MESSAGES:

        WM_COMMAND    - application menu (About dialog box)
        WM_DESTROY    - destroy window

    COMMENTS:

        To process the IDM_ABOUT message, call MakeProcInstance() to get the
        current instance address of the About() function.  Then call Dialog
        box which will create the box according to the information in your
        Platform.rc file and turn control over to the About() function. When
        it returns, free the intance address.

****************************************************************************/

LRESULT CALLBACK MainWndProc(HWND hWnd,      /* window handle             */
                             UINT message,   /* type of message           */
                             WPARAM wParam,  /* additional information    */
                             LPARAM lParam ) /* additional information    */
{
    DLGPROC lpProcAbout;
    DWORD   version, flags;
    char    buf[80];

    switch (message) {

        case WM_COMMAND:           /* message: command from application menu */
            if (LOWORD(wParam) == IDM_ABOUT) {
                lpProcAbout = (DLGPROC)MakeProcInstance((FARPROC)About, hInst );

                DialogBox(hInst,                 /* current instance         */
                    "AboutBox",                  /* resource to use          */
                    hWnd,                        /* parent handle            */
                    lpProcAbout);       /* About() instance address */

                FreeProcInstance((FARPROC)lpProcAbout);
                break;
            }
            else if ( LOWORD(wParam) == IDM_PLATFORM ) {
                version = GetVersion();
                wsprintf( buf, "The version of Windows that you are running is %d.%d",
                          LOBYTE(LOWORD(version)), HIBYTE(LOWORD(version)) );

                MessageBox( hWnd, buf, "Platform", MB_OK );
#if defined(_WIN32) || defined(WIN32)
/*
 * bugbug Currently HIWORD of version is 500. The highbit is not set
 */

                if (version < 0x80000000)                
                    wsprintf( buf, "This is running on Win32/NT" );
                else
                    if (LOBYTE(LOWORD(version)) < 4)
                        wsprintf( buf, "This is running on Win32s" );
                    else
                        wsprintf( buf, "This is running on Windows 95" );
                    
                UNREFERENCED_PARAMETER(flags);
#else
                flags = GetWinFlags();
                wsprintf ( buf, "flags are %#lx", flags );
//                MessageBox ( hWnd, buf, NULL, MB_OK );

                if ( flags & WF1_WINNT )
                    wsprintf ( buf, "This is running in the WOW layer" );
                else
                    if (HIBYTE(LOWORD(version)) == 95)
                        wsprintf( buf, "This is running on Windows 95" );
                    else    
                        wsprintf( buf, "The version of MS-DOS you're running is %d.%d",
                            HIBYTE(HIWORD(version)), LOBYTE(HIWORD(version)) );
#endif
                MessageBox( hWnd, buf, "Platform", MB_OK );
            }
            else                            /* Lets Windows process it       */
                return (DefWindowProc(hWnd, message, wParam, lParam));

            break;

        case WM_DESTROY:                  /* message: window being destroyed */
            PostQuitMessage(0);
            break;

        default:                          /* Passes it on if unproccessed    */
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return ( 0 );
}


/****************************************************************************

    FUNCTION: About(HWND, unsigned, WORD, LONG)

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
                     LPARAM lParam )
{
    switch (message) {
        case WM_INITDIALOG:                /* message: initialize dialog box */
            return (TRUE);

        case WM_COMMAND:                      /* message: received a command */
            if (wParam == IDOK                /* "OK" box selected?          */
                || wParam == IDCANCEL) {      /* System menu close command? */
                EndDialog(hDlg, TRUE);        /* Exits the dialog box        */
                return (TRUE);
            }
            break;
    }
    return (FALSE);                           /* Didn't process a message    */
UNREFERENCED_PARAMETER(lParam);
}
