
#include <windows.h>

#define IDM_ABOUT   0x1000

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FAR PASCAL AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

HANDLE hInst;

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    static char szAppName[20];
    WNDCLASS wndclass;
    HWND hwnd;
    MSG msg;

    wsprintf(szAppName,"Animate%d", hInstance);

    wndclass.style = CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;//LoadIcon(hInstance, "ANIMATE14");
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);
    hInst = hInstance;

    if ((hwnd = CreateWindow(szAppName, "Animation", 
    			     WS_SYSMENU|WS_MINIMIZEBOX, 
    			     CW_USEDEFAULT,CW_USEDEFAULT,
    			      CW_USEDEFAULT,CW_USEDEFAULT, 
    			      NULL, 
    			      NULL, 
    			      hInstance, 
    			      NULL
    			     )) != NULL) 
    	{
         SetTimer(hwnd, 1, 500, NULL);
         ShowWindow(hwnd, SW_SHOWMINIMIZED);

         while (GetMessage(&msg, NULL, 0, 0)) {
             TranslateMessage(&msg);
             DispatchMessage(&msg);
         }

    } else {

              MessageBox(NULL, "CreateWindow() Failed!", "Bummer Dude", MB_OK|MB_ICONEXCLAMATION);
    }

  
    UnregisterClass(szAppName, hInstance);
    return(FALSE);
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static unsigned char mode;
    char buf[12];
    HMENU hmenu;
    static HICON hIcon;
  
    switch (message) {

        case WM_CREATE:
            hmenu = GetSystemMenu(hwnd, FALSE);
            AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR) NULL);
            AppendMenu(hmenu, MF_STRING, IDM_ABOUT, "About...");
            break;
        
        case WM_TIMER:        	
        {
	    HDC hdc;
	    
	    hdc=GetDC(hwnd);
            wsprintf(buf, "ANIMATE%d", mode);
            hIcon = LoadIcon(hInst, buf);
            mode++;
            mode %= 15;
            DefWindowProc(hwnd,WM_ICONERASEBKGND,(WORD)hdc,0L);
            DrawIcon(hdc,0,0,hIcon);
            ReleaseDC(hwnd,hdc);
        }
	    break;

        case WM_PAINT:
           {
             PAINTSTRUCT ps;
	     if(IsIconic(hwnd))
             {
              BeginPaint(hwnd,(LPPAINTSTRUCT)&ps);
              DefWindowProc(hwnd,WM_ICONERASEBKGND,(WORD)ps.hdc,0L);
              DrawIcon(ps.hdc,0,0,hIcon);
    	      EndPaint(hwnd,&ps);
    	     }
    	     else
               return DefWindowProc(hwnd,message,wParam, lParam);
            }	  
	    break;	           

         case WM_ERASEBKGND:
             if(IsIconic(hwnd))
		return TRUE;
	     else		
	        return DefWindowProc(hwnd,message,wParam, lParam);	     	
               break;
            
         case WM_QUERYDRAGICON:
            return (LONG) (WORD) hIcon;
            break; 
       
        case WM_SYSCOMMAND:
            if (wParam ==  IDM_ABOUT) {
                DialogBox(hInst, "ABOUT", hwnd, (FARPROC)AboutDlgProc);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL FAR PASCAL AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {

        case WM_INITDIALOG:
            return(TRUE);

        case WM_COMMAND:
            EndDialog(hDlg, TRUE);
            return(TRUE);
    }

    return(FALSE);
}
