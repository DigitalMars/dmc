//THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright  1994-1995  Microsoft Corporation.  All Rights Reserved.
//
//	PROGRAM:	Wizard.c
//
//	PURPOSE:	Demonstrates how to manipulate a wizard control
//
//	PLATFORMS:	Windows 95
//
//	FUNCTIONS:	
//		WinMain() - calls initialization function, processes message loop
//		InitApplication() - Initializes window data nd registers window
//		InitInstance() -saves instance handle and creates main window
//		MainWindProc() Processes messages
//		About() - Process messages for "About" dialog box
//		YourInfo() - Process the "Your Information" Wizard page
//		WorkHabits() - Processes the "Work Habits" Wizard page
//		TeamWork() - Processes the "Team Work" Wizard page
//		Reliability() - Processes the "Reliability" Wizard page
//		Goals() - Processes the "Goal Attainment" Wizard page
// 		Adaptation() - Processes the "Adaptability to Change" Wizard page
//		FillInPropertyPage() - Fills in a PROPSHEETPAGE structure
//		CreateWizard() - Creates the wizard
//		GenerateReview() - Generates the resulting review
//
//	SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>    // includes basic windows functionality
#include <string.h>     // includes the string functions
#include <prsht.h>      // includes the property sheet functionality
#include "resource.h"   // includes the definitions for the resources
#include "wizard.h"     // includes the application-specific information

REVIEWINFO rvInfo;      // a structure containing the review information
HWND hwndEdit;          // handle to the main MLE
TCHAR lpReview[MAX_BUF]; // Buffer for the review

//
//
//   FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
//
//   PURPOSE: Main entry point for the application. 
//
//   COMMENTS:
//	
//	This function calls the initialization functions and processes
//	the main message loop.
// 
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	MSG msg;                       

      // save off the current instance
	rvInfo.hInst = hInstance;

      // if the initialization fails, return.
	if (!InitApplication(hInstance))
		return (FALSE);     

	// Perform initializations that apply to a specific instance 
	if (!InitInstance(hInstance, nCmdShow))
		return (FALSE);

	// Acquire and dispatch messages until a WM_QUIT message is received. 
	while (GetMessage(&msg,
		NULL,              
		0,                 
		0))                
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return (msg.wParam);  
												 
}


//
//
//   FUNCTION: InitApplication(HANDLE) 
//
//   PURPOSE: Initializes window data and registers window class 
//
//   COMMENTS:
//
//		This function registers the window class for the main window.	
// 
BOOL InitApplication(HANDLE hInstance)
{
        WNDCLASS  wcSample;
	
	// Fill in window class structure with parameters that describe the       
	// main window.                                                           

        wcSample.style = 0;                     
        wcSample.lpfnWndProc = (WNDPROC)MainWndProc; 
        wcSample.cbClsExtra = 0;              
        wcSample.cbWndExtra = 0;              
        wcSample.hInstance = hInstance;       
        wcSample.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(EXE_ICON));
        wcSample.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcSample.hbrBackground = GetStockObject(WHITE_BRUSH); 
        wcSample.lpszMenuName =  TEXT("WizardMenu");  
        wcSample.lpszClassName = TEXT("SampleWClass");

        return (RegisterClass(&wcSample));

}


//
//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Creates the main window.
//
//   COMMENTS: N/A
//	
// 
BOOL InitInstance(
	HANDLE          hInstance,
	int             nCmdShow) 
{
	HWND hWndMain;

	hWndMain = CreateWindow(
                TEXT("SampleWClass"),
                TEXT("Wizard Sample"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,               
		NULL,               
		hInstance,          
		NULL);

	/* If window could not be created, return "failure" */
	if (!hWndMain)
		return (FALSE);

	/* Make the window visible; update its client area; and return "success" */
	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain); 
	return (TRUE);      

}

//
//   FUNCTION: MainWndProc(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for the main window procedure 
//
//    MESSAGES:
//	
//	WM_CREATE - creates the main MLE for the window
//	WM_COMMAND - processes the menu commands for the application
//	WM_SIZE - sizes the MLE to fill the client area of the window
//	WM_DESTROY - posts a quit message and returns
//
LONG APIENTRY MainWndProc(
	HWND hWnd,                // window handle                   
	UINT message,             // type of message                 
	UINT wParam,              // additional information          
	LONG lParam)              // additional information          
{

	switch (message) 
	{
        case WM_CREATE:
			// Create an MLE for the file contents.
			hwndEdit = CreateWindow(
                TEXT("EDIT"),     
				NULL,       
                WS_CHILD | WS_VISIBLE | ES_WANTRETURN |
                    ES_MULTILINE | ES_AUTOVSCROLL,
                0, 0, 0, 0, 
                hWnd,       
                (HMENU) ID_EDITCHILD, 
                (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
                NULL);               

            // Update the MLE. 
            SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) "Choose Performance Review Wizard from the Options menu.");
            return 0;

        case WM_SIZE:
            // Make the edit control the size of the window's client area. 
            MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);          
            return 0;

        case WM_COMMAND:
            switch( LOWORD( wParam ))
			{
                case IDM_WIZARD:
                    CreateWizard(hWnd, rvInfo.hInst);

					// Update the MLE. 
				    if (SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) lpReview) != TRUE)
						MessageBox( hWnd, TEXT("Text not set!"), NULL, MB_OK);

					break;

				case IDM_EXIT:
					PostQuitMessage(0);
					break;

				case IDM_ABOUT:
					DialogBox(rvInfo.hInst, TEXT("AboutBox"), hWnd, (DLGPROC)About);
					break;

				default:
					return (DefWindowProc(hWnd, message, wParam, lParam));

		}
		break;

		case WM_DESTROY:                  /* message: window being destroyed */
			PostQuitMessage(0);
			break;

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}


//
//  FUNCTION: About(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for the "About" dialog box 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - initalizes the dialog box
//	WM_COMMAND - processes the input
//	
BOOL APIENTRY About(
   HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{
	switch (message)
	{
	   case WM_INITDIALOG:
		  return TRUE;

	   case WM_COMMAND:              
		  if (LOWORD(wParam) == IDOK)
		 {
			  EndDialog(hDlg, TRUE);
			  return TRUE;
		  }
		  break;

		default:
			return FALSE;
	}

}

//
//  FUNCTION: YourInfo(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Your Information" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//
BOOL APIENTRY YourInfo(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			lstrcpy(rvInfo.pszName, TEXT(""));
			lstrcpy(rvInfo.pszTitle, TEXT(""));
			lstrcpy(rvInfo.pszProject, TEXT(""));
			lstrcpy(rvInfo.pszDepartment, TEXT(""));
			
			break;

		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{

  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// reset to the original values
					lstrcpy(rvInfo.pszName, TEXT(""));
					lstrcpy(rvInfo.pszTitle,TEXT(""));
					lstrcpy(rvInfo.pszProject, TEXT(""));
					lstrcpy(rvInfo.pszDepartment, TEXT(""));
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
	    			PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_NEXT);
					SendMessage(GetDlgItem(hDlg,0x3024 ), BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, MAKELONG(FALSE, 0));
					//SendMessage(GetParent(hDlg), DM_SETDEFID, (WPARAM)IDC_BUTTON1, 0);
					SendMessage(GetDlgItem(hDlg, IDE_NAME), WM_SETTEXT, 0, (LPARAM)rvInfo.pszName);
					SendMessage(GetDlgItem(hDlg, IDE_TITLE), WM_SETTEXT, 0, (LPARAM)rvInfo.pszTitle);
					SendMessage(GetDlgItem(hDlg, IDE_PROJECT), WM_SETTEXT, 0, (LPARAM)rvInfo.pszProject);
					SendMessage(GetDlgItem(hDlg, IDE_DEPARTMENT), WM_SETTEXT, 0, (LPARAM)rvInfo.pszDepartment);
					break;

                case PSN_WIZNEXT:
					// the Next button was pressed
		 			SendDlgItemMessage(hDlg, IDE_NAME, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM) rvInfo.pszName);
					SendDlgItemMessage(hDlg, IDE_TITLE, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)rvInfo.pszTitle);
					SendDlgItemMessage(hDlg, IDE_PROJECT, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)rvInfo.pszProject);
					SendDlgItemMessage(hDlg, IDE_DEPARTMENT, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)rvInfo.pszDepartment);
     				break;

				default:
					return FALSE;

    	}
		break;

		default:
			return FALSE;
	}
	return TRUE;   
}

//
//  FUNCTION: WorkHabits(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Work Habits" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//	WM_COMMAND - saves the id of the choice selected
//
BOOL APIENTRY WorkHabits(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			rvInfo.iWorkHabits = 0;
			break;

		case WM_COMMAND:
			if (HIWORD(lParam) == BN_CLICKED)
				rvInfo.iWorkHabits = LOWORD(wParam);
			break;				

		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{

  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// rest to the original values
					rvInfo.iWorkHabits = 0;
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
	    			PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_NEXT | PSWIZB_BACK);
					if (rvInfo.iWorkHabits)
						SendMessage(GetDlgItem(hDlg, rvInfo.iWorkHabits), BM_SETCHECK, 1, 0L);
					break;

                case PSN_WIZNEXT:
					// get the selected radio button
     				break;

				default:
					return FALSE;

    	}
		break;

		default:
			return FALSE;
	}
	return TRUE;   
}

//
//  FUNCTION: TeamWork(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Team Work" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//	WM_COMMAND - saves the id of the choice selected
//
//
BOOL APIENTRY TeamWork(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			rvInfo.iTeamWork = 0;
			break;

		case WM_COMMAND:
			if (HIWORD(lParam) == BN_CLICKED)
				rvInfo.iTeamWork = LOWORD(wParam);
			break;				
		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{

  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// rest to the original values
					rvInfo.iTeamWork = 0;
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
					if (rvInfo.iTeamWork)
						SendMessage(GetDlgItem(hDlg, rvInfo.iTeamWork), BM_SETCHECK, 1, 0L);
					PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_NEXT);
					break;

                case PSN_WIZBACK:
                    break;

                case PSN_WIZNEXT:
                    break;

				default:
					return FALSE;

    	}
		break;

		default:
			return FALSE;

	}
	return TRUE;   
}
//
//  FUNCTION: Reliability(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Reliability" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//	WM_COMMAND - saves the id of the choice selected
//
BOOL APIENTRY Reliability(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			rvInfo.iReliability = 0;
			break;

		case WM_COMMAND:
			if (HIWORD(lParam) == BN_CLICKED)
				rvInfo.iReliability = LOWORD(wParam);
			break;				

		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{

  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// rest to the original values
					rvInfo.iReliability = 0;
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
					if (rvInfo.iReliability)
						SendMessage(GetDlgItem(hDlg, rvInfo.iReliability), BM_SETCHECK, 1, 0L);
					PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_NEXT);
					break;

                case PSN_WIZBACK:
                    break;

                case PSN_WIZNEXT:
                    break;
				
				default:
					return FALSE;
    	}
		break;
		
		default:
			return FALSE;
	}
	return TRUE;   
}
//
//  FUNCTION: Goals(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Goal Attainment" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//	WM_COMMAND - saves the id of the choice selected
//
BOOL APIENTRY Goals(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			rvInfo.iGoals = 0;
			break;

		case WM_COMMAND:
			if (HIWORD(lParam) == BN_CLICKED)
				rvInfo.iGoals = LOWORD(wParam);
			break;	
						
		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{

  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// rest to the original values
					rvInfo.iGoals = 0;
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
					if (rvInfo.iGoals)
						SendMessage(GetDlgItem(hDlg, rvInfo.iGoals), BM_SETCHECK, 1, 0L);
 					PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_NEXT);
					break;

                case PSN_WIZBACK:
                    break;

                case PSN_WIZNEXT:
                    break;

				default:
					return FALSE;

    	}
		break;

		default:
			return FALSE;
	}
	return TRUE;   
}
//
//  FUNCTION: Adaptation(HWND, UINT, UINT, LONG)
//
//  PURPOSE:  Processes messages for "Addaptability to Change" page 
//
//  MESSAGES:
//	
//	WM_INITDIALOG - intializes the page
//	WM_NOTIFY - processes the notifications sent to the page
//	WM_COMMAND - saves the id of the choice selected
//
//
BOOL APIENTRY Adaptation(
	HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{

	switch (message)
	{
		case WM_INITDIALOG:
			rvInfo.iAdaptation = 0;
			break;

		case WM_COMMAND:
			if (HIWORD(lParam) == BN_CLICKED)
				rvInfo.iAdaptation = LOWORD(wParam);
			break;				

		case WM_NOTIFY:
    		switch (((NMHDR FAR *) lParam)->code) 
    		{
  				case PSN_KILLACTIVE:
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					return 1;
					break;

				case PSN_RESET:
					// rest to the original values
					rvInfo.iAdaptation = 0;
	           		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
					break;

 				case PSN_SETACTIVE:
					if (rvInfo.iAdaptation)
						SendMessage(GetDlgItem(hDlg, rvInfo.iAdaptation), BM_SETCHECK, 1, 0L);
					PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_FINISH);
					break;

                case PSN_WIZBACK:
                    break;


                case PSN_WIZFINISH:
					GenerateReview(hDlg);
                    break;

				default:
					return FALSE;
    	}
		break;

		default:
			return FALSE;
	}
	return TRUE;   
}

//
//
//  FUNCTION: FillInPropertyPage(PROPSHEETPAGE *, int, LPSTR, LPFN) 
//
//  PURPOSE: Fills in the given PROPSHEETPAGE structure 
//
//  COMMENTS:
//
//      This function fills in a PROPSHEETPAGE structure with the
//      information the system needs to create the page.
// 
void FillInPropertyPage( PROPSHEETPAGE* psp, int idDlg, LPSTR pszProc, DLGPROC pfnDlgProc)
{
    psp->dwSize = sizeof(PROPSHEETPAGE);
    psp->dwFlags = 0;
    psp->hInstance = rvInfo.hInst;
    psp->pszTemplate = MAKEINTRESOURCE(idDlg);
    psp->pszIcon = NULL;
    psp->pfnDlgProc = pfnDlgProc;
    psp->pszTitle = pszProc;
    psp->lParam = 0;

}

//
//
//    FUNCTION: CreateWizard(HWND)
//
//    PURPOSE: Create the Wizard control. 
//
//   COMMENTS:
//	
//      This function creates the wizard property sheet.
//
int CreateWizard(HWND hwndOwner, HINSTANCE hInst)
{
    PROPSHEETPAGE psp[NUM_PAGES];
    PROPSHEETHEADER psh;

	FillInPropertyPage( &psp[0], IDD_INFO, TEXT("Your Information"), YourInfo);
	FillInPropertyPage( &psp[1], IDD_WORKHABITS, TEXT("Work Habits"), WorkHabits);
	FillInPropertyPage( &psp[2], IDD_TEAMWORK, TEXT("Team Work"), TeamWork);
	FillInPropertyPage( &psp[3], IDD_RELIABILITY, TEXT("Reliability"), Reliability);
	FillInPropertyPage( &psp[4], IDD_GOALS, TEXT("Attainment of Goals"), Goals);
	FillInPropertyPage( &psp[5], IDD_ADAPTATION, TEXT("Adaptability to Change"), Adaptation);
    
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_WIZARD | PSH_NOAPPLYNOW;
    psh.hwndParent = hwndOwner;
    psh.pszCaption = (LPSTR) TEXT("Review Wizard");
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;

    return (PropertySheet(&psh));
}

//
//
//    FUNCTION: GenerateReview(void)
//
//    PURPOSE: Generate the review. 
//
//   COMMENTS:
//	
//      This function generates the review based upon the answers
//      given by the Wizard. The function translates lame reality into
//      impressive-sounding manager-speak via a string table.
//
void GenerateReview( HWND hDlg )
{
    TCHAR lpBuf1[MAX_LINE];  // Buffers for the lines in the review.
    TCHAR lpBuf2[MAX_LINE];
    TCHAR lpBuf3[MAX_LINE];
    TCHAR lpBuf4[MAX_LINE];
    TCHAR lpBuf5[MAX_LINE];


	wsprintf(lpReview, TEXT("Name: %s%C%C%C%CTitle: %s%C%C%C%CDepartment: %s%C%C%C%CMain Project: %s%C%C%C%C"),
		rvInfo.pszName, 0x0d, 0x0a, 0x0d, 0x0a, 
		rvInfo.pszTitle, 0x0d, 0x0a, 0x0d, 0x0a, 
		rvInfo.pszDepartment, 0x0d, 0x0a, 0x0d, 0x0a, 
		rvInfo.pszProject,0x0d, 0x0a, 0x0d, 0x0a );

	// Add a line describing work habits
	if (LoadString(rvInfo.hInst, rvInfo.iWorkHabits, lpBuf1, sizeof(lpBuf1)) == 0)
		MessageBox(hDlg, TEXT("Error loading string!"), NULL, MB_OK);
	else
		lstrcat(lpReview, lpBuf1);

	// Add a line describing team work
	if (LoadString(rvInfo.hInst, rvInfo.iTeamWork, lpBuf2, sizeof(lpBuf2)) == 0)
		MessageBox(hDlg, TEXT("Error loading string!"), NULL, MB_OK);
	else
		lstrcat(lpReview, lpBuf2);

	// Add a line describing reliability
	if (LoadString(rvInfo.hInst, rvInfo.iReliability, lpBuf3, sizeof(lpBuf3)) == 0)
		MessageBox(hDlg, TEXT("Error loading string!"), NULL, MB_OK);
	else
		lstrcat(lpReview, lpBuf3);

	// Add a line describing goals
	if (LoadString(rvInfo.hInst, rvInfo.iGoals, lpBuf4, sizeof(lpBuf4)) == 0)
		MessageBox(hDlg, TEXT("Error loading string!"), NULL, MB_OK);
	else
		lstrcat(lpReview, lpBuf4);

	// Add a line describing adaptability
	if (LoadString(rvInfo.hInst, rvInfo.iAdaptation, lpBuf5, sizeof(lpBuf5)) == 0)
		MessageBox(hDlg, TEXT("Error loading string!"), NULL, MB_OK);
	else
		lstrcat(lpReview, lpBuf5);

   	
}
