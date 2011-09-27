//**********************************************************************
// File name: simpsvr.h
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************
#define IDM_ABOUT   100
#define IDM_INSERT  101
#define IDM_VERB0   1000   


int     PASCAL   WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DocWndProc (HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK About      (HWND, UINT, WPARAM, LPARAM);
