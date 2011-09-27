/*************************************************************************
**
**    OLE 2 DFVIEW Sample Code
**
**    dfview.h
**
**    (c) Copyright Microsoft Corp. 1990 - 1992 All Rights Reserved
**
*************************************************************************/

#define MAXFILENAMELEN 255

// Types of bitmaps for our owner-draw listbox.
#define TYPE_BULLET 1
#define TYPE_STORAGE 3
#define TYPE_FOLDER 2
#define TYPE_BLANK 4
#define TYPE_DOCUMENT 5


// These #defines are used to determine the size of our bitmaps in
// our owner-draw listbox
#define BMPSIZEX 16
#define BMPSIZEY 17

int _pascal WinMain( HANDLE, HANDLE, LPSTR, int );
long _far _pascal __export MainWndProc( HWND, UINT, WPARAM, LPARAM);
BOOL _far _pascal __export About( HWND, unsigned, WORD, LONG );
BOOL _far _pascal __export HelpProc( HWND, unsigned, WORD, LONG );
static BOOL InitApplication( HANDLE );
static BOOL InitInstance( HANDLE, int );
static int _pascal MakeDocFile( HWND hWnd );
static int _pascal ReadDocFile( HWND hWnd );
DumpStreamContents(int, LPSTREAM, ULONG);
MemError(void);
void ViewStorage(int, LPSTORAGE);
void SavePositionInfo(void);
void GetPositionInfo(void);
PlopBitmap(HWND, int, HDC, HBITMAP);
int LineOut(int type, int nID, const char *pszFormat, ...);
void ClearOut();
long DFListBoxCreate(HWND hWndParent, int, int);
void DFListBoxCleanup (void);
void DFListBoxSize (UINT cxWindow, UINT cyWindow);
