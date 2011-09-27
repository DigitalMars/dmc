/****************************************************************************

    PROGRAM: NTGREP.C

    PURPOSE: Installation application for the search browse file manager extension

    FUNCTIONS:


****************************************************************************/

#include <windows.h>
#include <string.h>
#include "ntgrep.h"

HINSTANCE hInst;


#define INITCLASSNAME "SearchInitClass"

LONG APIENTRY MainWndProc(
	HWND hWnd,
	UINT message,
	UINT wParam,
	LONG lParam);


BOOL APIENTRY About(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam);

BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(
    HINSTANCE          hInstance,
    int             nCmdShow);


int PASCAL WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{

    MSG msg;
    DWORD dwVersion;
    
    UNREFERENCED_PARAMETER( lpCmdLine );
    
    /* Get the version we're running on to make sure it's NT */
    dwVersion = GetVersion();
    if (dwVersion & 0x80000000)
    {
        /* This is not NT.  Display an error message and exit. */
        MessageBox( NULL,
            "This sample application can only be run on Windows NT.\n"
            "This application will now terminate.",
            "GUIGrep",
            MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
    }
    

    if (!hPrevInstance)
	if (!InitApplication((HINSTANCE)hInstance))
	    return (FALSE);


    if (!InitInstance((HINSTANCE)hInstance, nCmdShow))
	return (FALSE);


    while (GetMessage(&msg,NULL,0,0))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
    return (msg.wParam);
}



BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS  wc;
    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)MainWndProc;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "GrepMenu";
    wc.lpszClassName = INITCLASSNAME;

    return (RegisterClass(&wc));

}


BOOL InitInstance(HINSTANCE hInstance,int nCmdShow)
{
    HWND            hWnd;


    hInst = hInstance;

    hWnd = CreateWindow(
	INITCLASSNAME,
	"File Search Installer",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	hInstance,
	NULL
    );



    if (!hWnd)
	return (FALSE);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);

}


LONG APIENTRY MainWndProc(
	HWND hWnd,
	UINT message,
	UINT wParam,
	LONG lParam)
{

    switch (message) {
	case WM_COMMAND:
	    switch (LOWORD(wParam))
	    { static char szDirBuff[MAX_PATH+12];
	      case IDM_INSTALL:
		   GetSystemDirectory(szDirBuff,MAX_PATH);
		   strcat(szDirBuff,"\\guigrep.dll");
		   if (!CopyFile("guigrep.dll",szDirBuff,FALSE))
		    { MessageBox(hWnd,"Error copying file to system directory",
					  "Grep setup",MB_OK);
			  break;
			};
			WritePrivateProfileString("AddOns","File Grepper",
						  "guigrep.dll","winfile.ini");
	   DialogBox(hInst,"ExplainBox",hWnd,(DLGPROC)About);
		  /* Fall through... (vanity) */
	      case IDM_ABOUT:
		   DialogBox(hInst,"AboutBox",hWnd,(DLGPROC)About);
		   break;
		  case IDM_UNINSTALL:
			WritePrivateProfileString("AddOns","File Grepper",
						  "","winfile.ini");
			break;

	    default:
		    return (DefWindowProc(hWnd, message, wParam, lParam));
	    };
	    break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;

	default:
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}


BOOL APIENTRY About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
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

