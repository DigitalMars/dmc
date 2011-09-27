
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define WSA_ACCEPT   (WM_USER + 0)
#define WSA_READ     (WM_USER + 1)

#define IDM_ABOUT 100
#define IDM_HOSTNAME  102
#define IDM_LISTEN  103
#define IDM_CONNECT  104
#define IDM_ALISTEN  105
#define IDM_CANCEL   106
#define IDM_TLISTEN  107
#define IDM_SENDTCP  108

/*
*   Structure passed into the accept thread
*/
typedef struct {
   int    nThread;  // Thread number.
   HWND  hWnd;
   } THREADPACK, *PTHREADPACK;


 
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
void SetConnectMenus( HWND );
LONG APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
LRESULT APIENTRY About(HWND, UINT, UINT, LONG);
LRESULT APIENTRY GetHostName(HWND, UINT, UINT, LONG);
LRESULT APIENTRY DisplayHostEnt(HWND, UINT, UINT, LONG);
LRESULT APIENTRY GetTcpPort(HWND, UINT, UINT, LONG);
LRESULT APIENTRY GetSendString(HWND, UINT, UINT, LONG);

/* GLOBAL VARIABLES ======================================*/
PHOSTENT phe;

#define IDD_EDIT                    201
#define IDD_TEXT                    202
#define IDD_ALIAS                   203
#define IDD_HOSTNAME                206
#define IDD_LBALIAS                                             207
#define IDD_LBADDR                                              208

