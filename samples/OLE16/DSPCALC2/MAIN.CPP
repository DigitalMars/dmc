/***
*main.cpp
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  This module is the main entry point of the sample IDispatch
*  calculator, dspcalc2.exe.
*
*  This program is intended to demonstrate an implementation of
*  the IDispatch interface.
*
*Implementation Notes:
*
*****************************************************************************/

#include <windows.h>
#include <ole2.h>
#if !defined(WIN32)
#include <olenls.h>
#endif
#include <dispatch.h>

#include "resource.h"
#include "dspcalc2.h"


HANDLE g_hinst = 0;

DWORD g_dwCCalcCF = 0;
DWORD g_dwRegisterCCalc = 0;
CCalc FAR* g_pcalc = NULL;

char g_szAppName[] = "DspCalc2";


HRESULT InitOle(void);
void UninitOle(void);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);


extern "C" {
long FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);
int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
}


extern "C" int PASCAL
WinMain(
	HANDLE hinst,
	HANDLE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	if(!hPrevInstance)
	  if(!InitApplication(hinst))
	return FALSE;

	if(InitOle() != NOERROR)
	  return FALSE;

	if(!InitInstance(hinst, nCmdShow)){
	  UninitOle();
	  return FALSE;
	}

	while(GetMessage(&msg, NULL, NULL, NULL)) {
	  TranslateMessage(&msg);
	  DispatchMessage(&msg);
	}

	UninitOle();

	return msg.wParam;
}


BOOL
InitApplication(HANDLE hinst)
{
	WNDCLASS  wc;

	wc.style        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc  = WndProc;
	wc.cbClsExtra   = 0;
	wc.cbWndExtra   = DLGWINDOWEXTRA;
	wc.hInstance    = hinst;
	wc.hIcon        = LoadIcon(hinst, g_szAppName);
	wc.hCursor      = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName    = g_szAppName;

	if(!RegisterClass(&wc))
	  return FALSE;

	return TRUE;
}


BOOL
InitInstance(HANDLE hinst, int nCmdShow)
{
	g_hinst = hinst;

	g_pcalc->m_hwnd = CreateDialog(hinst, g_szAppName, 0, NULL);

	ShowWindow(g_pcalc->m_hwnd, nCmdShow);

	g_pcalc->m_arith.Display();

	return TRUE;
}


extern "C" long FAR PASCAL
WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch(message){
	case WM_COMMAND:
	  switch(wParam){
	  case IDC_ZERO:
	  case IDC_ONE:
	  case IDC_TWO:
	  case IDC_THREE:
	  case IDC_FOUR:
	  case IDC_FIVE:
	  case IDC_SIX:
	  case IDC_SEVEN:
	  case IDC_EIGHT:
	  case IDC_NINE:
	  case IDC_PLUS:
	  case IDC_MINUS:
	  case IDC_MULT:
	  case IDC_DIV:
	  case IDC_CLEAR:
	  case IDC_EQUALS:
	g_pcalc->m_arith.ButtonPush(wParam);
	return 0;
	  }
	  break;

	case WM_DESTROY:
	  PostQuitMessage(0);
	  return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


/***
*HRESULT InitOle(void)
*Purpose:
*  Initialize Ole, and register our class factories.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
HRESULT
InitOle()
{
	HRESULT hresult;
	IClassFactory FAR* pcf;


	if((hresult = OleInitialize(NULL)) != NOERROR)
	  goto LError0;

	// create the single global instance of CCalc
	if((g_pcalc = CCalc::Create()) == NULL){
	  hresult = ResultFromScode(E_OUTOFMEMORY);
	  goto LError0;
	}

	if((pcf = CCalcCF::Create()) == NULL)
	  goto LError1;

	hresult = CoRegisterClassObject(
	  CLSID_CCalc2,
	  pcf,
	  CLSCTX_LOCAL_SERVER,
	  REGCLS_MULTIPLEUSE,
	  &g_dwCCalcCF);
	if(hresult != NOERROR)
	  goto LError2;

	hresult = RegisterActiveObject(
	  g_pcalc, CLSID_CCalc2, NULL, &g_dwRegisterCCalc);
	if(hresult != NOERROR)
	  goto LError2;

	pcf->Release();

	return NOERROR;

LError2:;
	pcf->Release();

LError1:;
	UninitOle();

LError0:;
	return hresult;
}


void
UninitOle()
{
	if(g_dwRegisterCCalc != 0)
	  RevokeActiveObject(g_dwRegisterCCalc, NULL);

	if(g_dwCCalcCF != 0)
	  CoRevokeClassObject(g_dwCCalcCF);

	// cause the remaining typeinfo to be released
	if(g_pcalc != NULL)
	  g_pcalc->Release();

	OleUninitialize();
}
