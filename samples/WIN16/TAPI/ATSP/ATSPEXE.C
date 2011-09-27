//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Copyright 1993-1995 Microsoft Corporation, all rights reserved.
#include <windows.h>
#include <memory.h>
#include "atsp.h"


/* routines defined in serial.c */

long doAsyncCommand (int, ATSPLineData *);
int  commEvent (int hcd, int flag);
extern HWND hwndTalkDrop;


char ATSP_CLASS[] =  "AtspAppWinClass";
char atsp_tsp[] =  "ATSP.TSP";
char app_reg[]          =  "ATSPAppRegister";

HINSTANCE hinst;
HWND             myhwnd;


LRESULT CALLBACK __export atspwndproc (
HWND     hWnd,
UINT     uMsg,
WPARAM wParam,
LPARAM lParam)
{
        switch (uMsg)
        {
                case WM_CLOSE:
                        // the destroy and unregister need to be here to cover
                        // the case of atsp re-initializing quickly. In that case
                        // the class has to be unregistered before the sendmessage(wm_close)
                        // returns or the subsequent reinit will fail
                        DestroyWindow (myhwnd);
                        UnregisterClass (ATSP_CLASS, hinst);
              PostQuitMessage (0);
              return 0;

                case WM_EXEMSG:
                        return doAsyncCommand (wParam, (ATSPLineData *) lParam);

                case WM_COMMNOTIFY:
                        return commEvent (wParam, LOWORD (lParam));

                default:
                        return DefWindowProc (hWnd, uMsg, wParam, lParam);
        }
}


int RegisterWithTSP ()
{
        int                     errm     = SetErrorMode (SEM_NOOPENFILEERRORBOX);
        HINSTANCE   hpinst = LoadLibrary (atsp_tsp);
        SetErrorMode (errm);

        if (hpinst > HINSTANCE_ERROR)
        {
                int       res;
                FARPROC lpfnATSPAppRegister = GetProcAddress (hpinst, app_reg);

                if (lpfnATSPAppRegister)
                {
                        res = (*lpfnATSPAppRegister) (myhwnd);
                        FreeLibrary (hpinst);
                        return (res);
                }
        }

        return 1;
}

int FAR PASCAL __export WinMain (
HINSTANCE hInstance,
HINSTANCE hPrevInst,
LPSTR     lpszCmdLine,
int       nCmdShow)
{
        WNDCLASS        wc;
        int             res = FALSE;

        // Define and register main window's class.

        memset (&wc, 0, sizeof (WNDCLASS));

        wc.lpfnWndProc          = atspwndproc;
        wc.hInstance            = hInstance;
        wc.lpszClassName        = ATSP_CLASS;

        if (RegisterClass (&wc))
        {
                hinst  = hInstance;

                myhwnd = CreateWindow (ATSP_CLASS, NULL,
                                                                          WS_OVERLAPPED | WS_MINIMIZE,
                                                                          0, 0, 0, 0, NULL, NULL, hInstance, NULL);
                if (myhwnd)
                {
                        if (RegisterWithTSP () == 0)
                        {
                                MSG msg;

                                while (GetMessage (&msg, NULL, 0, 0))
                                {
                                        if (hwndTalkDrop == 0 || !IsDialogMessage (hwndTalkDrop, &msg))
                                        {
                                                TranslateMessage (&msg);
                                                DispatchMessage (&msg);
                                        }
                                }

                                res = msg.wParam;
                        }
                }
        }

        return res;
}
