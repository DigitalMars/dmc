#include <windows.h>
#include "phtest.h"
#include "proghelp.h"

LONG  APIENTRY MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LONG lParam);

DWORD GFlags;
DWORD type;
HANDLE hKey;

int WINAPI WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
INT nCmdShow)
{
    DialogBox(hInstance,
            MAKEINTRESOURCE(DID_PHTEST),
            NULL,
            (DLGPROC)MainWndProc);

    return(TRUE);
}



LONG  APIENTRY MainWndProc(
HWND hwnd,
UINT message,
WPARAM wParam,
LONG lParam)
{
    TCHAR szT[100];

    switch (message) {
    case WM_INITDIALOG:
        if (!ConnectToProgman()) {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return(0);
        }
        return(IDEF_EXECTEXT);

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            GetDlgItemText(hwnd, IDEF_EXECTEXT, szT, sizeof(szT));
            if (!ProgmanExecuteString(szT)) {
                MessageBeep(0);
            }
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        if (!DisconnectFromProgman()) {
            /*
             * Progman is not done yet - we need to wait around for the
             * transactions to complete before closing down so just
             * keep posting WM_CLOSE to ourselves to allow DDEML's DDE
             * messages to finish.
             */
            Sleep(100);
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return(0);
        };
        EndDialog(hwnd, 0);
        break;

    }
    return(0);
}


