//**********************************************************************
// File name: simpcntr.h
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************
#define IDM_ABOUT   100
#define IDM_INSERT  101
#define IDM_VERB0   1000

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
BOOL InitApplication(HANDLE hInstance);
BOOL InitInstance(HANDLE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DocWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
