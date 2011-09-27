//THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright  1994-1995  Microsoft Corporation.  All Rights Reserved.
//
//	PROGRAM:	Comdlg32.c
//
//	PURPOSE:	Demonstrates how to manipulate the file open common dialog.
//
//	PLATFORMS:	Windows 95
//
//	FUNCTIONS:	
//		WinMain() - calls initialization function, processes message loop
//		InitApplication() - Initializes window data nd registers window
//		InitInstance() -saves instance handle and creates main window
//		MainWindProc() Processes messages
//		About() - Process menssages for "About" dialog box
//      	OpenTheFile() - Processes the File Open common dialog box
//      	ComDlg32DlgProc() - Hook procedure for GetOpenFileName() common dialog
//		TestNotify() - Processes the WM_NOTIFY function for ComDlg32DlgProc
//		ProcessCDError() - uses CommonDialogExtendedError() to output useful error 
//      		messages
//
//	SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>    // includes basic windows functionality
#include <commdlg.h>    // includes common dialog functionality
#include <dlgs.h>       // includes common dialog template defines
#include <cderr.h>      // includes the common dialog error codes
#include "COMDLG32.h"   // includes application-specific information

const char szmsgSHAREVIOLATION[] = SHAREVISTRING;  // string for sharing violation
const char szmsgFILEOK[]         = FILEOKSTRING;   // string for OK button
const char szCommdlgHelp[]       = HELPMSGSTRING;  // string for Help button

UINT cdmsgShareViolation = 0;  // identifier from RegisterWindowMessage
UINT cdmsgFileOK         = 0;  // identifier from RegisterWindowMessage
UINT cdmsgHelp           = 0;  // identifier from RegisterWindowMessage

typedef struct _MYDATA
{
	char szTest1[80];		// a test buffer containing the file selected
	char szTest2[80];       // a test buffer containing the file path
} MYDATA, FAR * LPMYDATA;

HINSTANCE g_hInst;		// the current instance		
MYDATA sMyData;			// an instance of a MYDATA
 
//
//  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
//
//  PURPOSE: Entry point for the application.
//
//  COMMENTS:
//
//	This function initializes the application and processes the
//	message loop.
//
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{

    MSG msg;                         

    if (!InitApplication(hInstance))
            return (FALSE);              

    // Create the main window.
    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    // Acquire and dispatch messages until a WM_QUIT message is received.
    while (GetMessage(&msg,NULL,0,0))
        {
            TranslateMessage(&msg);    // Translates virtual key codes.
            DispatchMessage(&msg);     // Dispatches message to window.
        }
    return (msg.wParam);           // Returns the value from PostQuitMessage.

}

//
//  FUNCTION: InitApplication(HANDLE)
//
//  PURPOSE: Initializes window data and registers window class 
//
//  COMMENTS:
//
//       In this function, we initialize a window class by filling out a data
//       structure of type WNDCLASS and calling the Windows RegisterClass()
//       function.
//
BOOL InitApplication(HANDLE hInstance)       
{
    WNDCLASS  wc;

    // Register the window class for my window.                                                           */
    wc.style = 0;                       // Class style.
    wc.lpfnWndProc = (WNDPROC)MainWndProc; // Window procedure for this class.
    wc.cbClsExtra = 0;                  // No per-class extra data.
    wc.cbWndExtra = 0;                  // No per-window extra data.
    wc.hInstance = hInstance;           // Application that owns the class.
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "Comdlg32Menu";   // Name of menu resource in .RC file. 
    wc.lpszClassName = "Comdlg32WClass"; // Name used in call to CreateWindow.

    return (RegisterClass(&wc));
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window 
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance( HANDLE hInstance, int nCmdShow)           
{
    HWND            hWnd;              

	// Save off the handle to the current instance.
    g_hInst = hInstance;

    // Create a main window for this application instance. 
    hWnd = CreateWindow(
        "Comdlg32WClass",
        "Common Dialog Sample Application",
        WS_OVERLAPPEDWINDOW,            // Window style.
        CW_USEDEFAULT,                  // Default horizontal position.
        CW_USEDEFAULT,                  // Default vertical position.
        CW_USEDEFAULT,                  // Default width.
        CW_USEDEFAULT,                  // Default height.
        NULL,                           // Overlapped windows have no parent.
        NULL,                           // Use the window class menu.
        g_hInst,                        // This instance owns this window.
        NULL                            // Pointer not needed.
    );

    // If window could not be created, return "failure".
    if (!hWnd)
        return (FALSE);

    // Register the window messages to receive.
    cdmsgShareViolation = RegisterWindowMessage(szmsgSHAREVIOLATION);
    cdmsgFileOK         = RegisterWindowMessage(szmsgFILEOK);
    cdmsgHelp           = RegisterWindowMessage(szCommdlgHelp);

    // Make the window visible; update its client area; and return "success".
    ShowWindow(hWnd, nCmdShow);  
    UpdateWindow(hWnd);          
    return (TRUE);               

}

//
//  FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  MESSAGES:
//
//	WM_CREATE - initialize window and create the MLE
//	WM_COMMAND - process the application menu
//	WM_SIZE - size the MLE in the main window
//	WM_DESTROY - post a quit message and return
//
//
LONG APIENTRY MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	static HWND hwndEdit;
    CHAR lpszHello[] = "Choose File Open from the Options menu for a demo.";

    switch (message) {

        case WM_CREATE:
			// Create an MLE for the file contents.
			hwndEdit = CreateWindow(
                "EDIT",     
				NULL,       
                WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
                0, 0, 0, 0, 
                hWnd,       
                (HMENU) ID_EDITCHILD, 
                (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
                NULL);               

            // Update the MLE. 
            SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) lpszHello);
            return 0;
 
        case WM_SIZE:
            // Make the edit control the size of the window's client area. 
            MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);          
            return 0;

        case WM_COMMAND:           // message: command from application menu 
            switch( LOWORD( wParam ))
            {
                case IDM_FILEOPEN:
					// Call the FileOpen common dialog to open the file.
                    OpenTheFile( hWnd, hwndEdit );
                    break;

                case IDM_EXIT:
                    PostQuitMessage(0);
                    break;

                case IDM_ABOUT:
                    DialogBox(g_hInst,
                        MAKEINTRESOURCE(IDD_ABOUT),
                        hWnd,           
                        (DLGPROC)About);
                    break;

                default:
                    return (DefWindowProc(hWnd, message, wParam, lParam));

            }
            break;

        case WM_DESTROY:                  // message: window being destroyed
            PostQuitMessage(0);
            break;


        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}


//
//   FUNCTION: OpenTheFile(HWND hwnd, HWND hwndEdit)
//
//   PURPOSE: Invokes common dialog function to open a file and opens it.
//
//   COMMENTS:
//
//	This function initializes the OPENFILENAME structure and calls
//            the GetOpenFileName() common dialog function.  
//	
//    RETURN VALUES:
//        TRUE - The file was opened successfully and read into the buffer.
//        FALSE - No files were opened.
//
//
BOOL OpenTheFile( HWND hWnd, HWND hWndEdit )
{
    HANDLE hFile;
    DWORD dwBytesRead;
	DWORD dwFileSize;
	OPENFILENAME OpenFileName;
	TCHAR         szFile[MAX_PATH]      = "\0";
	char *lpBufPtr;

    strcpy( szFile, "");


	// Fill in the OPENFILENAME structure to support a template and hook.
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = hWnd;
    OpenFileName.hInstance         = g_hInst;
    OpenFileName.lpstrFilter       = NULL;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = "Open a File";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.lCustData         = (LPARAM)&sMyData;
	OpenFileName.lpfnHook 		   = ComDlg32DlgProc;
	OpenFileName.lpTemplateName    = MAKEINTRESOURCE(IDD_COMDLG32);
    OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE;

	// Call the common dialog function.
    if (GetOpenFileName(&OpenFileName))
    {
		// Open the file.
		if ((hFile = CreateFile((LPCTSTR)OpenFileName.lpstrFile,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL)) == (HANDLE)-1)
		{
	    	MessageBox( hWnd, "File open failed.", NULL, MB_OK );
	    	return FALSE;
		}

		// Get the size of the file.
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize == 0xFFFFFFFF)
		{
	    	MessageBox( NULL, "GetFileSize failed!", NULL, MB_OK);
	    	return FALSE;
		}

		// Allocate a buffer for the file to be read into.
		lpBufPtr = (char *)GlobalAlloc( GMEM_FIXED, dwFileSize );
		if (lpBufPtr == NULL)
		{
	    	MessageBox( NULL, "GlobalAlloc failed!", NULL, MB_OK);
	    	CloseHandle( hFile );
	    	return FALSE;
		}

		// Read it's contents into a buffer.
		ReadFile(hFile,(LPVOID)lpBufPtr, dwFileSize, &dwBytesRead, NULL);

		if (dwBytesRead == 0)
		{
	    	MessageBox( hWnd, "Zero bytes read.", NULL, MB_OK );
	    		return FALSE;
		}

        // Update the MLE with the file contents.
        SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM) lpBufPtr);

		// Close the file.
		CloseHandle(hFile);

		return TRUE;
    }
    else
   	{
		ProcessCDError(CommDlgExtendedError(), hWnd );
		return FALSE;
    }
	
}

//
//   FUNCTION: TestNotify( HWND hDlg, LPOFNOTIFY pofn)
//
//  PURPOSE:  Processes the WM_NOTIFY message notifications that is sent
//    to the hook dialog procedure for the File Open common dialog.
//
//
BOOL NEAR PASCAL TestNotify(HWND hDlg, LPOFNOTIFY pofn)
{
	switch (pofn->hdr.code)
	{
		// The selection has changed. 
		case CDN_SELCHANGE:
		{
			char szFile[MAX_PATH];

			// Get the file specification from the common dialog.
			if (CommDlg_OpenSave_GetSpec(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Set the dialog item to reflect this.
				SetDlgItemText(hDlg, IDE_SELECTED, szFile);
			}

			// Get the path of the selected file.
			if (CommDlg_OpenSave_GetFilePath(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Display this path in the appropriate box.
				SetDlgItemText(hDlg, IDE_PATH, szFile);
			}
		}
		break;

		// A new folder has been opened.
		case CDN_FOLDERCHANGE:
		{
			char szFile[MAX_PATH];

			if (CommDlg_OpenSave_GetFolderPath(GetParent(hDlg),
				szFile, sizeof(szFile)) <= sizeof(szFile))
			{
				// Display this new path in the appropriate box.
				SetDlgItemText(hDlg, IDE_SELECTED, szFile);
			}
		}
		break;

		// The "Help" pushbutton has been pressed.
		case CDN_HELP:
			MessageBox(hDlg, "Got the Help button notify.", "ComDlg32 Test", MB_OK);
			break;

		// The 'OK' pushbutton has been pressed.
		case CDN_FILEOK:
			// Update the appropriate box.
			SetDlgItemText(hDlg,IDE_SELECTED, pofn->lpOFN->lpstrFile);
			SetWindowLong(hDlg, DWL_MSGRESULT, 1L);
			break;

		// Received a sharing violation.
		case CDN_SHAREVIOLATION:
			// Update the appropriate box.
			SetDlgItemText(hDlg, IDE_SELECTED, pofn->pszFile);
			MessageBox(hDlg, "Got a sharing violation notify.", "ComDlg32 Test", MB_OK);
			break;
	}

	return(TRUE);
}

//
//   FUNCTION: ComDlg32DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//
//  PURPOSE:  Processes messages for the File Open common dialog box.
//
//    MESSAGES:
//
//	WM_INITDIALOG - save pointer to the OPENFILENAME structure in User data
//	WM_DESTROY - get the text entered and fill in the MyData structure
//	WM_NOTIFY - pass this message onto the TestNotify function
//	default - check for a sharing violation or the OK button and
//    	display a message box.
//
//
BOOL CALLBACK ComDlg32DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		case WM_INITDIALOG:
			// Save off the long pointer to the OPENFILENAME structure.
			SetWindowLong(hDlg, DWL_USER, lParam);
			break;

		case WM_DESTROY:
			{
			LPOPENFILENAME lpOFN = (LPOPENFILENAME)GetWindowLong(hDlg, DWL_USER);
			LPMYDATA psMyData = (LPMYDATA)lpOFN->lCustData;

			GetDlgItemText(hDlg, IDE_PATH, psMyData->szTest1, sizeof(psMyData->szTest1));
			GetDlgItemText(hDlg, IDE_SELECTED, psMyData->szTest2, sizeof(psMyData->szTest2));
			}
			break;

		case WM_NOTIFY:
			TestNotify(hDlg, (LPOFNOTIFY)lParam);

		default:
			if (uMsg == cdmsgFileOK)
			{
				SetDlgItemText(hDlg, IDE_SELECTED, ((LPOPENFILENAME)lParam)->lpstrFile);
				if (MessageBox(hDlg, "Got the OK button message.\n\nShould I open it?", "ComDlg32 Test", MB_YESNO)
					== IDNO)
				{
					SetWindowLong(hDlg, DWL_MSGRESULT, 1L);
				}
				break;
			}
			else if (uMsg == cdmsgShareViolation)
			{
				SetDlgItemText(hDlg, IDE_SELECTED, (LPSTR)lParam);
				MessageBox(hDlg, "Got a sharing violation message.", "ComDlg32 Test", MB_OK);
				break;
			}
			return FALSE;
	}
	return TRUE;
}

//
//  FUNCTION: About(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for "About" dialog box
//
//  MESSAGES:
//
//	WM_INITDIALOG - initialize dialog box
//	WM_COMMAND    - Input received
//
//
BOOL APIENTRY About( HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:                
            return (TRUE);

        case WM_COMMAND:                      
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
            {        
                EndDialog(hDlg, TRUE);        
                return TRUE;
            }
            break;
    }
    return FALSE;                           

}

//
//  FUNCTION: ProcessCDError(DWORD) 
//
//  PURPOSE: Processes errors from the common dialog functions.
//
//  COMMENTS:
//
//        This function is called whenever a common dialog function
//        fails.  The CommonDialogExtendedError() value is passed to
//        the function which maps the error value to a string table.
//        The string is loaded and displayed for the user. 
//
//
void ProcessCDError(DWORD dwErrorCode, HWND hWnd)
{
   WORD  wStringID;
   TCHAR  buf[MAX_PATH];

   switch(dwErrorCode)
      {
	 case CDERR_DIALOGFAILURE:   wStringID=IDS_DIALOGFAILURE;   break;
	 case CDERR_STRUCTSIZE:      wStringID=IDS_STRUCTSIZE;      break;
	 case CDERR_INITIALIZATION:  wStringID=IDS_INITIALIZATION;  break;
	 case CDERR_NOTEMPLATE:      wStringID=IDS_NOTEMPLATE;      break;
	 case CDERR_NOHINSTANCE:     wStringID=IDS_NOHINSTANCE;     break;
	 case CDERR_LOADSTRFAILURE:  wStringID=IDS_LOADSTRFAILURE;  break;
	 case CDERR_FINDRESFAILURE:  wStringID=IDS_FINDRESFAILURE;  break;
	 case CDERR_LOADRESFAILURE:  wStringID=IDS_LOADRESFAILURE;  break;
	 case CDERR_LOCKRESFAILURE:  wStringID=IDS_LOCKRESFAILURE;  break;
	 case CDERR_MEMALLOCFAILURE: wStringID=IDS_MEMALLOCFAILURE; break;
	 case CDERR_MEMLOCKFAILURE:  wStringID=IDS_MEMLOCKFAILURE;  break;
	 case CDERR_NOHOOK:          wStringID=IDS_NOHOOK;          break;
	 case PDERR_SETUPFAILURE:    wStringID=IDS_SETUPFAILURE;    break;
	 case PDERR_PARSEFAILURE:    wStringID=IDS_PARSEFAILURE;    break;
	 case PDERR_RETDEFFAILURE:   wStringID=IDS_RETDEFFAILURE;   break;
	 case PDERR_LOADDRVFAILURE:  wStringID=IDS_LOADDRVFAILURE;  break;
	 case PDERR_GETDEVMODEFAIL:  wStringID=IDS_GETDEVMODEFAIL;  break;
	 case PDERR_INITFAILURE:     wStringID=IDS_INITFAILURE;     break;
	 case PDERR_NODEVICES:       wStringID=IDS_NODEVICES;       break;
	 case PDERR_NODEFAULTPRN:    wStringID=IDS_NODEFAULTPRN;    break;
	 case PDERR_DNDMMISMATCH:    wStringID=IDS_DNDMMISMATCH;    break;
	 case PDERR_CREATEICFAILURE: wStringID=IDS_CREATEICFAILURE; break;
	 case PDERR_PRINTERNOTFOUND: wStringID=IDS_PRINTERNOTFOUND; break;
	 case CFERR_NOFONTS:         wStringID=IDS_NOFONTS;         break;
	 case FNERR_SUBCLASSFAILURE: wStringID=IDS_SUBCLASSFAILURE; break;
	 case FNERR_INVALIDFILENAME: wStringID=IDS_INVALIDFILENAME; break;
	 case FNERR_BUFFERTOOSMALL:  wStringID=IDS_BUFFERTOOSMALL;  break;

	 case 0:   //User may have hit CANCEL or we got a *very* random error
	    return;

	 default:
	    wStringID=IDS_UNKNOWNERROR;
      }

   LoadString(NULL, wStringID, buf, sizeof(buf));
   MessageBox(hWnd, buf, NULL, MB_OK);
   return;
}

