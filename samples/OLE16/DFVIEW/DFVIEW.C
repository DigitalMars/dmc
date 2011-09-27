/*************************************************************************
**
**     OLE 2 DFVIEW DocFile Viewer Sample Code
**
**     dfview.c
**
**     Demonstrates reading, writing, and enumerating OLE 2.0 DocFiles
**     (OLE 2.0 implementation of IStorage/IStream interfaces).
**     The contents of the docfile are displayed in a owner-draw
**     expandable listbox, similar to Windows File Manager.
**
**    (c) Copyright Microsoft Corp. 1993 All Rights Reserved
**
*************************************************************************/

#include "precomp.h"
#include <dlgs.h>
#include "dfview.h"
#include "resource.h"
#include "foldlist.h"

// This line is needed for the debug utilities in OLE2UI
OLEDBGDATA_MAIN("DFVIEW")

/********************** Locally Used "Globals" *************************/

// Version string to be placed in ABOUT box
char szVersion[] = "Version 1.1";
char szAppName[] = "OLE 2.0 DocFile Viewer";

WORD wFileOKMsg;                // Registered FILEOKSTRING
int giWindowWidth;              // Saved Window Width
int giWindowHeight;             // Saved Window Height
char buf[MAXFILENAMELEN];       // scratch buffer
HANDLE ghInst;                  // Handle to our instance
HANDLE ghFoldList;              // Handle to listbox -- will be initialized by DFLISTB.C
HWND   ghMainWnd;               // Handle to main window

char szClass[] = "DFViewClass"; // Class Name
char szIni[] = "DFVIEW.INI";    // Ini file to save position in

// The bitmap handles are used to create our owner-draw listbox.  They
// are created and deleted by DFLISTB.C, we just use them in this module

extern HBITMAP ghbmpFolders;    // Folder's bitmap
extern HBITMAP ghbmpBullets;    // 3-D Bullet bitmap
extern HBITMAP ghbmpWindows;    // Window guy bitmap
extern HBITMAP ghbmpBlanks;
extern HBITMAP ghbmpDocuments;


/****************************************************************************
**
**    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
**
****************************************************************************/

int _pascal WinMain(HANDLE hInstance, HANDLE hPrevInstance,
					LPSTR lpCmdLine,  int nCmdShow)
	{
	MSG msg;
	BOOL bFirstInstance = FALSE; // TRUE if we are the first instance
	int iExit = 0;               // Default is error exit code

	if (hPrevInstance == 0)
		{
		if (InitApplication(hInstance) == 0)
			{
			OleDbgOut("Could not initialize application\n");
			goto Error3Exit;
			}
		bFirstInstance = TRUE;
		}

	Ctl3dRegister(hInstance);
	Ctl3dAutoSubclass(hInstance);

	// Initialize our FoldList.  Pass in the whether or not we are
	// the first instance.  If so, we register our private window
	// class for the owner-draw list box.  If we are not the first
	// instance, the listbox class will already have been
	// registered
	FLInit (hInstance, BMPSIZEX, BMPSIZEY, bFirstInstance);

	if (InitInstance(hInstance, nCmdShow) == 0)
		{
		OleDbgOut("Could not initialize instance\n");
		goto Error2Exit; // Exit with error
		}

	// Initialization required for Component-Object model functions
	// (Like IStorage).  We do not need to call OleInitialize because
	// we aren't using any IOle* methods
	if (FAILED(CoInitialize(NULL)))
		{
		OleDbgOut("Could not initialize OLE2 libraries\n");
		goto Error2Exit;
		}

	/* Initialization required for OLE 2 UI library.  This call is
	**    needed ONLY if we are using the static link version of the UI
	**    library. If we are using the DLL version, we should NOT call
	**    this function in our application.
	*/
	if (!OleUIInitialize(hInstance, hPrevInstance))
		{
		OleDbgOut("Could not initialize OLEUI library\n");
		goto Error1Exit;
		}

	while (GetMessage(&msg, 0, 0, 0) != 0)
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	iExit = msg.wParam;

	// De-initialization for UI libraries.  Just like OleUIInitialize, this
	// funciton is needed ONLY if we are using the static link version of the
	// OLE UI library.
	OleUIUninitialize();

Error1Exit:
	// De-initialization for CO
	CoUninitialize();

Error2Exit:
	Ctl3dUnregister(hInstance);

Error3Exit:
	return (iExit);
	}


/****************************************************************************
**
**    FUNCTION: InitApplication(HANDLE)
**
*****************************************************************************/

static BOOL InitApplication(HANDLE hInstance)
	{
	WNDCLASS  wc;


	wc.style          = 0;
	wc.lpfnWndProc    = MainWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = hInstance;
	wc.hIcon          = LoadIcon(hInstance, "DFVIEW");
	wc.hCursor        = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground  = GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName   = "SAMPLEMENU";
	wc.lpszClassName  = szClass;

	return (RegisterClass(&wc));
	}


/****************************************************************************
**
**    FUNCTION:  InitInstance(HANDLE, int)
**
****************************************************************************/

static BOOL InitInstance(HANDLE hInstance, int nCmdShow)
	{

	ghInst = hInstance;

	// Get Previous window size information from INI file
	GetPositionInfo();

	ghMainWnd = CreateWindow(
			 szClass,
			 szAppName,
			 WS_OVERLAPPEDWINDOW,
			 CW_USEDEFAULT,
			 CW_USEDEFAULT,
			 giWindowWidth,
			 giWindowHeight,
			 0,
			 0,
			 hInstance,
			 0 );

	if (ghMainWnd == 0 )
		return ( FALSE );

	ShowWindow(ghMainWnd, nCmdShow);
	UpdateWindow(ghMainWnd);
	return (TRUE);
	}


/****************************************************************************
**
**    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)
**
****************************************************************************/

long _far _pascal __export MainWndProc(HWND hWnd, UINT message,
							  WPARAM wParam, LPARAM lParam)
{
	switch ( message )
		{
		case WM_CREATE:
			// Register message used in OpenHook to detect when user
			// has hit "OK" in the FileOpen dialog
			wFileOKMsg = RegisterWindowMessage((LPSTR)FILEOKSTRING);

			// Create our owner-draw listbox. See DFLISTB.C,
			return DFListBoxCreate (hWnd, 4, 0);
			break;

		case WM_SIZE:
			// Call routine which will size our owner-draw listbox to
			// cover client area
			DFListBoxSize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_COMMAND:
			switch ( wParam )
				{
				// File.About -- display about box
				case ID_FILE_ABOUT:
					DialogBox(ghInst, "ABOUTBOX", hWnd, About);
					break;

				// File.Create Doc file -- make new file
				case ID_FILE_CREATEDOCFILE:
					MakeDocFile(hWnd);
					break;

				// File.Exit -- exit app
				case ID_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				// File.Close -- close current file.  It really is already
				// closed, but this is in keeping with a standard UI.
				case ID_FILE_CLOSE:
					ClearOut(); // Clear list box
					break;

				// File.Open -- brings up FileOpen dialog then reads
				// docfile.
				case ID_FILE_READ:
					ReadDocFile(hWnd);
					break;

				// Help.Help -- bring up help dialog
				case ID_HELP_HELP:
					DialogBox(ghInst, "HELP", hWnd, HelpProc);
					break;

				// List.ExpandAll/CollapseAll -- send our owner-draw
				// listbox a special message that will do this for us
				case ID_LIST_EXPANDALL:
				case ID_LIST_COLLAPSEALL:
					SendMessage (ghFoldList,
						 wParam == ID_LIST_EXPANDALL ? WM_FLEXPANDALL : WM_FLCOLLAPSEALL, 0, 0L);
					break;
				}
			break;

		case WM_CLOSE:
			SavePositionInfo();
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_NCDESTROY:
			DFListBoxCleanup();
			break;

		case WM_SYSCOLORCHANGE:
			Ctl3dColorChange(); // Used for 3-D coloring of dialogs
			break;


		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	 }

	return (FALSE);
	}


/***************************************************************************
**
** MakeDocFile()
**
** Creates a new docfile called test.dfl.
**
** NOTE: This function does not correctly recover from error conditions
** in some cases.
**
****************************************************************************/

static int _pascal MakeDocFile( HWND hWnd )
{
  LPSTORAGE lpRootStg;
  LPSTORAGE lpStorage2;
  SCODE sc;
  LPSTR lpSlash;

  // flags for creating sub storages and streams.  Open in direct mode so
  // we don't have to committ each write.  Since we're opening in DIRECT
  // and WRITE modes, we must also specify SHARE_EXCLUSIVE, or the call to
  // create sub storages and streams will fail.

  DWORD dwCScode = STGM_WRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
  LPSTREAM lpStream;

  // NOTE: Strings stored in a stringtable resource take up less DGROUP
  // space for an application.
  char name1[] = "Document Name";
   char data1[] = "Seattle Mariners Baseball";

  char name2[] = "Coach Listing";
   char data2[] = "Lou Pinella, Manager\nKen Griffey Sr., Hitting Coach\n";

  char name3[] = "Player Roster";

   char name4[] = "Ken Griffey Jr.";
	char data4[] = "Center Field";

   char name5[] = "Jay Buhner";
	char data5[] = "Right Field";

   char name6[] = "Dave Valle";
	char data6[] = "Catcher";

   char name7[] = "Randy Johnson";
	char data7[] = "Pitcher";

  ULONG cbWritten;
  HCURSOR hc;

  hc = SetCursor(LoadCursor(NULL, IDC_WAIT));

  // Create a new docfile in the same directory as our EXE, wiping over
  // any one that currently exists with this name.

  GetModuleFileName(ghInst, buf, MAXFILENAMELEN);
  lpSlash = _fstrrchr(buf, '\\');   // Find last slash in name
  if (lpSlash) *(lpSlash+1) = '\0'; // Null term string after slash
  _fstrcat(buf, "TEST.DFL");        // And replace EXE name with TEST.DFL

  // Create New docfile.  Since we didn't specify STGM_FAILIFTHERE, this
  // funciton will overwrite any existing file.  We specify transacted
  // mode here, so we need to remember to commit our changes at the end
  // of using this root IStorage.

  sc = GetScode(StgCreateDocfile(buf, STGM_WRITE | STGM_SHARE_DENY_WRITE |
						STGM_TRANSACTED | STGM_CREATE, 0, &lpRootStg));

  if (FAILED(sc)) {
	OleDbgOutScode("StgCreateDocfile failed, code = ", sc);
	MessageBox(ghMainWnd, "StgCreateDocfile failed.", szAppName, MB_ICONEXCLAMATION);
	SetCursor(hc);
	return FALSE;
	}

  // Create stream #1 with the IStorage::CreateStream() function and write data to it
  sc = GetScode(lpRootStg->lpVtbl->CreateStream(lpRootStg, name1, dwCScode, 0, 0, &lpStream));
  if (FAILED(sc)) goto ErrorStg;
  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data1, sizeof(data1), &cbWritten));
  if (FAILED(sc)) goto ErrorStream;
  // And release the new stream since we are finished with it
  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

  // Create stream #2
  sc = GetScode(lpRootStg->lpVtbl->CreateStream(lpRootStg, name2, dwCScode, 0, 0, &lpStream));
  if (FAILED(sc)) goto ErrorStg;
  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data2, sizeof(data2), &cbWritten));
  if (FAILED(sc)) goto ErrorStream;
  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

  // Create child storage.  We use the IStorage::CreateStorage() function to do this.
  sc = GetScode(lpRootStg->lpVtbl->CreateStorage(lpRootStg, name3, dwCScode, 0, 0, &lpStorage2));
  if (FAILED(sc)) goto ErrorStg;

	  // Create stream #1 in SECOND STORAGE
	  sc = GetScode(lpStorage2->lpVtbl->CreateStream(lpStorage2, name4, dwCScode, 0, 0, &lpStream));
	  if (FAILED(sc)) goto ErrorStg;
	  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data4, sizeof(data4), &cbWritten));
	  if (FAILED(sc)) goto ErrorStream;
	  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

	  // Create stream #2 in SECOND STORAGE
	  sc = GetScode(lpStorage2->lpVtbl->CreateStream(lpStorage2, name5, dwCScode, 0, 0, &lpStream));
	  if (FAILED(sc)) goto ErrorStg;
	  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data5, sizeof(data5), &cbWritten));
	  if (FAILED(sc)) goto ErrorStream;
	  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

	  // Create stream #3 in SECOND STORAGE
	  sc = GetScode(lpStorage2->lpVtbl->CreateStream(lpStorage2, name6, dwCScode, 0, 0, &lpStream));
	  if (FAILED(sc)) goto ErrorStg;
	  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data6, sizeof(data6), &cbWritten));
	  if (FAILED(sc)) goto ErrorStream;
	  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

	  // Create stream #4 in SECOND STORAGE
	  sc = GetScode(lpStorage2->lpVtbl->CreateStream(lpStorage2, name7, dwCScode, 0, 0, &lpStream));
	  if (FAILED(sc)) goto ErrorStg;
	  sc = GetScode(lpStream->lpVtbl->Write(lpStream, data7, sizeof(data7), &cbWritten));
	  if (FAILED(sc)) goto ErrorStream;
	  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);

  // Now, we're completed with the second storage.  Release the pointer.
  OleStdVerifyRelease((LPUNKNOWN)lpStorage2, NULL);

  // And finally, we're done with our root storage.  Before we release it, we
  // must commit the data since we opened the root in Transacted mode
  OleStdCommitStorage(lpRootStg);
  OleStdVerifyRelease((LPUNKNOWN)lpRootStg, NULL);

  SetCursor(hc);
  return (TRUE);

ErrorStream:
  OleDbgOutScode("Stream Error, code = ", sc);
  OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);
ErrorStg:
  OleDbgOutScode("Storage Error, code = ", sc);
  OleStdVerifyRelease((LPUNKNOWN)lpRootStg, NULL);
  SetCursor(hc);
  return (FALSE);
}

/****************************************************************************
**
** OpenHook() -- hook for FileOpen
**
** Used to trap when the "OK" button is pressed.  We can they try opening
** up the docfile to see if it's valid.  If not, then we report the error
** before the GetOpenFileName() dialog is dismissed.
**
****************************************************************************/

UINT CALLBACK __export OpenHook(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{

  // Check if message equals the "user pressed FileOK" message, which we
  // registered earlier.  Can't use switch because wFileOKMsg is not const.

  if (wm == wFileOKMsg)
	{
	char fn[MAXFILENAMELEN];
	LPSTORAGE lpRootStg;
	SCODE sc;

	// Edit control 'edt1' contains the current filename
	GetDlgItemText(hwnd, edt1, fn, 255);

	// Try opening file
	sc = GetScode(StgOpenStorage(fn, NULL, STGM_READ | STGM_SHARE_DENY_WRITE |
						STGM_DIRECT, NULL, 0, &lpRootStg));

	if (FAILED(sc))
	  {
	  char buf [MAXFILENAMELEN];
	  wsprintf (buf, "Unable to open %s.", (LPSTR)fn);
	  OleDbgOutScode("Unable to open file, ret = ", sc);
	  MessageBox (hwnd, buf, szAppName, MB_ICONSTOP);
	  return TRUE; // Dialog will not end
	  }

	// Otherwise, open worked OK.  We'll just close the file and continue on.
	lpRootStg->lpVtbl->Release(lpRootStg);
	return FALSE; // so dialog will end
	}

	else
	  switch (wm)
		{
		case WM_INITDIALOG:
		  return TRUE; // need to return TRUE because we did not set focus
		  break;
		}

  return FALSE;
}

/****************************************************************************
**
** ReadDocFile()
**
** Prompts for and reads the selected DocFile.
**
*****************************************************************************/

static int _pascal ReadDocFile( HWND hWnd )
{
  LPSTORAGE lpRootStg;
  SCODE sc;
  int iID;
  OPENFILENAME   of;
  char szFile[MAXFILENAMELEN];
  HCURSOR hc = NULL;
  char szFilter[] = "All Files (*.*)\0*.*\0Doc Files (*.DFL; *.OL?)\0*.DFL;*.OL?\0";

  // Initialize the OPENFILENAME members
  lstrcpy(szFile, "*.*");

  of.lStructSize       = sizeof (OPENFILENAME);
  of.hwndOwner         = hWnd;
  of.hInstance         = ghInst;
  of.lpstrFilter       = (LPSTR)szFilter;
  of.lpstrCustomFilter = NULL;
  of.nMaxCustFilter    = 0L;
  of.nFilterIndex      = 1L;
  of.lpstrFile         = (LPSTR)szFile;
  of.nMaxFile          = MAXFILENAMELEN;
  of.lpstrFileTitle    = NULL;
  of.nMaxFileTitle     = 0;
  of.lpstrInitialDir   = NULL;
  of.lpstrTitle        = "Open Docfile To View";
  of.Flags             = OFN_HIDEREADONLY | OFN_ENABLEHOOK;
  of.nFileOffset       = 0;
  of.nFileExtension    = 0;
  of.lpstrDefExt       = "dfl";
  of.lpfnHook          = OpenHook;
  of.lpTemplateName    = NULL;
  of.lCustData         = 0;

  if (GetOpenFileName (&of))
	{

	hc = SetCursor(LoadCursor(NULL, IDC_WAIT));
	sc = GetScode(StgOpenStorage(szFile, NULL, STGM_READ | STGM_SHARE_DENY_WRITE, NULL, 0, &lpRootStg));

	// At this point, we've already successfully opened the file, but we
	// will double-check anyway...

	if (FAILED(sc))
	  {
	  wsprintf(buf, "Could not open storage file %s.", (LPSTR)szFile);
	  OleDbgOutScode("Unable to open storage file, ret = ", sc);
	  MessageBox(hWnd, buf, szAppName, MB_ICONEXCLAMATION);
	  }
	else
	  {
	  ClearOut(); // Clear screen
	  iID = LineOut(TYPE_STORAGE, -1, "Storage file %s", (LPSTR)szFile);

	  // Call ViewStorage, which will recursively view our storage file
	  ViewStorage(iID, lpRootStg);
	  OleStdVerifyRelease((LPUNKNOWN)lpRootStg, NULL);
	  }

	SetCursor(hc);
	}

  return (TRUE);

}



/***************************************************************************
**
** ViewStorage()
**
** This function dumps the specified IStorage instance.  It recursively calls
** itself on any contained storages.
**
****************************************************************************/

void ViewStorage(int inID, LPSTORAGE lpStorage)
{
  LPENUMSTATSTG lpEnum;
  STATSTG ss;
  ULONG ulCount;
  SCODE sc;
  LPSTORAGE lpSubStg;
  LPSTREAM lpStream;
  int subID;
  int type;
  char *szType;
  int iReturn;
  HRESULT hr;

  // Assume we have a valid IStorage instance in lpStorage,
  // so get the enumerator

  hr = lpStorage->lpVtbl->EnumElements(lpStorage, 0, NULL, 0, &lpEnum);
  if (hr != NOERROR)
	{
	OleDbgOutHResult("Unable to get storage enumerator, returned", hr);
	MessageBox(ghMainWnd, "Could not get storage enumerator, bailing out.", szAppName, MB_ICONEXCLAMATION);
	return;
	}

  // Continue enumeration until IEnumStatStg->Next returns non-S_OK
  while (1)
	{
	// Enumerate one element at a time
	sc = GetScode(lpEnum->lpVtbl->Next(lpEnum, 1, &ss, &ulCount));
	if (sc != S_OK)
	  {
	  if (sc != S_FALSE) OleDbgOutScode("IEnumStatStg->Next returned", sc);
	  goto ErrorReleaseExit;
	  }

	// Select the human-readable type of object to display in listbox
	switch(ss.type)
	  {
	  case STGTY_STREAM:
		szType = "Stream";
		type = TYPE_DOCUMENT;
		break;
	  case STGTY_STORAGE:
		szType = "Storage";
		type = TYPE_FOLDER;
		break;
	  case STGTY_LOCKBYTES:
		szType = "LockBytes";
		type = TYPE_FOLDER;
		break;
	  case STGTY_PROPERTY:
		szType = "Property";
		type = TYPE_FOLDER;
		break;
	  default:
		szType = "**Unknown**";
		type = TYPE_FOLDER;
		break;
	  }

	subID = LineOut(type, inID, "'%s', Type: %s, Size: %lu",
			  (LPSTR)ss.pwcsName, (LPSTR)szType,
			  (ULONG)ss.cbSize.LowPart);

	// Check if we had an error putting string into listbox.  If so, error out.
	if (subID == -1)
	  goto ErrorExit;              // we still need to release enumerator

	// If we're a stream, output the data in HEX-format.  The function
	// DumpStreamContents does this for us.
	if (ss.type == STGTY_STREAM)
	  {
	  sc = GetScode(lpStorage->lpVtbl->OpenStream(lpStorage,
				  (LPSTR)ss.pwcsName,
				  NULL,
				  STGM_READ | STGM_SHARE_EXCLUSIVE,
				  0,
				  &lpStream));
	  if (!FAILED(sc))
		{
		iReturn = DumpStreamContents(subID, lpStream, ss.cbSize.LowPart);
		OleStdVerifyRelease((LPUNKNOWN)lpStream, NULL);
		if (iReturn == -1)
		  goto ErrorExit;
		}
	  // Else we failed, just continue
	  }

	// If we're a storage, recursively view by calling ViewStorage again.
	if (ss.type == STGTY_STORAGE)
	  {
	  sc = GetScode(lpStorage->lpVtbl->OpenStorage(lpStorage,
				  (LPSTR)ss.pwcsName,
				  NULL,
				  STGM_READ | STGM_SHARE_EXCLUSIVE,
				  NULL, 0, &lpSubStg));
	  if (!FAILED(sc))
		{
		ViewStorage(subID, lpSubStg);
		OleStdVerifyRelease((LPUNKNOWN)lpSubStg, NULL);
		}
		// Else we failed, just continue
	  } // END type == STGTY_STORATE

	// Free string (docs say it must be caller-freed, and that's us).
	OleStdFreeString(ss.pwcsName, NULL);

	} // END enumeration
	OleStdVerifyRelease((LPUNKNOWN)lpEnum, NULL);
	return;

ErrorExit:
	OleStdFreeString(ss.pwcsName, NULL);
ErrorReleaseExit:
	OleStdVerifyRelease((LPUNKNOWN)lpEnum, NULL);
}

/***************************************************************************
**
** ClearOut()
**
** Clears our listbox of all output.  To clear our owner-draw listbox, we
** just send it a LB_RESETCONTENT.
**
****************************************************************************/

void ClearOut()
{
   SendMessage (ghFoldList, LB_RESETCONTENT, 0, 0L);
}


/****************************************************************************
**
** LineOut()
**
** Sends a line of text to the listbox, using the nID parameter to determine
** under which listbox entry the new line should be located (nID specifies
** the desired *parent* of the line to add).
**
** Returns: New ID for the current line.  To add another line as a child of
** the current line, just pass in the retured ID.  Returns -1 for error.
**
****************************************************************************/

int LineOut(int type, int nID, const char* pszFormat, ...)
{
  int nBuf;
  static FL_ITEMINFOSTRUCT gItemInfo;  // Saves stack space being global (in recursive routine)
  int createID;
  HBITMAP hbmtouse;

  va_list args;
  va_start(args, pszFormat);
  nBuf = wvsprintf(buf, pszFormat, args);

  switch (type) {
	case TYPE_BULLET:
	  hbmtouse = ghbmpBullets;
	  break;
	case TYPE_FOLDER:
	  hbmtouse = ghbmpFolders;
	  break;
	case TYPE_BLANK:
	  hbmtouse = ghbmpBlanks;
	  break;
	case TYPE_DOCUMENT:
	  hbmtouse = ghbmpDocuments;
	  break;
	default:
	  hbmtouse = ghbmpWindows;
	  break;
	}

  if(nID == -1)
	{
	// This is a top-level item, not a child

   gItemInfo.bIsFolder     = TRUE;
   gItemInfo.bIsOpen       = FALSE;
   gItemInfo.bIsVisible    = TRUE;
   gItemInfo.nMsgClick     = FLNoMessage;
   gItemInfo.nMsgDblClick  = FLNoMessage;
   gItemInfo.bFLOwnsBitmap = FALSE;
   gItemInfo.hBitmaps      = hbmtouse;
   strcpy(gItemInfo.szText, buf);

   createID = (int) SendMessage (ghFoldList, WM_FLADDITEM, 0,
							 (LPARAM) (LPFL_ITEMINFOSTRUCT) &gItemInfo);

   if (createID == LB_ERR || createID == LB_ERRSPACE || createID == -1)
	 {
	 MemError();
	 return -1;
	 }

   return createID;
   }
   // else this is a child.  Make it a child of nID.
   else
   {
   gItemInfo.bIsVisible    = FALSE;
   gItemInfo.bIsFolder     = TRUE;
   gItemInfo.bIsOpen       = FALSE;
   gItemInfo.nMsgClick     = FLNoMessage;
   gItemInfo.nMsgDblClick  = FLNoMessage;
   gItemInfo.bFLOwnsBitmap = FALSE;
   gItemInfo.hBitmaps      = hbmtouse;
   strcpy(gItemInfo.szText, buf);

   createID = (int)SendMessage (ghFoldList, WM_FLADDITEMCHILD, nID,
							(LPARAM) (LPFL_ITEMINFOSTRUCT) &gItemInfo);
   if (createID == LB_ERR || createID == LB_ERRSPACE || createID == -1)
	 {
	 MemError();
	 return -1;
	 }

   return createID;
   }

}

/***************************************************************************
**
** DumpStreamContents()
**
** Dumps specified IStream contents using Hex-like notation.  Assume lpStream
** is an initialized IStream, and we do not have to release it.
**
** Returns: 0 if OK, -1 if failed.
**
****************************************************************************/

DumpStreamContents(int ID, LPSTREAM lpStream, ULONG ulSize)
{
  LPBYTE lpData;
  ULONG ulCount;
  ULONG ulBytesLeft;
  ULONG ulOffset;
  int iBytesInLine;
  char szTmpBuf[85];
  int iReturnCode = 0;
  HRESULT hr;

  // If size of stream is 0, there's nothing to dump
  if (ulSize == 0)
	return 0;

  if ((lpData = (LPBYTE)GlobalAllocPtr(GHND, (DWORD)ulSize)) == NULL)
	return -1;

  hr = lpStream->lpVtbl->Read(lpStream, lpData, ulSize, &ulCount);
  if (hr != NOERROR)
	{
	OleDbgOutHResult("Unable to read from stream, ret = ", hr);
	MessageBox(ghMainWnd, "Unable to read from stream.", szAppName, MB_ICONEXCLAMATION);
	return -1;
	}

  // Now dump data in hex-format
  ulBytesLeft = ulCount;

  ulOffset = 0;
  while (ulBytesLeft > 0)
	{
	memset(szTmpBuf, ' ', 85);
	iBytesInLine = 0;

	// Print out offset information
	wsprintf(&szTmpBuf[1], "%05x ", ulOffset);

	while (ulBytesLeft > 0 && (iBytesInLine < 16))
	  {
	  // print out data
	  wsprintf(&szTmpBuf[iBytesInLine * 3 + 7], "%02x ", lpData[0]);

	  // print out character representation, or a '.' if we can't display
	  wsprintf(&szTmpBuf[17 * 3 + iBytesInLine + 7], "%c",
						(isprint(lpData[0]) ? (lpData[0]) : '.'));
	  lpData++;
	  ulBytesLeft--;
	  iBytesInLine++;
	  ulOffset++;
	  }

	szTmpBuf[iBytesInLine * 3 + 7] = ' ';
	szTmpBuf[17 * 3 + iBytesInLine + 7] = ' ';
	szTmpBuf[74] = '\0';

	if ((iReturnCode = LineOut(TYPE_BULLET, ID, "%s", (LPSTR)szTmpBuf)) == -1)
	  break;
	}

  GlobalFreePtr(lpData);
  return iReturnCode;
}

/***************************************************************************
**
** SavePositionInfo / GetPositionInfo
**
** Saves and restores window size info from INI file.
**
****************************************************************************/

void SavePositionInfo()
{
	RECT rc;

	GetWindowRect(ghMainWnd, &rc);
	giWindowWidth = rc.right - rc.left;
	giWindowHeight = rc.bottom - rc.top;

	wsprintf(buf, "%d", giWindowHeight);
	WritePrivateProfileString("Window Size", "Height", buf, szIni);
	wsprintf(buf, "%d", giWindowWidth);
	WritePrivateProfileString("Window Size", "Width", buf, szIni);
}

void GetPositionInfo()
{
	giWindowHeight = GetPrivateProfileInt("Window Size", "Height", 400, szIni);
	giWindowWidth = GetPrivateProfileInt("Window Size", "Width", 500, szIni);
}

/***************************************************************************
**
** MemError()
**
** Called when we get a memory allocation error.
**
****************************************************************************/

MemError()
{
	MessageBox(ghMainWnd, "Memory Allocation Error -- docfile is "
					 "too large to view with this viewer", szAppName, MB_ICONSTOP);
	return -1;
}
