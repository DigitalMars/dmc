/**************************************************************************
 *
 *	SOUNDAPP:
 *
 *	A sample app showing the sndPlaySound API
 *
 **************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
#pragma warning(disable: 4001)
#define __SOUNDAPP_C
#define Unused(var)	(var=var)

// header dependancies 
#include <windows.h>
#include <mmsystem.h>
#include <commdlg.h>
#include <windowsx.h>
#include "resource.h"

//message cracker prototypes 
void SoundApp_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);
void SoundApp_OnDestroy(HWND hwnd);
void SoundApp_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL About_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
BOOL About_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

//function prototypes 
long PASCAL WndProc (HWND, UINT, UINT, LONG);
BOOL CALLBACK AboutDlgProc(HWND hdlg, UINT mMsg, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlgProc2(HWND hdlg, UINT mMsg,	WPARAM wParam, LPARAM lParam);

//global variables 
HANDLE hInst;
BOOL bRecursing = FALSE;

////////////////////////////////////////////////////////////////////////////////
//WinMain
//
//Purpose: The main procedure
//
//Parameters: HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, 
//int nCmdShow
//
//Returns: int
////////////////////////////////////////////////////////////////////////////////// 
int PASCAL WinMain (
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, 
	int nCmdShow)

	{
	static char szAppName[] = "SoundApp";
	HWND        hwnd;
	MSG         msg;
	WNDCLASS    wndclass;

	Unused(lpszCmdParam);

	if (!hPrevInstance)
		{
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   = WndProc;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = DLGWINDOWEXTRA;
		wndclass.hInstance     = hInstance;
		wndclass.hIcon         = LoadIcon (hInstance, "IDI_SOUNDAPP");
		wndclass.hCursor       = LoadCursor (NULL, IDI_APPLICATION);
		wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = szAppName;

		RegisterClass (&wndclass);
		}

	hInst = hInstance;
	hwnd = CreateDialog(hInstance, "SoundApp", 0, NULL);
	CreateDialog(hInstance, "ID_SOUNDAPP_ABOUT", hwnd, NULL);
	ShowWindow (hwnd, nCmdShow);

	while (GetMessage (&msg, NULL, 0, 0))
		{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
		}
	return msg.wParam;
	}

/////////////////////////////////////////////////////////////////////////
//WndProc
//
//Purpose: The window procedure
//
//Parameters: HWND hwnd, UINT message, UINT wParam, LONG lParam
//
//Returns: long
///////////////////////////////////////////////////////////////////////////  
long PASCAL WndProc (
	HWND hwnd, 
	UINT message, 
	UINT wParam, 
	LONG lParam)

	{
	switch (message)
		{
		HANDLE_MSG(hwnd, WM_DESTROY, SoundApp_OnDestroy);
		HANDLE_MSG(hwnd, WM_ACTIVATE, SoundApp_OnActivate);
		HANDLE_MSG(hwnd, WM_COMMAND, SoundApp_OnCommand);
		default:
			return DefWindowProc (hwnd, message, wParam, lParam);
		}
	}

//////////////////////////////////////////////////////////////////////
//SoundApp_OnActivate
//
//Purpose: processes a WM_ACTIVATE message
//Initializes controls in the dialog the first time it is activated:
//Checks Synchronous, disables Loop and Stop, and gets default sound
//from win.ini or registry. 
//
//Parameters: HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized
//
//Returns: void
//////////////////////////////////////////////////////////////////////
void SoundApp_OnActivate(
	HWND hwnd,
	UINT state,
	HWND hwndActDeact,
	BOOL fMinimized)

	{
	static BOOL fFirstTime = TRUE;
	HMENU hMenu;
	char SoundBuff[256];
	int i=0;

	if (fFirstTime)
		{
		hMenu = GetMenu(hwnd);
		SendMessage(GetDlgItem(hwnd, IDC_SYNC), BM_SETCHECK, 1, 0);
		CheckMenuItem(hMenu, ID_OPTIONS_SYNC,MF_CHECKED);
		EnableWindow(GetDlgItem(hwnd, IDC_LOOP), FALSE);			
		EnableMenuItem(hMenu, ID_OPTIONS_LOOP, MF_DISABLED | MF_GRAYED);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_STOP), FALSE);
		GetProfileString("Sounds", "SystemDefault", ",Default", SoundBuff, 256);
		//get rid of everything after the filename
		while(SoundBuff[i] != ',' && i<255)
			{
			i++;
			}
		SoundBuff[i] = 0;
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_FILE), WM_SETTEXT, 0, (LPARAM)SoundBuff);
		fFirstTime = FALSE;		
		} 
	}


//////////////////////////////////////////////////////////////////////
//SoundApp_OnDestroy
//
//Purpose: processes a WM_DESTROY message
//
//Parameters: handle to the main window
//
//Returns: void  
//////////////////////////////////////////////////////////////////////
void SoundApp_OnDestroy(HWND hwnd)
	{
	PostQuitMessage (0);
	}


///////////////////////////////////////////////////////////////////////
//SoundApp_OnCommand
//
//Purpose: processes a WM_COMMAND message
//details below for each command.
//
//Parameters: HWND hwnd, int id, HWND hwndCtl, UINT codeNotify
//
//Returns: void  
///////////////////////////////////////////////////////////////////////
void SoundApp_OnCommand(
	HWND hwnd,
	int id,
	HWND hwndCtl,
	UINT codeNotify)

	{
	static LPCTSTR lpszSoundName = NULL;	//name of the sound to play	
	static UINT uFlags = SND_SYNC;			//option flags for sndPlaySound	
	HWND hButton;
	HMENU hMenu;
	LRESULT nCheckState;
	char SoundBuff[256];
	static fFirstTime = TRUE;

	OPENFILENAME ofn;       // common dialog box structure 
	char szDirName[256];    // directory string 
static	char szFile[256];       // filename string 
	char szFileTitle[256];  // file-title string 
	char szFilter[256];     // filter string 
	char chReplace;         // string separator for szFilter 
	int i, cbString;        // integer count variables 
	char szSelectFile[256];	// title of select file dialog 

	//first get default sound name from win.ini or registry 
	if (fFirstTime)
		{
		GetProfileString("Sounds", "SystemDefault", ",Default", SoundBuff, 256);
		//get rid of everything after the filename in the ini string
		i=0;
		while(SoundBuff[i] != ',' && i<255)
			{
			i++;
			}
		SoundBuff[i] = 0;
		if (i == 0)
			{
			lpszSoundName = NULL;
			}
		else
			{
			lpszSoundName = SoundBuff;
			} 
		fFirstTime = FALSE;
		}
	hMenu = GetMenu(hwnd);

	//if File|Exit is selected, quit
	if(id == ID_FILE_EXIT)
		{
		PostQuitMessage (0);
		}

	//if Play button is clicked, play sound
	if(id == IDC_BUTTON_PLAY)
		{   
		sndPlaySound(lpszSoundName, uFlags);
		}

	//if Stop button is pushed, stop playing sound
	if(id == IDC_BUTTON_STOP)
		{
		sndPlaySound(NULL, SND_SYNC);
		}

	//if File|Select File or the Select File button is selected, show the
	//file open dialog and get the name of the file to play
	if(id == ID_FILE_SELECT || id == IDC_BUTTON_OPEN)
		{
		//Set up structure for file dialog
		GetWindowsDirectory(szDirName, sizeof(szDirName));
		szFile[0] = '\0';
		LoadString(NULL, IDS_SELECT_FILE, szSelectFile, sizeof(szSelectFile));
		cbString = LoadString(NULL, IDS_FILTERSTRING, szFilter, sizeof(szFilter));
		chReplace = szFilter[cbString - 1];
		for (i = 0; szFilter[i] != '\0'; i++)
			{
			if (szFilter[i] == chReplace)
				szFilter[i] = '\0';
			}

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = sizeof(szFileTitle);
		ofn.lpstrInitialDir = szDirName;
		ofn.lpstrTitle = szSelectFile;
		ofn.lpstrDefExt = NULL;				
		ofn.Flags = OFN_SHAREAWARE | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//Put up Open File dialog to get filename  
		if (GetOpenFileName(&ofn)) 
			{
			lpszSoundName = ofn.lpstrFile;
			hButton = GetDlgItem(hwnd, IDC_EDIT_FILE);
			SendMessage(hButton, WM_SETTEXT, 0, (LPARAM)ofn.lpstrFileTitle);
			}	   
		}

	//if Help|About is selected, show the About SoundApp dialog
	if(id == ID_HELP_ABOUT)
		{
		FARPROC lpfnAbout;

		lpfnAbout = MakeProcInstance((FARPROC)AboutDlgProc, hInst);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwnd, (DLGPROC)lpfnAbout);
		FreeProcInstance(lpfnAbout);
		}

	//if Options|Sync or the Sync button is selected, set checks and flags
	if(id == IDC_SYNC || id == ID_OPTIONS_SYNC)
		{
		hButton = GetDlgItem(hwnd, IDC_SYNC);
		nCheckState = SendMessage(hButton, BM_GETCHECK, 0, 0);
		//if Synchronous ws unchecked
		if (nCheckState == 0)
			{
			//check Synchronous checkbox, uncheck Asynchronous checkbox,
			//check Synchronous menu, uncheck Asynchronous menu,
			//uncheck Loop checkbox and menu,
			//disable Loop checkbox and menu,
			//disable Stop button, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 1, 0);			   		
			SendMessage(GetDlgItem(hwnd, IDC_ASYNC),BM_SETCHECK, 0, 0);
			CheckMenuItem(hMenu, ID_OPTIONS_SYNC,MF_CHECKED);	   		
			CheckMenuItem(hMenu, ID_OPTIONS_ASYNC, MF_UNCHECKED); 		
			SendMessage(GetDlgItem(hwnd, IDC_LOOP), BM_SETCHECK, 0, 0);	
			CheckMenuItem(hMenu, ID_OPTIONS_LOOP,MF_UNCHECKED);	   		
			EnableWindow(GetDlgItem(hwnd, IDC_LOOP), FALSE);			
			EnableMenuItem(hMenu, ID_OPTIONS_LOOP, MF_DISABLED | MF_GRAYED);
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_STOP), FALSE);		
			uFlags = uFlags & ~SND_ASYNC & ~SND_LOOP;					
			}
		}

	//if Options|Async or the Async button is selected, set checks and flags
	if(id == IDC_ASYNC || id == ID_OPTIONS_ASYNC)
		{
		hButton = GetDlgItem(hwnd, IDC_ASYNC);
		nCheckState = SendMessage(hButton, BM_GETCHECK, 0, 0);
		//if Asynchronous was unchecked
		if (nCheckState == 0)
			{
			//check Asynchronous checkbox, uncheck Synchronous checkbox,
			//check Asynchronous menu, uncheck Synchronous menu,
			//enable Loop checkbox and menu,
			//enable Stop button, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 1, 0);					
			SendMessage(GetDlgItem(hwnd, IDC_SYNC),BM_SETCHECK, 0, 0);
			CheckMenuItem(hMenu, ID_OPTIONS_ASYNC,MF_CHECKED);			 
			CheckMenuItem(hMenu, ID_OPTIONS_SYNC, MF_UNCHECKED); 		
			EnableWindow(GetDlgItem(hwnd, IDC_LOOP), TRUE);				
			EnableMenuItem(hMenu, ID_OPTIONS_LOOP, MF_ENABLED);	 		
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_STOP), TRUE);		
			uFlags = uFlags | SND_ASYNC;								
			}
		}

	//if Options|NoDefault or the NoDefault button is selected, set checks and flags
	if(id == ID_OPTIONS_NODEFAULT || id == IDC_NODEFAULT)
		{ 
		hButton = GetDlgItem(hwnd, IDC_NODEFAULT);
		nCheckState = SendMessage(hButton, BM_GETCHECK, 0, 0);
		//if NoDefault was unchecked
		if (nCheckState == 0)
			{
			//check NoDefault checkbox and menu, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 1, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_NODEFAULT, MF_CHECKED); 	
			uFlags = uFlags | SND_NODEFAULT;
			}
		else
			{
			//uncheck NoDefault checkbox and menu, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 0, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_NODEFAULT, MF_UNCHECKED); 	
			uFlags = uFlags	& ~SND_NODEFAULT;
			}
		}

	//if Options|Loop or the Loop button is selected, set checks and flags
	if(id == ID_OPTIONS_LOOP ||	id == IDC_LOOP)
		{
		hButton = GetDlgItem(hwnd, IDC_LOOP);
		nCheckState = SendMessage(hButton, BM_GETCHECK, 0, 0);
		//if Loop was unchecked
		if (nCheckState == 0)
			{
			//check Loop checkbox and menu, and set sndPlaySound flags.	
			SendMessage(hButton, BM_SETCHECK, 1, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_LOOP,MF_CHECKED);	   	
			uFlags = uFlags | SND_LOOP;
			}
		else
			{
			//uncheck Loop checkbox and menu, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 0, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_LOOP,MF_UNCHECKED);  		
			uFlags = uFlags	& ~SND_LOOP;
			}
		}

	//if Options|NoStop or the NoStop button is selected, set checks and flags
	if(id == ID_OPTIONS_NOSTOP || id == IDC_NOSTOP)
		{
		hButton = GetDlgItem(hwnd, IDC_NOSTOP);
		nCheckState = SendMessage(hButton, BM_GETCHECK, 0, 0);
		//if NoStop was unchecked
		if (nCheckState == 0)
			{
			//check NoStop checkbox and menu, and set sndPlaySound flags. 
			SendMessage(hButton, BM_SETCHECK, 1, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_NOSTOP,MF_CHECKED);	   		
			uFlags = uFlags | SND_NOSTOP;
			}
		else
			{
			//uncheck NoStop checkbox and menu, and set sndPlaySound flags.	
			SendMessage(hButton, BM_SETCHECK, 0, 0);					
			CheckMenuItem(hMenu, ID_OPTIONS_NOSTOP,MF_UNCHECKED);  		
			uFlags = uFlags	& ~SND_NOSTOP;
			}
		}
	}						   
			  
/////////////////////////////////////////////////////////////////////////////
//AboutDlgProc
//
//Purpose: Calls AboutDlgProc2
//This makes it possible to use messge crackers in the dialog procedure
//
//Parameters:HWND hdlg, UINT mMsg, WPARAM wParam, LPARAM lParam
//
//Returns: BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK AboutDlgProc(
	HWND hdlg,
	UINT mMsg,
	WPARAM wParam,
	LPARAM lParam)

	{
	CheckDefDlgRecursion(&bRecursing);
	return(SetDlgMsgResult(hdlg, mMsg, AboutDlgProc2(hdlg, mMsg, wParam, lParam)));
	}

/////////////////////////////////////////////////////////////////////////////
//AboutDlgProc2
//
//Purpose: handle messages to the About dialog
//
//Parameters: HWND hdlg, UINT mMsg, WPARAM wParam, LPARAM lParam
//
//Returns: LRESULT
///////////////////////////////////////////////////////////////////////////////
LRESULT AboutDlgProc2(
	HWND hdlg,
	UINT mMsg,
	WPARAM wParam,
	LPARAM lParam)

    {
	switch(mMsg)
		{
		HANDLE_MSG(hdlg, WM_INITDIALOG, About_OnInitDialog);
		HANDLE_MSG(hdlg, WM_COMMAND, About_OnCommand);
		default:
			return(DefDlgProcEx(hdlg, mMsg, wParam, lParam, &bRecursing));
		}
	}

/////////////////////////////////////////////////////////////////////////
//About_OnInitDialog
//
//Purpose: Initialize About dialog. Set focus to the OK button.
//
//Parameters: HWND hdlg, HWND hwndFocus, LPARAM lParam
//
//Returns: TRUE
//////////////////////////////////////////////////////////////////////
BOOL About_OnInitDialog(HWND hdlg, HWND hwndFocus, LPARAM lParam)
	{
	SetFocus(GetDlgItem(hdlg, IDOK));
	return TRUE;
	}
	
/////////////////////////////////////////////////////////////////////////
//About_OnCommand
//
//Purpose: Closes the About dialog when the OK button is clicked
//
//Parameters: HWND hdlg, int id, HWND hwndCtl, UINT codeNotify
//
//Returns: BOOL
//////////////////////////////////////////////////////////////////////
BOOL About_OnCommand(HWND hdlg, int id, HWND hwndCtl, UINT codeNotify)
	{	
	if(id == IDOK)
		{
		EndDialog(hdlg, TRUE);
		return TRUE;
		}
	return FALSE;
	}
