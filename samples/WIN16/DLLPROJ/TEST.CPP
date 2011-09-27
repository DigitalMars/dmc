// *******************************************************************
//
//
//			TEST.CPP
//
//
//
//		This program calls a C++ DLL function. This is barely
//		C++ code, and is only intended to show the use of C++
//		DLLs.
//
//
// *******************************************************************




#include <windows.h>
#include <stdio.h>
#include "thedll.hpp"



extern "C" LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
extern "C" int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
                              LPSTR lpszCmdLine, int nCmdShow);

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
		    LPSTR lpszCmdLine, int nCmdShow)

{

  static char szAppName[] = "Test";
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;

  if (!hPrevInstance)
  {
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);
  }

  hwnd = CreateWindow (szAppName,
  		       "DLL TESTER",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

  ShowWindow (hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessage (&msg, NULL, 0, 0))
  {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  return msg.wParam;

}



LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  static RECT winrect;
  static BOOL bDown   = FALSE;
  static int x = -1;
  static int y = -1;

  switch (message)
  {
    case WM_CREATE:
      GetClientRect(hwnd,&winrect);
      return 0;
    break;
    
    case WM_LBUTTONDOWN:
      bDown = TRUE;
      x = LOWORD(lParam);
      y = HIWORD(lParam);
      InvalidateRect(hwnd,&winrect,TRUE);
      return 0;
    break;

    case WM_LBUTTONUP:
      bDown = FALSE;
      return 0;
    break;
    
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      DrawText(hdc, "This is a test...", -1, &winrect,
      			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

// ***** Call the static DLL member function
      test_class::DllFunc(hdc,x,y);

      EndPaint(hwnd, &ps);
      return 0;
    break;

    case WM_DESTROY:
      PostQuitMessage (0);
      return 0;
    break;

  }

  return DefWindowProc (hwnd, message, wParam, lParam);

}
