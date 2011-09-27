
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// GUI.C
// ================================================================
// This module contains all of the functions that interface to the
// 'graphical' part of this program. This currently only relates to
// the code that brings up the dialog box, and calls the WinHelp
// engine.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "ConGUI.h"

int DoHelp (char *szHelpTopic);
int GetDialogArgs (char ***pargv);
BOOL CenterWindow (HWND hwnd);
BOOL APIENTRY CLDlgProc (HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);


// Use WINHELP to bring up the applicaiton help file
int DoHelp (char *szHelpTopic)
{
    WinHelp (GetFocus(), "ConGUI.HLP", HELP_KEY, (DWORD)(LPSTR)szHelpTopic);
    return TRUE;
}


// Bring up the dialog box, and pass back a 'command line' as was
// specified by the user
int GetDialogArgs (char ***pargv)
{
    int ret;
    HANDLE hinst;
    HWND hwnd;
    char szFile[80];

    hinst = GetModuleHandle (NULL);
    hwnd = GetFocus();

    ret = DialogBoxParam (hinst, "CL", NULL, CLDlgProc, (LPARAM)pargv);

    if (-1 == ret) {
        ret = GetLastError();
        printf ("Unable to create dialog: %d\n", ret);
        GetModuleFileName (hinst, szFile, sizeof(szFile));
        printf ("hinst = %d\n", hinst);
        printf ("hwnd = %d\n", hwnd);
        printf ("File = %s\n", szFile);
        return FALSE;

    }
    return ret;
}

// A quick little routine that will center a window on the screen.
// Handy for dialog boxes
BOOL CenterWindow (HWND hwnd)
{
    RECT    rect;
    int     w, h;
    int     wScreen, hScreen, xNew, yNew;
    HDC     hdc;

    GetWindowRect (hwnd, &rect);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top;

    hdc = GetDC (hwnd);
    wScreen = GetDeviceCaps (hdc, HORZRES);
    hScreen = GetDeviceCaps (hdc, VERTRES);
    ReleaseDC (hwnd, hdc);

    xNew = wScreen/2 - w/2;
    yNew = hScreen/2 - h/2;

    return SetWindowPos (hwnd, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


// Create a data structure that will hold the strings for the combo boxes
// we have in our dialog. This just illustrates 'a' way to do this, not
// necessarily the best.

typedef struct tagDlgCtrls {
    int ctrlId;
    int def;
    char str[25];
    char opt[5];
} DlgCtrls;

DlgCtrls dlgctrls[] = {
    { 415, FALSE, "DOS EXE", "..." },
    { 415, FALSE, "Windows 3.0 EXE", "..."  },
    { 415, FALSE, "Windows 3.0 DLL", "..."  },
    { 415, FALSE, "Windows 3.1 EXE", "..."  },
    { 415, FALSE, "Windows 3.1 DLL", "..."  },
    { 415, TRUE,  "Windows NT EXE", "..."  },
    { 415, FALSE, "Windows NT DLL", "..."  },
    { 415, FALSE, "Windows NT Console App", "..."  },

    { 402, FALSE, "Small", "AS" },
    { 402, FALSE, "Medium", "AM" },
    { 402, FALSE, "Compact", "AC" },
    { 402, TRUE,  "Large", "AL" },
    { 402, FALSE, "Huge", "AH" },
    { 402, FALSE, "Customize", "A?" },

    { 404, FALSE, "8086", "G0" },
    { 404, FALSE, "80186", "G1" },
    { 404, TRUE,  "80286", "G2" },
    { 404, FALSE, "80386", "G3" },
    { 404, FALSE, "80486", "G4" },

    { 406, TRUE,  "stdcall", "Gz" },
    { 406, FALSE, "Pascal", "Gc" },
    { 406, FALSE, "C", "Gd" },

    { 408, FALSE, "Level 0", "W0" },
    { 408, FALSE, "Level 1", "W1" },
    { 408, FALSE, "Level 2", "W2" },
    { 408, TRUE,  "Level 3", "W3" },
    { 408, FALSE, "Level 4", "W4" },

    { 411, FALSE, "None", "" },
    { 411, FALSE, "Line Numbers Only", "Zd" },
    { 411, TRUE,  "Full Information", "Zi" },

    { 418, FALSE, "Ansi C", "Za" },
    { 418, TRUE, "MS Extensions", "Ze" },

    { 413, FALSE, "None", "" },
    { 413, TRUE,  "Protect Mode App", "GA" },
    { 413, FALSE, "Protect Mode DLL", "GD" },

    { 0, 0}  // End Of List
};


BOOL APIENTRY CLDlgProc (HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int wmId;
    static char ***pargv;
    static char **argv;

    int i, item, index, iCtrl, argc;
    char *cmd;
    char *cmdline;

    switch (msg) {
        case WM_INITDIALOG:
            // We need to initialize stuff in the dialog box...

            pargv = (char ***)lParam;
            argv = *pargv;
            CenterWindow (hdlg);

            iCtrl = i = 0;
            while (dlgctrls[i].ctrlId) {
                if (dlgctrls[i].ctrlId != iCtrl) { // Starting a new list
                    iCtrl = dlgctrls[i].ctrlId;
                }
                index = SendDlgItemMessage (hdlg, iCtrl, CB_ADDSTRING, 0, (DWORD)(LPSTR)dlgctrls[i].str);
                SendDlgItemMessage (hdlg, iCtrl, CB_SETITEMDATA, index, i);
                if (dlgctrls[i].def) {
                    SendDlgItemMessage (hdlg, dlgctrls[i].ctrlId, CB_SETCURSEL, index, 0);
                }
                i++;
            }
            return (TRUE);

        case WM_DESTROY:
            break;

        case WM_COMMAND:
            wmId = LOWORD(wParam);
            switch (wmId) {

                case T_HELP:
                    DoHelp ("Contents");
                    break;

                case IDOK:
                    cmd = cmdline = (char *)GlobalAlloc (GPTR, 128);
                    argv[0] = cmdline;
                    argc = 0;

                    if (cmdline) {
                        iCtrl = i = 0;
                        while (dlgctrls[i].ctrlId) {
                            if (dlgctrls[i].ctrlId != iCtrl) {
                                iCtrl = dlgctrls[i].ctrlId;
                                index = SendDlgItemMessage(hdlg, iCtrl, CB_GETCURSEL, 0, 0);
                                if (index) {
                                    item = SendDlgItemMessage (hdlg, iCtrl, CB_GETITEMDATA, index, 0);
                                    wsprintf ((LPSTR)cmd, "-%s", (LPSTR)dlgctrls[item].opt);
                                    cmd += strlen(cmd);
                                    cmd[0] = 0;
                                    argv[++argc] = ++cmd;
                                }
                            }
                            i++;
                        }

                    } // if (cmdline)...

                    EndDialog(hdlg, argc);
                    return (TRUE);

                case IDCANCEL:
                    EndDialog(hdlg, 0);
                    return (TRUE);
            }
            break;
    }
    return (FALSE);

    lParam; // unreferenced formal parameter
}
