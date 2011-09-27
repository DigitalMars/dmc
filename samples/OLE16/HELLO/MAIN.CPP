/*
 *  main.cpp
 *
 *  This file contains:
 *
 *   WinMain() and WndProc(), which are called by Windows when the app is run.
 *   Code to initialize and uninitialize OLE.
 *   Code to create and destroy the window for the application.
 *
 */

#include "common.h"
#include "resource.h"
#include "hello.h"

#include <string.h>


/*  Global variables.
 */

DWORD g_dwCHelloCF = 0;   // Holds the return code for class factory creation.

CHello FAR* g_phello = NULL;    // Pointer to a CHello object.

char g_szAppName[] = "Hello";   // Name of the application.



/*  Forward declarations.
 */
HRESULT InitOle(void);
void UninitOle(void);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);



/*  Let Windows call WinMain and WndProc.
 */
extern "C"
{
	long FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);
	int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
}





/*  WinMain
 *
 *  Windows calls WinMain when the application starts.
 *
 */
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

	// Uninitialize OLE.
	UninitOle();

	return msg.wParam;
}





/*  InitApplication
 *
 *  Create a window and register it with Windows.
 *
 *  Return FALSE if an error occurs and TRUE otherwise.
 */
BOOL
InitApplication(HANDLE hinst)
{
	WNDCLASS  wc;

	wc.style        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc  = WndProc;
	wc.cbClsExtra   = 0;
	wc.cbWndExtra   = DLGWINDOWEXTRA;
	wc.hInstance    = hinst;
	wc.hIcon        = LoadIcon(hinst, g_szAppName); // Loads hello.ico
	wc.hCursor      = LoadCursor(NULL, IDC_ARROW);  // Normal arrow pointer
	wc.hbrBackground    = (HBRUSH) (COLOR_APPWORKSPACE+1);
	wc.lpszMenuName = NULL;             // No menus.
	wc.lpszClassName= g_szAppName;

	// Tell Windows about the window class we just created.
	// Exit if an error occurs.
	if(!RegisterClass(&wc))
	  return FALSE;

	return TRUE;
}





/*  InitInstance
 *
 *  Create an instance of CHello and make g_phello
 *  point to it..
 *
 *  Return FALSE if an error occurs and TRUE otherwise.
 */
BOOL
InitInstance(HANDLE hinst, int nCmdShow)
{
	HWND hwnd;

	// Create the window and show it
	hwnd = CreateDialog(hinst, g_szAppName, 0, NULL);
	ShowWindow(hwnd, nCmdShow);
	g_phello->m_hwnd = hwnd;

	return TRUE;
}





extern "C" long FAR PASCAL
WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch(message) {
	  case WM_COMMAND:
	  g_phello->ProcessCommand(wParam);
	  return 0;

	  case WM_DESTROY:
	   PostQuitMessage(0);
		   return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}






HRESULT
InitOle()
{
	HRESULT hresult;
	IClassFactory FAR* pcf;

	if((hresult = OleInitialize(NULL)) != NOERROR)
	  return hresult;

	// create the single global instance of CHello
	if((g_phello = CHello::Create()) == NULL){
	  hresult = ResultFromScode(E_OUTOFMEMORY);
	  return hresult;
	}

	// Create an instance of the class factory for CHello.
	// Exit if an error occurs.
	pcf = CHelloCF::Create();
	if (pcf == NULL) {
	  UninitOle();
	  return hresult;
	}

	// Register the class factroy.  Exit if an error occurs.
	hresult = CoRegisterClassObject(CLSID_CHello,
						pcf,
					CLSCTX_LOCAL_SERVER,
					REGCLS_MULTIPLEUSE,
					&g_dwCHelloCF);
	if (hresult != NOERROR) {
	  pcf->Release();
	  UninitOle();
	  return hresult;
	}

	pcf->Release();

	// If execution has reached this spot, then no errors have occurred.
	return NOERROR;
}





/*  UninitOLE
 *
 *  Tell OLE that we are going away.
 *
 */
void
UninitOle()
{
	// If a class factory was successfully created earlier then
	// tell OLE that the object is no longer available.
	if(g_dwCHelloCF != 0)
	  CoRevokeClassObject(g_dwCHelloCF);

	// cause the remaining typeinfo to be released
	if (g_phello != NULL)
	  g_phello->Release();

	// Tell OLE we are done using them.
	OleUninitialize();
}
