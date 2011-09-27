/* joytoy.c - WinMain() and WndProc() for JOYTOY, along with
 *      initialization and support code.
 *
 * JOYTOY is a Windows with Multimedia application that illustrates
 *  how to use the joystick services. When run, it presents the user
 *  with a crosshair cursor. When the joystick or the mouse is moved,
 *  the cursor follows. When a joystick button is pressed, JOYTOY
 *  produces a sound and draws a bullet hole icon onto the screen.
 *
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
 *  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
 */

#include <windows.h>
#include <mmsystem.h>
#include "joytoy.h"


char szAppName[] = "JoyToy";     
HANDLE hSound1, hSound2;
HANDLE hHole;
LPSTR lpSound1, lpSound2;


/* WinMain - Entry point for JOYTOY.
 */
int PASCAL WinMain(hInstance,hPrevInstance,lpszCmdLine,cmdShow)
HANDLE hInstance,hPrevInstance;
LPSTR lpszCmdLine;
int cmdShow;
{
    MSG msg;
    OSVERSIONINFO	    osvi;

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) {
	MessageBox(NULL, "Sorry, but this sample will not run correctly on Windows NT.", "JOYTOY SDK sample", MB_OK | MB_ICONSTOP);
	return 0;
    }

    /* Initialize the application.
     */
    if(! InitJoyToy(hInstance,hPrevInstance))
        return FALSE;

    /* Standard Windows message processing loop.  We don't drop out of
     * this loop until the user quits the application.
     */
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* Unlock and free resources allocated by InitJoyToy().
     */
    UnlockResource(hSound1);
    UnlockResource(hSound2);
    FreeResource(hSound1);
    FreeResource(hSound2);
    FreeResource(hHole);

    return (msg.wParam);
}

/* DrawSight - Takes the new joystick position and moves the 
 *  mouse cursor accordingly.
 *
 * Params:  lParam - Specifies the new joystick position. The
 *  high-order word is the y position and the low-order word is
 *  the x position.
 *
 * Returns: void
 */
void DrawSight(DWORD lParam)
{
    WORD x, y;
    POINT pt;

    /* Get the current cursor position in screen coordinates.
     */
    GetCursorPos(&pt);

    /* Joystick positions are expressed in a coordinate system with the
     * origin in the upper left corner and with coordinate ranges from
     * 0 to 65535. Take the 5 most significant bits, so the position is 
     * expressed in the range of 0 to 31.
     */
    x = LOWORD(lParam) >> 11;
    y = HIWORD(lParam) >> 11;

    /* If the joystick is to the left of the center position, then move
     * the cursor position to the left. Otherwise, if the joystick is to 
     * the right of the center position, then move the cursor position 
     * to the right.
     */
    if(x <= 12)
        pt.x = pt.x + x - 17;
    else if(x >= 20)
        pt.x = pt.x + x - 15;

    /* If the joystick is below the center position, then move the cursor 
     * position down. Otherwise, if the joystick is above the center 
     * position, then move the cursor position up.
     */
    if(y <= 12)
        pt.y = pt.y + y - 17;
    else if(y >= 20)
        pt.y = pt.y + y - 15;

    /* Set the new cursor position.
     */
    SetCursorPos(pt.x, pt.y);
}

/* DrawHole - Draws an icon representing a hole in the given window at the 
 * current cursor position.
 *
 * Params:  hWnd - Specifies the handle to the window to draw in.
 *
 * Returns: void
 */
void DrawHole(HWND hWnd)
{
    HDC hDC;
    POINT pt;

    /* Get the current cursor position.
     */
    GetCursorPos(&pt);
    
    /* Get a DC, draw the icon, release DC.
     */
    hDC = GetDC(hWnd);
    DrawIcon(hDC, pt.x - 16, pt.y - 16, hHole);
    ReleaseDC(hWnd, hDC);
}


/* WndProc - Main window procedure function.
 */
LRESULT FAR PASCAL WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
            /* Capture the joystick. If this fails, beep and display
             * error, then quit.
             */
            if(joySetCapture(hWnd, JOYSTICKID1, 0, FALSE))
            {
                MessageBeep(MB_ICONEXCLAMATION);
                MessageBox(hWnd, "Couldn't capture the joystick", NULL, 
                           MB_OK | MB_ICONEXCLAMATION);
                return -1;
            }            
            break;

        case WM_ACTIVATE:
        case WM_ACTIVATEAPP:
            /* If app becomes inactive (wParam == 0), then we want to quit.
             */
            if(wParam)
                break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_KEYDOWN:
        case WM_CHAR:
            /* Also, any keystrokes or mouse buttons cause us to quit.
             */
            PostMessage(hWnd,WM_CLOSE,0,0L);
            break;

        case WM_ERASEBKGND:
            /* Process this message to keep Windows from erasing background.
             */
            return 0l;
 
        case MM_JOY1BUTTONDOWN :
            /* Joystick button pressed. Detect which button, play appropriate
             * sound, then draw bullet hole. To play sound, pass 
             * sndPlaySound() a pointer to an in-memory WAVE file and 
             * specify the SND_MEMORY flag. Specify SND_LOOP and SND_ASYNC
             * so that sound keeps playing until it's turned off after 
             * joystick button is released.
             */
            if (wParam & JOY_BUTTON1CHG)
            {
                sndPlaySound(lpSound1, SND_LOOP | SND_ASYNC | SND_MEMORY);
                DrawHole(hWnd);
            }
            else if (wParam & JOY_BUTTON2CHG)
            {
                sndPlaySound(lpSound2, SND_LOOP | SND_ASYNC | SND_MEMORY);
                DrawHole(hWnd);
            }
            break;

        case MM_JOY1BUTTONUP :
            /* Stop playing looped sound.
             */
            if (wParam & JOY_BUTTON1)
                sndPlaySound(lpSound1, SND_LOOP | SND_ASYNC | SND_MEMORY);
            else if (wParam & JOY_BUTTON2)
                sndPlaySound(lpSound2, SND_LOOP | SND_ASYNC | SND_MEMORY);
            else
                sndPlaySound(NULL, 0);
            break;

        case MM_JOY1MOVE :
            /* If joystick buttons are pressed, draw bullet holes
             * while joystick is moved.
             */
            if(wParam & (JOY_BUTTON1 | JOY_BUTTON2))
                DrawHole(hWnd);

            /* Redraw the crosshair in its new position.
             */
            DrawSight(lParam);
            break;


        case WM_DESTROY:
            /* We're shutting down. Release capture on the joystick, 
             * make sure any sounds that are playing are stopped.
             */
            joyReleaseCapture(JOYSTICKID1);
            sndPlaySound(NULL, 0);
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


/* InitJoyToy - Application initialization routine.
 *
 * Params:  hInstance - App's instance handle.
 *
 * Returns: TRUE if initialization is successful, FALSE otherwise.
 */
BOOL InitJoyToy(HANDLE hInstance, HANDLE hPrevInstance)
{
    WNDCLASS wc;
    HWND     hWnd;

    /* Make sure there is a joystick device installed.
     */
    if (!joyGetNumDevs())
    {
        MessageBox(NULL, "There are no joystick devices installed. Exiting.",
                   NULL, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }
   
    /* Load bullet hole icon.
     */
    hHole = LoadIcon(hInstance, "HoleIcon");

    if (hPrevInstance == NULL) {
        /* Setup and register a window class for our main window.
         */
        wc.hCursor          = LoadCursor(NULL, IDC_CROSS);
        wc.hIcon            = NULL;
        wc.lpszMenuName     = NULL;
        wc.lpszClassName    = szAppName;
        wc.hbrBackground    = GetStockObject(BLACK_BRUSH);
        wc.hInstance        = hInstance;
        wc.style            = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc      = WndProc;
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
    
        if(! RegisterClass(&wc))
            return FALSE;   
    }

    /* Create a full-screen window with no title bar or scroll bars.
     */
    hWnd = CreateWindow(szAppName, szAppName, WS_POPUP | WS_VISIBLE, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    if (hWnd != NULL) {
        /* Load and lock sound resources.
         */
        hSound1 = LoadResource(hInstance, FindResource(hInstance, "SOUND1", "WAVE"));
        hSound2 = LoadResource(hInstance, FindResource(hInstance, "SOUND2", "WAVE"));
        lpSound1 = LockResource(hSound1);
        lpSound2 = LockResource(hSound2);
    }
    return hWnd != NULL;
}
