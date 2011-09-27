/**
* STRESS.C    - Symantec !Stress!
*
*
* Description - Main module for the STRESS.EXE utility
*
*
* (C)Copyright 1993 by Brian Weed for Symantec Corporation
* All rights reserved.
*
*
* Version    Date       By            Comments
* -------    --------   ------------  -------------------------------------
*  1.0a      05-13-93   Brian Weed    Initial design
*
**/

#include <windows.h>
#include <stress.h>
#include <stdlib.h>

#define IDM_ABOUT   0x1000
#define IDM_CPU     0x1001
#define HOG_OK      101
#define HOG_CANCEL  102
#define HOG_MEM     108
#define HOG_USERMEM 109
#define HOG_GDIMEM  110
#define HOG_HANDLES 111
#define HOG_DISK    112

#define ULONG unsigned long

/* globals */
struct G_STRESS {

    BOOL cpu;            /* TRUE if we are to use CPU time slices */
    BOOL end;            /* TRUE if the user picks Cancel to end the App */

    HANDLE hInst;
    ULONG  mem;             /* ammount of global memory (in k )to leave available */
    UINT   userMem;         /* ammount of user memory (in k) to leave available */
    UINT   gdiMem;          /* ammount of GDI memory (in k) to leave */
    UINT   handles;         /* number of file handles to leave available */
    ULONG  diskSpace;       /* disk space (in k) to leave */
    int    mode;            /* specifies Stress Icon animation mode */
} stress;

/*
* prototypes
*/
int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK HogDlgProc(HWND, UINT, WPARAM, LPARAM);
int StressMessageLoop(void);

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    static char szAppName[] = "Symantec !Stress!";
    WNDCLASS wndclass;
    HWND hwnd;

    if (!hPrevInstance) {
        wndclass.style = 0;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(hInstance, "STRESS0");
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        RegisterClass(&wndclass);
    }

    /* remember who we are */
    stress.hInst = hInstance;
    
    /* load initial dialog box to ask user what resources to use up */
    DialogBox(hInstance, "HOG_DIALOG", NULL, (FARPROC)HogDlgProc);

    /*  NOTE: MakeProcInstance() was not needed because the compiler is
    *   generating prolog/epilog code for the local functions
    */

    /*
    * if the user didn't click CANCEL then continue by showing the Icon, and
    * taking resources
    */
    if (!stress.end) {

        /* timer needed for Animated Icon */
        SetTimer(hwnd = CreateWindow(szAppName, "Symantec !Stress!", WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,NULL, hInstance, NULL), 1, 1000, NULL);

        /* App is always minimized */
        ShowWindow(hwnd, SW_SHOWMINNOACTIVE);

        return(StressMessageLoop());
    }

  return(FALSE);
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HMENU hmenu;
    HICON hIcon;

    switch (message)
    {
        case WM_SYSCOMMAND:
            switch (wParam)
            {
                /*
                * toggle the "CPU Stress" option
                */
                case IDM_CPU:

                    /* toggle */
                    stress.cpu = ~stress.cpu;

                    /*
                    * either check or uncheck the CPU Stress option on
                    * the System Menu
                    */
                    CheckMenuItem(GetSystemMenu(hwnd, FALSE), IDM_CPU, MF_BYCOMMAND | (stress.cpu ? MF_CHECKED:MF_UNCHECKED));
                break;

                /*
                * display the About Box dialog and execute AboutDlgProc
                */
                case IDM_ABOUT:
                    DialogBox(stress.hInst, "ABOUT", hwnd, (FARPROC)AboutDlgProc);
                break;
            }
            break;

        case WM_TIMER:
            InvalidateRect(hwnd, NULL, TRUE);

            switch (stress.mode) {
                case 0:
                    hIcon = LoadIcon(stress.hInst, "STRESS0");
                    break;
                case 1:
                    hIcon = LoadIcon(stress.hInst, "STRESS1");
                    break;
                case 2:
                    hIcon = LoadIcon(stress.hInst, "STRESS2");
                    break;
                case 3:
                    hIcon = LoadIcon(stress.hInst, "STRESS3");
                    break;
                case 4:
                    hIcon = LoadIcon(stress.hInst, "STRESS2");
                    break;
                case 5:
                    hIcon = LoadIcon(stress.hInst, "STRESS1");
                    break;
            }

            stress.mode++;
            stress.mode %= 6;

            /* reset the application's icon to the next frame */
            SetClassWord(hwnd, GCW_HICON, hIcon);

            return(0);

        case WM_PAINT:

            /* display the new icon */
            ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
            return(0);

        /*
        * On initialization of the WindowProcedure
        */
        case WM_CREATE:

            /*
            * add these menu items to the System Menu
            */
            hmenu = GetSystemMenu(hwnd, FALSE);

            /* set initial "CPU Stress" menu item to UNCHECKED */
            stress.cpu = FALSE;

            /* CPU Stress option */
            AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR) NULL);
            AppendMenu(hmenu, MF_STRING, IDM_CPU, "CPU Stress");

            /* about box */
            AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR) NULL);
            AppendMenu(hmenu, MF_STRING, IDM_ABOUT, "About...");
        break;

        /*
        * don't allow this application to maximize
        */
        case WM_QUERYOPEN:
            return(0);
        break;

        case WM_DESTROY:
          PostQuitMessage(0);
          return(0);
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

/*
*
* main message processing loop - will dispatch messages to our WndProc
* after using up many resources.
*
*/
int StressMessageLoop()
{
    MSG msg;
    LONG i;

    msg.message = ~WM_QUIT;

    /* use all global memory except for what the user specified */
    AllocMem(stress.mem*1024);

    /* use all user memory except for what the user specified */
    AllocUserMem(stress.userMem*1024);

    /* use all Graphics Device Memory except... */
    AllocGDIMem(stress.gdiMem*1024);

    /* use all file handles except... */
    AllocFileHandles(stress.handles);

    /* use all disk space except... */
    AllocDiskSpace(stress.diskSpace*1024, EDS_WIN);

    /*
    * keep processing messages until the Stress App is closed
    */
    while (1) {

        /*
        * if the "CPU Stress" menu item is checked then stress the CPU also
        */
        if (stress.cpu) {
            for (i=0;i<100000;i++)
                ;
        }

        /*
        * make sure to release SOME time slices to windows
        */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    /*
    * free all used resources (in reverse order)
    */
    UnAllocDiskSpace(EDS_WIN);
    UnAllocFileHandles();
    FreeAllGDIMem();
    FreeAllUserMem();
    FreeAllMem();

    return(msg.wParam);
}

BOOL FAR PASCAL AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {

    case WM_INITDIALOG:
        return(TRUE);

    case WM_COMMAND:
        EndDialog(hDlg, TRUE);
        return(TRUE);
    }

    return(FALSE);
}

BOOL FAR PASCAL HogDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char tempBuf[128];

    switch (msg) {

        case WM_INITDIALOG:
            return(TRUE);

        case WM_SYSCOMMAND:

            switch (wParam) {

                case SC_CLOSE:
                    stress.end = TRUE;
                    EndDialog(hDlg, TRUE);
                    return(TRUE);
            }
            return (FALSE);

        case WM_COMMAND:

            switch (wParam) {

                case HOG_CANCEL:
                    stress.end = TRUE;
                case HOG_OK:
                    EndDialog(hDlg, TRUE);
            }

            /*
            * If anything was done, then re-read all of these fields to make sure
            * we got any changes - there might be a better way of doing this, but
            * I don't know it.
            */

            /*
            * get the info from the Global Memory edit field
            */
            *(WORD *)tempBuf = sizeof(tempBuf) - 1;
            tempBuf[SendDlgItemMessage(hDlg, HOG_MEM, EM_GETLINE, 0, (DWORD)(LPSTR)tempBuf)] = '\0';

            /* Windows needs atleast 1meg to continue running */
            if ((stress.mem = atol(tempBuf)) < 1000)
                stress.mem = 1000;

            /*
            * get the info from the User Memory edit field
            */
            *(WORD *)tempBuf = sizeof(tempBuf) - 1;
            tempBuf[SendDlgItemMessage(hDlg, HOG_USERMEM, EM_GETLINE, 0, (DWORD)(LPSTR)tempBuf)] = '\0';
            stress.userMem = atoi(tempBuf);

            /*
            * get the info from the GDI Memory field
            */
            *(WORD *)tempBuf = sizeof(tempBuf) - 1;
            tempBuf[SendDlgItemMessage(hDlg, HOG_GDIMEM, EM_GETLINE, 0, (DWORD)(LPSTR)tempBuf)] = '\0';
            stress.gdiMem = atoi(tempBuf);

            /*
            * get the info from the File Handles field
            */
            *(WORD *)tempBuf = sizeof(tempBuf) - 1;
            tempBuf[SendDlgItemMessage(hDlg, HOG_HANDLES, EM_GETLINE, 0, (DWORD)(LPSTR)tempBuf)] = '\0';
            stress.handles = atoi(tempBuf);

            /*
            * get the info from the Disk Space field
            */
            *(WORD *)tempBuf = sizeof(tempBuf) - 1;
            tempBuf[SendDlgItemMessage(hDlg, HOG_DISK, EM_GETLINE, 0, (DWORD)(LPSTR)tempBuf)] = '\0';
            stress.diskSpace = atol(tempBuf);

            return(TRUE);
    }

    return(FALSE);
}
